/***********
 * Include *
 ***********/
#include "checkpoint.h"

/******************
 * �R���X�g���N�^ *
 ******************/
CheckPoint::CheckPoint(VOID)
{
	/* �v���C���[�̐��l�̃��Z�b�g */
	for (INT i = 0; i < kAllPlayer; i++)
	{
		player_check_[i].ReSet();
		return_position_[i] = D3DXVECTOR3(0.0F, 0.0F, 0.0F);
	}
}

/****************
 * �f�X�g���N�^ *
 ****************/
CheckPoint::~CheckPoint(VOID)
{
	Mesh::Release(my_mesh_);
	LayoutBase::Destroy();
}

/*********************************
 * �֐��� : Init�֐�             *
 * ��  �e : ���������s���܂�     *
 * ��  �� : �Ȃ�                 *
 * �߂�l : ����������TRUE��Ԃ� *
 *********************************/
BOOL CheckPoint::Init(VOID)
{
	/* ���[�J���ϐ� */
	BOOL ret = TRUE;

	do
	{
		/* �t�@�C���̓ǂݍ��� */
		if (!FileCheck())
		{
			/* �G���[ */
			ErrorDialog("�t�@�C���̓ǂݍ��݂̎擾�Ɏ��s");
			ret = FALSE;
			break;
		}

		/* �e�N�X�`���̓ǂݍ��� */
		if (!LayoutBase::Init())
		{
			/* �G���[ */
			ErrorDialog("�e�N�X�`���̏��擾�Ɏ��s");
			ret = FALSE;
			break;
		}

		/* �K�w���ړ����� */
		_chdir("Resource_fast/Mesh/Block");

		/* ���b�V���̓ǂݍ��� */
		//if ((my_mesh_ = Mesh::Load(kHitCheck)) == NULL)
		//{
		//	/* �G���[ */
		//	ErrorDialog("�u���b�N�̃��b�V�����̎擾�Ɏ��s");
		//	ret = FALSE;
		//	break;
		//}

		if ((my_mesh_ = Mesh::Load("wall.x")) == NULL)
		{
			ErrorDialog("�G���[");
			ret = FALSE;
		}

		/* �K�w��߂� */
		_chdir("../../../");

	} while (0);

	return ret;
}

/*********************************
 * �֐��� : ReSet�֐�            *
 * ��  �e : �������������s���܂� *
 * ��  �� : �Ȃ�                 *
 * �߂�l : �Ȃ�                 *
 *********************************/
VOID CheckPoint::ReSet(VOID)
{
	/* �v���C���[���̏����� */
	for (INT i = 0; i < kAllPlayer; i++)
	{
		player_check_[i].ReSet();
	}
}

/*******************************
 * �֐��� : UpDate�֐�         *
 * ��  �e : �X�V�������s���܂� *
 * ��  �� : �Ȃ�               *
 * �߂�l : �Ȃ�               *
 *******************************/
INT CheckPoint::UpDate(INT player_no_)
{
	///* �{�^�����͕ۑ��p */
	//ULONG push_button[kAllPlayer];

	///* �{�^�����͂��i�[ */
	//push_button[player_no_] = XInput::ReturnPushButton(player_no_);

	///* �\�����L�[�������ꂽ�� */
	//if (push_button[player_no_] == XINPUT_GAMEPAD_DPAD_DOWN)
	//{
	//	/* ���̃v���C���[���S�[�������� */
	//	player_check_[player_no_].lap = kFinalLap + 1;
	//}

	///* �\���L�[�オ�����ꂽ�� */
	//if (push_button[player_no_] == XINPUT_GAMEPAD_DPAD_UP)
	//{
	//	/* �S�����S�[�������� */
	//	for (INT i = 0; i < kAllPlayer; i++)
	//	{
	//		player_check_[i].lap = kFinalLap + 1;
	//	}
	//}

	return LapCheck();
}

