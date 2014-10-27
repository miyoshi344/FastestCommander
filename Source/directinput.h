/*************************
 * DirectInput����N���X *
 *************************/

/* ���dInclude�h�~ */
#ifndef RACE_DINPUT_H_
#define RACE_DINPUT_H_

/***********
 * Include *
 ***********/
#include "common.h"
#include "release.h"

/***********************
 * �����萔/�}�N����` *
 ***********************/
#define	PUSHKEY(name, key)	(name[ key ] & 0x80)	// �L�[���͗p

/* �L�[�{�[�h */
CONST ULONG	UP_KEY		= 1;                // �J�[�\���L�[��
CONST ULONG	RIGHT_KEY	= UP_KEY     << 1;	// �J�[�\���L�[�E
CONST ULONG	DOWN_KEY	= RIGHT_KEY  << 1;	// �J�[�\���L�[��
CONST ULONG	LEFT_KEY	= DOWN_KEY   << 1;	// �J�[�\���L�[��
CONST ULONG	SPACE_KEY	= LEFT_KEY   << 1;	// �X�y�[�X�L�[
CONST ULONG	ENTER_KEY	= SPACE_KEY  << 1;	// �G���^�[�L�[
CONST ULONG KEY_1       = ENTER_KEY  << 1;  // 1�L�[
CONST ULONG KEY_2       = KEY_1      << 1;  // 2�L�[
CONST ULONG KEY_3       = KEY_2      << 1;  // 3�L�[
CONST ULONG KEY_4       = KEY_3      << 1;  // 4�L�[
CONST ULONG KEY_5       = KEY_4      << 1;  // 5�L�[
CONST ULONG Z_KEY       = KEY_5      << 1;  // Z�L�[

/* �ʏ�p�b�h�z�u */	
CONST ULONG	JS_UP        = 1;                  // �Q�[���R���g���[����
CONST ULONG	JS_RIGHT     = JS_UP        << 1;  // �Q�[���R���g���[���E
CONST ULONG	JS_DOWN      = JS_RIGHT     << 1;  // �Q�[���R���g���[����
CONST ULONG	JS_LEFT      = JS_DOWN      << 1;  // �Q�[���R���g���[����
CONST ULONG	JS_BUTTONA   = JS_LEFT      << 1;  // �Q�[���R���g���[��A
CONST ULONG	JS_BUTTONB   = JS_BUTTONA   << 1;  // �Q�[���R���g���[��B
CONST ULONG	JS_BUTTONX   = JS_BUTTONB   << 1;  // �Q�[���R���g���[��X
CONST ULONG	JS_BUTTONY   = JS_BUTTONX   << 1;  // �Q�[���R���g���[��Y
CONST ULONG JS_BUTTONLB	 = JS_BUTTONY   << 1;  // �Q�[���R���g���[���[LB
CONST ULONG JS_BUTTONRB	 = JS_BUTTONLB  << 1;  // �Q�[���R���g���[���[RB
CONST ULONG JS_BUTTONST  = JS_BUTTONRB  << 1;  // �X�^�[�g�{�^��
CONST ULONG	JS_BUTTONSL	 = JS_BUTTONST  << 1;  // �Z���N�g�{�^��
CONST ULONG	JS_BUTTONLF	 = JS_BUTTONSL  << 1;  // ���X�e�B�b�N��������
CONST ULONG JS_BUTTONRF  = JS_BUTTONLF  << 1;  // �E�X�e�B�b�N��������

/* �X�e�B�b�N�p�萔 */
CONST INT STROKE_MIN     = -1000;  // ���ŏ��l
CONST INT STROKE_MAX     = 1000;   // ���ő�l
CONST INT STROKE_MARGIN  = 300;    // ���̗V�ђl

/***********************************
 * DirectInput�𐧌䂷��N���X�ł� *
 ***********************************/
class DInput
{
private:

    /* ���� */
    static LPDIRECTINPUT8  input_;  // DirectInput�C���^�[�t�F�C�X

	/* �L�[�{�[�h�p */
	static LPDIRECTINPUTDEVICE8  input_device_;       // DirectInputDevice�C���^�[�t�F�C�X�i�L�[�{�[�h�j
	static ULONG                 save_push_key_;      // �O�񉟂���Ă����{�^��(�L�[�{�[�h)
    static BYTE                  key_buffer_[ 256 ];  // ���̓L�[�o�b�t�@

	/* �p�b�h�p */
	static LPDIRECTINPUTDEVICE8  input_joydevice_;   // DirectInpuDeivice�C���^�[�t�F�C�X(�W���C�X�e�B�b�N)
	static ULONG                 save_push_button_;	 // �O�񉟂���Ă����{�^��(�W���C�X�e�B�b�N)
	static DIJOYSTATE2           ButtonBuffer;       // �{�^���o�b�t�@(�W���C�X�e�B�b�N)
	static BOOL                  getpad_flag_;       // �R���g���[���[�̎g�p

public:
	/* ���� */
    DInput( VOID );   // �R���X�g���N�^
    ~DInput( VOID );  // �f�X�g���N�^
	static BOOL Init( CONST HWND hWnd, CONST HINSTANCE Instance );  // ������
	static VOID  GetInput( VOID );  // ���͂̎擾
    static VOID  Destroy( VOID );   // �j��

	/* �L�[�{�[�h�p */
    static ULONG  GetDownKey( VOID );  // ������Ă���L�[�̎擾
	static ULONG  GetPushKey( VOID );  // �����ꂽ�L�[�̎擾

	/* �p�b�h�p */
	static ULONG  GetDownButton( VOID );  // ������Ă���{�^���̎擾
	static ULONG  GetPushButton( VOID );  // �����ꂽ�{�^���̎擾

	static BOOL   Get_PadFlag( VOID ) { return getpad_flag_; }  // �R���g���[���[���擾�������`�F�b�N

	/* �R�[���o�b�N�֐� */
	static BOOL	CALLBACK EnumJoyCallback( CONST DIDEVICEINSTANCE* pdidInstance, VOID* pContext );
	static BOOL	CALLBACK EnumAxesCallback( LPCDIDEVICEOBJECTINSTANCE Lpddoi, LPVOID pVRef );

};

#endif