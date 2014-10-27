/***********
 * Include *
 ***********/
#include "collision.h"

/******************
 * �R���X�g���N�^ *
 ******************/
Collision::Collision()
{
}

/****************
 * �f�X�g���N�^ *
 ****************/
Collision::~Collision()
{
}

/*****************************************
 * �֐��� : Destory�֐�                  *
 * ��  �e : �擾�������b�V����j�����܂� *
 * ��  �� : �Ȃ�                         *
 * �߂�l : �Ȃ�                         *
 *****************************************/
VOID Collision::Destroy(VOID)
{
}

/***********************************************************
 * �֐��� : RayIntersect�֐�                               *
 * ��  �e : ���C�`�F�b�N�p�֐�                             *
 * ��  �� : pMesh         ���C�`�F�b�N���������b�V���\���� *
 *          MeshWorldMat  ���b�V���̃��[���h�s��           *
 *          RayStartPos   ���C�̊J�n���W                   *
 *          RayVec        ���C�̕���                       *
 *          pPosition     �q�b�g�������ʂ̍��W             *
 * �߂�l : �q�b�g������TRUE��Ԃ�                         *
 ***********************************************************/
BOOL Collision::RayIntersect(CONST MeshTable* pMesh, CONST D3DXMATRIX MeshWorldMat,
	CONST D3DXVECTOR3 RayStartPos, CONST D3DXVECTOR3 RayVec, D3DXVECTOR3* pPosition)
{
	LPD3DXMESH mesh_ = pMesh->mesh;

	/* ���b�V����񂪖�����ΏI�� */
	if (mesh_ == NULL) return FALSE;

	/* ���b�V���̃��[���h�s��̋t�s��𐶐� */
	D3DXMATRIX   mesh_mat_inv;
	D3DXMatrixInverse(&mesh_mat_inv, NULL, &MeshWorldMat);

	/* ���C�̊J�n�ʒu�ƕ����ɋt�s���K�p */
	D3DXVECTOR3 start = RayStartPos,
		        end   = RayVec + RayStartPos,
	         	direction;

	D3DXVec3TransformCoord(&start, &start, &mesh_mat_inv);
	D3DXVec3TransformCoord(&end, &end, &mesh_mat_inv);
	direction = end - start;

	/* ���C�`�F�b�N���s */
	BOOL  ret = FALSE;
	DWORD index;
	FLOAT u, v;
	D3DXIntersect(mesh_, &start, &direction, &ret, &index, &u, &v, NULL, NULL, NULL);
	if (ret)
	{
		/* ���_���W������ */
		D3DXVECTOR3 vertex[3];
		FindVertices(mesh_, index, vertex);

		/* �d�S���W�����_���W���Z�o */
		*pPosition = vertex[0] + u * (vertex[1] - vertex[0]) + v * (vertex[2] - vertex[0]);
		D3DXVec3TransformCoord(pPosition, pPosition, &MeshWorldMat);
		return TRUE;
	}
	else return FALSE;
}

/****************************************************
 * �֐��� : FindVertices                            *
 * ��  �e : ���_���W����������                      *
 * ��  �� : pMesh  ���C�`�F�b�N���������b�V���\���� *
 *          Index  ���b�V���̒��_�C���f�b�N�X       *
 *          pVertices  3���_�̔z��|�C���^          *
 * �߂�l : �Ȃ�                                    *
 ****************************************************/
void Collision::FindVertices(LPD3DXMESH pMesh, DWORD Index, D3DXVECTOR3* pVertices)
{
	/* ���b�V����񂪖�����ΏI�� */
	if (pMesh == NULL)
	{
		return;
	}

	/* �C���f�b�N�X�o�b�t�@���擾 */
	WORD* p_index_buffer = NULL;
	if (SUCCEEDED(pMesh->LockIndexBuffer(D3DLOCK_READONLY, (VOID**)&p_index_buffer)))
	{
		/* ���_���W��W�J */
		DWORD stride = pMesh->GetNumBytesPerVertex();
		BYTE* p_byte_vertices = NULL;
		FLOAT* p_float_vertices = NULL;
		LPDIRECT3DVERTEXBUFFER9 vertex_buffer = NULL;
		if (SUCCEEDED(pMesh->GetVertexBuffer(&vertex_buffer)))
		{
			if (SUCCEEDED(vertex_buffer->Lock(0, 0, (VOID**)&p_byte_vertices, 0)))
			{
				/* ��1���_ */
				p_float_vertices = (FLOAT*)&p_byte_vertices[stride * p_index_buffer[Index * 3]];
				pVertices[0].x = p_float_vertices[0];
				pVertices[0].y = p_float_vertices[1];
				pVertices[0].z = p_float_vertices[2];

				/* ��2���_ */
				p_float_vertices = (FLOAT*)&p_byte_vertices[stride * p_index_buffer[Index * 3 + 1]];
				pVertices[1].x = p_float_vertices[0];
				pVertices[1].y = p_float_vertices[1];
				pVertices[1].z = p_float_vertices[2];

				/* ��3���_ */
				p_float_vertices = (FLOAT*)&p_byte_vertices[stride * p_index_buffer[Index * 3 + 2]];
				pVertices[2].x = p_float_vertices[0];
				pVertices[2].y = p_float_vertices[1];
				pVertices[2].z = p_float_vertices[2];

				/* ���_���W����� */
				pMesh->UnlockIndexBuffer();
				pMesh->UnlockVertexBuffer();
			}
		}
	}
}

