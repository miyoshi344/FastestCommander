/******************
 * �^�C�g���N���X *
 ******************/

/* ���dInclude�h�~ */
#ifndef RACE_TITLE_H_
#define RACE_TITLE_H_

/***********
 * Include *
 ***********/
#include "flow_base.h"
#include "game_base.h"
#include "demoplay.h"

/********
 * �萔 *
 ********/
/* �e�N�X�`���� */
CONST LPCSTR kTitleTexture = "Layout.png";

/* �X�^�[�g�p�e�N�X�`����� */
CONST D3DXVECTOR3 kStartPosition       = D3DXVECTOR3(440.0F, 450.0F, 0.0F);
CONST D3DXVECTOR3 kTitlePlayerPosition = D3DXVECTOR3(250.0F, 500.0F, 0.0F);

/* �^�C�g���e�N�X�`����� */
CONST D3DXVECTOR3 kTitlePosition = D3DXVECTOR3(100.0F, 50.0F, 0.0F);

/**************************************************
 * �^�C�g���̊Ǘ����s���N���X�ł�                 *
 * GameBase��LayoutBase�N���X���p�����č쐬���܂� *
 **************************************************/
class Title : public GameBase, public LayoutBase
{
private:

	DemoPlay demo_play_;

	/********
	 * �ϐ� *
	 ********/
	D3DXVECTOR3 position_;  // �L�����N�^�[���W
	
	BOOL start_button_;     // �X�^�[�g�t���O
	BOOL button_flag_;      // �{�^���������ꂽ���̃t���O

	INT count;              // ����p�J�E���g

	INT flag_count;         // ���͉\�ɂȂ�ҋ@����
	BOOL flag_;             // ���͉\�t���O

	/************
	 * �����֐� *
	 ************/
	BOOL TextureInit(VOID);       // �e�N�X�`��������
	BOOL ButtonUpDate(VOID);      // �{�^���X�V����
	BOOL StartButton(VOID);       // �X�^�[�g�\������
	INT  PlayerSelect(VOID);      // �v���C���[�l���I������
	VOID SpriteDraw(VOID);        // �X�v���C�g�`�揈��
	
public:
	Title(VOID);            // �R���X�g���N�^
	~Title(VOID);           // �f�X�g���N�^

	BOOL Init(VOID);        // �����ݒ�
	VOID ReSet(VOID);       // ����������
	BOOL UpDate(VOID);      // �X�V����
	VOID Draw(VOID);        // �`�揈��
	VOID Destroy(VOID);     // �j������
};

#endif