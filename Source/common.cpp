/**************
 * ���ʃN���X *
 **************/
#include "common.h"

/**********************************************
 * �֐��� : ErrorDialog�֐�                   *
 * ��  �e : ���b�Z�[�W�{�b�N�X��\��          *
 * ��  �� : LPCTSTR Message�F�\������e�L�X�g *
 * �߂�l�F�Ȃ�                               *
 **********************************************/
VOID ErrorDialog( LPCTSTR Message )
{
    /* �_�C�A���O�^�C�v */
    INT type = MB_OK | MB_ICONERROR | MB_ICONSTOP | MB_TASKMODAL;

#ifdef _DEBUG
    /* �_�C�A���O��\�� */
    MessageBox(	NULL,     // �E�B���h�E�n���h��
				Message,  // �\�����郁�b�Z�[�W
				"ERROR",  // �^�C�g���ɕ\�����镶����
				type );	  // �_�C�A���O�^�C�v8
#endif
}