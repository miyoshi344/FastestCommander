/********************
 * プレイヤークラス *
 ********************/

/* 多重Include防止 */
#ifndef RACE_PLAYER_H_
#define RACE_PLAYER_H_

/***********
 * Include *
 ***********/
#include "character_base.h"
#include "bullet.h"

/************************************
 * プレイヤーの管理を行うクラスです *
 ************************************/
class Player : public CharacterBase
{
private:

	/************
	 * 内部関数 *
	 ************/
	VOID Jump(VOID);             // ジャンプ処理
	VOID Direction(VOID);        // 移動処理
	VOID TurretDirection(VOID);  // 砲塔の視点移動
	VOID Accel(VOID);            // アクセル処理

public:
	Player(VOID);        // コンストラクタ
	~Player(VOID);       // デストラクタ

	BOOL Init(VOID);     // 初期処理
	VOID ReSet(VOID);    // 状態初期化
	VOID UpDate(VOID);   // 更新処理
	VOID Draw(VOID);     // 描画処理
	VOID Destroy(VOID);  // 破棄処理

	VOID ReplaySave(VOID);
};

#endif