/*********************************************
 * �֐��� : Draw�֐�                         *
 * ��  �e : �`�F�b�N�|�C���g�̕`����s���܂� *
 * ��  �� : INT player_no �v���C���[�ԍ�     *
 * �߂�l : �Ȃ�                             *
 *********************************************/
VOID CheckPoint::Draw(INT player_no)
{
	//it_matrix_ = check_matrix_.begin();

	//while (it_matrix_ != check_matrix_.end())
	//{
	//	Mesh::Draw(*it_matrix_, my_mesh_);

	//	++it_matrix_;
	//}
}

/*****************************************
 * �֐��� : TwoDraw�֐�                  *
 * ��  �e : 2�l�p�̕`�揈�����s���܂�    *
 * ��  �� : INT player_no �v���C���[�ԍ� *
 * �߂�l : �Ȃ�                         *
 *****************************************/
VOID CheckPoint::TwoDraw(INT player_no)
{
	TwoLapDraw(player_no);      // ���񐔂̕`��
	TwoRankingDraw(player_no);  // ���ʂ̕`��
}

/*****************************************
 * �֐��� : MultiDraw�֐�                *
 * ��  �e : ���l���p�̕`�揈�����s���܂� *
 * ��  �� : INT player_no �v���C���[�ԍ� *
 * �߂�l : �Ȃ�                         *
 *****************************************/
VOID CheckPoint::MultiDraw(INT player_no)
{
	MultiLapDraw(player_no);      // ���񐔂̕`��
	MultiRankingDraw(player_no);  // ���ʂ̕`��
}

/*********************************
 * �֐��� : Destroy�֐�          *
 * ��  �e : ���̔j�����s���܂� *
 * ��  �� : �Ȃ�                 *
 * �߂�l : �Ȃ�                 *
 *********************************/
VOID CheckPoint::Destroy(VOID)
{
	Mesh::Release(my_mesh_);     // ���b�V���̉��
	Texture::Release(texture_);  // �e�N�X�`���̉��
}

/*****************************************
 * �֐��� : TwoLapDraw�֐�               *
 * ��  �e : 2�l�p�̎����`�悵�܂�      *
 * ��  �� : INT player_no �v���C���[�ԍ� *
 * �߂�l : �Ȃ�                         *
 *****************************************/
VOID CheckPoint::TwoLapDraw(INT player_no)
{
	/* �z�u���W */
	D3DXVECTOR3 kMultiRankingPosition[kAllPlayer] =
	{
		D3DXVECTOR3(10.0F,                  kMultiY, 0.0F),  // �v���C���[1
		D3DXVECTOR3(10.0F, kMultiWindow.y + kMultiY, 0.0F),  // �v���C���[2
	};

	/* ���W�̍쐬 */
	D3DXVECTOR3 now_pos = kMultiRankingPosition[player_no];
	now_pos.x += kLapPosition;
	now_pos.y -= 15.0F;

	/* ���W�̍쐬 */
	D3DXVECTOR3 pos = now_pos;
	pos.x += (kLapPosition / 2);

	/* ���W�̍쐬 */
	D3DXVECTOR3 final_pos_ = now_pos;
	final_pos_.x += kLapPosition;

	/* �����̕`�� */
	DrawSet(kMultiRankingPosition[player_no], kLapRect, player_no);

	/* ���݂̎��񐔂ƍŏI����ڕW��`�� */
	CountSet(now_pos, player_check_[player_no].lap, MIDDLE, player_no, 1);

	pos.y += 10.0F;
	DrawSet(pos, kBarRect, player_no);
	CountSet(final_pos_, kFinalLap, MIDDLE, player_no, 1);

	/* �C�e���[�^�[�̐ݒ� */
	it_matrix_ = check_matrix_.begin();       // ���[���h���W

	//CountSet(D3DXVECTOR3(0.0F, 0.0F, 0.0F), player_check_[player_no].check_no, MIDDLE, player_no);
}

/*****************************************
 * �֐��� : MultiLapDraw�֐�             *
 * ��  �e : ���l���̎����`�悵�܂�     *
 * ��  �� : INT player_no �v���C���[�ԍ� *
 * �߂�l : �Ȃ�                         *
 *****************************************/
