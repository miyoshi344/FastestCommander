/*****************
 * Texture�N���X *
******************/

/* ���dInclude�h�~ */
#ifndef RACE_TEXTURE_H_
#define RACE_TEXTURE_H_

/***********
 * Include *
 ***********/
#include "direct3d.h"  // Direct3D�N���X

/*******************
 * �萔/�}�N���錾 *
 *******************/
CONST D3DCOLOR	TEXTURE_COLORKEY = D3DCOLOR_XRGB(0, 255, 0);  // �΂��J���[�L�[�Ɏw��

/**************
 * �\���̐錾 *
 **************/
typedef struct TEXTURETABLE
{
	CHAR				m_TextureName[256];	// �e�N�X�`�����i�[�p
	LPDIRECT3DTEXTURE9	m_pTexture;			// �e�N�X�`���C���^�[�t�F�C�X
	INT					m_RentalNum;		// ���݂݂̑��o����
	D3DXIMAGE_INFO		m_ImgInfo;			// �e�N�X�`�����
	TEXTURETABLE*		m_pNext;			// ���X�g�p�i���̂ւ̃|�C���^�j

	/* �R���X�g���N�^ */
	TEXTURETABLE(VOID)						// �e�N�X�`���o�b�t�@�̏�����
	{
		m_TextureName[0] = '\0';
		m_pTexture = NULL;
		m_RentalNum = 0;
		m_pNext = NULL;
	}

	/* �f�X�g���N�^ */
	~TEXTURETABLE()
	{
		/* �e�N�X�`����� */
		SafeRelease(m_pTexture);
	}

}*LPTEXTURETABLE;

/**************
 * �N���X�錾 *
 **************/
class Texture
{
private:
	static LPTEXTURETABLE	m_pTextureList;	// �e�N�X�`���Ǘ��o�b�t�@
	static LPTEXTURETABLE	m_pStockList;	// ���g�p�̊Ǘ����X�g
	static LPTEXTURETABLE	m_pUseList;		// �g�p���̊Ǘ����X�g
	static INT				m_MaxUseNum;	// �g�p�e�N�X�`����

public:
	static BOOL Init(VOID);													// ������
	static LPDIRECT3DTEXTURE9	LoadTexture(LPCSTR TextureName);			// �e�N�X�`���̓ǂݍ���
	static VOID					Release(LPDIRECT3DTEXTURE9 CONST pTexture);	// �e�N�X�`���̔j��
	static VOID					Destroy(VOID);								// �j��
};

#endif