/***********
 * Include *
 ***********/
#include "player.h"

/******************
 * �R���X�g���N�^ *
 ******************/
Player::Player(VOID)
{
}

/****************
 * �f�X�g���N�^ *
 ****************/
Player::~Player(VOID)
{
}

/*********************************
 * �֐��� : Init�֐�             *
 * ��  �e : �������������s���܂� *
 * ��  �� : �Ȃ�                 *
 * �߂�l : �Ȃ�                 *
 *********************************/
BOOL Player::Init(VOID)
{
	/* �߂�l�p */
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

		player_radius_ = collision_.SphereCreate(turret_mesh_);
		player_radius_ /= 1.5F;

		bullet_radius_ = bullet_.ReturnRadius();
		bullet_radius_ *= 1.5F;
	} while (0);

	return ret;
}

/*******************************
 * �֐��� : UpDate             *
 * ��  �e : �X�V�������s���܂� *
 * ��  �� : �Ȃ�               *
 * �߂�l : �Ȃ�               *
 *******************************/
VOID Player::UpDate(VOID)
{
	button_[player_no_] = XInput::ReturnPushButton(player_no_);

	Jump();             // �W�����v
	TurretDirection();  // �^���b�g����
	Direction();        // �L�����N�^�[����
	Accel();            // �A�N�Z��

	/* �v���C���[�̕`��͈͂̐ݒ� */
	draw_pos_ = body_position_;
	draw_pos_.x += sinf(body_direction_) * kDrawPosition;
	draw_pos_.z += cosf(body_direction_) * kDrawPosition;

	bullet_.UpDate();  // �e�̍X�V
	if (bullet_flag_)bullet_pos_ = bullet_.ReturnPosition();
	bullet_pos_.y += 0.5F;

	if (body_position_.y <= -20.0F)
	{
		body_position_.y = 0.0F;
		body_position_ = save_pos_;
		origin_pos_ = save_pos_;
	}
}

/***********************************************
 * �֐��� : Jump�֐�                           *
 * ��  �e : �v���C���[�̃W�����v�������s���܂� *
 * ��  �� : �Ȃ�                               *
 * �߂�l : �Ȃ�                               *
 ***********************************************/
VOID Player::Jump(VOID)
{
	/* �n�ʂɂ��ă{�^���������ꂽ�� */
	if ((button_[player_no_] & XINPUT_GAMEPAD_A) && !jump_flag_)
	{
		DSound::Restart(JUMP_SE);
		DSound::Play(JUMP_SE);

		jump_list_.push_back(XINPUT_GAMEPAD_A);

		jump_        = performance_.Jump;  // �W�����v�l����
		jump_flag_   = TRUE;               // �W�����v�t���O�𗧂Ă�

		bullet_flag_ = TRUE;               // �e�����Ă��Ԃɂ���
		bullet_.BulletSet(body_position_, turret_direction_, move_);  // �e�̐ݒ�
	}

	/* ���W�̍X�V */
	body_position_.y += jump_;      // ���W�̍X�V
	origin_pos_ = body_position_;   // �ړ��O�̍��W��ݒ�
}

/***********************************************
 * �֐��� : Direction                          *
 * ��  �e : �L�����N�^�[�̌����̊Ǘ����s���܂� *
 * ��  �� : �Ȃ�                               *
 * �߂�l : �Ȃ�                               *
 ***********************************************/
VOID Player::Direction(VOID)
{
	/* ���X�e�B�b�N�̒l���擾 */
	INT thumb = XInput::AdditionLeftX(player_no_);

	/* �E�ɓ��͂��ꂽ�� */
	if (thumb > 0)
	{
		/* �E������ */
		body_slope_ += performance_.Direction;  // �p�x

		/* �ő�l�ɂȂ�����Œ肷�� */
		if (body_slope_ >= performance_.MaxDirection)
		{
			body_slope_ = performance_.MaxDirection;
		}
	}

	/* ���ɓ��͂��ꂽ�� */
	if (thumb < 0)
	{
		/* �������� */
		body_slope_ -= performance_.Direction;  // �p�x

		/* �ő�l�ɂȂ�����Œ肷�� */
		if (body_slope_ <= -performance_.MaxDirection)
		{
			body_slope_ = -performance_.MaxDirection;
		}
	}

	/* ���͂��Ȃ������Ȃ珉�������� */
	if (thumb == 0)
	{
		body_slope_ = 0.0F;
		
	}

	left_stick_list_.push_back(thumb);

	/* ������ݒ肷�� */
	body_direction_ += body_slope_;
}

