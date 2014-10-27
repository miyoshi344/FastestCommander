/******************************
 * �t�B�[���h�ɕ\������N���X *
 ******************************/

/* ���dInclude�h�~ */
#ifndef RACE_FIELD_H_
#define RACE_FIELD_H_

/***********
 * Include *
 ***********/
#include "objectbase.h"
#include "fieldobject.h"
#include "collision.h"

/********
 * �萔 *
 ********/

/* ���b�V����� */
CONST LPSTR kFieldMap = "Field.csv";  // �X�e�[�W�t�@�C��
CONST LPSTR kSkyBall  = "skyball_morning.x";  // �X�J�C�{�[��

/* �t�B�[���h�֌W�̐��� */
CONST FLOAT kFieldCollision = 1.0F;   // �����蔻��擾�͈�
CONST FLOAT kSlopeDirection = 90.0F;  // ��̊p�x

/* �ǂ̍��� */
CONST INT kWallLoop  = 5;  // �ǂ̍���
CONST INT kDammyLoop = 5;  // �_�~�[�ǂ̍���

/******************************************
 * �t�B�[���h�̕`���Ǘ����s���N���X�ł� *
 * ObjectBase���p�����č쐬���܂�         *
 ******************************************/
class Field : public ObjectBase
{
private:

	/**************
	 * �N���X��` *
	 **************/
	Collision    collision_;  // �����蔻��N���X

	FieldObject* block[BLOCK_MAX];  // �u���b�N�i�[�p�N���X�|�C���^
	D3DXVECTOR3 min_, max_, len_;

	/********
	 * �ϐ� *
	 ********/
	LPCSTR map_no_;    // �}�b�v�̖��O
	INT    player_;    // �v���C���[�ԍ�
	INT    field_no_;  // �t�B�[���h�ԍ�

	/* �v���C���[���W */
	D3DXVECTOR3 origin_pos_[kAllPlayer];      // �v���C���[�̈ړ��O���W
	D3DXVECTOR3 player_pos_[kAllPlayer];      // �v���C���[�̈ړ�����W
	D3DXVECTOR3 player_draw_pos_[kAllPlayer]; // �`��p�v���C���[�̑O�����W
	FLOAT       player_radius_[kAllPlayer];

	/* �e�̍��W */
	D3DXVECTOR3 origin_bullet_[kAllPlayer];  // �e�̈ړ��O���W
	D3DXVECTOR3 bullet_pos_[kAllPlayer];     // �e�̈ړ�����W
	FLOAT       bullet_radius_[kAllPlayer];

	/**********
	 * ���X�g *
	 **********/

	/* �}�b�v�̏�� */
	std::list<D3DXMATRIX>  matrix_list_;  // ���[���h���X�g
	std::list<D3DXVECTOR3> pos_list_;     // ���W���X�g
	std::list<INT> information_list_;     // �u���b�N�ԍ�

	/* ��ԕۑ��p */
	std::list<INT> info_list_;            // �t�B�[���h��ԕۑ��p���X�g

	/* �`��p */
	std::list<D3DXMATRIX>  draw_matrix_[kAllPlayer];       // �`��p���[���h���X�g
	std::list<D3DXVECTOR3> draw_pos_[kAllPlayer];          // �`��͈͓�������W���X�g
	std::list<INT>         draw_information_[kAllPlayer];  // �`��͈͓��u���b�N�ԍ�

	/* �����蔻��p */
	std::list<D3DXVECTOR3> player_pos_list_;
	std::list<D3DXMATRIX>  player_list_;         // �v���C���[�t�߂̔���p���X�g
	std::list<INT>         player_information_;  // �v���C���[�t�߂̔���p�u���b�N�ԍ�
	std::list<FLOAT>       player_radius_list_;
	std::list<D3DXVECTOR3> player_len_list_;

	/**************
	 * �C�e���[�^ *
	 **************/

	/* �}�b�v�̏�� */
	std::list<D3DXMATRIX>::iterator  it_matrix_;       // �S�̂̃��[���h���W
	std::list<D3DXVECTOR3>::iterator it_position_;     // �S�̂̍��W
	std::list<INT>::iterator         it_infomation_;   // �S�̂̃��X�g
	std::list<D3DXVECTOR3>::iterator it_end_position_; // �S�̂̏I���p

