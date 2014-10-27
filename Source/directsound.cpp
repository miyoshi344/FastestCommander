/***********
 * Include *
 ***********/
#include "directsound.h"

/**********************
 * �ÓI�����o�ϐ��錾 *
 **********************/
LPDIRECTSOUND8         DSound::dsound_                     = NULL;       // �_�C���N�g�T�E���h�o�b�t�@
LPDIRECTSOUNDBUFFER8   DSound::secondary_buffer_[MaxSound] = { NULL };   // �Z�J���_���o�b�t�@
INT DSound::count_ = 0;   // �Đ��ʒu

/********************************************
 * �֐��� : Init�֐�                        *
 * ��  �e : DirectSound�̍쐬�Ə��������s�� *
 * ��  �� : �E�B���h�E�n���h��              *
 * �߂�l : �������ɐ��������TRUE��Ԃ�    *
 ********************************************/
BOOL DSound::Init(CONST HWND hWnd)
{
	BOOL ret = TRUE;   // �߂�l�p

	do{
		/* DirectSound�̍쐬�Ɛݒ� */
		/* �쐬 */
		if (FAILED(DirectSoundCreate8(NULL,       // �T�E���h�f�o�C�X��\��GUID(�f�t�H���g�f�o�C�X��NULL)
									  &dsound_,   // �C���^�[�t�F�C�X�i�[��|�C���^
									  NULL)))     // ��� NULL
		{
			/* �G���[ */
			ErrorDialog("DirectSound�̍쐬�Ɏ��s���܂���");
			ret = FALSE;
			break;
		}

		/* �������x���̐ݒ� */               
		// ( �E�B���h�E�n���h��, �������x�� )
		if (FAILED(dsound_->SetCooperativeLevel(hWnd, DSSCL_PRIORITY)))
		{
			/* �G���[ */
			ErrorDialog("�������x���̐ݒ�Ɏ��s���܂���");
			ret = FALSE;
			break;
		}
	} while (0);

	/* �ȓǂݍ��� */
	for (INT i = 0; i < MaxSound; i++)
	{
		SetSound((kSoundSet[i]));   //�T�E���h��ǂݍ���
	}

	return ret;
}

/*************************************************
 * �֐��� : SetSound�֐�                         *
 * ��  �e : �t�@�C���̓ǂݍ��݂Ɠǂݍ���         *
 * ��  �� : LPCTSTR FileName(�Ȃ̃f�[�^�t�@�C��) *
 * �߂�l : ���s�ɐ��������TRUE��Ԃ�           *
 *************************************************/
