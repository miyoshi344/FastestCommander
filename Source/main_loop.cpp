/***********
 * Include *
 ***********/
#include "main_loop.h"

/********************
 * プロトタイプ宣言 *
 ********************/
LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

/******************
 * コンストラクタ *
 ******************/
MainLoop::MainLoop(VOID)
{
	/* 初期化 */
	application_instance_ = NULL;
	mutex_handle_ = NULL;
	loop_state_ = kRun;

	for (INT i = 0; i < kAllPlayer; i++)
	{
		button[i] = 0;
	}
}

/****************
 * デストラクタ *
 ****************/
MainLoop::~MainLoop(VOID)
{
	CloseHandle(mutex_handle_);   // ミューテックスの解放
}

/*****************************************************************
 * 関数名 : Init関数                                             *
 * 内  容 : ウィンドウの作成と各種ライブラリの初期化を行います   *
 * 引  数 : HINSTANCE　Instance：アプリケーションインスタンス　  *
 * 戻り値 : 初期化及びウィンドウの作成に成功するとTRUEを返します *
 *****************************************************************/
BOOL MainLoop::Init(HINSTANCE Instance)
{
	Camera camera_;

	BOOL ret = TRUE;    // 戻り値用
	HWND window_handle; // ウィンドウハンドル

	/* ミューテックスの作成 */
	mutex_handle_ = CreateMutex(NULL, FALSE, kWindowName);

	do
	{
		/* 二重起動のチェック止 */
		if (GetLastError() == ERROR_ALREADY_EXISTS)
		{
			/* すでに起動していたら終了 */
			ErrorDialog("多重起動はできません。");
			ret = FALSE;
			break;
		}

		/* ウィンドウの作成 */
		if (!(window_handle = MyCreateWindow(Instance)))
		{
			ret = FALSE;
			break;
		}

		/* インスタンスの作成 */
		if (!CreateInstance(Instance, window_handle))
		{
			ret = FALSE;
			break;
		}

		/* timeGetTime()の分解能を上げる */
		TIMECAPS time_caps;
		UINT time_res;
		timeGetDevCaps(&time_caps, sizeof(TIMECAPS));                       // 分解能を調べる
		time_res = min(max(time_caps.wPeriodMin, 1), time_caps.wPeriodMax);	// より誤差が少ない方を格納
		timeBeginPeriod(time_res);                                          // 最小タイマ分解能を設定

		/* ウィンドウを画面上に表示する */
		ShowWindow(window_handle, SW_SHOWNORMAL);

	} while (0);

	return ret;
}

/*************************************
 * 関数名 : Run関数                  *
 * 内  容 : メインループを処理します *
 * 引  数 : なし　                   *
 * 戻り値 : なし                     *
 *************************************/
VOID MainLoop::Run(VOID)
{
	BOOL reset = FALSE;
	MSG       msg;          // メッセージ
	FrameRate frame_rate;   // フレームレート調整
	Camera camera_;

	/* メインループ */
	while (loop_state_)
	{
		/* メッセージ処理 */
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			/* 終了メッセージだったらループを抜ける */
			if (msg.message == WM_QUIT) break;

			/* メッセージ処理 */
			TranslateMessage(&msg);	// キーボードメッセージを文字メッセージに変換(キーボードの違いを吸収）
			DispatchMessage(&msg);	// ウィンドウプロシージャにメッセージを送信
		}
		else
		{
			/* ゲーム処理の開始 **/
			if (frame_rate.Check())
			{
				/* 入力 */
				DInput::GetInput();

				/* XInputデバイスの取得 */
				if (!XInput::Device()) ErrorDialog("ゲームパッドが接続されていません");
			
				/* ボタンの入力を取得 */
				for (INT i = 0; i < kMaxPad; i++)
				{
					XInput::PushStick(i);
					XInput::PushButton(i);

					if (XInput::DownButton(i) == XINPUT_GAMEPAD_BACK) button[i]++;
					else button[i] = 0;

					if (button[i] >= 60) reset = TRUE;
				}

				if (reset) break;

				/* ゲームの更新処理 */
				Progress_.UpDate();

				/* スプライトの描画処理 */
				D3DDEV->BeginScene();

				/* 画面クリア */
				D3DDEV->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(200, 200, 200), 1.0F, 0);

				/* 描画 */
				Progress_.Draw();

				D3DDEV->EndScene();

				/* デバイスが消失したら終了 */
				if (D3DDEV->Present(NULL, NULL, NULL, NULL) == D3DERR_DEVICELOST) break;

				frame_rate.Draw();
			}
		}
	}
}


/**************************************************************
 * 関数名 : MyCreateWindow関数                                *
 * 内  容 : ウィンドウを作成します                            *
 * 引  数 : HINSTANCE　Instance：アプリケーションインスタンス *
 * 戻り値 : ウィンドウハンドルを返します                      *
 **************************************************************/
