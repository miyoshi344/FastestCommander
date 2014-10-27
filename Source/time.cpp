/***********
 * Include *
 ***********/
#include "time.h"

/******************
 * �R���X�g���N�^ *
 ******************/
Time::Time(VOID)
{
	TimeFlag    = FALSE;  // ���~�b�g�t���O
	lap_count_  = 0;      // ���b�v�J�E���g
	count_      = 0;      // �~���b�J�E���g
	count_down_ = 5;      // �b�J�E���g
	limit_.ReSet();       // �^�C�����~�b�g������
}

/****************
 * �f�X�g���N�^ *
 ****************/
Time::~Time(VOID)
{
	/* �e�N�X�`���j�� */
	LayoutBase::Destroy();
}

/*********************************
 * �֐��� : ReSet�֐�            *
 * ��  �e : �������������s���܂� *
 * ��  �� : �Ȃ�                 *
 * �߂�l : �Ȃ�                 *
 *********************************/
VOID Time::ReSet(VOID)
{
	TimeFlag = FALSE;  // �^�C�����~�b�g�t���O
	lap_count_  = 0;   // ���b�v�J�E���g
	limit_.ReSet();    // �^�C�����~�b�g������
	count_      = 0;   // �~���b�J�E���g
	count_down_ = 5;   // �b�J�E���g
}

/*******************************
 * �֐��� : Update�֐�         *
 * ��  �e : �X�V�������s���܂� *
 * ��  �� : �Ȃ�               *
 * �߂�l : �Ȃ�               *
 *******************************/
VOID Time::UpDate(VOID)
{
	/* �^�C���̍X�V */
	total_.sec[player_no_]++;
	lap_time_[player_no_].sec[0]++;
	limit_.sec--;

	/* ���b�v���Ԃ̐ݒ� */

	/* �~���b�̐ݒ� */
	if (lap_time_[player_no_].sec[0] >= 100)
	{
		lap_time_[player_no_].sec[1]++;
		lap_time_[player_no_].sec[0] = 0;
	}

	/* �b�̐ݒ� */
	if (lap_time_[player_no_].sec[1] >= 60)
	{
		lap_time_[player_no_].sec[2]++;
		lap_time_[player_no_].sec[1] = 0;
	}

	/* �g�[�^�����Ԃ̐ݒ� */

	/* �~���b�̐ݒ� */
	if (total_.sec[0] >= 100)
	{
		total_.sec[1]++;
		total_.sec[0] = 0;
	}

	/* �b�̐ݒ� */
	if (total_.sec[1] >= 60)
	{
		total_.sec[2]++;
		total_.sec[1] = 0;
	}
}

/*******************************
 * �֐��� : Draw�֐�           *
 * ��  �e : �`�揈�����s���܂� *
 * ��  �� : �Ȃ�               *
 * �߂�l : �Ȃ�               *
 *******************************/
VOID Time::Draw(VOID)
{
	//MultiDraw(player_no_);
}

/*********************************************
 * �֐��� : TimeLimit�֐�                    *
 * ��  �e : �������Ԃ̏������s���܂�         *
 * ��  �� : �Ȃ�                             *
 * �߂�l : �������ԂɒB������TRUE��Ԃ��܂� *
 *********************************************/
BOOL Time::TimeLimit(VOID)
{
	BOOL ret = FALSE;

	/* ���~�b�g��0�ɂȂ�����TRUE��Ԃ� */
	if (limit_.second == 0 && limit_.sec == 0) ret = TRUE;
	else if (limit_.sec <= 0)
	{
		limit_.second--;      // �b�����炷
		limit_.sec = MaxSec;  // �~���b��������
	}

	return ret;
}

/*******************************
 * �֐��� : Draw�֐�           *
 * ��  �e : �`�揈�����s���܂� *
 * ��  �� : �Ȃ�               *
 * �߂�l : �Ȃ�               *
 *******************************/
VOID Time::SoloDraw(VOID)
{
	CountSet(D3DXVECTOR3(80.0F, 50.0F, 0.0F), limit_.second, LAGE);
	CountSet(D3DXVECTOR3(150.0F, 70.0F, 0.0F), limit_.sec, SMALL);
}

/*****************************************
 * �֐��� : MultiDraw�֐�                *
 * ��  �e : ���l���p�̕`�揈�����s���܂� *
 * ��  �� : INT player_no �v���C���[�ԍ� *
 * �߂�l : �Ȃ�                         *
 *****************************************/
