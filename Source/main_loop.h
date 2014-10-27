/**************************************
* ���C�����[�v����������N���X�w�b�_ *
**************************************/

/* ���dInclude�h�~ */
#ifndef RACE_MAIN_LOOP_H_
#define RACE_MAIN_LOOP_H_

/***********
* Include *
***********/
#include "common.h"        // ���ʃw�b�_
#include "direct3d.h"      // Direct3D
#include "xinput.h"        // XInput�N���X
#include "directinput.h"   // DirectInput
#include "directsound.h"   // DirectSound
#include "sprite.h"        // Sprite
#include "camera.h"        // �J�����N���X
#include "framerate.h"     // �t���[�����[�g�N���X
#include "Progress.h"

/******************************
* ���C�����[�v���������܂��B *
******************************/
class MainLoop
{
private:
	INT button[4];

	Progress Progress_;

	enum SceneName
	{
		kTitle,     // �^�C�g��
		kGame,      // �Q�[��
		kNumScenes  // �V�[����
	};

	enum LoopState
	{
		kStop,  // ��~
		kRun    // ���s
	};

	HWND MyCreateWindow(CONST HINSTANCE Instance);                    // �E�B���h�E�̍쐬
	BOOL CreateInstance(CONST HINSTANCE Instance, CONST HWND hWnd);   // DirectX�C���X�^���X�̍쐬

	HINSTANCE application_instance_;  // �A�v���P�[�V�����C���X�^���X
	HANDLE    mutex_handle_;          // �~���[�e�b�N�X�n���h��
	LoopState loop_state_;            // ���C�����[�v�̏��

public:
	MainLoop(VOID);						  // �R���X�g���N�^
	~MainLoop(VOID);                      // �f�X�g���N�^
	BOOL Init(CONST HINSTANCE Instance);  // ������
	VOID Run(VOID);                       // ���C�����[�v����
	VOID Destroy(VOID);                   // �j��
};

#endif