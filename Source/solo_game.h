/****************
* �Q�[���N���X *
****************/

/* ���dInclude�h�~ */
#ifndef RACE_SOLO_GAME_H_
#define RACE_SOLO_GAME_H_

/***********
* Include *
***********/
#include "game_base.h"

/******************************************
 * �Q�[���̃��C���������Ǘ�����N���X�ł� *
 * GameBase�N���X���p�����č쐬���܂�     *
 ******************************************/
class SoloGame : public GameBase
{
private:

	VOID SpriteDraw(VOID);  // �X�v���C�g�`�揈��
public:

	SoloGame(VOID);    // �R���X�g���N�^
	~SoloGame(VOID);   // �f�X�g���N�^

	INT  UpDate(VOID); // �X�V����
	VOID Draw(VOID);   // �`�揈��
};

#endif