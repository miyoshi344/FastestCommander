/***********
 * Include *
 ***********/
#include "direct3d.h"


/**********************
 * �ÓI�����o�ϐ��錾 *
 **********************/
LPDIRECT3D9            Direct3D::d3d_;         // Direct3D�C���^�[�t�F�C�X
LPDIRECT3DDEVICE9      Direct3D::d3d_device_;  // �f�o�C�X��IDirect3Device9�C���^�[�t�F�C�X�̃|�C���^
D3DPRESENT_PARAMETERS  Direct3D::present_;     // �v���[���e�[�V�����p�����[�^
D3DCAPS9               Direct3D::caps_;        // �n�[�h�E�F�A�\�͊i�[�\����


/******************
 * �R���X�g���N�^ *
 ******************/
Direct3D::Direct3D( VOID )
{
	/* �����o�ϐ������� */
	d3d_        = NULL;
	d3d_device_ = NULL;
	ZeroMemory( &caps_, sizeof(caps_) );
	ZeroMemory( &present_, sizeof(present_) );
}

/*****************************************************
 * �֐��� : Init�֐�                                 *
 * ��  �e : �E�B���h�E�̍쐬�Ɗe�탉�C�u�����̏����� *
 * ��  �� : CONST HWND hWnd�F�E�B���h�E�n���h��      *
 * �߂�l : �������ɐ��������TRUE��Ԃ�             *
 *****************************************************/
BOOL Direct3D::Init( CONST HWND hWnd )
{
    BOOL ret = TRUE;    // �߂�l�p

	/* ���Ƀf�o�C�X���擾�ς݂Ȃ珈���𔲂��� */
	if( d3d_device_ ) return ret;
    
    do
    {
        /* Direct3D�̏����� */
	    if( !(d3d_ = Direct3DCreate9( D3D_SDK_VERSION )) )
	    {
		    /* �G���[ */
            ErrorDialog( "Direct3D�C���^�[�t�F�C�X�̎擾�Ɏ��s" );
            ret = FALSE;
            break;
	    }

        /* �e��p�����[�^�̐ݒ� */
        SetPresentParameters(hWnd);
        SetMultiSamples();

        /* D3D�f�o�C�X�̍쐬 */
        ret = CreateD3DDevice( hWnd );
        
    }while( 0 );

    return ret;
}

/***************************
 * �֐��� : Destroy�֐�    *
 * ��  �e : Direct3D��j�� *
 * ��  �� : �Ȃ�           *
 * �߂�l : �Ȃ�           *
 ***************************/
VOID Direct3D::Destroy( VOID )
{
    SafeRelease( d3d_device_ );  // Direct3DDevice�C���^�t�F�[�X
    SafeRelease( d3d_ );         // Direct3D�C���^�t�F�[�X
}

/***********************************
 * �֐��� : CreateD3DDevice�֐�    *
 * ��  �e : Direct3D�f�o�C�X���쐬 * 
 * �� ���F�Ȃ�                     *
 * �߂�l�F�Ȃ�                    *
 ***********************************/
BOOL Direct3D::CreateD3DDevice( CONST HWND hWnd )
{
    BOOL ret = TRUE;    // �߂�l�p

    /* Direct3D�f�o�C�X�̍쐬 */
    if( FAILED(d3d_->CreateDevice(
            D3DADAPTER_DEFAULT,                   // �A�_�v�^�̑I��
            D3DDEVTYPE_HAL,                       // �n�[�h�E�F�A���[�h
            hWnd,                                 // �`�悷��E�B���h�E�̃n���h��
            D3DCREATE_HARDWARE_VERTEXPROCESSING,  // �n�[�h�ɂ�钸�_�������w��
            &present_,                            // �������p�����[�^�̃|�C���^
            &d3d_device_ )))                      // �擾�����C���^�t�F�[�X�̊i�[��|�C���^
	    {
            /* ���s������SOFTWARE�ō쐬�����݂� */
		    if( FAILED(d3d_->CreateDevice(
                D3DADAPTER_DEFAULT,                   // �A�_�v�^�̑I��
                D3DDEVTYPE_HAL,                       // �n�[�h�E�F�A���[�h
                hWnd,                                 // �`�悷��E�B���h�E�̃n���h��
                D3DCREATE_SOFTWARE_VERTEXPROCESSING,  // �\�t�g�ɂ�钸�_�������w��
                &present_,                            // �������p�����[�^�̃|�C���^
                &d3d_device_ )))                      // �擾�����C���^�t�F�[�X�̊i�[��|�C���^
		    {
			    /* �G���[�i�K�v�Ȃ�REF�ł̓���j */
                ErrorDialog( "D3D�f�o�C�X�̍쐬�Ɏ��s" );
                ret = FALSE;
		    }	
	    }   

    return ret;
}

/*************************************************
 * �֐��� : SetPresentParameters�֐�             *
 * ��  �e : �Œ�@�\�̃����_�����O�X�e�[�g��ݒ� *
 * ��  �� : �Ȃ�                                 *
 * �߂�l : �Ȃ�                                 *
 *************************************************/