/*****************************************************************
 * �֐��� : SphereCreate�֐�                                     *
 * ��  �e : �����̃��b�V���p�̃o�E���f�B���O�X�t�B�A���쐬���܂� *
 * ��  �� : MeshTable* pMesh�@���肷�郁�b�V��                   *
 * �߂�l : Float      radius �X�t�B�A�̔��a                     *
 *****************************************************************/
FLOAT Collision::SphereCreate(CONST MeshTable* pMesh)
{
	LPD3DXMESH mesh = pMesh->mesh;

	/* ���b�V���̒��_�����b�N���Đ擪�|�C���^���擾 */
	VOID* vertex_buffer = NULL;
	mesh->LockVertexBuffer(D3DLOCK_READONLY, &vertex_buffer);

	/* ���b�V�����狫�E�����쐬���� */
	D3DXVECTOR3 center;
	FLOAT radius = 0.0F;  // ���a�ۑ��p

	D3DXComputeBoundingSphere(
		(D3DXVECTOR3*)vertex_buffer,   // ���_�̐擪�|�C���^
		mesh->GetNumVertices(),        // ���_��
		mesh->GetNumBytesPerVertex(),  // ���_�T�C�Y
		&center,                       // ���a
		&radius);

	/* ���b�N���� */
	mesh->UnlockVertexBuffer();

	/* �X�t�B�A�̔��a��Ԃ� */
	return radius;
}

/*****************************************************
 * �֐��� : SphereMesh�֐�                           *
 * ��  �e : �����蔻��p�ɍ쐬�����X�t�B�A��\������ *
 * ��  �� : MeshTable*  pMesh    �\�����郁�b�V��    *
 *          D3DXVECTOR3 position ���b�V���̍��W      *
 *          FLOAT       radius   �X�t�B�A�̔��a      *
 * �߂�l : �Ȃ�                                     *
 *****************************************************/
VOID Collision::SphereMesh(CONST MeshTable* pMesh, CONST D3DXVECTOR3 position, CONST FLOAT radius)
{
	D3DXMATRIX world;

	/* ���F�ł���悤�X�t�B�A���b�V�����쐬 */
	D3DXCreateSphere(
		D3DDEV,     // D3D�f�o�C�X
		radius,     // ���a
		32,         // �X���C�X��(���̕�����)
		32,         // �X�^�b�N��(�c�̕�����)
		&sphire_,   // ���惁�b�V���C���^�[�t�F�C�X�|�C���^
		NULL);      // �����o�b�t�@

	/* ���E����`�� */
	D3DXMatrixIdentity(&world);

	D3DXMatrixTranslation(&world, position.x, position.y, position.z);
	D3DDEV->SetTransform(D3DTS_WORLD, &world);

	D3DDEV->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);  // ���C���[�t���[����
	D3DDEV->SetTexture(0, NULL);                                // �e�N�X�`������
	sphire_->DrawSubset(0);                                     // �`��
	D3DDEV->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);      // �ʏ�`��
}

/***********************************************
 * �֐��� : SphereHitCheck�֐�                 *
 * ��  �e : �X�t�B�A�ł̓����蔻����m�F���܂� *
 * ��  �� : D3DXVECTOR3 pos1 ���肷����W1     *
 *          D3DXVECTOR3 pos2 ���肷����W2     *
 *          FLOAT radius     ���肷�锼�a1     *
 *          FLOAT radius2    ���肷�锼�a2     *
 * �߂�l : �d�Ȃ��Ă�����TRUE��Ԃ��܂�       *
 ***********************************************/
