/**************
* timeクラス *
**************/

/* 多重Include防止 */
#ifndef RACE_TIME_H_
#define RACE_TIME_H_

/***********
 * Include *
 ***********/
#include "layout_base.h"

/************
 * 文字情報 *
 ************/
CONST D3DXVECTOR3 kPosition = D3DXVECTOR3(300.0F, 500.0F, 0.0F);
CONST FLOAT kTimeInterval   = 50.0F;   // 間隔

/* 追加時間 */
CONST INT MaxSecond = 30;  // 
CONST INT MaxSec    = 100;
CONST INT countUp   = 0;

/**********
* 構造体 *
**********/

/* Far用構造体 */
struct LapTime
{
	INT lap_count_;

	INT sec[3];

	INT lap1[3];
	INT lap2[3];
	INT lap3[3];

	D3DXVECTOR3 lap1_position_[3];
	D3DXVECTOR3 lap2_position_[3];
	D3DXVECTOR3 lap3_position_[3];

	LapTime(VOID)
	{
		lap_count_ = 0;

		for (INT i = 0; i < 3; i++)
		{
			sec[i] = 0;

			lap1[i] = 0;
			lap2[i] = 0;
			lap3[i] = 0;

			lap1_position_[i] = D3DXVECTOR3(0.0F, 0.0F, 0.0F);
			lap2_position_[i] = D3DXVECTOR3(0.0F, 0.0F, 0.0F);
			lap3_position_[i] = D3DXVECTOR3(0.0F, 0.0F, 0.0F);
		}
	}
};

/* 制限時間用構造体 */
struct LimitTime
{
	/* タイム */
	INT second,
		sec;

	/* コンストラクタ */
	VOID ReSet(VOID)
	{
		second = MaxSecond;
		sec    = MaxSec;
	}
};

/****************************
* 時間を管理するクラスです *
****************************/
class Time : public LayoutBase
{
private:

	/**************
	 * 構造体宣言 *
	 **************/
	LimitTime limit_;
	LapTime   total_;
	LapTime   lap_time_[kAllPlayer];

	/************
	 * 変数宣言 *
	 ************/
	INT lap_count_;
	BOOL TimeFlag;

	INT count_;
	INT count_down_;

	/************
	 * 内部関数 *
	 ************/
	VOID SoloDraw(VOID);            // 1人用描画処理
	VOID MultiDraw(INT player_no);  // 多人数用描画処理

public:
	Time(VOID);            // コンストラクタ
	~Time(VOID);           // デストラクタ

	VOID ReSet(VOID);
	VOID UpDate(VOID);     // 更新処理
	VOID LapUpDate(VOID);  // 周回更新処理
	BOOL TimeLimit(VOID);  // 制限時間処理
	VOID Draw(VOID);       // 描画処理
	VOID Lap(VOID);        // 周回更新処理

	VOID Total(VOID);      // トータル処理

	BOOL TimeCount(VOID);      // タイムカウント処理
	VOID TimeCountDraw(VOID);  // タイムカウント描画処理

	/* タイムリミット増加処理 */
	VOID CheckPointLimit(VOID) { limit_.second += countUp; }

	/* タイムリミット処理 */
	BOOL ReturnFlag(VOID) { return TimeFlag; }
};

#endif