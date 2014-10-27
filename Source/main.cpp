/***********
 * Include *
 ***********/
#include "main.h"

/********************************
 * プログラムエントリーポイント *
 ********************************/
INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, INT nCmdShow)
{
#ifdef _DEBUG
	//_CrtSetBreakAlloc( 199 );
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	/* メインループ初期化 */
	MainLoop loop;
	if (loop.Init(hInstance))
	{
		loop.Run();  // メインループ処理
	}

	/* 破棄 */
	loop.Destroy();

	return 0;
}