/***********
 * FPS管理 *
 ***********/

/* 多重Include防止 */
#ifndef RACE_FRAME_RATE_H_
#define RACE_FRAME_RATE_H_

/***********
 * Include *
 ***********/
#include "common.h"

/********
 * 定数 *
 ********/
CONST INT kRate       = 16;  // 60フレーム調整用
CONST INT kNumSamples = 10;  // フレームレート算出用サンプル数

/******************************
 * フレームレートを管理します *
 ******************************/
class FrameRate
{
private:

	static DWORD previous_time_;             // １フレーム前の時刻
	static DWORD difference_;                // 前の時間との誤差
	static CHAR  fps_string_[MAX_PATH];      // フレーム文字列

	static LARGE_INTEGER     clock_counter_;   // クロックカウンター
	static LONGLONG          previous_count_;  // 以前のクロックカウント数
	static std::list<DOUBLE> list_;            // サンプル格納用
	static BOOL              count_flag_;      // 計測方法
	static DOUBLE            freq_;            // 1秒当たりのクロックカウント数
	static DOUBLE            sum_times_;       // 合計値

	static DOUBLE Update(CONST DOUBLE Difference); // FPSを更新します
	static DOUBLE GetFps(VOID);                    // FPSを取得します

public:
	FrameRate(VOID);  // コンストラクタ

	static BOOL Check(VOID); // 60フレーム更新を確認します
	static VOID Draw(VOID);  // FPSを表示します

	static DOUBLE GetDifferenceTime(VOID);         // 差分時刻を取得します
};

#endif