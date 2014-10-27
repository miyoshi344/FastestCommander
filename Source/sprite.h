/****************
 * Sprite�N���X *
 ****************/

/* ���dInclude�h�~ */
#ifndef RACE_SPRITE_H_
#define RACE_SPRITE_H_

/***********
 * Include *
 ***********/
#include "common.h"    // ���ʃN���X
#include "direct3d.h"  // Direct3D�N���X

/*******************
 * �萔/�}�N���錾 *
 *******************/
#define SPRITE  (Sprite::GetInstance())	// Sprite�C���^�[�t�F�C�X�Ăяo���ȈՔ�

/**************
 * �N���X�錾 *
 **************/
class Sprite
{
private:
	static LPD3DXSPRITE sprite_;  // �X�v���C�g�C���^�t�F�[�X

public:
	Sprite(VOID);				// �R���X�g���N�^
	static BOOL Init(VOID);		// ������
	static VOID Destroy(VOID);	// �j��

	/* LPD3DXSPRITE�C���^�[�t�F�C�X��Ԃ� */
	static CONST LPD3DXSPRITE GetSprite(VOID) { return sprite_; }

	static VOID	DrawSprite(
			CONST LPDIRECT3DTEXTURE9 pTexture,                        // �e�N�X�`���C���^�[�t�F�C�X
			CONST D3DXVECTOR3* pPos = &D3DXVECTOR3(0.0F, 0.0F, 0.0F), // �s��
			CONST RECT         rect = { 0, 0, 1280, 720 },            // �؂����`
			CONST INT          Alpha = 255,                           // �A���t�@�l
			CONST D3DXVECTOR3* pCenter = NULL);                       // ���S���W	
};

#endif