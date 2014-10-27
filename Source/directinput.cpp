/***********
 * Include *
 ***********/
#include "directinput.h"

/******************
 * 静的メンバ変数 *
 ******************/
LPDIRECTINPUT8        DInput::input_;  // DirectInputインターフェイス

/* キーボード */
LPDIRECTINPUTDEVICE8  DInput::input_device_;        // DirectInputDeviceインターフェイス（キーボード）
BYTE                  DInput::key_buffer_[ 256 ];   // 入力キーバッファ
ULONG                 DInput::save_push_key_ = 0L;  // 前回押されたキーの記憶ビット

/* コントローラー */
LPDIRECTINPUTDEVICE8  DInput::input_joydevice_;          // DirectInputDeviceインターフェイス(ゲームパッド) 
DIJOYSTATE2           DInput::ButtonBuffer;              // ボタンバッファ
ULONG                 DInput::save_push_button_	= 0L;    // 前回押されていたボタン
BOOL                  DInput::getpad_flag_      = TRUE;  // コントローラーの使用　ＴＲＵＥなら使っている

/******************
 * コンストラクタ *
 ******************/
DInput::DInput( VOID )
{
    input_            = NULL;
    input_device_     = NULL;
	input_joydevice_  = NULL;
}

/****************
 * デストラクタ *
 ****************/
DInput::~DInput( VOID )
{
}

/******************************************
 * 関数名 : Init関数                      *
 * 内  容 : DirectInputの初期化を行います *
 * 引  数 : なし                          *
 * 戻り値 : なし                          *
 ******************************************/
BOOL DInput::Init( CONST HWND hWnd, CONST HINSTANCE Instance )
{
    BOOL ret = TRUE;    // 戻り値用

    do
    {
        /* DiurectInputインターフェイスの取得 */
        if( FAILED( DirectInput8Create( GetModuleHandle( NULL ),  // インスタンスハンドルを返す
            DIRECTINPUT_VERSION,                                  // Inputのバージョン
            IID_IDirectInput8,                                    // インターフェイスの識別子
            (VOID**)&input_,                                      // インターフェイスポインタの受取先
            NULL ) ) )                                            // NULL
        {
            /* エラー */
            ErrorDialog( "Inputインターフェイスの取得に失敗" );
            ret = FALSE;
            break;
        }


		/* キーボード */

        /* キーボードデバイスインターフェイスの取得 */
        if( FAILED( input_->CreateDevice( GUID_SysKeyboard, &input_device_, NULL ) ) )
        {
            /* エラー */
            ErrorDialog( "キーボードデバイスインターフェイスの取得に失敗" );
            ret = FALSE;
            break;
        }

        /* データ形式の設定 */
        if( FAILED( input_device_->SetDataFormat( &c_dfDIKeyboard ) ) )
        {
            /* エラー */
            ErrorDialog( "データ形式の設定に失敗" );
            ret = FALSE;
            break;
        }

        /* 強調モードの設定 */
        if( FAILED( input_device_->SetCooperativeLevel( hWnd,
            DISCL_BACKGROUND | DISCL_NONEXCLUSIVE ) ) )
        {
            /* エラー */
            ErrorDialog( "強調レベルの設定に失敗" );
            ret = FALSE;
            break;
        }

        /* キーボードの入力受付開始 */
        input_device_->Acquire();



		/* ゲームパッドデバイスを列挙 */
		if( FAILED( input_->EnumDevices( DI8DEVCLASS_GAMECTRL,
										 EnumJoyCallback,
										 NULL,
										 DIEDFL_ATTACHEDONLY ) ) ||
										 input_joydevice_ == NULL )
		{
			/* パッドデバイスの列挙に失敗(コントローラー未使用時) */
			getpad_flag_ = FALSE;
			break;
		}

		/* コントローラー使用時 */
		if( getpad_flag_ )
		{
			/* データ形式の設定 */
			if( FAILED( input_joydevice_->SetDataFormat( &c_dfDIJoystick2 ) ) )
			{
				/* エラー */
				ErrorDialog( "パッドデバイスの初期化に失敗" );
				ret = FALSE;
				break;
			}
			
			/* 強調モードの設定 */
			if( FAILED( input_joydevice_->SetCooperativeLevel( hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE ) ) )
			{
				/* エラー */
				ErrorDialog( "強調レベルの設定に失敗" );
				SafeRelease( input_joydevice_ );
				input_joydevice_ = NULL;
				ret = FALSE;
				break;
			}

			/* デバイスの設定 */
			DIPROPDWORD diprop;
			diprop.diph.dwSize = sizeof( DIPROPDWORD );
			diprop.diph.dwHeaderSize = sizeof( diprop.diph );
			diprop.diph.dwObj = 0;
			diprop.diph.dwHow = DIPH_DEVICE;
			diprop.dwData = DIPROPAXISMODE_ABS;

			/* 軸のモード設定 */
			if( FAILED( input_joydevice_->SetProperty( DIPROP_AXISMODE, &diprop.diph ) ) )
			{
				/* エラー */
				ErrorDialog( "軸モードの設定に失敗" );
				ret = FALSE;
				break;
			}

			/* 軸の値の範囲の設定 */
			if( FAILED( input_joydevice_->EnumObjects( EnumAxesCallback, NULL, DIDFT_AXIS ) ) )
			{
				/* エラー */
				ErrorDialog( "軸の列挙に失敗" );
				ret = FALSE;
				break;
			}

			/* コントローラーデバイスの初期化 */
			if( FAILED( input_joydevice_->SetProperty( DIPROP_BUFFERSIZE, &diprop.diph )) )
			{
				/* エラー */
				ErrorDialog( "コントローラーデバイスの初期化に失敗" );
				ret = FALSE;
				break;
			}

			/* ゲームパッドの入力受付開始 */
			input_joydevice_->Acquire();

		}

    }while( 0 );

    return ret;
}

