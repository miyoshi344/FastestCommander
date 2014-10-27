/* Include */
#include "field.h"

/******************
 * �R���X�g���N�^ *
 ******************/
Field::Field(VOID)
{
	/* ������ */
	for (INT i = 0; i < BLOCK_MAX; i++)
	{
		block[i] = NULL;
		block[i] = new FieldObject;
	}
}

/****************
 * �f�X�g���N�^ *
 ****************/
Field::~Field(VOID)
{
	/* �j�����s�� */
	for (INT i = 0; i < BLOCK_MAX; i++)
	{
		block[i]->Destroy();
		SafeDelete(block[i]);
	}
}

/***********************************
 * �֐��� : Init�֐�               *
 * ��  �e : �������������s���܂�   *
 * ��  �� : �Ȃ�                   *
 * �߂�l : �������ɐ���������TRUE *
 ***********************************/
BOOL Field::Init(VOID)
{
	BOOL ret = TRUE;    // ����p

	do
	{
		/* �X�J�C�{�[�������� */
		if (!SkyInit())
		{
			/* �G���[ */
			ErrorDialog("�X�J�C�{�[���̏������Ɏ��s���܂���");
			ret = FALSE;
			break;
		}

		/* �u���b�N������ */
		if (!BlockInit())
		{
			/* �G���[ */
			ErrorDialog("�u���b�N�̏������Ɏ��s���܂���");
			ret = FALSE;
		}

		/* �}�b�v������ */
		if (!MapFileSet())
		{
			/* �G���[ */
			ErrorDialog("�t�@�C���̓ǂݍ��݂Ɏ��s���܂���");
			ret = FALSE;
		}

	} while (0);

	return ret;
}

/***********************************
 * �֐��� : ReSet�֐�              *
 * ��  �e : ��Ԃ̏��������s���܂� *
 * ��  �� : �Ȃ�                   *
 * �߂�l : �Ȃ�                   *
 ***********************************/
VOID Field::ReSet(VOID)
{
	information_list_ = info_list_;
}

/*******************************
 * �֐��� : UpDate�֐�         *
 * ��  �e : �X�V�������s���܂� *
 * ��  �� : �Ȃ�               *
 * �߂�l : �Ȃ�               *
 *******************************/
VOID Field::UpDate(VOID)
{
	/* �����蔻��̎擾 */
	PlayerHitCheck();
}

/*******************************
 * �֐��� : Draw�֐�           *
 * ��  �e : �`�揈�����s���܂� *
 * ��  �� : �Ȃ�               *
 * �߂�l : �Ȃ�               *
 *******************************/
VOID Field::Draw(VOID)
{
	/* �t�B�[���h�̕`�� */
	FieldDraw();

	/* �X�J�C�{�[���̕`�� */
	SkyDraw();
}

/*******************************
 * �֐��� : Destroy�֐�        *
 * ��  �e : �j���������s���܂� *
 * ��  �� : �Ȃ�               *
 * �߂�l : �Ȃ�               *
 *******************************/
VOID Field::Destroy(VOID)
{
	collision_.Destroy();

	for (INT i = 0; i < BLOCK_MAX; i++)
	{
		block[i]->Destroy();
	}
}

/***********************************************************
 * �֐��� : BlockCheck�֐�                                 *
 * ��  �e : �v���C���[�����ݐG��Ă���u���b�N���擾���܂� *
 * ��  �� : �Ȃ�                                           *
 * �߂�l : ���������u���b�N�̑�����Ԃ��܂�               *
 ***********************************************************/
INT Field::BlockCheck(VOID)
{
	/* �C�e���[�^ */
	it_player_pos_        = player_pos_list_.begin();
	it_player_matrix_     = player_list_.begin();
	it_player_infomation_ = player_information_.begin();
	it_radius_            = player_radius_list_.begin();
	it_player_end_        = player_list_.end();

	D3DXVECTOR3* pos = new D3DXVECTOR3(0.0F, 0.0F, 0.0F);
	D3DXVECTOR3 ray = player_pos_[player_] - origin_pos_[player_];
	ray.y = 0.1F;

	/* ���X�g�̍Ō�܂Ŕ��肷�� */
	while (it_player_matrix_ != it_player_end_)
	{
		if (!(*it_player_infomation_ == SLOPE || *it_player_infomation_ == WALL))
		{
			/* �����蔻����擾���܂� */
			if (collision_.RayIntersect(block[*it_player_infomation_]->ReturnMesh(), *it_player_matrix_, origin_pos_[player_], ray, pos))
			{
				/* �`�F�b�N�|�C���g�Ȃ� */
				if (*it_player_infomation_ == CHECK) return GROUND;

				return *it_player_infomation_;
			}
		}
		else
		{
			if(collision_.SphereHitCheck(
				player_pos_[player_],
				*it_player_pos_,
				player_radius_[player_],
				*it_radius_))
			{
				return *it_player_infomation_;
			}
		}

		/* �C�e���[�^��i�߂� */
		++it_player_matrix_; 
		++it_player_infomation_;
		++it_radius_;
	}

	return BLOCK_MAX;
}

