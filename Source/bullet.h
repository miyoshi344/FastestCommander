/**************
 * �e�̃N���X *
 **************/

/* ���dInclude�h�~ */
#ifndef RACE_BULLET_H_
#define RACE_BULLET_H_

/***********
 * Include *
 ***********/
#include "objectbase.h"

/********
 * �萔 *
 ********/
CONST FLOAT kBurretSpeed   = 0.4F;    // �e�̏����l
CONST FLOAT kBurretGravity = 0.007F;  // �d�͒l
CONST FLOAT kBurretHeight  = 0.7F;    // �e�̏o�鍂��
CONST FLOAT kBurretJump    = 0.1F;    // �e�̔�ѕ�

/****************************
 * �e�̊Ǘ�������N���X�ł� *
 ****************************/
class Bullet : public ObjectBase
{
private:
	/********
	 * �ϐ� *
	 ********/
	LPCSTR bullet_name_;  // �L�����N�^�[���Ƃ̒e�̐ݒ�

	FLOAT direction_;     // ����
	FLOAT move_;          // �����x
	FLOAT speed_;         // �����l
	FLOAT jump_;          // �����l

	/************
	 * �����֐� *
	 ************/
	BOOL BulletMeshInit(VOID);  // �e�̃��b�V���擾����
	VOID BulletMove(VOID);      // �e�̈ړ�����
	VOID BulletDraw(VOID);      // �e�̕`�揈��

public:
	Bullet(VOID);        // �R���X�g���N�^
	~Bullet(VOID);       // �f�X�g���N�^

	BOOL Init(VOID);     // �����ݒ�
	VOID ReSet(VOID);    // ������
	VOID UpDate(VOID);   // �X�V����
	VOID Draw(VOID);     // �`�揈��
	VOID Destroy(VOID);  // �j������

	/****************
	 * �l��Ԃ��֐� *
	 ****************/
	D3DXVECTOR3 ReturnOriginPosition(VOID) { return origin_pos_; }  // �X�V�O�̒e�̍��W
	D3DXVECTOR3 ReturnPosition(VOID) { return position_; }          // �X�V��̒e�̍��W

	/******************
	 * �l�����炤�֐� *
	 ******************/
	VOID BulletColor(LPCSTR bullet_name) { bullet_name_ = bullet_name; }  // �e�̏����擾����
	VOID BulletSet(D3DXVECTOR3 pos, FLOAT direction, FLOAT move);         // �e�̈ʒu���擾����
};

#endif