/***********
 * Include *
 ***********/
#include "title.h"

/******************
 * �R���X�g���N�^ *
 ******************/
Title::Title(VOID)
{
	/* �ϐ������� */
	start_button_ = FALSE;  // �X�^�[�g�̕\���t���O
	button_flag_  = TRUE;   // �{�^���̃t���O������
	count         = 0;      // �Ó]�p�J�E���g
	flag_count    = 0;      // ���͎�t�J�E���g������
	flag_         = FALSE;  // ���͎�t�t���O������
}

/****************
 * �f�X�g���N�^ *
 ****************/
Title::~Title(VOID)
{
	/* �e�N�X�`���̉�� */
	LayoutBase::Destroy();

	/* �f���v���C�̔j�� */
	demo_play_.Destroy();
}

/*********************************
 * �֐��� : Init�֐�             *
 * ��  �e : �������������s���܂� *
 * ��  �� : �Ȃ�                 *
 * �߂�l : ����������TRUE��Ԃ� *
 *********************************/
BOOL Title::Init(VOID)
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

		/* �^�C�g���e�N�X�`���̏����� */
		if (!TextureInit())
		{
			/* �G���[ */
			ErrorDialog("�e�N�X�`���̍쐬�Ɏ��s���܂���");
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
VOID Title::ReSet(VOID)
{
	/* �ϐ������� */ 
	player_count_ = 2;           // �v���C���[�l��

	start_button_ = FALSE;       // �X�^�[�g�t���O
	button_flag_  = TRUE;        // �{�^���t���O
	count         = 0;           // �Ó]�p�J�E���g

	flag_         = FALSE;       // ���͎�t�t���O
	flag_count    = 0;           // ���͎�t�p�J�E���g

	/* �f���v���C�̏�Ԃ������� */
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
INT Title::UpDate(VOID)
{
	/* �T�E���h�̍Đ� */
	DSound::Play(TITLE_BGM, DSBPLAY_LOOPING);

	/* �f����ʂ��X�V */
	demo_play_.UpDate();

	/* �{�^���������ꂽ��J�ڂ�ς��� */
	return (ButtonUpDate()) ? MULTI_GAME : TITLE;
}

/*******************************
 * �֐��� : Draw�֐�           *
 * ��  �e : �`�揈�����s���܂� *
 * ��  �� : �Ȃ�               *
 * �߂�l : �Ȃ�               *
 *******************************/
VOID Title::Draw(VOID)
{
	SpriteDraw();    // �X�v���C�g�̕`��
	demo_play_.Draw();
}

/*******************************
 * �֐��� : Destroy�֐�        *
 * ��  �e : �j���������s���܂� *
 * ��  �� : �Ȃ�               *
 * �߂�l : �Ȃ�               *
 *******************************/
VOID Title::Destroy(VOID)
{
	/* �e�N�X�`���̉�� */
	LayoutBase::Destroy();

	demo_play_.Destroy();
}

/*********************************************
 * �֐��� : TextureInit�֐�                  *
 * ��  �e : �v���C���[�̏��������s���܂�     *
 * ��  �� : �Ȃ�                             *
 * �߂�l : �������ɐ���������TRUE��Ԃ��܂� *
 *********************************************/
BOOL Title::TextureInit(VOID)
{
	BOOL ret = TRUE;

	/* �t�H���_�K�w���ړ� */
	_chdir("Resource_fast/Texture");

	do
	{
		/* �e�N�X�`���C���^�[�t�F�C�X�̎擾 */
		if (!(texture_ = Texture::LoadTexture(kTitleTexture)))
		{
			/* �G���[ */
			ErrorDialog("�^�C�g����ʂ̓ǂݍ��݂Ɏ��s���܂���");
			ret = FALSE;
			break;
		}

	} while (0);

	/* �t�H���_�K�w��߂� */
	_chdir("../../");

	return ret;
}

/*******************************************
 * �֐��� : ButtonUpDate�֐�               *
 * ��  �e : �{�^�����͏����̍X�V���s���܂� *
 * ��  �� : �Ȃ�                           *
 * �߂�l : �Ȃ�                           *
 *******************************************/
BOOL Title::ButtonUpDate(VOID)
{
	for (INT i = 0; i < kMaxPad; i++)
	{
		/* ���͂��擾 */
		push_button_[i] = XInput::ReturnPushButton(i);  // �{�^������
		push_stick_[i] = XInput::ReturnPushStick(i);    // �X�e�B�b�N����

		/* A�{�^���������ꂽ���̊m�F */
		if (push_button_[i] & XINPUT_GAMEPAD_A)
		{
			DSound::Play(PUSH_SE);  // �T�E���h�̍Đ�
			start_button_ = TRUE;   // �v���C���[�l���I���̃t���O���I���ɂ���
			button_flag_ = FALSE;   // �{�^���̕\�����I�t�ɂ���
		}
	}

	/* A�{�^���������ꂽ�� */
	if (start_button_)
	{
		if (PlayerSelect())
		{
			DSound::Restart(PUSH_SE);  // �T�E���h�̍Đ��ʒu��߂�
			DSound::Play(PUSH_SE);     // �T�E���h�̍Đ�

			DSound::Pause(TITLE_BGM);  // BGM�̒�~
			return TRUE;
		}
	}
	else StartButton();

	return FALSE;
}

/*************************************************
 * �֐��� : StartButton�֐�                      *
 * ��  �e : �J�E���g�𐔂��ĕ\���̊Ǘ����s���܂� *
 * ��  �� : �Ȃ�                                 *
 * �߂�l : �\�����邩�ǂ����̃t���O��Ԃ��܂�   *
 *************************************************/
BOOL Title::StartButton(VOID)
{
	/* �J�E���g��60�ȏ�ɂȂ�����*/
	if (count++ >= 60)
	{
		/* �{�^���t���O�𔽓]������ */
		if (button_flag_) button_flag_ = FALSE;
		else button_flag_ = TRUE;

		count = 0;
	}

	return button_flag_;
}

/*********************************************
 * �֐��� : PlayerSelect�֐�                 *
 * ��  �e : �v���C���[�l���̑I�����Ǘ����܂� *
 * ��  �� : �Ȃ�                             *
 * �߂�l : �I�����ꂽ��Ԃ�Ԃ��܂�         *
 *********************************************/
INT Title::PlayerSelect(VOID)
{
	/* �{�^�����͎擾�̃t���O */
	if (flag_count++ >= 30)
	{
		flag_ = TRUE;
		flag_count = 30;
	}

	/* �l�����̃{�^�����͂��Ď����܂� */
	for (INT i = 0; i < kMaxPad; i++)
	{
		/* �X�e�B�b�N���E�ɓ|���ꂽ�� */
		if (push_stick_[i] & LEFT_STICK_RIGHT && (player_count_ < 4))
		{
			DSound::Restart(CURSOR_SE); // �T�E���h�̍Đ��ʒu��߂�
			DSound::Play(CURSOR_SE);    // �T�E���h�̍Đ�
			player_count_++;            // �v���C���[�J�E���g�𑝂₷
		}

		/* �X�e�B�b�N�����ɓ|���ꂽ�� */
		else if (push_stick_[i] & LEFT_STICK_LEFT && (player_count_ > 2))
		{
			DSound::Restart(CURSOR_SE);  // �T�E���h�̍Đ��ʒu��߂�
			DSound::Play(CURSOR_SE);     // �T�E���h�̍Đ�
			player_count_--;             // �v���C���[�J�E���g�����炷
		}

		/* B�{�^���������ꂽ��t���O���I�t�ɂ��� */
		else if (push_button_[i] == XINPUT_GAMEPAD_B)
		{
			start_button_ = FALSE;  // �{�^���t���O���I�t�ɂ���
			flag_         = FALSE;  // ���̓t���O���I�t�ɂ���
			flag_count    = 0;      // �t���O�J�E���g�����Z�b�g����
		}

		/* A�{�^���������ꂽ�� */
		if (flag_ && push_button_[i] == XINPUT_GAMEPAD_A) return TRUE;
	}

	return FALSE;
}

/*******************************************
 * �֐��� : SpriteDraw�֐�                 *
 * ��  �e : �X�v���C�g�̕`�揈�����s���܂� *
 * ��  �� : �Ȃ�                           *
 * �߂�l : �Ȃ�                           *
 *******************************************/
VOID Title::SpriteDraw(VOID)
{
	Sprite::GetSprite()->Begin(D3DXSPRITE_ALPHABLEND);

	/* �^�C�g���̕`�� */
	Sprite::DrawSprite(texture_, &kTitlePosition, kTitleRect);

	/* �{�^����������Ă��Ȃ��Ȃ�PushStart��`�� */
	if (button_flag_) Sprite::DrawSprite(texture_, &kStartPosition, kStartRect);

	/* ������Ă����Ȃ�1�l�p�����l������I�� */
	if (start_button_)
	{
		/* �v���C�l���̕`�� */
		D3DXVECTOR3 pos = kTitlePlayerPosition;
		pos.x = kTitlePlayerPosition.x + (275.0F * (player_count_ - 2));  // �I������Ă���v���C���[���̕`��

		RECT rect = kPlayer;
		rect.left = kPlayer.left + (kPlayer.right * (player_count_ - 1));

		Sprite::DrawSprite(texture_, &pos, rect);
	}

	Sprite::GetSprite()->End();
}