/********************
 * 関数名 : Destroy *
 * 内  容 : 破棄    *
 * 引  数 : なし    *
 * 戻り値 : なし    *
*********************/
VOID DInput::Destroy( VOID )
{
	/* キーボードのデバイス解放 */
    input_device_->Unacquire();    // 入力停止
    SafeRelease( input_device_ );  // キーボードデバイスの解放

	/* コントローラーのデバイス解放 */
	if( getpad_flag_ )
	{
		input_joydevice_->Unacquire();    // 入力停止
		SafeRelease( input_joydevice_ );  // コントローラーデバイスの解放
	}

	/* DirectInputインターフェイスの解放 */
    SafeRelease( input_ );
}

/***********************************************************
 * 関数名 : GetInput関数                                   *
 * 内  容 : ゲームパッドまたはキーボードの状態を取得します *
 * 引  数 : なし                                           *
 * 戻り値 : なし                                           *
 ***********************************************************/
VOID DInput::GetInput( VOID )
{
	if( getpad_flag_ )
	{
		/* ボタンの入力状態を得る */
		if( FAILED( input_joydevice_->GetDeviceState( sizeof( DIJOYSTATE2 ),(LPVOID)&ButtonBuffer ) ) )
		{
			/* 再度入力受付 */
			input_joydevice_->Acquire();
		}
	}

    /* キーボードの入力状態を得る */
    if( FAILED(input_device_->GetDeviceState( sizeof(key_buffer_), (LPVOID)&key_buffer_ ) ) )
	{ 
		/* 再度、入力開始 */
		input_device_->Acquire();
	}
}

/*********************************
 * 関数名 : GetDownKey関数       *
 * 内  容 : キーボード入力の取得 *
 * 引  数 : なし                 *
 * 戻り値 : なし                 *
 *********************************/
ULONG DInput::GetDownKey( VOID )
{
	ULONG GetKey = 0L;	// 取得キー格納用

	/* キー入力判定 */
	if( PUSHKEY( key_buffer_, DIK_UP     ) )  GetKey |= UP_KEY;     // 上キー
	if( PUSHKEY( key_buffer_, DIK_DOWN   ) )  GetKey |= DOWN_KEY;   // 下キー
	if( PUSHKEY( key_buffer_, DIK_LEFT   ) )  GetKey |= LEFT_KEY;   // 左キー
	if( PUSHKEY( key_buffer_, DIK_RIGHT  ) )  GetKey |= RIGHT_KEY;  // 右キー
	if( PUSHKEY( key_buffer_, DIK_SPACE  ) )  GetKey |= SPACE_KEY;  // スペースキー
	if( PUSHKEY( key_buffer_, DIK_RETURN ) )  GetKey |= ENTER_KEY;  // エンターキー
	if( PUSHKEY( key_buffer_, DIK_1      ) )  GetKey |= KEY_1;      // 1キー
	if( PUSHKEY( key_buffer_, DIK_1      ) )  GetKey |= KEY_2;      // 2キー
	if( PUSHKEY( key_buffer_, DIK_1      ) )  GetKey |= KEY_3;      // 3キー
	if( PUSHKEY( key_buffer_, DIK_1      ) )  GetKey |= KEY_4;      // 4キー
	if( PUSHKEY( key_buffer_, DIK_1      ) )  GetKey |= KEY_5;      // 5キー
	if( PUSHKEY( key_buffer_, DIK_Z      ) )  GetKey |= Z_KEY;      // Zキー 

	return GetKey;
}

