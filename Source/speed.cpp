/***********
* Include *
***********/
#include "speed.h"

/******************
* コンストラクタ *
******************/
Speed::Speed(VOID)
{
	speed_ = 0.0F;
}

/****************
* デストラクタ *
****************/
Speed::~Speed(VOID)
{
	Texture::Release(texture_);
}

/*******************************
 * 関数名 : UpDate関数         *
 * 内  容 : 更新処理を行います *
 * 引  数 : なし               *
 * 戻り値 : なし               *
 *******************************/
VOID Speed::UpDate(VOID)
{
	speed_ *= 100.0F;
}

/**************************************
 * 関数名 : SoloDraw関数              *
 * 内  容 : 1人用の描画処理を行います *
 * 引  数 : なし                      *
 * 戻り値 :                           *
***************************************/
VOID Speed::SoloDraw(VOID)
{
}

/*****************************************
 * 関数名 : TwoDraw関数                  *
 * 内  容 : 2人用の描画処理を行います    *
 * 引  数 : INT player_no プレイヤー番号 *
 * 戻り値 : なし                         *
 *****************************************/
VOID Speed::TwoDraw(INT player_no)
{
	D3DXVECTOR3 kTwoSpeedPosition[2] = {
		{ D3DXVECTOR3(kWindowWidth - kSpeedPos.x, kMultiWindow.y - kSpeedPos.y, 0.0F) },
		{ D3DXVECTOR3(kWindowWidth - kSpeedPos.x, kWindowHeight  - kSpeedPos.y, 0.0F) }
	};

	/* 座標セット */
	D3DXVECTOR3 position = kTwoSpeedPosition[player_no] - kMultiSpeedPos;

	/* 描画セット */
	DrawSet(kTwoSpeedPosition[player_no], MeterRect, player_no);
	CountSet(position, (INT)speed_, MIDDLE, player_no);
}

/*****************************************
 * 関数名 : MultiDraw関数                *
 * 内  容 : 多人数用の描画処理を行います *
 * 引  数 : INT player_no プレイヤー番号 *
 * 戻り値 : なし                         *
 *****************************************/
VOID Speed::MultiDraw(INT player_no)
{
	D3DXVECTOR3 kMultiSpeedPosition[kAllPlayer] = {
		{ D3DXVECTOR3(kMultiWindow.x - kSpeedPos.x, kMultiWindow.y - kSpeedPos.y, 0.0F) },
		{ D3DXVECTOR3(kWindowWidth   - kSpeedPos.x, kMultiWindow.y - kSpeedPos.y, 0.0F) },
		{ D3DXVECTOR3(kMultiWindow.x - kSpeedPos.x, kWindowHeight  - kSpeedPos.y, 0.0F) },
		{ D3DXVECTOR3(kWindowWidth   - kSpeedPos.x, kWindowHeight  - kSpeedPos.y, 0.0F) }
	};

	/* 座標セット */
	D3DXVECTOR3 position = kMultiSpeedPosition[player_no] - kMultiSpeedPos;

	/* 描画セット */
	DrawSet(kMultiSpeedPosition[player_no], MeterRect, player_no);
	CountSet(position, (INT)speed_, MIDDLE, player_no);
}