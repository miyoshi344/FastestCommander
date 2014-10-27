/**************
* time�N���X *
**************/

/* ���dInclude�h�~ */
#ifndef RACE_TIME_H_
#define RACE_TIME_H_

/***********
 * Include *
 ***********/
#include "layout_base.h"

/************
 * ������� *
 ************/
CONST D3DXVECTOR3 kPosition = D3DXVECTOR3(300.0F, 500.0F, 0.0F);
CONST FLOAT kTimeInterval   = 50.0F;   // �Ԋu

/* �ǉ����� */
CONST INT MaxSecond = 30;  // 
CONST INT MaxSec    = 100;
CONST INT countUp   = 0;

/**********
* �\���� *
**********/

/* Far�p�\���� */
struct LapTime
{
	INT lap_count_;

	INT sec[3];

	INT lap1[3];
	INT lap2[3];
	INT lap3[3];

	D3DXVECTOR3 lap1_position_[3];
	D3DXVECTOR3 lap2_position_[3];
	D3DXVECTOR3 lap3_position_[3];

	LapTime(VOID)
	{
		lap_count_ = 0;

		for (INT i = 0; i < 3; i++)
		{
			sec[i] = 0;

			lap1[i] = 0;
			lap2[i] = 0;
			lap3[i] = 0;

			lap1_position_[i] = D3DXVECTOR3(0.0F, 0.0F, 0.0F);
			lap2_position_[i] = D3DXVECTOR3(0.0F, 0.0F, 0.0F);
			lap3_position_[i] = D3DXVECTOR3(0.0F, 0.0F, 0.0F);
		}
	}
};

/* �������ԗp�\���� */
struct LimitTime
{
	/* �^�C�� */
	INT second,
		sec;

	/* �R���X�g���N�^ */
	VOID ReSet(VOID)
	{
		second = MaxSecond;
		sec    = MaxSec;
	}
};

/****************************
* ���Ԃ��Ǘ�����N���X�ł� *
****************************/
class Time : public LayoutBase
{
private:

	/**************
	 * �\���̐錾 *
	 **************/
	LimitTime limit_;
	LapTime   total_;
	LapTime   lap_time_[kAllPlayer];

	/************
	 * �ϐ��錾 *
	 ************/
	INT lap_count_;
	BOOL TimeFlag;

	INT count_;
	INT count_down_;

	/************
	 * �����֐� *
	 ************/
	VOID SoloDraw(VOID);            // 1�l�p�`�揈��
	VOID MultiDraw(INT player_no);  // ���l���p�`�揈��

public:
	Time(VOID);            // �R���X�g���N�^
	~Time(VOID);           // �f�X�g���N�^

	VOID ReSet(VOID);
	VOID UpDate(VOID);     // �X�V����
	VOID LapUpDate(VOID);  // ����X�V����
	BOOL TimeLimit(VOID);  // �������ԏ���
	VOID Draw(VOID);       // �`�揈��
	VOID Lap(VOID);        // ����X�V����

	VOID Total(VOID);      // �g�[�^������

	BOOL TimeCount(VOID);      // �^�C���J�E���g����
	VOID TimeCountDraw(VOID);  // �^�C���J�E���g�`�揈��

	/* �^�C�����~�b�g�������� */
	VOID CheckPointLimit(VOID) { limit_.second += countUp; }

	/* �^�C�����~�b�g���� */
	BOOL ReturnFlag(VOID) { return TimeFlag; }
};

#endif