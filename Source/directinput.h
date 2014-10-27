/*************************
 * DirectInput制御クラス *
 *************************/

/* 多重Include防止 */
#ifndef RACE_DINPUT_H_
#define RACE_DINPUT_H_

/***********
 * Include *
 ***********/
#include "common.h"
#include "release.h"

/***********************
 * 内部定数/マクロ定義 *
 ***********************/
#define	PUSHKEY(name, key)	(name[ key ] & 0x80)	// キー入力用

/* キーボード */
CONST ULONG	UP_KEY		= 1;                // カーソルキー上
CONST ULONG	RIGHT_KEY	= UP_KEY     << 1;	// カーソルキー右
CONST ULONG	DOWN_KEY	= RIGHT_KEY  << 1;	// カーソルキー下
CONST ULONG	LEFT_KEY	= DOWN_KEY   << 1;	// カーソルキー左
CONST ULONG	SPACE_KEY	= LEFT_KEY   << 1;	// スペースキー
CONST ULONG	ENTER_KEY	= SPACE_KEY  << 1;	// エンターキー
CONST ULONG KEY_1       = ENTER_KEY  << 1;  // 1キー
CONST ULONG KEY_2       = KEY_1      << 1;  // 2キー
CONST ULONG KEY_3       = KEY_2      << 1;  // 3キー
CONST ULONG KEY_4       = KEY_3      << 1;  // 4キー
CONST ULONG KEY_5       = KEY_4      << 1;  // 5キー
CONST ULONG Z_KEY       = KEY_5      << 1;  // Zキー

/* 通常パッド配置 */	
CONST ULONG	JS_UP        = 1;                  // ゲームコントローラ上
CONST ULONG	JS_RIGHT     = JS_UP        << 1;  // ゲームコントローラ右
CONST ULONG	JS_DOWN      = JS_RIGHT     << 1;  // ゲームコントローラ下
CONST ULONG	JS_LEFT      = JS_DOWN      << 1;  // ゲームコントローラ左
CONST ULONG	JS_BUTTONA   = JS_LEFT      << 1;  // ゲームコントローラA
CONST ULONG	JS_BUTTONB   = JS_BUTTONA   << 1;  // ゲームコントローラB
CONST ULONG	JS_BUTTONX   = JS_BUTTONB   << 1;  // ゲームコントローラX
CONST ULONG	JS_BUTTONY   = JS_BUTTONX   << 1;  // ゲームコントローラY
CONST ULONG JS_BUTTONLB	 = JS_BUTTONY   << 1;  // ゲームコントローラーLB
CONST ULONG JS_BUTTONRB	 = JS_BUTTONLB  << 1;  // ゲームコントローラーRB
CONST ULONG JS_BUTTONST  = JS_BUTTONRB  << 1;  // スタートボタン
CONST ULONG	JS_BUTTONSL	 = JS_BUTTONST  << 1;  // セレクトボタン
CONST ULONG	JS_BUTTONLF	 = JS_BUTTONSL  << 1;  // 左スティック押し込み
CONST ULONG JS_BUTTONRF  = JS_BUTTONLF  << 1;  // 右スティック押し込み

/* スティック用定数 */
CONST INT STROKE_MIN     = -1000;  // 軸最小値
CONST INT STROKE_MAX     = 1000;   // 軸最大値
CONST INT STROKE_MARGIN  = 300;    // 軸の遊び値

/***********************************
 * DirectInputを制御するクラスです *
 ***********************************/
class DInput
{
private:

    /* 共通 */
    static LPDIRECTINPUT8  input_;  // DirectInputインターフェイス

	/* キーボード用 */
	static LPDIRECTINPUTDEVICE8  input_device_;       // DirectInputDeviceインターフェイス（キーボード）
	static ULONG                 save_push_key_;      // 前回押されていたボタン(キーボード)
    static BYTE                  key_buffer_[ 256 ];  // 入力キーバッファ

	/* パッド用 */
	static LPDIRECTINPUTDEVICE8  input_joydevice_;   // DirectInpuDeiviceインターフェイス(ジョイスティック)
	static ULONG                 save_push_button_;	 // 前回押されていたボタン(ジョイスティック)
	static DIJOYSTATE2           ButtonBuffer;       // ボタンバッファ(ジョイスティック)
	static BOOL                  getpad_flag_;       // コントローラーの使用

public:
	/* 共通 */
    DInput( VOID );   // コンストラクタ
    ~DInput( VOID );  // デストラクタ
	static BOOL Init( CONST HWND hWnd, CONST HINSTANCE Instance );  // 初期化
	static VOID  GetInput( VOID );  // 入力の取得
    static VOID  Destroy( VOID );   // 破棄

	/* キーボード用 */
    static ULONG  GetDownKey( VOID );  // 押されているキーの取得
	static ULONG  GetPushKey( VOID );  // 押されたキーの取得

	/* パッド用 */
	static ULONG  GetDownButton( VOID );  // 押されているボタンの取得
	static ULONG  GetPushButton( VOID );  // 押されたボタンの取得

	static BOOL   Get_PadFlag( VOID ) { return getpad_flag_; }  // コントローラーを取得したかチェック

	/* コールバック関数 */
	static BOOL	CALLBACK EnumJoyCallback( CONST DIDEVICEINSTANCE* pdidInstance, VOID* pContext );
	static BOOL	CALLBACK EnumAxesCallback( LPCDIDEVICEOBJECTINSTANCE Lpddoi, LPVOID pVRef );

};

#endif