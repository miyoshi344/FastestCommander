/***********
 * Include *
 ***********/
#include "game_base.h"

/******************
 * �R���X�g���N�^ *
 ******************/
GameBase::GameBase(VOID)
{
	/* ������ */
	for (INT i = 0; i < kAllPlayer; i++)
	{
		player_[i]       = NULL;        // ��U��ɂ���
		player_[i]       = new Player;  // �v���C���[�̏����擾
		ranking_flag_[i] = FALSE;       // �����L���O�t���O�̏�����
	}

	result_count_ = 0;                       // ���U���g��ʈڍs�v���C���[���̏�����
	pause_        = FALSE;                   // �|�[�Y�t���O

	ranking_.GetPlayerCount(player_count_);  // �v���C���[�l���̎擾

	/* �}�b�v�t�@�C���ǂݍ��� */
	field_[GRASS].SetMapNo(map_.FieldMap1);
	field_[SNOW].SetMapNo(map_.FieldMap2);
	field_[MOUNTAIN].SetMapNo(map_.FieldMap3);
	field_[BEACH].SetMapNo(map_.FieldMap4);

	/* �`�F�b�N�|�C���g�t�@�C���ǂݍ��� */
	check_point_[GRASS].SetMapNo(map_.FieldMap1);
	check_point_[SNOW].SetMapNo(map_.FieldMap2);
	check_point_[MOUNTAIN].SetMapNo(map_.FieldMap3);
	check_point_[BEACH].SetMapNo(map_.FieldMap4);
}

/****************
 * �f�X�g���N�^ *
 ****************/
GameBase::~GameBase(VOID)
{
	/* �t�B�[���h�̔j�� */
	for (INT i = 0; i < MAX_FIELD; i++)
	{
		check_point_[i].Destroy();  // �`�F�b�N�|�C���g�̔j��
		field_[i].Destroy();        // �t�B�[���h�̔j��
	}

	/* �v���C���[�̔j�� */
	for (INT i = 0; i < TANK_ALL; i++)
	{
		player_[i]->Destroy();  // �v���C���[�̔j��
		speed_[i].Destroy(); // �X�s�[�h�̔j��
	}

	time_.Destroy();  // �^�C���̔j��

	DSound::Pause(bgm_);
}

/***********************************
 * �֐��� : ReSet�֐�              *
 * ��  �e : ���l�̏��������s���܂� *
 * ��  �� : �Ȃ�                   *
 * �߂�l : �Ȃ�                   *
 ***********************************/
VOID GameBase::ReSet(VOID)
{
	/* �v���C���[�̃��Z�b�g */
	for (INT i = 0; i < player_count_; i++)
	{
		player_[i]->ReSet();       // �v���C���[�̏�����
		camera_[i].Init();         // �J�����̏�����
		ranking_flag_[i] = FALSE;  // �����L���O�t���O�̏�����

		/* �v���C���[�̏����z�u */
		player_[i]->StartPosition(check_point_[field_map_].ReturnStartPos(i));

		/* �v���C���[�ԍ��擾 */
		player_[i]->SetPlayerNo(i);
	}

	/* ���C�A�E�g�̃��Z�b�g */
	ranking_.ReSet();  // �����L���O��������
	time_.ReSet();     // �^�C����������

	/* �}�b�v�̃��Z�b�g */
	field_[field_map_].ReSet();        // �t�B�[���h�̏�����
	check_point_[field_map_].ReSet();  // �`�F�b�N�|�C���g�̏�����

	/* �ϐ��̏����� */
	pause_        = FALSE;  // �|�[�Y�t���O�̏�����
	result_count_ = 0;      // ���U���g��ʈڍs�v���C���[���̏�����

	/* �J�E���g�_�E���p */
	count_      = 0;        // �J�E���g������
	count_down_ = 3;        // �J�E���g������
}

/*************************************
 * �֐��� : Init�֐�                 *
 * ��  �e : �������������s���܂�     *
 * ��  �� : �Ȃ�                     *
 * �߂�l : ����������TRUE��Ԃ��܂� * 
 *************************************/
