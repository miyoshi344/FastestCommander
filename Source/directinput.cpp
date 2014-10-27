/***********
 * Include *
 ***********/
#include "directinput.h"

/******************
 * �ÓI�����o�ϐ� *
 ******************/
LPDIRECTINPUT8        DInput::input_;  // DirectInput�C���^�[�t�F�C�X

/* �L�[�{�[�h */
LPDIRECTINPUTDEVICE8  DInput::input_device_;        // DirectInputDevice�C���^�[�t�F�C�X�i�L�[�{�[�h�j
BYTE                  DInput::key_buffer_[ 256 ];   // ���̓L�[�o�b�t�@
ULONG                 DInput::save_push_key_ = 0L;  // �O�񉟂��ꂽ�L�[�̋L���r�b�g

/* �R���g���[���[ */
LPDIRECTINPUTDEVICE8  DInput::input_joydevice_;          // DirectInputDevice�C���^�[�t�F�C�X(�Q�[���p�b�h) 
DIJOYSTATE2           DInput::ButtonBuffer;              // �{�^���o�b�t�@
ULONG                 DInput::save_push_button_	= 0L;    // �O�񉟂���Ă����{�^��
BOOL                  DInput::getpad_flag_      = TRUE;  // �R���g���[���[�̎g�p�@�s�q�t�d�Ȃ�g���Ă���

/******************
 * �R���X�g���N�^ *
 ******************/
DInput::DInput( VOID )
{
    input_            = NULL;
    input_device_     = NULL;
	input_joydevice_  = NULL;
}

/****************
 * �f�X�g���N�^ *
 ****************/
DInput::~DInput( VOID )
{
}

/******************************************
 * �֐��� : Init�֐�                      *
 * ��  �e : DirectInput�̏��������s���܂� *
 * ��  �� : �Ȃ�                          *
 * �߂�l : �Ȃ�                          *
 ******************************************/
