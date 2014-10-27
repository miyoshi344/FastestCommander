/********************
 * �v���C���[�N���X *
 ********************/

/* ���dInclude�h�~ */
#ifndef RACE_PLAYER_H_
#define RACE_PLAYER_H_

/***********
 * Include *
 ***********/
#include "character_base.h"
#include "bullet.h"

/************************************
 * �v���C���[�̊Ǘ����s���N���X�ł� *
 ************************************/
class Player : public CharacterBase
{
private:

	/************
	 * �����֐� *
	 ************/
	VOID Jump(VOID);             // �W�����v����
	VOID Direction(VOID);        // �ړ�����
	VOID TurretDirection(VOID);  // �C���̎��_�ړ�
	VOID Accel(VOID);            // �A�N�Z������

public:
	Player(VOID);        // �R���X�g���N�^
	~Player(VOID);       // �f�X�g���N�^

	BOOL Init(VOID);     // ��������
	VOID ReSet(VOID);    // ��ԏ�����
	VOID UpDate(VOID);   // �X�V����
	VOID Draw(VOID);     // �`�揈��
	VOID Destroy(VOID);  // �j������

	VOID ReplaySave(VOID);
};

#endif