BOOL GameBase::Init(VOID)
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

		/* �^�C���̏����� */
		if (!time_.Init())
		{
			/* �G���[ */
			ErrorDialog("�e�N�X�`���̎擾�Ɏ��s");
			ret = FALSE;
			break;
		}

		/* �����L���O�̏����� */
		if (!ranking_.Init())
		{
			/* �G���[ */
			ErrorDialog("�e�N�X�`���̎擾�Ɏ��s");
			ret = FALSE;
			break;
		}

	} while (0);

	return ret;
}

/*******************************
 * �֐��� : Destroy�֐�        *
 * ��  �e : �j���������s���܂� *
 * ��  �� : �Ȃ�               *
 * �߂�l : �Ȃ�               *
 *******************************/
VOID GameBase::Destroy(VOID)
{
	time_.Destroy(); // �^�C���̔j��

	/* �t�B�[���h���̔j�� */
	for (INT i = 0; i < MAX_FIELD; i++)
	{
		check_point_[i].Destroy();  // �`�F�b�N�|�C���g�̔j��
		field_[i].Destroy();        // �t�B�[���h�̔j��
	}

	/* �v���C���[�̔j�� */
	for (INT i = 0; i < player_count_; i++)
	{
		player_[i]->Destroy();  // �v���C���[�̔j��
		speed_[i].Destroy();    // �X�s�[�h�̔j��
	}
}

/*********************************************
 * �֐��� : MapInit�֐�                      *
 * ��  �e : �}�b�v�֌W�̏������������s���܂� *
 * ��  �� : �Ȃ�                             *
 * �߂�l : �������ɐ���������TRUE��Ԃ��܂� *
 *********************************************/
