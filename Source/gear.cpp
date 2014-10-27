/**********************
 * メモリリーク確認用 *
 **********************
#ifndef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DE | _CRTDBG_LEAK_CHECK_DF);
#endif

/***********
 * Include *
 ***********/
#include "gear.h"

/******************
 * コンストラクタ *
 ******************/
Gear::Gear(VOID)
{
	gear_ = 0;
}

/****************
 * デストラクタ *
 ****************/
Gear::~Gear(VOID)
{
}

/*******************************
 * 関数名 : UpDate関数         *
 * 内  容 : 更新処理を行います *
 * 引  数 : なし               *
 * 戻り値 : なし               *
 *******************************/
VOID Gear::UpDate(VOID)
{
}

/*******************************
 * 関数名 : Draw関数           *
 * 内  容 : 描画処理を行います *
 * 引  数 : なし               *
 * 戻り値 : なし               *
 *******************************/
VOID Gear::Draw(VOID)
{
	Sprite::DrawSprite(texture_, &kGearPos, kGearStartX, kGearStartY, kGearEndX, kGearEndY);
	DrawSet();
}

/*********************************
 * 関数名 : DrawSet関数          *
 * 内  容 : 文字の描画を行います *
 * 引  数 : なし                 *
 * 戻り値 : なし                 *
 *********************************/
VOID Gear::DrawSet(VOID)
{
	INT now_gear_[kNumberCount];
	INT gear_change_[kNumberCount];

	D3DXVECTOR3 GearPosition = kGearPos + kGearNamePos;
	D3DXVECTOR3 GearChangePosition = GearPosition;
	GearChangePosition.y += kSmallScoreEndY;

	/* 文字の配置 */
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