BOOL DInput::Init( CONST HWND hWnd, CONST HINSTANCE Instance )
{
    BOOL ret = TRUE;    // �߂�l�p

    do
    {
        /* DiurectInput�C���^�[�t�F�C�X�̎擾 */
        if( FAILED( DirectInput8Create( GetModuleHandle( NULL ),  // �C���X�^���X�n���h����Ԃ�
            DIRECTINPUT_VERSION,                                  // Input�̃o�[�W����
            IID_IDirectInput8,                                    // �C���^�[�t�F�C�X�̎��ʎq
            (VOID**)&input_,                                      // �C���^�[�t�F�C�X�|�C���^�̎���
            NULL ) ) )                                            // NULL
        {
            /* �G���[ */
            ErrorDialog( "Input�C���^�[�t�F�C�X�̎擾�Ɏ��s" );
            ret = FALSE;
            break;
        }


		/* �L�[�{�[�h */

        /* �L�[�{�[�h�f�o�C�X�C���^�[�t�F�C�X�̎擾 */
        if( FAILED( input_->CreateDevice( GUID_SysKeyboard, &input_device_, NULL ) ) )
        {
            /* �G���[ */
            ErrorDialog( "�L�[�{�[�h�f�o�C�X�C���^�[�t�F�C�X�̎擾�Ɏ��s" );
            ret = FALSE;
            break;
        }

        /* �f�[�^�`���̐ݒ� */
        if( FAILED( input_device_->SetDataFormat( &c_dfDIKeyboard ) ) )
        {
            /* �G���[ */
            ErrorDialog( "�f�[�^�`���̐ݒ�Ɏ��s" );
            ret = FALSE;
            break;
        }

        /* �������[�h�̐ݒ� */
        if( FAILED( input_device_->SetCooperativeLevel( hWnd,
            DISCL_BACKGROUND | DISCL_NONEXCLUSIVE ) ) )
        {
            /* �G���[ */
            ErrorDialog( "�������x���̐ݒ�Ɏ��s" );
            ret = FALSE;
            break;
        }

        /* �L�[�{�[�h�̓��͎�t�J�n */
        input_device_->Acquire();



		/* �Q�[���p�b�h�f�o�C�X��� */
		if( FAILED( input_->EnumDevices( DI8DEVCLASS_GAMECTRL,
										 EnumJoyCallback,
										 NULL,
										 DIEDFL_ATTACHEDONLY ) ) ||
										 input_joydevice_ == NULL )
		{
			/* �p�b�h�f�o�C�X�̗񋓂Ɏ��s(�R���g���[���[���g�p��) */
			getpad_flag_ = FALSE;
			break;
		}

		/* �R���g���[���[�g�p�� */
		if( getpad_flag_ )
		{
			/* �f�[�^�`���̐ݒ� */
			if( FAILED( input_joydevice_->SetDataFormat( &c_dfDIJoystick2 ) ) )
			{
				/* �G���[ */
				ErrorDialog( "�p�b�h�f�o�C�X�̏������Ɏ��s" );
				ret = FALSE;
				break;
			}
			
			/* �������[�h�̐ݒ� */
			if( FAILED( input_joydevice_->SetCooperativeLevel( hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE ) ) )
			{
				/* �G���[ */
				ErrorDialog( "�������x���̐ݒ�Ɏ��s" );
				SafeRelease( input_joydevice_ );
				input_joydevice_ = NULL;
				ret = FALSE;
				break;
			}

			/* �f�o�C�X�̐ݒ� */
			DIPROPDWORD diprop;
			diprop.diph.dwSize = sizeof( DIPROPDWORD );
			diprop.diph.dwHeaderSize = sizeof( diprop.diph );
			diprop.diph.dwObj = 0;
			diprop.diph.dwHow = DIPH_DEVICE;
			diprop.dwData = DIPROPAXISMODE_ABS;

			/* ���̃��[�h�ݒ� */
			if( FAILED( input_joydevice_->SetProperty( DIPROP_AXISMODE, &diprop.diph ) ) )
			{
				/* �G���[ */
				ErrorDialog( "�����[�h�̐ݒ�Ɏ��s" );
				ret = FALSE;
				break;
			}

			/* ���̒l�͈̔͂̐ݒ� */
			if( FAILED( input_joydevice_->EnumObjects( EnumAxesCallback, NULL, DIDFT_AXIS ) ) )
			{
				/* �G���[ */
				ErrorDialog( "���̗񋓂Ɏ��s" );
				ret = FALSE;
				break;
			}

			/* �R���g���[���[�f�o�C�X�̏����� */
			if( FAILED( input_joydevice_->SetProperty( DIPROP_BUFFERSIZE, &diprop.diph )) )
			{
				/* �G���[ */
				ErrorDialog( "�R���g���[���[�f�o�C�X�̏������Ɏ��s" );
				ret = FALSE;
				break;
			}

			/* �Q�[���p�b�h�̓��͎�t�J�n */
			input_joydevice_->Acquire();

		}

    }while( 0 );

    return ret;
}

/********************
 * �֐��� : Destroy *
 * ��  �e : �j��    *
 * ��  �� : �Ȃ�    *
 * �߂�l : �Ȃ�    *
*********************/
VOID DInput::Destroy( VOID )
{
	/* �L�[�{�[�h�̃f�o�C�X��� */
    input_device_->Unacquire();    // ���͒�~
    SafeRelease( input_device_ );  // �L�[�{�[�h�f�o�C�X�̉��

	/* �R���g���[���[�̃f�o�C�X��� */
	if( getpad_flag_ )
	{
		input_joydevice_->Unacquire();    // ���͒�~
		SafeRelease( input_joydevice_ );  // �R���g���[���[�f�o�C�X�̉��
	}

	/* DirectInput�C���^�[�t�F�C�X�̉�� */
    SafeRelease( input_ );
}

/***********************************************************
 * �֐��� : GetInput�֐�                                   *
 * ��  �e : �Q�[���p�b�h�܂��̓L�[�{�[�h�̏�Ԃ��擾���܂� *
 * ��  �� : �Ȃ�                                           *
 * �߂�l : �Ȃ�                                           *
 ***********************************************************/
VOID DInput::GetInput( VOID )
{
	if( getpad_flag_ )
	{
		/* �{�^���̓��͏�Ԃ𓾂� */
		if( FAILED( input_joydevice_->GetDeviceState( sizeof( DIJOYSTATE2 ),(LPVOID)&ButtonBuffer ) ) )
		{
			/* �ēx���͎�t */
			input_joydevice_->Acquire();
		}
	}

    /* �L�[�{�[�h�̓��͏�Ԃ𓾂� */
    if( FAILED(input_device_->GetDeviceState( sizeof(key_buffer_), (LPVOID)&key_buffer_ ) ) )
	{ 
		/* �ēx�A���͊J�n */
		input_device_->Acquire();
	}
}

