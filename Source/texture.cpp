/*****************
 * Texture�N���X *
 *****************/
#include "texture.h"

/**************
 * static�ϐ� *
 **************/
LPTEXTURETABLE	Texture::m_pTextureList = NULL;	// �e�N�X�`���Ǘ��o�b�t�@
LPTEXTURETABLE	Texture::m_pStockList   = NULL;	// ���g�p�̊Ǘ����X�g
LPTEXTURETABLE	Texture::m_pUseList     = NULL;	// �g�p���̊Ǘ����X�g
INT				Texture::m_MaxUseNum    = NULL;	// �g�p�e�N�X�`����

/******************
 * �R���X�g���N�^ *
 ******************/
BOOL Texture::Init(VOID)
{
	BOOL ret = TRUE;

	do
	{
		/* �ő�ێ������i�[ */
		m_MaxUseNum = TEXTURE_NUM;

		/* �n���ꂽ�g�p�������̃��������m�� */
		m_pTextureList = new TEXTURETABLE[m_MaxUseNum];

		/* �m�ۂł��Ȃ���΃G���[�Ƃ��ď������� */
		if (!m_pTextureList)
		{
			/* �G���[ */
			ret = FALSE;
			break;
		}

		/* ���g�p�e�N�X�`���Ǘ����X�g�쐬 */
		m_pStockList = m_pTextureList;

		/* ���X�g�\���쐬 */
		for (INT i = 0; i < m_MaxUseNum - 1; i++) m_pStockList[i].m_pNext = &m_pStockList[i + 1];
	} while (0);

	return ret;
}

/****************
 * �f�X�g���N�^ *
 ****************/
VOID Texture::Destroy(VOID)
{
	/* �g�p���Ă���e�N�X�`���Ǘ����X�g������ */
	m_pUseList = NULL;

	/* �e�N�X�`���Ǘ��e�[�u���o�b�t�@��� */
	SafeDeleteArray(m_pTextureList);

	/* �e�N�X�`���g�p������0�ɂ��Ă��� */
	m_MaxUseNum = 0;
}

/**********************************************************
 * �֐��� : LoadTexture�֐�                               *
 * ��  �e : �e�N�X�`���̓ǂݍ���                          *
 * ��  �� : LPCSTR�F�e�N�X�`����                          *
 * �߂�l�FLPDIRECT3DTEXTURE9�F�C���^�[�t�F�C�X�̃|�C���^ *
 **********************************************************/
