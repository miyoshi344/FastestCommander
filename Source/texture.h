/*****************
 * Textureクラス *
******************/

/* 多重Include防止 */
#ifndef RACE_TEXTURE_H_
#define RACE_TEXTURE_H_

/***********
 * Include *
 ***********/
#include "direct3d.h"  // Direct3Dクラス

/*******************
 * 定数/マクロ宣言 *
 *******************/
CONST D3DCOLOR	TEXTURE_COLORKEY = D3DCOLOR_XRGB(0, 255, 0);  // 緑をカラーキーに指定

/**************
 * 構造体宣言 *
 **************/
typedef struct TEXTURETABLE
{
	CHAR				m_TextureName[256];	// テクスチャ名格納用
	LPDIRECT3DTEXTURE9	m_pTexture;			// テクスチャインターフェイス
	INT					m_RentalNum;		// 現在の貸し出し数
	D3DXIMAGE_INFO		m_ImgInfo;			// テクスチャ情報
	TEXTURETABLE*		m_pNext;			// リスト用（次のへのポインタ）

	/* コンストラクタ */
	TEXTURETABLE(VOID)						// テクスチャバッファの初期化
	{
		m_TextureName[0] = '\0';
		m_pTexture = NULL;
		m_RentalNum = 0;
		m_pNext = NULL;
	}

	/* デストラクタ */
	~TEXTURETABLE()
	{
		/* テクスチャ解放 */
		SafeRelease(m_pTexture);
	}

}*LPTEXTURETABLE;

/**************
 * クラス宣言 *
 **************/
class Texture
{
private:
	static LPTEXTURETABLE	m_pTextureList;	// テクスチャ管理バッファ
	static LPTEXTURETABLE	m_pStockList;	// 未使用の管理リスト
	static LPTEXTURETABLE	m_pUseList;		// 使用中の管理リスト
	static INT				m_MaxUseNum;	// 使用テクスチャ数

public:
	static BOOL Init(VOID);													// 初期化
	static LPDIRECT3DTEXTURE9	LoadTexture(LPCSTR TextureName);			// テクスチャの読み込み
	static VOID					Release(LPDIRECT3DTEXTURE9 CONST pTexture);	// テクスチャの破棄
	static VOID					Destroy(VOID);								// 破棄
};

#endif