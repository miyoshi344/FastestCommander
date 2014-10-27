/***********
 * Include *
 ***********/
#include "Sprite.h"

/**********************
 * �ÓI�����o�ϐ��錾 *
 **********************/
LPD3DXSPRITE Sprite::sprite_;

/******************
 * �R���X�g���N�^ *
 ******************/
Sprite::Sprite(VOID)
{
	/* ������ */
	sprite_ = NULL;
}

/*******************************************
 * �֐��� : Init�֐�                       *
 * ��  �e : �X�v���C�g�N���X�����������܂� *
 * ��  �� : �Ȃ��@                         *
 * �߂�l : ����������TRUE��Ԃ��܂�       *
 *******************************************/
BOOL Sprite::Init(VOID)
{
	BOOL ret = TRUE;

	do
	{
		/* �X�v���C�g�C���^�[�t�F�C�X�̍쐬 */
		if (FAILED(D3DXCreateSprite(D3DDEV, &sprite_)))
		{
			/* �G���[ */
			ErrorDialog("�X�v���C�g�C���^�[�t�F�C�X�̍쐬�Ɏ��s");
			ret = FALSE;
			break;
		}
	} while (0);

	return ret;
};

/************************************************************
 * �֐��� : DrawSprite�֐�                                  *
 * ��  �e : �X�v���C�g��`�悵�܂�                          *
 * ��  �� : CONST LPDIRECT3DTEXTURE9 pTexture  �e�N�X�`��   *
 *          CONST D3DXVECTOR3* pPos,           �`����W     *
 *          CONST RECT         rect            �؂����` *
 *          CONT  INT          Alpha           ����         *
 *          CONST D3DXVECTOR3* pCenter�@       ���S���W     *
 * �߂�l : ����������TRUE��Ԃ��܂�                        *
 ************************************************************/
VOID Sprite::DrawSprite(CONST LPDIRECT3DTEXTURE9 pTexture,
						CONST D3DXVECTOR3* pPos,
						CONST RECT rect,
						CONST INT Alpha,
						CONST D3DXVECTOR3* pCenter)
{
	RECT my_rect;  // �e�N�X�`���̐؂�o���͈�

	/* �e�N�X�`���g�p�͈͐ݒ� */
	my_rect.left   = rect.left;
	my_rect.top    = rect.top;
	my_rect.right  = rect.left + rect.right;
	my_rect.bottom = rect.top + rect.bottom;

	/* �`�� */
	sprite_->Draw(pTexture,						   // �e�N�X�`���C���^�t�F�[�X�|�C���^
				  &my_rect,                        // �e�N�X�`���؂�o���͈�
				  pCenter,                         // ��]���S���W
				  pPos,                            // �`��J�n�ʒu
				  D3DCOLOR_ARGB(Alpha, 255, 255, 255));  // �A���t�@�`���l���A�J���[�`���l��
}

/*****************************************
 * �֐��� : Destroy�֐�                  *
 * ��  �e : �X�v���C�g�N���X��j�����܂� *
 * ��  �� : �Ȃ��@                       *
 * �߂�l : �Ȃ�                         *
//****************************************/
VOID Sprite::Destroy(VOID)
{
	/* �X�v���C�g�C���^�[�t�F�C�X�̔j�� */
	SafeRelease(sprite_);
}