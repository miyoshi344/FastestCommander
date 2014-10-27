/***********
 * Include *
 ***********/
#include "main_loop.h"

/********************
 * �v���g�^�C�v�錾 *
 ********************/
LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

/******************
 * �R���X�g���N�^ *
 ******************/
MainLoop::MainLoop(VOID)
{
	/* ������ */
	application_instance_ = NULL;
	mutex_handle_ = NULL;
	loop_state_ = kRun;

	for (INT i = 0; i < kAllPlayer; i++)
	{
		button[i] = 0;
	}
}

/****************
 * �f�X�g���N�^ *
 ****************/
MainLoop::~MainLoop(VOID)
{
	CloseHandle(mutex_handle_);   // �~���[�e�b�N�X�̉��
}

/*****************************************************************
 * �֐��� : Init�֐�                                             *
 * ��  �e : �E�B���h�E�̍쐬�Ɗe�탉�C�u�����̏��������s���܂�   *
 * ��  �� : HINSTANCE�@Instance�F�A�v���P�[�V�����C���X�^���X�@  *
 * �߂�l : �������y�уE�B���h�E�̍쐬�ɐ��������TRUE��Ԃ��܂� *
 *****************************************************************/
BOOL MainLoop::Init(HINSTANCE Instance)
{
	Camera camera_;

	BOOL ret = TRUE;    // �߂�l�p
	HWND window_handle; // �E�B���h�E�n���h��

	/* �~���[�e�b�N�X�̍쐬 */
	mutex_handle_ = CreateMutex(NULL, FALSE, kWindowName);

	do
	{
		/* ��d�N���̃`�F�b�N�~ */
		if (GetLastError() == ERROR_ALREADY_EXISTS)
		{
			/* ���łɋN�����Ă�����I�� */
			ErrorDialog("���d�N���͂ł��܂���B");
			ret = FALSE;
			break;
		}

		/* �E�B���h�E�̍쐬 */
		if (!(window_handle = MyCreateWindow(Instance)))
		{
			ret = FALSE;
			break;
		}

		/* �C���X�^���X�̍쐬 */
		if (!CreateInstance(Instance, window_handle))
		{
			ret = FALSE;
			break;
		}

		/* timeGetTime()�̕���\���グ�� */
		TIMECAPS time_caps;
		UINT time_res;
		timeGetDevCaps(&time_caps, sizeof(TIMECAPS));                       // ����\�𒲂ׂ�
		time_res = min(max(time_caps.wPeriodMin, 1), time_caps.wPeriodMax);	// ���덷�����Ȃ������i�[
		timeBeginPeriod(time_res);                                          // �ŏ��^�C�}����\��ݒ�

		/* �E�B���h�E����ʏ�ɕ\������ */
		ShowWindow(window_handle, SW_SHOWNORMAL);

	} while (0);

	return ret;
}

/*************************************
 * �֐��� : Run�֐�                  *
 * ��  �e : ���C�����[�v���������܂� *
 * ��  �� : �Ȃ��@                   *
 * �߂�l : �Ȃ�                     *
 *************************************/
