/****************************
* ���C�A�E�g�̃R�����N���X *
**************************** /

/* ���dInclude�h�~ */
#ifndef RACE_LAYOUT_H_
#define RACE_LAYOUT_H_

/***********
* Include *
***********/
#include "common.h"
#include "objectbase.h"

/********
* �� *
********/

/* �����̑傫�� */
enum
{
	SMALL,   // ����������
	MIDDLE,  // ���̕���
	LAGE,    // �傫������
};

/* �����J���[ */
enum
{
	RED,     // ��
	GREEN,   // ��
	YELLOW,  // ��
	BLUE,    // ��
};

/********
 * �萔 *
 ********/
CONST LPCSTR kScore  = "score.png";
CONST LPCSTR kLayout = "Layout.png";

/* �����p��` */
CONST RECT SmallScore  = { 0, 160, 25, 25 };
CONST RECT MiddleScore = { 0, 268, 50, 60 };
CONST RECT LageScore   = { 0, 519, 80, 100 };

/* �؂����` */
CONST RECT kStartRect    = {   0,  85,  425,  75 };  // �X�^�[�g�{�^��
CONST RECT kPlayer       = {   0,   0,  200,  80 };  // �v���C���[�l���I��
CONST RECT kTitleRect    = {   0, 280, 1000, 150 };  // �^�C�g��

CONST RECT kLapRect      = {   0,  50,   90,  40 };  // ���b�v����
CONST RECT kBarRect      = { 600, 268,   50,  60 };  // �o�[
CONST RECT kRankRect     = { 600, 212,   60,  28 };  // �����N
CONST RECT kRankingRect  = {   0,  0,   70,  38 };  // �����L���O

CONST RECT kCheckRect    = { 700, 350,  350, 100 };  // �`�F�b�N�|�C���g
CONST RECT kStageRect    = { 450, 338,  200,  95 };  // �X�e�[�W��

CONST RECT kTotalRanking = {   0,  940, 330,  60 };  // ���v����
CONST RECT kTotalScore   = {   0, 1010, 330,  50 };  // ���v�X�R�A
CONST RECT kRaceRanking  = {   0, 1060, 330,  59 };  // ���[�X�������L���O
CONST RECT kGetPoint     = {   0, 1120, 330,  60 };  // ���[�X���|�C���g

CONST RECT kPoint        = { 300,   0,  69,  40 };  // PT����
CONST RECT MeterRect     = {   0,   100, 150, 60 };  // km/h�\��


/**************************
* ���C�A�E�g�p��b�N���X *
**************************/
class LayoutBase
{
protected:

	/********
	 * �ϐ� *
	 ********/
	LPDIRECT3DTEXTURE9 texture_;   // �e�N�X�`��
	LPDIRECT3DTEXTURE9 texture2_;  // �e�N�X�`��

	INT player_no_;

	/************
	 * �����֐� *
	 ************/

	/* �`��p�֐� */
	VOID CountSet(D3DXVECTOR3 pos, INT count = 0, INT size = SMALL, INT color = RED, INT loop = 2);
	VOID DrawSet(D3DXVECTOR3 pos, RECT rect, INT player_no = 0, INT texture = 0);

public:
	LayoutBase(VOID);    // �R���X�g���N�^
	~LayoutBase(VOID);   // �f�X�g���N�^

	BOOL Init(VOID);
	VOID Destroy(VOID);  // �j��

	VOID SetPlayerNo(INT player_no) { player_no_ = player_no; }
};
#endif