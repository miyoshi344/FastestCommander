/***********
 * Include *
 ***********/
#include "collision.h"

/******************
 * コンストラクタ *
 ******************/
Collision::Collision()
{
}

/****************
 * デストラクタ *
 ****************/
Collision::~Collision()
{
}

/*****************************************
 * 関数名 : Destory関数                  *
 * 内  容 : 取得したメッシュを破棄します *
 * 引  数 : なし                         *
 * 戻り値 : なし                         *
 *****************************************/
VOID Collision::Destroy(VOID)
{
}

/***********************************************************
 * 関数名 : RayIntersect関数                               *
 * 内  容 : レイチェック用関数                             *
 * 引  数 : pMesh         レイチェックしたいメッシュ構造体 *
 *          MeshWorldMat  メッシュのワールド行列           *
 *          RayStartPos   レイの開始座標                   *
 *          RayVec        レイの方向                       *
 *          pPosition     ヒットした平面の座標             *
 * 戻り値 : ヒットしたらTRUEを返す                         *
 ***********************************************************/
BOOL Collision::RayIntersect(CONST MeshTable* pMesh, CONST D3DXMATRIX MeshWorldMat,
	CONST D3DXVECTOR3 RayStartPos, CONST D3DXVECTOR3 RayVec, D3DXVECTOR3* pPosition)
{
	LPD3DXMESH mesh_ = pMesh->mesh;

	/* メッシュ情報が無ければ終了 */
	if (mesh_ == NULL) return FALSE;

	/* メッシュのワールド行列の逆行列を生成 */
	D3DXMATRIX   mesh_mat_inv;
	D3DXMatrixInverse(&mesh_mat_inv, NULL, &MeshWorldMat);

	/* レイの開始位置と方向に逆行列を適用 */
	D3DXVECTOR3 start = RayStartPos,
		        end   = RayVec + RayStartPos,
	         	direction;

	D3DXVec3TransformCoord(&start, &start, &mesh_mat_inv);
	D3DXVec3TransformCoord(&end, &end, &mesh_mat_inv);
	direction = end - start;

	/* レイチェック実行 */
	BOOL  ret = FALSE;
	DWORD index;
	FLOAT u, v;
	D3DXIntersect(mesh_, &start, &direction, &ret, &index, &u, &v, NULL, NULL, NULL);
	if (ret)
	{
		/* 頂点座標を検索 */
		D3DXVECTOR3 vertex[3];
		FindVertices(mesh_, index, vertex);

		/* 重心座標から交点座標を算出 */
		*pPosition = vertex[0] + u * (vertex[1] - vertex[0]) + v * (vertex[2] - vertex[0]);
		D3DXVec3TransformCoord(pPosition, pPosition, &MeshWorldMat);
		return TRUE;
	}
	else return FALSE;
}

/****************************************************
 * 関数名 : FindVertices                            *
 * 内  容 : 頂点座標を検索する                      *
 * 引  数 : pMesh  レイチェックしたいメッシュ構造体 *
 *          Index  メッシュの頂点インデックス       *
 *          pVertices  3頂点の配列ポインタ          *
 * 戻り値 : なし                                    *
 ****************************************************/
void Collision::FindVertices(LPD3DXMESH pMesh, DWORD Index, D3DXVECTOR3* pVertices)
{
	/* メッシュ情報が無ければ終了 */
	if (pMesh == NULL)
	{
		return;
	}

	/* インデックスバッファを取得 */
	WORD* p_index_buffer = NULL;
	if (SUCCEEDED(pMesh->LockIndexBuffer(D3DLOCK_READONLY, (VOID**)&p_index_buffer)))
	{
		/* 頂点座標を展開 */
		DWORD stride = pMesh->GetNumBytesPerVertex();
		BYTE* p_byte_vertices = NULL;
		FLOAT* p_float_vertices = NULL;
		LPDIRECT3DVERTEXBUFFER9 vertex_buffer = NULL;
		if (SUCCEEDED(pMesh->GetVertexBuffer(&vertex_buffer)))
		{
			if (SUCCEEDED(vertex_buffer->Lock(0, 0, (VOID**)&p_byte_vertices, 0)))
			{
				/* 第1頂点 */
				p_float_vertices = (FLOAT*)&p_byte_vertices[stride * p_index_buffer[Index * 3]];
				pVertices[0].x = p_float_vertices[0];
				pVertices[0].y = p_float_vertices[1];
				pVertices[0].z = p_float_vertices[2];

				/* 第2頂点 */
				p_float_vertices = (FLOAT*)&p_byte_vertices[stride * p_index_buffer[Index * 3 + 1]];
				pVertices[1].x = p_float_vertices[0];
				pVertices[1].y = p_float_vertices[1];
				pVertices[1].z = p_float_vertices[2];

				/* 第3頂点 */
				p_float_vertices = (FLOAT*)&p_byte_vertices[stride * p_index_buffer[Index * 3 + 2]];
				pVertices[2].x = p_float_vertices[0];
				pVertices[2].y = p_float_vertices[1];
				pVertices[2].z = p_float_vertices[2];

				/* 頂点座標を閉じる */
				pMesh->UnlockIndexBuffer();
				pMesh->UnlockVertexBuffer();
			}
		}
	}
}

