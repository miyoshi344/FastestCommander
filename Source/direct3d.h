/******************
 * Direct3D�N���X *
 ******************/

/* ���dInclude�h�~ */
#ifndef RACE_DIRECT3D_H_
#define RACE_DIRECT3D_H_

/***********
 * Include *
 ***********/
#include "common.h"
#include "release.h"

/***********************
 * �����萔/�}�N����` *
 ***********************/
#define D3DDEV (Direct3D::d3d_device()) // D3DDevice�����o�Ăяo���ȈՔ�

/********************************
 * Direct3D���Ǘ�����N���X�ł� *
 ********************************/
class Direct3D
{
private:
    static VOID SetMultiSamples( VOID );             // �A���`�G�C���A�X�̐ݒ���s���܂�
	static VOID SetPresentParameters(CONST HWND hWnd );        // D3DPRESENT_PARAMETERS�\���̂̐ݒ���s���܂�
    static BOOL CreateD3DDevice( CONST HWND hWnd );  // DirectD�f�o�C�X���쐬���܂�

	static LPDIRECT3D9            d3d_;              // Direct3D�C���^�[�t�F�C�X
	static LPDIRECT3DDEVICE9      d3d_device_;       // �f�o�C�X��IDirect3Device9�C���^�[�t�F�C�X�̃|�C���^
	static D3DPRESENT_PARAMETERS  present_;          // �v���[���e�[�V�����p�����[�^
	static D3DCAPS9               caps_;             // �n�[�h�E�F�A�\�͊i�[�\����

public:
    Direct3D( VOID );                                // �R���X�g���N�^
	static BOOL Init( CONST HWND hWnd );             // ������
	static VOID Destroy( VOID );                     // �j��

	static CONST LPDIRECT3DDEVICE9 d3d_device( VOID ) { return d3d_device_; }  // D3DDevice�̎擾
    static CONST LPDIRECT3D9 d3d( VOID ) { return d3d_; }                      // D3D�I�u�W�F�N�g�̎擾
};

#endif