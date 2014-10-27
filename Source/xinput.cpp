/* Include */
#include "xinput.h"

/******************
* 静的メンバ変数 *
******************/
CONTROLER_STATE XInput::game_pad_[kMaxPad];
ULONG XInput::push_stick_[kMaxPad];
ULONG XInput::save_push_stick_[kMaxPad];

ULONG XInput::push_button_[kMaxPad];
ULONG XInput::save_push_button_[kMaxPad];

/**********************************************************
* 関数名 : Device関数                                    *
* 内  容 : デバイスを取得しているかを確認します          *
* 引  数 : なし                                          *
* 戻り値 : デバイスを見つけられなかったらFALSEを返します *
**********************************************************/
BOOL XInput::Device(VOID)
{
	BOOL ret = TRUE;  // 戻り値用
	INT count_ = 0;   // デバイスカウント用

	DWORD dwResult;

	/* デバイスを取得します */
	for (DWORD i = 0; i < kMaxPad; i++)
	{
		dwResult = XInputGetState(i, &game_pad_[i].state_);

		if (dwResult == 0L) game_pad_[i].connected_ = TRUE;  // デバイスを取得
		else game_pad_[i].connected_ = FALSE;                // デバイスを取得できませんでした
	}

	/* デバイスが取得できたか判定 */
	for (DWORD i = 0; i < kMaxPad; i++)
	{
		if (game_pad_[i].connected_) count_++;
	}

	/* 1個もデバイスがなければFALSEを返す */
	if (count_ == 0) ret = FALSE;

	return ret;
}

/*******************************************
* 関数名 : AdditionLeftX関数              *
* 内  容 : 左スティックの入力を取得します *
* 引  数 : 判定したいパッド番号           *
* 戻り値 : 取得した値のX値                *
*******************************************/
INT XInput::AdditionLeftX(CONST INT player_pad_)
{
	/* 遊び値の判定 */
	if (game_pad_[player_pad_].state_.Gamepad.sThumbLX <  kLeftDeadZone &&
		game_pad_[player_pad_].state_.Gamepad.sThumbLX > -kLeftDeadZone)
	{
		/* 遊び値未満は0にする */
		return game_pad_[player_pad_].state_.Gamepad.sThumbLX = 0;
	}

	return game_pad_[player_pad_].state_.Gamepad.sThumbLX;
}

/********************************************
* 関数名 : AdditionLeftY関数               *
* 内  容 : 左スティックの入力を取得します  *
* 引  数 : 判定したいパッド番号            *
* 戻り値 : 取得した値のY値                 *
********************************************/
INT XInput::AdditionLeftY(CONST INT player_pad_)
{
	/* 遊び値の判定 */
	if (game_pad_[player_pad_].state_.Gamepad.sThumbLY <  kLeftDeadZone &&
		game_pad_[player_pad_].state_.Gamepad.sThumbLY > -kLeftDeadZone)
	{
		/* 遊び値未満は0にする */
		game_pad_[player_pad_].state_.Gamepad.sThumbLY = 0;
	}

	return game_pad_[player_pad_].state_.Gamepad.sThumbLY;
}

/********************************************
* 関数名 : AdditionRightX関数              *
* 内  容 : 右スティックの入力を取得します  *
* 引  数 : 判定したいパッド番号            *
* 戻り値 : 取得した値のX値                 *
********************************************/
INT XInput::AdditionRightX(CONST INT player_pad_)
{
	/* 遊び値の判定 */
	if (game_pad_[player_pad_].state_.Gamepad.sThumbRX <  kRightDeadZone &&
		game_pad_[player_pad_].state_.Gamepad.sThumbRX > -kRightDeadZone)
	{
		/* 遊び値未満は0にする */
		game_pad_[player_pad_].state_.Gamepad.sThumbRX = 0;
	}

	return game_pad_[player_pad_].state_.Gamepad.sThumbRX;
}

/********************************************
* 関数名 : AdditionRightY関数              *
* 内  容 : 右スティックの入力を取得します  *
* 引  数 : 判定したいパッド番号            *
* 戻り値 : 取得した値のY値                 *
********************************************/
INT XInput::AdditionRightY(CONST INT player_pad_)
{
	/* 遊び値の判定 */
	if (game_pad_[player_pad_].state_.Gamepad.sThumbRY <  kRightDeadZone &&
		game_pad_[player_pad_].state_.Gamepad.sThumbRY > -kRightDeadZone)
	{
		/* 遊び値未満は0にする */
		game_pad_[player_pad_].state_.Gamepad.sThumbRY = 0;
	}

	return game_pad_[player_pad_].state_.Gamepad.sThumbRY;
}