/*****************************************************************
 * 関数名 : SphereCreate関数                                     *
 * 内  容 : 引数のメッシュ用のバウンディングスフィアを作成します *
 * 引  数 : MeshTable* pMesh　判定するメッシュ                   *
 * 戻り値 : Float      radius スフィアの半径                     *
 *****************************************************************/
FLOAT Collision::SphereCreate(CONST MeshTable* pMesh)
{
	LPD3DXMESH mesh = pMesh->mesh;

	/* メッシュの頂点をロックして先頭ポインタを取得 */
	VOID* vertex_buffer = NULL;
	mesh->LockVertexBuffer(D3DLOCK_READONLY, &vertex_buffer);

	/* メッシュから境界球を作成する */
	D3DXVECTOR3 center;
	FLOAT radius = 0.0F;  // 半径保存用

	D3DXComputeBoundingSphere(
		(D3DXVECTOR3*)vertex_buffer,   // 頂点の先頭ポインタ
		mesh->GetNumVertices(),        // 頂点数
		mesh->GetNumBytesPerVertex(),  // 頂点サイズ
		&center,                       // 半径
		&radius);

	/* ロック解除 */
	mesh->UnlockVertexBuffer();

	/* スフィアの半径を返す */
	return radius;
}

/*****************************************************
 * 関数名 : SphereMesh関数                           *
 * 内  容 : 当たり判定用に作成したスフィアを表示する *
 * 引  数 : MeshTable*  pMesh    表示するメッシュ    *
 *          D3DXVECTOR3 position メッシュの座標      *
 *          FLOAT       radius   スフィアの半径      *
 * 戻り値 : なし                                     *
 *****************************************************/
VOID Collision::SphereMesh(CONST MeshTable* pMesh, CONST D3DXVECTOR3 position, CONST FLOAT radius)
{
	D3DXMATRIX world;

	/* 視認できるようスフィアメッシュを作成 */
	D3DXCreateSphere(
		D3DDEV,     // D3Dデバイス
		radius,     // 半径
		32,         // スライス数(横の分割数)
		32,         // スタック数(縦の分割数)
		&sphire_,   // 受取先メッシュインターフェイスポインタ
		NULL);      // 属性バッファ

	/* 境界球を描画 */
	D3DXMatrixIdentity(&world);

	D3DXMatrixTranslation(&world, position.x, position.y, position.z);
	D3DDEV->SetTransform(D3DTS_WORLD, &world);

	D3DDEV->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);  // ワイヤーフレーム化
	D3DDEV->SetTexture(0, NULL);                                // テクスチャ無効
	sphire_->DrawSubset(0);                                     // 描画
	D3DDEV->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);      // 通常描画
}

/***********************************************
 * 関数名 : SphereHitCheck関数                 *
 * 内  容 : スフィアでの当たり判定を確認します *
 * 引  数 : D3DXVECTOR3 pos1 判定する座標1     *
 *          D3DXVECTOR3 pos2 判定する座標2     *
 *          FLOAT radius     判定する半径1     *
 *          FLOAT radius2    判定する半径2     *
 * 戻り値 : 重なっていたらTRUEを返します       *
 ***********************************************/
