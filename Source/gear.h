/************************
 * �M�A��\������N���X *
 ************************/
#ifndef RACE_GEAR_H_
#define RACE_GEAR_H_

/***********
 * Include *
 ***********/
#include "layout_base.h"

/* �萔 */

/************
 * ������� *
 ************/
CONST INT kGearSize  = Small;
CONST INT kGearColor = Red;

CONST INT kNumberCount = 6;

/********
 * �z�u *
 ********/
CONST D3DXVECTOR3 kGearPos = D3DXVECTOR3(920.0F, 540.0F, 0.0F);

CONST D3DXVECTOR3 kGearNamePos = D3DXVECTOR3(11.0F, 12.0F, 0.0);

/****************
 * �؂����` *
 ****************/
CONST INT kGearStartX = 12,  kGearStartY = 200,
		  kGearEndX   = 114, kGearEndY   = 95;

/****************************************
 * �M�A�̃X�v���C�g�\�����s���N���X�ł� *
 ****************************************/
class Gear : public LayoutBase
{
private:
	INT gear_;

	VOID NameSet(VOID);
	VOID DrawSet(VOID);
public:
	Gear(VOID);         // �R���X�g���N�^
	~Gear(VOID);        // �f�X�g���N�^

	VOID UpDate(VOID);  // �X�V����
	VOID Draw(VOID);    // �`�揈��

	VOID SetGearCount(INT set_gear_) { gear_ = set_gear_; }
};

#endif