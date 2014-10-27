/***********
 * Include *
 ***********/
#include "character_base.h"

/******************
 * �R���X�g���N�^ *
 ******************/
CharacterBase::CharacterBase(VOID)
{
	/* �ϐ������� */
	player_radius_ = 0.0F;
	bullet_radius_ = 0.0F;

	/* �����l������ */
	body_direction_   = 0.0F;
	turret_direction_ = 0.0F;
	body_slope_       = 0.0F;
	turret_slope_     = 0.0F;

	/* ��ԏ����� */
	move_ = 0.0F;
	jump_ = 0.0F;

	/* �X�s�[�h�̏����� */
	max_speed_ = performance_.MaxSpeed;  // �ō���
	brake_     = performance_.Brake;     // �u���[�L�l

	/* �t���O�Ǘ� */
	jump_flag_   = FALSE;
	bullet_flag_ = FALSE;
	hold_flag_   = FALSE;
}

/****************
 * �f�X�g���N�^ *
 ****************/
CharacterBase::~CharacterBase(VOID)
{
	/* ���b�V���̉�� */
	Mesh::Release(body_mesh_);
	Mesh::Release(turret_mesh_);

	/* �e�̔j�� */
	bullet_.Destroy();
}


/***********************************
 * �֐��� : ReSet�֐�              *
 * ��  �e : ��Ԃ̏��������s���܂� *
 * ��  �� : �Ȃ�                   *
 * �߂�l : �Ȃ�                   *
 ***********************************/
VOID CharacterBase::ReSet(VOID)
{
	/* �X�^�[�g�ʒu��ݒ� */
	body_position_ = start_pos_;
	draw_pos_ = body_position_;

	/* �ϐ������� */

	/* �����l������ */
	body_direction_   = 0.0F;
	turret_direction_ = 0.0F;
	body_slope_       = 0.0F;
	turret_slope_     = 0.0F;

	/* �X�e�[�^�X������ */
	move_ = 0.0F;
	jump_ = 0.0F;

	/* ���x�̏����� */
	max_speed_ = performance_.MaxSpeed;
	brake_     = performance_.Brake;

	/* �t���O�Ǘ� */
	jump_flag_   = FALSE;
	bullet_flag_ = FALSE;
	hold_flag_   = FALSE;

	/* �e�̏����� */
	bullet_.ReSet();

	DSound::Pause(ENGINE_SE);
}

/*******************************
 * �֐��� : Draw�֐�           *
 * ��  �e : �`�揈�����s���܂� *
 * ��  �� : �Ȃ�               *
 * �߂�l : �Ȃ�               *
 *******************************/
VOID CharacterBase::Draw(VOID)
{
	BodyDraw();    // �{�f�B�̕`��
	TurretDraw();  // �^���b�g�̕`��

	/* �e�̕`�� */
	if (bullet_flag_) bullet_.Draw();

	//D3DXVECTOR3 pos = turret_position_;
	//pos.y += 0.5F;

	//collision_.SphereMesh(turret_mesh_, pos, radius_);
}

/*******************************
 * �֐��� : Destroy�֐�        *
 * ��  �e : �j���������s���܂� *
 * ��  �� : �Ȃ�               *
 * �߂�l : �Ȃ�               *
 *******************************/
VOID CharacterBase::Destroy(VOID)
{
	/* ���b�V���̉�� */
	Mesh::Release(body_mesh_);
	Mesh::Release(turret_mesh_);

	/* �e�̔j�� */
	bullet_.Destroy();
}

/*******************************************
 * �֐��� : GroundCheck�֐�                *
 * ��  �e : �n�ʂ̃u���b�N�����擾���܂� *
 * ��  �� : �Ȃ�                           *
 * �߂�l : �Ȃ�                           *
 *******************************************/
VOID CharacterBase::BlockCheck(INT block_)
{
	/* ���������u���b�N���Ƃ̏������s�� */
	switch (block_)
	{
		case GROUND       : GroundHit();              break;  // �n��
		case THORN        : Thorn();                  break;  // ��
		case SLOPE        : SlopeHit();               break;  // ��
		case WALL         : WallHit();                break;  // ��
		case RED_BLOCK    : ColorBlock(RED_BLOCK);    break;  // �Ԓn��
		case GREEN_BLOCK  : ColorBlock(GREEN_BLOCK);  break;  // �Βn��
		case YELLOW_BLOCK : ColorBlock(YELLOW_BLOCK); break;  // ���n��
		case BLUE_BLOCK   : ColorBlock(BLUE_BLOCK);   break;  // �n��
		case DUMMY        : break;
		default           : jump_flag_ = TRUE;       
							jump_ -= performance_.Gravity;  // �W�����v�l�����X�ɉ�����
							break;  // �n�ʂɂ��Ȃ����
	}
}