VOID Time::MultiDraw(INT player_no)
{
	/* ���W�̍쐬 */
	D3DXVECTOR3 kMultiTime[kAllPlayer] = {
		{ kMultiWindow.x - kTimeInterval,                  50.0F, 0.0F },
		{ kWindowWidth   - kTimeInterval,                  50.0F, 0.0F },
		{ kMultiWindow.x - kTimeInterval, kMultiWindow.y + 50.0F, 0.0F },
		{ kWindowWidth   - kTimeInterval, kMultiWindow.y + 50.0F, 0.0F }
	};

	/* ���W�̐ݒ� */
	D3DXVECTOR3 pos = kMultiTime[player_no];

	/* �`����s�� */
	for (INT i = 0; i < 3; i++)
	{
		pos.x -= 75.0F;
		CountSet(pos, lap_time_[0].sec[i]);
	}
}

/************************************
 * �֐��� : Total�֐�               *
 * ��  �e : Total�̐��l��`�悵�܂� *
 * ��  �� : �Ȃ�                    *
 * �߂�l : �Ȃ�                    *
 ************************************/
VOID Time::Total(VOID)
{
	D3DXVECTOR3 pos = kPosition;
	pos.x += 50.0F;

	D3DXVECTOR3 pos2 = pos;
	pos2.x += 50.0F;

	/* �`��Z�b�g */
	CountSet(kPosition, total_.sec[2]); // ���̕`��
	CountSet(pos, total_.sec[1]);  // �b�̕`��
	CountSet(pos2, total_.sec[0]);     // �~���b�̕`��
}

/**********************************
 * �֐��� : Lap�֐�               *
 * ��  �e : Lap�̐��l��`�悵�܂� *
 * ��  �� : �Ȃ�                  *
 * �߂�l : �Ȃ�                  *
 **********************************/
VOID Time::Lap(VOID)
{

	for (INT i = 0; i < 3; i++)
	{
		for (INT n = 0; n < 3; n++)
		{
			CountSet(D3DXVECTOR3(400.0F + (100.0F * n), 100.0F, 0.0F), lap_time_[i].sec[n]);
		}
	}
}

/*****************************************
 * �֐��� : LapUpDate�֐�                *
 * ��  �e : ���b�v�^�C���̍X�V���s���܂� *
 * ��  �� : �Ȃ�                         *
 * �߂�l : �Ȃ�                         *
 *****************************************/
VOID Time::LapUpDate(VOID)
{
	for (INT i = 0; i < 3; i++)
	{
		switch (lap_time_[player_no_].lap_count_)
		{
			case 0:lap_time_[player_no_].lap1[i] = lap_time_[player_no_].sec[i]; break;
			case 1:lap_time_[player_no_].lap2[i] = lap_time_[player_no_].sec[i]; break;
			case 2:lap_time_[player_no_].lap3[i] = lap_time_[player_no_].sec[i]; break;
			default:break; 
		}

		lap_time_[player_no_].sec[i] = 0;
	}
}

/*****************************************************
 * �֐��� : TimeCount�֐�                            *
 * ��  �e : �J�E���g�_�E���̏������s���܂�           *
 * ��  �� : �Ȃ�                                     *
 * �߂�l : �J�E���g�_�E�����I��������TRUE��Ԃ��܂� *
 *****************************************************/
BOOL Time::TimeCount(VOID)
{
	if (!TimeFlag)
	{
		/* �J�E���g���K�萔�ɓ��B������ */
		if (count_++ >= 60)
		{
			/* �J�E���g�����Z�b�g */
			count_ = 0;


			if (count_down_ > 0)
			{
				//DSound::Pause(COUNT_SE);
				DSound::Play(COUNT_SE);
				count_down_--;
			}
		}

		/* �J�E���g���w�萔�ɓ��B������ */
		if (count_down_ == 0)
		{
			DSound::Pause(COUNT_SE);
			TimeFlag = TRUE;
			return TRUE;
		}
	}

	return FALSE;
}

/*******************************************
 * �֐��� : TimeCountDraw�֐�              *
 * ��  �e : �J�E���g�_�E���̕`����s���܂� *
 * ��  �� : �Ȃ�                           *
 * �߂�l : �Ȃ�                           *
 *******************************************/
VOID Time::TimeCountDraw(VOID)
{
	/* �`����W��ݒ� */
	D3DXVECTOR3 pos = D3DXVECTOR3(kMultiWindow.x, kMultiWindow.y, 0.0F);
	pos.x -= 25.0F;
	pos.y -= 25.0F;

	/* �J�E���g��`�� */
	CountSet(pos, count_down_, MIDDLE, RED, 1);
}