/****************
 * XInputクラス *
 ****************/

/* 多重Include防止 */
#ifndef RACE_XINPUT_H_
#define RACE_XINPUT_H_

/* Include */
#include "common.h"
#include <XInput.h>

/**********
 * 構造体 *
 **********/
struct CONTROLER_STATE
{
	XINPUT_STATE state_;
	BOOL         connected_;
};

/********
 * 定数 *
 ********/
/* ゲームパッド情報 */
CONST INT kMaxPad        = 4;     // パッドの数
CONST INT kLeftDeadZone  = 20000; // 左スティックの遊び値
CONST INT kRightDeadZone = 6000;  // 右スティックの遊び値
CONST INT kTrigger       = 20;    // トリガーの遊び値

/* 入力用定義 */
CONST ULONG LEFT_STICK_UP     = 1;                       // 左スティック上方向
CONST ULONG LEFT_STICK_RIGHT  = LEFT_STICK_UP     << 1;  // 左スティック右方向
CONST ULONG LEFT_STICK_DOWN   = LEFT_STICK_RIGHT  << 1;  // 左スティック下方向
CONST ULONG LEFT_STICK_LEFT   = LEFT_STICK_DOWN   << 1;  // 左スティック左方向
CONST ULONG LEFT_STICK_PUSH   = LEFT_STICK_LEFT   << 1;  // 左スティック押し込み

CONST ULONG RIGHT_STICK_UP    = LEFT_STICK_PUSH   << 1;  // 右スティック上方向
CONST ULONG RIGHT_STICK_RIGHT = RIGHT_STICK_UP    << 1;  // 右スティック右方向
CONST ULONG RIGHT_STICK_DOWN  = RIGHT_STICK_RIGHT << 1;  // 右スティック下方向
CONST ULONG RIGHT_STICK_LEFT  = RIGHT_STICK_DOWN  << 1;  // 右スティック左方向
CONST ULONG RIGHT_STICK_PUSH  = RIGHT_STICK_LEFT  << 1;  // 右スティック押し込み

CONST ULONG LEFT_TRIGGER      = RIGHT_STICK_PUSH  << 1;  // 左トリガー
CONST ULONG RIGHT_TRIGGER     = LEFT_TRIGGER      << 1;  // 右トリガー

/****************
 * XInputクラス *
 ****************/
class XInput
{
private:
	static CONTROLER_STATE game_pad_[kMaxPad];  // ゲームパッド構造体

	/* 入力保存用 */
	static ULONG down_button_;

	/* スティック */
	static ULONG save_push_stick_[kMaxPad];   // 前のフレーム確認用
	static ULONG push_stick_[kMaxPad];        // 入力保存用

	/* ボタン */
	static ULONG push_button_[kMaxPad];        // 前のフレーム確認用
	static ULONG save_push_button_[kMaxPad];   // 入力保存用

public:

	static BOOL Device(VOID);  // デバイスの更新処理

	/* 入力を返す関数 */

	/* 左スティック */
	static INT AdditionLeftX(CONST INT player_pad_ = 0);  // X値
	static INT AdditionLeftY(CONST INT player_pad_ = 0);  // Y値

	/* 右スティック */
	static INT AdditionRightX(CONST INT player_pad = 0);  // X値
	static INT AdditionRightY(CONST INT player_pad = 0);  // Y値

	/* ボタン */
	static INT DownLeftTrigger(CONST INT player_pad_ = 0);
	static INT DownRightTrigger(CONST INT player_pad_ = 0);

	static INT DownButton(INT player_pad = 0) { return game_pad_[player_pad].state_.Gamepad.wButtons; }

	/* 入力を返す関数(フレーム単位) */
	/* フレーム単位の取得を管理する関数 */
	static VOID PushStick(INT player_pad = 0);      // 1回判定用
	static VOID PushButton(INT player_pad = 0);

	static ULONG ReturnPushStick(INT player_pad = 0){ return push_stick_[player_pad]; }
	static ULONG ReturnPushButton(INT player_pad = 0) { return push_button_[player_pad]; }
};

#endif