VOID CheckPoint::MultiLapDraw(INT player_no)
{
	/* �z�u���W */
	D3DXVECTOR3 kMultiRankingPosition[kAllPlayer] =
	{
		D3DXVECTOR3(10.0F                 , kMultiY                 , 0.0F),  // �v���C���[1
		D3DXVECTOR3(10.0F + kMultiWindow.x, kMultiY                 , 0.0F),  // �v���C���[2
		D3DXVECTOR3(10.0F                 , kMultiWindow.y + kMultiY, 0.0F),  // �v���C���[3
		D3DXVECTOR3(10.0F + kMultiWindow.x, kMultiWindow.y + kMultiY, 0.0F)   // �v���C���[4
	};

	/* ���W�̍쐬 */
	D3DXVECTOR3 now_pos = kMultiRankingPosition[player_no];
	now_pos.x += kLapPosition;
	now_pos.y -= 15.0F;

	/* ���W�̍쐬 */
	D3DXVECTOR3 pos = now_pos;
	pos.x += (kLapPosition / 2);

	/* ���W�̍쐬 */
	D3DXVECTOR3 final_pos_ = now_pos;
	final_pos_.x += kLapPosition;

	/* �����̕`�� */
	DrawSet(kMultiRankingPosition[player_no], kLapRect, player_no);
	
	/* ���݂̎��񐔂ƍŏI����ڕW��`�� */
	CountSet(now_pos, player_check_[player_no].lap, MIDDLE, player_no, 1);

	pos.y += 10.0F;
	DrawSet(pos, kBarRect, player_no);
	CountSet(final_pos_, kFinalLap, MIDDLE, player_no, 1);
}

/******************************************
 * �֐��� : TwoRankingDraw�֐�            *
 * ��  �e : 2�l�p�̃����L���O��`�悵�܂� *
 * ��  �� : INT player_no �v���C���[�ԍ�  *
 * �߂�l : �Ȃ�                          *
 ******************************************/
VOID CheckPoint::TwoRankingDraw(INT player_no)
{
	/* �z�u���W */
	D3DXVECTOR3 kMultiRankingPosition[kAllPlayer] =
	{
		D3DXVECTOR3(10.0F,                  kMultiY, 0.0F),  // �v���C���[1
		D3DXVECTOR3(10.0F, kMultiWindow.y + kMultiY, 0.0F),  // �v���C���[2
	};

	/* ���W�̍쐬 */
	D3DXVECTOR3 position = kMultiRankingPosition[player_no] + kMultiRankingPos;
	D3DXVECTOR3 position2 = position + kMultiRankPos;

	/* ���݂̃����L���O��`�� */
	CountSet(position, player_check_[player_no].ranking, MIDDLE, player_no, 1);
	DrawSet(position2, kRankingRect, player_no, 0);
}

/*******************************************
 * �֐��� : MultiRankingDraw�֐�           *
 * ��  �e : ���l���̃����L���O��`�悵�܂� *
 * ��  �� : INT player_no �v���C���[�ԍ�   *
 * �߂�l : �Ȃ�                           *
 *******************************************/
VOID CheckPoint::MultiRankingDraw(INT player_no)
{
	/* �z�u���W */
	D3DXVECTOR3 kMultiRankingPosition[kAllPlayer] =
	{
		D3DXVECTOR3(10.0F                 ,                  kMultiY, 0.0F),  // �v���C���[1
		D3DXVECTOR3(10.0F + kMultiWindow.x,                  kMultiY, 0.0F),  // �v���C���[2
		D3DXVECTOR3(10.0F                 , kMultiWindow.y + kMultiY, 0.0F),  // �v���C���[3
		D3DXVECTOR3(10.0F + kMultiWindow.x, kMultiWindow.y + kMultiY, 0.0F)   // �v���C���[4
	};

	/* ���W�̍쐬 */
	D3DXVECTOR3 position  = kMultiRankingPosition[player_no] + kMultiRankingPos;
	D3DXVECTOR3 position2 = position + kMultiRankPos;

	/* ���݂̃����L���O��`�� */
	CountSet(position, player_check_[player_no].ranking, MIDDLE, player_no, 1);
	DrawSet(position2, kRankingRect, player_no, 0);
}