BOOL DSound::SetSound(LPCTSTR FileName)
{
	BOOL ret = TRUE;   // �߂�l�p

	do{
		secondary_buffer_[count_] = NULL;   //������

		/* Wave�t�@�C�����Z�J���_���o�b�t�@�ɓǂݍ��� */
		FILE* fp = NULL;

		/* Wave�t�@�C���̓ǂݍ��� */
		fopen_s(&fp, FileName, "rb");

		/* WAVEFORMATEX�\���̂Ƀt�H�[�}�b�g����ǂݍ��� */
		WAVEFORMATEX wavefmt;   // wave�t�@�C���̃t�H�[�}�b�g���i�[�\����

		/* �[���N���A */
		ZeroMemory(&wavefmt, sizeof(WAVEFORMATEX));

		/* �t�H�[�}�b�g���̈ʒu�܂Ńt�@�C���|�C���^�����炷 */
		/* (�擪����20byte�ڂɃt�H�[�}�b�g���(16byte)�������Ă��� */
		fseek(fp, 20L, SEEK_SET);

		/* �t�H�[�}�b�g���(�S16byte)��ǂݍ��� */
		fread((VOID*)&wavefmt, 16, 1, fp);

		/* wave�f�[�^�̃T�C�Y�𓾂� */
		ULONG data_size;   // wave�f�[�^�̃T�C�Y

		/* wave�t�@�C���S�̂̃T�C�Y�i�[�ʒu�܂Ńt�@�C���|�C���^�����炷 */
		/* (�擪����41byte�ڂ�4byte�Ŋi�[����Ă���) */
		fseek(fp, 40L, SEEK_SET);

		/* wave�t�@�C���S�̂̃T�C�Y��ǂݍ��� */
		fread((VOID*)&data_size, 4, 1, fp);

		/* �w�b�_���̃T�C�Y��������wave�f�[�^�̃T�C�Y�ɂȂ� */
		data_size -= 44L;

		/* �Z�J���_���o�b�t�@�̍쐬 */
		DSBUFFERDESC dsbufferdesc;

		ZeroMemory(&dsbufferdesc, sizeof(DSBUFFERDESC));   // �[���N���A
		dsbufferdesc.dwSize     = sizeof(DSBUFFERDESC);    // �\���̂̃T�C�Y(�o�C�g�P��)�̍\���̂��g���O�ɁA���̃����o�����������Ȃ���΂Ȃ�Ȃ�

		// �V���� DirectSoundBuffer �I�u�W�F�N�g���쐬����Ƃ��Ɋ܂߂�\�͂����ʂ���t���O
		dsbufferdesc.dwFlags = DSBCAPS_GETCURRENTPOSITION2 | DSBCAPS_GLOBALFOCUS | DSBCAPS_LOCDEFER;

		dsbufferdesc.dwBufferBytes = data_size;   // �쐬����o�b�t�@�̃T�C�Y(�o�C�g�P��)
		dsbufferdesc.lpwfxFormat   = &wavefmt;    // �o�b�t�@�̃E�F�[�u�@�t�H�[�}�b�g�i�[��|�C���^

		/* �o�b�t�@�̍쐬 */
		LPDIRECTSOUNDBUFFER dsbuff_;

		/* �o�b�t�@����� */
		if (FAILED(dsound_->CreateSoundBuffer(&dsbufferdesc, &dsbuff_, NULL)))
		{
			/* �o�b�t�@�̍쐬�Ɏ��s */
			ErrorDialog("�Z�J���_���o�b�t�@�̍쐬�Ɏ��s���܂���");
		}
		/* DirectSoundBuffer8�C���^�[�t�F�C�X�����o�� */
		if (FAILED(dsbuff_->QueryInterface(IID_IDirectSoundBuffer8, (VOID**)&secondary_buffer_ + count_)))
		{
			/* �G���[ */
			ret = FALSE;
		}

		/* �o�b�t�@�̉�� */
		SafeRelease(dsbuff_);

		/* �t�@�C������wave�f�[�^��ǂݍ��� */
		fseek(fp, 44L, SEEK_SET);

		/* ���b�N���� */
		LPVOID p_mem1, p_mem2;
		DWORD size1, size2;

		secondary_buffer_[count_]->Lock(0,       // �o�b�t�@�̐擪���烍�b�N�J�n�ʒu�܂ł̃I�t�Z�b�g���o�C�g�P�ʂŎw�肷��l
										0,       // ���b�N�����o�b�t�@�����̃T�C�Y���o�C�g�P�ʂŎw�肷��l(DSBLOCK_ENTIREBUFFER���w�肵���ꍇ��0�ł���)
										&p_mem1, // 1�ԖڂɃ��b�N�����������ւ̃|�C���^�i�[��|�C���^
										&size1,  // 1�ԖڂɃ��b�N�����������̃T�C�Y�i�[��|�C���^
										&p_mem2, // 2�ԖڂɃ��b�N�����������ւ̃|�C���^�i�[��|�C���^
										&size2,  // 2�ԖڂɃ��b�N�����������ւ̃T�C�Y�i�[��|�C���^
										DSBLOCK_ENTIREBUFFER);   // ���b�N �C�x���g�ɕύX��������t���O(DSBLOCK_ENTIREBUFFER:�S�̂����b�N

		/* �ǂݍ��� */
		fread(p_mem1, data_size, 1, fp);

		/* �A�����b�N���� */
		secondary_buffer_[count_]->Unlock(p_mem1, size1, p_mem2, size2);

		/* �t�@�C���N���[�Y */
		fclose(fp);

		count_++;

	} while (0);

	return ret;
}

/***************************************
 * �֐��� : Play�֐�                   *
 * ��  �e : ���̍Đ�                   *
 * ��  �� : �Đ�����t�@�C���ƍĐ����@ *
 * �߂�l : ���s�ɐ��������TRUE��Ԃ� *
 ***************************************/
VOID DSound::Play(INT i, INT PlayMode)
{
	secondary_buffer_[i]->Play(0,   // ���0
							   0,   // �T�E���h�̗D��x( �Œ�:0 �ō�:0xFFFFFFFF )
							   PlayMode);   // �Đ����@
}

/***********************************
 * �֐��� : Pause�֐�              *
 * ��  �e : ���̈ꎞ��~           *
 * ��  �� : ��~����t�@�C���̖��O *
 * �߂�l : �Ȃ�                   *
 ***********************************/
VOID DSound::Pause(INT i)
{
	secondary_buffer_[i]->Stop();   // �ꎞ��~
}

/***********************************
 * �֐��� : Restart�֐�            *
 * ��  �e : ���̒�~               *
 * ��  �� : ��~����t�@�C���̖��O *
 * �߂�l : �Ȃ�                   *
 ***********************************/
VOID DSound::Restart(INT i)
{
	Reset(i);   //��~
}

/***************************
 * �֐��� : Destroy�֐�    *
 * ��  �e : �T�E���h�̉�� *
 * ��  �� : �Ȃ�           *
 * �߂�l : �Ȃ�           *
 ***************************/
VOID DSound::Destroy(VOID)
{
	/* ��� */
	for (INT i = 0; i < MaxSound; i++)
	{
		secondary_buffer_[i] = NULL;
	}

	dsound_ = NULL;
}