	/* �`��p */
	std::list<D3DXVECTOR3>::iterator it_draw_pos_;
	std::list<D3DXMATRIX>::iterator  it_draw_matrix_;      // �`��p���[���h���W
	std::list<INT>::iterator         it_draw_infomation_;  // �`��p�u���b�N�ԍ�
	std::list<D3DXMATRIX>::iterator  it_draw_end_;         // �`��I���p

	/* �v���C���[�����蔻��p */
	std::list<D3DXVECTOR3>::iterator it_player_pos_;
	std::list<D3DXMATRIX>::iterator  it_player_matrix_;      // �v���C���[�߂��̃u���b�N�̍��W
	std::list<INT>::iterator         it_player_infomation_;  // �v���C���[�߂��̃u���b�N�̏��
	std::list<FLOAT>::iterator       it_radius_;             
	std::list<D3DXVECTOR3>::iterator it_len_list_;
	std::list<D3DXMATRIX>::iterator  it_player_end_;         // �v���C���[�߂�����̏I���p

	/* �e�p�����蔻�� */
	std::list<D3DXMATRIX>::iterator  it_bullet_matrix_;      // �e�̋߂��̃u���b�N�̃��[���h���W
	std::list<D3DXVECTOR3>::iterator it_bullet_position_;    // �e�̋߂��̃u���b�N���W
	std::list<INT>::iterator         it_bullet_infomation_;  // �e�̋߂��̃u���b�N���
	std::list<D3DXMATRIX>::iterator  it_bullet_end_;         // �e�̔���̏I���p

	/************
	 * �����֐� *
	 ************/
	BOOL SkyInit(VOID);         // �X�J�C�{�[�������ǂݍ���
	BOOL BlockInit(VOID);       // �u���b�N�����ǂݍ���
	BOOL MapFileSet(VOID);      // �}�b�v�����ǂݍ���
	 
	VOID PlayerHitCheck(VOID);  // �v���C���[�̓����蔻��`�F�b�N
	VOID FieldDraw(VOID);       // �t�B�[���h�̕`��
	VOID SkyDraw(VOID);         // �X�J�C�{�[���̕`��

public:
	Field(VOID);         // �R���X�g���N�^
	~Field(VOID);        // �f�X�g���N�^

	BOOL Init(VOID);     // ��������
	VOID ReSet(VOID);    // ����������
	VOID UpDate(VOID);   // �X�V����
	VOID Draw(VOID);     // �`�揈��
	VOID Destroy(VOID);  // �j������
	INT  BlockCheck(VOID);              // �v���C���[�̃u���b�N����
	BOOL BulletCheck(INT PlayerColor);  // �e�̃u���b�N����

	/**********************
	 * �ϐ����󂯎��֐� *
	 **********************/
	VOID SetPlayerPos(D3DXVECTOR3 player_pos)       { player_pos_[player_] = player_pos; }        // �v���C���[���Ƃ̍X�V�O���W�����炤
	VOID SetOriginPos(D3DXVECTOR3 origin_pos)       { origin_pos_[player_] = origin_pos; }        // �v���C���[���Ƃ̍X�V��̍��W�����炤
	VOID SetDrawPos(D3DXVECTOR3 draw_pos)           { player_draw_pos_[player_] = draw_pos; }     // �`�悷��ʒu�̍��W�����炤
	VOID SetOriginBullet(D3DXVECTOR3 origin_bullet) { origin_bullet_[player_] = origin_bullet; }  // �e�̍X�V�O�̍��W�����炤
	VOID SetBulletPos(D3DXVECTOR3 bullet_pos)       { bullet_pos_[player_] = bullet_pos; }        // �e�̍X�V��̍��W�����炤 
	VOID SetPlayerRadius(FLOAT radius)              { player_radius_[player_] = radius; }
	VOID SetBulletRadius(FLOAT radius)              { bullet_radius_[player_] = radius; }

	VOID SetPlayerNo(INT player_no) { player_   = player_no; }  // �v���C���[�̔ԍ�
	VOID SetMapNo(LPCSTR MapNo)     { map_no_   = MapNo; }      // �ǂݍ��ރ}�b�v�̃t�@�C����
	VOID SetFieldNo(INT field_no)   { field_no_ = field_no; }   // ���݂̃}�b�v�ԍ�
};

#endif