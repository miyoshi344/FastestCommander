/***********
 * Include *
 ***********/
#include "flow_base.h"

/**********************
 * �ÓI�����o�ϐ��錾 *
 **********************/
INT FlowBase::player_count_ = 2;
INT FlowBase::field_map_    = GRASS;
INT FlowBase::total_point_[kAllPlayer];

/******************
 * �R���X�g���N�^ *
 ******************/
FlowBase::FlowBase(VOID)
{
	for (INT i = 0; i < kMaxPad; i++)
	{
		push_button_[i] = 0L;
	}
}

/****************
 * �f�X�g���N�^ *
 ****************/
FlowBase::~FlowBase(VOID)
{
}

