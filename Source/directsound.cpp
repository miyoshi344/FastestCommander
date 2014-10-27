/***********
 * Include *
 ***********/
#include "directsound.h"

/**********************
 * 静的メンバ変数宣言 *
 **********************/
LPDIRECTSOUND8         DSound::dsound_                     = NULL;       // ダイレクトサウンドバッファ
LPDIRECTSOUNDBUFFER8   DSound::secondary_buffer_[MaxSound] = { NULL };   // セカンダリバッファ
INT DSound::count_ = 0;   // 再生位置

/********************************************
 * 関数名 : Init関数                        *
 * 内  容 : DirectSoundの作成と初期化を行う *
 * 引  数 : ウィンドウハンドル              *
 * 戻り値 : 初期化に成功するとTRUEを返す    *
 ********************************************/
BOOL DSound::Init(CONST HWND hWnd)
{
	BOOL ret = TRUE;   // 戻り値用

	do{
		/* DirectSoundの作成と設定 */
		/* 作成 */
		if (FAILED(DirectSoundCreate8(NULL,       // サウンドデバイスを表すGUID(デフォルトデバイスはNULL)
									  &dsound_,   // インターフェイス格納先ポインタ
									  NULL)))     // 常に NULL
		{
			/* エラー */
			ErrorDialog("DirectSoundの作成に失敗しました");
			ret = FALSE;
			break;
		}

		/* 協調レベルの設定 */               
		// ( ウィンドウハンドル, 協調レベル )
		if (FAILED(dsound_->SetCooperativeLevel(hWnd, DSSCL_PRIORITY)))
		{
			/* エラー */
			ErrorDialog("協調レベルの設定に失敗しました");
			ret = FALSE;
			break;
		}
	} while (0);

	/* 曲読み込み */
	for (INT i = 0; i < MaxSound; i++)
	{
		SetSound((kSoundSet[i]));   //サウンドを読み込む
	}

	return ret;
}

/*************************************************
 * 関数名 : SetSound関数                         *
 * 内  容 : ファイルの読み込みと読み込み         *
 * 引  数 : LPCTSTR FileName(曲のデータファイル) *
 * 戻り値 : 実行に成功するとTRUEを返す           *
 *************************************************/