/***************************************
 * �֐��� : TurretDirection�֐�        *
 * ��  �e : �^���b�g�̌������Ǘ����܂� *
 * ��  �� : �Ȃ�                       *
 * �߂�l : �Ȃ�                       *
 ***************************************/
VOID Player::TurretDirection(VOID)
{
	/* �E�X�e�B�b�N�̒l���擾 */
	INT thumb = XInput::AdditionRightX(player_no_);

	if (button_[player_no_] & XINPUT_GAMEPAD_RIGHT_THUMB) hold_flag_ = !hold_flag_;

	/* �E�ɓ��͂��ꂽ�� */
	if (thumb > 0)
	{
		/* �E������ */
		turret_slope_ += performance_.Direction;  // �p�x

		/* �ő�l�ɂȂ�����Œ肷�� */
		if (turret_slope_ >= performance_.MaxDirection)
		{
			turret_slope_ = performance_.MaxDirection;
		}
	}

	/* ���ɓ��͂��ꂽ�� */
	if (thumb < 0)
	{
		/* �������� */
		turret_slope_ -= performance_.Direction;  // �p�x

		/* �ő�l�ɂȂ�����Œ肷�� */
		if (turret_slope_ <= -performance_.MaxDirection)
		{
			turret_slope_ = -performance_.MaxDirection;
		}
	}

	/* ���͂��Ȃ������Ȃ珉�������� */
	if (thumb == 0)
	{

		turret_slope_ = 0.0F;        // �p�x�̏�����


		if (!hold_flag_)
		{
			/* ���͂��Ȃ�������C���̌�����߂� */
			if (turret_direction_ > body_direction_) turret_direction_ -= performance_.MaxDirection;
			else if (turret_direction_ < body_direction_) turret_direction_ += performance_.MaxDirection;
		}
	}

	right_stick_list_.push_back(thumb);

	/* �^���b�g�̌��������肷�� */
	turret_direction_ += (body_slope_ + turret_slope_);
}
/*****************************************
 * �֐��� : Accel�֐�                    *
 * ��  �e : �v���C���[�̈ړ����Ǘ����܂� *
 * ��  �� : �Ȃ�                         *
 * �߂�l : �Ȃ�                         *
 *****************************************/
VOID Player::Accel(VOID)
{
	INT engine_se = ENGINE_SE + player_no_;

	/* �{�^������ */
	INT right_trigger = XInput::DownRightTrigger(player_no_);
	INT left_trigger  = XInput::DownLeftTrigger(player_no_);

	/* �A�N�Z���{�^����������Ă����� */
	if (right_trigger > 0)
	{
		/* �G���W�����̍Đ� */
		DSound::Play(engine_se);

		/* ���X�ɉ����������� */
		if      (move_ <= performance_.GearOne)   move_ += right_trigger * performance_.GearOneSpeed;    // 1�� 
		else if (move_ <= performance_.GearTwo)   move_ += right_trigger * performance_.GearTwoSpeed;    // 2��
		else if (move_ <= performance_.GearThree) move_ += right_trigger * performance_.GearThreeSpeed;  // 3��
		else                                      move_ += right_trigger * performance_.GearForSpeed;    // 4��

		/* �ő呬�x�ɂȂ����� */
		if (move_ > max_speed_)
		{
			move_ -= performance_.Inertia ;  // �ō������ێ�����

			/* 0�ȉ��ɂȂ�����Œ肷�� */
			if (move_ <= 0.0F) move_ = 0.0F;
		}
	}
	/* �A�N�Z���{�^���������ꂽ�� */
	else if (move_ >= 0.0F)
	{
		DSound::Pause(engine_se);

		/* ���X�ɑ��x�������� */
		move_ -= performance_.Inertia;

		/* 0�ɂȂ�����Œ肷�� */
		if (move_ < 0.0F)  move_ = 0.0F;
	}
	/* �u���[�L���� */
	if (left_trigger > 0)
	{

		/* �������c���Ă����� */
		if (move_ > 0.0F)
		{
			move_ -= performance_.Brake;

			/* �����x��0�ȉ��ɂȂ�����Œ肷�� */
			if (move_ < 0.0F) move_ = 0.0F;
		}
	}

	/* �����Ă�������Ɉړ� */
	body_position_.x += sin(body_direction_) * move_;
	body_position_.z += cos(body_direction_) * move_;
}