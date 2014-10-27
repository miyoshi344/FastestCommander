/***********
 * Include *
 ***********/
#include "ending.h"

/******************
 * �R���X�g���N�^ *
 ******************/
Ending::Ending(VOID)
{
	/* ������ */
	for (INT i = 0; i < kAllPlayer; i++)
	{
		total_point_[i]   = 0;
		total_ranking_[i] = 0;
	}
}

/****************
 * �f�X�g���N�^ *
 ****************/
Ending::~Ending(VOID)
{
	/* ���C�A�E�g�̉�� */
	LayoutBase::Destroy();

	demo_play_.Destroy();
}

/*********************************
 * �֐��� : Init�֐�             *
 * ��  �e : �������������s���܂� *
 * ��  �� : �Ȃ�                 *
 * �߂�l : ����������TRUE��Ԃ� *
 *********************************/
BOOL Ending::Init(VOID)
{
	BOOL ret = TRUE;

	do
	{
		/* �f���v���C�̏����� */
		if (!demo_play_.Init())
		{
			/* �G���[ */
			ErrorDialog("�f���v���C�̍쐬�Ɏ��s���܂���");
			ret = FALSE;
			break;
		}

		/* �t�H���_�K�w���ړ� */
		_chdir("Resource_fast/Texture");

		if (!(texture_ = Texture::LoadTexture(Result)))
		{
			/* �G���[ */
			break;
		}

		/* �t�H���_�K�w��߂� */
		_chdir("../../");

		/* �e�N�X�`�����̎擾 */
		if (!LayoutBase::Init())
		{
			/* �G���[ */
			ErrorDialog("�e�N�X�`���̎擾�Ɏ��s");
			ret = FALSE;
			break;
		}

		/* ������ */
		for (INT i = 0; i < kAllPlayer; i++)
		{
			total_point_[i]   = 0;
			total_ranking_[i] = 0;
		}

	} while (0);

	return ret;
}

/***********************************
 * �֐��� : ReSet�֐�              *
 * ��  �e : ���l�̏��������s���܂� *
 * ��  �� : �Ȃ�                   *
 * �߂�l : �Ȃ�                   *
 ***********************************/
VOID Ending::ReSet(VOID)
{
	/* �|�C���g�̐ݒ���s�� */
	for (INT i = 0; i < player_count_; i++)
	{
		total_point_[i] = SetTotalRanking(i);
	}

	/* �f���v���C�̏�Ԃ����������� */
	demo_play_.ReSet();

	/* �T�E���h�̍Đ��ʒu�������� */
	DSound::Restart(TITLE_BGM);
}

/*******************************
 * �֐��� : UpDate�֐�         *
 * ��  �e : �X�V�������s���܂� *
 * ��  �� : �Ȃ�               *
 * �߂�l : �Ȃ�               *
 *******************************/
INT Ending::UpDate(VOID)
{
	/* �v���C���[�̐������s�� */
	for (INT i = 0; i < player_count_; i++)
	{
		RankingSort(i);  // �����L���O�\�[�g
		push_button_[i] = XInput::ReturnPushButton(i);  // �{�^�����͂��擾

		/* �X�^�[�g�{�^���������ꂽ�� */
		if (push_button_[i] == XINPUT_GAMEPAD_START) return TITLE;
	}

	/* �f���v���C��ʂ̍X�V */
	demo_play_.UpDate();

	/* �T�E���h�̍Đ� */
	DSound::Play(TITLE_BGM, DSBPLAY_LOOPING);

	return ENDING;
}

/*******************************
 * �֐��� : Draw�֐�           *
 * ��  �e : �`�揈�����s���܂� *
 * ��  �� : �Ȃ�               *
 * �߂�l : �Ȃ�               *
 *******************************/
VOID Ending::Draw(VOID)
{
	/* �X�v���C�g�̕`�� */
	SpriteDraw();

	/* �f���v���C��ʂ̕`�� */
	demo_play_.Draw();
}

/*******************************
 * �֐��� : Destroy�֐�        *
 * ��  �e : �j���������s���܂� *
 * ��  �� : �Ȃ�               *
 * �߂�l : �Ȃ�               *
 *******************************/
VOID Ending::Destroy(VOID)
{
	/* ���C�A�E�g�̉�� */
	LayoutBase::Destroy();

	/* �f����ʂ̔j�� */
	demo_play_.Destroy();

}

/*****************************************
 * �֐��� : RankingSort�֐�              *
 * ��  �e : ���ʂ̃\�[�g���s���܂�       *
 * ��  �� : INT player_no �v���C���[�ԍ� *
 * �߂�l : �Ȃ�                         *
 *****************************************/
VOID Ending::RankingSort(INT player_no)
{
	INT ranking = 0;

	/* ���̃v���C���[�ƃ|�C���g���r */
	for (INT i = 0; i < player_count_; i++)
	{
		if (total_point_[player_no] < total_point_[i]) ranking++;  // �����菭�Ȃ��Ȃ珇�ʂ��グ��
	}

	/* �����L���O���i�[ */
	total_ranking_[player_no] = ranking;
}

/*******************************************
 * �֐��� : SpriteDraw�֐�                 *
 * ��  �e : �X�v���C�g�̕`�揈�����s���܂� *
 * ��  �� : �Ȃ�                           *
 * �߂�l : �Ȃ�                           *
 *******************************************/
VOID Ending::SpriteDraw(VOID)
{
	RECT rect;

	rect.left = 0;
	rect.top = 0;
	rect.right = 1280;
	rect.bottom = 720;

	Sprite::GetSprite()->Begin(D3DXSPRITE_ALPHABLEND);

	Sprite::DrawSprite(texture_, &D3DXVECTOR3(0.0F, 0.0F, 0.0F),rect);

	RankingDraw();  // �����L���O�̕`��

	Sprite::GetSprite()->End();
}

/*******************************************
 * �֐��� : RankingDraw�֐�                *
 * ��  �e : �����L���O�̕`�揈�����s���܂� *
 * ��  �� : �Ȃ�                           *
 * �߂�l : �Ȃ�                           *
 *******************************************/
VOID Ending::RankingDraw(VOID)
{
	/* �������W�̐ݒ� */
	D3DXVECTOR3 pos = kEnding;

	/* �v���C���[�̐������s�� */
	for (INT i = 0; i < player_count_; i++)
	{
		INT player_no = total_ranking_[i];

		/* �v���C���[�ԍ��̕`�� */
		DrawSet(pos, kPlayer, i, 1);

		/* ���W�̈ړ� */
		pos.x += 230.0F;
		pos.y += 20.0F;

		/* ���ʂ̕`�� */
		CountSet(pos, i, MIDDLE, i, 1);

		/* ���W�̈ړ� */
		pos.x += 75.0F;
		pos.y += 20.0F;

		/* �����̕`�� */
		DrawSet(pos, kRankingRect, i);

		/* ���W�̈ړ� */
		pos.x -= 275.0F;
		pos.y += 40.0F;

		/* �|�C���g�̕`�� */
		CountSet(pos, total_point_[total_ranking_[i]], MIDDLE, i, 2);

		/* ���W���ړ� */
		pos.x += 100.0F;

		/* �����̕`�� */
		DrawSet(pos, kTotalScore, i);

		/* ���W�̈ړ� */
		pos.x = kEnding.x;
		pos.y += 60.0F;
	}
}