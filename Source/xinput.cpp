/* Include */
#include "xinput.h"

/******************
* �ÓI�����o�ϐ� *
******************/
CONTROLER_STATE XInput::game_pad_[kMaxPad];
ULONG XInput::push_stick_[kMaxPad];
ULONG XInput::save_push_stick_[kMaxPad];

ULONG XInput::push_button_[kMaxPad];
ULONG XInput::save_push_button_[kMaxPad];

/**********************************************************
* �֐��� : Device�֐�                                    *
* ��  �e : �f�o�C�X���擾���Ă��邩���m�F���܂�          *
* ��  �� : �Ȃ�                                          *
* �߂�l : �f�o�C�X���������Ȃ�������FALSE��Ԃ��܂� *
**********************************************************/
BOOL XInput::Device(VOID)
{
	BOOL ret = TRUE;  // �߂�l�p
	INT count_ = 0;   // �f�o�C�X�J�E���g�p

	DWORD dwResult;

	/* �f�o�C�X���擾���܂� */
	for (DWORD i = 0; i < kMaxPad; i++)
	{
		dwResult = XInputGetState(i, &game_pad_[i].state_);

		if (dwResult == 0L) game_pad_[i].connected_ = TRUE;  // �f�o�C�X���擾
		else game_pad_[i].connected_ = FALSE;                // �f�o�C�X���擾�ł��܂���ł���
	}

	/* �f�o�C�X���擾�ł��������� */
	for (DWORD i = 0; i < kMaxPad; i++)
	{
		if (game_pad_[i].connected_) count_++;
	}

	/* 1���f�o�C�X���Ȃ����FALSE��Ԃ� */
	if (count_ == 0) ret = FALSE;

	return ret;
}

/*******************************************
* �֐��� : AdditionLeftX�֐�              *
* ��  �e : ���X�e�B�b�N�̓��͂��擾���܂� *
* ��  �� : ���肵�����p�b�h�ԍ�           *
* �߂�l : �擾�����l��X�l                *
*******************************************/
INT XInput::AdditionLeftX(CONST INT player_pad_)
{
	/* �V�ђl�̔��� */
	if (game_pad_[player_pad_].state_.Gamepad.sThumbLX <  kLeftDeadZone &&
		game_pad_[player_pad_].state_.Gamepad.sThumbLX > -kLeftDeadZone)
	{
		/* �V�ђl������0�ɂ��� */
		return game_pad_[player_pad_].state_.Gamepad.sThumbLX = 0;
	}

	return game_pad_[player_pad_].state_.Gamepad.sThumbLX;
}

/********************************************
* �֐��� : AdditionLeftY�֐�               *
* ��  �e : ���X�e�B�b�N�̓��͂��擾���܂�  *
* ��  �� : ���肵�����p�b�h�ԍ�            *
* �߂�l : �擾�����l��Y�l                 *
********************************************/
INT XInput::AdditionLeftY(CONST INT player_pad_)
{
	/* �V�ђl�̔��� */
	if (game_pad_[player_pad_].state_.Gamepad.sThumbLY <  kLeftDeadZone &&
		game_pad_[player_pad_].state_.Gamepad.sThumbLY > -kLeftDeadZone)
	{
		/* �V�ђl������0�ɂ��� */
		game_pad_[player_pad_].state_.Gamepad.sThumbLY = 0;
	}

	return game_pad_[player_pad_].state_.Gamepad.sThumbLY;
}

/********************************************
* �֐��� : AdditionRightX�֐�              *
* ��  �e : �E�X�e�B�b�N�̓��͂��擾���܂�  *
* ��  �� : ���肵�����p�b�h�ԍ�            *
* �߂�l : �擾�����l��X�l                 *
********************************************/
INT XInput::AdditionRightX(CONST INT player_pad_)
{
	/* �V�ђl�̔��� */
	if (game_pad_[player_pad_].state_.Gamepad.sThumbRX <  kRightDeadZone &&
		game_pad_[player_pad_].state_.Gamepad.sThumbRX > -kRightDeadZone)
	{
		/* �V�ђl������0�ɂ��� */
		game_pad_[player_pad_].state_.Gamepad.sThumbRX = 0;
	}

	return game_pad_[player_pad_].state_.Gamepad.sThumbRX;
}

