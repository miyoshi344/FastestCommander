/***********
 * Include *
 ***********/
#include "time.h"

/******************
 * コンストラクタ *
 ******************/
Time::Time(VOID)
{
	TimeFlag    = FALSE;  // リミットフラグ
	lap_count_  = 0;      // ラップカウント
	count_      = 0;      // ミリ秒カウント
	count_down_ = 5;      // 秒カウント
	limit_.ReSet();       // タイムリミット初期化
}

/****************
 * デストラクタ *
 ****************/
Time::~Time(VOID)
{
	/* テクスチャ破棄 */
	LayoutBase::Destroy();
}

/*********************************
 * 関数名 : ReSet関数            *
 * 内  容 : 初期化処理を行います *
 * 引  数 : なし                 *
 * 戻り値 : なし                 *
 *********************************/
VOID Time::ReSet(VOID)
{
	TimeFlag = FALSE;  // タイムリミットフラグ
	lap_count_  = 0;   // ラップカウント
	limit_.ReSet();    // タイムリミット初期化
	count_      = 0;   // ミリ秒カウント
	count_down_ = 5;   // 秒カウント
}

/*******************************
 * 関数名 : Update関数         *
 * 内  容 : 更新処理を行います *
 * 引  数 : なし               *
 * 戻り値 : なし               *
 *******************************/
VOID Time::UpDate(VOID)
{
	/* タイムの更新 */
	total_.sec[player_no_]++;
	lap_time_[player_no_].sec[0]++;
	limit_.sec--;

	/* ラップ時間の設定 */

	/* ミリ秒の設定 */
	if (lap_time_[player_no_].sec[0] >= 100)
	{
		lap_time_[player_no_].sec[1]++;
		lap_time_[player_no_].sec[0] = 0;
	}

	/* 秒の設定 */
	if (lap_time_[player_no_].sec[1] >= 60)
	{
		lap_time_[player_no_].sec[2]++;
		lap_time_[player_no_].sec[1] = 0;
	}

	/* トータル時間の設定 */

	/* ミリ秒の設定 */
	if (total_.sec[0] >= 100)
	{
		total_.sec[1]++;
		total_.sec[0] = 0;
	}

	/* 秒の設定 */
	if (total_.sec[1] >= 60)
	{
		total_.sec[2]++;
		total_.sec[1] = 0;
	}
}

/*******************************
 * 関数名 : Draw関数           *
 * 内  容 : 描画処理を行います *
 * 引  数 : なし               *
 * 戻り値 : なし               *
 *******************************/
VOID Time::Draw(VOID)
{
	//MultiDraw(player_no_);
}

/*********************************************
 * 関数名 : TimeLimit関数                    *
 * 内  容 : 制限時間の処理を行います         *
 * 引  数 : なし                             *
 * 戻り値 : 制限時間に達したらTRUEを返します *
 *********************************************/
BOOL Time::TimeLimit(VOID)
{
	BOOL ret = FALSE;

	/* リミットが0になったらTRUEを返す */
	if (limit_.second == 0 && limit_.sec == 0) ret = TRUE;
	else if (limit_.sec <= 0)
	{
		limit_.second--;      // 秒を減らす
		limit_.sec = MaxSec;  // ミリ秒を初期化
	}

	return ret;
}

/*******************************
 * 関数名 : Draw関数           *
 * 内  容 : 描画処理を行います *
 * 引  数 : なし               *
 * 戻り値 : なし               *
 *******************************/
VOID Time::SoloDraw(VOID)
{
	CountSet(D3DXVECTOR3(80.0F, 50.0F, 0.0F), limit_.second, LAGE);
	CountSet(D3DXVECTOR3(150.0F, 70.0F, 0.0F), limit_.sec, SMALL);
}

/*****************************************
 * 関数名 : MultiDraw関数                *
 * 内  容 : 多人数用の描画処理を行います *
 * 引  数 : INT player_no プレイヤー番号 *
 * 戻り値 : なし                         *
 *****************************************/
