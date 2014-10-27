/********************
 * 当たり判定クラス *
 ********************/

/* 多重Include防止 */
#ifndef RACE_COLLISION_H_
#define RACE_COLLISION_H_

/***********
 * Include *
 ***********/
#include "common.h"
#include "mesh.h"

/**************************
 * 当たり判定を行うクラス *
 **************************/
class Collision
{
private:

	/**************
	 * クラス定義 *
	 **************/
	MeshTable*  my_mesh_;  // メッシュ情報

	/********
	 * 変数 *
	 ********/
	LPD3DXMESH  sphire_;    // スフィア用メッシュ
	LPD3DXMESH  box_;       // ボックス用メッシュ

	D3DXVECTOR3 position_;  // 座標保存用

public:
	Collision();   // コンストラクタ
	~Collision();  // デストラクタ

	VOID Init(VOID);
	VOID Destroy(VOID);  // 破棄処理

	/******************
	 * レイ判定用関数 *
	 ******************/
	BOOL RayIntersect(CONST MeshTable* mesh_, CONST D3DXMATRIX meshWorldMat, CONST D3DXVECTOR3 RayStartPos, CONST D3DXVECTOR3 RayVec, D3DXVECTOR3* position_);
	VOID FindVertices(LPD3DXMESH mesh_, DWORD Index, D3DXVECTOR3* vertices_);

	/**********************
	 * スフィア判定用関数 *
	 **********************/
	FLOAT SphereCreate(CONST MeshTable* mesh);                                               // スフィア作成関数
	VOID SphereMesh(CONST MeshTable* pMesh, CONST D3DXVECTOR3 position, CONST FLOAT radius); // スフィア表示関数
	BOOL SphereHitCheck(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, FLOAT radius1, FLOAT radius2);   // スフィア判定関数

	/**********************
	 * ボックス判定用関数 *
	 **********************/
	VOID BoxCreate(MeshTable* mesh, D3DXVECTOR3* min, D3DXVECTOR3* max);             // ボックス作成関数    
	VOID BoxMesh(CONST MeshTable* pMesh, CONST D3DXVECTOR3 position, D3DXVECTOR3 size);                       // ボックス表示関数
	BOOL BoxHitCheck(D3DXVECTOR3 min_pos1, D3DXVECTOR3 max_pos1, D3DXVECTOR3 min_pos2, D3DXVECTOR3 max_pos2); // ボックス判定関数

	/**************************
	 * 箱と球の当たり判定確認 *
	 **************************/
	BOOL MultiHitCheck(D3DXVECTOR3 min_pos, D3DXVECTOR3 max_pos, D3DXVECTOR3 pos, FLOAT radius);

	/* プレイヤー座標受け取り関数 */
	VOID SetPlayerPosition(D3DXVECTOR3 pos) { position_ = pos; }
};
#endif