/*********************************
 * �֐��� : GetDownKey�֐�       *
 * ��  �e : �L�[�{�[�h���͂̎擾 *
 * ��  �� : �Ȃ�                 *
 * �߂�l : �Ȃ�                 *
 *********************************/
ULONG DInput::GetDownKey( VOID )
{
	ULONG GetKey = 0L;	// �擾�L�[�i�[�p

	/* �L�[���͔��� */
	if( PUSHKEY( key_buffer_, DIK_UP     ) )  GetKey |= UP_KEY;     // ��L�[
	if( PUSHKEY( key_buffer_, DIK_DOWN   ) )  GetKey |= DOWN_KEY;   // ���L�[
	if( PUSHKEY( key_buffer_, DIK_LEFT   ) )  GetKey |= LEFT_KEY;   // ���L�[
	if( PUSHKEY( key_buffer_, DIK_RIGHT  ) )  GetKey |= RIGHT_KEY;  // �E�L�[
	if( PUSHKEY( key_buffer_, DIK_SPACE  ) )  GetKey |= SPACE_KEY;  // �X�y�[�X�L�[
	if( PUSHKEY( key_buffer_, DIK_RETURN ) )  GetKey |= ENTER_KEY;  // �G���^�[�L�[
	if( PUSHKEY( key_buffer_, DIK_1      ) )  GetKey |= KEY_1;      // 1�L�[
	if( PUSHKEY( key_buffer_, DIK_1      ) )  GetKey |= KEY_2;      // 2�L�[
	if( PUSHKEY( key_buffer_, DIK_1      ) )  GetKey |= KEY_3;      // 3�L�[
	if( PUSHKEY( key_buffer_, DIK_1      ) )  GetKey |= KEY_4;      // 4�L�[
	if( PUSHKEY( key_buffer_, DIK_1      ) )  GetKey |= KEY_5;      // 5�L�[
	if( PUSHKEY( key_buffer_, DIK_Z      ) )  GetKey |= Z_KEY;      // Z�L�[ 

	return GetKey;
}

/******************************************
 * �֐��� : GetPushKey�֐�                *
 * ��  �e : �L�[�{�[�h��1��ł̓��͂̎擾 *
 * ��  �� : �Ȃ�                          *
 * �߂�l : �����ꂽ�L�[                  *
 ******************************************/
