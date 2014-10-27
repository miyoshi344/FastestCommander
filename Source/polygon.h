/******************
 * �|���S���N���X *
 ******************/

/***********
 * Include *
 ***********/
#include "common.h"
#include "direct3d.h"

/* ���dInclude�h�~ */
#ifndef RACE_POLYGON_H_
#define RACE_POLYGON_H_

/**********
 * �\���� *
 **********/
struct COSTOMVERTEX
{
	D3DXVECTOR3 pos_;
	D3DCOLOR    color_;
	D3DXVECTOR2 uv;
};

/**********************************
 * �|���S�����쐬����N���X�ł� *
 **********************************/
class PolygonPlate
{
private:
	/* �ϐ� */
	LPDIRECT3DVERTEXBUFFER9		 vertex_buffer_; // ���_�o�b�t�@
	LPDIRECT3DINDEXBUFFER9       index_buffer_;  // �C���f�b�N�X�o�b�t�@
	LPDIRECT3DVERTEXDECLARATION9 declaration_;	 // ���_�f�N�����[�V����

	LPDIRECT3DTEXTURE9 p_texture_;                  // �e�N�X�`��
	LPDIRECT3DSURFACE9 p_texture_render_target_;    // �����_�[�^�[�Q�b�g
	LPDIRECT3DSURFACE9 p_texture_stencil_surface_;  // �X�e���V���T�[�t�F�C�X

	/* �Q�[������ */
	LPDIRECT3DSURFACE9	p_before_render_target,
		                p_before_stencil_surface;
public:
	PolygonPlate(VOID);    // �R���X�g���N�^
	~PolygonPlate(VOID);   // �f�X�g���N�^

	BOOL Init(VOID);  // ����������
	VOID TextureDraw(VOID);
	VOID ReturnDraw(VOID);
	VOID Draw(VOID);
	VOID Destroy(VOID);
};

#endif