/***********************************************
 * �֐��� : BulletCheck�֐�                    *
 * ��  �e : �e�̓��������u���b�N������s���܂� *
 * ��  �� : �Ȃ�                               *
 * �߂�l : ����������TRUE��Ԃ��܂�           *
 ***********************************************/
BOOL Field::BulletCheck(INT PlayerColor)
{
	/* �t�B�[���h�̍��W���X�V */
	it_bullet_matrix_     = matrix_list_.begin();
	it_bullet_position_   = pos_list_.begin();
	it_bullet_infomation_ = information_list_.begin();
	it_bullet_end_        = matrix_list_.end();

	D3DXVECTOR3* pos = new D3DXVECTOR3(0.0F, 0.0F, 0.0F);
	D3DXVECTOR3 ray = bullet_pos_[player_] - origin_bullet_[player_];

	/* �����蔻�������͈͂𔻒肷�� */
	while (it_bullet_matrix_ != it_bullet_end_)
	{
		if (D3DXVec3Length(&(D3DXVECTOR3(*it_bullet_position_ - bullet_pos_[player_]))) < kFieldCollision)
		{
			/* �����蔻����擾���܂� */
			if (collision_.SphereHitCheck(
					*it_bullet_position_,
					bullet_pos_[player_],
					radius_,
					bullet_radius_[player_]))
			{
				/* �F�����Ȃ��u���b�N�Ȃ�F��ς����ɕԂ� */
				if ((*it_bullet_infomation_ == WALL)  ||
					(*it_bullet_infomation_ == SLOPE) ||
					(*it_bullet_infomation_ == DUMMY))
				{
					return TRUE;
				}

				/* �ς�����u���b�N�Ȃ�F��ς��� */
				*it_bullet_infomation_ = PlayerColor;
				return TRUE;
			}
		}

		/* �C�e���[�^��i�߂� */
		++it_bullet_matrix_;
		++it_bullet_position_;
		++it_bullet_infomation_;
	}

	return FALSE;
}

/*********************************************
 * �֐��� : SkyInit�֐�                      *
 * ��  �e : �X�J�C�{�[���̏��������s���܂�   *
 * ��  �� : �Ȃ�                             *
 * �߂�l : �������ɐ���������TRUE��Ԃ��܂� *
 *********************************************/
BOOL Field::SkyInit(VOID)
{
	BOOL ret = TRUE;

	/* �t�H���_�K�w���ړ����� */
	_chdir("Resource_fast/Mesh");

	do
	{
		/* �L�����N�^�[�̃��b�V���̎擾 */
		if ((my_mesh_ = Mesh::Load(kSkyBall)) == NULL)
		{
			/* �v���C���[���b�V���̏��擾�Ɏ��s */
			ErrorDialog("���b�V���̏��擾�Ɏ��s���܂���");
			ret = FALSE;
			break;
		}

		/* �K�w���ړ����� */
		_chdir("../../");

	} while (0);

	return ret;
}

/*********************************************
 * �֐��� : BlockInit�֐�                    *
 * ��  �e : �u���b�N�̏��������s���܂�       *
 * ��  �� : �Ȃ�                             *
 * �߂�l : �������ɐ���������TRUE��Ԃ��܂� *
 *********************************************/
