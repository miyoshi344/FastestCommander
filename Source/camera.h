/****************
 * �J�����N���X *
 ****************/

/* ���dInclude�h�~ */
#ifndef RACE_CAMERA_H_
#define RACE_CAMERA_H_

/***********
 * Include *
 ***********/
#include "common.h"
#include "direct3d.h"
#include "framerate.h"
#include "objectbase.h"

/*********************
 * �萔 / �}�N����c *
 *********************/
CONST D3DXVECTOR3 kCameraPosition  = D3DXVECTOR3(8.5F, 2.7F, 8.5F);  // �J�����̈ʒu
CONST D3DXVECTOR3 kCameraPrev      = D3DXVECTOR3(0.0F, 0.0F, 0.0F);  // �J�����̒����_
CONST D3DXVECTOR3 kCameraDirection = D3DXVECTOR3(0.0F, 1.0F, 0.0F);  // �J�����̌���

CONST D3DXCOLOR   LightDiffuse     = D3DXCOLOR(1.0F, 1.0F, 1.0F, 1.0F);  // ���̓����镔��
CONST D3DXCOLOR   Ambient          = D3DXCOLOR(0.5F, 0.5F, 0.5F, 1.0F);  // ����������Ȃ�����
CONST D3DXVECTOR3 ColorDirection   = D3DXVECTOR3(0.0F, -20.0F, 0.0F);    // ���̌���

CONST FLOAT Near = 1.0F;    // �J������near�l
CONST FLOAT Far  = 1000.0F; // �J������far�l

/********************************
 * �J�����̊Ǘ����s���N���X�ł� *
 ********************************/
class Camera
{
private:

	/********
	 * �ϐ� *
	 ********/
	D3DXVECTOR3 position_;     // �J�������W�ϊ��p
	D3DXMATRIX  view_, proj_;  // ���[���h���W�ϊ��p

	D3DXVECTOR3 pos_;    // �J�������W
	D3DXVECTOR3 point_;  // �J���������_

	FLOAT direction_;    // �v���C���[�p�x�i�[�p

	/************
	 * �����֐� *
	 ************/
	VOID Light(VOID); // ����
	
public:
	Camera(VOID);       // �R���X�g���N�^ 
	~Camera(VOID);      // �f�X�g���N�^

	BOOL Init(VOID);    // �J�����̏����z�u
	VOID UpDate(VOID);  // �X�V���� 

	VOID Aspect(D3DXVECTOR3 aspect);  // �A�X�y�N�g��ݒ�
	
	/* �v���C���[����̏��擾 */
	VOID Direction(FLOAT direction) { direction_ = direction; }
	VOID SetPosition(D3DXVECTOR3 pos) { position_ = pos; }
};
#endif