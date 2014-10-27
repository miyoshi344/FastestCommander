/***********
 * Include *
 ***********/
#include "polygon.h"

/******************
 * �R���X�g���N�^ *
 ******************/
PolygonPlate::PolygonPlate(VOID)
{
}

/****************
 * �f�X�g���N�^ *
 ****************/
PolygonPlate::~PolygonPlate(VOID)
{
}

/*********************************************
 * �֐��� : Init�֐�                         *
 * ��  �e : �������������s���܂�             *
 * ��  �� : �Ȃ�                             *
 * �߂�l : �������ɐ���������TRUE��Ԃ��܂� *
 *********************************************/
BOOL PolygonPlate::Init(VOID)
{
	BOOL ret = TRUE;

	do
	{
		/* ���_�z�� */
		COSTOMVERTEX vertex[] = {
			{ D3DXVECTOR3(0.0F, 0.0F, 0.0F), D3DCOLOR_RGBA(255,   0,   0, 255), D3DXVECTOR2(0.0F, 1.0F)},
			{ D3DXVECTOR3(0.0F, 1.0F, 0.0F), D3DCOLOR_RGBA(  0, 255,   0, 255), D3DXVECTOR2(0.0F, 0.0F)},
			{ D3DXVECTOR3(1.0F, 1.0F, 0.0F), D3DCOLOR_RGBA(  0,   0, 255, 255), D3DXVECTOR2(1.0F, 0.0F)},
			{ D3DXVECTOR3(1.0F, 0.0F, 0.0F), D3DCOLOR_RGBA(255, 255, 255, 255), D3DXVECTOR2(1.0F, 1.0F)},
		};

		/* �����_�[�^�[�Q�b�g�p�e�N�X�`���̍쐬	*/
		if (FAILED(D3DXCreateTexture(
			D3DDEV,                 // Direct3DDevice
			kWindowWidth,           // �e�N�X�`���̕�
			kWindowHeight,          // �e�N�X�`���̍���
			0,                      // 0�ŌŒ�
			D3DUSAGE_RENDERTARGET,  // �g�p�p�r�̐ݒ�)
			D3DFMT_A8R8G8B8,        // �����t�H�[�}�b�g�̐ݒ�
			D3DPOOL_DEFAULT,        // D3DPOOL_MANAGED�Œ�
			&p_texture_             // �i�[��|�C���^
			)))
		{
			/* �G���[ */
			ErrorDialog("�����_�[�^�[�Q�b�g�p�e�N�X�`���̍쐬�Ɏ��s");
			ret = FALSE;
			break;
		}

		/* �e�N�X�`�������擾 */
		D3DSURFACE_DESC desc;

		p_texture_->GetLevelDesc(0, &desc);
		p_texture_->GetSurfaceLevel(0, &p_texture_render_target_);

		/* �[�x�o�b�t�@�̐ݒ� */
		if (FAILED(D3DDEV->CreateDepthStencilSurface(
			desc.Width,                   // �e�N�X�`���̕�
			desc.Height,                  // �e�N�X�`���̍���
			D3DFMT_D16,                   // D3DFMT_D16�ŌŒ�
			D3DMULTISAMPLE_NONE,          // D3DMULTISAMPLE_NONE�ŌŒ�
			0,                            // 0�ŌŒ�
			TRUE,                         // TRUE�ŌŒ�
			&p_texture_stencil_surface_,  // �i�[��T�[�t�F�C�X�|�C���^
			NULL
			)))
		{
			/* �G���[ */
			ErrorDialog("�[�x�o�b�t�@�̍쐬�Ɏ��s");
			ret = FALSE;
			break;
		}

		/* ���_�X�g���[���̍쐬 */
		D3DVERTEXELEMENT9 elements[] =
		{
			{
				0,                      // �X�g���[���ԍ�
				0,                      // �I�t�Z�b�g�l
				D3DDECLTYPE_FLOAT3,     // �f�[�^�^
				D3DDECLMETHOD_DEFAULT,  // ���_�����^�C�v
				D3DDECLUSAGE_POSITION,  // �f�[�^�^�C�v
				0                       // 0
			},
			{
				0,                      // �X�g���[���ԍ�
				12,                     // �I�t�Z�b�g�l
				D3DDECLTYPE_FLOAT2,     // �f�[�^�^
				D3DDECLMETHOD_DEFAULT,  // ���_�����^�C�v
				D3DDECLUSAGE_COLOR,     // �f�[�^�^�C�v
				0,
			},
			{
				0,                      // �X�g���[���ԍ�
				16,                     // �I�t�Z�b�g�l
				D3DDECLTYPE_FLOAT2,     // �f�[�^�^
				D3DDECLMETHOD_DEFAULT,  // ���_�����^�C�v
				D3DDECLUSAGE_TEXCOORD,  // �f�[�^�^�C�v
				0,
			},

			D3DDECL_END()
		};

		/* ���_�f�N�����[�V�����̍쐬 */
		D3DDEV->CreateVertexDeclaration(elements, &declaration_);

		/* ���_�o�b�t�@�̍쐬 */
		if (FAILED(D3DDEV->CreateVertexBuffer(
			4 * sizeof(COSTOMVERTEX),  // ���_�̃o�b�t�@�T�C�Y
			D3DUSAGE_WRITEONLY,        // ���_�o�b�t�@�̎g�p���@
			0,                         // 0�ŌŒ�
			D3DPOOL_MANAGED,           // D3DPOOL_MANAGED�ŌŒ�
			&vertex_buffer_,           // ���_�o�b�t�@�̊i�[��|�C���^
			NULL
			)))
		{
			/* �G���[ */
			ErrorDialog("���_�o�b�t�@�̍쐬�Ɏ��s");
			ret = FALSE;
			break;
		}

		/* ���_�o�b�t�@�����b�N���ă������ւ̃|�C���^���擾���� */
		COSTOMVERTEX* p_vertex;
		vertex_buffer_->Lock(0, 0, (VOID**)&p_vertex, 0);

		/* ���e���R�s�[���� */
		memcpy(p_vertex, vertex, sizeof(vertex));

		/* ���b�N���� */
		vertex_buffer_->Unlock();

		/* �C���f�b�N�X��z���` */
		WORD indeices[] = { 0, 1, 2, 3 };

		/* ���_�C���f�b�N�X�o�b�t�@�̍쐬 */
		if (FAILED(D3DDEV->CreateIndexBuffer(
			4 * sizeof(WORD),    // �C���f�b�N�X�̐����`
			D3DUSAGE_WRITEONLY,  // D3DUSAGE_WRITEONLY�ŌŒ�
			D3DFMT_INDEX16,      // D3DFMT_INDEX16�ŌŒ�
			D3DPOOL_MANAGED,     // D3DPOOL_MANAGED�ŌŒ�
			&index_buffer_,      // �C���f�b�N�X�o�b�t�@�̊i�[����w��
			NULL                 // NULL�ŌŒ�
			)))
		{
			/* �G���[ */
			ErrorDialog("���_�C���f�b�N�X�̍쐬�Ɏ��s");
			ret = FALSE;
			break;
		}

		/* ���_�o�b�t�@�����b�N���ă������ւ̃|�C���^���擾���� */
		COSTOMVERTEX* p_index;
		index_buffer_->Lock(0, 0, (VOID**)&p_index, 0);

		/* ���e���R�s�[���� */
		memcpy(p_index, indeices, sizeof(indeices));

		/* ���b�N���� */
		index_buffer_->Unlock();

	} while (0);

	return ret;
}

