/***************
 * DirectSound *
 ***************/

/* ���dInclude�h�~ */
#ifndef RACE_DSOUND_H__
#define RACE_DSOUND_H__

/**********
 * Inluce *
 **********/
#include <dsound.h>
#include "common.h"
#include "release.h"

/************
 * �����萔 *
 ************/
CONST LPSTR kSoundSet[] =   //�T�E���h���i�[����
{
	/* �T�E���h����荞�� */

	/* BGM */
	"Resource_fast/Sound/1635.wav",
	"Resource_fast/Sound/bgm_maoudamashii_neorock33.wav",
	"Resource_fast/Sound/1594.wav",
	"Resource_fast/Sound/1382.wav",
	"Resource_fast/Sound/1357.wav",

	/* SE */
	"Resource_fast/Sound/freeze10.wav",
	"Resource_fast/Sound/kachi41.wav",
	"Resource_fast/Sound/drag00.wav",
	"Resource_fast/Sound/power36.wav",
	"Resource_fast/Sound/cursor37.wav",
	"Resource_fast/Sound/on08.wav",
	"Resource_fast/Sound/whistle00.wav",
	"Resource_fast/Sound/hit_p03.wav",
	"Resource_fast/Sound/engine.wav",
	"Resource_fast/Sound/engine.wav",
	"Resource_fast/Sound/engine.wav",
	"Resource_fast/Sound/engine.wav",
};

/**********
 * �񋓑� *
 **********/
enum
{
	/* �Ȉꗗ */

	/* BGM */
	TITLE_BGM,
	GRASS_BGM,
	SNOW_BGM,
	MOUNTAIN_BGM,
	BEACH_BGM,

	/* SE */
	GOAL_SE,
	JUMP_SE,
	DOWN_SE,
	DASH_SE,
	CURSOR_SE,
	PUSH_SE,
	COUNT_SE,
	WALL_SE,
	ENGINE_SE,
	ENGINE_SE2,
	ENGINE_SE3,
	ENGINE_SE4,

	MaxSound
};

class DSound
{
private:
	static LPDIRECTSOUND8        dsound_;                      // DirectSound�C���^�[�t�F�C�X
	static LPDIRECTSOUNDBUFFER8  secondary_buffer_[MaxSound];  // �_�C���N�g�T�E���h�Z�J���_���[�o�b�t�@
	static INT count_;                                         //���o��

	static inline VOID Reset(INT i)
	{
		// �Đ��ʒu�̃��Z�b�g
		secondary_buffer_[i]->SetCurrentPosition(0);
	}   

public:
	static BOOL Init(CONST HWND hWnd);          // DirectSound������
	static BOOL SetSound(LPCTSTR FileName);     //�Ȃ��Z�b�g
	static VOID Play(INT i, INT PlayMode = 0);  //�Đ�
	static VOID Pause(INT i);                   // �ꎞ��~
	static VOID Restart(INT i);                 // �����ʒu�ɖ߂�
	static VOID Destroy(VOID);                  // �������
};

#endif