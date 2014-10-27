/***********
 * Include *
 ***********/
#include "solo_game.h"

/******************
 * コンストラクタ *
 ******************/
SoloGame::SoloGame(VOID)
{
}

/****************
 * デストラクタ *
 ****************/
SoloGame::~SoloGame(VOID)
{
}

/*******************************
 * 関数名 : UpDate関数         *
 * 内  容 : 更新処理を行います *
 * 引  数 : なし               *
 * 戻り値 : なし               *
 *******************************/
INT SoloGame::UpDate(VOID)
{	
	/* プレイヤーの更新 */
	for (INT i = 0; i < kAllPlayer; i++)
	{
		/* 情報の更新 */
		GameSet(i);

		/* プレイヤーの更新 */
		GameUpDate(i);
	}

	return (field_map_ == MAX_FIELD) ? TITLE : SOLO_GAME;
}

/*******************************
 * 関数名 : Draw関数           *
 * 内  容 : 描画処理を行います *
 * 引  数 : なし               *
 * 戻り値 : なし               *
 *******************************/
VOID SoloGame::Draw(VOID)
{
	//カメラの座標を変更（ビュー行列）
	camera_[0].UpDate();
	player_[0]->Draw();

	/* 画面の更新 */
	field_[field_map_].SetPlayerNo(0);
	field_[field_map_].Draw();
}

/*****************************************
 * 関数名 : SpriteDraw関数               *
 * 内  容 : スプライト描画処理を行います *
 * 引  数 : なし                         *
 * 戻り値 : なし                         *
 *****************************************/
VOID SoloGame::SpriteDraw(VOID)
{
}