/******************************************
 * �֐��� : RankingSort�֐�               *
 * ��  �e : ���ʂ��\�[�g���ĕԂ��܂�      *
 * ��  �� : INT player_no_ �v���C���[�ԍ� *
 * �߂�l : INT ranking   ����            *
 ******************************************/
VOID CheckPoint::RankingSort(INT player_no, INT player_count)
{
	INT ranking = 1;

	/* �����L���O���\�[�g���� */
	for (INT i = 0; i < player_count; i++)
	{
		/* ���񐔂����̃v���C���[��菭�Ȃ��Ȃ� */
		if (player_check_[player_no].lap < player_check_[i].lap)
		{
			/* ���ʂ�1���₷ */
			ranking++;
		}
		/* �`�F�b�N�|�C���g�����Ȃ��Ȃ� */
		else if (player_check_[player_no].lap == player_check_[i].lap &&
			player_check_[player_no].check_no < player_check_[i].check_no)
		{
			/* ���ʂ�1���₷ */
			ranking++;
		}
		/* �����������Ȃ� */
		else if (player_check_[player_no].lap == player_check_[i].lap &&
				 player_check_[player_no].check_no == player_check_[i].check_no &&
			     player_check_[player_no].vector > player_check_[i].vector)
		{
			/* ���ʂ�1���₷ */
			ranking++;
		}
	}

	/* ���ʂ�ۑ� */
	player_check_[player_no].ranking = ranking;   
}

/*********************************************************
 * �֐��� : LapCheck�֐�                                 *
 * ��  �e : �v���C���[�̃`�F�b�N�|�C���g���B���Ǘ����܂� *
 * ��  �� : �Ȃ�                                         *
 * �߂�l : ���݂̏�Ԃ�Ԃ��܂�                         *
 *********************************************************/
INT CheckPoint::LapCheck(VOID)
{
	INT point = NoPoint;

	/* �C�e���[�^�[�̐ݒ� */
	it_matrix_    = check_matrix_.begin();       // ���[���h���W
	it_position_  = check_position_.begin();     // ���W
	it_infomation = check_information_.begin();  // �u���b�N���

	D3DXVECTOR3* pos = new D3DXVECTOR3(0.0F, 0.0F, 0.0F);

	/* �ړ���̍��W����ړ��O�̍��W���������l�����C�Ƃ��Ĕ��肷�� */
	D3DXVECTOR3  ray = D3DXVECTOR3(-1.0F, 0.0F, -1.0F);

	/* �I���܂ő����� */
	while (it_matrix_ != check_matrix_.end())
	{
		/* 1�T�����ꍇ */
		if ((*it_infomation == 0) && (player_check_[player_no_].check_no == check_max_))
		{
			player_check_[player_no_].vector = D3DXVec3LengthSq(&(D3DXVECTOR3(*it_position_ - player_pos_[player_no_].player_pos_)));
		}
		else if (*it_infomation == player_check_[player_no_].check_no + 1)
		{
			player_check_[player_no_].vector = D3DXVec3LengthSq(&(D3DXVECTOR3(*it_position_ - player_pos_[player_no_].player_pos_)));
		}

		/* �v���C���[���߂��ɂ��邩�̔��� */
		if (D3DXVec3Length(&(D3DXVECTOR3(*it_position_ - player_pos_[player_no_].player_pos_))) < kCheckCollision)
		{
			/* �����蔻�� */
			if (collision_.RayIntersect(my_mesh_, *it_matrix_, player_pos_[player_no_].origin_pos_, ray, pos))
			{
				return_position_[player_no_] = *it_position_;

				/* 1�T�����ꍇ */
				if ((*it_infomation == 0) && player_check_[player_no_].check_no == check_max_)
				{
					player_check_[player_no_].lap++;  // �����1���₷
					player_check_[player_no_].check_no = *it_infomation;

					/* �߂�l��Ԃ� */
					point = WeekPoint;

					/* SE�̍Đ� */
					DSound::Restart(GOAL_SE);
					DSound::Play(GOAL_SE);
					break;
				}
				/* �`�F�b�N�|�C���g�Ȃ� */
				else if (player_check_[player_no_].check_no + 1 == *it_infomation)
				{
					player_check_[player_no_].check_no = *it_infomation;
					point = MiddlePoint;
					break;
				}
			}
		}

		++it_matrix_;
		++it_position_;
		++it_infomation;
	}

	return point;
}

