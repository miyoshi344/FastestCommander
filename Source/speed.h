/***************
 * speed�N���X *
 ***************/

/* ���dInclude�h�~ */
#ifndef RACE_SPEED_H_
#define RACE_SPEED_H_

/***********
 * Include *
 ***********/
#include "layout_base.h"

/********
 * �萔 *
 ********/

/* �z�u���W */
CONST D3DXVECTOR3 kSpeedPos      = D3DXVECTOR3(160.0F, 70.0F, 0.0F);  // �����̍��W���ߗp
CONST D3DXVECTOR3 kMultiSpeedPos = D3DXVECTOR3(-50.0F, 50.0F, 0.0F);  // �����̍��W���ߗp

/****************************************
 * �X�s�[�h���Ǘ�����N���X�ł�         *
 * LayoutBase�N���X���p�����č쐬���܂� *
 ****************************************/
class Speed : public LayoutBase
{
private:

	/********
	 * �ϐ� *
	 ********/
	FLOAT speed_;

	/************
	 * �����֐� *
	 ************/
	VOID SpeedCount(VOID);  // �X�s�[�h�\��

public:
	Speed(VOID);                    // �R���X�g���N�^
	~Speed(VOID);                   // �f�X�g���N�^

	VOID UpDate(VOID);              // �X�V����
	VOID SoloDraw(VOID);            // �`�揈��
	VOID TwoDraw(INT player_no);    // 2�l�p�`�揈��
	VOID MultiDraw(INT player_no);  // ���l���p�`�揈��

	/* �v���C���[�̑��x��\�� */
	VOID SetPlayerSpeed(FLOAT move) { speed_ = move; }
};

#endif