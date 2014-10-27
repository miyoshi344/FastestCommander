/***********
* Include *
***********/
#include "multi_game.h"

/******************
* �R���X�g���N�^ *
******************/
MultiGame::MultiGame(VOID)
{
}

/****************
* �f�X�g���N�^ *
****************/
MultiGame::~MultiGame(VOID)
{
}

/*****************************
* �֐��� : Init�֐�         *
* ��  �e : ���������s���܂� *
* ��  �� : �Ȃ�             *
* �߂�l : �Ȃ�             *
*****************************/
BOOL MultiGame::Init(VOID)
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
			ErrorDialog("�e�N�X�`���̎擾�Ɏ��s");
			ret = FALSE;
			break;
		}

		if (!ranking_.Init())
		{
			ErrorDialog("�e�N�X�`���̎擾�Ɏ��s");
			ret = FALSE;
			break;
		}

		if (!TextureInit())
		{
			/* �G���[ */
			ErrorDialog("���̎擾�Ɏ��s");
			ret = FALSE;
			break;
		}

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
VOID MultiGame::ReSet(VOID)
{
	/* �v���C���[�̃��Z�b�g */
	for (INT i = 0; i < player_count_; i++)
	{
		player_[i]->ReSet();       // �v���C���[������
		camera_[i].Init();         // �J����������
		ranking_flag_[i] = FALSE;  // �����L���O�t���O�̐ݒ�I�t

		/* �v���C���[�̏����z�u */
		player_[i]->StartPosition(check_point_[field_map_].ReturnStartPos(i));

		/* �v���C���[�ԍ��擾 */
		player_[i]->SetPlayerNo(i);
	}

	/* BGM�̍Đ��ʒu������ */
	for (INT i = GRASS_BGM; i < BEACH_BGM + 1; i++)
	{
		DSound::Restart(i);  // �Đ��ʒu�̏�����
	}

	/* �r���[�̐ݒ� */
	ViewPort();

	/* ��Ԃ̏����� */
	ranking_.ReSet();  // �����L���O�̏�����
	time_.ReSet();     // �^�C���̏�����

	/* �t�B�[���h�̏����� */
	field_[field_map_].ReSet();        // �t�B�[���h������
	check_point_[field_map_].ReSet();  // �`�F�b�N�|�C���g������

	/* �ϐ������� */
	pause_        = FALSE;  // �|�[�Y�t���O�̐ݒ�I�t
	result_count_ = 0;      // ���U���g�J�E���g��0
}

/*******************************
 * �֐��� : UpDate�֐�         *
 * ��  �e : �X�V�������s���܂� *
 * ��  �� : �Ȃ�               *
 * �߂�l : �Ȃ�               *
 *******************************/
INT MultiGame::UpDate(VOID)
{
	/* �v���C���[�̍X�V */
	for (INT i = 0; i < player_count_; i++)
	{
		/* ���͏��̎擾 */
		push_button_[i] = XInput::ReturnPushButton(i);

		/* �|�[�Y�����s����Ă��Ȃ���Ύ��s */
		if (!Pause(i))
		{
			/* ���̍X�V */
			GameSet(i);

			/* �v���C���[�̍X�V */
			if (time_.ReturnFlag()) GameUpDate(i);
		}
		/* �|�[�Y���Ƀo�b�N�{�^���������ꂽ��^�C�g���ɖ߂� */
		else if (pause_ && (push_button_[i] == XINPUT_GAMEPAD_BACK)) return TITLE;
	}

	/* �X�e�[�W�̏��� */
	return NextStage();
}

/*******************************
 * �֐��� : Draw�֐�           *
 * ��  �e : �`�揈�����s���܂� *
 * ��  �� : �Ȃ�               *
 * �߂�l : �Ȃ�               *
 *******************************/
VOID MultiGame::Draw(VOID)
{
	/* �r���[�|�[�g���m�ۂ��Ă��� */
	D3DVIEWPORT9 view;
	D3DDEV->GetViewport(&view);

	/* �Q�[����ʂ̕`�� */
	MultiDraw();

	//collision_.Draw();

	/* �r���[��߂� */
	D3DDEV->SetViewport(&view);

	/* �X�v���C�g�̕`�� */
	SpriteDraw();
}

/*********************************************
 * �֐��� : TextureInit�֐�                  *
 * ��  �e : �e�N�X�`���̏��������s���܂�     *
 * ��  �� : �Ȃ�                             *
 * �߂�l : �������ɐ���������TRUE��Ԃ��܂� *
 *********************************************/
BOOL MultiGame::TextureInit(VOID)
{
	BOOL ret = TRUE;

	/* �t�H���_�K�w���ړ� */
	_chdir("Resource_fast/Texture");

	do
	{
		/* �e�N�X�`���C���^�[�t�F�C�X�̎擾 */
		if (!(player2_edge_ = Texture::LoadTexture(k2PlayEdge)))
		{
			/* �G���[ */
			ErrorDialog("�g���̓ǂݍ��݂Ɏ��s���܂���");
			ret = FALSE;
			break;
		}

		/* �e�N�X�`���C���^�[�t�F�C�X�̎擾 */
		if (!(player4_edge_ = Texture::LoadTexture(k4PlayEdge)))
		{
			/* �G���[ */
			ErrorDialog("�g���̓ǂݍ��݂Ɏ��s���܂���");
			ret = FALSE;
			break;
		}

	} while (0);

	/* �t�H���_�K�w��߂� */
	_chdir("../../");

	return ret;
}

