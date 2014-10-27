/***********
 * Include *
 ***********/
#include "solo_game.h"

/******************
 * �R���X�g���N�^ *
 ******************/
SoloGame::SoloGame(VOID)
{
}

/****************
 * �f�X�g���N�^ *
 ****************/
SoloGame::~SoloGame(VOID)
{
}

/*******************************
 * �֐��� : UpDate�֐�         *
 * ��  �e : �X�V�������s���܂� *
 * ��  �� : �Ȃ�               *
 * �߂�l : �Ȃ�               *
 *******************************/
INT SoloGame::UpDate(VOID)
{	
	/* �v���C���[�̍X�V */
	for (INT i = 0; i < kAllPlayer; i++)
	{
		/* ���̍X�V */
		GameSet(i);

		/* �v���C���[�̍X�V */
		GameUpDate(i);
	}

	return (field_map_ == MAX_FIELD) ? TITLE : SOLO_GAME;
}

/*******************************
 * �֐��� : Draw�֐�           *
 * ��  �e : �`�揈�����s���܂� *
 * ��  �� : �Ȃ�               *
 * �߂�l : �Ȃ�               *
 *******************************/
VOID SoloGame::Draw(VOID)
{
	//�J�����̍��W��ύX�i�r���[�s��j
	camera_[0].UpDate();
	player_[0]->Draw();

	/* ��ʂ̍X�V */
	field_[field_map_].SetPlayerNo(0);
	field_[field_map_].Draw();
}

/*****************************************
 * �֐��� : SpriteDraw�֐�               *
 * ��  �e : �X�v���C�g�`�揈�����s���܂� *
 * ��  �� : �Ȃ�                         *
 * �߂�l : �Ȃ�                         *
 *****************************************/
VOID SoloGame::SpriteDraw(VOID)
{
}