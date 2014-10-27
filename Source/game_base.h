/**********************
 * �Q�[���̊�b�N���X *
 **********************/

/* ���dInclude�h�~ */
#ifndef RACE_GAME_BASE_H_
#define RACE_GAME_BASE_H_

/***********
 * Include *
 ***********/
#include "flow_base.h"
#include "directsound.h"
#include "field.h"
#include "character_base.h"
#include "player.h"
#include "player_ai.h"
#include "collision.h"
#include "camera.h"
#include "time.h"
#include "speed.h"
#include "checkPoint.h"
#include "camera.h"
#include "ranking.h"

/********
 * �萔 *
 ********/

/**********
 * �\���� *
 **********/
CONST struct FieldMap
{
	LPCSTR FieldMap1 = "Resource_fast/Csv/Map1";  // �}�b�v1�̃t�@�C���ۊǏꏊ
	LPCSTR FieldMap2 = "Resource_fast/Csv/Map2";  // �}�b�v2�̃t�@�C���ۊǏꏊ
	LPCSTR FieldMap3 = "Resource_fast/Csv/Map3";  // �}�b�v3�̃t�@�C���ۊǏꏊ
	LPCSTR FieldMap4 = "Resource_fast/Csv/Map4";  // �}�b�v4�̃t�@�C���ۊǏꏊ
};

/******************************************
* �Q�[���̃��C���������Ǘ�����N���X�ł� *
******************************************/
class GameBase : public FlowBase
{
protected:

	/**************
	 * �N���X��` *
	 **************/

	/* �t�B�[���h�֌W */
	FieldMap       map_;                     // �}�b�v�̃t�@�C���ۊǏꏊ
	Collision      collision_;               // �����蔻��N���X
	Field          field_[MAX_FIELD];        // �t�B�[���h�N���X
	CheckPoint     check_point_[MAX_FIELD];  // �`�F�b�N�|�C���g�N���X

	/* �L�����N�^�[�֌W */
	CharacterBase* player_[kAllPlayer];  // �v���C���[�N���X
	Camera         camera_[kAllPlayer];  // �J�����N���X

	/* ���C�A�E�g�֌W */
	Speed          speed_[kAllPlayer];   // �X�s�[�h�J�E���^�N���X
	Time           time_;                // �^�C���N���X
	Ranking        ranking_;             // �����L���O�N���X

	/************
	 * �ϐ��錾 *
	 ************/
	BOOL pause_;                     // �|�[�Y�t���O
	BOOL time_count_;                // �J�E���g�_�E���t���O
	BOOL ranking_flag_[kAllPlayer];  // �����L���O�\���t���O

	INT count_;         // �J�E���g�_�E���p�~���b
	INT count_down_;    // �J�E���g�_�E���p�b
	INT result_count_;  // ���U���g�ڍs�l���ۑ��p

	INT bgm_;  // �Đ�����BGM�ԍ��ۑ��p

	/************
	 * �����֐� *
	 ************/
	BOOL MapInit(VOID);                // �}�b�v�̏�����
	BOOL PlayerInit(VOID);             // �v���C���[�̏�����
	VOID DrawPlayer(INT player);       // �v���C���[���m�̕`��͈͏���
	VOID GameSet(INT player);          // �Q�[�����̏�ԍX�V����
	VOID GameUpDate(INT player);       // �Q�[�����̍X�V����
	VOID GameResult(INT player);       // �Q�[�����̃��U���g����
	VOID ResultUpDate(INT player_no);  // ���U���g�̍X�V����
	VOID ResultDraw(INT player_no);    // ���U���g�̕`�揈��
	INT NextStage(VOID);               // �X�e�[�W�ڍs����
	BOOL CountDowm(VOID);              // �J�E���g�_�E������
	BOOL Pause(INT player_no);         // �|�[�Y�̏��� 

	/* �������z�֐� */
	virtual VOID SpriteDraw(VOID) = 0; // �X�v���C�g�`�揈��

public:
	GameBase(VOID);   // �R���X�g���N�^
	~GameBase(VOID);  // �f�X�g���N�^

	BOOL Init(VOID);     // �����ݒ�
	VOID ReSet(VOID);    // ����������
	VOID Destroy(VOID);  // �j������

	/* �������z�֐� */
	virtual INT UpDate(VOID) = 0; // �X�V����
	virtual VOID Draw(VOID)  = 0; // �`�揈��
};

#endif