/******************************************
 * 関数名 : GetPushKey関数                *
 * 内  容 : キーボードの1回での入力の取得 *
 * 引  数 : なし                          *
 * 戻り値 : 押されたキー                  *
 ******************************************/
ULONG DInput::GetPushKey( VOID )
{
	ULONG key = 0L;	// キー入力ビット

	/* カーソルキー上 */
	if( PUSHKEY(key_buffer_, DIK_UP) )
	{
		/* 前回押されていなければフラグを上げる */
		if( !(save_push_key_ & UP_KEY) )
		{
			key            |= UP_KEY;  // キー入力ビット
			save_push_key_ |= UP_KEY;  // 記憶用ビット
		}
	}
	else save_push_key_ &= ~UP_KEY;    // 記憶用ビットを下げる

	/* カーソルキー右 */
	if( PUSHKEY(key_buffer_, DIK_RIGHT) )
	{
		/* 前回押されていなければフラグを上げる */
		if( !(save_push_key_ & RIGHT_KEY) )
		{
			key            |= RIGHT_KEY;  // キー入力ビット
			save_push_key_ |= RIGHT_KEY;  // 記憶用ビット
		}
	}
	else save_push_key_ &= ~RIGHT_KEY;    // 記憶用ビットを下げる

	/* カーソルキー下 */
	if( PUSHKEY(key_buffer_, DIK_DOWN) )
	{
		/* 前回押されていなければフラグを上げる */
		if( !(save_push_key_ & DOWN_KEY) )
		{
			key            |= DOWN_KEY;  // キー入力ビット
			save_push_key_ |= DOWN_KEY;  // 記憶用ビット
		}
	}
	else save_push_key_ &= ~DOWN_KEY;    // 記憶用ビットを下げる

	/* カーソルキー左 */
	if( PUSHKEY(key_buffer_, DIK_LEFT) )
	{
		/* 前回押されていなければフラグを上げる */
		if( !(save_push_key_ & LEFT_KEY) )
		{
			key            |= LEFT_KEY;  // キー入力ビット
			save_push_key_ |= LEFT_KEY;  // 記憶用ビット
		}
	}
	else save_push_key_ &= ~LEFT_KEY;    // 記憶用ビットを下げる

	/* スペースキー */
	if( PUSHKEY(key_buffer_, DIK_SPACE) )
	{
		/* 前回押されていなければフラグを上げる */
		if( !(save_push_key_ & SPACE_KEY) )
		{
			key            |= SPACE_KEY;  // キー入力ビット
			save_push_key_ |= SPACE_KEY;  // 記憶用ビット
		}
	}
	else save_push_key_ &= ~SPACE_KEY;	  // 記憶用ビットを下げる

	/* Enterキー */
	if( PUSHKEY(key_buffer_, DIK_RETURN) )
	{
		/* 前回押されていなければフラグを上げる */
		if( !(save_push_key_ & ENTER_KEY) )
		{
			key            |= ENTER_KEY;  // キー入力ビット
			save_push_key_ |= ENTER_KEY;  // 記憶用ビット
		}
	}
	else save_push_key_ &= ~ENTER_KEY;	  // 記憶用ビットを下げる

	/* 1キー */
	if( PUSHKEY(key_buffer_, DIK_1) )
	{
		/* 前回押されていなければフラグを上げる */
		if (!(save_push_key_ & KEY_1))
		{
			key            |= KEY_1;  // キー入力ビット
			save_push_key_ |= KEY_1;  // 記憶用ビット
		}
	}
	else save_push_key_ &= ~KEY_1;	  // 記憶用ビットを下げる

	/* 2キー */
	if (PUSHKEY(key_buffer_, DIK_2) )
	{
		/* 前回押されていなければフラグを上げる */
		if (!(save_push_key_ & KEY_2))
		{
			key            |= KEY_2;  // キー入力ビット
			save_push_key_ |= KEY_2;  // 記憶用ビット
		}
	}
	else save_push_key_ &= ~KEY_2;	  // 記憶用ビットを下げる

	/* 3キー */
	if (PUSHKEY(key_buffer_, DIK_3) )
	{
		/* 前回押されていなければフラグを上げる */
		if (!(save_push_key_ & KEY_3))
		{
			key            |= KEY_3;  // キー入力ビット
			save_push_key_ |= KEY_3;  // 記憶用ビット
		}
	}
	else save_push_key_ &= ~KEY_3;	  // 記憶用ビットを下げる

	/* 4キー */
	if (PUSHKEY(key_buffer_, DIK_4))
	{
		/* 前回押されていなければフラグを上げる */
		if (!(save_push_key_ & KEY_4) )
		{
			key            |= KEY_4;  // キー入力ビット
			save_push_key_ |= KEY_4;  // 記憶用ビット
		}
	}
	else save_push_key_ &= ~KEY_4;	  // 記憶用ビットを下げる

	/* 5キー */
	if (PUSHKEY(key_buffer_, DIK_5) )
	{
		/* 前回押されていなければフラグを上げる */
		if (!(save_push_key_ & KEY_5))
		{
			key            |= KEY_5;  // キー入力ビット
			save_push_key_ |= KEY_5;  // 記憶用ビット
		}
	}
	else save_push_key_ &= ~KEY_5;	  // 記憶用ビットを下げる

	/* Zキー */
	if (PUSHKEY(key_buffer_, DIK_Z))
	{
		/* 前回押されていなければフラグを上げる */
		if (!(save_push_key_ & Z_KEY))
		{
			key            |= Z_KEY;  // キー入力ビット
			save_push_key_ |= Z_KEY;  // 記憶用ビット
		}
	}
	else save_push_key_ &= ~Z_KEY;    // 記憶用ビットを下げる

	return key;
}

