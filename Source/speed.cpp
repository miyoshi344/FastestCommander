/***********
* Include *
***********/
#include "speed.h"

/******************
* �R���X�g���N�^ *
******************/
Speed::Speed(VOID)
{
	speed_ = 0.0F;
}

/****************
* �f�X�g���N�^ *
****************/
Speed::~Speed(VOID)
{
	Texture::Release(texture_);
}

/*******************************
 * �֐��� : UpDate�֐�         *
 * ��  �e : �X�V�������s���܂� *
 * ��  �� : �Ȃ�               *
 * �߂�l : �Ȃ�               *
 *******************************/
VOID Speed::UpDate(VOID)
{
	speed_ *= 100.0F;
}

/**************************************
 * �֐��� : SoloDraw�֐�              *
 * ��  �e : 1�l�p�̕`�揈�����s���܂� *
 * ��  �� : �Ȃ�                      *
 * �߂�l :                           *
***************************************/
VOID Speed::SoloDraw(VOID)
{
}

/*****************************************
 * �֐��� : TwoDraw�֐�                  *
 * ��  �e : 2�l�p�̕`�揈�����s���܂�    *
 * ��  �� : INT player_no �v���C���[�ԍ� *
 * �߂�l : �Ȃ�                         *
 *****************************************/
VOID Speed::TwoDraw(INT player_no)
{
	D3DXVECTOR3 kTwoSpeedPosition[2] = {
		{ D3DXVECTOR3(kWindowWidth - kSpeedPos.x, kMultiWindow.y - kSpeedPos.y, 0.0F) },
		{ D3DXVECTOR3(kWindowWidth - kSpeedPos.x, kWindowHeight  - kSpeedPos.y, 0.0F) }
	};

	/* ���W�Z�b�g */
	D3DXVECTOR3 position = kTwoSpeedPosition[player_no] - kMultiSpeedPos;

	/* �`��Z�b�g */
	DrawSet(kTwoSpeedPosition[player_no], MeterRect, player_no);
	CountSet(position, (INT)speed_, MIDDLE, player_no);
}

/*****************************************
 * �֐��� : MultiDraw�֐�                *
 * ��  �e : ���l���p�̕`�揈�����s���܂� *
 * ��  �� : INT player_no �v���C���[�ԍ� *
 * �߂�l : �Ȃ�                         *
 *****************************************/
VOID Speed::MultiDraw(INT player_no)
{
	D3DXVECTOR3 kMultiSpeedPosition[kAllPlayer] = {
		{ D3DXVECTOR3(kMultiWindow.x - kSpeedPos.x, kMultiWindow.y - kSpeedPos.y, 0.0F) },
		{ D3DXVECTOR3(kWindowWidth   - kSpeedPos.x, kMultiWindow.y - kSpeedPos.y, 0.0F) },
		{ D3DXVECTOR3(kMultiWindow.x - kSpeedPos.x, kWindowHeight  - kSpeedPos.y, 0.0F) },
		{ D3DXVECTOR3(kWindowWidth   - kSpeedPos.x, kWindowHeight  - kSpeedPos.y, 0.0F) }
	};

	/* ���W�Z�b�g */
	D3DXVECTOR3 position = kMultiSpeedPosition[player_no] - kMultiSpeedPos;

	/* �`��Z�b�g */
	DrawSet(kMultiSpeedPosition[player_no], MeterRect, player_no);
	CountSet(position, (INT)speed_, MIDDLE, player_no);
}