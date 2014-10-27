/****************
 * XInput�N���X *
 ****************/

/* ���dInclude�h�~ */
#ifndef RACE_XINPUT_H_
#define RACE_XINPUT_H_

/* Include */
#include "common.h"
#include <XInput.h>

/**********
 * �\���� *
 **********/
struct CONTROLER_STATE
{
	XINPUT_STATE state_;
	BOOL         connected_;
};

/********
 * �萔 *
 ********/
/* �Q�[���p�b�h��� */
CONST INT kMaxPad        = 4;     // �p�b�h�̐�
CONST INT kLeftDeadZone  = 20000; // ���X�e�B�b�N�̗V�ђl
CONST INT kRightDeadZone = 6000;  // �E�X�e�B�b�N�̗V�ђl
CONST INT kTrigger       = 20;    // �g���K�[�̗V�ђl

/* ���͗p��` */
CONST ULONG LEFT_STICK_UP     = 1;                       // ���X�e�B�b�N�����
CONST ULONG LEFT_STICK_RIGHT  = LEFT_STICK_UP     << 1;  // ���X�e�B�b�N�E����
CONST ULONG LEFT_STICK_DOWN   = LEFT_STICK_RIGHT  << 1;  // ���X�e�B�b�N������
CONST ULONG LEFT_STICK_LEFT   = LEFT_STICK_DOWN   << 1;  // ���X�e�B�b�N������
CONST ULONG LEFT_STICK_PUSH   = LEFT_STICK_LEFT   << 1;  // ���X�e�B�b�N��������

CONST ULONG RIGHT_STICK_UP    = LEFT_STICK_PUSH   << 1;  // �E�X�e�B�b�N�����
CONST ULONG RIGHT_STICK_RIGHT = RIGHT_STICK_UP    << 1;  // �E�X�e�B�b�N�E����
CONST ULONG RIGHT_STICK_DOWN  = RIGHT_STICK_RIGHT << 1;  // �E�X�e�B�b�N������
CONST ULONG RIGHT_STICK_LEFT  = RIGHT_STICK_DOWN  << 1;  // �E�X�e�B�b�N������
CONST ULONG RIGHT_STICK_PUSH  = RIGHT_STICK_LEFT  << 1;  // �E�X�e�B�b�N��������

CONST ULONG LEFT_TRIGGER      = RIGHT_STICK_PUSH  << 1;  // ���g���K�[
CONST ULONG RIGHT_TRIGGER     = LEFT_TRIGGER      << 1;  // �E�g���K�[

/****************
 * XInput�N���X *
 ****************/
class XInput
{
private:
	static CONTROLER_STATE game_pad_[kMaxPad];  // �Q�[���p�b�h�\����

	/* ���͕ۑ��p */
	static ULONG down_button_;

	/* �X�e�B�b�N */
	static ULONG save_push_stick_[kMaxPad];   // �O�̃t���[���m�F�p
	static ULONG push_stick_[kMaxPad];        // ���͕ۑ��p

	/* �{�^�� */
	static ULONG push_button_[kMaxPad];        // �O�̃t���[���m�F�p
	static ULONG save_push_button_[kMaxPad];   // ���͕ۑ��p

public:

	static BOOL Device(VOID);  // �f�o�C�X�̍X�V����

	/* ���͂�Ԃ��֐� */

	/* ���X�e�B�b�N */
	static INT AdditionLeftX(CONST INT player_pad_ = 0);  // X�l
	static INT AdditionLeftY(CONST INT player_pad_ = 0);  // Y�l

	/* �E�X�e�B�b�N */
	static INT AdditionRightX(CONST INT player_pad = 0);  // X�l
	static INT AdditionRightY(CONST INT player_pad = 0);  // Y�l

	/* �{�^�� */
	static INT DownLeftTrigger(CONST INT player_pad_ = 0);
	static INT DownRightTrigger(CONST INT player_pad_ = 0);

	static INT DownButton(INT player_pad = 0) { return game_pad_[player_pad].state_.Gamepad.wButtons; }

	/* ���͂�Ԃ��֐�(�t���[���P��) */
	/* �t���[���P�ʂ̎擾���Ǘ�����֐� */
	static VOID PushStick(INT player_pad = 0);      // 1�񔻒�p
	static VOID PushButton(INT player_pad = 0);

	static ULONG ReturnPushStick(INT player_pad = 0){ return push_stick_[player_pad]; }
	static ULONG ReturnPushButton(INT player_pad = 0) { return push_button_[player_pad]; }
};

#endif