/*****************************************
 * 関数名 : GetDownButton関数            *
 * 内  容 : ゲームパッド入力の取得します *
 * 引  数 : なし                         *
 * 戻り値 : 押されているボタン           *
 *****************************************/
ULONG DInput::GetDownButton( VOID )
{
	ULONG button = 0L;   // ボタン入力ビット 

	/* キー入力判定 */
	if( ButtonBuffer.lY < -STROKE_MARGIN ) button |= JS_UP;        // スティック上
	if( ButtonBuffer.lX > STROKE_MARGIN )  button |= JS_RIGHT;     // スティック右
	if( ButtonBuffer.lY > STROKE_MARGIN )  button |= JS_DOWN;      // スティック下
	if( ButtonBuffer.lX < -STROKE_MARGIN ) button |= JS_LEFT;      // スティック左
	if( ButtonBuffer.rgbButtons[ 0 ] )     button |= JS_BUTTONA;   // ボタンA
	if( ButtonBuffer.rgbButtons[ 1 ] )     button |= JS_BUTTONB;   // ボタンB
	if( ButtonBuffer.rgbButtons[ 2 ] )     button |= JS_BUTTONX;   // ボタンX
	if( ButtonBuffer.rgbButtons[ 3 ] )     button |= JS_BUTTONY;   // ボタンY
	if( ButtonBuffer.rgbButtons[ 4 ] )     button |= JS_BUTTONLB;  // ボタンLB
	if( ButtonBuffer.rgbButtons[ 5 ] )     button |= JS_BUTTONRB;  // ボタンRB
	if( ButtonBuffer.rgbButtons[ 6 ] )     button |= JS_BUTTONSL;  // セレクトボタン
	if( ButtonBuffer.rgbButtons[ 7 ] )     button |= JS_BUTTONST;  // スタートボタン
	if( ButtonBuffer.rgbButtons[ 8 ] )     button |= JS_BUTTONA;   // 左スティック押し込み
	if( ButtonBuffer.rgbButtons[ 9 ] )     button |= JS_BUTTONA;   // 右スティック押し込み

	return button;
}