BOOL DSound::SetSound(LPCTSTR FileName)
{
	BOOL ret = TRUE;   // 戻り値用

	do{
		secondary_buffer_[count_] = NULL;   //初期化

		/* Waveファイルをセカンダリバッファに読み込む */
		FILE* fp = NULL;

		/* Waveファイルの読み込み */
		fopen_s(&fp, FileName, "rb");

		/* WAVEFORMATEX構造体にフォーマット情報を読み込む */
		WAVEFORMATEX wavefmt;   // waveファイルのフォーマット情報格納構造体

		/* ゼロクリア */
		ZeroMemory(&wavefmt, sizeof(WAVEFORMATEX));

		/* フォーマット情報の位置までファイルポインタをずらす */
		/* (先頭から20byte目にフォーマット情報(16byte)が入っている */
		fseek(fp, 20L, SEEK_SET);

		/* フォーマット情報(全16byte)を読み込む */
		fread((VOID*)&wavefmt, 16, 1, fp);

		/* waveデータのサイズを得る */
		ULONG data_size;   // waveデータのサイズ

		/* waveファイル全体のサイズ格納位置までファイルポインタをずらす */
		/* (先頭から41byte目に4byteで格納されている) */
		fseek(fp, 40L, SEEK_SET);

		/* waveファイル全体のサイズを読み込む */
		fread((VOID*)&data_size, 4, 1, fp);

		/* ヘッダ分のサイズを引くとwaveデータのサイズになる */
		data_size -= 44L;

		/* セカンダリバッファの作成 */
		DSBUFFERDESC dsbufferdesc;

		ZeroMemory(&dsbufferdesc, sizeof(DSBUFFERDESC));   // ゼロクリア
		dsbufferdesc.dwSize     = sizeof(DSBUFFERDESC);    // 構造体のサイズ(バイト単位)個の構造体を使う前に、このメンバを初期化しなければならない

		// 新しい DirectSoundBuffer オブジェクトを作成するときに含める能力を識別するフラグ
		dsbufferdesc.dwFlags = DSBCAPS_GETCURRENTPOSITION2 | DSBCAPS_GLOBALFOCUS | DSBCAPS_LOCDEFER;

		dsbufferdesc.dwBufferBytes = data_size;   // 作成するバッファのサイズ(バイト単位)
		dsbufferdesc.lpwfxFormat   = &wavefmt;    // バッファのウェーブ　フォーマット格納先ポインタ

		/* バッファの作成 */
		LPDIRECTSOUNDBUFFER dsbuff_;

		/* バッファを作る */
		if (FAILED(dsound_->CreateSoundBuffer(&dsbufferdesc, &dsbuff_, NULL)))
		{
			/* バッファの作成に失敗 */
			ErrorDialog("セカンダリバッファの作成に失敗しました");
		}
		/* DirectSoundBuffer8インターフェイスを取り出す */
		if (FAILED(dsbuff_->QueryInterface(IID_IDirectSoundBuffer8, (VOID**)&secondary_buffer_ + count_)))
		{
			/* エラー */
			ret = FALSE;
		}

		/* バッファの解放 */
		SafeRelease(dsbuff_);

		/* ファイルからwaveデータを読み込む */
		fseek(fp, 44L, SEEK_SET);

		/* ロックする */
		LPVOID p_mem1, p_mem2;
		DWORD size1, size2;

		secondary_buffer_[count_]->Lock(0,       // バッファの先頭からロック開始位置までのオフセットをバイト単位で指定する値
										0,       // ロックされるバッファ部分のサイズをバイト単位で指定する値(DSBLOCK_ENTIREBUFFERを指定した場合は0でいい)
										&p_mem1, // 1番目にロックしたメモリへのポインタ格納先ポインタ
										&size1,  // 1番目にロックしたメモリのサイズ格納先ポインタ
										&p_mem2, // 2番目にロックしたメモリへのポインタ格納先ポインタ
										&size2,  // 2番目にロックしたメモリへのサイズ格納先ポインタ
										DSBLOCK_ENTIREBUFFER);   // ロック イベントに変更を加えるフラグ(DSBLOCK_ENTIREBUFFER:全体をロック

		/* 読み込む */
		fread(p_mem1, data_size, 1, fp);

		/* アンロックする */
		secondary_buffer_[count_]->Unlock(p_mem1, size1, p_mem2, size2);

		/* ファイルクローズ */
		fclose(fp);

		count_++;

	} while (0);

	return ret;
}

/***************************************
 * 関数名 : Play関数                   *
 * 内  容 : 音の再生                   *
 * 引  数 : 再生するファイルと再生方法 *
 * 戻り値 : 実行に成功するとTRUEを返す *
 ***************************************/
VOID DSound::Play(INT i, INT PlayMode)
{
	secondary_buffer_[i]->Play(0,   // 常に0
							   0,   // サウンドの優先度( 最低:0 最高:0xFFFFFFFF )
							   PlayMode);   // 再生方法
}

/***********************************
 * 関数名 : Pause関数              *
 * 内  容 : 音の一時停止           *
 * 引  数 : 停止するファイルの名前 *
 * 戻り値 : なし                   *
 ***********************************/
VOID DSound::Pause(INT i)
{
	secondary_buffer_[i]->Stop();   // 一時停止
}

/***********************************
 * 関数名 : Restart関数            *
 * 内  容 : 音の停止               *
 * 引  数 : 停止するファイルの名前 *
 * 戻り値 : なし                   *
 ***********************************/
VOID DSound::Restart(INT i)
{
	Reset(i);   //停止
}

/***************************
 * 関数名 : Destroy関数    *
 * 内  容 : サウンドの解放 *
 * 引  数 : なし           *
 * 戻り値 : なし           *
 ***************************/
VOID DSound::Destroy(VOID)
{
	/* 解放 */
	for (INT i = 0; i < MaxSound; i++)
	{
		secondary_buffer_[i] = NULL;
	}

	dsound_ = NULL;
}