VOID Direct3D::SetPresentParameters(CONST HWND hWnd)
{
    /* D3DPRESENT_PARAMETERS���ʍ��� */
	present_.SwapEffect              = D3DSWAPEFFECT_DISCARD;  // �X���b�v�G�t�F�N�g
	present_.EnableAutoDepthStencil	 = TRUE;                   // �[�x�F�X�e���V���o�b�t�@�̗L��
	present_.AutoDepthStencilFormat	 = D3DFMT_D24S8;           // �[�x�F�X�e���V���o�b�t�@�̃t�H�[�}�b�g�j
	present_.MultiSampleType         = D3DMULTISAMPLE_NONE;    // �A���`�G�C���A�X���x��
#ifndef FULLSCREEN_MODE
	/* �E�B���h�E���[�h�̐ݒ� */
	present_.Windowed				= TRUE;                         // �E�B���h�E���[�h�̎w��
	present_.BackBufferFormat		= D3DFMT_UNKNOWN;                   // �o�b�N�o�b�t�@�̃t�H�[�}�b�g
	present_.PresentationInterval	= D3DPRESENT_INTERVAL_DEFAULT;  // �e�A�����O�h�~
#else
	/* �t���X�N���[�����̐ݒ� */
	present_.Windowed					= FALSE;                        // �E�B���h�E���[�h�̎w��	
	present_.FullScreen_RefreshRateInHz	= D3DPRESENT_RATE_DEFAULT;      // ���t���b�V�����[�g
	present_.BackBufferWidth			= kWindowWidth;                // �o�b�N�o�b�t�@����
	present_.BackBufferHeight			= kWindowHeight;               // �E�B���h�E�̏c��
	present_.BackBufferFormat			= D3DFMT_X8R8G8B8;               // �o�b�N�o�b�t�@�t�H�[�}�b�g
	present_.BackBufferCount            = 1;                            // �o�b�N�o�b�t�@�̐�
	present_.hDeviceWindow              = hWnd;                         // �f�o�C�X�E�B���h�E
	present_.PresentationInterval       = D3DPRESENT_INTERVAL_DEFAULT;  // �e�A�����O�h�~
#endif
}

/***************************************
 * �֐��� : SetMultiSamples�֐�        *
 * ��  �e : �A���`�G�C���A�X���s���܂� *
 * ��  �� : �Ȃ��@                     *
 * �߂�l : �Ȃ�                       *
 ***************************************/
VOID Direct3D::SetMultiSamples( VOID )
{
    /* �f�B�X�v���C���[�h�̎擾 */
	D3DDISPLAYMODE display;
	if( FAILED( d3d_->GetAdapterDisplayMode( D3DADAPTER_DEFAULT, &display ) ))
	{
        /* �G���[ */
        ErrorDialog( "�f�B�X�v���C���[�h�̎擾�Ɏ��s" );
        return;
	}

    /* �}���`�T���v�����O�`�F�b�N */
	DWORD quality_buf = 0L;
	DWORD quality_z   = 0L;

    /* �A���`�G�C���A�X�̐ݒ� */
    for( INT i = D3DMULTISAMPLE_16_SAMPLES; i >= D3DMULTISAMPLE_NONE; i-- )
    {
        /* �����_�����O�^�[�Q�b�g�A���`�G�C���A�V���O���T�|�[�g����Ă��邩���ׂ� */
        if( SUCCEEDED( d3d_->CheckDeviceMultiSampleType(
            D3DADAPTER_DEFAULT,      // �₢���킹��f�B�X�v���C�A�_�v�^�[����������
            D3DDEVTYPE_HAL,          // �f�o�C�X�̎��
            display.Format,          // �T�[�t�F�X�̃t�H�[�}�b�g
            present_.Windowed,       // �t���E�B���h�E�}���`�T���v�����O�FTRUE, �܂��́A�t���X�N���[���A���`�G�C���A�V���O�FFALSE, ��₢���킹�� 
            (D3DMULTISAMPLE_TYPE)i,	 // �e�X�g����}���`�T���v�����O�e�N�j�b�N
            &quality_buf ) ) )
		{
			/* �[�x�X�e���V���ŃA���`�G�C���A�V���O���T�|�[�g����Ă��邩���ׂ� */
			if( SUCCEEDED( d3d_->CheckDeviceMultiSampleType(
                D3DADAPTER_DEFAULT,      // �₢���킹��f�B�X�v���C�A�_�v�^�[����������
                D3DDEVTYPE_HAL,          // �f�o�C�X�̎��
                display.Format,          // �T�[�t�F�X�̃t�H�[�}�b�g
                present_.Windowed,       // �t���E�B���h�E�}���`�T���v�����O�FTRUE, �܂��́A�t���X�N���[���A���`�G�C���A�V���O�FFALSE, ��₢���킹�� 
                (D3DMULTISAMPLE_TYPE)i,  // �e�X�g����}���`�T���v�����O�e�N�j�b�N
                &quality_z ) ) )
			{
				/* �T�|�[�g����Ă��郌�x����ݒ� */
				present_.MultiSampleType = (D3DMULTISAMPLE_TYPE)i;

				/* MultiSampleQuality�̓����_�����O�^�[�Q�b�g�Ɛ[�x�X�e���V���Œl�̏����������̗p����B*/
				if( quality_buf < quality_z ) present_.MultiSampleQuality = quality_buf - 1L;
				else present_.MultiSampleQuality = quality_z - 1L;
				break;
			}
		}
	}
}