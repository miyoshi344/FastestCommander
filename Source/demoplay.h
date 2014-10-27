/********************
 * デモプレイクラス *
 ********************/

/* 多重Include防止 */
#ifndef RACE_DEMOPLAY_H_
#define RACE_DEMOPLAY_H_

/***********
 * Include *
 ***********/
#include "game_base.h"

/********
 * 定数 *
 ********/
CONST INT kStartMap = GRASS;



/********************************************
 * デモプレイを表示するするためのクラスです *
 ********************************************/
class DemoPlay : public GameBase
{
private:
	BOOL MapInit(VOID);
	BOOL PlayerInit(VOID);
	VOID GameSet(INT player);
	VOID GameUpDate(INT player);
	VOID BackGameDraw(VOID);

public:
	DemoPlay(VOID);   // コンストラクタ
	~DemoPlay(VOID);  // デストラクタ

	BOOL Init(VOID);     // 初期設定
	VOID ReSet(VOID);    // 初期化処理
	BOOL UpDate(VOID);   // 更新処理
	VOID Draw(VOID);     // 描画処理
	VOID SpriteDraw(VOID);
	VOID Destroy(VOID);  // 破棄処理
};

#endif