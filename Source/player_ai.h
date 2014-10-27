/**********************
 * プレイヤーAIクラス *
 **********************/

/* 多重Include防止 */
#ifndef RACE_PLAYERAI_H_
#define RACE_PLAYERAI_H_

/***********
 * Include *
 ***********/
#include "character_base.h"
#include "bullet.h"

/**************************************
 * プレイヤーのAIを管理するクラスです *
 * CharacterBaseを継承して作成します  *
 **************************************/
class PlayerAI : public CharacterBase
{
private:
	
	/********
	 * 変数 *
	 ********/
	LPCSTR ai_name_;

	/************
	 * 内部関数 *
	 ************/
	BOOL AiInit(VOID);           // AIテキストの読み込み処理
	VOID Jump(VOID);             // ジャンプ処理
	VOID Direction(VOID);        // 移動処理
	VOID TurretDirection(VOID);  // 砲塔視点移動
	VOID Accel(VOID);            // アクセル処理

public:
	PlayerAI(VOID);      // コンストラクタ
	~PlayerAI(VOID);     // デストラクタ

	BOOL Init(VOID);     // 初期処理
	VOID ReSet(VOID);    // 状態初期化
	VOID UpDate(VOID);   // 更新処理
	VOID Draw(VOID);     // 描画処理
	VOID Destroy(VOID);  // 破棄処理
};

#endif