/*************************************
 * �֐��� : ViewPort�֐�             *
 * ��  �e : ��ʂ̕����ݒ���s���܂� *
 * ��  �� : �Ȃ�                     *
 * �߂�l : �Ȃ�                     *
 *************************************/
VOID MultiGame::ViewPort(VOID)
{
	/* �I�����ꂽ�v���C���[��2�l�ȏ�Ȃ� */
	if (player_count_ > 2)
	{
		/* ��ʂ�4�������� */
		port_[0] = {                0,                 0, kWindowWidth / 2, kWindowHeight / 2, 0.0f, 1.0f };
		port_[1] = { kWindowWidth / 2,                 0, kWindowWidth / 2, kWindowHeight / 2, 0.0f, 1.0f };
		port_[2] = {                0, kWindowHeight / 2, kWindowWidth / 2, kWindowHeight / 2, 0.0f, 1.0f };
		port_[3] = { kWindowWidth / 2, kWindowHeight / 2, kWindowWidth / 2, kWindowHeight / 2, 0.0f, 1.0f };

		/* �A�X�y�N�g��̐ݒ� */
		aspect_ = kMultiWindow;
	}
	/* 2�l�Ȃ� */
	else
	{
		/* ��ʂ�2�������� */
		port_[0] = { 0,                     0, kWindowWidth, (DWORD)kMultiWindow.y, 0.0f, 1.0f };
		port_[1] = { 0, (DWORD)kMultiWindow.y, kWindowWidth, (DWORD)kMultiWindow.y, 0.0f, 1.0f };

		/* �A�X�y�N�g��̐ݒ� */
		aspect_ = D3DXVECTOR3((FLOAT)kWindowWidth, kMultiWindow.y, 0.0F);
	}
}

/***************************************
 * �֐��� : MultiDraw�֐�              *
 * ��  �e : ���l���p�̉�ʂ�`�悵�܂� *
 * ��  �� : �Ȃ�                       *
 * �߂�l : �Ȃ�                       *
 ***************************************/
VOID MultiGame::MultiDraw(VOID)
{
	time_.TimeCount();

	/* �v���C���[�l������������ */
	for (int i = 0; i < player_count_; i++)
	{
		//�`��̈��ύX�i�r���[�|�[�g�s��j
		D3DDEV->SetViewport(&port_[i]);
		D3DDEV->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0F, 0);

		/* �J�����̃A�X�y�N�g��ݒ� */
		camera_[i].Aspect(aspect_);

		//�J�����̍��W��ύX�i�r���[�s��j
		camera_[i].UpDate();

		/* ��ʂ̍X�V */
		field_[field_map_].SetPlayerNo(i);  // �v���C���[�ԍ��̎擾
		field_[field_map_].Draw();          // �t�B�[���h�̕`��

		/* �v���C���[�̕`�� */
		DrawPlayer(i);

		/* �`�F�b�N�|�C���g�̔����\�� */
		check_point_[field_map_].Draw(i);

		/* �X�v���C�g�̕`�� */
		Sprite::GetSprite()->Begin(D3DXSPRITE_ALPHABLEND);

		/* �����L���O�t���O�������Ă��Ȃ��Ȃ� */
		if (!ranking_flag_[i])
		{
			/* �J�E���g�_�E�����I������� */
			if (time_.ReturnFlag())
			{
				/* �v���C���[����2�l�ȏ�Ȃ� */
				if (player_count_ > 2)
				{
					speed_[i].MultiDraw(i);                // 4�l�p�X�s�[�h�̕`��
					check_point_[field_map_].MultiDraw(i); // �`�F�b�N�|�C���g�̕`��
				}
				/* 2�l�Ȃ� */
				else
				{
					speed_[i].TwoDraw(i);                // 2�l�p�X�s�[�h�̕`�� 
					check_point_[field_map_].TwoDraw(i); // ���񐔂̕`��
				}
			}
		}
		/* �����L���O�t���O�������Ă����� */
		else
		{
			/* �v���C���[����2�l�ȏ�Ȃ� */
			if (player_count_ > 2) ranking_.MultiDraw(i);  // 4�l�p�����L���O�̕`��

			/* 2�l�Ȃ� */
			else ranking_.TwoDraw(i);                      // 2�l�p�����L���O�̕`��
		}

		Sprite::GetSprite()->End();
	}
}

/*****************************************
 * �֐��� : SpriteDraw�֐�               *
 * ��  �e : �X�v���C�g�`�揈�����s���܂� *
 * ��  �� : �Ȃ�                         *
 * �߂�l : �Ȃ�                         *
 *****************************************/
VOID MultiGame::SpriteDraw(VOID)
{
	Sprite::GetSprite()->Begin(D3DXSPRITE_ALPHABLEND);

	/* �v���C���[��2�l�ȏ�Ȃ� */
	if (player_count_ > 2) Sprite::DrawSprite(player4_edge_);  // 4�l�p�t���[���̕`��

	/* �v���C���[��2�l�Ȃ� */ 
	else Sprite::DrawSprite(player2_edge_);                    // 2�l�p�t���[���̕`��

	/* �^�C���J�E���g�̕`����s�� */
	if (!time_.ReturnFlag()) time_.TimeCountDraw();

	Sprite::GetSprite()->End();
}
