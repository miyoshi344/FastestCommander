/**********************
 * ���������[�N�m�F�p *
 **********************
#ifndef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DE | _CRTDBG_LEAK_CHECK_DF);
#endif

/***********
 * Include *
 ***********/
#include "gear.h"

/******************
 * �R���X�g���N�^ *
 ******************/
Gear::Gear(VOID)
{
	gear_ = 0;
}

/****************
 * �f�X�g���N�^ *
 ****************/
Gear::~Gear(VOID)
{
}

/*******************************
 * �֐��� : UpDate�֐�         *
 * ��  �e : �X�V�������s���܂� *
 * ��  �� : �Ȃ�               *
 * �߂�l : �Ȃ�               *
 *******************************/
VOID Gear::UpDate(VOID)
{
}

/*******************************
 * �֐��� : Draw�֐�           *
 * ��  �e : �`�揈�����s���܂� *
 * ��  �� : �Ȃ�               *
 * �߂�l : �Ȃ�               *
 *******************************/
VOID Gear::Draw(VOID)
{
	Sprite::DrawSprite(texture_, &kGearPos, kGearStartX, kGearStartY, kGearEndX, kGearEndY);
	DrawSet();
}

/*********************************
 * �֐��� : DrawSet�֐�          *
 * ��  �e : �����̕`����s���܂� *
 * ��  �� : �Ȃ�                 *
 * �߂�l : �Ȃ�                 *
 *********************************/
VOID Gear::DrawSet(VOID)
{
	INT now_gear_[kNumberCount];
	INT gear_change_[kNumberCount];

	D3DXVECTOR3 GearPosition = kGearPos + kGearNamePos;
	D3DXVECTOR3 GearChangePosition = GearPosition;
	GearChangePosition.y += kSmallScoreEndY;

	/* �����̔z�u */
	now_gear_[0] = Small_G;
	now_gear_[1] = Small_E;
	now_gear_[2] = Small_A;
	now_gear_[3] = Small_R;
	now_gear_[4] = 0;
	now_gear_[5] = gear_ + 2;

	gear_change_[0] = Small_C;
	gear_change_[1] = Small_H;
	gear_change_[2] = Small_A;
	gear_change_[3] = Small_N;
	gear_change_[4] = Small_G;
	gear_change_[5] = Small_E;

	DigitSet(GearPosition, now_gear_, kGearColor);
	DigitSet(GearChangePosition, gear_change_, kGearColor);
}