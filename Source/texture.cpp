/*****************
 * Textureクラス *
 *****************/
#include "texture.h"

/**************
 * static変数 *
 **************/
LPTEXTURETABLE	Texture::m_pTextureList = NULL;	// テクスチャ管理バッファ
LPTEXTURETABLE	Texture::m_pStockList   = NULL;	// 未使用の管理リスト
LPTEXTURETABLE	Texture::m_pUseList     = NULL;	// 使用中の管理リスト
INT				Texture::m_MaxUseNum    = NULL;	// 使用テクスチャ数

/******************
 * コンストラクタ *
 ******************/
BOOL Texture::Init(VOID)
{
	BOOL ret = TRUE;

	do
	{
		/* 最大保持数を格納 */
		m_MaxUseNum = TEXTURE_NUM;

		/* 渡された使用枚数分のメモリを確保 */
		m_pTextureList = new TEXTURETABLE[m_MaxUseNum];

		/* 確保できなければエラーとして処理する */
		if (!m_pTextureList)
		{
			/* エラー */
			ret = FALSE;
			break;
		}

		/* 未使用テクスチャ管理リスト作成 */
		m_pStockList = m_pTextureList;

		/* リスト構造作成 */
		for (INT i = 0; i < m_MaxUseNum - 1; i++) m_pStockList[i].m_pNext = &m_pStockList[i + 1];
	} while (0);

	return ret;
}

/****************
 * デストラクタ *
 ****************/
VOID Texture::Destroy(VOID)
{
	/* 使用しているテクスチャ管理リスト初期化 */
	m_pUseList = NULL;

	/* テクスチャ管理テーブルバッファ解放 */
	SafeDeleteArray(m_pTextureList);

	/* テクスチャ使用枚数を0にしておく */
	m_MaxUseNum = 0;
}

/**********************************************************
 * 関数名 : LoadTexture関数                               *
 * 内  容 : テクスチャの読み込み                          *
 * 引  数 : LPCSTR：テクスチャ名                          *
 * 戻り値：LPDIRECT3DTEXTURE9：インターフェイスのポインタ *
 **********************************************************/
LPDIRECT3DTEXTURE9 Texture::LoadTexture(LPCSTR TextureName)
{
	LPDIRECT3DTEXTURE9	p_Texture = NULL;			// 戻り値として返すテクスチャポインタ
	LPTEXTURETABLE		p_use_list = m_pUseList;	// 作業用
	CHAR				str[256] = { 0 };			// エラー用

	/* テクスチャが既に読み込まれているか使用中リストから検索する */
	while (p_use_list != NULL)
	{
		/* ファイル名が一致したら同じものと判断 */
		if (!strcmp(p_use_list->m_TextureName, TextureName))
		{
			/* リスト内に存在するならばそのポインタを返す */
			p_Texture = p_use_list->m_pTexture;

			/* 現在の読み込み数を増やす */
			p_use_list->m_RentalNum++;

			break;
		}

		/* 次へ */
		p_use_list = p_use_list->m_pNext;
	}

	/* テクスチャが読み込まれていなければ新規に読み込む */
	if (p_Texture == NULL)
	{
		/* 未使用テクスチャリストに空きがあるならば読み込む */
		if (m_pStockList != NULL)
		{
			/* 未使用リストから1つ取り出す */
			LPTEXTURETABLE	ptable = m_pStockList;

			/* テクスチャを読み込む */
			D3DXCreateTextureFromFileEx(D3DDEV,  // テクスチャに関連付けるデバイス
										TextureName,			// テクスチャファイル
										D3DX_DEFAULT,			// テクスチャの幅
										D3DX_DEFAULT,			// テクスチャの高さ
										0,						// ミップレベル数
										0,						// 0を指定
										D3DFMT_UNKNOWN,			// テクスチャフォーマット
										D3DPOOL_MANAGED,		// メモリ管理方式
										D3DX_DEFAULT,			// フィルタリング方法
										D3DX_DEFAULT,
										TEXTURE_COLORKEY,		// カラーキー(RGB)
										&ptable->m_ImgInfo,		// テクスチャ情報格納
										NULL,					// NULLを指定
										&ptable->m_pTexture);	// テクスチャインタフェースポインタ

			/* 読み込みに成功したら使用リストに加え、内容を設定する */
			if (ptable->m_pTexture != NULL)
			{
				/* 未使用リストの先頭から今使用したテーブルを外す**/
				m_pStockList = m_pStockList->m_pNext;

				/* 使用リストの先頭に加える */
				ptable->m_pNext = m_pUseList;
				m_pUseList = ptable;

				/* ファイル名を保存する */
				strcpy(ptable->m_TextureName, TextureName);

				/* 使用数を1にする */
				ptable->m_RentalNum = 1;

				/* 戻り値に設定 */
				p_Texture = ptable->m_pTexture;
			}
			else
			{
				/* エラー */
				sprintf(str, "テクスチャの読み込みに失敗(%s)", TextureName);
				ErrorDialog(str);
			}
		}
	}

	return p_Texture;
}

/*************************************************************************
 * 関数名 : ReleaseTexture関数                                           *
 * 内  容 : テクスチャの破棄                                             *
 * 引  数 : LPDIRECT3DTEXTURE9 CONST：解放するテクスチャインターフェイス *
 * 戻り値 : なし                                                         *
 *************************************************************************/
VOID Texture::Release(LPDIRECT3DTEXTURE9 CONST pTexture)
{
	/***** テクスチャテーブルから探す *****/
	LPTEXTURETABLE p_work = m_pUseList;	// 作業用
	LPTEXTURETABLE p_prev = NULL;		// 1つ前

	// テクスチャインターフェイスの解放 //
	/* 登録されているテクスチャ管理リストから探す */
	while (p_work != NULL)
	{
		/* 見つかったら抜ける */
		if (p_work->m_pTexture == pTexture) break;

		/* 次へ */
		p_prev = p_work;
		p_work = p_work->m_pNext;
	}

	do
	{
		/* 見つからなかったら戻る */
		if (p_work == NULL) break;


		/* 使用数を1つ減らす */
		p_work->m_RentalNum--;

		/* 使用数が0以下ならば解放 */
		if (p_work->m_RentalNum <= 0)
		{
			/* 使用リストから外す */
			if (p_work == m_pUseList)	m_pUseList = p_work->m_pNext;		// 先頭の場合
			else						p_prev->m_pNext = p_work->m_pNext;  // それ以外

			SafeRelease(p_work->m_pTexture);  // テクスチャの解放
			p_work->m_TextureName[0] = '\0';  // 名前初期化
			p_work->m_RentalNum      = 0;	  // 貸出数を0に

			/* 未使用リストの先頭に戻す */
			p_work->m_pNext = m_pStockList;
			m_pStockList = p_work;
		}

	} while (0);
}