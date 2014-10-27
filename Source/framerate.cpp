/***********
 * Include *
 ***********/
#include "framerate.h"

DWORD             FrameRate::previous_time_;
DWORD             FrameRate::difference_;
CHAR              FrameRate::fps_string_[MAX_PATH];
LARGE_INTEGER     FrameRate::clock_counter_;
LONGLONG          FrameRate::previous_count_;
std::list<DOUBLE> FrameRate::list_;
BOOL              FrameRate::count_flag_;
DOUBLE            FrameRate::freq_;
DOUBLE            FrameRate::sum_times_;

/******************
 * コンストラクタ *
 ******************/
FrameRate::FrameRate(VOID)
{
	difference_ = 0L;
	previous_time_ = timeGetTime();
	sum_times_ = 0.0;
	strcpy_s(fps_string_, kWindowName);

	/* サンプル数 */
	list_.resize(kNumSamples, 0.0);

	/* QueryPerformanceCounterが使用可能か */
	if (QueryPerformanceCounter(&clock_counter_))
	{
		count_flag_ = TRUE; // QueryPerformanceCounterが使用可能

		/* 現在のクロック数と１秒あたりのクロック数を取得 */
		previous_count_ = clock_counter_.QuadPart;
		LARGE_INTEGER freq;
		QueryPerformanceFrequency(&freq);
		freq_ = static_cast<DOUBLE>(freq.QuadPart);
	}
	else
	{
		count_flag_ = FALSE;  // QueryPerformanceCounterは使用できない
		previous_count_ = timeGetTime();
	}
}

/*******************************************
 * 関数名 : GetDifferenceTime関数          *
 * 内  容 : 差分時刻を取得します           *
 * 引  数 : なし                           *
 * 戻り値 : 差分時刻をミリ秒にして返します *
 *******************************************/
DOUBLE FrameRate::GetDifferenceTime(VOID)
{
	DOUBLE current_difference = 0.0;

	/* QueryPerformanceCounterが使用できるか */
	if (count_flag_)
	{
		/* 現在の時刻を取得して差分を計算 */
		QueryPerformanceCounter(&clock_counter_);
		LONGLONG difference = clock_counter_.QuadPart - previous_count_;
		DOUBLE def = static_cast<DOUBLE>(difference);
		previous_count_ = clock_counter_.QuadPart;  // 今の時間を前回の時間にする
		current_difference = def * 1000.0 / freq_;  // 差分を算出
	}
	else
	{
		/* QueryPerformanceCounterは使えないのでtimeGetTime */
		DWORD current = timeGetTime();
		current_difference = static_cast<DOUBLE>(current - previous_count_);
		previous_count_ = current;
	}

	return current_difference;
}

/*************************************
 * 関数名 : Update関数               *
 * 内  容 : FPSを更新します          *
 * 引  数 :                          *
 * 戻り値 : フレームレートを返します *
 *************************************/
DOUBLE FrameRate::Update(CONST DOUBLE Difference)
{
	/* １つ古いデータを削除して現在の値を追加 */
	list_.pop_front();
	list_.push_back(Difference);

	/* FPSを算出 */
	DOUBLE fps = 0.0;
	DOUBLE average = (sum_times_ + Difference) / kNumSamples;
	if (average) fps = 1000.0 / average;

	/* 平均値更新 */
	sum_times_ += Difference - *list_.begin();

	return fps;
}

/*************************************
 * 関数名 : GetFps関数               *
 * 内  容 : FPSを取得します          *
 * 引  数 : なし                     *
 * 戻り値 : フレームレートを返します *
 *************************************/
DOUBLE FrameRate::GetFps(VOID)
{
	DOUBLE difference = GetDifferenceTime();

	/* 0の場合は計算不可 */
	if (!difference) return 0;

	return Update(difference);
}

/*******************************************************
 * 関数名 : Check関数                                  *
 * 内  容 : フレームレートの調整を行います             *
 * 引  数 : なし                                       *
 * 戻り値 : メインループの処理を行う場合TRUEを返します *
 *******************************************************/
BOOL FrameRate::Check(VOID)
{
	BOOL ret = FALSE;


	/* 16ミリ秒経過しているかチェック */
	DWORD current_time = timeGetTime();
	DWORD judge = (current_time - previous_time_) + difference_;

	if (judge > kRate)
	{
		difference_ = judge % kRate;   // 超えた時間を算出
		previous_time_ = current_time; // 今の時間を保存
		ret = TRUE;
	}

	return ret;
}

/******************************************
 * 関数名 : Draw関数                      *
 * 内  容 : FPSをタイトルバーに表示します *
 * 引  数 : なし                          *
 * 戻り値 : なし                          *
 ******************************************/
VOID FrameRate::Draw(VOID)
{
	CHAR str[MAX_PATH];	// タイトル名用

	/* タイトル名を作成して設定 */
	sprintf_s(str, MAX_PATH, "%s：%.1f", kWindowName, GetFps());
	SetWindowText(FindWindow(kClassName, fps_string_), str);

	/* 更新した名前を保存 */
	strcpy_s(fps_string_, str);
}