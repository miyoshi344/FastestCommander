/***********
 * Include *
 ***********/
#include "demoplay.h"

/******************
 * �R���X�g���N�^ *
 ******************/
DemoPlay::DemoPlay(VOID)
{
	/* ������ */
	for (INT i = 0; i < kAllPlayer; i++ )
	{
		player_[i] = NULL;          // ��ɂ���
		player_[i] = new PlayerAI;  // �v���C���[�̊i�[
	}

	player_count_ = 2;      // �v���C���[�l��
}

/****************
 * �f�X�g���N�^ *
 ****************/
DemoPlay::~DemoPlay(VOID)
{
	/* �t�B�[���h�̉�� */
	for (INT i = 0; i < MAX_FIELD; i++)
	{
		check_point_[i].Destroy();  // �`�F�b�N�|�C���g�̉��
		field_[i].Destroy();        // �t�B�[���h�̉��
	}

	/* �v���C���[�̉�� */
	for (INT i = 0; i < kAllPlayer; i++)
	{
		player_[i]->Destroy();   // �v���C���[�̉��
		SafeDelete(player_[i]);  // ���������������
	}
}

/*********************************************
 * �֐��� : Init�֐�                         *
 * ��  �e : �������������s��                 *
 * ��  �� : �Ȃ�                             *
 * �߂�l : �����������ɐ���������TRUE��Ԃ� *
 *********************************************/
BOOL DemoPlay::Init(VOID)
{
	BOOL ret = TRUE;

	do
	{
		/* �}�b�v�̏����� */
		if (!MapInit())
		{
			/* �G���[ */
			ErrorDialog("�}�b�v�̏������Ɏ��s���܂���");
			ret = FALSE;
			break;
		}

		/* �v���C���[�̏����� */
		if (!PlayerInit())
		{
			/* �G���[ */
			ErrorDialog("�v���C���[�̏������Ɏ��s���܂���");
			ret = FALSE;
			break;
		}

		/* ������ */
		ReSet();

	} while (0);

	return ret;
}

/***********************************
 * �֐��� : ReSet�֐�              *
 * ��  �e : ���l�̏��������s���܂� *
 * ��  �� : �Ȃ�                   *
 * �߂�l : �Ȃ�                   *
 ***********************************/
VOID DemoPlay::ReSet(VOID)
{
	field_map_ = kStartMap;
	field_[field_map_].ReSet();

	/* �v���C���[�̏����� */
	for (INT i = 0; i < kAllPlayer; i++)
	{
		player_[i]->ReSet();  // �v���C���[�̏�����
		camera_[i].Init();    // �J�����̏�����
		total_point_[i] = 0;  // �|�C���g�̏�����

		/* �v���C���[�̏����z�u */
		player_[i]->StartPosition(check_point_[field_map_].ReturnStartPos(i));

		/* �v���C���[�ԍ��擾 */
		player_[i]->SetPlayerNo(i);
	}
}

/*******************************
 * �֐��� : UpDate�֐�         *
 * ��  �e : �X�V�������s���܂� *
 * ��  �� : �Ȃ�               *
 * �߂�l : �Ȃ�               *
 *******************************/
BOOL DemoPlay::UpDate(VOID)
{
	/* �Q�[����ʂ̍X�V */
	for (INT i = 0; i < kAllPlayer; i++)
	{
		GameSet(i);
		GameUpDate(i);
	}

	return TRUE;
}

/*******************************
 * �֐��� : Draw�֐�           *
 * ��  �e : �`�揈�����s���܂� *
 * ��  �� : �Ȃ�               *
 * �߂�l : �Ȃ�               *
 *******************************/
VOID DemoPlay::Draw(VOID)
{
	BackGameDraw();  // �Q�[����ʂ̕`��
}

/***************************
 * �֐��� : Destroy�֐�    *
 * ��  �e : �j���������s�� *
 * ��  �� : �Ȃ�           *
 * �߂�l : �Ȃ�           *
 ***************************/
VOID DemoPlay::Destroy(VOID)
{
	/* �t�B�[���h�̉�� */
	for (INT i = 0; i < MAX_FIELD; i++)
	{
		check_point_[i].Destroy();  // �`�F�b�N�|�C���g�̔j��
		field_[i].Destroy();        // �t�B�[���h�̔j��
	}

	/* �v���C���[�̔j�� */
	for (INT i = 0; i < kMaxPad; i++)
	{
		player_[i]->Destroy();   // �v���C���[�̔j��
		SafeDelete(player_[i]);  // �v���C���[�̔j��
	}
}

/*******************************************
 * �֐��� : GameSet�֐�                    *
 * ��  �e : �Q�[�����̏��̍X�V���s���܂� *
 * ��  �� : INT player �v���C���[�ԍ�      *
 * �߂�l : �Ȃ�                           *
 *******************************************/