/***********************************************
 * �֐��� : Thorn�֐�                          *
 * ��  �e : ���n�`�ɓ����������̏������s���܂� *
 * ��  �� : �Ȃ�                               *
 * �߂�l : �Ȃ�                               *
 ***********************************************/
VOID CharacterBase::Thorn(VOID)
{
	/* ��������SE���Đ� */
	DSound::Play(DOWN_SE);

	/* ���x�𗎂Ƃ� */
	max_speed_ = performance_.DownMaxSpeed;  // �ō�����������
	if (max_speed_ < move_) move_ -= performance_.DownSpeed;  // �ō��������Ă����ꍇ���x��������

	jump_      = 0.0F;  // �W�����v�l��0�ɂ���
	jump_flag_ = TRUE;  // �W�����v���ł��Ȃ���Ԃɂ���
}

/*********************************************
 * �֐��� : GroundHit�֐�                    *
 * ��  �e : �n�ʂɓ����������̏������s���܂� *
 * ��  �� : �Ȃ�                             *
 * �߂�l : �Ȃ�                             *
 *********************************************/
VOID CharacterBase::GroundHit(VOID)
{
	jump_ = 0.0F;        // �W�����v�l��0�ɂ���
	jump_flag_ = FALSE;  // �W�����v���ł����Ԃɂ���

	body_position_.y += 0.005F;

	/* �ō�����߂� */
	max_speed_ = performance_.MaxSpeed;
}

/***********************************************
 * �֐��� : Wallhit�֐�                        *
 * ��  �e : �ǂɓ����������̒�~�������s���܂� *
 * ��  �� : �Ȃ�                               *
 * �߂�l : �Ȃ�                               *
 ***********************************************/
VOID CharacterBase::WallHit(VOID)
{
	/* �ǂɂԂ�����SE���Đ� */
	DSound::Restart(WALL_SE);
	DSound::Play(WALL_SE);

	/* �����Ă�������̋t�Ɉړ� */
	body_position_.x += sin(body_direction_) * -(move_ + 0.5F);
	body_position_.z += cos(body_direction_) * -(move_ + 0.5F);

	/* �����x�������� */
	move_ /= 10.0F;

	/* �����x��0�ȉ��ɂȂ肻���Ȃ�Œ肷�� */
	if (move_ <= 0.0F) move_ = 0.0F;
}

/*************************************************
 * �֐��� : RedBlock�֐�                         *
 * ��  �e : �F�u���b�N�ɐG�ꂽ���̏������s���܂� *
 * ��  �� : INT color_block �G�ꂽ�u���b�N�̐F   *
 * �߂�l : �Ȃ�                                 *
 *************************************************/
VOID CharacterBase::ColorBlock(INT color_block)
{
	/* �����̐F���ǂ�����r���s�� */
	if (player_color_ == color_block)
	{
		/* �����̐F�ƈ�v������������s�� */
		DSound::Pause(DASH_SE);
		DSound::Play(DASH_SE);

		move_     += 0.05F;
		jump_      = 0.0F;
		jump_flag_ = TRUE;
	}
	/* �����ȊO�̐F�Ȃ� */
	else
	{
		/* ��������SE���Đ� */
		DSound::Play(DOWN_SE);

		/* ���x�𗎂Ƃ� */
		max_speed_ = performance_.DownMaxSpeed;  // �ō�����������
		if (max_speed_ < move_) move_ -= performance_.DownColorSpeed;     // �ō��������Ă����ꍇ���x��������

		jump_ = 0.0F;  // �W�����v�l��0�ɂ���
		jump_flag_ = TRUE;  // �W�����v���ł��Ȃ���Ԃɂ���
	}
}

/*******************************************
 * �֐��� : SlopeHit�֐�                   *
 * ��  �e : ��ɓ����������̏������s���܂� *
 * ��  �� : �Ȃ�                           *
 * �߂�l : �Ȃ�                           *
 *******************************************/
VOID CharacterBase::SlopeHit(VOID)
{
	body_position_.y += sin(move_ * 1.3F);  // �L�����N�^�[�̈ʒu���グ��
}

/*************************************************
 * �֐��� : PlayerMeshInit�֐�                   *
 * ��  �e : �v���C���[�̃��b�V�������擾���܂� *
 * ��  �� : �Ȃ�                                 *
 * �߂�l : �Ȃ�                                 *
 *************************************************/
