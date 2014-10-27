/****************************
 * �L�����N�^�[�̊�b�N���X *
 ****************************/

/* ���dInclude�h�~ */
#ifndef RACE_CHARACTER_BASE_H_
#define RACE_CHARACTER_BASE_H_

/***********
 * Include *
 ***********/
#include "objectbase.h"
#include "collision.h"
#include "bullet.h"

/********
 * �萔 *
 ********/
CONST FLOAT kDrawPosition = 22.0F;  // �v���C���[���̕`��͈�

/**********
 * �\���� *
 **********/

/* �v���C���[�̃X�e�[�^�X�Ɋւ���\���� */
CONST struct TankPerformance
{
	/************
	 * �X�s�[�h *
	 ************/
	FLOAT MaxSpeed       = 0.41F;  // �ō���
	FLOAT DownSpeed      = 0.01F;  // ���n�ʂł̌����l
	FLOAT DownColorSpeed = 0.05F;  // ���̃t�B�[���h�𓥂񂾎��̌����l
	FLOAT DownMaxSpeed   = 0.11F;  // ���n�ʂł̍ō���
	
	FLOAT Brake        = 0.005F; // �u���[�L

	FLOAT Inertia      = 0.003F;   // �����l
	FLOAT Accel        = 0.00001F; // �����x

	/********
	 * �M�A *
	 ********/
	FLOAT GearOne      = 0.2F;   // 2���؂�ւ�
	FLOAT GearTwo      = 0.3F;   // 3���؂�ւ�
	FLOAT GearThree    = 0.35F;  // 4���؂�ւ�

	FLOAT GearOneSpeed   = 0.000005F;  // 1�������l
	FLOAT GearTwoSpeed   = 0.000003F;  // 2�������l
	FLOAT GearThreeSpeed = 0.000002F;  // 3�������l
	FLOAT GearForSpeed   = 0.0000008F;  // 4�������l

	/************
	 * �W�����v *
	 ************/
	FLOAT Jump    = 0.07F;   // �W�����v�l
	FLOAT Gravity = 0.005F; // �d�͒l

	/********
	 * ���� *
	 ********/
	FLOAT Direction    = 0.0003F;  // �����̓��͒l
	FLOAT MaxDirection = 0.015F;   // �����̓��͍ő�l 
};

/* �v���C���[�Ɋւ���\���� */
CONST struct Tank
{
	/* �Ԑ�� */
	LPCSTR red_body       = "tank_red_body.x";        // �{�f�B���b�V����
	LPCSTR red_turret     = "tank_red_turret.x";      // �^���b�g���b�V����
	LPCSTR red_burret     = "bullet_red.x";           // �o���b�g���b�V����
	LPCSTR red_ai_name    = "Resource_fast/AI/red_ai";     // AI�e�L�X�g�ꏊ

	/* �ΐ�� */
	LPCSTR green_body     = "tank_green_body.x";      // �{�f�B���b�V����
	LPCSTR green_turret   = "tank_green_turret.x";    // �^���b�g���b�V����
	LPCSTR green_burret   = "bullet_green.x";         // �o���b�g���b�V����
	LPCSTR green_ai_name  = "Resource_fast/AI/green_ai";    // AI�e�L�X�g�ꏊ

	/* ����� */
	LPCSTR yellow_body    = "tank_yellow_body.x";     // �{�f�B���b�V����
	LPCSTR yellow_turret  = "tank_yellow_turret.x";   // �^���b�g���b�V����
	LPCSTR yellow_bullet  = "bullet_yellow.x";        // �o���b�g���b�V����
	LPCSTR yellow_ai_name = "Resource_fast/AI/yellow_ai";  // AI�e�L�X�g�ꏊ

	/* ��� */
	LPCSTR blue_body      = "tank_blue_body.x";       // �{�f�B���b�V����
	LPCSTR blue_turret    = "tank_blue_turret.x";     // �^���b�g���b�V����
	LPCSTR blue_burret    = "bullet_blue.x";          // �o���b�g���b�V����
	LPCSTR blue_ai_name   = "Resource_fast/AI/blue_ai";    // AI�e�L�X�g�ꏊ
};

/************************************************
 * �L�����N�^�[�p�̊�b�N���X�ł�               *
 * �L�����N�^�[�͂��̃N���X���p�����č쐬���܂� *
 ************************************************/
class CharacterBase
{
protected:
	/**************
	 * �N���X��` *
	 **************/
	Collision collision_;
	Bullet    bullet_;  // �e�̃N���X

	/**************
	 * �\���̒�` *
	 **************/
	TankPerformance performance_;  // ��Ԃ̃X�e�[�^�X���
	Tank tank_;                    // ��ԏ��

	/* ���b�V�� */
	MeshTable* body_mesh_;   // �{�f�B�p���b�V��
	MeshTable* turret_mesh_; // �^���b�g�p���b�V��  

	/********
	 * �ϐ� *
	 ********/
	FLOAT player_radius_;
	FLOAT bullet_radius_;

	/* ���W */
	D3DXVECTOR3 body_position_;    // �{�f�B�p���W
	D3DXVECTOR3 turret_position_;  // �^���b�g�p���W

	D3DXVECTOR3 start_pos_;      // �X�^�[�g�ʒu
	D3DXVECTOR3 origin_pos_;     // �ړ�����O�̍��W
	D3DXVECTOR3 draw_pos_;       // �}�b�v�̕`�悷����W
	D3DXVECTOR3 save_pos_;       // ����������W�ʒu
	D3DXVECTOR3 bullet_pos_;     // �e�̍��W�ۑ��ʒu
	D3DXVECTOR3 origin_bullet_;  // �ړ�����O�̒e�̍��W

