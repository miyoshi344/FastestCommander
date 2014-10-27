/***********
 * Include *
 ***********/
#include "flow_base.h"

/**********************
 * 静的メンバ変数宣言 *
 **********************/
INT FlowBase::player_count_ = 2;
INT FlowBase::field_map_    = GRASS;
INT FlowBase::total_point_[kAllPlayer];

/******************
 * コンストラクタ *
 ******************/
FlowBase::FlowBase(VOID)
{
	for (INT i = 0; i < kMaxPad; i++)
	{
		push_button_[i] = 0L;
	}
}

/****************
 * デストラクタ *
 ****************/
FlowBase::~FlowBase(VOID)
{
}