LPDIRECT3DTEXTURE9 Texture::LoadTexture(LPCSTR TextureName)
{
	LPDIRECT3DTEXTURE9	p_Texture = NULL;			// �߂�l�Ƃ��ĕԂ��e�N�X�`���|�C���^
	LPTEXTURETABLE		p_use_list = m_pUseList;	// ��Ɨp
	CHAR				str[256] = { 0 };			// �G���[�p

	/* �e�N�X�`�������ɓǂݍ��܂�Ă��邩�g�p�����X�g���猟������ */
	while (p_use_list != NULL)
	{
		/* �t�@�C��������v�����瓯�����̂Ɣ��f */
		if (!strcmp(p_use_list->m_TextureName, TextureName))
		{
			/* ���X�g���ɑ��݂���Ȃ�΂��̃|�C���^��Ԃ� */
			p_Texture = p_use_list->m_pTexture;

			/* ���݂̓ǂݍ��ݐ��𑝂₷ */
			p_use_list->m_RentalNum++;

			break;
		}

		/* ���� */
		p_use_list = p_use_list->m_pNext;
	}

	/* �e�N�X�`�����ǂݍ��܂�Ă��Ȃ���ΐV�K�ɓǂݍ��� */
	if (p_Texture == NULL)
	{
		/* ���g�p�e�N�X�`�����X�g�ɋ󂫂�����Ȃ�Γǂݍ��� */
		if (m_pStockList != NULL)
		{
			/* ���g�p���X�g����1���o�� */
			LPTEXTURETABLE	ptable = m_pStockList;

			/* �e�N�X�`����ǂݍ��� */
			D3DXCreateTextureFromFileEx(D3DDEV,  // �e�N�X�`���Ɋ֘A�t����f�o�C�X
										TextureName,			// �e�N�X�`���t�@�C��
										D3DX_DEFAULT,			// �e�N�X�`���̕�
										D3DX_DEFAULT,			// �e�N�X�`���̍���
										0,						// �~�b�v���x����
										0,						// 0���w��
										D3DFMT_UNKNOWN,			// �e�N�X�`���t�H�[�}�b�g
										D3DPOOL_MANAGED,		// �������Ǘ�����
										D3DX_DEFAULT,			// �t�B���^�����O���@
										D3DX_DEFAULT,
										TEXTURE_COLORKEY,		// �J���[�L�[(RGB)
										&ptable->m_ImgInfo,		// �e�N�X�`�����i�[
										NULL,					// NULL���w��
										&ptable->m_pTexture);	// �e�N�X�`���C���^�t�F�[�X�|�C���^

			/* �ǂݍ��݂ɐ���������g�p���X�g�ɉ����A���e��ݒ肷�� */
			if (ptable->m_pTexture != NULL)
			{
				/* ���g�p���X�g�̐擪���獡�g�p�����e�[�u�����O��**/
				m_pStockList = m_pStockList->m_pNext;

				/* �g�p���X�g�̐擪�ɉ����� */
				ptable->m_pNext = m_pUseList;
				m_pUseList = ptable;

				/* �t�@�C������ۑ����� */
				strcpy(ptable->m_TextureName, TextureName);

				/* �g�p����1�ɂ��� */
				ptable->m_RentalNum = 1;

				/* �߂�l�ɐݒ� */
				p_Texture = ptable->m_pTexture;
			}
			else
			{
				/* �G���[ */
				sprintf(str, "�e�N�X�`���̓ǂݍ��݂Ɏ��s(%s)", TextureName);
				ErrorDialog(str);
			}
		}
	}

	return p_Texture;
}

/*************************************************************************
 * �֐��� : ReleaseTexture�֐�                                           *
 * ��  �e : �e�N�X�`���̔j��                                             *
 * ��  �� : LPDIRECT3DTEXTURE9 CONST�F�������e�N�X�`���C���^�[�t�F�C�X *
 * �߂�l : �Ȃ�                                                         *
 *************************************************************************/
VOID Texture::Release(LPDIRECT3DTEXTURE9 CONST pTexture)
{
	/***** �e�N�X�`���e�[�u������T�� *****/
	LPTEXTURETABLE p_work = m_pUseList;	// ��Ɨp
	LPTEXTURETABLE p_prev = NULL;		// 1�O

	// �e�N�X�`���C���^�[�t�F�C�X�̉�� //
	/* �o�^����Ă���e�N�X�`���Ǘ����X�g����T�� */
	while (p_work != NULL)
	{
		/* ���������甲���� */
		if (p_work->m_pTexture == pTexture) break;

		/* ���� */
		p_prev = p_work;
		p_work = p_work->m_pNext;
	}

	do
	{
		/* ������Ȃ�������߂� */
		if (p_work == NULL) break;


		/* �g�p����1���炷 */
		p_work->m_RentalNum--;

		/* �g�p����0�ȉ��Ȃ�Ή�� */
		if (p_work->m_RentalNum <= 0)
		{
			/* �g�p���X�g����O�� */
			if (p_work == m_pUseList)	m_pUseList = p_work->m_pNext;		// �擪�̏ꍇ
			else						p_prev->m_pNext = p_work->m_pNext;  // ����ȊO

			SafeRelease(p_work->m_pTexture);  // �e�N�X�`���̉��
			p_work->m_TextureName[0] = '\0';  // ���O������
			p_work->m_RentalNum      = 0;	  // �ݏo����0��

			/* ���g�p���X�g�̐擪�ɖ߂� */
			p_work->m_pNext = m_pStockList;
			m_pStockList = p_work;
		}

	} while (0);
}