BOOL Field::BlockInit(VOID)
{
	BOOL ret = TRUE;

	do
	{
		/* �X�e�[�W���Ƃ̓ǂݍ��݃��b�V�����擾 */
		switch (field_no_)
		{
			case GRASS:
				block[GROUND]->SetMeshName(kGrassGround);  // ���̃u���b�N
				break;

			case SNOW:
				block[GROUND]->SetMeshName(kSnowGround);   // ��̃u���b�N
				break;

			case MOUNTAIN:
				block[GROUND]->SetMeshName(kRockyGround);  // ��̃u���b�N
				break;

			case BEACH:
				block[GROUND]->SetMeshName(kSandGround);   // ���̃u���b�N
				break;
		}

		/* ���ʂ̃��b�V�����擾 */
		block[THORN]->SetMeshName(kThorn);
		block[SLOPE]->SetMeshName(kSlope);
		block[CHECK]->SetMeshName(kCheck);
		block[WALL]->SetMeshName(kWall);
		block[DUMMY]->SetMeshName(kWall);

		/* �e�F�̃��b�V�����擾 */
		block[RED_BLOCK]->SetMeshName(kRedBlock);
		block[GREEN_BLOCK]->SetMeshName(kGreenBlock);
		block[YELLOW_BLOCK]->SetMeshName(kYellowBlock);
		block[BLUE_BLOCK]->SetMeshName(kBlueBlock);

		/* ������ */
		for (INT i = 0; i < BLOCK_MAX; i++)
		{
			/* �e�u���b�N�̏��������� */
			if (!block[i]->Init())
			{
				/* �u���b�N�̏��擾�Ɏ��s */
				ErrorDialog("�u���b�N�̃��b�V�����擾�Ɏ��s���܂���");
				ret = FALSE;
				break;
			}
		}

	} while (0);

	return ret;
}

/************************************************************
 * �֐��� : MapFileSet�֐�                                  *
 * ��  �e : �}�b�v�p��csv�t�@�C����W�J���A�����i�[���܂� *
 * ��  �� : �Ȃ�                                            *
 * �߂�l : �Ȃ�                                            *
 ************************************************************/
BOOL Field::MapFileSet(VOID)
{
	D3DXVECTOR3 pos_ = D3DXVECTOR3(0.0F, 0.0F, 0.0F);
	FLOAT       direction = 0.0F;  // �u���b�N�̌���
	FLOAT       wall      = 0.0F;  // �u���b�N�̍���
	INT         loop      = 0;     // �u���b�N�̏�ɐςތ�
	INT         block_type;        // �u���b�N�̑���
	INT         count = 0;
	FILE*       fp;            // �t�@�C���|�C���^
	CHAR        buf[NAME_LEN]; // �����i�[�p
	D3DXMATRIX  pos, rota;     // ���i�[�p
	BOOL ret = TRUE;           // �߂�l

	/* �t�H���_�K�w���ړ����� */
	_chdir(map_no_);

	do
	{
		/* �t�@�C���̓ǂݍ��� */
		fopen_s(&fp, kFieldMap, "r");

		/* �擾�ł������ǂ��� */
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
				if ((buf[i] != ',') && (buf[i] != '\n'))
				{
					/* CHAR�^����INT�^�ɕϊ� */
					block_type = ((INT)buf[i] - (INT)'0');  // ���̑�

					switch (buf[i])
					{
						case '*': pos_.y += 1.0F; count++; continue;  // �������グ��
						case '/': pos_.y -= 1.0F; count--; continue;  // ������������
						case 'a': pos_.y += 0.5F; continue;  // ��̕␳�グ
						case 'i': pos_.y -= 0.5F; continue;  // ��̕␳����
						case 'l': direction = (kSlopeDirection / 180.0F)     * D3DX_PI; continue; // ��������
						case 't': direction = (kSlopeDirection * 2 / 180.0F) * D3DX_PI; continue; // �������
						case 'r': direction = (kSlopeDirection * 3 / 180.0F) * D3DX_PI; continue; // �E������
						case 'd': direction = (kSlopeDirection * 4 / 180.0F) * D3DX_PI; continue; // ��������
						case '3': loop = 1; break;
						case '4': loop = kDammyLoop; break;  // �_�~�[
						default : loop = 1; 
					}

					/* ������ۑ����Ă��� */
					wall = pos_.y;
					if (buf[i] == '3' || buf[i] == '4') wall += 0.5F;

					/* �w�肳�ꂽ�������̃��[�v���s�� */
					for (INT n = 0; n < loop; n++)
					{
						/* ���W��ۑ����� */
						pos_list_.push_back(pos_);

						/* ���W���Z�b�g���� */
						D3DXMatrixIdentity(&world_);
						D3DXMatrixRotationY(&rota, direction);
						D3DXMatrixTranslation(&pos, pos_.x, wall, pos_.z);

						D3DXMatrixMultiply(&world_, &rota, &pos);
						matrix_list_.push_back(world_);  // ���W���i�[����

						if (buf[i] == '4' && n == 0)
						{
							information_list_.push_back(GROUND);
							wall -= 1.0F;
						}
						else information_list_.push_back(block_type); // �u���b�N�̑������i�[

						wall += 1.0F;
					}

					i++;
				}

				pos_.x += 1.0F;    // ���W�𓮂���
				direction = 0.0F;  // ������������
			}

			pos_.x  = 0.0F;  // �����̏�����
			pos_.y  = 0.0F;  // �����̏�����
			pos_.z -= 1.0F;  // �c���𓮂���
		}

		info_list_ = information_list_;

	}while (0);

	/* �t�H���_�K�w��߂� */
	_chdir("../../../");

	/* �t�@�C������� */
	fclose(fp);

	return ret;
}

