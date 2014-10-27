/***********
 * Include *
 ***********/
#include "fieldobject.h"

/*******************************************
 * �֐��� : Init�֐�                       *
 * ��  �e : ���b�V���̏��������s���܂�     *
 * ��  �� : �Ȃ�                           *
 * �߂�l : �擾�ɐ���������TRUE��Ԃ��܂� *
 *******************************************/
BOOL FieldObject::Init(VOID)
{
	BOOL ret = TRUE;

	do
	{
		/* �u���b�N�̏����� */
		if (!ObjectInit())
		{
			/* �G���[ */
			ErrorDialog("�u���b�N�̏������Ɏ��s���܂���");
			ret = FALSE;
			break;
		}

	} while (0);

	return ret;
}

/*******************************
 * �֐��� : UpDadate�֐�       *
 * ��  �e : �X�V�������s���܂� *
 * ��  �� : �Ȃ�               *
 * �߂�l : �Ȃ�               *
 *******************************/
VOID FieldObject::UpDate(VOID)
{
}

/*********************************************
 * �֐��� : Draw�֐�                         *
 * ��  �e : �`�揈�����s���܂�               *
 * ��  �� : D3DXMATRIX matrix ���[���h���W�n *
 * �߂�l : �Ȃ�                             *
 *********************************************/
VOID FieldObject::Draw(VOID)
{
	/* ���b�V���̕`�� */
	Mesh::Draw(world_, my_mesh_);
	//collision_.Sphere(my_mesh_, position_);
}

/*******************************
 * �֐��� : Destroy�֐�        *
 * ��  �e : �j���������s���܂� *
 * ��  �� : �Ȃ�               *
 * �߂�l : �Ȃ�               *
 *******************************/
VOID FieldObject::Destroy(VOID)
{
	/* ���b�V���̔j�� */
	Mesh:: Release(my_mesh_);
}

/*******************************************
 * �֐��� : ObjectInit�֐�                 *
 * ��  �e : �u���b�N�̃��b�V�����擾���܂� *
 * ��  �� : �Ȃ�                           *
 * �߂�l : �Ȃ�                           *
 *******************************************/
BOOL FieldObject::ObjectInit(VOID)
{
	BOOL ret = TRUE;

	/* �t�H���_�K�w���ړ����� */
	_chdir("Resource_fast/Mesh/Block");

	do
	{
		/* ���b�V���̓ǂݍ��� */
		if ((my_mesh_ = Mesh::Load(mesh_name_)) == NULL)
		{
			/* �G���[ */
			ErrorDialog("�u���b�N�̃��b�V�����̎擾�Ɏ��s");
			ret = FALSE;
			break;
		}

	} while (0);

	/* �t�H���_�K�w��߂� */
	_chdir("../../../");

	return ret;
}