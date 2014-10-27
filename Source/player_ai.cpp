/***********
 * Include *
 ***********/
#include "player_ai.h"

/******************
 * �R���X�g���N�^ *
 ******************/
PlayerAI::PlayerAI(VOID)
{
}

/****************
 * �f�X�g���N�^ *
 ****************/
PlayerAI::~PlayerAI(VOID)
{
}

/*******************************************
 * �֐��� : Init�֐�                       *
 * ��  �e : �����������s���܂�             *
 * ��  �� : �Ȃ�                           *
 * �߂�l : �����ɐ���������TRUE��Ԃ��܂� *
 *******************************************/
BOOL PlayerAI::Init(VOID)
{
	BOOL ret = TRUE;

	do
	{
		/* �v���C���[�̃��b�V�������擾 */
		if (!PlayerMeshInit())
		{
			/* �G���[ */
			ErrorDialog("�v���C���[���̎擾�Ɏ��s");
			ret = FALSE;
			break;
		}

		/* AI�����擾 */
		//if (!AiInit())
		//{
		//	/* �G���[ */
		//	ErrorDialog("AI���̎擾�Ɏ��s");
		//	ret = FALSE;
		//	break;
		//}

	} while (0);

	return ret;
}

/*******************************
 * �֐��� : UpDate�֐�         *
 * ��  �e : �X�V�������s���܂� *
 * ��  �� : �Ȃ�               *
 * �߂�l : �Ȃ�               *
 *******************************/
VOID PlayerAI::UpDate(VOID)
{
	Jump();             // �W�����v
	Direction();        // �L�����N�^�[����
	TurretDirection();  // �^���b�g����
	Accel();            // �A�N�Z��

	/* �v���C���[���̕`�攻��̐ݒ� */
	draw_pos_ = body_position_;
	draw_pos_.x += sinf(body_direction_) * kDrawPosition;
	draw_pos_.z += cosf(body_direction_) * kDrawPosition;

	bullet_.UpDate();  // �e�̍X�V
	if (bullet_flag_)bullet_pos_ = bullet_.ReturnPosition();
}

/***********************************
 * �֐��� : Jump�֐�               *
 * ��  �e : �W�����v�������s���܂� *
 * ��  �� : �Ȃ�                   *
 * �߂�l : �Ȃ�                   *
 ***********************************/
VOID PlayerAI::Jump(VOID)
{
	/* �n�ʂɂ��ă{�^���t���O�������Ă���� */

	/* ���W�̍X�V */
	body_position_.y += jump_;      // ���W�̍X�V
	jump_ -= performance_.Gravity;  // �W�����v�l�����X�ɉ�����
	origin_pos_ = body_position_;   // �ړ��O�̍��W��ݒ�
}

/***********************************************
 * �֐��� : Direction�֐�                      *
 * ��  �e : �L�����N�^�[�̌����̊Ǘ����s���܂� *
 * ��  �� : �Ȃ�                               *
 * �߂�l : �Ȃ�                               *
 ***********************************************/
VOID PlayerAI::Direction(VOID)
{
	/* ������ݒ肷�� */
	body_direction_ += body_slope_;
}

/***************************************
 * �֐��� : TurretDirection�֐�        *
 * ��  �e : �^���b�g�̌������Ǘ����܂� *
 * ��  �� : �Ȃ�                       *
 * �߂�l : �Ȃ�                       *
 ***************************************/
VOID PlayerAI::TurretDirection(VOID)
{
	/* �^���b�g�̌��������肷�� */
	turret_direction_ += (body_slope_ + turret_slope_);
}

/*********************************
 * �֐��� : Accel�֐�            *
 * ��  �e : �ړ��̊Ǘ����s���܂� *
 * ��  �� : �Ȃ�                 *
 * �߂�l : �Ȃ�                 *
 *********************************/
VOID PlayerAI::Accel(VOID)
{
	/* �����Ă�������Ɉړ� */
	body_position_.x += sin(body_direction_) * move_;
	body_position_.z += cos(body_direction_) * move_;
}

/***********************************************
 * �֐��� : AiInit�֐�                         *
 * ��  �e : AI�e�L�X�g��ǂݍ��ݕۑ����܂�     *
 * ��  �� : �Ȃ�                               *
 * �߂�l : �ǂݍ��݂ɐ���������TRUE��Ԃ��܂� *
 ***********************************************/
BOOL PlayerAI::AiInit(VOID)
{
	BOOL ret = TRUE;  // �߂�l�p
	FILE* fp;         // �t�@�C���|�C���^
	CHAR file_name[NAME_LEN];

	INT left_stcik,
		right_stick,
		accel,
		brake,
		jump;

	_chdir("Resource_fast/AI/green_ai");

	do
	{
		/* �}�b�v�ԍ��̎w�� */
		sprintf_s(file_name, "MapAI%d.txt");

		/* �t�@�C���̓ǂݍ��� */
		fopen_s(&fp, file_name, "r");

		/* �t�@�C�����ǂݍ��߂����̊m�F */
		if (!fp)
		{
			/* �G���[ */
			ErrorDialog("AI�̃t�@�C���ǂݍ��݂Ɏ��s���܂���");
			ret = FALSE;
			break;
		}

		while (fscanf_s(fp, "%d %d %d %d %d", &left_stcik, &right_stick, &accel, &brake, &jump) == EOF)
		{
			left_stick_list_.push_back(left_stcik);
			right_stick_list_.push_back(right_stick);
			accel_list_.push_back(accel);
			brake_list_.push_back(brake);
			jump_list_.push_back(jump);
		}

	} while (0);

	_chdir("../../../");

	return ret;
}