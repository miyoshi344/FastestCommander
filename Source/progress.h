/******************************
* �Q�[���������Ǘ�����N���X *
******************************/

/* ���dInclude�h�~ */
#ifndef RACE_PROGRESS_H_
#define RACE_PROGRESS_H_

/***********
* Include *
***********/
#include "flow_base.h"
#include "title.h"
#include "game_base.h"
#include "solo_game.h"
#include "multi_game.h"
#include "ending.h"

/**********************************
* �Q�[���������Ǘ�����N���X�ł� *
**********************************/
class Progress
{
private:

	/**************
	 * �N���X��` *
	 **************/
	FlowBase* flow_[MAX_FLOW];  // ��Ƃ��Ƃ̃N���X�i�[�p�|�C���^

	/********
	 * �ϐ� *
	 ********/
	INT flow_no_;     // ��ƒ��̑J��

public:
	Progress(VOID);    // �R���X�g���N�^
	~Progress(VOID);   // �f�X�g���N�^

	BOOL Init(VOID);     // ����������
	VOID UpDate(VOID);   // �X�V����
	VOID Draw(VOID);     // �`�揈��
	VOID Destroy(VOID);  // �j������
};

#endif