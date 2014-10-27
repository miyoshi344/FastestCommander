/**************************************
 * メモリ、インターフェイス解放関数群 *
 **************************************/

/* 多重Include防止 */
#ifndef RACE_RELEASE_H_
#define RACE_RELEASE_H_

/*********************************************
 * 内  容 : brief メモリを解放します         *
 * 引  数 : param 解放するメモリへのポインタ *
 * 戻り値 : return なし                      *
 *********************************************/
template<typename T>
inline VOID SafeDelete( T*& p )
{
	delete p;
	p = NULL;
}

/*********************************************
 * 内  容 : brief 配列メモリを解放します     *
 * 引  数 : param 解放するメモリへのポインタ *
 * 戻り値 : なし                             *
 *********************************************/
template<typename T>
inline VOID SafeDeleteArray( T*& p )
{
	delete[] p;
	p = NULL;
}

/*****************************************************
 * 内  容 : brief インターフェイスを解放します       *
 * 引  数 : param 解放するインターフェイスのポインタ *
 * 戻り値 : なし                                     *
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