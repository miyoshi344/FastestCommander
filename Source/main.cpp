/***********
 * Include *
 ***********/
#include "main.h"

/********************************
 * �v���O�����G���g���[�|�C���g *
 ********************************/
INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, INT nCmdShow)
{
#ifdef _DEBUG
	//_CrtSetBreakAlloc( 199 );
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	/* ���C�����[�v������ */
	MainLoop loop;
	if (loop.Init(hInstance))
	{
		loop.Run();  // ���C�����[�v����
	}

	/* �j�� */
	loop.Destroy();

	return 0;
}