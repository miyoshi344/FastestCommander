/**************
 * ���ʃN���X *
 **************/

/* ���dInclude�h�~ */
#ifndef RACE_COMMON_H_
#define RACE_COMMON_H_

#define	DIRECTINPUT_VERSION	0x0800
#pragma warning(disable:4996)

/***********
 * Include *
 ***********/
/* Windows */
#include <windows.h> 
#include <mmsystem.h>
#include <stdio.h>
#include <crtdbg.h>
#include <direct.h>

/* STL */
#include <vector>
#include <list>
#include <map>
#include <bitset>
#include <string>
#include <iostream>

/* DirectX */
#include <d3d9.h>
#include <d3dx9.h>
#include <dinput.h>
#include <DxErr.h>

/***********************
 * �����ϐ�/�}�N����` *
 ***********************/
#define	WIN32_LEAN_AND_MEAN	 // Windows�w�b�_����g�p���Ȃ��w�b�_�����ȗ�
#ifndef STRICT               // �^�`�F�b�N������`
#define STRICT
#endif
#ifndef DIRECTINPUT_VERSION  // DirectInput�o�[�W�����w��
#define DIRECTINPUT_VERSION 0x800
#endif

#define _CRTDBG_MAP_ALLOC
#define METRIC_MODE

/* �E�B���h�E�T�C�Y */
//#define FULLSCREEN_MODE  // �t���X�N���[�����[�h

#ifndef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>
#define DEBUG_CLIENTBLOCK new (_CLIENT_BLOCK, __FILE__, __LINE__)
#define new DEBUG_CLIENTBLOCK
#endif



CONST INT  kWindowWidth   = 1280;  // �E�B���h�E����
CONST INT  kWindowHeight  = 720;   // �E�B���h�E�c��

CONST LPSTR kWindowName   = "FastestCommander";  // �E�B���h�E�^�C�g��
CONST LPSTR kClassName    = "FastestCommander";  // �E�B���h�E�N���X�l�[��

CONST INT TEXTURE_NUM     = 30;      // �ő�e�N�X�`���Ǘ�����
CONST INT MESH_NUM        = 30;      // �ő僁�b�V���Ǘ���
CONST UINT NAME_LEN		  = 32767;   // ������
CONST INT MAX_ALPHA       = 255;     // �ő�A���t�@�l

CONST INT   kAllPlayer = 4;      // �v���C���[�ő吔
CONST FLOAT kFieldDraw = 35.0F;  // �`��͈�

/* ��ʕ����p */
CONST D3DXVECTOR3 kMultiWindow = D3DXVECTOR3((kWindowWidth / 2), (kWindowHeight / 2), 0.0F);

/********
 * �� *
 ********/

/* �n�ʂ̃u���b�N���� */
enum
{
	THORN,   // ���n��
	GROUND,  // �ʏ�n�� 
	SLOPE,   // ��

	/* �ǂ̃u���b�N���� */
	WALL,    // �� 
	DUMMY,   // �_�~�[��
	CHECK,   // �`�F�b�N�|�C���g

	/* �F�̕t�����n�ʂ̃u���b�N���� */
	RED_BLOCK,     // �ԃu���b�N
	GREEN_BLOCK,   // �΃u���b�N
	YELLOW_BLOCK,  // ���u���b�N
	BLUE_BLOCK,    // �u���b�N

	BLOCK_MAX,  // �u���b�N�̍ő吔
};

/* �t�B�[���h�̎�� */
enum
{
	GRASS,     // ����
	SNOW,      // �ጴ
	MOUNTAIN,  // �R��
	BEACH,     // �l��
	MAX_FIELD, 
};

/* �v���C���[�̎�� */
enum
{
	RED_TANK,     // �ԃ^���N
	GREEN_TANK,   // �΃^���N
	YELLOW_TANK,  // ���^���N
	BLUE_TANK,    // �^���N
	TANK_ALL,
};

/**********************************
 * �O���[�o���֐��v���g�^�C�v�錾 *
 **********************************/
VOID ErrorDialog( LPCTSTR Message ); // ���b�Z�[�W�{�b�N�X�̕\��

#endif