BOOL Collision::SphereHitCheck(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, FLOAT radius, FLOAT radius2)
{
	/* 2つの物体の中心の距離を求める */
	D3DXVECTOR3 vecLength = pos2 - pos1;
	FLOAT length = D3DXVec3Length(&vecLength);

	/* 重なっていたらTRUEを返す */
	if (length < radius + radius2)
	{
		return TRUE;
	}

	return FALSE;
}

/***************************************************************
 * 関数名 : BoxCreate関数                                      *
 * 内  容 : 引数のメッシュのバウンディングボックスを作成します *
 * 引  数 : MeshTable*   pMesh 判定するメッシュ                *
 * 戻り値 : なし                                               *
 ***************************************************************/
VOID Collision::BoxCreate(MeshTable* pMesh, D3DXVECTOR3* min, D3DXVECTOR3* max)
{
	LPD3DXMESH mesh = pMesh->mesh;

	/* メッシュの頂点をロックして先頭ポインタを取得 */
	VOID* vertex_buffer = NULL;
	mesh->LockVertexBuffer(D3DLOCK_READONLY, &vertex_buffer);

	/* メッシュから境界箱を作成する */
	D3DXComputeBoundingBox(
		(D3DXVECTOR3*)vertex_buffer,   // 頂点の先頭ポインタ
		mesh->GetNumVertices(),        // 頂点数
		mesh->GetNumBytesPerVertex(),  // 頂点サイズ
		min,                           // 最小値
		max);                          // 最大値

	/* ロック解除 */
	mesh->UnlockVertexBuffer();
}

/*********************************************************
 * 関数名 : BoxMesh関数                                  *
 * 内  容 : 当たり判定用に作成したボックスを表示する     *
 * 引  数 : MeshTable*  pMesh    判定するメッシュ        *
 *          D3DXVECTOR3 position 表示する座標            *
 *          D3DXVECTOR3 min      ボックスの最小値        *
 *          D3DXVECTOR3 max      ボックスの最大値        *
 * 戻り値 : なし                                         *
 *********************************************************/
VOID Collision::BoxMesh(CONST MeshTable* pMesh, D3DXVECTOR3 position, D3DXVECTOR3 size)
{
	D3DXMATRIX world;

	/* 視認できるようスフィアメッシュを作成 */
	D3DXCreateBox(
		D3DDEV,     // D3Dデバイス
		size.x,     // ボックスの幅
		size.y,     // ボックスの高さ
		size.z,     // ボックスの深さ
		&box_,      // 受取先メッシュインターフェイスポインタ
		NULL);      // 属性バッファ

	/* 境界球を描画 */
	D3DXMatrixIdentity(&world);

	D3DXMatrixTranslation(&world, position.x, position.y, position.z);
	D3DDEV->SetTransform(D3DTS_WORLD, &world);

	D3DDEV->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);  // ワイヤーフレーム化
	D3DDEV->SetTexture(0, NULL);                                // テクスチャ無効
	box_->DrawSubset(0);                                        // 描画
	D3DDEV->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);      // 通常描画
}

/**********************************************
* 関数名 : BoxHitCheck関数                    *
* 内  容 : ボックスでの当たり判定を確認します *
* 引  数 : D3DXVECTOR3 min_pos1 1個目の最小値 *
*          D3DXVECTOR3 max_pos1 1個目の最大値 *
*          D3DXVECTOR3 min_pos2 2個目の最小値 *
*          D3DXVECTOR3 max_pos2 2個目の最大値 *
* 戻り値 : 重なっていたらTRUEを返します       *
***********************************************/
BOOL Collision::BoxHitCheck(D3DXVECTOR3 min_pos1, D3DXVECTOR3 max_pos1, D3DXVECTOR3 min_pos2, D3DXVECTOR3 max_pos2)
{
	if (min_pos1.z < max_pos2.z && max_pos1.z > min_pos2.z &&
		min_pos1.x < max_pos2.x && max_pos1.x > min_pos2.x &&
		min_pos1.y < max_pos2.y && max_pos2.y > min_pos2.y)
	{
		return TRUE;
	}

	return FALSE;
}

BOOL Collision::MultiHitCheck(D3DXVECTOR3 min_pos, D3DXVECTOR3 max_pos, D3DXVECTOR3 pos, FLOAT radius)
{
	return TRUE;
}