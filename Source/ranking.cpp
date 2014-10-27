/***********
 * Include *
 ***********/
#include "ranking.h"

/******************
 * �R���X�g���N�^ *
 ******************/
Ranking::Ranking(VOID)
{
	/* �X�R�A�̏����� */
	for (INT i = 0; i < kAllPlayer; i++)
	{
		race_point_[i]    = 0;
		race_ranking_[i]  = 0;
	}
}

/****************
 * �f�X�g���N�^ *
 ****************/
Ranking::~Ranking(VOID)
{
	/* ���C�A�E�g�̔j�� */
	LayoutBase::Destroy();
}

/*******************************
 * �֐��� : Init�֐�           *
 * ��  �e : �����������s���܂� *
 * ��  �� : �Ȃ�               *
 * �߂�l : �Ȃ�               *
 *******************************/
BOOL Ranking::Init(VOID)
{
	BOOL ret = TRUE;

	do
	{
		/* ���C�A�E�g�̏����� */
		if (!LayoutBase::Init())
		{
			/* �G���[ */
			ErrorDialog("�e�N�X�`���̓ǂݍ��݂Ɏ��s");
			ret = FALSE;
			break;
		}

	} while (0);

	return ret;
}

/*****************************
 * �֐��� : ReSet�֐�        *
 * ��  �e : ���������s���܂� *
 * ��  �� : �Ȃ�             *
 * �߂�l : �Ȃ�             *
 *****************************/
VOID Ranking::ReSet(VOID)
{
	/* �X�R�A�̏����� */
	for (INT i = 0; i < kAllPlayer; i++)
	{
		race_point_[i]    = 0;
		race_ranking_[i]  = 0;
	}
}

/********************************************
 * �֐��� : UpDate�֐�                      *
 * ��  �e : �X�V�������s���܂�              *
 * ��  �� : INT player_no  �v���C���[�ԍ�   *
 *          INT ranking    �v���C���[�̏��� *
 * �߂�l : �Ȃ�                            *
 ********************************************/
VOID Ranking::UpDate(INT player_no, INT ranking)
{
	/* ���ʂɂ���Đ������i�[ */
	switch (ranking)
	{
		case 1: race_point_[player_no] = 10; break;
		case 2: race_point_[player_no] = 8; break;
		case 3: race_point_[player_no] = 6; break;
		case 4: race_point_[player_no] = 4; break;
		default:break;
	}

	/* ���ʂ��i�[ */
	race_ranking_[player_no] = ranking;
}

/*********************************************
 * �֐��� : SoloDraw�֐�                     *
 * ��  �e : ���[�X�̌��ʂ̃X�R�A��`�悵�܂� *
 * ��  �� : �Ȃ�                             *
 * �߂�l : �Ȃ�                             *
 *********************************************/
VOID Ranking::SoloDraw(VOID)
{
}

/*********************************************
 * �֐��� : TwoDraw�֐�                      *
 * ��  �e : ���[�X�̌��ʂ̃X�R�A��`�悵�܂� *
 * ��  �� : INT player_no �v���C���[�ԍ�     *
 * �߂�l : �Ȃ�                             *
 *********************************************/
VOID Ranking::TwoDraw(INT player_no)
{
	/* �����L���O���W */
	D3DXVECTOR3 race_ranking[2] =
	{
		D3DXVECTOR3(kRaceRankingPosition.x, kRaceRankingPosition.y, kRaceRankingPosition.z),  // �v���C���[1
		D3DXVECTOR3(kRaceRankingPosition.x, kRaceRankingPosition.y + kMultiWindow.y, kRaceRankingPosition.z),  // �v���C���[2
	};

	/* �X�R�A���W */
	D3DXVECTOR3 race_point[2] =
	{
		D3DXVECTOR3(kRacePointPosition.x, kRacePointPosition.y                 , kRacePointPosition.z),  // �v���C���[1
		D3DXVECTOR3(kRacePointPosition.x, kRacePointPosition.y + kMultiWindow.y, kRacePointPosition.z),  // �v���C���[3
	};

	/* �z�u�p���W */
	D3DXVECTOR3 pos;

	/* �����̍��W��ݒ� */
	pos = race_ranking[player_no];
	pos.x += kCountPosition;

	/* ���[�X�����L���O��`�� */
	DrawSet(race_ranking[player_no], kRaceRanking, player_no);

	/* �����L���O��`�� */
	CountSet(pos, race_ranking_[player_no], MIDDLE, player_no, 1);

	pos.x += 50.0F;
	pos.y += 15.0F;

	DrawSet(pos, kRankingRect, player_no);

	/* �����̍��W��ݒ� */
	pos = race_point[player_no];
	pos.x += kCountPosition;

	/* ���[�X�|�C���g��`�� */
	DrawSet(race_point[player_no], kGetPoint, player_no);

	/* �|�C���g��`�� */
	CountSet(pos, race_point_[player_no], MIDDLE, player_no);

	pos.x += 50.0F;
	pos.y += 15.0F;
	DrawSet(pos, kPoint, player_no);
}