BOOL GameBase::MapInit(VOID)
{
	BOOL ret = TRUE;

	/* �t�B�[���h���̏����� */
	for (INT i = 0; i < MAX_FIELD; i++)
	{
		/* �`�F�b�N�|�C���g�̏����� */
		if (!check_point_[i].Init())
		{
			ErrorDialog("�`�F�b�N�|�C���g�̎擾�Ɏ��s");
			ret = FALSE;
			break;
		}

		/* �t�@�C���ԍ����擾 */
		field_[i].SetFieldNo(i);

		/* �t�B�[���h�̏����� */
		if (!field_[i].Init())
		{
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
BOOL GameBase::PlayerInit(VOID)
{
	BOOL ret = TRUE;

	/* �v���C���[�̏����� */
	for (INT i = 0; i < kAllPlayer; i++)
	{
		camera_[i].Init();           // �J�����̏�����

		player_[i]->SetPlayerNo(i);  // �v���C���[�ԍ��̎擾

		/* �v���C���[�̏����z�u */
		player_[i]->StartPosition(check_point_[field_map_].ReturnStartPos(i));

		/* �v���C���[�̏����� */
		if (!player_[i]->Init())
		{
			/* �G���[ */
			ErrorDialog("�v���C���[�̃��b�V���擾�Ɏ��s");
			ret = FALSE;
			break;
		}

		/* �X�s�[�h�̏����� */
		if (!speed_[i].Init())
		{
			/* �G���[ */
			ErrorDialog("�e�N�X�`���̎擾�Ɏ��s");
			ret = FALSE;
			break;
		}
	}

	return ret;
}

/*************************************************
 * �֐��� : DrawPlayer�֐�                       *
 * ��  �e : �v���C���[���m�̕`��͈͂�ݒ肵�܂� *
 * ��  �� : INT player �v���C���[�ԍ�            *
 * �߂�l : �Ȃ�                                 *
 *************************************************/
VOID GameBase::DrawPlayer(INT player)
{
	/* �v���C���[�̕`��͈͂�ݒ� */
	for (INT i = 0; i < player_count_; i++)
	{
		/* �v���C���[���m���͈͓��ɂ���̂Ȃ� */
		if (D3DXVec3Length(&(D3DXVECTOR3(player_[player]->ReturnDrawPosition() - player_[i]->ReturnPosition()))) < kFieldDraw)
		{
			player_[i]->Draw();  // ���̃v���C���[�̉�ʂɑ��̃v���C���[��`�悷��
		}
	}
}

/***********************************************
 * �֐��� : CountDown�֐�                      *
 * ��  �e : �J�E���g�_�E�����s���܂�           *
 * ��  �� : �Ȃ�                               *
 * �߂�l : �J�E���g���I��������TRUE��Ԃ��܂� *
 ***********************************************/
BOOL GameBase::CountDowm(VOID)
{
	/* �J�E���g���w�萔�ɒB������ */
	if (count_++ >= 180)
	{
		/* �J�E���g���Z�b�g */
		count_ = 0;

		/* �b���܂��c���Ă���Ȃ� */
		if (count_down_ > 0)
		{
			count_down_--;  // �b��������
		}
	}

	/* �`�斳���Ȃ�����TRUE��Ԃ� */
	return (count_down_ <= 0) ? TRUE : FALSE;
}

/*****************************************
 * �֐��� : Pause�֐�                    *
 * ��  �e : �Q�[�����̈ꎞ��~�p�֐�     *
 * ��  �� : �Ȃ�                         *
 * �߂�l : �ꎞ��~���s���Ȃ�TRUE��Ԃ� *
 *****************************************/
BOOL GameBase::Pause(INT player_no)
{
	/* �X�^�[�g�{�^���������ꂽ��X�V���~�߂� */
	if (time_.ReturnFlag() && (!pause_ && (push_button_[player_no] == XINPUT_GAMEPAD_START)))
	{
		DSound::Pause(bgm_);  // BGM�̒�~
		pause_ = TRUE;        // �|�[�Y�t���O���I���ɂ���
	}
	/* �|�[�Y�t���O���I���̎��ɉ����ꂽ�� */
	else if (push_button_[player_no] == XINPUT_GAMEPAD_START)
	{
		DSound::Play(bgm_);  // BGM�̍Đ�
		pause_ = FALSE;      // �|�[�Y�t���O���I�t�ɂ���
	}

	/* �|�[�Y�t���O��Ԃ� */
	return pause_;
}

/*******************************************
 * �֐��� : GameSet�֐�                    *
 * ��  �e : �Q�[�����̏��̍X�V���s���܂� *
 * ��  �� : INT player  �v���C���[�ԍ�     *
 * �߂�l : �Ȃ�                           *
 *******************************************/
VOID GameBase::GameSet(INT player)
{
	/* �}�b�v���Ƃ�BGM�ݒ� */
	switch (field_map_)
	{
		case GRASS    : bgm_ = GRASS_BGM;    break;
		case SNOW     : bgm_ = SNOW_BGM;     break;
		case MOUNTAIN : bgm_ = MOUNTAIN_BGM; break;
		case BEACH    : bgm_ = BEACH_BGM;    break;
		default:break;
	}

	/* BGM�̍Đ� */
	DSound::Play(bgm_, DSBPLAY_LOOPING);

	/* �v���C���[�̐ݒ� */
	player_[player]->SetPlayerColor(RED_BLOCK + player);
	player_[player]->SetPlayerNo(player);

	/* �t�B�[���h�Ƃ̓����蔻��̍X�V */
	field_[field_map_].SetPlayerNo(player);
	field_[field_map_].SetOriginPos(player_[player]->ReturnOrigin());
	field_[field_map_].SetPlayerPos(player_[player]->ReturnPosition());
	field_[field_map_].SetDrawPos(player_[player]->ReturnDrawPosition());
	field_[field_map_].SetPlayerRadius(player_[player]->ReturnPlayerRadius());
	field_[field_map_].SetBulletRadius(player_[player]->ReturnBulletRadius());

	// �t�B�[���h�̍X�V*/
	field_[field_map_].UpDate();

	/* �J�����̐ݒ� */
	camera_[player].SetPosition(player_[player]->ReturnPosition());
	camera_[player].Direction(player_[player]->ReturnDirection());

	/* �X�s�[�h���[�^�[�̐ݒ� */
	speed_[player].SetPlayerSpeed(player_[player]->ReturnMove());
	speed_[player].UpDate();
}

/***************************************
 * �֐��� : GameUpDate�֐�             *
 * ��  �e : �Q�[�����̍X�V���s���܂�   *
 * ��  �� : INT player  �v���C���[�ԍ� *
 * �߂�l : �Ȃ�                       *
 ***************************************/
VOID GameBase::GameUpDate(INT player)
{
	/* �v���C���[�̍X�V */
	player_[player]->BlockCheck(field_[field_map_].BlockCheck());  // �v���C���[�̓������Ă���u���b�N�̔���
	player_[player]->UpDate();                                     // �v���C���[�̍X�V
	
	for (INT i = 0; i < kAllPlayer; i++)
	{
		if (!(i == player) && 
			collision_.SphereHitCheck(
			player_[player]->ReturnPosition(),
			player_[i]->ReturnPosition(),
			player_[player]->ReturnPlayerRadius(),
			player_[i]->ReturnBulletRadius()))
		{
			player_[player]->PlayerHit();
		}
	}

	player_[player]->ReturnPosition(check_point_[field_map_].ReturnPosition(player));

	/* �e�̔�����s�� */
	field_[field_map_].SetBulletPos(player_[player]->ReturnBullet());
	field_[field_map_].SetOriginBullet(player_[player]->ReturnOriginBullet());
	if (field_[field_map_].BulletCheck(RED_BLOCK + player)) player_[player]->BulletFlag();

	/* �`�F�b�N�|�C���g�̍X�V */
	check_point_[field_map_].SetPlayerNo(player);
	check_point_[field_map_].SetPlayerOrigin(player_[player]->ReturnOrigin());
	check_point_[field_map_].SetPlayerPosition(player_[player]->ReturnPosition());
	check_point_[field_map_].RankingSort(player, player_count_);

	// �`�F�b�N�|�C���g�̍X�V
	check_point_[field_map_].UpDate(player);

	/* �S�[������ */
	if (check_point_[field_map_].Goal(player))
	{
		GameResult(player);
	}
}

/*****************************************************
 * �֐��� : GameResult�֐�                           *
 * ��  �e : �Q�[�����̃}�b�v�ł̃��U���g��\�����܂� *
 * ��  �� : INT player  �v���C���[�ԍ�               *
 * �߂�l : �Ȃ�                                     *
 *****************************************************/
VOID GameBase::GameResult(INT player)
{
	/* �����L���O�t���O��TRUE�ɂȂ��Ă����� */
	if (!ranking_flag_[player])
	{
		/* �S�[��SE�̍Đ� */
		DSound::Restart(GOAL_SE);
		DSound::Play(GOAL_SE);

		/* �����L���O�̍X�V */
		ranking_.UpDate(player, check_point_[field_map_].ReturnRanking(player));
		ranking_flag_[player] = TRUE;  // �t���O��TRUE�ɂ���

		/* ���U���g�J�E���g�𑝂₷ */
		result_count_++;
		count_down_ = 3;
	}
}

/***************************************************
 * �֐��� : NextStage�֐�                          *
 * ��  �e : ���̃}�b�v�ֈړ�����Ԃ̏������s���܂� *
 * ��  �� : �Ȃ�                                   *
 * �߂�l : �Ȃ�                                   *
 ***************************************************/
BOOL GameBase::NextStage(VOID)
{
	/* ���U���g�J�E���g���v���C���[���ɒB������ */
	if (result_count_ >= player_count_)
	{
		/* �J�E���g�t���O���I���ɂȂ����� */
		if (CountDowm())
		{
			/* �v���C���[�S�� */
			for (INT i = 0; i < player_count_; i++)
			{
				ranking_flag_[i] = FALSE;  // �����L���O�t���O���I�t�ɂ���
				total_point_[i] += ranking_.ReturnPoint(i);  // �|�C���g���g�[�^���ɉ��Z����
			}

			/* �t�B�[���h�ԍ��𑝂₷ */
			field_map_++;

			/* BGM���~�߂� */
			DSound::Pause(bgm_);

			/* �t�B�[���h��S��������Ȃ� */
			if (field_map_ >= MAX_FIELD) return ENDING;  // �G���f�B���O�Ɉڍs
			else ReSet();  // �܂��̏ꍇ�͏�Ԃ�����������
		}
	}

	return MULTI_GAME;
}


