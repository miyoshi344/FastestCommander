/***********
 * Include *
 ***********/
#include "bullet.h"

/******************
 * �R���X�g���N�^ *
 ******************/
Bullet::Bullet(VOID)
{
	speed_ = kBurretSpeed;  // �e�̃X�s�[�h������
}

/****************
 * �f�X�g���N�^ *
 ****************/
Bullet::~Bullet(VOID)
{
	Mesh::Release(my_mesh_);  // ���b�V���j��
}

/*************************************
 * �֐��� : Init�֐�                 *
 * ��  �e : �������������s���܂�     *
 * ��  �� : �Ȃ�                     *
 * �߂�l : ����������TRUE��Ԃ��܂� *
 *************************************/
BOOL Bullet::Init(VOID)
{
	BOOL ret = TRUE;  // �߂�l�p

	do
	{
		/* �e�̃��b�V�����擾 */
		if (!BulletMeshInit())
		{
			ErrorDialog("�e�̏������Ɏ��s");
			ret = FALSE;
			break;
		}

	} while (0);

	return ret;
}

/***********************************
 * �֐��� : ReSet�֐�              *
 * ��  �e : ���l�̏��������s���܂� *
 * ��  �� : �Ȃ�                   *
 * �߂�l : �Ȃ�                   *
 ***********************************/
VOID Bullet::ReSet(VOID)
{
	position_ = D3DXVECTOR3(0.0F, 0.0F, 0.0F);
}

/*******************************
 * �֐��� : UpDate�֐�         *
 * ��  �e : �X�V�������s���܂� *
 * ��  �� : �Ȃ�               *
 * �߂�l : �Ȃ�               *
 *******************************/
VOID Bullet::UpDate(VOID)
{
	/* �e�̈ړ����� */
	BulletMove();
}

/*******************************
 * �֐��� : Draw�֐�           *
 * ��  �e : �`�揈�����s���܂� *
 * ��  �� : �Ȃ�               *
 * �߂�l : �Ȃ�               *
 *******************************/
VOID Bullet::Draw(VOID)
{
	/* �e�̕`�揈�� */
	BulletDraw();
}

/*******************************
 * �֐��� : Destroy�֐�        *
 * ��  �e : �j���������s���܂� *
 * ��  �� : �Ȃ�               *
 * �߂�l : �Ȃ�               *
 *******************************/
VOID Bullet::Destroy(VOID)
{
	/* ���b�V���̉�� */
	Mesh::Release(my_mesh_);
}

/***************************************************
 * �֐��� : BulletSet�֐�                          *
 * ��  �e : �e�̐ݒ���s���܂�                     *
 * ��  �� : D3DXVECTOR3 pos       �e�𔭎˂�����W *
 *          FLOAT       direction ����             *
 *          FLOAT       move      �����x           *
 ***************************************************/
VOID Bullet::BulletSet(D3DXVECTOR3 pos, FLOAT direction, FLOAT move)
{
	/* �e�̍��W��ݒ肷�� */
	position_    = pos;
	position_.x += sin(direction) * 1.0F;
	position_.z += cos(direction) * 1.0F;
	position_.y += kBurretHeight;

	/* ������ݒ肷�� */
	jump_ = kBurretJump;

	/* �����Ɖ����x��ݒ肷�� */
	direction_ = direction;
	move_ = move;
}

/*******************************************
 * �֐��� : BulletMeshInit�֐�             *
 * ��  �e : �e�̃��b�V�������擾���܂�   *
 * ��  �� : �Ȃ�                           *
 * �߂�l : �擾�ɐ���������TRUE��Ԃ��܂� *
 *******************************************/
BOOL Bullet::BulletMeshInit(VOID)
{
	BOOL ret = TRUE;

	/* �t�H���_�K�w���ړ����� */
	_chdir("Resource_fast/Mesh/Bullet");

	/* �e�򃁃b�V���̎擾 */
	if ((my_mesh_ = Mesh::Load(bullet_name_)) == NULL)
	{
		/* �v���C���[�p�e�����b�V���̏��擾�Ɏ��s */
		ErrorDialog("�e�򃁃b�V���̏��擾�Ɏ��s���܂���");
		ret = FALSE;
	}

	radius_ = collision_.SphereCreate(my_mesh_);

	/* �t�H���_�K�w��߂� */
	_chdir("../../../");

	return ret;
}

/***********************************
 * �֐��� : BulletMove�֐�         *
 * ��  �e : �e�̈ړ��������s���܂� *
 * ��  �� : �Ȃ�                   *
 * �߂�l : �Ȃ�                   *
 ***********************************/
VOID Bullet::BulletMove(VOID)
{
	origin_pos_ = position_;

	/* �����Ă�������Ɉړ� */
	position_.x += sin(direction_) * (move_ + speed_);
	position_.z += cos(direction_) * (move_ + speed_);

	/* �����𒲐����� */
	position_.y += jump_;
	jump_ -= kBurretGravity;
}

/***********************************
 * �֐��� : BulletDraw�֐�         *
 * ��  �e : �e�̕`�揈�����s���܂� *
 * ��  �� : �Ȃ�                   *
 * �߂�l : �Ȃ�                   *
 ***********************************/
VOID Bullet::BulletDraw(VOID)
{
	/* ���W�Ɗp�x��ݒ� */
	D3DXMATRIX pos_, rota_;

	/* ���[���h���W�n���쐬 */
	D3DXMatrixIdentity(&world_);
	D3DXMatrixRotationY(&rota_, direction_);
	D3DXMatrixTranslation(&pos_, position_.x, position_.y, position_.z);

	/* �s����|�����킹�� */
	D3DXMatrixMultiply(&world_, &rota_, &pos_);

	/* �`�� */
	Mesh::Draw(world_, my_mesh_);


}