ULONG DInput::GetPushKey( VOID )
{
	ULONG key = 0L;	// �L�[���̓r�b�g

	/* �J�[�\���L�[�� */
	if( PUSHKEY(key_buffer_, DIK_UP) )
	{
		/* �O�񉟂���Ă��Ȃ���΃t���O���グ�� */
		if( !(save_push_key_ & UP_KEY) )
		{
			key            |= UP_KEY;  // �L�[���̓r�b�g
			save_push_key_ |= UP_KEY;  // �L���p�r�b�g
		}
	}
	else save_push_key_ &= ~UP_KEY;    // �L���p�r�b�g��������

	/* �J�[�\���L�[�E */
	if( PUSHKEY(key_buffer_, DIK_RIGHT) )
	{
		/* �O�񉟂���Ă��Ȃ���΃t���O���グ�� */
		if( !(save_push_key_ & RIGHT_KEY) )
		{
			key            |= RIGHT_KEY;  // �L�[���̓r�b�g
			save_push_key_ |= RIGHT_KEY;  // �L���p�r�b�g
		}
	}
	else save_push_key_ &= ~RIGHT_KEY;    // �L���p�r�b�g��������

	/* �J�[�\���L�[�� */
	if( PUSHKEY(key_buffer_, DIK_DOWN) )
	{
		/* �O�񉟂���Ă��Ȃ���΃t���O���グ�� */
		if( !(save_push_key_ & DOWN_KEY) )
		{
			key            |= DOWN_KEY;  // �L�[���̓r�b�g
			save_push_key_ |= DOWN_KEY;  // �L���p�r�b�g
		}
	}
	else save_push_key_ &= ~DOWN_KEY;    // �L���p�r�b�g��������

	/* �J�[�\���L�[�� */
	if( PUSHKEY(key_buffer_, DIK_LEFT) )
	{
		/* �O�񉟂���Ă��Ȃ���΃t���O���グ�� */
		if( !(save_push_key_ & LEFT_KEY) )
		{
			key            |= LEFT_KEY;  // �L�[���̓r�b�g
			save_push_key_ |= LEFT_KEY;  // �L���p�r�b�g
		}
	}
	else save_push_key_ &= ~LEFT_KEY;    // �L���p�r�b�g��������

	/* �X�y�[�X�L�[ */
	if( PUSHKEY(key_buffer_, DIK_SPACE) )
	{
		/* �O�񉟂���Ă��Ȃ���΃t���O���グ�� */
		if( !(save_push_key_ & SPACE_KEY) )
		{
			key            |= SPACE_KEY;  // �L�[���̓r�b�g
			save_push_key_ |= SPACE_KEY;  // �L���p�r�b�g
		}
	}
	else save_push_key_ &= ~SPACE_KEY;	  // �L���p�r�b�g��������

	/* Enter�L�[ */
	if( PUSHKEY(key_buffer_, DIK_RETURN) )
	{
		/* �O�񉟂���Ă��Ȃ���΃t���O���グ�� */
		if( !(save_push_key_ & ENTER_KEY) )
		{
			key            |= ENTER_KEY;  // �L�[���̓r�b�g
			save_push_key_ |= ENTER_KEY;  // �L���p�r�b�g
		}
	}
	else save_push_key_ &= ~ENTER_KEY;	  // �L���p�r�b�g��������

	/* 1�L�[ */
	if( PUSHKEY(key_buffer_, DIK_1) )
	{
		/* �O�񉟂���Ă��Ȃ���΃t���O���グ�� */
		if (!(save_push_key_ & KEY_1))
		{
			key            |= KEY_1;  // �L�[���̓r�b�g
			save_push_key_ |= KEY_1;  // �L���p�r�b�g
		}
	}
	else save_push_key_ &= ~KEY_1;	  // �L���p�r�b�g��������

	/* 2�L�[ */
	if (PUSHKEY(key_buffer_, DIK_2) )
	{
		/* �O�񉟂���Ă��Ȃ���΃t���O���グ�� */
		if (!(save_push_key_ & KEY_2))
		{
			key            |= KEY_2;  // �L�[���̓r�b�g
			save_push_key_ |= KEY_2;  // �L���p�r�b�g
		}
	}
	else save_push_key_ &= ~KEY_2;	  // �L���p�r�b�g��������

	/* 3�L�[ */
	if (PUSHKEY(key_buffer_, DIK_3) )
	{
		/* �O�񉟂���Ă��Ȃ���΃t���O���グ�� */
		if (!(save_push_key_ & KEY_3))
		{
			key            |= KEY_3;  // �L�[���̓r�b�g
			save_push_key_ |= KEY_3;  // �L���p�r�b�g
		}
	}
	else save_push_key_ &= ~KEY_3;	  // �L���p�r�b�g��������

	/* 4�L�[ */
	if (PUSHKEY(key_buffer_, DIK_4))
	{
		/* �O�񉟂���Ă��Ȃ���΃t���O���グ�� */
		if (!(save_push_key_ & KEY_4) )
		{
			key            |= KEY_4;  // �L�[���̓r�b�g
			save_push_key_ |= KEY_4;  // �L���p�r�b�g
		}
	}
	else save_push_key_ &= ~KEY_4;	  // �L���p�r�b�g��������

	/* 5�L�[ */
	if (PUSHKEY(key_buffer_, DIK_5) )
	{
		/* �O�񉟂���Ă��Ȃ���΃t���O���グ�� */
		if (!(save_push_key_ & KEY_5))
		{
			key            |= KEY_5;  // �L�[���̓r�b�g
			save_push_key_ |= KEY_5;  // �L���p�r�b�g
		}
	}
	else save_push_key_ &= ~KEY_5;	  // �L���p�r�b�g��������

	/* Z�L�[ */
	if (PUSHKEY(key_buffer_, DIK_Z))
	{
		/* �O�񉟂���Ă��Ȃ���΃t���O���グ�� */
		if (!(save_push_key_ & Z_KEY))
		{
			key            |= Z_KEY;  // �L�[���̓r�b�g
			save_push_key_ |= Z_KEY;  // �L���p�r�b�g
		}
	}
	else save_push_key_ &= ~Z_KEY;    // �L���p�r�b�g��������

	return key;
}