/***************************************************
 * �֐��� : PlayerHitCheck�֐�                     *
 * ��  �e : �v���C���[�̋߂��̃u���b�N�𔻒肵�܂� *
 * ��  �� : �Ȃ�                                   *
 * �߂�l : �Ȃ�                                   *
 ***************************************************/
VOID Field::PlayerHitCheck(VOID)
{
	/* �t�B�[���h�̍��W���X�V */
	it_matrix_     = matrix_list_.begin();
	it_position_   = pos_list_.begin();
	it_infomation_ = information_list_.begin();

	/* �I���ʒu */
	it_end_position_ = pos_list_.end();

	/* ������ */
	draw_matrix_[player_].clear();
	draw_pos_[player_].clear();
	draw_information_[player_].clear();

	player_pos_list_.clear();
	player_list_.clear();
	player_information_.clear();
	player_radius_list_.clear();

	player_len_list_.clear();

	/* �����蔻�������͈͂𔻒肷�� */
	while (it_position_ != it_end_position_)
	{
		/* �v���C���[�̍��W�̋߂��𔻒肷�� */
		if (D3DXVec3Length(&(D3DXVECTOR3(*it_position_ - player_draw_pos_[player_]))) < kFieldDraw)
		{
			/* ���W���Z�b�g���� */
			draw_pos_[player_].push_back(*it_position_);
			draw_matrix_[player_].push_back(*it_matrix_);  // ���W���i�[����
			draw_information_[player_].push_back(*it_infomation_);

			len_ = max_ - min_;

			player_len_list_.push_back(len_);
		}

		/* �v���C���[�̍��W�̋߂��𔻒肷�� */
		if (D3DXVec3Length(&(D3DXVECTOR3(*it_position_ - player_pos_[player_]))) < kFieldCollision)
		{
			player_pos_list_.push_back(*it_position_);
			player_list_.push_back(*it_matrix_);  // ���W���i�[����
			player_information_.push_back(*it_infomation_);

			player_radius_list_.push_back(collision_.SphereCreate(block[*it_infomation_]->ReturnMesh()));
		}

		/* �C�e���[�^��i�߂� */
		++it_matrix_;
		++it_position_;
		++it_infomation_;
	}
}

/***************************************
 * �֐��� : FieldDraw�֐�              *
 * ��  �e : �t�B�[���h�̕`����s���܂� *
 * ��  �� : �Ȃ�                       *
 * �߂�l : �Ȃ�                       *
 ***************************************/
VOID Field::FieldDraw(VOID)
{
	/* �C�e���[�^ */
	it_draw_pos_        = draw_pos_[player_].begin();
	it_draw_matrix_     = draw_matrix_[player_].begin();
	it_draw_infomation_ = draw_information_[player_].begin();
	it_draw_end_        = draw_matrix_[player_].end();
	it_len_list_ = player_len_list_.begin();

	/* �S�̂̕`����s�� */
	while (it_draw_matrix_ != it_draw_end_)
	{
		if (*it_draw_infomation_ <= BLOCK_MAX)
		{
			/* �`����s�� */
			block[*it_draw_infomation_]->SetMatrix(*it_draw_matrix_);
			block[*it_draw_infomation_]->SetPosition(*it_draw_pos_);

			/* �B���ʘH�Ȃ� */
			if (*it_draw_infomation_ == DUMMY)
			{
				block[WALL]->Draw();
			}

			//collision_.BoxMesh(block[*it_draw_infomation_]->ReturnMesh(),
			//	*it_draw_pos_,
			//	*it_len_list_);

			block[*it_draw_infomation_]->Draw();
		}

		/* �X�V */
		++it_draw_pos_;
		++it_draw_matrix_;
		++it_draw_infomation_;
	}
}

/*****************************************
 * �֐��� : SkyDraw�֐�                  *
 * ��  �e : �X�J�C�{�[���̕`����s���܂� *
 * ��  �� : �Ȃ�                         *
 * �߂�l : �Ȃ�                         *
 *****************************************/
VOID Field::SkyDraw(VOID)
{
	/* ���[���h���W�n���쐬 */
	D3DXMatrixIdentity(&world_);
	D3DXMatrixTranslation(&world_, player_pos_[player_].x,
		player_pos_[player_].y - 50.0F,
		player_pos_[player_].z);

	/* �X�J�C�{�[����`�� */
	mesh_.Draw(world_, my_mesh_);
}