/*********************************************
 * �֐��� : TextureDraw�֐�                  *
 * ��  �e : ������|���S���ɕ`����s���܂� *
 * ��  �� : �Ȃ�                             *
 * �߂�l : �Ȃ�                             *
 *********************************************/
VOID PolygonPlate::TextureDraw(VOID)
{
	/* �o�b�N�o�b�t�@���擾 */
	D3DDEV->GetRenderTarget(0, &p_before_render_target);
	D3DDEV->GetDepthStencilSurface(&p_before_stencil_surface);

	D3DDEV->SetRenderTarget(0, p_texture_render_target_);
	D3DDEV->SetDepthStencilSurface(p_texture_stencil_surface_);

	D3DDEV->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(255, 255, 255), 1.0F, 0);
	D3DDEV->SetTexture(0, NULL);
}

/*********************************************
 * �֐��� : ReturnDraw�֐�                   *
 * ��  �e : �`�����o�b�N�o�b�t�@�ɖ߂��܂� *
 * ��  �� : �Ȃ�                             *
 * �߂�l : �Ȃ�                             *
 *********************************************/
VOID PolygonPlate::ReturnDraw(VOID)
{
	D3DDEV->SetRenderTarget(0, p_before_render_target);
	D3DDEV->SetDepthStencilSurface(p_before_stencil_surface);
}

/*****************************************************
 * �֐��� : Draw�֐�                                 *
 * ��  �e : �e�N�X�`���ݒ肵���|���S����`�悵�܂� *
 * ��  �� : �Ȃ�                                     *
 * �߂�l : �Ȃ�                                     *
 *****************************************************/
VOID PolygonPlate::Draw(VOID)
{
	/* �����_�����O�^�[�Q�b�g������ */
	D3DDEV->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0F, 0);

	/* ���[���h�s����쐬 */
	D3DXMATRIX world, work;
	D3DXMatrixIdentity(&world);

	/* �|���S���̃��[���h�s����쐬���Đݒ� */
	D3DXMatrixIdentity(&world);
	D3DXMatrixIdentity(&work);
	D3DXMatrixTranslation(&work, -1.0F, -0.5F, 1.0F);
	D3DXMatrixMultiply(&world, &world, &work);

	D3DDEV->SetTransform(D3DTS_WORLD, &world);

	/* �|���S���`�� */
	D3DDEV->SetStreamSource(0, vertex_buffer_, 0, sizeof(COSTOMVERTEX));  // ���_�o�b�t�@�̐ݒ�(�f�o�C�X�ɃZ�b�g)
	D3DDEV->SetVertexDeclaration(declaration_);				              // ���_�o�b�t�@�̃t�H�[�}�b�g�̐ݒ�
	D3DDEV->SetIndices(index_buffer_);
	D3DDEV->SetTexture(0, p_texture_);
	D3DDEV->DrawIndexedPrimitive(D3DPT_TRIANGLEFAN, 0, 0, 3, 0, 2);
}

/*****************************************
 * �֐��� : Destroy�֐�                  *
 * ��  �e : �ݒ肵���o�b�t�@��j�����܂� *
 * ��  �� : �Ȃ�                         *
 * �߂�l : �Ȃ�                         *
 *****************************************/
VOID PolygonPlate::Destroy(VOID)
{
	declaration_->Release();
	index_buffer_->Release();
	vertex_buffer_->Release();

	p_texture_->Release();
	p_texture_render_target_->Release();
	p_texture_stencil_surface_->Release();

	p_before_render_target->Release();
	p_before_stencil_surface->Release();
}