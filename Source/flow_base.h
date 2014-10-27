/**********************
 * ��ԑJ�ڊ�b�N���X *
 **********************/

/* ���dInclude�h�~ */
#ifndef RACE_FLOW_BASE_H_
#define RACE_FLOW_BASE_H_

/***********
 * Include *
 ***********/
#include "common.h"
#include "sprite.h"
#include "texture.h"
#include "xinput.h"

/********
 * �� *
 ********/

/* �J�ڗp�� */
enum
{
	TITLE,       // �^�C�g��
	SOLO_GAME,   // �\���Q�[��
	MULTI_GAME,  // �}���`�Q�[��
	ENDING,      // �G���f�B���O
	GAME_OVER,   // �Q�[���I�[�o�[
	MAX_FLOW,    // �J�ڍő吔
};

/**********************************
 * ��ԑJ�ڂ̊�b�N���X�ɂȂ�܂� *
 **********************************/
class FlowBase
{
protected:

	/************
	 * �ÓI�ϐ� *
	 ************/
	static INT player_count_;             // �v���C���[��
	static INT field_map_;                // �t�B�[���h�ԍ�
	static INT total_point_[kAllPlayer];  // �v���C���[���Ƃ̃X�R�A�̍��v�l

	/********
	 * �ϐ� *
	 ********/
	ULONG push_stick_[kMaxPad];   // �X�e�B�b�N���͕ۑ��p
	ULONG push_button_[kMaxPad];  // �{�^�����͕ۑ��p

	/************
	 * �����֐� *
	 ************/
	virtual	VOID SpriteDraw(VOID) = 0;  // �X�v���C�g�`�揈��

public:
	FlowBase(VOID);   // �R���X�g���N�^
	~FlowBase(VOID);  // �f�X�g���N�^

	virtual BOOL Init(VOID)       = 0;  // ����������
	virtual VOID ReSet(VOID)      = 0;  // ���l�̏�����
	virtual INT  UpDate(VOID)     = 0;  // �X�V����
	virtual VOID Draw(VOID)       = 0;  // �`�揈��
	virtual VOID Destroy(VOID)    = 0;  // �j������

	/* �X�R�A���󂯓n���֐� */
	INT SetTotalRanking(INT player_no) { return total_point_[player_no]; }

	VOID GetTotalRanking(INT player_no, INT total_point) { total_point_[player_no] = total_point; }
};

#endif