/********************************************
* �֐��� : AdditionRightY�֐�              *
* ��  �e : �E�X�e�B�b�N�̓��͂��擾���܂�  *
* ��  �� : ���肵�����p�b�h�ԍ�            *
* �߂�l : �擾�����l��Y�l                 *
********************************************/
INT XInput::AdditionRightY(CONST INT player_pad_)
{
	/* �V�ђl�̔��� */
	if (game_pad_[player_pad_].state_.Gamepad.sThumbRY <  kRightDeadZone &&
		game_pad_[player_pad_].state_.Gamepad.sThumbRY > -kRightDeadZone)
	{
		/* �V�ђl������0�ɂ��� */
		game_pad_[player_pad_].state_.Gamepad.sThumbRY = 0;
	}

	return game_pad_[player_pad_].state_.Gamepad.sThumbRY;
}

/*****************************************
* �֐��� : DownLeftTrigger�֐�          *
* ��  �e : ���g���K�[�̓��͂��擾���܂� *
* ��  �� : ���肵�����p�b�h�ԍ�         *
* �߂�l : �擾�����g���K�[�̒l         *
*****************************************/
INT XInput::DownLeftTrigger(CONST INT player_pad_)
{
	/* �V�ђl�̔��� */
	if (game_pad_[player_pad_].state_.Gamepad.bLeftTrigger < kTrigger)
	{
		/* �V�ђl������0�ɂ��� */
		game_pad_[player_pad_].state_.Gamepad.bLeftTrigger = 0;
	}

	return game_pad_[player_pad_].state_.Gamepad.bLeftTrigger;
}

/*****************************************
* �֐��� : DownRightTrigger�֐�         *
* ��  �e : �E�g���K�[�̓��͂��擾���܂� *
* ��  �� : ���肵�����p�b�h�ԍ�         *
* �߂�l : �擾�����g���K�[�̒l         *
*****************************************/
INT XInput::DownRightTrigger(CONST INT player_pad_)
{
	/* �V�ђl�̔��� */
	if (game_pad_[player_pad_].state_.Gamepad.bRightTrigger < kTrigger)
	{
		/* �V�ђl������0�ɂ��� */
		game_pad_[player_pad_].state_.Gamepad.bRightTrigger = 0;
	}

	return game_pad_[player_pad_].state_.Gamepad.bRightTrigger;
}