/*************************************
 * 関数名 : GetPushButton関数        *
 * 内  容 : ボタン入力の取得をします *
 * 引  数 : なし                     *
 * 戻り値 : 押されていたボタン       *
 *************************************/
ULONG DInput::GetPushButton( VOID )
{
	ULONG button = 0L;   // キー入力ビット
	
	if( getpad_flag_ )
	{
		/* コントローラー上 */
		if( ButtonBuffer.lY < -STROKE_MARGIN )
		{
			/* 前回押されていなければフラグを上げる */
			if( !( save_push_button_ & JS_UP ) )
			{
				button            |= JS_UP;  // ボタン入力ビット
				save_push_button_ |= JS_UP;  // 記憶用ビット
			}
		}
		else save_push_button_ &= ~JS_UP;    // 記憶用ビットを下げる

		/* コントローラー右 */
		if( ButtonBuffer.lX < STROKE_MARGIN )
		{
			/* 前回押されていなければフラグを上げる */
			if( !( save_push_button_ & JS_RIGHT ) )
			{
                button            |= JS_RIGHT;  // ボタン入力ビット
				save_push_button_ |= JS_RIGHT;  // 記憶用ビット
			}
		}
		else save_push_button_ &= ~JS_RIGHT;    // 記憶用ビットを下げる

		/* コントローラー下 */
		if( ButtonBuffer.lY < STROKE_MARGIN )
		{
			/* 前回押されていなければフラグを上げる */
			if( !( save_push_button_ & JS_DOWN ) )
			{
				button            |= JS_DOWN;  // ボタン入力ビット
				save_push_button_ |= JS_DOWN;  // 記憶用ビット
			}
		}
		else save_push_button_ &= ~JS_DOWN;    // 記憶用ビットを下げる
		
		/* コントローラー左 */
		if( ButtonBuffer.lX < -STROKE_MARGIN )
		{
			/* 前回押されていなければフラグを上げる */
			if( !( save_push_button_ & JS_LEFT ) )
			{
				button            |= JS_LEFT;  // ボタン入力ビット
				save_push_button_ |= JS_LEFT;  // 記憶用ビット
			}
		}
		else save_push_button_ &= ~JS_LEFT;    // 記憶用ビットを下げる

		/* ボタンA */
		if( ButtonBuffer.rgbButtons[ 0 ] )
		{
			/* 前回押されていなければフラグを上げる */
			if( !( save_push_button_ & JS_BUTTONA ) )
			{
				button            |= JS_BUTTONA;  // ボタン入力ビット
				save_push_button_ |= JS_BUTTONA;  // 記憶用ビット
			}
		}
		else save_push_button_ &= ~JS_BUTTONA;    // 記憶用ビットを下げる

		/* ボタンB */
		if( ButtonBuffer.rgbButtons[ 1 ] )
		{
			/* 前回押されていなければフラグを上げる */
			if( !( save_push_button_ & JS_BUTTONB ) )
			{
				button            |= JS_BUTTONB;  // ボタン入力ビット
				save_push_button_ |= JS_BUTTONB;  // 記憶用ビット
			}
		}
		else save_push_button_ &= ~JS_BUTTONB;    // 記憶用ビットを下げる
		
		/* ボタンX  */
		if( ButtonBuffer.rgbButtons[ 2 ] )
		{
			/* 前回押されていなければフラグを上げる */
			if( !( save_push_button_ & JS_BUTTONX ) )
			{
				button            |= JS_BUTTONX;  // ボタン入力ビット
				save_push_button_ |= JS_BUTTONX;  // 記憶用ビット
			}
		}
		else save_push_button_ &= ~JS_BUTTONX;    // 記憶用ビットを下げる

		/* ボタンY */
		if( ButtonBuffer.rgbButtons[ 3 ] )
		{
			/* 前回押されていなければフラグを上げる */
			if( !( save_push_button_ & JS_BUTTONY ) )
			{
				button            |= JS_BUTTONY;  // ボタン入力ビット
				save_push_button_ |= JS_BUTTONY;  // 記憶用ビット
			}
		}
		else save_push_button_ &= ~JS_BUTTONY;    // 記憶用ビットを下げる

		/* ボタンLB */
		if (ButtonBuffer.rgbButtons[4])
		{
			/* 前回押されていなければフラグを上げる */
			if (!(save_push_button_ & JS_BUTTONLB))
			{
				button            |= JS_BUTTONLB;  // ボタン入力ビット
				save_push_button_ |= JS_BUTTONLB;  // 記憶用ビット
			}
		}
		else save_push_button_ &= ~JS_BUTTONLB;    // 記憶用ビットを下げる

		/* ボタンRB */
		if (ButtonBuffer.rgbButtons[5])
		{
			/* 前回押されていなければフラグを上げる */
			if (!(save_push_button_ & JS_BUTTONRB))
			{
				button            |= JS_BUTTONRB;  // ボタン入力ビット
				save_push_button_ |= JS_BUTTONRB;  // 記憶用ビット
			}
		}
		else save_push_button_ &= ~JS_BUTTONRB;    // 記憶用ビットを下げる

		/* セレクトボタン */
		if( ButtonBuffer.rgbButtons[ 6 ] )
		{
			/* 前回押されていなければフラグを上げる */
			if( !( save_push_button_ & JS_BUTTONSL ) )
			{
				button            |= JS_BUTTONSL;  // ボタン入力ビット
				save_push_button_ |= JS_BUTTONSL;  // 記憶用ビット
			}
		}
		else save_push_button_ &= ~JS_BUTTONSL;    // 記憶用ビットを下げる
		
		/* スタートボタン */
		if( ButtonBuffer.rgbButtons[ 7 ] )
		{
			/* 前回押されていなければフラグを上げる */
			if( !( save_push_button_ & JS_BUTTONST ) )
			{
				button            |= JS_BUTTONST;  // ボタン入力ビット
				save_push_button_ |= JS_BUTTONST;  // 記憶用ビット
			}
		}
		else save_push_button_ &= ~JS_BUTTONST;    // 記憶用ビットを下げる
	}

		
	return button;
}

