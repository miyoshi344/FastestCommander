/* ���dInclude�h�~ */
#ifndef RACE_CHECKPOINT_H_
#define RACE_CHECKPOINT_H_

/***********
* Include *
***********/
#include "layout_base.h"
#include "collision.h"

/**********
 * �񋓌^ *
 **********/
enum
{
	WeekPoint,    // 1�T
	MiddlePoint,  // �`�F�b�N�|�C���g
	NoPoint,      // �Ȃ�
};

/**********
 * �\���� *
 **********/

/* �v���C���[�̍��W�Ɋ֐��\���� */
struct PlayerPos
{
	D3DXVECTOR3 start_position_;  // �v���C���[�̊J�n���W
	D3DXVECTOR3 player_pos_;      // �v���C���[���W
	D3DXVECTOR3 origin_pos_;      // �v���C���[�̓����O�̍��W

	/******************
	 * �R���X�g���N�^ *
	 ******************/
	PlayerPos(VOID)
	{
		start_position_ = D3DXVECTOR3(0.0F, 0.0F, 0.0F);
		player_pos_     = D3DXVECTOR3(0.0F, 0.0F, 0.0F);
		origin_pos_     = D3DXVECTOR3(0.0F, 0.0F, 0.0F);
	}
};

/* �v���C���[�̏��Ɋւ���\���� */
struct PlayerCheck
{
	INT   check_no;  // ���݂̃`�F�b�N�|�C���g���B��
	INT   lap;       // ����
	INT   ranking;   // ����
	FLOAT vector;    // ���̃`�F�b�N�|�C���g�܂ł̋���

	/* ������ */
	VOID ReSet(VOID)
	{
		check_no = 0;
		lap      = 1;
		ranking  = 0;
		vector   = 0.0F;
	}
};

/********
 * �萔 *
 ********/
CONST FLOAT kMultiY = 60.0F;
CONST FLOAT kMultiX = 30.0F;

/* �z�u��� */
CONST D3DXVECTOR3 kMultiRankingPos   = { 10.0F, 50.0F, 0.0F };  // ��ʂƂ̊Ԋu
CONST D3DXVECTOR3 kMultiRankPos      = { 50.0F, 15.0F, 0.0F };  // ��ʂƂ̊Ԋu

/* �\���Ԋu */
CONST FLOAT kLapPosition    = 80.0F;  // �`�F�b�N�|�C���g�̔z�u�Ԋu
CONST FLOAT kFienalPosition = 200.0F;

/****************
 * �t�@�C����� *
 ****************/
CONST LPSTR kCheckCsv = "checkpoint.csv";   // �`�F�b�N�|�C���g�pCSV
CONST LPSTR kHitCheck = "box_obj.x";	    // �v���C���[�p���b�V��

/* ������ */
CONST INT   kFinalLap       = 3;     // ����
CONST FLOAT kCheckCollision = 20.0F;  // �`�F�b�N�|�C���g�̓����蔻��傫��

/****************************************
 * �`�F�b�N�|�C���g�Ɋ֐��N���X�ł�     *
 * LayoutBase�N���X���p�����č쐬���܂� *
 ****************************************/
class CheckPoint : public LayoutBase
{
private:
	D3DXVECTOR3 return_position_[kAllPlayer];

	/**************
	 * �\���̒�` *
	 **************/
	PlayerPos   player_pos_[kAllPlayer];    // �z�u���W
	PlayerCheck player_check_[kAllPlayer];  // �v���C���[���

	/**************
	 * �N���X��` *
	 **************/
	MeshTable*  my_mesh_;    // ���b�V�����
	Collision   collision_;  // �����蔻��

	/********
	 * �ϐ� *
	 ********/
	LPCSTR check_no_;   // �}�b�v�ԍ�
	INT    check_max_;  // �`�F�b�N�|�C���g���ۑ��p

	/**********
	 * ���X�g *
	 **********/
	std::list<D3DXMATRIX>  check_matrix_;           // �`�F�b�N�|�C���g�p���[���h���W
	std::list<D3DXVECTOR3> check_position_;         // �`�F�b�N�|�C���g�p���W
	std::list<INT>         check_information_;      // �`�F�b�N�|�C���g�p�u���b�N���

	/****************
	 * �C�e���[�^�[ *
	 ****************/
	std::list<D3DXMATRIX>::iterator  it_matrix_;     // ���[���h���W�p�C�e���[�^
	std::list<D3DXVECTOR3>::iterator it_position_;   // ���W�p�C�e���[�^
	std::list<INT>::iterator         it_infomation;  // �u���b�N���p�C�e���[�^

	std::list<D3DXMATRIX>::iterator end_matrix_;

	/************
	 * �����֐� *
	 ************/
	INT  LapCheck(VOID);                   // �v���C���[�̎�����`�F�b�N����֐�
	BOOL FileCheck(VOID);                  // �}�b�v��ǂݍ��ފ֐�
	VOID TwoLapDraw(INT player_no);        // 2�l�p���b�v���`�揈��
	VOID TwoRankingDraw(INT player_no);    // 2�l�p���[�X���ʂ�`�悷��֐�
	VOID MultiLapDraw(INT player_no);      // ���l���p���b�v���`�揈�� 
	VOID MultiRankingDraw(INT player_no);  // ���[�X���ʂ�`�悷��֐�

public:
	CheckPoint(VOID);    // �R���X�g���N�^
	~CheckPoint(VOID);   // �f�X�g���N�^

	BOOL Init(VOID);                // �����ݒ�
	VOID ReSet(VOID);               // ����������
	INT  UpDate(INT player_no);     // �X�V����
	VOID Draw(INT player_no);       // �`�揈��
	VOID TwoDraw(INT player_no);    // 2�l�p�`�揈��
	VOID MultiDraw(INT player_no);  // ���l���p�`�揈��
	VOID Destroy(VOID);             // �j������

	VOID RankingSort(INT player_no, INT player_count);  // �����L���O�`�揈��
	VOID CheckDraw(INT player_no);                      // ���݂̃`�F�b�N�|�C���g���B����`��

	/* �S�[���������̏��� */
	BOOL Goal(INT player_no)
	{
		/* �v���C���[�̃��b�v�����K�萔�𒴂����Ȃ� */
		if (player_check_[player_no].lap > kFinalLap) return TRUE;
		else return FALSE;
	}

	/* ���������炤�֐� */
	VOID SetPlayerPosition(D3DXVECTOR3 position) { player_pos_[player_no_].player_pos_ = position; }
	VOID SetPlayerOrigin(D3DXVECTOR3 origin) { player_pos_[player_no_].origin_pos_ = origin; }
	VOID SetMapNo(LPCSTR MapNo) { check_no_ = MapNo; }

	/* �ϐ���n���֐� */
	D3DXVECTOR3 ReturnStartPos(INT no) { return player_pos_[no].start_position_; }
	INT ReturnRanking(INT player_no) { return player_check_[player_no].ranking; }

	D3DXVECTOR3 ReturnPosition(INT player_no) { return return_position_[player_no]; }
};
#endif