/***********************************************
* �֐��� : PushButton�֐�                     *
* ��  �e : �t���[���ł�1����͂��Ǘ����܂�    *
* ��  �� : INT player_pad  �Ǘ�����p�b�h�ԍ� *
* �߂�l : �Ȃ�                               *
***********************************************/
VOID XInput::PushStick(INT player_pad)
{
	/* ���͕ۑ��p */
	ULONG stick = 0L;

	/* �Q�[���p�b�h���h�����Ă邩�̊m�F */
	if (game_pad_[player_pad].connected_)
	{
		/* ���X�e�B�b�N����� */
		if (game_pad_[player_pad].state_.Gamepad.sThumbLY > kLeftDeadZone)
		{
			/* �O�񉟂���Ă��Ȃ���΃t���O���グ�� */
			if (!(save_push_stick_[player_pad] & LEFT_STICK_UP))
			{
				stick |= LEFT_STICK_UP;  // �{�^�����̓r�b�g
				save_push_stick_[player_pad] |= LEFT_STICK_UP;  // �L���p�r�b�g
			}
		}
		else save_push_stick_[player_pad] &= ~LEFT_STICK_UP;    // �L���p�r�b�g��������

		/* ���X�e�B�b�N�E���� */
		if (game_pad_[player_pad].state_.Gamepad.sThumbLX > kLeftDeadZone)
		{
			/* �O�񉟂���Ă��Ȃ���΃t���O���グ�� */
			if (!(save_push_stick_[player_pad] & LEFT_STICK_RIGHT))
			{
				stick |= LEFT_STICK_RIGHT;  // �{�^�����̓r�b�g
				save_push_stick_[player_pad] |= LEFT_STICK_RIGHT;  // �L���p�r�b�g
			}
		}
		else save_push_stick_[player_pad] &= ~LEFT_STICK_RIGHT;    // �L���p�r�b�g��������

		/* ���X�e�B�b�N������ */
		if (game_pad_[player_pad].state_.Gamepad.sThumbLY < -kLeftDeadZone)
		{
			/* �O�񉟂���Ă��Ȃ���΃t���O���グ�� */
			if (!(save_push_stick_[player_pad] & LEFT_STICK_DOWN))
			{
				stick |= LEFT_STICK_DOWN;   // �{�^�����̓r�b�g
				save_push_stick_[player_pad] |= LEFT_STICK_DOWN;   // �L���p�r�b�g
			}
		}
		else save_push_stick_[player_pad] &= ~LEFT_STICK_DOWN;     // �L���p�r�b�g��������

		/* ���X�e�B�b�N������ */
		if (game_pad_[player_pad].state_.Gamepad.sThumbLX < -kLeftDeadZone)
		{
			/* �O�񉟂���Ă��Ȃ���΃t���O���グ�� */
			if (!(save_push_stick_[player_pad] & LEFT_STICK_LEFT))
			{
				stick |= LEFT_STICK_LEFT;   // �{�^�����̓r�b�g
				save_push_stick_[player_pad] |= LEFT_STICK_LEFT;   // �L���p�r�b�g
			}
		}
		else save_push_stick_[player_pad] &= ~LEFT_STICK_LEFT;     // �L���p�r�b�g�������� 

		/* �E�X�e�B�b�N����� */
		if (game_pad_[player_pad].state_.Gamepad.sThumbRY > kRightDeadZone)
		{
			/* �O�񉟂���Ă��Ȃ���΃t���O���グ�� */
			stick |= RIGHT_STICK_UP;    // �{�^�����̓r�b�g
			save_push_stick_[player_pad] |= RIGHT_STICK_UP;    // �L���p�r�b�g
		}
		else save_push_stick_[player_pad] &= ~RIGHT_STICK_UP;  // �L���p�r�b�g��������

		/* �E�X�e�B�b�N�E���� */
		if (game_pad_[player_pad].state_.Gamepad.sThumbRX > kRightDeadZone)
		{
			/* �O�񉟂���Ă��Ȃ���΃t���O���グ�� */
			stick |= RIGHT_STICK_RIGHT;    // �{�^�����̓r�b�g
			save_push_stick_[player_pad] |= RIGHT_STICK_RIGHT;    // �L���p�r�b�g
		}
		else save_push_stick_[player_pad] &= ~RIGHT_STICK_RIGHT;  // �L���p�r�b�g��������

		/* �E�X�e�B�b�N������ */
		if (game_pad_[player_pad].state_.Gamepad.sThumbRY < -kRightDeadZone)
		{
			/* �O�񉟂���Ă��Ȃ���΃t���O���グ�� */
			stick |= RIGHT_STICK_DOWN;    // �{�^�����̓r�b�g
			save_push_stick_[player_pad] |= RIGHT_STICK_DOWN;    // �L���p�r�b�g
		}
		else save_push_stick_[player_pad] &= ~RIGHT_STICK_DOWN;  // �L���p�r�b�g��������

		/* �E�X�e�B�b�N������ */
		if (game_pad_[player_pad].state_.Gamepad.sThumbRX < -kRightDeadZone)
		{
			/* �O�񉟂���Ă��Ȃ���΃t���O���グ�� */
			stick |= RIGHT_STICK_LEFT;    // �{�^�����̓r�b�g
			save_push_stick_[player_pad] |= RIGHT_STICK_LEFT;    // �L���p�r�b�g
		}
		else save_push_stick_[player_pad] &= ~RIGHT_STICK_LEFT;  // �L���p�r�b�g��������
	}

	push_stick_[player_pad] = stick;  // ���͂��ꂽ�{�^����ۑ�
}

