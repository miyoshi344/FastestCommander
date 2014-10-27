/*********** 
 * Include *
 ***********/
#include "progress.h"

/******************
 * �R���X�g���N�^ *
 ******************/
Progress::Progress(VOID)
{
	/* �J�n�̑J�� */
	flow_no_ = TITLE;

	/* ��Ɣԍ����Ƃ̃N���X�i�[ */
	flow_[TITLE]      = new Title;      // �^�C�g���N���X
	flow_[SOLO_GAME]  = new SoloGame;   // �\���Q�[���N���X
	flow_[MULTI_GAME] = new MultiGame;  // �}���`�Q�[���N���X
	flow_[GAME_OVER]  = new Ending;     // �Q�[���I�[�o�[�N���X
	flow_[ENDING]     = new Ending;     // �G���f�B���O�N���X
}

/****************
 * �f�X�g���N�^ *
 ****************/
Progress::~Progress(VOID)
{
	/* ��Ɣԍ��̓��e��j������ */
	for (INT i = 0; i < MAX_FLOW; i++)
	{
		/* ���g�����݂���Ȃ� */
		if (flow_[i] != NULL)
		{
			/* ���g�̔j�� */
			flow_[i]->Destroy();
			SafeDelete(flow_[i]);
		}
	}
}

/*********************************************
 * �֐��� : Init�֐�                         *
 * ��  �e : �������������s���܂�             *
 * ��  �� : �Ȃ�                             *
 * �߂�l : �������ɐ���������TRUE��Ԃ��܂� *
 *********************************************/
BOOL Progress::Init(VOID)
{
	BOOL ret = TRUE;
	do
	{
		/* �m�ۂ����N���X������������ */
		for (INT i = 0; i < MAX_FLOW; i++)
		{
			/* �������� */
			if (!flow_[i]->Init())
			{
				/* �G���[ */
				ErrorDialog("�������Ɏ��s���܂���");
				ret = FALSE;
				break;
			}
		}

	} while (0);

	return ret;
}

/*******************************
 * �֐��� : UpDate�֐�         *
 * ��  �e : �X�V�������s���܂� *
 * ��  �� : �Ȃ�               *
 * �߂�l : �Ȃ�               *
 *******************************/
VOID Progress::UpDate(VOID)
{
	/* ���݂̍�Ɣԍ����擾 */
	INT progression = flow_[flow_no_]->UpDate();

	/* �Ⴄ�ԍ����Ԃ��ꂽ�� */
	if (flow_no_ != progression)
	{
		/* ��Ɣԍ���؂�ւ��� */
		flow_no_ = progression;

		/* ������ */
		flow_[flow_no_]->ReSet();
	}
}

/*******************************
 * �֐��� : Draw�֐�           *
 * ��  �e : �`�揈�����s���܂� *
 * ��  �� : �Ȃ�               *
 * �߂�l : �Ȃ�               *
 *******************************/
VOID Progress::Draw(VOID)
{
	/* ���݂̍�Ɣԍ��̕`����s�� */
	flow_[flow_no_]->Draw();
}

/*******************************
 * �֐��� : Destroy�֐�        *
 * ��  �e : �j���������s���܂� *
 * ��  �� : �Ȃ�               *
 * �߂�l : �Ȃ�               *
 *******************************/
VOID Progress::Destroy(VOID)
{
	/* ��Ɣԍ����Ƃ̓��e��j������ */
	for (INT i = 0; i < MAX_FLOW; i++)
	{
		/* ���g�����݂���Ȃ� */
		if (flow_ != NULL)
		{
			/* ���g��j������ */
			flow_[i]->Destroy();
			SafeDelete(flow_[i]);
		}
	}
}