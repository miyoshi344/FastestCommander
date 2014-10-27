/********************
 * �f���v���C�N���X *
 ********************/

/* ���dInclude�h�~ */
#ifndef RACE_DEMOPLAY_H_
#define RACE_DEMOPLAY_H_

/***********
 * Include *
 ***********/
#include "game_base.h"

/********
 * �萔 *
 ********/
CONST INT kStartMap = GRASS;



/********************************************
 * �f���v���C��\�����邷�邽�߂̃N���X�ł� *
 ********************************************/
class DemoPlay : public GameBase
{
private:
	BOOL MapInit(VOID);
	BOOL PlayerInit(VOID);
	VOID GameSet(INT player);
	VOID GameUpDate(INT player);
	VOID BackGameDraw(VOID);

public:
	DemoPlay(VOID);   // �R���X�g���N�^
	~DemoPlay(VOID);  // �f�X�g���N�^

	BOOL Init(VOID);     // �����ݒ�
	VOID ReSet(VOID);    // ����������
	BOOL UpDate(VOID);   // �X�V����
	VOID Draw(VOID);     // �`�揈��
	VOID SpriteDraw(VOID);
	VOID Destroy(VOID);  // �j������
};

#endif