/*****************************************
* �֐��� : PushButton�֐�               *
* ��  �e : �{�^����1�񔻒���s���܂�    *
* ��  �� : INT�@player_pad �p�b�h�̔ԍ� *
* �߂�l : �Ȃ�                         *
*****************************************/
VOID XInput::PushButton(INT player_pad)
{
	/* ���͕ۑ��p */
	ULONG button = 0L;

	/* �Q�[���p�b�h���h�����Ă��邩�̊m�F */
	if (game_pad_[player_pad].connected_)
	{
		/* A�{�^�� */
		if (game_pad_[player_pad].state_.Gamepad.wButtons & XINPUT_GAMEPAD_A)
		{
			/* �O�񉟂���Ă��Ȃ���΃t���O���グ�� */
			if (!(save_push_button_[player_pad] & XINPUT_GAMEPAD_A))
			{
				button |= XINPUT_GAMEPAD_A;  // �{�^�����̓r�b�g
				save_push_button_[player_pad] |= XINPUT_GAMEPAD_A;  // �L���p�r�b�g
			}
		}
		else save_push_button_[player_pad] &= ~XINPUT_GAMEPAD_A;    // �L���p�r�b�g��������

		/* B�{�^�� */
		if (game_pad_[player_pad].state_.Gamepad.wButtons & XINPUT_GAMEPAD_B)
		{
			/* �O�񉟂���Ă��Ȃ���΃t���O���グ�� */
			if (!(save_push_button_[player_pad] & XINPUT_GAMEPAD_B))
			{
				button |= XINPUT_GAMEPAD_B;  // �{�^�����̓r�b�g
				save_push_button_[player_pad] |= XINPUT_GAMEPAD_B;  // �L���p�r�b�g
			}
		}
		else save_push_button_[player_pad] &= ~XINPUT_GAMEPAD_B;    // �L���p�r�b�g��������

		/* X�{�^�� */
		if (game_pad_[player_pad].state_.Gamepad.wButtons & XINPUT_GAMEPAD_X)
		{
			/* �O�񉟂���Ă��Ȃ���΃t���O���グ�� */
			if (!(save_push_button_[player_pad] & XINPUT_GAMEPAD_X))
			{
				button |= XINPUT_GAMEPAD_X;  // �{�^�����̓r�b�g
				save_push_button_[player_pad] |= XINPUT_GAMEPAD_X;  // �L���p�r�b�g
			}
		}
		else save_push_button_[player_pad] &= ~XINPUT_GAMEPAD_X;    // �L���p�r�b�g��������

		/* Y�{�^�� */
		if (game_pad_[player_pad].state_.Gamepad.wButtons & XINPUT_GAMEPAD_Y)
		{
			/* �O�񉟂���Ă��Ȃ���΃t���O���グ�� */
			if (!(save_push_button_[player_pad] & XINPUT_GAMEPAD_Y))
			{
				button |= XINPUT_GAMEPAD_Y;  // �{�^�����̓r�b�g
				save_push_button_[player_pad] |= XINPUT_GAMEPAD_Y;  // �L���p�r�b�g
			}
		}
		else save_push_button_[player_pad] &= ~XINPUT_GAMEPAD_Y;    // �L���p�r�b�g��������

		/* �\���L�[�� */
		if (game_pad_[player_pad].state_.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP)
		{
			/* �O�񉟂���Ă��Ȃ���΃t���O���グ�� */
			if (!(save_push_button_[player_pad] & XINPUT_GAMEPAD_DPAD_UP))
			{
				button |= XINPUT_GAMEPAD_DPAD_UP;  // �{�^�����̓r�b�g
				save_push_button_[player_pad] |= XINPUT_GAMEPAD_DPAD_UP;  // �L���p�r�b�g
			}
		}
		else save_push_button_[player_pad] &= ~XINPUT_GAMEPAD_DPAD_UP;    // �L���p�r�b�g��������

		/* �\���L�[�E */
		if (game_pad_[player_pad].state_.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT)
		{
			/* �O�񉟂���Ă��Ȃ���΃t���O���グ�� */
			if (!(save_push_button_[player_pad] & XINPUT_GAMEPAD_DPAD_RIGHT))
			{
				button |= XINPUT_GAMEPAD_DPAD_RIGHT;  // �{�^�����̓r�b�g
				save_push_button_[player_pad] |= XINPUT_GAMEPAD_DPAD_RIGHT;  // �L���p�r�b�g
			}
		}
		else save_push_button_[player_pad] &= ~XINPUT_GAMEPAD_DPAD_RIGHT;    // �L���p�r�b�g��������

		/* �\���L�[�� */
		if (game_pad_[player_pad].state_.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN)
		{
			/* �O�񉟂���Ă��Ȃ���΃t���O���グ�� */
			if (!(save_push_button_[player_pad] & XINPUT_GAMEPAD_DPAD_DOWN))
			{
				button |= XINPUT_GAMEPAD_DPAD_DOWN;  // �{�^�����̓r�b�g
				save_push_button_[player_pad] |= XINPUT_GAMEPAD_DPAD_DOWN;  // �L���p�r�b�g
			}
		}
		else save_push_button_[player_pad] &= ~XINPUT_GAMEPAD_DPAD_DOWN;    // �L���p�r�b�g��������

		/* �\���L�[�� */
		if (game_pad_[player_pad].state_.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT)
		{
			/* �O�񉟂���Ă��Ȃ���΃t���O���グ�� */
			if (!(save_push_button_[player_pad] & XINPUT_GAMEPAD_DPAD_LEFT))
			{
				button |= XINPUT_GAMEPAD_DPAD_LEFT;  // �{�^�����̓r�b�g
				save_push_button_[player_pad] |= XINPUT_GAMEPAD_DPAD_LEFT;  // �L���p�r�b�g
			}
		}
		else save_push_button_[player_pad] &= ~XINPUT_GAMEPAD_DPAD_LEFT;    // �L���p�r�b�g��������

		/* �X�^�[�g�{�^�� */
		if (game_pad_[player_pad].state_.Gamepad.wButtons & XINPUT_GAMEPAD_START)
		{
			/* �O�񉟂���Ă��Ȃ���΃t���O���グ�� */
			if (!(save_push_button_[player_pad] & XINPUT_GAMEPAD_START))
			{
				button |= XINPUT_GAMEPAD_START;  // �{�^�����̓r�b�g
				save_push_button_[player_pad] |= XINPUT_GAMEPAD_START;  // �L���p�r�b�g
			}
		}
		else save_push_button_[player_pad] &= ~XINPUT_GAMEPAD_START;    // �L���p�r�b�g��������

		/* �o�b�N�{�^�� */
		if (game_pad_[player_pad].state_.Gamepad.wButtons & XINPUT_GAMEPAD_BACK)
		{
			/* �O�񉟂���Ă��Ȃ���΃t���O���グ�� */
			if (!(save_push_button_[player_pad] & XINPUT_GAMEPAD_BACK))
			{
				button |= XINPUT_GAMEPAD_BACK;  // �{�^�����̓r�b�g
				save_push_button_[player_pad] |= XINPUT_GAMEPAD_BACK;  // �L���p�r�b�g
			}
		}
		else save_push_button_[player_pad] &= ~XINPUT_GAMEPAD_BACK;    // �L���p�r�b�g��������

		/* LEFT_THUMB(���X�e�B�b�N��������) */
		if (game_pad_[player_pad].state_.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB)
		{
			/* �O�񉟂���Ă��Ȃ���΃t���O���グ�� */
			if (!(save_push_button_[player_pad] & XINPUT_GAMEPAD_LEFT_THUMB))
			{
				button |= XINPUT_GAMEPAD_LEFT_THUMB;  // �{�^�����̓r�b�g
				save_push_button_[player_pad] |= XINPUT_GAMEPAD_LEFT_THUMB;  // �L���p�r�b�g
			}
		}
		else save_push_button_[player_pad] &= ~XINPUT_GAMEPAD_LEFT_THUMB;    // �L���p�r�b�g��������

		/* RIGHT_THUMB(�E�X�e�B�b�N��������) */
		if (game_pad_[player_pad].state_.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB)
		{
			/* �O�񉟂���Ă��Ȃ���΃t���O���グ�� */
			if (!(save_push_button_[player_pad] & XINPUT_GAMEPAD_RIGHT_THUMB))
			{
				button |= XINPUT_GAMEPAD_RIGHT_THUMB;  // �{�^�����̓r�b�g
				save_push_button_[player_pad] |= XINPUT_GAMEPAD_RIGHT_THUMB;  // �L���p�r�b�g
			}
		}
		else save_push_button_[player_pad] &= ~XINPUT_GAMEPAD_RIGHT_THUMB;    // �L���p�r�b�g��������

		/* LEFT_SHOULDER(L1�{�^��)  */
		if (game_pad_[player_pad].state_.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER)
		{
			/* �O�񉟂���Ă��Ȃ���΃t���O���グ�� */
			if (!(save_push_button_[player_pad] & XINPUT_GAMEPAD_LEFT_SHOULDER))
			{
				button |= XINPUT_GAMEPAD_LEFT_SHOULDER;  // �{�^�����̓r�b�g
				save_push_button_[player_pad] |= XINPUT_GAMEPAD_LEFT_SHOULDER;  // �L���p�r�b�g
			}
		}
		else save_push_button_[player_pad] &= ~XINPUT_GAMEPAD_LEFT_SHOULDER;    // �L���p�r�b�g��������

		/* RIGHT_SHOULDER(R1�{�^��) */
		if (game_pad_[player_pad].state_.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER)
		{
			/* �O�񉟂���Ă��Ȃ���΃t���O���グ�� */
			if (!(save_push_button_[player_pad] & XINPUT_GAMEPAD_RIGHT_SHOULDER))
			{
				button |= XINPUT_GAMEPAD_RIGHT_SHOULDER;  // �{�^�����̓r�b�g
				save_push_button_[player_pad] |= XINPUT_GAMEPAD_RIGHT_SHOULDER;  // �L���p�r�b�g
			}
		}
		else save_push_button_[player_pad] &= ~XINPUT_GAMEPAD_RIGHT_SHOULDER;    // �L���p�r�b�g��������

		/* LEFT_TRIGGER(���g���K�[) */
		if (game_pad_[player_pad].state_.Gamepad.bLeftTrigger > kTrigger)
		{
			/* �O�񉟂���Ă��Ȃ���΃t���O���グ�� */
			if (!(save_push_button_[player_pad] & LEFT_TRIGGER))
			{
				button |= LEFT_TRIGGER;  // �{�^�����̓r�b�g
				save_push_button_[player_pad] |= LEFT_TRIGGER;  // �L���p�r�b�g
			}
		}
		else save_push_button_[player_pad] &= ~LEFT_TRIGGER;    // �L���p�r�b�g��������

		/* RIGHT_TRIGGER(�E�g���K�[) */
		if (game_pad_[player_pad].state_.Gamepad.bRightTrigger > kTrigger)
		{
			/* �O�񉟂���Ă��Ȃ���΃t���O���グ�� */
			if (!(save_push_button_[player_pad] & RIGHT_TRIGGER))
			{
				button |= RIGHT_TRIGGER;  // �{�^�����̓r�b�g
				save_push_button_[player_pad] |= RIGHT_TRIGGER;  // �L���p�r�b�g
			}
		}
		else save_push_button_[player_pad] &= ~RIGHT_TRIGGER;    // �L���p�r�b�g��������
	}

	push_button_[player_pad] = button;
}