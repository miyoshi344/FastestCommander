/**************
 * 弾のクラス *
 **************/

/* 多重Include防止 */
#ifndef RACE_BULLET_H_
#define RACE_BULLET_H_

/***********
 * Include *
 ***********/
#include "objectbase.h"

/********
 * 定数 *
 ********/
CONST FLOAT kBurretSpeed   = 0.4F;    // 弾の初速値
CONST FLOAT kBurretGravity = 0.007F;  // 重力値
CONST FLOAT kBurretHeight  = 0.7F;    // 弾の出る高さ
CONST FLOAT kBurretJump    = 0.1F;    // 弾の飛び方

/****************************
 * 弾の管理をするクラスです *
 ****************************/
class Bullet : public ObjectBase
{
private:
	/********
	 * 変数 *
	 ********/
	LPCSTR bullet_name_;  // キャラクターごとの弾の設定

	FLOAT direction_;     // 向き
	FLOAT move_;          // 加速度
	FLOAT speed_;         // 初速値
	FLOAT jump_;          // 高さ値

	/************
	 * 内部関数 *
	 ************/
	BOOL BulletMeshInit(VOID);  // 弾のメッシュ取得処理
	VOID BulletMove(VOID);      // 弾の移動処理
	VOID BulletDraw(VOID);      // 弾の描画処理

public:
	Bullet(VOID);        // コンストラクタ
	~Bullet(VOID);       // デストラクタ

	BOOL Init(VOID);     // 初期設定
	VOID ReSet(VOID);    // 初期化
	VOID UpDate(VOID);   // 更新処理
	VOID Draw(VOID);     // 描画処理
	VOID Destroy(VOID);  // 破棄処理

	/****************
	 * 値を返す関数 *
	 ****************/
	D3DXVECTOR3 ReturnOriginPosition(VOID) { return origin_pos_; }  // 更新前の弾の座標
	D3DXVECTOR3 ReturnPosition(VOID) { return position_; }          // 更新後の弾の座標

	/******************
	 * 値をもらう関数 *
	 ******************/
	VOID BulletColor(LPCSTR bullet_name) { bullet_name_ = bullet_name; }  // 弾の情報を取得する
	VOID BulletSet(D3DXVECTOR3 pos, FLOAT direction, FLOAT move);         // 弾の位置を取得する
};

#endif