/*****************************************
* 関数名 : DownLeftTrigger関数          *
* 内  容 : 左トリガーの入力を取得します *
* 引  数 : 判定したいパッド番号         *
* 戻り値 : 取得したトリガーの値         *
*****************************************/
INT XInput::DownLeftTrigger(CONST INT player_pad_)
{
	/* 遊び値の判定 */
	if (game_pad_[player_pad_].state_.Gamepad.bLeftTrigger < kTrigger)
	{
		/* 遊び値未満は0にする */
		game_pad_[player_pad_].state_.Gamepad.bLeftTrigger = 0;
	}

	return game_pad_[player_pad_].state_.Gamepad.bLeftTrigger;
}

/*****************************************
* 関数名 : DownRightTrigger関数         *
* 内  容 : 右トリガーの入力を取得します *
* 引  数 : 判定したいパッド番号         *
* 戻り値 : 取得したトリガーの値         *
*****************************************/
INT XInput::DownRightTrigger(CONST INT player_pad_)
{
	/* 遊び値の判定 */
	if (game_pad_[player_pad_].state_.Gamepad.bRightTrigger < kTrigger)
	{
		/* 遊び値未満は0にする */
		game_pad_[player_pad_].state_.Gamepad.bRightTrigger = 0;
	}

	return game_pad_[player_pad_].state_.Gamepad.bRightTrigger;
}

/***********************************************
* 関数名 : PushButton関数                     *
* 内  容 : フレームでの1回入力を管理します    *
* 引  数 : INT player_pad  管理するパッド番号 *
* 戻り値 : なし                               *
***********************************************/
VOID XInput::PushStick(INT player_pad)
{
	/* 入力保存用 */
	ULONG stick = 0L;

	/* ゲームパッドが刺さってるかの確認 */
	if (game_pad_[player_pad].connected_)
	{
		/* 左スティック上方向 */
		if (game_pad_[player_pad].state_.Gamepad.sThumbLY > kLeftDeadZone)
		{
			/* 前回押されていなければフラグを上げる */
			if (!(save_push_stick_[player_pad] & LEFT_STICK_UP))
			{
				stick |= LEFT_STICK_UP;  // ボタン入力ビット
				save_push_stick_[player_pad] |= LEFT_STICK_UP;  // 記憶用ビット
			}
		}
		else save_push_stick_[player_pad] &= ~LEFT_STICK_UP;    // 記憶用ビットを下げる

		/* 左スティック右方向 */
		if (game_pad_[player_pad].state_.Gamepad.sThumbLX > kLeftDeadZone)
		{
			/* 前回押されていなければフラグを上げる */
			if (!(save_push_stick_[player_pad] & LEFT_STICK_RIGHT))
			{
				stick |= LEFT_STICK_RIGHT;  // ボタン入力ビット
				save_push_stick_[player_pad] |= LEFT_STICK_RIGHT;  // 記憶用ビット
			}
		}
		else save_push_stick_[player_pad] &= ~LEFT_STICK_RIGHT;    // 記憶用ビットを下げる

		/* 左スティック下方向 */
		if (game_pad_[player_pad].state_.Gamepad.sThumbLY < -kLeftDeadZone)
		{
			/* 前回押されていなければフラグを上げる */
			if (!(save_push_stick_[player_pad] & LEFT_STICK_DOWN))
			{
				stick |= LEFT_STICK_DOWN;   // ボタン入力ビット
				save_push_stick_[player_pad] |= LEFT_STICK_DOWN;   // 記憶用ビット
			}
		}
		else save_push_stick_[player_pad] &= ~LEFT_STICK_DOWN;     // 記憶用ビットを下げる

		/* 左スティック左方向 */
		if (game_pad_[player_pad].state_.Gamepad.sThumbLX < -kLeftDeadZone)
		{
			/* 前回押されていなければフラグを上げる */
			if (!(save_push_stick_[player_pad] & LEFT_STICK_LEFT))
			{
				stick |= LEFT_STICK_LEFT;   // ボタン入力ビット
				save_push_stick_[player_pad] |= LEFT_STICK_LEFT;   // 記憶用ビット
			}
		}
		else save_push_stick_[player_pad] &= ~LEFT_STICK_LEFT;     // 記憶用ビットを下げる 

		/* 右スティック上方向 */
		if (game_pad_[player_pad].state_.Gamepad.sThumbRY > kRightDeadZone)
		{
			/* 前回押されていなければフラグを上げる */
			stick |= RIGHT_STICK_UP;    // ボタン入力ビット
			save_push_stick_[player_pad] |= RIGHT_STICK_UP;    // 記憶用ビット
		}
		else save_push_stick_[player_pad] &= ~RIGHT_STICK_UP;  // 記憶用ビットを下げる

		/* 右スティック右方向 */
		if (game_pad_[player_pad].state_.Gamepad.sThumbRX > kRightDeadZone)
		{
			/* 前回押されていなければフラグを上げる */
			stick |= RIGHT_STICK_RIGHT;    // ボタン入力ビット
			save_push_stick_[player_pad] |= RIGHT_STICK_RIGHT;    // 記憶用ビット
		}
		else save_push_stick_[player_pad] &= ~RIGHT_STICK_RIGHT;  // 記憶用ビットを下げる

		/* 右スティック下方向 */
		if (game_pad_[player_pad].state_.Gamepad.sThumbRY < -kRightDeadZone)
		{
			/* 前回押されていなければフラグを上げる */
			stick |= RIGHT_STICK_DOWN;    // ボタン入力ビット
			save_push_stick_[player_pad] |= RIGHT_STICK_DOWN;    // 記憶用ビット
		}
		else save_push_stick_[player_pad] &= ~RIGHT_STICK_DOWN;  // 記憶用ビットを下げる

		/* 右スティック左方向 */
		if (game_pad_[player_pad].state_.Gamepad.sThumbRX < -kRightDeadZone)
		{
			/* 前回押されていなければフラグを上げる */
			stick |= RIGHT_STICK_LEFT;    // ボタン入力ビット
			save_push_stick_[player_pad] |= RIGHT_STICK_LEFT;    // 記憶用ビット
		}
		else save_push_stick_[player_pad] &= ~RIGHT_STICK_LEFT;  // 記憶用ビットを下げる
	}

	push_stick_[player_pad] = stick;  // 入力されたボタンを保存
}