VOID DemoPlay::GameSet(INT player)
{
	/* �v���C���[�̐ݒ� */
	player_[player]->SetPlayerColor(RED_BLOCK + player);  // �v���C���[�̐F���
	player_[player]->SetPlayerNo(player);                 // �v���C���[�ԍ�

	/* �t�B�[���h�Ƃ̓����蔻��̍X�V */
	field_[field_map_].SetPlayerNo(player);                                // �v���C���[�ԍ����擾
	field_[field_map_].SetOriginPos(player_[player]->ReturnOrigin());      // �v���C���[�̍X�V�O�̍��W���擾
	field_[field_map_].SetPlayerPos(player_[player]->ReturnPosition());    // �v���C���[�̍X�V��̍��W���擾
	field_[field_map_].SetDrawPos(player_[player]->ReturnDrawPosition());  // �v���C���[�̕`����W���擾

	/* �t�B�[���h�̍X�V */
	field_[field_map_].UpDate();

	/* �J�����̐ݒ� */
	camera_[player].SetPosition(player_[player]->ReturnPosition());  // �v���C���[�̍��W���擾
	camera_[player].Direction(player_[player]->ReturnDirection());  // �v���C���[�̐���l���擾
}

/***************************************
 * �֐��� : GameUpDate�֐�             *
 * ��  �e : �Q�[�����̍X�V���s���܂�   *
 * ��  �� : INT player  �v���C���[�ԍ� *
 * �߂�l : �Ȃ�                       *
 ***************************************/
VOID DemoPlay::GameUpDate(INT player)
{
	/* �v���C���[�̍X�V */
	player_[player]->BlockCheck(field_[field_map_].BlockCheck());  // �v���C���[�̃u���b�N�̔���
	player_[player]->UpDate();                                     // �v���C���[�̍X�V

	/* �e�̔�����s�� */
	field_[field_map_].SetOriginBullet(player_[player]->ReturnOriginBullet());              // �e�̍X�V�O�̍��W���擾
	field_[field_map_].SetBulletPos(player_[player]->ReturnBullet());                       // �e�̍X�V��̍��W���擾
	if (field_[field_map_].BulletCheck(RED_BLOCK + player)) player_[player]->BulletFlag();  // �e�̐F�̏����擾

	/* �`�F�b�N�|�C���g�̍X�V */
	check_point_[field_map_].SetPlayerNo(player);                                   // �v���C���[�̔ԍ����擾
	check_point_[field_map_].SetPlayerOrigin(player_[player]->ReturnOrigin());      // �v���C���[�̍X�V�O�̍��W���擾
	check_point_[field_map_].SetPlayerPosition(player_[player]->ReturnPosition());  // �v���C���[�̍X�V��̍��W���擾              

	// �`�F�b�N�|�C���g�̍X�V
	check_point_[field_map_].UpDate(player);
}

/*******************************
 * �֐��� : BackGameDraw�֐�   *
 * ��  �e : �`�揈�����s���܂� *
 * ��  �� : �Ȃ�               *
 * �߂�l : �Ȃ�               *
 *******************************/
VOID DemoPlay::BackGameDraw(VOID)
{
	/* ��ʂ̕`�� */
	camera_[0].UpDate();  // �J�����̍X�V
	//player_[0]->Draw();   // �v���C���[�̕`��

	/* ��ʂ̍X�V */
	field_[field_map_].SetPlayerNo(0);
	field_[field_map_].Draw();
}

/*********************************************
 * �֐��� : MapInit�֐�                      *
 * ��  �e : �}�b�v�֌W�̏������������s���܂� *
 * ��  �� : �Ȃ�                             *
 * �߂�l : �������ɐ���������TRUE��Ԃ��܂� *
 *********************************************/
BOOL DemoPlay::MapInit(VOID)
{
	BOOL ret = TRUE;

	for (INT i = 0; i < MAX_FIELD; i++)
	{
		/* �`�F�b�N�|�C���g�̏����� */
		if (!check_point_[i].Init())
		{
			/* �G���[ */
			ErrorDialog("�`�F�b�N�|�C���g�̎擾�Ɏ��s");
			ret = FALSE;
			break;
		}

		field_[i].SetFieldNo(i);

		/* �t�B�[���h�̏����� */
		if (!field_[i].Init())
		{
			/* �G���[ */
			ErrorDialog("�u���b�N�̃��b�V���擾�Ɏ��s");
			ret = FALSE;
			break;
		}
	}

	return ret;
}

/*************************************************
 * �֐��� : PlayerInit�֐�                       *
 * ��  �e : �v���C���[�֌W�̏������������s���܂� *
 * ��  �� : �Ȃ�                                 *
 * �߂�l : �Ȃ�                                 *
 *************************************************/
BOOL DemoPlay::PlayerInit(VOID)
{
	BOOL ret = TRUE;

	/* �v���C���[�̏����� */
	for (INT i = 0; i < kMaxPad; i++)
	{
		/* �v���C���[�̏����z�u */
		player_[i]->StartPosition(check_point_[field_map_].ReturnStartPos(i));

		/* �v���C���[�ԍ��擾 */
		player_[i]->SetPlayerNo(i);

		/* �v���C���[�̏����� */
		if (!player_[i]->Init())
		{
			ErrorDialog("�v���C���[�̃��b�V���擾�Ɏ��s");
			ret = FALSE;
			break;
		}
	}

	return ret;
}

VOID DemoPlay::SpriteDraw(VOID)
{

}