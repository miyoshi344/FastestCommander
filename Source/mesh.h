/******************
 * メッシュクラス *
 ******************/

/* 多重Include防止 */
#ifndef RACE_MESH_H_
#define RACE_MESH_H_

/************
 *  Include *
 ************/
#include "common.h"
#include "direct3d.h"
#include "texture.h"

/**********
 * 構造体 *
 **********/
struct MeshTable
{
	LPDIRECT3DTEXTURE9* texture;            // テクスチャインターフェイス
	LPD3DXMESH          mesh;               // メッシュインターフェイス
	D3DMATERIAL9*       material;           // マテリアル構造体
	INT                 num_materials;      // マテリアル数 
	INT                 reference_count;    // 参照回数

	/* コンストラクタ */
	MeshTable(VOID)
	{
		texture = NULL;
		mesh = NULL;
		material = NULL;
		num_materials = 0;
		reference_count = 0;
	}

	/* デストラクタ */
	~MeshTable()
	{
		/*** テクスチャ解放 ***/
		for (INT i = 0; i < num_materials; i++)
		{
			if (texture[i]) Texture::Release(texture[i]);
		}

		/* メモリ解放 */
		SafeDeleteArray(texture);   // テクスチャインターフェイス
		SafeDeleteArray(material);  // マテリアル
		SafeRelease(mesh);			// メッシュ
	}
};

/**********
 * クラス *
 **********/
class Mesh
{
private:
	static std::map<std::string, MeshTable*> list_; // メッシュリスト

public:
	Mesh(VOID);		            // コンストラクタ
	~Mesh(VOID);                // デストラクタ
	static VOID Destroy(VOID);  // テクスチャ管理クラスを破棄します

	static MeshTable* Load(CONST std::string TextureName);             // メッシュの読み込み
	static VOID Draw(CONST D3DMATRIX world, CONST MeshTable* Texture); // 描画
	static VOID Release(CONST MeshTable* Texture);                     // 解放
};

#endif