/******************
 * ���b�V���N���X *
 ******************/

/* ���dInclude�h�~ */
#ifndef RACE_MESH_H_
#define RACE_MESH_H_

/************
 *  Include *
 ************/
#include "common.h"
#include "direct3d.h"
#include "texture.h"

/**********
 * �\���� *
 **********/
struct MeshTable
{
	LPDIRECT3DTEXTURE9* texture;            // �e�N�X�`���C���^�[�t�F�C�X
	LPD3DXMESH          mesh;               // ���b�V���C���^�[�t�F�C�X
	D3DMATERIAL9*       material;           // �}�e���A���\����
	INT                 num_materials;      // �}�e���A���� 
	INT                 reference_count;    // �Q�Ɖ�

	/* �R���X�g���N�^ */
	MeshTable(VOID)
	{
		texture = NULL;
		mesh = NULL;
		material = NULL;
		num_materials = 0;
		reference_count = 0;
	}

	/* �f�X�g���N�^ */
	~MeshTable()
	{
		/*** �e�N�X�`����� ***/
		for (INT i = 0; i < num_materials; i++)
		{
			if (texture[i]) Texture::Release(texture[i]);
		}

		/* ��������� */
		SafeDeleteArray(texture);   // �e�N�X�`���C���^�[�t�F�C�X
		SafeDeleteArray(material);  // �}�e���A��
		SafeRelease(mesh);			// ���b�V��
	}
};

/**********
 * �N���X *
 **********/
class Mesh
{
private:
	static std::map<std::string, MeshTable*> list_; // ���b�V�����X�g

public:
	Mesh(VOID);		            // �R���X�g���N�^
	~Mesh(VOID);                // �f�X�g���N�^
	static VOID Destroy(VOID);  // �e�N�X�`���Ǘ��N���X��j�����܂�

	static MeshTable* Load(CONST std::string TextureName);             // ���b�V���̓ǂݍ���
	static VOID Draw(CONST D3DMATRIX world, CONST MeshTable* Texture); // �`��
	static VOID Release(CONST MeshTable* Texture);                     // ���
};

#endif