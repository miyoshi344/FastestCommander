/**************************************
* メインループを実装するクラスヘッダ *
**************************************/

/* 多重Include防止 */
#ifndef RACE_MAIN_LOOP_H_
#define RACE_MAIN_LOOP_H_

/***********
* Include *
***********/
#include "common.h"        // 共通ヘッダ
#include "direct3d.h"      // Direct3D
#include "xinput.h"        // XInputクラス
#include "directinput.h"   // DirectInput
#include "directsound.h"   // DirectSound
#include "sprite.h"        // Sprite
#include "camera.h"        // カメラクラス
#include "framerate.h"     // フレームレートクラス
#include "Progress.h"

/******************************
* メインループを実装します。 *
******************************/
class MainLoop
{
private:
	INT button[4];

	Progress Progress_;

	enum SceneName
	{
		kTitle,     // タイトル
		kGame,      // ゲーム
		kNumScenes  // シーン数
	};

	enum LoopState
	{
		kStop,  // 停止
		kRun    // 実行
	};

	HWND MyCreateWindow(CONST HINSTANCE Instance);                    // ウィンドウの作成
	BOOL CreateInstance(CONST HINSTANCE Instance, CONST HWND hWnd);   // DirectXインスタンスの作成

	HINSTANCE application_instance_;  // アプリケーションインスタンス
	HANDLE    mutex_handle_;          // ミューテックスハンドル
	LoopState loop_state_;            // メインループの状態

public:
	MainLoop(VOID);						  // コンストラクタ
	~MainLoop(VOID);                      // デストラクタ
	BOOL Init(CONST HINSTANCE Instance);  // 初期化
	VOID Run(VOID);                       // メインループ処理
	VOID Destroy(VOID);                   // 破棄
};

#endif