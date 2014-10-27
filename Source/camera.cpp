/***********
 * Include *
 ***********/
#include "camera.h"

/******************
 * �R���X�g���N�^ *
 ******************/
Camera::Camera(VOID)
{
}

/****************
 * �f�X�g���N�^ *
 ****************/
Camera::~Camera(VOID)
{
}

/*******************************
 * �֐��� : Init�֐�           *
 * ��  �e : �J�����̐ݒ���s�� *
 * ��  �� : �Ȃ�               *
 * �߂�l : �Ȃ�               *
 *******************************/
BOOL Camera::Init(VOID)
{
	/* �P�ʍs��ŏ����� */
	D3DXMatrixIdentity(&proj_);

	/* �ˉe�s����쐬 */
	D3DXMatrixPerspectiveFovLH(&proj_,
		D3DXToRadian(45.0F),                           // ��p
		((FLOAT)kWindowWidth / (FLOAT)kWindowHeight),  // ��ʔ䗦
		Near,                                          // near�N���b�v��
		Far);                                          // far�N���b�v��

	/* �쐬�����ˉe�s���ݒ肷�� */
	D3DDEV->SetTransform(D3DTS_PROJECTION, &proj_);

	/* �J�����̍��W�A���_�A���������߂�r���[�s����쐬 */
	D3DXMatrixLookAtLH(&view_,
		&kCameraPosition,    // �J�������W
		&kCameraPrev,        // �����_���W
		&kCameraDirection);  // �J�����̌�����ԁi��j

	/* �쐬�����r���[�s���ݒ肷�� */
	D3DDEV->SetTransform(D3DTS_VIEW, &view_);

	return TRUE;
}

/*******************************
 * �֐��� : UpDate�֐�         *
 * ��  �e : �X�V�������s���܂� *
 * ��  �� : �Ȃ�               *
 * �߂�l : �Ȃ�               *
 *******************************/
VOID Camera::UpDate(VOID)
{
	Light();

	D3DXVECTOR3 camera_position_;
	D3DXVECTOR3 look_point_;

	D3DXMatrixIdentity(&view_);

	camera_position_ = D3DXVECTOR3(
		position_.x + sinf(direction_) * -kCameraPosition.x,
		kCameraPosition.y + position_.y,
		position_.z + cosf(direction_) * -kCameraPosition.z
		);

	look_point_ = D3DXVECTOR3(
		position_.x + sinf(direction_) * kCameraPosition.x,
		kCameraPosition.y + position_.y,
		position_.z + cosf(direction_) * kCameraPosition.z
		);


	// �|�W�V�����Əœ_�����߂�
	D3DXVECTOR3 prev_position = pos_,
		new_position = prev_position + (camera_position_ - prev_position),
		new_look_at_pos = point_ + (look_point_ - point_);

	/* �V������������W�ɍX�V */
	pos_   = new_position;
	point_ = new_look_at_pos;

	/* �J�����̍��W�A���_�A���������߂�r���[�s����쐬 */
	D3DXMatrixLookAtLH(&view_,
		&new_position,       // �J�������W
		&new_look_at_pos,    // �����_���W
		&kCameraDirection);  // �J�����̌�����ԁi��j

	/* �쐬�����r���[�s���ݒ肷�� */
	D3DDEV->SetTransform(D3DTS_VIEW, &view_);
}

/************************************************************
 * �֐��� : Light�֐�                                       *
 * ��  �e : ���̓�������̐ݒ�                              *
 * ��  �� : �Ȃ�                                            *
 * �߂�l : �Ȃ�                                            *
 ************************************************************/
VOID Camera::Light(VOID)
{
	// CONST D3DXCOLOR Diffuse(���̓����蕔���̐F)
	// CONST D3DXCOLOR Ambient(���̓�����Ȃ������̐F)
	// CONST D3DXVECTOR3 Direction(���̓��������)

	/* ���C�e�B���O�̐ݒ� */
	D3DLIGHT9 light_;
	ZeroMemory(&light_, sizeof(light_));  // ������
	light_.Type = D3DLIGHT_DIRECTIONAL;   // ���s����

	/* ���������镔���̐F */
	light_.Diffuse.r = LightDiffuse.r;  // ��
	light_.Diffuse.g = LightDiffuse.g;  // ��
	light_.Diffuse.b = LightDiffuse.b;  // ��
	light_.Diffuse.a = LightDiffuse.a;  // ���l(�����x)

	/* ����������Ȃ������̐F */
	light_.Ambient.r = Ambient.r;  // ��
	light_.Ambient.g = Ambient.g;  // ��
	light_.Ambient.b = Ambient.b;  // ��
	light_.Ambient.a = Ambient.a;  // ���l(�����x)

	/* ���̓����������ݒ� */
	light_.Direction.x = ColorDirection.x;
	light_.Direction.y = ColorDirection.y;
	light_.Direction.z = ColorDirection.z;

	/* ������ݒ������ */
	D3DDEV->SetLight(0, &light_);  // �����ɂ͌������
	D3DDEV->LightEnable(0, TRUE);  // ������0�@TRUE������

	/* �����_�����O�X�e�[�g�̐ݒ� */
	D3DDEV->SetRenderState(D3DRS_LIGHTING, TRUE);         // ���C�e�B���O����
	D3DDEV->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW );  // �J�����O���[�h
}

/*********************************************
 * �֐��� : Aspect�֐�                       *
 * ��  �e : �A�X�y�N�g���ݒ肵�Ȃ����܂�   *
 * ��  �� : D3DXVECTOR3 aspect�@�A�X�y�N�g�� *
 * �߂�l : �Ȃ�                             *
 *********************************************/
VOID Camera::Aspect(D3DXVECTOR3 aspect)
{
	/* �P�ʍs��ŏ����� */
	D3DXMatrixIdentity(&proj_);

	/* �ˉe�s����쐬 */
	D3DXMatrixPerspectiveFovLH(&proj_,
		D3DXToRadian(45.0F),   // ��p
		(aspect.x / aspect.y), // ��ʔ䗦
		Near,                  // near�N���b�v��
		Far);                  // far�N���b�v��

	/* �쐬�����ˉe�s���ݒ肷�� */
	D3DDEV->SetTransform(D3DTS_PROJECTION, &proj_);
}