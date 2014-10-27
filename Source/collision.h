/********************
 * �����蔻��N���X *
 ********************/

/* ���dInclude�h�~ */
#ifndef RACE_COLLISION_H_
#define RACE_COLLISION_H_

/***********
 * Include *
 ***********/
#include "common.h"
#include "mesh.h"

/**************************
 * �����蔻����s���N���X *
 **************************/
class Collision
{
private:

	/**************
	 * �N���X��` *
	 **************/
	MeshTable*  my_mesh_;  // ���b�V�����

	/********
	 * �ϐ� *
	 ********/
	LPD3DXMESH  sphire_;    // �X�t�B�A�p���b�V��
	LPD3DXMESH  box_;       // �{�b�N�X�p���b�V��

	D3DXVECTOR3 position_;  // ���W�ۑ��p

public:
	Collision();   // �R���X�g���N�^
	~Collision();  // �f�X�g���N�^

	VOID Init(VOID);
	VOID Destroy(VOID);  // �j������

	/******************
	 * ���C����p�֐� *
	 ******************/
	BOOL RayIntersect(CONST MeshTable* mesh_, CONST D3DXMATRIX meshWorldMat, CONST D3DXVECTOR3 RayStartPos, CONST D3DXVECTOR3 RayVec, D3DXVECTOR3* position_);
	VOID FindVertices(LPD3DXMESH mesh_, DWORD Index, D3DXVECTOR3* vertices_);

	/**********************
	 * �X�t�B�A����p�֐� *
	 **********************/
	FLOAT SphereCreate(CONST MeshTable* mesh);                                               // �X�t�B�A�쐬�֐�
	VOID SphereMesh(CONST MeshTable* pMesh, CONST D3DXVECTOR3 position, CONST FLOAT radius); // �X�t�B�A�\���֐�
	BOOL SphereHitCheck(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, FLOAT radius1, FLOAT radius2);   // �X�t�B�A����֐�

	/**********************
	 * �{�b�N�X����p�֐� *
	 **********************/
	VOID BoxCreate(MeshTable* mesh, D3DXVECTOR3* min, D3DXVECTOR3* max);             // �{�b�N�X�쐬�֐�    
	VOID BoxMesh(CONST MeshTable* pMesh, CONST D3DXVECTOR3 position, D3DXVECTOR3 size);                       // �{�b�N�X�\���֐�
	BOOL BoxHitCheck(D3DXVECTOR3 min_pos1, D3DXVECTOR3 max_pos1, D3DXVECTOR3 min_pos2, D3DXVECTOR3 max_pos2); // �{�b�N�X����֐�

	/**************************
	 * ���Ƌ��̓����蔻��m�F *
	 **************************/
	BOOL MultiHitCheck(D3DXVECTOR3 min_pos, D3DXVECTOR3 max_pos, D3DXVECTOR3 pos, FLOAT radius);

	/* �v���C���[���W�󂯎��֐� */
	VOID SetPlayerPosition(D3DXVECTOR3 pos) { position_ = pos; }
};
#endif