/**************************************
 * �������A�C���^�[�t�F�C�X����֐��Q *
 **************************************/

/* ���dInclude�h�~ */
#ifndef RACE_RELEASE_H_
#define RACE_RELEASE_H_

/*********************************************
 * ��  �e : brief ��������������܂�         *
 * ��  �� : param ������郁�����ւ̃|�C���^ *
 * �߂�l : return �Ȃ�                      *
 *********************************************/
template<typename T>
inline VOID SafeDelete( T*& p )
{
	delete p;
	p = NULL;
}

/*********************************************
 * ��  �e : brief �z�񃁃�����������܂�     *
 * ��  �� : param ������郁�����ւ̃|�C���^ *
 * �߂�l : �Ȃ�                             *
 *********************************************/
template<typename T>
inline VOID SafeDeleteArray( T*& p )
{
	delete[] p;
	p = NULL;
}

/*****************************************************
 * ��  �e : brief �C���^�[�t�F�C�X��������܂�       *
 * ��  �� : param �������C���^�[�t�F�C�X�̃|�C���^ *
 * �߂�l : �Ȃ�                                     *
 *****************************************************/
template<typename T>
inline VOID SafeRelease( T*& p )
{
	if( p != NULL )
	{
		p->Release();
		p = NULL;
	}
}

#endif