BOOL Collision::SphereHitCheck(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, FLOAT radius, FLOAT radius2)
{
	/* 2�̕��̂̒��S�̋��������߂� */
	D3DXVECTOR3 vecLength = pos2 - pos1;
	FLOAT length = D3DXVec3Length(&vecLength);

	/* �d�Ȃ��Ă�����TRUE��Ԃ� */
	if (length < radius + radius2)
	{
		return TRUE;
	}

	return FALSE;
}

/***************************************************************
 * �֐��� : BoxCreate�֐�                                      *
 * ��  �e : �����̃��b�V���̃o�E���f�B���O�{�b�N�X���쐬���܂� *
 * ��  �� : MeshTable*   pMesh ���肷�郁�b�V��                *
 * �߂�l : �Ȃ�                                               *
 ***************************************************************/
VOID Collision::BoxCreate(MeshTable* pMesh, D3DXVECTOR3* min, D3DXVECTOR3* max)
{
	LPD3DXMESH mesh = pMesh->mesh;

	/* ���b�V���̒��_�����b�N���Đ擪�|�C���^���擾 */
	VOID* vertex_buffer = NULL;
	mesh->LockVertexBuffer(D3DLOCK_READONLY, &vertex_buffer);

	/* ���b�V�����狫�E�����쐬���� */
	D3DXComputeBoundingBox(
		(D3DXVECTOR3*)vertex_buffer,   // ���_�̐擪�|�C���^
		mesh->GetNumVertices(),        // ���_��
		mesh->GetNumBytesPerVertex(),  // ���_�T�C�Y
		min,                           // �ŏ��l
		max);                          // �ő�l

	/* ���b�N���� */
	mesh->UnlockVertexBuffer();
}

/*********************************************************
 * �֐��� : BoxMesh�֐�                                  *
 * ��  �e : �����蔻��p�ɍ쐬�����{�b�N�X��\������     *
 * ��  �� : MeshTable*  pMesh    ���肷�郁�b�V��        *
 *          D3DXVECTOR3 position �\��������W            *
 *          D3DXVECTOR3 min      �{�b�N�X�̍ŏ��l        *
 *          D3DXVECTOR3 max      �{�b�N�X�̍ő�l        *
 * �߂�l : �Ȃ�                                         *
 *********************************************************/
VOID Collision::BoxMesh(CONST MeshTable* pMesh, D3DXVECTOR3 position, D3DXVECTOR3 size)
{
	D3DXMATRIX world;

	/* ���F�ł���悤�X�t�B�A���b�V�����쐬 */
	D3DXCreateBox(
		D3DDEV,     // D3D�f�o�C�X
		size.x,     // �{�b�N�X�̕�
		size.y,     // �{�b�N�X�̍���
		size.z,     // �{�b�N�X�̐[��
		&box_,      // ���惁�b�V���C���^�[�t�F�C�X�|�C���^
		NULL);      // �����o�b�t�@

	/* ���E����`�� */
	D3DXMatrixIdentity(&world);

	D3DXMatrixTranslation(&world, position.x, position.y, position.z);
	D3DDEV->SetTransform(D3DTS_WORLD, &world);

	D3DDEV->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);  // ���C���[�t���[����
	D3DDEV->SetTexture(0, NULL);                                // �e�N�X�`������
	box_->DrawSubset(0);                                        // �`��
	D3DDEV->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);      // �ʏ�`��
}

/**********************************************
* �֐��� : BoxHitCheck�֐�                    *
* ��  �e : �{�b�N�X�ł̓����蔻����m�F���܂� *
* ��  �� : D3DXVECTOR3 min_pos1 1�ڂ̍ŏ��l *
*          D3DXVECTOR3 max_pos1 1�ڂ̍ő�l *
*          D3DXVECTOR3 min_pos2 2�ڂ̍ŏ��l *
*          D3DXVECTOR3 max_pos2 2�ڂ̍ő�l *
* �߂�l : �d�Ȃ��Ă�����TRUE��Ԃ��܂�       *
***********************************************/
BOOL Collision::BoxHitCheck(D3DXVECTOR3 min_pos1, D3DXVECTOR3 max_pos1, D3DXVECTOR3 min_pos2, D3DXVECTOR3 max_pos2)
{
	if (min_pos1.z < max_pos2.z && max_pos1.z > min_pos2.z &&
		min_pos1.x < max_pos2.x && max_pos1.x > min_pos2.x &&
		min_pos1.y < max_pos2.y && max_pos2.y > min_pos2.y)
	{
		return TRUE;
	}

	return FALSE;
}

BOOL Collision::MultiHitCheck(D3DXVECTOR3 min_pos, D3DXVECTOR3 max_pos, D3DXVECTOR3 pos, FLOAT radius)
{
	return TRUE;
}