/*****************************************
 * �֐��� : GetDownButton�֐�            *
 * ��  �e : �Q�[���p�b�h���͂̎擾���܂� *
 * ��  �� : �Ȃ�                         *
 * �߂�l : ������Ă���{�^��           *
 *****************************************/
ULONG DInput::GetDownButton( VOID )
{
	ULONG button = 0L;   // �{�^�����̓r�b�g 

	/* �L�[���͔��� */
	if( ButtonBuffer.lY < -STROKE_MARGIN ) button |= JS_UP;        // �X�e�B�b�N��
	if( ButtonBuffer.lX > STROKE_MARGIN )  button |= JS_RIGHT;     // �X�e�B�b�N�E
	if( ButtonBuffer.lY > STROKE_MARGIN )  button |= JS_DOWN;      // �X�e�B�b�N��
	if( ButtonBuffer.lX < -STROKE_MARGIN ) button |= JS_LEFT;      // �X�e�B�b�N��
	if( ButtonBuffer.rgbButtons[ 0 ] )     button |= JS_BUTTONA;   // �{�^��A
	if( ButtonBuffer.rgbButtons[ 1 ] )     button |= JS_BUTTONB;   // �{�^��B
	if( ButtonBuffer.rgbButtons[ 2 ] )     button |= JS_BUTTONX;   // �{�^��X
	if( ButtonBuffer.rgbButtons[ 3 ] )     button |= JS_BUTTONY;   // �{�^��Y
	if( ButtonBuffer.rgbButtons[ 4 ] )     button |= JS_BUTTONLB;  // �{�^��LB
	if( ButtonBuffer.rgbButtons[ 5 ] )     button |= JS_BUTTONRB;  // �{�^��RB
	if( ButtonBuffer.rgbButtons[ 6 ] )     button |= JS_BUTTONSL;  // �Z���N�g�{�^��
	if( ButtonBuffer.rgbButtons[ 7 ] )     button |= JS_BUTTONST;  // �X�^�[�g�{�^��
	if( ButtonBuffer.rgbButtons[ 8 ] )     button |= JS_BUTTONA;   // ���X�e�B�b�N��������
	if( ButtonBuffer.rgbButtons[ 9 ] )     button |= JS_BUTTONA;   // �E�X�e�B�b�N��������

	return button;
}

/*************************************
 * �֐��� : GetPushButton�֐�        *
 * ��  �e : �{�^�����͂̎擾�����܂� *
 * ��  �� : �Ȃ�                     *
 * �߂�l : ������Ă����{�^��       *
 *************************************/