	/* ���[���h���W */
	D3DXMATRIX body_world_;    // �{�f�B�p���[���h���W
	D3DXMATRIX turret_world_;  // �^���b�g�p���[���h���W

	/* �X�e�[�^�X */
	FLOAT move_;              // ���x
	FLOAT jump_;              // �W�����v�l 
	FLOAT brake_;             // �u���[�L
	FLOAT max_speed_;         // �ő呬�x
	FLOAT body_direction_;    // �{�f�B����
	FLOAT body_slope_;        // �{�f�B�����̓��͕ۑ�
	FLOAT turret_direction_;  // �^���b�g����
	FLOAT turret_slope_;      // �^���b�g�����̓��͕ۑ�

	BOOL jump_flag_;    // �W�����v�o���邩�̃t���O
	BOOL bullet_flag_;  // �e�����Ă邩�̃t���O
	BOOL hold_flag_;

	INT player_no_;     // �v���C���[�ԍ�
	INT player_color_;  // �v���C���[�J���[

	ULONG button_[kMaxPad];  // �{�^�����͕ۑ��p

	/**********
	 * ���X�g *
	 **********/
	std::list<INT> left_stick_list_;   // ���X�e�B�b�N���͕ۑ��p
	std::list<INT> right_stick_list_;  // �E�X�e�B�b�N���͕ۑ��p
	std::list<INT> accel_list_;        // �A�N�Z�����͕ۑ��p
	std::list<INT> brake_list_;        // �u���[�L���͕ۑ��p
	std::list<WORD> jump_list_;        // �W�����v���͕ۑ��p

	/**************
	 * �C�e���[�^ *
	 **************/
	std::list<INT>::iterator it_left_stick_;  // ���X�e�B�b�N�p�C�e���[�^
	std::list<INT>::iterator it_right_stick_; // �E�X�e�B�b�N�p�C�e���[�^
	std::list<INT>::iterator it_accel_;       // �A�N�Z���p�C�e���[�^
	std::list<INT>::iterator it_brake_;       // �u���[�L�p�C�e���[�^
	std::list<WORD>::iterator it_jump_;       // �W�����v�p�C�e���[�^

	/************
	 * �����֐� *
	 ************/

	/* �������z�֐� */
	virtual VOID Jump(VOID)            = 0;  // �W�����v����
	virtual VOID Direction(VOID)       = 0;  // �ړ�����
	virtual VOID TurretDirection(VOID) = 0;  // �C���̎��_�ړ�
	virtual VOID Accel(VOID)           = 0;  // �A�N�Z������

	/* �u���b�N�ɓ����������̏��� */
	VOID Thorn(VOID);       // ���ɓ����������̏���
	VOID GroundHit(VOID);   // �n�ʂɓ����������̏���
	VOID WallHit(VOID);     // �ǂɂԂ���������
	VOID SlopeHit(VOID);    // ��ɓ����������̏���

	/* ���̎擾��`�� */
	BOOL PlayerMeshInit(VOID);         // ���b�V�����̎擾����
	VOID BodyDraw(VOID);               // �{�f�B�̕`�揈��
	VOID TurretDraw(VOID);             // �^���b�g�̕`�揈��
	VOID ColorBlock(INT block_color);  // �F�̕t�����n�ʂ̏���

public:
	CharacterBase(VOID);   // �R���X�g���N�^
	~CharacterBase(VOID);  // �f�X�g���N�^

	virtual BOOL Init(VOID)    = 0;  // �����ݒ�
	virtual VOID UpDate(VOID)  = 0;  // �X�V����

	VOID ReSet(VOID);      // ������
	VOID Draw(VOID);       // �`�揈��
	VOID Destroy(VOID);    // �j��
	VOID PlayerHit(VOID);  // �v���C���[���m�̓����蔻��

	VOID BlockCheck(INT block_);    // �����蔻��`�F�b�N�p�֐�

	/* �l��n���֐� */ 
	D3DXVECTOR3 ReturnOrigin(VOID)       { return origin_pos_; }     // �X�V����O�̍��W
	D3DXVECTOR3 ReturnPosition(VOID)     { return body_position_; }  // �X�V������̍��W
	D3DXVECTOR3 ReturnDrawPosition(VOID) { return draw_pos_; }       // �}�b�v�̕`��ʒu���W
	D3DXVECTOR3 ReturnOriginBullet(VOID) { return origin_bullet_; }  // �e�̍X�V����O�̍��W
	D3DXVECTOR3 ReturnBullet(VOID)       { return bullet_pos_; }     // �e�̍X�V������̍��W
	MeshTable*  ReturnMesh(VOID)         { return body_mesh_; }

	FLOAT ReturnDirection(VOID)    { return body_direction_; }          // ����l
	FLOAT ReturnMove(VOID)         { return move_; }                    // ���x
	FLOAT ReturnPlayerRadius(VOID) { return player_radius_; }
	FLOAT ReturnBulletRadius(VOID) { return bullet_radius_; }

	/* �l�����炤�֐� */
	VOID StartPosition(D3DXVECTOR3 pos)   { start_pos_ = pos; }              // �X�^�[�g�ʒu
	VOID BulletFlag(VOID)                 { bullet_flag_ = FALSE; }          // �e���������Ă��邩�̃t���O
	VOID SetPlayerColor(INT player_color) { player_color_ = player_color; }  // �v���C���[�̐F���
	VOID SetPlayerNo(INT player_no)       { player_no_ = player_no; }        // �v���C���[�̔ԍ����

	VOID ReturnPosition(D3DXVECTOR3 pos) { save_pos_ = pos; }
};

#endif