/*************************************
 * �֐��� : FileCheck�֐�            *
 * ��  �e : �t�@�C�����J���Ǘ����܂� *
 * ��  �� : �Ȃ�                     *
 * �߂�l : ����������TRUE��Ԃ�     *
 *************************************/
BOOL CheckPoint::FileCheck(VOID)
{
	BOOL ret = TRUE;
	FILE* fp;

	INT   player    = 0;     // �v���C���[�ԍ�
	INT   check_no  = 0;     // �`�F�b�N�|�C���g�ԍ�
	FLOAT direction = 0.0F;  // �����̎w��
	CHAR  buf[NAME_LEN];     // �f�[�^�i�[�p

	D3DXVECTOR3 pos = D3DXVECTOR3(0.0F, 0.0F, 0.0F);
	D3DXMATRIX  world, rota, w_pos;

	/* �K�w���ړ����� */
	_chdir(check_no_);

	do
	{
		/* �t�@�C���̓ǂݍ��� */
		fopen_s(&fp, kCheckCsv, "r");

		/* �t�@�C�����ǂݍ��߂����m�F */
		if (!fp)
		{
			/* �G���[ */
			ErrorDialog("�u���b�N�̃t�@�C���ǂݍ��݂Ɏ��s���܂���");
			ret = FALSE;
			break;
		}

		/* �t�@�C���̏I�[�܂ŕ�����ǂݎ��\������ */
		while (fgets(buf, NAME_LEN, fp) != NULL)
		{
			for (UINT i = 0; i < strlen(buf); i++)
			{
				/* ���g�������Ȃ� */
				if (((buf[i] != ',') && (buf[i] != '\n') && (buf[i] != '-')))
				{
					/* �u*�v�Ȃ�X�^�[�g�ʒu��ݒ� */
					if (buf[i] == 's')
					{
						pos.y = 0.0F;
						player_pos_[player++].start_position_ = pos;
					}
					else
					{
						switch (buf[i])
						{
							case 'h' : direction = 0.0F; continue;
							case 'w' : direction = (90.0F / 180.0F) * D3DX_PI; continue;
							default : break;
						}

						check_no = (INT)buf[i] - (INT)'0';

						/* ���W��ۑ����� */
						D3DXMatrixIdentity(&world);  // ������
						D3DXMatrixRotationY(&rota, direction);  // ��]�l
						D3DXMatrixTranslation(&w_pos, pos.x, pos.y, pos.z);  // ���W�ʒu
						D3DXMatrixMultiply(&world, &rota, &w_pos);

						check_position_.push_back(pos);          // ���W���i�[
						check_matrix_.push_back(world);          // ���[���h���W���i�[
						check_information_.push_back(check_no);  // int�^�ɃL���X�g���Ċi�[

						/* �`�F�b�N�|�C���g�̍ő�l���擾 */
						if (check_max_ <= (check_no)) check_max_ = check_no;
					}
					i++;
				}
				pos.x += 1.0F;  // ���W�𓮂���
			}

			pos.x = 0.0F;  // �����̏�����
			pos.z -= 1.0F;  // �c���𓮂���
		}

	} while (0);

	/* �t�@�C������� */
	fclose(fp);

	/* �K�w���ړ����� */
	_chdir("../../../");

	return ret;
}