ULONG DInput::GetPushButton( VOID )
{
	ULONG button = 0L;   // �L�[���̓r�b�g
	
	if( getpad_flag_ )
	{
		/* �R���g���[���[�� */
		if( ButtonBuffer.lY < -STROKE_MARGIN )
		{
			/* �O�񉟂���Ă��Ȃ���΃t���O���グ�� */
			if( !( save_push_button_ & JS_UP ) )
			{
				button            |= JS_UP;  // �{�^�����̓r�b�g
				save_push_button_ |= JS_UP;  // �L���p�r�b�g
			}
		}
		else save_push_button_ &= ~JS_UP;    // �L���p�r�b�g��������

		/* �R���g���[���[�E */
		if( ButtonBuffer.lX < STROKE_MARGIN )
		{
			/* �O�񉟂���Ă��Ȃ���΃t���O���グ�� */
			if( !( save_push_button_ & JS_RIGHT ) )
			{
                button            |= JS_RIGHT;  // �{�^�����̓r�b�g
				save_push_button_ |= JS_RIGHT;  // �L���p�r�b�g
			}
		}
		else save_push_button_ &= ~JS_RIGHT;    // �L���p�r�b�g��������

		/* �R���g���[���[�� */
		if( ButtonBuffer.lY < STROKE_MARGIN )
		{
			/* �O�񉟂���Ă��Ȃ���΃t���O���グ�� */
			if( !( save_push_button_ & JS_DOWN ) )
			{
				button            |= JS_DOWN;  // �{�^�����̓r�b�g
				save_push_button_ |= JS_DOWN;  // �L���p�r�b�g
			}
		}
		else save_push_button_ &= ~JS_DOWN;    // �L���p�r�b�g��������
		
		/* �R���g���[���[�� */
		if( ButtonBuffer.lX < -STROKE_MARGIN )
		{
			/* �O�񉟂���Ă��Ȃ���΃t���O���グ�� */
			if( !( save_push_button_ & JS_LEFT ) )
			{
				button            |= JS_LEFT;  // �{�^�����̓r�b�g
				save_push_button_ |= JS_LEFT;  // �L���p�r�b�g
			}
		}
		else save_push_button_ &= ~JS_LEFT;    // �L���p�r�b�g��������

		/* �{�^��A */
		if( ButtonBuffer.rgbButtons[ 0 ] )
		{
			/* �O�񉟂���Ă��Ȃ���΃t���O���グ�� */
			if( !( save_push_button_ & JS_BUTTONA ) )
			{
				button            |= JS_BUTTONA;  // �{�^�����̓r�b�g
				save_push_button_ |= JS_BUTTONA;  // �L���p�r�b�g
			}
		}
		else save_push_button_ &= ~JS_BUTTONA;    // �L���p�r�b�g��������

		/* �{�^��B */
		if( ButtonBuffer.rgbButtons[ 1 ] )
		{
			/* �O�񉟂���Ă��Ȃ���΃t���O���グ�� */
			if( !( save_push_button_ & JS_BUTTONB ) )
			{
				button            |= JS_BUTTONB;  // �{�^�����̓r�b�g
				save_push_button_ |= JS_BUTTONB;  // �L���p�r�b�g
			}
		}
		else save_push_button_ &= ~JS_BUTTONB;    // �L���p�r�b�g��������
		
		/* �{�^��X  */
		if( ButtonBuffer.rgbButtons[ 2 ] )
		{
			/* �O�񉟂���Ă��Ȃ���΃t���O���グ�� */
			if( !( save_push_button_ & JS_BUTTONX ) )
			{
				button            |= JS_BUTTONX;  // �{�^�����̓r�b�g
				save_push_button_ |= JS_BUTTONX;  // �L���p�r�b�g
			}
		}
		else save_push_button_ &= ~JS_BUTTONX;    // �L���p�r�b�g��������

		/* �{�^��Y */
		if( ButtonBuffer.rgbButtons[ 3 ] )
		{
			/* �O�񉟂���Ă��Ȃ���΃t���O���グ�� */
			if( !( save_push_button_ & JS_BUTTONY ) )
			{
				button            |= JS_BUTTONY;  // �{�^�����̓r�b�g
				save_push_button_ |= JS_BUTTONY;  // �L���p�r�b�g
			}
		}
		else save_push_button_ &= ~JS_BUTTONY;    // �L���p�r�b�g��������

		/* �{�^��LB */
		if (ButtonBuffer.rgbButtons[4])
		{
			/* �O�񉟂���Ă��Ȃ���΃t���O���グ�� */
			if (!(save_push_button_ & JS_BUTTONLB))
			{
				button            |= JS_BUTTONLB;  // �{�^�����̓r�b�g
				save_push_button_ |= JS_BUTTONLB;  // �L���p�r�b�g
			}
		}
		else save_push_button_ &= ~JS_BUTTONLB;    // �L���p�r�b�g��������

		/* �{�^��RB */
		if (ButtonBuffer.rgbButtons[5])
		{
			/* �O�񉟂���Ă��Ȃ���΃t���O���グ�� */
			if (!(save_push_button_ & JS_BUTTONRB))
			{
				button            |= JS_BUTTONRB;  // �{�^�����̓r�b�g
				save_push_button_ |= JS_BUTTONRB;  // �L���p�r�b�g
			}
		}
		else save_push_button_ &= ~JS_BUTTONRB;    // �L���p�r�b�g��������

		/* �Z���N�g�{�^�� */
		if( ButtonBuffer.rgbButtons[ 6 ] )
		{
			/* �O�񉟂���Ă��Ȃ���΃t���O���グ�� */
			if( !( save_push_button_ & JS_BUTTONSL ) )
			{
				button            |= JS_BUTTONSL;  // �{�^�����̓r�b�g
				save_push_button_ |= JS_BUTTONSL;  // �L���p�r�b�g
			}
		}
		else save_push_button_ &= ~JS_BUTTONSL;    // �L���p�r�b�g��������
		
		/* �X�^�[�g�{�^�� */
		if( ButtonBuffer.rgbButtons[ 7 ] )
		{
			/* �O�񉟂���Ă��Ȃ���΃t���O���グ�� */
			if( !( save_push_button_ & JS_BUTTONST ) )
			{
				button            |= JS_BUTTONST;  // �{�^�����̓r�b�g
				save_push_button_ |= JS_BUTTONST;  // �L���p�r�b�g
			}
		}
		else save_push_button_ &= ~JS_BUTTONST;    // �L���p�r�b�g��������
	}

		
	return button;
}

