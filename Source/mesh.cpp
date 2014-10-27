/***********
 * Include *
 ***********/
#include "mesh.h"

/**********************
 * 静的メンバ変数実装 *
 **********************/
std::map<std::string, MeshTable*> Mesh::list_;

/******************
 * コンストラクタ *
 ******************/
Mesh::Mesh(VOID)
{
}

/****************
 * デストラクタ *
 ****************/
Mesh::~Mesh(VOID)
{
}

/*******************************************
 * 関数名 : Destroy関数                    *
 * 内  容 : メッシュ管理クラスを破棄します *
 * 引  数 : なし                           *
 * 戻り値 : なし                           *
 *******************************************/
VOID Mesh::Destroy(VOID)
{
	/* リストは空か */
	if (!list_.empty())
	{
		/* テクスチャを検索 */
		std::map<std::string, MeshTable*>::iterator it = list_.begin();
		std::map<std::string, MeshTable*>::iterator end = list_.end();

		/*** テクスチャが残っていたら解放 ***/
		while (it != end)
		{
			/* メモリ解放 */
			SafeDelete(it->second);
			++it;
		}
	}

	list_.clear();
}

/***********************************************************
 * 関数名 : Load関数                                       * 
 * 内  容 : メッシュをロードしてポインタを返します。       *
 * 引  数 : CONST string MeshName　                        *
 * 戻り値 : メッシュインターフェイスのポインタを返します。 *
 *          ロードに失敗した場合はNULLを返します。         *
 ***********************************************************/
MeshTable* Mesh::Load(CONST std::string MeshName)
{
	MeshTable* p_mesh = NULL;

	/* メッシュが既に読み込まれているか検索 */
	std::map<std::string, MeshTable*>::iterator it;
	it = list_.find(MeshName);

	/* 末尾なら新規に読み込む */
	if (it == list_.end())
	{
		do
		{
			/* メッシュテーブルメモリを確保 */
			p_mesh = new MeshTable;

			/* NULLチェック */
			if (!p_mesh)
			{
				/* エラー */
				ErrorDialog("メッシュテーブルメモリ確保に失敗");
				break;
			}

			/* メッシュを読み込む */
			LPD3DXBUFFER p_matbuff = NULL;

			if (FAILED(D3DXLoadMeshFromX(
				MeshName.c_str(),               // ファイル名
				D3DXMESH_MANAGED,               // メッシュ作成オプションフラグ
				D3DDEV,                         // Direct3DDevice
				NULL,                           // メッシュ接続情報格納先ポインタのポインタ
				&p_matbuff,                     // マテリアル情報格納先ポインタ
				NULL,                           // 属性グループバッファへのポインタ
				(DWORD*)&p_mesh->num_materials,	// マテリアル情報数
				&p_mesh->mesh)))                // 作成されたメッシュを受け取るポインタのポインタ
			{
				/* エラー */
				CHAR name[MAX_PATH] = "";
				sprintf_s(name, "メッシュの読み込みに失敗（%s）", MeshName.c_str());
				ErrorDialog(name);
				SafeDelete(p_mesh);
				break;
			}

			/* メモリの確保 */
			/* テクスチャインターフェイス */
			if ((p_mesh->texture = new LPDIRECT3DTEXTURE9[p_mesh->num_materials]) == NULL)
			{
				/* エラー */
				ErrorDialog("テクスチャインターフェイスのメモリ確保に失敗");
				break;
			}

			/* マテリアルインターフェイス */
			if ((p_mesh->material = new D3DMATERIAL9[p_mesh->num_materials]) == NULL)
			{
				/* エラー */
				ErrorDialog("マテリアルインターフェイスのメモリ確保に失敗");
				break;
			}

			/* マテリアルバッファのポインタを取得する */
			D3DXMATERIAL*	p_d3dmtrl;
			p_d3dmtrl = static_cast<D3DXMATERIAL*>(p_matbuff->GetBufferPointer());

			/* マテリアルとテクスチャの設定 */
			for (INT i = 0; i < static_cast<INT>(p_mesh->num_materials); i++)
			{
				/* マテリアルをコピーする */
				p_mesh->material[i] = p_d3dmtrl[i].MatD3D;
				p_mesh->material[i].Ambient = p_mesh->material[i].Diffuse;

				/* テクスチャを読み込む */
				p_mesh->texture[i] = NULL;

				/* テクスチャが存在していていたら読み込む */
				if (p_d3dmtrl[i].pTextureFilename)
				{
					if ((p_mesh->texture[i] = Texture::LoadTexture(p_d3dmtrl[i].pTextureFilename)) == NULL)
					{
						/* 失敗 */
						ErrorDialog("テクスチャの読み込みに失敗(Meshクラス)");
						break;
					}
				}
			}

			/* 読み込み終わったので汎用バッファを解放 */
			SafeRelease(p_matbuff);

			/* リストに追加 */
			list_.insert(std::pair<std::string, MeshTable*>(MeshName, p_mesh));

		} while (0);
	}
	else
	{
		/*** 既に読み込まれているならポインタを返す ***/
		p_mesh = it->second;
		++p_mesh->reference_count;
	}

	return p_mesh;
}

/****************************************
 * 関数名 : Draw関数                    *
 * 内  容 : メッシュを描画します        *
 * 引  数 : CONST MeshTable* pMeshTable *
 * 戻り値 : なし                        *
 ****************************************/
VOID Mesh::Draw(CONST D3DMATRIX world, CONST MeshTable* pMeshTable)
{
	/* 描画する物をセットする */
	D3DDEV->SetTransform(D3DTS_WORLD, &world);

	/* メッシュ描画 */
	for (INT i = 0; i < pMeshTable->num_materials; i++)
	{
		D3DDEV->SetTexture(0, pMeshTable->texture[i]);  // テクスチャの設定
		D3DDEV->SetMaterial(&pMeshTable->material[i]);	// マテリアルの設定
		pMeshTable->mesh->DrawSubset(i);                  // メッシュの描画
	}
}

/******************************************************************
 * 関数名 : Release関数                                           *
 * 内  容 : メッシュを解放します。参照数が0になったら破棄します。 *
 * 引  数 : CONST MeshTable* pMeshTable                           *
 * 戻り値 : なし                                                  *
 ******************************************************************/
VOID Mesh::Release(CONST MeshTable* pMeshTable)
{
	if (!list_.empty())
	{
		/* テクスチャを検索 */
		std::map<std::string, MeshTable*>::iterator it = list_.begin();
		std::map<std::string, MeshTable*>::iterator end = list_.end();

		while (it != end)
		{
			if (it->second == pMeshTable)
			{
				/* 参照回数を減らす */
				--it->second->reference_count;

				/* 参照が0になったら削除 */
				if (it->second->reference_count <= 0)
				{
					SafeDelete(it->second);
					list_.erase(it);
				}
				break;
			}
			++it;
		}
	}
}