VOID MainLoop::Run(VOID)
{
	BOOL reset = FALSE;
	MSG       msg;          // ���b�Z�[�W
	FrameRate frame_rate;   // �t���[�����[�g����
	Camera camera_;

	/* ���C�����[�v */
	while (loop_state_)
	{
		/* ���b�Z�[�W���� */
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			/* �I�����b�Z�[�W�������烋�[�v�𔲂��� */
			if (msg.message == WM_QUIT) break;

			/* ���b�Z�[�W���� */
			TranslateMessage(&msg);	// �L�[�{�[�h���b�Z�[�W�𕶎����b�Z�[�W�ɕϊ�(�L�[�{�[�h�̈Ⴂ���z���j
			DispatchMessage(&msg);	// �E�B���h�E�v���V�[�W���Ƀ��b�Z�[�W�𑗐M
		}
		else
		{
			/* �Q�[�������̊J�n **/
			if (frame_rate.Check())
			{
				/* ���� */
				DInput::GetInput();

				/* XInput�f�o�C�X�̎擾 */
				if (!XInput::Device()) ErrorDialog("�Q�[���p�b�h���ڑ�����Ă��܂���");
			
				/* �{�^���̓��͂��擾 */
				for (INT i = 0; i < kMaxPad; i++)
				{
					XInput::PushStick(i);
					XInput::PushButton(i);

					if (XInput::DownButton(i) == XINPUT_GAMEPAD_BACK) button[i]++;
					else button[i] = 0;

					if (button[i] >= 60) reset = TRUE;
				}

				if (reset) break;

				/* �Q�[���̍X�V���� */
				Progress_.UpDate();

				/* �X�v���C�g�̕`�揈�� */
				D3DDEV->BeginScene();

				/* ��ʃN���A */
				D3DDEV->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(200, 200, 200), 1.0F, 0);

				/* �`�� */
				Progress_.Draw();

				D3DDEV->EndScene();

				/* �f�o�C�X������������I�� */
				if (D3DDEV->Present(NULL, NULL, NULL, NULL) == D3DERR_DEVICELOST) break;

				frame_rate.Draw();
			}
		}
	}
}


/**************************************************************
 * �֐��� : MyCreateWindow�֐�                                *
 * ��  �e : �E�B���h�E���쐬���܂�                            *
 * ��  �� : HINSTANCE�@Instance�F�A�v���P�[�V�����C���X�^���X *
 * �߂�l : �E�B���h�E�n���h����Ԃ��܂�                      *
 **************************************************************/
HWND MainLoop::MyCreateWindow(CONST HINSTANCE Instance)
{
	HWND window_handle = NULL;  // �E�B���h�E�n���h���i�[��

	do
	{
		/* �E�B���h�E�N���X�̓o�^ */
		WNDCLASSEX wndclassex;                             // �E�B���h�E�N���X�o�^�p�ϐ�
		ZeroMemory(&wndclassex, sizeof(WNDCLASSEX));       // �[���N���A
		wndclassex.cbSize     = sizeof(WNDCLASSEX);        // �T�C�Y
		wndclassex.hInstance = Instance;                   // �C���X�^���X
		wndclassex.style = CS_HREDRAW | CS_VREDRAW;        // �X�^�C���i�T�C�Y�������E�����ŕύX���ꂽ��ĕ`��j
		wndclassex.lpszMenuName = kWindowName;             // �A�v���P�[�V������
		wndclassex.lpszClassName = kClassName;             // �N���X��
		wndclassex.lpfnWndProc = WindowProc;               // �R�[���o�b�N�֐��|�C���^
		wndclassex.hIcon = NULL;                           // �A�C�R��
		wndclassex.hCursor = LoadCursor(NULL, IDC_ARROW);  // �J�[�\���`��

		/* �E�B���h�E�N���X�̓o�^ */
		if (!RegisterClassEx(&wndclassex))
		{
			/* �G���[ */
			ErrorDialog("�E�B���h�E�N���X�̓o�^�Ɏ��s");
			break;
		}

		/* �E�B���h�E�̍쐬 */
		INT	 width = kWindowWidth + GetSystemMetrics(SM_CXDLGFRAME) * 2;									// �E�B���h�E����
		INT	 height = kWindowHeight + GetSystemMetrics(SM_CYDLGFRAME) * 2 + GetSystemMetrics(SM_CYCAPTION);	// �E�B���h�E�c��

		/* �f�X�N�g�b�v�T�C�Y���擾 */
		RECT rect_desktop;
		GetWindowRect(GetDesktopWindow(), (LPRECT)&rect_desktop);

#ifndef FULLSCREEN_MODE
		DWORD style = WS_SYSMENU | WS_MINIMIZEBOX;
#else
		DWORD style = WS_SYSMENU | WS_MINIMIZEBOX | WS_POPUP;
		ShowCursor(FALSE);   // �}�E�X�J�[�\���̔�\��
#endif

		window_handle = CreateWindowEx(
			WS_EX_APPWINDOW,                    // �E�B���h�E�X�^�C��
			kClassName,					        // �E�B���h�E�N���X�œo�^�����N���X
			kWindowName,                        // �E�B���h�E��
			style,                              // �E�B���h�E�̃X�^�C��
			(rect_desktop.right - width)   / 2, // �\�����WX
			(rect_desktop.bottom - height) / 2, // �\�����WY
			width,                              // �E�B���h�E�T�C�YX
			height,                             // �E�B���h�E�T�C�YY
			NULL,                               // �e�E�B���h�E�܂��̓I�[�i�[�E�B���h�E�̃n���h��
			NULL,                               // ���j���[�n���h���܂��͎q���ʎq
			Instance,                           // �A�v���P�[�V�����̃C���X�^���X
			NULL);                              // �E�B���h�E�쐬�f�[�^

		/* �n���h���擾�`�F�b�N */
		if (!window_handle)
		{
			/* �G���[ */
			window_handle = NULL;
			ErrorDialog("�E�B���h�E�n���h���̍쐬�Ɏ��s");
			break;
		}

	} while (0);

	return window_handle;
}

