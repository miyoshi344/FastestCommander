/***********
 * Include *
 ***********/
#include "layout_base.h"

/******************
 * �R���X�g���N�^ *
 ******************/
LayoutBase::LayoutBase(VOID)
{
	texture_   = NULL;
	texture2_  = NULL;
	player_no_ = 0;
}

/****************
 * �f�X�g���N�^ *
 ****************/
LayoutBase::~LayoutBase(VOID)
{
	/* �e�N�X�`����� */
	Texture::Release(texture_);
	Texture::Release(texture2_);
}

/*************************************
 * �֐��� : Init�֐�                 *
 * ��  �e : ���������s���܂�         *
 * ��  �� : �Ȃ�                     *
 * �߂�l : ����������TRUE��Ԃ��܂� *
 *************************************/
BOOL LayoutBase::Init(VOID)
{
	BOOL ret = TRUE;

	do
	{
		/* �t�H���_�K�w���ړ� */
		_chdir("Resource_fast/Texture");

		/* �e�N�X�`��1�̓ǂݍ��� */
		if (!(texture_ = Texture::LoadTexture(kScore)))
		{
			/* �G���[ */
			ErrorDialog("�X�R�A�p�̉摜�̓ǂݍ��݂Ɏ��s���܂���");
			ret = FALSE;
			break;
		}

		/* �e�N�X�`��2�̓ǂݍ��� */
		if (!(texture2_ = Texture::LoadTexture(kLayout)))
		{
			/* �G���[ */
			ErrorDialog("���C�A�E�g�p�̉摜�̓ǂݍ��݂Ɏ��s���܂���");
			ret = FALSE;
			break;
		}

		/* �t�H���_�K�w��߂� */
		_chdir("../../");

	} while (0);

	return ret;
}

/***************************
 * �֐��� : Destroy�֐�    *
 * ��  �e : �j�����s���܂� *
 * ��  �� : �Ȃ�           *
 * �߂�l : �Ȃ�           *
 ***************************/
VOID LayoutBase::Destroy(VOID)
{
	/* �e�N�X�`���C���^�[�t�F�C�X�̔j�� */
	Texture::Release(texture_);
	Texture::Release(texture2_);
}

/********************************************************
 * �֐��� : Count�֐�                                   *
 * ��  �e : ������Ă����������g�����l�̕`����s���܂�  *
 * ��  �� : RECT        rect   �`��p��`               *
 *          D3DXVECTOR3 pos_   �`��ʒu                 *
 *          INT         count_ �ǂݍ��݂�������         *
 *          INT         loop_  ����                     *
 * �߂�l : �Ȃ�                                        *
 ********************************************************/
VOID LayoutBase::CountSet(D3DXVECTOR3 pos, INT count, INT size, INT color, INT loop)
{
	/* ���[�J���ϐ� */
	INT         digit;
	INT         left;
	RECT        rect;
	D3DXVECTOR3 position = pos;

	/* �����̃T�C�Y�ɂ���ċ�`���쐬 */
	switch (size)
	{
		case SMALL:
		/* �؂����`�쐬 */
		rect.left   = SmallScore.left;
		rect.top    = SmallScore.top + (color * SmallScore.bottom);  // �F�̎w��
		rect.right  = SmallScore.right;
		rect.bottom = SmallScore.bottom;
		break;

		case MIDDLE:
		/* �؂����`�쐬 */
		rect.left   = MiddleScore.left;
		rect.top    = MiddleScore.top + (color * MiddleScore.bottom);  // �F�̎w��
		rect.right  = MiddleScore.right;
		rect.bottom = MiddleScore.bottom;
		break;

		case LAGE:
		/* �؂����`�쐬 */
		rect.left   = LageScore.left;
		rect.top    = LageScore.top + (color * LageScore.bottom);  // �F�̎w��
		rect.right  = LageScore.right;
		rect.bottom = LageScore.bottom;
		break;

		default: break;
	}


	/* �񐔕����[�v���s�� */
	for (INT i = 0; i < loop; i++)
	{
		digit = (count % 10);

		left = rect.left;

		/* �؂���͈͂����߂� */
		left += (digit * rect.right);

		RECT sprite
		{
			left,
			rect.top,
			rect.right,
			rect.bottom
		};

		/* �`�� */
		Sprite::DrawSprite(texture_, &position, sprite);

		/* ���̌��ňړ����� */
		count /= 10;
		position.x -= rect.right;
	}
}

/********************************************
 * �֐��� : DrawSet�֐�                     *
 * ��  �e : �`��̐F���w�肵�܂�            *
 * ��  �� : D3DXVECTOR3 pos  �\�����W       *
 *          INT player_no    �v���C���[�ԍ� *
 * �߂�l : �Ȃ�                            *
 ********************************************/
VOID LayoutBase::DrawSet(D3DXVECTOR3 pos, RECT rect, INT player_no, INT texture)
{
	/* �`��p��`�̍쐬 */
	RECT my_rect =
	{
		rect.left + (rect.right * player_no),
		rect.top,
		rect.right,
		rect.bottom
	};

	/* �w�肪0�Ȃ�1���ڂ̃e�N�X�`���ŕ`����s�� */
	if (texture == 0) Sprite::DrawSprite(texture_, &pos, my_rect);
	else Sprite::DrawSprite(texture2_, &pos, my_rect);
}