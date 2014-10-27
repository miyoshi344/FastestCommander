/**********************
 * �v���C���[AI�N���X *
 **********************/

/* ���dInclude�h�~ */
#ifndef RACE_PLAYERAI_H_
#define RACE_PLAYERAI_H_

/***********
 * Include *
 ***********/
#include "character_base.h"
#include "bullet.h"

/**************************************
 * �v���C���[��AI���Ǘ�����N���X�ł� *
 * CharacterBase���p�����č쐬���܂�  *
 **************************************/
class PlayerAI : public CharacterBase
{
private:
	
	/********
	 * �ϐ� *
	 ********/
	LPCSTR ai_name_;

	/************
	 * �����֐� *
	 ************/
	BOOL AiInit(VOID);           // AI�e�L�X�g�̓ǂݍ��ݏ���
	VOID Jump(VOID);             // �W�����v����
	VOID Direction(VOID);        // �ړ�����
	VOID TurretDirection(VOID);  // �C�����_�ړ�
	VOID Accel(VOID);            // �A�N�Z������

public:
	PlayerAI(VOID);      // �R���X�g���N�^
	~PlayerAI(VOID);     // �f�X�g���N�^

	BOOL Init(VOID);     // ��������
	VOID ReSet(VOID);    // ��ԏ�����
	VOID UpDate(VOID);   // �X�V����
	VOID Draw(VOID);     // �`�揈��
	VOID Destroy(VOID);  // �j������
};

#endif