BOOL CharacterBase::PlayerMeshInit(VOID)
{
	BOOL ret = TRUE;
	LPCSTR body,
		   turret,
		   bullet;

	/* �v���C���[�̐F�ɍ��킹�� */
	switch (player_no_)
	{
		/* �Ԑ�� */
		case RED_TANK:
			body     = tank_.red_body;      // �{�f�B���b�V��
			turret   = tank_.red_turret;    // �^���b�g���b�V��
			bullet   = tank_.red_burret;    // �o���b�g���b�V��
			break;

		/* �ΐ�� */
		case GREEN_TANK:
			body     = tank_.green_body;     // �{�f�B���b�V��
			turret   = tank_.green_turret;   // �^���b�g���b�V��
			bullet   = tank_.green_burret;   // �o���b�g���b�V��
			break;

		/* ����� */
		case YELLOW_TANK:
			body     = tank_.yellow_body;     // �{�f�B���b�V��
			turret   = tank_.yellow_turret;   // �^���b�g���b�V��
			bullet   = tank_.yellow_bullet;   // �o���b�g���b�V��
			break;

		/* ��� */
		case BLUE_TANK:
			body     = tank_.blue_body;       // �{�f�B���b�V��
			turret   = tank_.blue_turret;     // �^���b�g���b�V��
			bullet   = tank_.blue_burret;     // �o���b�g���b�V��
			break;

		default: break;
	}

	/* �ǂݍ��ރ��b�V����I�� */
	bullet_.BulletColor(bullet);

	do
	{
		/* �e�̏����� */
		if (!bullet_.Init())
		{
			/* �G���[ */
			ErrorDialog("�e�̃��b�V���擾�Ɏ��s");
			ret = FALSE;
			break;
		}

		/* �t�H���_�K�w���ړ����� */
		_chdir("Resource_fast/Mesh/Tank");

		/* ���b�V���̓ǂݍ��� */
		if ((body_mesh_ = Mesh::Load(body)) == NULL)
		{
			/* �G���[ */
			ErrorDialog("�{�f�B�̃��b�V���擾�Ɏ��s");
			ret = FALSE;
			break;
		}

		/* ���b�V���̓ǂݍ��� */
		if ((turret_mesh_ = Mesh::Load(turret)) == NULL)
		{
			/* �G���[ */
			ErrorDialog("�^���b�g�̃��b�V���擾�Ɏ��s");
			ret = FALSE;
			break;
		}

	} while (0);

	/* �t�H���_�K�w��߂� */
	_chdir("../../../");

	return ret;
}

/***************************************
 * �֐��� : BodyDraw�֐�               *
 * ��  �e : �{�f�B�̕`�揈�����s���܂� *
 * ��  �� : �Ȃ�                       *
 * �߂�l : �Ȃ�                       *
 ***************************************/
VOID CharacterBase::BodyDraw(VOID)
{
	/* ���W�Ɗp�x��ݒ� */
	D3DXMATRIX body_pos, body_rota;

	/* ��]�̐ݒ� */
	D3DXMatrixRotationY(&body_rota, body_direction_);

	/* �ړ��̐ݒ� */
	D3DXMatrixTranslation(&body_pos, body_position_.x, body_position_.y, body_position_.z);

	/* ���[���h���W�n���쐬 */
	D3DXMatrixIdentity(&body_world_);

	/* �s����|�����킹�� */
	D3DXMatrixMultiply(&body_world_, &body_rota, &body_pos);

	/* �`�� */
	Mesh::Draw(body_world_, body_mesh_);
}

/*****************************************
 * �֐��� : TurretDraw�֐�               *
 * ��  �e : �^���b�g�̕`�揈�����s���܂� *
 * ��  �� : �Ȃ�                         *
 * �߂�l : �Ȃ�                         *
 *****************************************/
VOID CharacterBase::TurretDraw(VOID)
{
	/* ���W���{�f�B�Ɠ������� */
	turret_position_ = body_position_;

	/* ���W�Ɗp�x��ݒ� */
	D3DXMATRIX turret_pos, turret_rota;

	/* ��]�̐ݒ� */
	D3DXMatrixRotationY(&turret_rota, turret_direction_);

	/* �ړ��̐ݒ� */
	D3DXMatrixTranslation(&turret_pos, turret_position_.x, turret_position_.y, turret_position_.z);

	/* ���[���h���W�n���쐬 */
	D3DXMatrixIdentity(&turret_world_);

	/* �s����|�����킹�� */
	D3DXMatrixMultiply(&turret_world_, &turret_rota, &turret_pos);

	/* �`�� */
	Mesh::Draw(turret_world_, turret_mesh_);
}

VOID CharacterBase::PlayerHit(VOID)
{
	DSound::Play(WALL_SE);

	/* �����Ă�������̋t�Ɉړ� */
	body_position_.x += sin(body_direction_) * -(move_ * 5.0F);
	body_position_.z += cos(body_direction_) * -(move_ * 5.0F);

	/* �����x�������� */
	move_ /= 10.0F;

	/* �����x��0�ȉ��ɂȂ肻���Ȃ�Œ肷�� */
	if (move_ <= 0.0F) move_ = 0.0F;
}