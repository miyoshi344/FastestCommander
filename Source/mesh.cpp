/***********
 * Include *
 ***********/
#include "mesh.h"

/**********************
 * �ÓI�����o�ϐ����� *
 **********************/
std::map<std::string, MeshTable*> Mesh::list_;

/******************
 * �R���X�g���N�^ *
 ******************/
Mesh::Mesh(VOID)
{
}

/****************
 * �f�X�g���N�^ *
 ****************/
Mesh::~Mesh(VOID)
{
}

/*******************************************
 * �֐��� : Destroy�֐�                    *
 * ��  �e : ���b�V���Ǘ��N���X��j�����܂� *
 * ��  �� : �Ȃ�                           *
 * �߂�l : �Ȃ�                           *
 *******************************************/
VOID Mesh::Destroy(VOID)
{
	/* ���X�g�͋� */
	if (!list_.empty())
	{
		/* �e�N�X�`�������� */
		std::map<std::string, MeshTable*>::iterator it = list_.begin();
		std::map<std::string, MeshTable*>::iterator end = list_.end();

		/*** �e�N�X�`�����c���Ă������� ***/
		while (it != end)
		{
			/* ��������� */
			SafeDelete(it->second);
			++it;
		}
	}

	list_.clear();
}

/***********************************************************
 * �֐��� : Load�֐�                                       * 
 * ��  �e : ���b�V�������[�h���ă|�C���^��Ԃ��܂��B       *
 * ��  �� : CONST string MeshName�@                        *
 * �߂�l : ���b�V���C���^�[�t�F�C�X�̃|�C���^��Ԃ��܂��B *
 *          ���[�h�Ɏ��s�����ꍇ��NULL��Ԃ��܂��B         *
 ***********************************************************/
MeshTable* Mesh::Load(CONST std::string MeshName)
{
	MeshTable* p_mesh = NULL;

	/* ���b�V�������ɓǂݍ��܂�Ă��邩���� */
	std::map<std::string, MeshTable*>::iterator it;
	it = list_.find(MeshName);

	/* �����Ȃ�V�K�ɓǂݍ��� */
	if (it == list_.end())
	{
		do
		{
			/* ���b�V���e�[�u�����������m�� */
			p_mesh = new MeshTable;

			/* NULL�`�F�b�N */
			if (!p_mesh)
			{
				/* �G���[ */
				ErrorDialog("���b�V���e�[�u���������m�ۂɎ��s");
				break;
			}

			/* ���b�V����ǂݍ��� */
			LPD3DXBUFFER p_matbuff = NULL;

			if (FAILED(D3DXLoadMeshFromX(
				MeshName.c_str(),               // �t�@�C����
				D3DXMESH_MANAGED,               // ���b�V���쐬�I�v�V�����t���O
				D3DDEV,                         // Direct3DDevice
				NULL,                           // ���b�V���ڑ����i�[��|�C���^�̃|�C���^
				&p_matbuff,                     // �}�e���A�����i�[��|�C���^
				NULL,                           // �����O���[�v�o�b�t�@�ւ̃|�C���^
				(DWORD*)&p_mesh->num_materials,	// �}�e���A�����
				&p_mesh->mesh)))                // �쐬���ꂽ���b�V�����󂯎��|�C���^�̃|�C���^
			{
				/* �G���[ */
				CHAR name[MAX_PATH] = "";
				sprintf_s(name, "���b�V���̓ǂݍ��݂Ɏ��s�i%s�j", MeshName.c_str());
				ErrorDialog(name);
				SafeDelete(p_mesh);
				break;
			}

			/* �������̊m�� */
			/* �e�N�X�`���C���^�[�t�F�C�X */
			if ((p_mesh->texture = new LPDIRECT3DTEXTURE9[p_mesh->num_materials]) == NULL)
			{
				/* �G���[ */
				ErrorDialog("�e�N�X�`���C���^�[�t�F�C�X�̃������m�ۂɎ��s");
				break;
			}

			/* �}�e���A���C���^�[�t�F�C�X */
			if ((p_mesh->material = new D3DMATERIAL9[p_mesh->num_materials]) == NULL)
			{
				/* �G���[ */
				ErrorDialog("�}�e���A���C���^�[�t�F�C�X�̃������m�ۂɎ��s");
				break;
			}

			/* �}�e���A���o�b�t�@�̃|�C���^���擾���� */
			D3DXMATERIAL*	p_d3dmtrl;
			p_d3dmtrl = static_cast<D3DXMATERIAL*>(p_matbuff->GetBufferPointer());

			/* �}�e���A���ƃe�N�X�`���̐ݒ� */
			for (INT i = 0; i < static_cast<INT>(p_mesh->num_materials); i++)
			{
				/* �}�e���A�����R�s�[���� */
				p_mesh->material[i] = p_d3dmtrl[i].MatD3D;
				p_mesh->material[i].Ambient = p_mesh->material[i].Diffuse;

				/* �e�N�X�`����ǂݍ��� */
				p_mesh->texture[i] = NULL;

				/* �e�N�X�`�������݂��Ă��Ă�����ǂݍ��� */
				if (p_d3dmtrl[i].pTextureFilename)
				{
					if ((p_mesh->texture[i] = Texture::LoadTexture(p_d3dmtrl[i].pTextureFilename)) == NULL)
					{
						/* ���s */
						ErrorDialog("�e�N�X�`���̓ǂݍ��݂Ɏ��s(Mesh�N���X)");
						break;
					}
				}
			}

			/* �ǂݍ��ݏI������̂Ŕėp�o�b�t�@����� */
			SafeRelease(p_matbuff);

			/* ���X�g�ɒǉ� */
			list_.insert(std::pair<std::string, MeshTable*>(MeshName, p_mesh));

		} while (0);
	}
	else
	{
		/*** ���ɓǂݍ��܂�Ă���Ȃ�|�C���^��Ԃ� ***/
		p_mesh = it->second;
		++p_mesh->reference_count;
	}

	return p_mesh;
}