VOID Time::MultiDraw(INT player_no)
{
	/* 座標の作成 */
	D3DXVECTOR3 kMultiTime[kAllPlayer] = {
		{ kMultiWindow.x - kTimeInterval,                  50.0F, 0.0F },
		{ kWindowWidth   - kTimeInterval,                  50.0F, 0.0F },
		{ kMultiWindow.x - kTimeInterval, kMultiWindow.y + 50.0F, 0.0F },
		{ kWindowWidth   - kTimeInterval, kMultiWindow.y + 50.0F, 0.0F }
	};

	/* 座標の設定 */
	D3DXVECTOR3 pos = kMultiTime[player_no];

	/* 描画を行う */
	for (INT i = 0; i < 3; i++)
	{
		pos.x -= 75.0F;
		CountSet(pos, lap_time_[0].sec[i]);
	}
}

/************************************
 * 関数名 : Total関数               *
 * 内  容 : Totalの数値を描画します *
 * 引  数 : なし                    *
 * 戻り値 : なし                    *
 ************************************/
VOID Time::Total(VOID)
{
	D3DXVECTOR3 pos = kPosition;
	pos.x += 50.0F;

	D3DXVECTOR3 pos2 = pos;
	pos2.x += 50.0F;

	/* 描画セット */
	CountSet(kPosition, total_.sec[2]); // 分の描画
	CountSet(pos, total_.sec[1]);  // 秒の描画
	CountSet(pos2, total_.sec[0]);     // ミリ秒の描画
}

/**********************************
 * 関数名 : Lap関数               *
 * 内  容 : Lapの数値を描画します *
 * 引  数 : なし                  *
 * 戻り値 : なし                  *
 **********************************/
VOID Time::Lap(VOID)
{

	for (INT i = 0; i < 3; i++)
	{
		for (INT n = 0; n < 3; n++)
		{
			CountSet(D3DXVECTOR3(400.0F + (100.0F * n), 100.0F, 0.0F), lap_time_[i].sec[n]);
		}
	}
}

/*****************************************
 * 関数名 : LapUpDate関数                *
 * 内  容 : ラップタイムの更新を行います *
 * 引  数 : なし                         *
 * 戻り値 : なし                         *
 *****************************************/
VOID Time::LapUpDate(VOID)
{
	for (INT i = 0; i < 3; i++)
	{
		switch (lap_time_[player_no_].lap_count_)
		{
			case 0:lap_time_[player_no_].lap1[i] = lap_time_[player_no_].sec[i]; break;
			case 1:lap_time_[player_no_].lap2[i] = lap_time_[player_no_].sec[i]; break;
			case 2:lap_time_[player_no_].lap3[i] = lap_time_[player_no_].sec[i]; break;
			default:break; 
		}

		lap_time_[player_no_].sec[i] = 0;
	}
}

/*****************************************************
 * 関数名 : TimeCount関数                            *
 * 内  容 : カウントダウンの処理を行います           *
 * 引  数 : なし                                     *
 * 戻り値 : カウントダウンが終了したらTRUEを返します *
 *****************************************************/
BOOL Time::TimeCount(VOID)
{
	if (!TimeFlag)
	{
		/* カウントが規定数に到達したら */
		if (count_++ >= 60)
		{
			/* カウントをリセット */
			count_ = 0;


			if (count_down_ > 0)
			{
				//DSound::Pause(COUNT_SE);
				DSound::Play(COUNT_SE);
				count_down_--;
			}
		}

		/* カウントが指定数に到達したら */
		if (count_down_ == 0)
		{
			DSound::Pause(COUNT_SE);
			TimeFlag = TRUE;
			return TRUE;
		}
	}

	return FALSE;
}

/*******************************************
 * 関数名 : TimeCountDraw関数              *
 * 内  容 : カウントダウンの描画を行います *
 * 引  数 : なし                           *
 * 戻り値 : なし                           *
 *******************************************/
VOID Time::TimeCountDraw(VOID)
{
	/* 描画座標を設定 */
	D3DXVECTOR3 pos = D3DXVECTOR3(kMultiWindow.x, kMultiWindow.y, 0.0F);
	pos.x -= 25.0F;
	pos.y -= 25.0F;

	/* カウントを描画 */
	CountSet(pos, count_down_, MIDDLE, RED, 1);
}