/***************************************************
 * �֐��� : CreateInstance                         *
 * ��  �e : ���ʂŎg�p����C���X�^���X���쐬���܂� *
 * ��  �� : CONST HINSTANCE Instance               *
 *          CONST HWND hWnd�@                      *
 * �߂�l : ����������TRUE��Ԃ��܂�               *
 ***************************************************/
BOOL MainLoop::CreateInstance(CONST HINSTANCE Instance, CONST HWND hWnd)
{
	BOOL ret = TRUE;    // �߂�l�p

	do
	{
		/* Direct3D�̏����� */
		if (!Direct3D::Init(hWnd))
		{
			/* �G���[ */
			ErrorDialog("Direct3D�̏������Ɏ��s���܂���");
			return FALSE;
		}

		/* Sprite�̏����� */
		if (!Sprite::Init())
		{
			/* �G���[ */
			ErrorDialog("Sprite�̏������Ɏ��s���܂���");
			return FALSE;
		}

		/* Texture�̏����� */
		if (!Texture::Init())
		{
			/* �G���[ */
			ErrorDialog("Texture�̏������Ɏ��s���܂���");
			return FALSE;
		}

		/* DirectInput�̏����� */
		if (!DInput::Init(hWnd, Instance))
		{
			/* �G���[ */
			ErrorDialog("DirectInput�̏������Ɏ��s���܂���");
			return FALSE;
		}

		/* DirectSound�̏����� */
		if (!DSound::Init(hWnd))
		{
			/* �G���[ */
			ErrorDialog("DirectSound�̏������Ɏ��s���܂���");
			return FALSE;
		}

		/* ��ԑJ�ڂ̏����� */
		if (!Progress_.Init())
		{
			/* �G���[ */
			ErrorDialog("��ԑJ�ڂ̏������Ɏ��s���܂���");
			return FALSE;
		}

	} while (0);

	return ret;
}

/*********************************************
 * �֐��� : Destroy�֐�                      *
 * ��  �e : ���������C���X�^���X��j�����܂� *
 * ��  �� : �Ȃ��@                           *
 * �߂�l : �Ȃ�                             *
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
 * �֐��� : WindowProc�֐�                     *
 * ��  �e : �E�B���h�E�v���V�[�W��             *
 * ��  �� : HWND hWnd       �E�B���h�E�n���h�� *
 *          UINT msg        ���b�Z�[�W         *
 *          WPARAM wParam   �p�����[�^1        *
 *          LPARAM lParam�@ �p�����[�^2        *
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

		/***** �L�[�������ꂽ�ꍇ *****/
		case WM_KEYDOWN:

		switch (wParam)
		{
			/* ESC�������ꂽ�� */
			case VK_ESCAPE:
				PostMessage(hWnd, WM_CLOSE, 0, 0);	// �v���O�����I��
				break;
		}
		break;

		/* �I�� */
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}