/***********************************************
 * �֐��� : MultiDraw�֐�                      *
 * ��  �e : ���[�X���ʂ̃X�R�A�̕`����s���܂� *
 * ��  �� : INT player_no �v���C���[�ԍ�       *
 * �߂�l : �Ȃ�                               *
 ***********************************************/
VOID Ranking::MultiDraw(INT player_no)
{
	/* �����L���O���W */
	D3DXVECTOR3 race_ranking[kAllPlayer] =
	{
		D3DXVECTOR3(kRaceRankingPosition.x,                  kRaceRankingPosition.y,                  kRaceRankingPosition.z),  // �v���C���[1
		D3DXVECTOR3(kRaceRankingPosition.x + kMultiWindow.x, kRaceRankingPosition.y,                  kRaceRankingPosition.z),  // �v���C���[2
		D3DXVECTOR3(kRaceRankingPosition.x,                  kRaceRankingPosition.y + kMultiWindow.y, kRaceRankingPosition.z),  // �v���C���[3
		D3DXVECTOR3(kRaceRankingPosition.x + kMultiWindow.x, kRaceRankingPosition.y + kMultiWindow.y, kRaceRankingPosition.z)   // �v���C���[4
	};

	/* �g�[�^���X�R�A���W */
	D3DXVECTOR3 race_point[kAllPlayer] =
	{
		D3DXVECTOR3(kRacePointPosition.x,                  kRacePointPosition.y,                  kRacePointPosition.z),  // �v���C���[1
		D3DXVECTOR3(kRacePointPosition.x + kMultiWindow.x, kRacePointPosition.y,                  kRacePointPosition.z),  // �v���C���[2
		D3DXVECTOR3(kRacePointPosition.x,                  kRacePointPosition.y + kMultiWindow.y, kRacePointPosition.z),  // �v���C���[3
		D3DXVECTOR3(kRacePointPosition.x + kMultiWindow.x, kRacePointPosition.y + kMultiWindow.y, kRacePointPosition.z),  // �v���C���[4
	};

	/* �z�u�p���W */
	D3DXVECTOR3 pos;

	/* �����̍��W��ݒ� */
	pos = race_ranking[player_no];
	pos.x += kCountPosition;

	/* ���[�X�����L���O��`�� */
	DrawSet(race_ranking[player_no], kRaceRanking, player_no);

	/* �����L���O��`�� */
	CountSet(pos, race_ranking_[player_no], MIDDLE, player_no, 1);
	
	pos.x += 50.0F;
	pos.y += 15.0F;

	DrawSet(pos, kRankingRect, player_no);

	/* �����̍��W��ݒ� */
	pos = race_point[player_no];
	pos.x += kCountPosition;

	/* ���[�X�|�C���g��`�� */
	DrawSet(race_point[player_no], kGetPoint, player_no);

	/* �|�C���g��`�� */
	CountSet(pos, race_point_[player_no], MIDDLE, player_no);

	pos.x += 50.0F;
	pos.y += 15.0F;
	DrawSet(pos, kPoint, player_no);
}

/*******************************
 * �֐��� : Destroy�֐�        *
 * ��  �e : �j���������s���܂� *
 * ��  �� : �Ȃ�               *
 * �߂�l : �Ȃ�               *
 *******************************/
VOID Ranking::Destroy(VOID)
{
	/* ���C�A�E�g�j�� */
	LayoutBase::Destroy();
}