HWND MainLoop::MyCreateWindow(CONST HINSTANCE Instance)
{
	HWND window_handle = NULL;  // ウィンドウハンドル格納先

	do
	{
		/* ウィンドウクラスの登録 */
		WNDCLASSEX wndclassex;                             // ウィンドウクラス登録用変数
		ZeroMemory(&wndclassex, sizeof(WNDCLASSEX));       // ゼロクリア
		wndclassex.cbSize     = sizeof(WNDCLASSEX);        // サイズ
		wndclassex.hInstance = Instance;                   // インスタンス
		wndclassex.style = CS_HREDRAW | CS_VREDRAW;        // スタイル（サイズが水平・垂直で変更されたら再描画）
		wndclassex.lpszMenuName = kWindowName;             // アプリケーション名
		wndclassex.lpszClassName = kClassName;             // クラス名
		wndclassex.lpfnWndProc = WindowProc;               // コールバック関数ポインタ
		wndclassex.hIcon = NULL;                           // アイコン
		wndclassex.hCursor = LoadCursor(NULL, IDC_ARROW);  // カーソル形状

		/* ウィンドウクラスの登録 */
		if (!RegisterClassEx(&wndclassex))
		{
			/* エラー */
			ErrorDialog("ウィンドウクラスの登録に失敗");
			break;
		}

		/* ウィンドウの作成 */
		INT	 width = kWindowWidth + GetSystemMetrics(SM_CXDLGFRAME) * 2;									// ウィンドウ横幅
		INT	 height = kWindowHeight + GetSystemMetrics(SM_CYDLGFRAME) * 2 + GetSystemMetrics(SM_CYCAPTION);	// ウィンドウ縦幅

		/* デスクトップサイズを取得 */
		RECT rect_desktop;
		GetWindowRect(GetDesktopWindow(), (LPRECT)&rect_desktop);

#ifndef FULLSCREEN_MODE
		DWORD style = WS_SYSMENU | WS_MINIMIZEBOX;
#else
		DWORD style = WS_SYSMENU | WS_MINIMIZEBOX | WS_POPUP;
		ShowCursor(FALSE);   // マウスカーソルの非表示
#endif

		window_handle = CreateWindowEx(
			WS_EX_APPWINDOW,                    // ウィンドウスタイル
			kClassName,					        // ウィンドウクラスで登録したクラス
			kWindowName,                        // ウィンドウ名
			style,                              // ウィンドウのスタイル
			(rect_desktop.right - width)   / 2, // 表示座標X
			(rect_desktop.bottom - height) / 2, // 表示座標Y
			width,                              // ウィンドウサイズX
			height,                             // ウィンドウサイズY
			NULL,                               // 親ウィンドウまたはオーナーウィンドウのハンドル
			NULL,                               // メニューハンドルまたは子識別子
			Instance,                           // アプリケーションのインスタンス
			NULL);                              // ウィンドウ作成データ

		/* ハンドル取得チェック */
		if (!window_handle)
		{
			/* エラー */
			window_handle = NULL;
			ErrorDialog("ウィンドウハンドルの作成に失敗");
			break;
		}

	} while (0);

	return window_handle;
}

/***************************************************
 * 関数名 : CreateInstance                         *
 * 内  容 : 共通で使用するインスタンスを作成します *
 * 引  数 : CONST HINSTANCE Instance               *
 *          CONST HWND hWnd　                      *
 * 戻り値 : 成功したらTRUEを返します               *
 ***************************************************/
BOOL MainLoop::CreateInstance(CONST HINSTANCE Instance, CONST HWND hWnd)
{
	BOOL ret = TRUE;    // 戻り値用

	do
	{
		/* Direct3Dの初期化 */
		if (!Direct3D::Init(hWnd))
		{
			/* エラー */
			ErrorDialog("Direct3Dの初期化に失敗しました");
			return FALSE;
		}

		/* Spriteの初期化 */
		if (!Sprite::Init())
		{
			/* エラー */
			ErrorDialog("Spriteの初期化に失敗しました");
			return FALSE;
		}

		/* Textureの初期化 */
		if (!Texture::Init())
		{
			/* エラー */
			ErrorDialog("Textureの初期化に失敗しました");
			return FALSE;
		}

		/* DirectInputの初期化 */
		if (!DInput::Init(hWnd, Instance))
		{
			/* エラー */
			ErrorDialog("DirectInputの初期化に失敗しました");
			return FALSE;
		}

		/* DirectSoundの初期化 */
		if (!DSound::Init(hWnd))
		{
			/* エラー */
			ErrorDialog("DirectSoundの初期化に失敗しました");
			return FALSE;
		}

		/* 状態遷移の初期化 */
		if (!Progress_.Init())
		{
			/* エラー */
			ErrorDialog("状態遷移の初期化に失敗しました");
			return FALSE;
		}

	} while (0);

	return ret;
}

/*********************************************
 * 関数名 : Destroy関数                      *
 * 内  容 : 生成したインスタンスを破棄します *
 * 引  数 : なし　                           *
 * 戻り値 : なし                             *
 *********************************************/
VOID MainLoop::Destroy(VOID)
{
	DSound::Destroy();
	Texture::Destroy();
	DInput::Destroy();
	Direct3D::Destroy();
	Sprite::Destroy();
	Mesh::Destroy();
}

/***********************************************
 * 関数名 : WindowProc関数                     *
 * 内  容 : ウィンドウプロシージャ             *
 * 引  数 : HWND hWnd       ウィンドウハンドル *
 *          UINT msg        メッセージ         *
 *          WPARAM wParam   パラメータ1        *
 *          LPARAM lParam　 パラメータ2        *
 ***********************************************/
LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		case WM_ACTIVATEAPP:
		{
			if (wParam == TRUE)
			{
				XInputEnable(true);
			}
			else
			{
				XInputEnable(false);
			}
			break;
		}

		/***** キーが押された場合 *****/
		case WM_KEYDOWN:

		switch (wParam)
		{
			/* ESCが押された時 */
			case VK_ESCAPE:
				PostMessage(hWnd, WM_CLOSE, 0, 0);	// プログラム終了
				break;
		}
		break;

		/* 終了 */
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}