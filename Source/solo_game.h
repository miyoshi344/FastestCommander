/****************
* ゲームクラス *
****************/

/* 多重Include防止 */
#ifndef RACE_SOLO_GAME_H_
#define RACE_SOLO_GAME_H_

/***********
* Include *
***********/
#include "game_base.h"

/******************************************
 * ゲームのメイン部分を管理するクラスです *
 * GameBaseクラスを継承して作成します     *
 ******************************************/
class SoloGame : public GameBase
{
private:

	VOID SpriteDraw(VOID);  // スプライト描画処理
public:

	SoloGame(VOID);    // コンストラクタ
	~SoloGame(VOID);   // デストラクタ

	INT  UpDate(VOID); // 更新処理
	VOID Draw(VOID);   // 描画処理
};

#endif