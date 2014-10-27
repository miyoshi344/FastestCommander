/****************
 * �Q�[���N���X *
 ****************/

/* ���dInclude�h�~ */
#ifndef RACE_MULTI_GAME_H_
#define RACE_MULTI_GAME_H_

/***********
 * Include *
 ***********/
#include "game_base.h"

/********
 * �萔 *
 ********/
CONST LPCSTR k2PlayEdge = "2PlayEdge.png";  // 2�l�p�t���[��
CONST LPCSTR k4PlayEdge = "4PlayEdge.png";  // 3�l�ȏ�p�t���[��

/******************************************
 * �Q�[���̃��C���������Ǘ�����N���X�ł� *
 * GameBase���p�����č쐬���܂�           *
 ******************************************/
class MultiGame : public GameBase
{
private:
	/********
	 * �ϐ� *
	 ********/

	/* ��ʍ\�� */
	D3DVIEWPORT9 port_[kAllPlayer];  // �r���[�|�[�g
	D3DXVECTOR3 aspect_;             // �A�X�y�N�g

	/* �t���[���p�e�N�X�`�� */
	LPDIRECT3DTEXTURE9 player2_edge_;  // 2�l�p�t���[��
	LPDIRECT3DTEXTURE9 player4_edge_;  // 3�l�ȏ�p�t���[��

	/************
	 * �����֐� *
	 ************/
	BOOL TextureInit(VOID);  // �e�N�X�`��������
	VOID ViewPort(VOID);     // �r���[�̐ݒ�
	VOID MultiDraw(VOID);    // �`�揈��
	VOID SpriteDraw(VOID);   // �X�v���C�g�`�揈��

public:
	MultiGame(VOID);   // �R���X�g���N�^
	~MultiGame(VOID);  // �f�X�g���N�^

	BOOL Init(VOID);   // ��������
	VOID ReSet(VOID);  // ����������
	INT  UpDate(VOID); // �X�V����
	VOID Draw(VOID);   // �`�揈��
};

#endif