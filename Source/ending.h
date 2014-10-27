/**********************
 * �G���f�B���O�N���X *
 **********************/

/* ���dInclude�h�~ */
#ifndef RACE_ENDING_H_
#define RACE_ENDING_H_

/***********
 * Include *
 ***********/
#include "flow_base.h"
#include "layout_base.h"
#include "demoplay.h"

/********
 * �萔 *
 ********/
CONST D3DXVECTOR3 kEnding = D3DXVECTOR3(400, 110.0F, 0.0F);  // �G���f�B���O��ʕ`��ʒu

CONST LPCSTR Result = "result.png";

/********************************************************
 * �G���f�B���O�̊Ǘ����s���N���X�ł�                   *
 * FlowBase�N���X��LayoutBase�N���X���p�����č쐬���܂� *
 ********************************************************/
class Ending : public FlowBase, public LayoutBase
{
private:

	DemoPlay demo_play_;

	LPDIRECT3DTEXTURE9 texture_; // �e�N�X�`��

	/* �ϐ� */
	INT total_point_[kAllPlayer];    // �v���C���[�̑����|�C���g
	INT total_ranking_[kAllPlayer];  // �v���C���[�̑��������L���O

	/* �����֐� */
	VOID RankingSort(INT player_no);  // ���ʂ��\�[�g
	VOID RankingDraw(VOID);           // �����L���O�̕`��
	VOID SpriteDraw(VOID);            // �X�v���C�g�`�揈��

public:
	Ending(VOID);           // �R���X�g���N�^
	~Ending(VOID);          // �f�X�g���N�^

	BOOL Init(VOID);        // �����ݒ�
	VOID ReSet(VOID);       // ����������
	INT  UpDate(VOID);      // �X�V����
	VOID Draw(VOID);        // �`�揈��
	VOID Destroy(VOID);     // �j������


};

#endif