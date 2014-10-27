/**************
 * ��b�N���X *
 **************/

/* ���dInclude�h�~ */
#ifndef RACE_OBJECTBASE_H_
#define RACE_OBJECTBASE_H_

/***********
 * Include *
 ***********/
#include "common.h"
#include "mesh.h"
#include "sprite.h"
#include "texture.h"
#include "xinput.h"
#include "directsound.h"
#include "collision.h"

/********
 * �萔 *
 ********/

/********************************************
 * ���ׂẴN���X�̊�b�N���X�ł�           *
 * ���̃N���X�͂�����p�����č쐬���܂�     *
 ********************************************/
class ObjectBase
{
protected:

	/**************
	 * �N���X�錾 *
	 **************/
	Collision   collision_;  // �����蔻��N���X
	Mesh        mesh_;       // ���b�V���N���X
	MeshTable*  my_mesh_;    // ���b�V�����

	/********
	 * �ϐ� *
	 ********/
	D3DXVECTOR3  origin_pos_;  // �X�V�O�̍��W
	D3DXVECTOR3  position_;    // ���W
	D3DXMATRIX   world_;       // ���[���h
	FLOAT radius_;

public:

	/* ���z�֐� */
	virtual BOOL Init(VOID)    = 0;  // ������
	virtual VOID UpDate(VOID)  = 0;  // �X�V����
	virtual VOID Draw(VOID)    = 0;  // �`�揈��
	virtual VOID Destroy(VOID) = 0;  // �j��

	/* ����Ԃ��֐� */
	D3DXVECTOR3 ReturnOriginPos(VOID) { return origin_pos_; }  // �X�V�O���W
	D3DXVECTOR3 ReturnPosition(VOID){ return position_; }      // �X�V����W
	D3DXMATRIX  ReturnMatrix(VOID)  { return world_;}          // ���[���h���W
	FLOAT       ReturnRadius(VOID)  { return radius_; }
	MeshTable*  ReturnMesh(VOID)    { return my_mesh_; }       // ���b�V�����

	/* �����E���֐� */
	VOID SetPosition(D3DXVECTOR3 pos) { position_ = pos; } 
	VOID SetMatrix(D3DXMATRIX matrix) { world_ = matrix; }
};

#endif