/*************************************************************
 * �֐��� : EnumJoyCallback�֐�                              *
 * ��  �e : �Q�[���p�b�h��񋓂���R�[���o�b�N�֐���Ԃ��܂� *
 * ��  �� : DIDEVICEINSTANCE* pdidInstance                   *
 *			VOID* pContext                                   *
 * �߂�l�F����                                              *
**************************************************************/
BOOL CALLBACK DInput::EnumJoyCallback( const DIDEVICEINSTANCE* pdidInstance, VOID* pContext )
{
	BOOL ret = DIENUM_STOP;

	static GUID pad_discrimination;

	do
	{
		/* ���Ɏ擾�ς݂̃f�o�C�X�������ꍇ�͂��������s���� */
		if( pad_discrimination == pdidInstance->guidInstance )
		{
			return DIENUM_CONTINUE;
		}

		/* �񋓂��ꂽ�p�b�h�ւ̃C���^�[�t�F�C�X���擾���� */
		if( FAILED( input_->CreateDevice( pdidInstance->guidInstance, &input_joydevice_, NULL ) ) )
		{
			ErrorDialog( "�p�b�h�̃C���^�[�t�F�C�X�̎擾�Ɏ��s" );
			ret = FALSE;
			break;
		}
		
		
		/* �p�b�h�̔\�͂𒲂ׂ� */
		DIDEVCAPS diDevCaps;

		/* ������ */
		ZeroMemory( &diDevCaps, sizeof( DIDEVCAPS ) );

		diDevCaps.dwSize = sizeof( DIDEVCAPS );

		if( FAILED( input_joydevice_->GetCapabilities( &diDevCaps ) ) )
		{
			ErrorDialog( "�p�b�h�̔\�͎擾�Ɏ��s" );
			SafeRelease( input_joydevice_ );
			input_joydevice_ = NULL;
			ret = DIENUM_CONTINUE;	// (�񋓂𑱂���)
			break;
		}
		
		pad_discrimination = pdidInstance->guidInstance;

	}while( 0 );

	return ret;
}

/***********************************************************
 * �֐��� : EnumAxesCallback�֐�                           *
 * ��  �e : �����ƂɌĂяo�����R�[���o�b�N�֐���Ԃ��܂� *
 * ��  �� : DIDEVICEINSTANCE* Lpddoi                       *
 *			VOID* pVRef                                    *
 * �߂�l�F����                                            *
 ***********************************************************/
BOOL CALLBACK DInput::EnumAxesCallback( LPCDIDEVICEOBJECTINSTANCE Lpddoi, LPVOID pVRef )
{
	BOOL ret = DIENUM_CONTINUE;   // �߂�l

	DIPROPRANGE diprg;
	ZeroMemory( &diprg, sizeof(diprg) );
	diprg.diph.dwSize = sizeof(diprg);
	diprg.diph.dwHeaderSize = sizeof(diprg.diph);
	diprg.diph.dwObj  = Lpddoi->dwType;
	diprg.diph.dwHow  = DIPH_BYID;
	diprg.lMin        = STROKE_MIN;
	diprg.lMax        = STROKE_MAX;

	if( FAILED(input_joydevice_->SetProperty( DIPROP_RANGE, &diprg.diph ) ) )
	{
		/* �G���[ */
		ret = DIENUM_STOP;
	}

	return ret;
}