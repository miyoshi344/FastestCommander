/**************
 * 基礎クラス *
 **************/

/* 多重Include防止 */
#ifndef RACE_OBJECTBASE_H_
#define RACE_OBJECTBASE_H_

/***********
 * Include *
 ***********/
#include "common.h"
#include "mesh.h"
#include "sprite.h"
#include "texture.h"
#include "xinput.h"
#include "directsound.h"
#include "collision.h"

/********
 * 定数 *
 ********/

/********************************************
 * すべてのクラスの基礎クラスです           *
 * 他のクラスはこれを継承して作成します     *
 ********************************************/
class ObjectBase
{
protected:

	/**************
	 * クラス宣言 *
	 **************/
	Collision   collision_;  // 当たり判定クラス
	Mesh        mesh_;       // メッシュクラス
	MeshTable*  my_mesh_;    // メッシュ情報

	/********
	 * 変数 *
	 ********/
	D3DXVECTOR3  origin_pos_;  // 更新前の座標
	D3DXVECTOR3  position_;    // 座標
	D3DXMATRIX   world_;       // ワールド
	FLOAT radius_;

public:

	/* 仮想関数 */
	virtual BOOL Init(VOID)    = 0;  // 初期化
	virtual VOID UpDate(VOID)  = 0;  // 更新処理
	virtual VOID Draw(VOID)    = 0;  // 描画処理
	virtual VOID Destroy(VOID) = 0;  // 破棄

	/* 情報を返す関数 */
	D3DXVECTOR3 ReturnOriginPos(VOID) { return origin_pos_; }  // 更新前座標
	D3DXVECTOR3 ReturnPosition(VOID){ return position_; }      // 更新後座標
	D3DXMATRIX  ReturnMatrix(VOID)  { return world_;}          // ワールド座標
	FLOAT       ReturnRadius(VOID)  { return radius_; }
	MeshTable*  ReturnMesh(VOID)    { return my_mesh_; }       // メッシュ情報

	/* 情報を拾う関数 */
	VOID SetPosition(D3DXVECTOR3 pos) { position_ = pos; } 
	VOID SetMatrix(D3DXMATRIX matrix) { world_ = matrix; }
};

#endif
