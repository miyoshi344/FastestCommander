/***************
 * DirectSound *
 ***************/

/* 多重Include防止 */
#ifndef RACE_DSOUND_H__
#define RACE_DSOUND_H__

/**********
 * Inluce *
 **********/
#include <dsound.h>
#include "common.h"
#include "release.h"

/************
 * 内部定数 *
 ************/
CONST LPSTR kSoundSet[] =   //サウンドを格納する
{
	/* サウンドを取り込む */

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
 * 列挙体 *
 **********/
enum
{
	/* 曲一覧 */

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
	static LPDIRECTSOUND8        dsound_;                      // DirectSoundインターフェイス
	static LPDIRECTSOUNDBUFFER8  secondary_buffer_[MaxSound];  // ダイレクトサウンドセカンダリーバッファ
	static INT count_;                                         //頭出し

	static inline VOID Reset(INT i)
	{
		// 再生位置のリセット
		secondary_buffer_[i]->SetCurrentPosition(0);
	}   

public:
	static BOOL Init(CONST HWND hWnd);          // DirectSound初期化
	static BOOL SetSound(LPCTSTR FileName);     //曲をセット
	static VOID Play(INT i, INT PlayMode = 0);  //再生
	static VOID Pause(INT i);                   // 一時停止
	static VOID Restart(INT i);                 // 初期位置に戻す
	static VOID Destroy(VOID);                  // 解放処理
};

#endif