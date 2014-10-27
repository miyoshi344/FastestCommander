/***********
 * Include *
 ***********/
#include "player_select.h"

/******************
 * �R���X�g���N�^ *
 ******************/
PlayerSelect::PlayerSelect(VOID)
{

}

/****************
 * �f�X�g���N�^ *
 ****************/
PlayerSelect::~PlayerSelect(VOID)
{
	Texture::Release(texture_);
}

/*********************************************
 * �֐��� : Init�֐�                         *
 * ��  �e : �����������s���܂�               *
 * ��  �� : �Ȃ�                             *
 * �߂�l : �������ɐ���������TRUE��Ԃ��܂� *
 *********************************************/
BOOL PlayerSelect::Init(VOID)
{
	BOOL ret = TRUE;

	do
	{
		/* �t�H���_�K�w���ړ� */
		_chdir("Resource/Texture");

		/* �e�N�X�`���C���^�[�t�F�C�X�̎擾 */
		if (!(texture_ = Texture::LoadTexture("back.bmp")))
		{
			ErrorDialog("�Z���N�g��ʂ̓ǂݍ��݂Ɏ��s���܂���");
			ret = FALSE;
			break;
		}

		/* �t�H���_�K�w��߂� */
		_chdir("../../");

	} while (0);

	return ret;
}

/***********************************
 * �֐��� : ReSet�֐�              *
 * ��  �e : ��Ԃ̏��������s���܂� *
 * ��  �� : �Ȃ�                   *
 * �߂�l : �Ȃ�                   *
 ***********************************/
VOID PlayerSelect::ReSet(VOID)
{

}

/*********************************
 * �֐��� : UpDate�֐�           *
 * ��  �e : �X�V�������s���܂�   *
 * ��  �� : �Ȃ�                 *
 * �߂�l : �J�ڂ̔ԍ���Ԃ��܂� *
 *********************************/
INT PlayerSelect::UpDate(VOID)
{
	for (INT i = 0; i < kMaxPad; i++)
	{
		XInput::PushButton(i);
		push_button = XInput::ReturnPushButton();

		if (push_button == XINPUT_GAMEPAD_B) return GAME;
	}

	return SELECT;
}

/*******************************
 * �֐��� : Draw�֐�           *
 * ��  �e : �`�揈�����s���܂� *
 * ��  �� : �Ȃ�               *
 * �߂�l : �Ȃ�               *
 *******************************/
VOID PlayerSelect::Draw(VOID)
{
	SpriteDraw();
}

/*******************************
 * �֐��� : Destroy�֐�        *
 * ��  �e : �j���������s���܂� *
 * ��  �� : �Ȃ�               *
 * �߂�l : �Ȃ�               *
 *******************************/
VOID PlayerSelect::Destroy(VOID)
{
	Texture::Release(texture_);
}

/*******************************************
 * �֐��� : SpriteDraw�֐�                 *
 * ��  �e : �X�v���C�g�̕`�揈�����s���܂� *
 * ��  �� : �Ȃ�                           *
 * �߂�l : �Ȃ�                           *
 *******************************************/
VOID PlayerSelect::SpriteDraw(VOID)
{
	Sprite::GetSprite()->Begin(D3DXSPRITE_ALPHABLEND);

	Sprite::DrawSprite(texture_);

	Sprite::GetSprite()->End();
}