/****************************************
 * �֐��� : Draw�֐�                    *
 * ��  �e : ���b�V����`�悵�܂�        *
 * ��  �� : CONST MeshTable* pMeshTable *
 * �߂�l : �Ȃ�                        *
 ****************************************/
VOID Mesh::Draw(CONST D3DMATRIX world, CONST MeshTable* pMeshTable)
{
	/* �`�悷�镨���Z�b�g���� */
	D3DDEV->SetTransform(D3DTS_WORLD, &world);

	/* ���b�V���`�� */
	for (INT i = 0; i < pMeshTable->num_materials; i++)
	{
		D3DDEV->SetTexture(0, pMeshTable->texture[i]);  // �e�N�X�`���̐ݒ�
		D3DDEV->SetMaterial(&pMeshTable->material[i]);	// �}�e���A���̐ݒ�
		pMeshTable->mesh->DrawSubset(i);                  // ���b�V���̕`��
	}
}

/******************************************************************
 * �֐��� : Release�֐�                                           *
 * ��  �e : ���b�V����������܂��B�Q�Ɛ���0�ɂȂ�����j�����܂��B *
 * ��  �� : CONST MeshTable* pMeshTable                           *
 * �߂�l : �Ȃ�                                                  *
 ******************************************************************/
VOID Mesh::Release(CONST MeshTable* pMeshTable)
{
	if (!list_.empty())
	{
		/* �e�N�X�`�������� */
		std::map<std::string, MeshTable*>::iterator it = list_.begin();
		std::map<std::string, MeshTable*>::iterator end = list_.end();

		while (it != end)
		{
			if (it->second == pMeshTable)
			{
				/* �Q�Ɖ񐔂����炷 */
				--it->second->reference_count;

				/* �Q�Ƃ�0�ɂȂ�����폜 */
				if (it->second->reference_count <= 0)
				{
					SafeDelete(it->second);
					list_.erase(it);
				}
				break;
			}
			++it;
		}
	}
}