/*****************************************
* 関数名 : PushButton関数               *
* 内  容 : ボタンの1回判定を行います    *
* 引  数 : INT　player_pad パッドの番号 *
* 戻り値 : なし                         *
*****************************************/
VOID XInput::PushButton(INT player_pad)
{
	/* 入力保存用 */
	ULONG button = 0L;

	/* ゲームパッドが刺さっているかの確認 */
	if (game_pad_[player_pad].connected_)
	{
		/* Aボタン */
		if (game_pad_[player_pad].state_.Gamepad.wButtons & XINPUT_GAMEPAD_A)
		{
			/* 前回押されていなければフラグを上げる */
			if (!(save_push_button_[player_pad] & XINPUT_GAMEPAD_A))
			{
				button |= XINPUT_GAMEPAD_A;  // ボタン入力ビット
				save_push_button_[player_pad] |= XINPUT_GAMEPAD_A;  // 記憶用ビット
			}
		}
		else save_push_button_[player_pad] &= ~XINPUT_GAMEPAD_A;    // 記憶用ビットを下げる

		/* Bボタン */
		if (game_pad_[player_pad].state_.Gamepad.wButtons & XINPUT_GAMEPAD_B)
		{
			/* 前回押されていなければフラグを上げる */
			if (!(save_push_button_[player_pad] & XINPUT_GAMEPAD_B))
			{
				button |= XINPUT_GAMEPAD_B;  // ボタン入力ビット
				save_push_button_[player_pad] |= XINPUT_GAMEPAD_B;  // 記憶用ビット
			}
		}
		else save_push_button_[player_pad] &= ~XINPUT_GAMEPAD_B;    // 記憶用ビットを下げる

		/* Xボタン */
		if (game_pad_[player_pad].state_.Gamepad.wButtons & XINPUT_GAMEPAD_X)
		{
			/* 前回押されていなければフラグを上げる */
			if (!(save_push_button_[player_pad] & XINPUT_GAMEPAD_X))
			{
				button |= XINPUT_GAMEPAD_X;  // ボタン入力ビット
				save_push_button_[player_pad] |= XINPUT_GAMEPAD_X;  // 記憶用ビット
			}
		}
		else save_push_button_[player_pad] &= ~XINPUT_GAMEPAD_X;    // 記憶用ビットを下げる

		/* Yボタン */
		if (game_pad_[player_pad].state_.Gamepad.wButtons & XINPUT_GAMEPAD_Y)
		{
			/* 前回押されていなければフラグを上げる */
			if (!(save_push_button_[player_pad] & XINPUT_GAMEPAD_Y))
			{
				button |= XINPUT_GAMEPAD_Y;  // ボタン入力ビット
				save_push_button_[player_pad] |= XINPUT_GAMEPAD_Y;  // 記憶用ビット
			}
		}
		else save_push_button_[player_pad] &= ~XINPUT_GAMEPAD_Y;    // 記憶用ビットを下げる

		/* 十字キー上 */
		if (game_pad_[player_pad].state_.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP)
		{
			/* 前回押されていなければフラグを上げる */
			if (!(save_push_button_[player_pad] & XINPUT_GAMEPAD_DPAD_UP))
			{
				button |= XINPUT_GAMEPAD_DPAD_UP;  // ボタン入力ビット
				save_push_button_[player_pad] |= XINPUT_GAMEPAD_DPAD_UP;  // 記憶用ビット
			}
		}
		else save_push_button_[player_pad] &= ~XINPUT_GAMEPAD_DPAD_UP;    // 記憶用ビットを下げる

		/* 十字キー右 */
		if (game_pad_[player_pad].state_.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT)
		{
			/* 前回押されていなければフラグを上げる */
			if (!(save_push_button_[player_pad] & XINPUT_GAMEPAD_DPAD_RIGHT))
			{
				button |= XINPUT_GAMEPAD_DPAD_RIGHT;  // ボタン入力ビット
				save_push_button_[player_pad] |= XINPUT_GAMEPAD_DPAD_RIGHT;  // 記憶用ビット
			}
		}
		else save_push_button_[player_pad] &= ~XINPUT_GAMEPAD_DPAD_RIGHT;    // 記憶用ビットを下げる

		/* 十字キー下 */
		if (game_pad_[player_pad].state_.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN)
		{
			/* 前回押されていなければフラグを上げる */
			if (!(save_push_button_[player_pad] & XINPUT_GAMEPAD_DPAD_DOWN))
			{
				button |= XINPUT_GAMEPAD_DPAD_DOWN;  // ボタン入力ビット
				save_push_button_[player_pad] |= XINPUT_GAMEPAD_DPAD_DOWN;  // 記憶用ビット
			}
		}
		else save_push_button_[player_pad] &= ~XINPUT_GAMEPAD_DPAD_DOWN;    // 記憶用ビットを下げる

		/* 十字キー左 */
		if (game_pad_[player_pad].state_.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT)
		{
			/* 前回押されていなければフラグを上げる */
			if (!(save_push_button_[player_pad] & XINPUT_GAMEPAD_DPAD_LEFT))
			{
				button |= XINPUT_GAMEPAD_DPAD_LEFT;  // ボタン入力ビット
				save_push_button_[player_pad] |= XINPUT_GAMEPAD_DPAD_LEFT;  // 記憶用ビット
			}
		}
		else save_push_button_[player_pad] &= ~XINPUT_GAMEPAD_DPAD_LEFT;    // 記憶用ビットを下げる

		/* スタートボタン */
		if (game_pad_[player_pad].state_.Gamepad.wButtons & XINPUT_GAMEPAD_START)
		{
			/* 前回押されていなければフラグを上げる */
			if (!(save_push_button_[player_pad] & XINPUT_GAMEPAD_START))
			{
				button |= XINPUT_GAMEPAD_START;  // ボタン入力ビット
				save_push_button_[player_pad] |= XINPUT_GAMEPAD_START;  // 記憶用ビット
			}
		}
		else save_push_button_[player_pad] &= ~XINPUT_GAMEPAD_START;    // 記憶用ビットを下げる

		/* バックボタン */
		if (game_pad_[player_pad].state_.Gamepad.wButtons & XINPUT_GAMEPAD_BACK)
		{
			/* 前回押されていなければフラグを上げる */
			if (!(save_push_button_[player_pad] & XINPUT_GAMEPAD_BACK))
			{
				button |= XINPUT_GAMEPAD_BACK;  // ボタン入力ビット
				save_push_button_[player_pad] |= XINPUT_GAMEPAD_BACK;  // 記憶用ビット
			}
		}
		else save_push_button_[player_pad] &= ~XINPUT_GAMEPAD_BACK;    // 記憶用ビットを下げる

		/* LEFT_THUMB(左スティック押し込み) */
		if (game_pad_[player_pad].state_.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB)
		{
			/* 前回押されていなければフラグを上げる */
			if (!(save_push_button_[player_pad] & XINPUT_GAMEPAD_LEFT_THUMB))
			{
				button |= XINPUT_GAMEPAD_LEFT_THUMB;  // ボタン入力ビット
				save_push_button_[player_pad] |= XINPUT_GAMEPAD_LEFT_THUMB;  // 記憶用ビット
			}
		}
		else save_push_button_[player_pad] &= ~XINPUT_GAMEPAD_LEFT_THUMB;    // 記憶用ビットを下げる

		/* RIGHT_THUMB(右スティック押し込み) */
		if (game_pad_[player_pad].state_.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB)
		{
			/* 前回押されていなければフラグを上げる */
			if (!(save_push_button_[player_pad] & XINPUT_GAMEPAD_RIGHT_THUMB))
			{
				button |= XINPUT_GAMEPAD_RIGHT_THUMB;  // ボタン入力ビット
				save_push_button_[player_pad] |= XINPUT_GAMEPAD_RIGHT_THUMB;  // 記憶用ビット
			}
		}
		else save_push_button_[player_pad] &= ~XINPUT_GAMEPAD_RIGHT_THUMB;    // 記憶用ビットを下げる

		/* LEFT_SHOULDER(L1ボタン)  */
		if (game_pad_[player_pad].state_.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER)
		{
			/* 前回押されていなければフラグを上げる */
			if (!(save_push_button_[player_pad] & XINPUT_GAMEPAD_LEFT_SHOULDER))
			{
				button |= XINPUT_GAMEPAD_LEFT_SHOULDER;  // ボタン入力ビット
				save_push_button_[player_pad] |= XINPUT_GAMEPAD_LEFT_SHOULDER;  // 記憶用ビット
			}
		}
		else save_push_button_[player_pad] &= ~XINPUT_GAMEPAD_LEFT_SHOULDER;    // 記憶用ビットを下げる

		/* RIGHT_SHOULDER(R1ボタン) */
		if (game_pad_[player_pad].state_.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER)
		{
			/* 前回押されていなければフラグを上げる */
			if (!(save_push_button_[player_pad] & XINPUT_GAMEPAD_RIGHT_SHOULDER))
			{
				button |= XINPUT_GAMEPAD_RIGHT_SHOULDER;  // ボタン入力ビット
				save_push_button_[player_pad] |= XINPUT_GAMEPAD_RIGHT_SHOULDER;  // 記憶用ビット
			}
		}
		else save_push_button_[player_pad] &= ~XINPUT_GAMEPAD_RIGHT_SHOULDER;    // 記憶用ビットを下げる

		/* LEFT_TRIGGER(左トリガー) */
		if (game_pad_[player_pad].state_.Gamepad.bLeftTrigger > kTrigger)
		{
			/* 前回押されていなければフラグを上げる */
			if (!(save_push_button_[player_pad] & LEFT_TRIGGER))
			{
				button |= LEFT_TRIGGER;  // ボタン入力ビット
				save_push_button_[player_pad] |= LEFT_TRIGGER;  // 記憶用ビット
			}
		}
		else save_push_button_[player_pad] &= ~LEFT_TRIGGER;    // 記憶用ビットを下げる

		/* RIGHT_TRIGGER(右トリガー) */
		if (game_pad_[player_pad].state_.Gamepad.bRightTrigger > kTrigger)
		{
			/* 前回押されていなければフラグを上げる */
			if (!(save_push_button_[player_pad] & RIGHT_TRIGGER))
			{
				button |= RIGHT_TRIGGER;  // ボタン入力ビット
				save_push_button_[player_pad] |= RIGHT_TRIGGER;  // 記憶用ビット
			}
		}
		else save_push_button_[player_pad] &= ~RIGHT_TRIGGER;    // 記憶用ビットを下げる
	}

	push_button_[player_pad] = button;
}