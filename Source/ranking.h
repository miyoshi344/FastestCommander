/********************
 * �����L���O�N���X *
 ********************/

/* ���dInclude�h�~ */
#ifndef RACE_RANKING_H_
#define RACE_RANKING_H_

/***********
 * Include *
 ***********/
#include "layout_base.h"

/********
 * �萔 *
 ********/
CONST D3DXVECTOR3 kRaceRankingPosition  = D3DXVECTOR3(50.0F, 100.0F, 0.0F);
CONST D3DXVECTOR3 kRacePointPosition    = D3DXVECTOR3(50.0F, 200.0F, 0.0F);

CONST FLOAT kCountPosition = 400.0F;

/****************************************
 * �����L���O���Ǘ�����N���X�ł�       *
 * LayoutBase�N���X���p�����č쐬���܂� *
 ****************************************/
class Ranking : public LayoutBase
{
private:

	/********
	 * �ϐ� *
	 ********/
	INT race_ranking_[kAllPlayer];  // ���[�X�����L���O
	INT race_point_[kAllPlayer];    // ���[�X�|�C���g
	INT player_count_;              // �v���C���[��

	/************
	 * �����֐� *
	 ************/
	VOID RaceDraw(INT player_no);  // ���[�X���ʕ`��

public:
	Ranking(VOID);   // �R���X�g���N�^
	~Ranking(VOID);  // �f�X�g���N�^
	
	BOOL Init(VOID);   // ��������
	VOID ReSet(VOID);  // ����������
	VOID UpDate(INT player_no, INT ranking);  // �X�V����

	VOID SoloDraw(VOID);           // 1�l�p�`�揈��
	VOID TwoDraw(INT player_no);   // 2�l�p�`�揈��
	VOID MultiDraw(INT player_no); // �`�揈��
	VOID Destroy(VOID);            // �j������

	VOID RankingSort(INT player_no);

	VOID GetPlayerCount(INT player) { player_count_ = player; }

	/* �l��Ԃ��֐� */
	INT ReturnPoint(INT player_no)   { return race_point_[player_no]; }
};

#endif