/*************************************************************
 * 関数名 : EnumJoyCallback関数                              *
 * 内  容 : ゲームパッドを列挙するコールバック関数を返します *
 * 引  数 : DIDEVICEINSTANCE* pdidInstance                   *
 *			VOID* pContext                                   *
 * 戻り値：結果                                              *
**************************************************************/
BOOL CALLBACK DInput::EnumJoyCallback( const DIDEVICEINSTANCE* pdidInstance, VOID* pContext )
{
	BOOL ret = DIENUM_STOP;

	static GUID pad_discrimination;

	do
	{
		/* 既に取得済みのデバイスだった場合はもう一回実行する */
		if( pad_discrimination == pdidInstance->guidInstance )
		{
			return DIENUM_CONTINUE;
		}

		/* 列挙されたパッドへのインターフェイスを取得する */
		if( FAILED( input_->CreateDevice( pdidInstance->guidInstance, &input_joydevice_, NULL ) ) )
		{
			ErrorDialog( "パッドのインターフェイスの取得に失敗" );
			ret = FALSE;
			break;
		}
		
		
		/* パッドの能力を調べる */
		DIDEVCAPS diDevCaps;

		/* 初期化 */
		ZeroMemory( &diDevCaps, sizeof( DIDEVCAPS ) );

		diDevCaps.dwSize = sizeof( DIDEVCAPS );

		if( FAILED( input_joydevice_->GetCapabilities( &diDevCaps ) ) )
		{
			ErrorDialog( "パッドの能力取得に失敗" );
			SafeRelease( input_joydevice_ );
			input_joydevice_ = NULL;
			ret = DIENUM_CONTINUE;	// (列挙を続ける)
			break;
		}
		
		pad_discrimination = pdidInstance->guidInstance;

	}while( 0 );

	return ret;
}

/***********************************************************
 * 関数名 : EnumAxesCallback関数                           *
 * 内  容 : 軸ごとに呼び出されるコールバック関数を返します *
 * 引  数 : DIDEVICEINSTANCE* Lpddoi                       *
 *			VOID* pVRef                                    *
 * 戻り値：結果                                            *
 ***********************************************************/
BOOL CALLBACK DInput::EnumAxesCallback( LPCDIDEVICEOBJECTINSTANCE Lpddoi, LPVOID pVRef )
{
	BOOL ret = DIENUM_CONTINUE;   // 戻り値

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
		/* エラー */
		ret = DIENUM_STOP;
	}

	return ret;
}