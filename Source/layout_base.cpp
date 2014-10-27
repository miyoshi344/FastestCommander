/***********
 * Include *
 ***********/
#include "layout_base.h"

/******************
 * コンストラクタ *
 ******************/
LayoutBase::LayoutBase(VOID)
{
	texture_   = NULL;
	texture2_  = NULL;
	player_no_ = 0;
}

/****************
 * デストラクタ *
 ****************/
LayoutBase::~LayoutBase(VOID)
{
	/* テクスチャ解放 */
	Texture::Release(texture_);
	Texture::Release(texture2_);
}

/*************************************
 * 関数名 : Init関数                 *
 * 内  容 : 初期化を行います         *
 * 引  数 : なし                     *
 * 戻り値 : 成功したらTRUEを返します *
 *************************************/
BOOL LayoutBase::Init(VOID)
{
	BOOL ret = TRUE;

	do
	{
		/* フォルダ階層を移動 */
		_chdir("Resource_fast/Texture");

		/* テクスチャ1の読み込み */
		if (!(texture_ = Texture::LoadTexture(kScore)))
		{
			/* エラー */
			ErrorDialog("スコア用の画像の読み込みに失敗しました");
			ret = FALSE;
			break;
		}

		/* テクスチャ2の読み込み */
		if (!(texture2_ = Texture::LoadTexture(kLayout)))
		{
			/* エラー */
			ErrorDialog("レイアウト用の画像の読み込みに失敗しました");
			ret = FALSE;
			break;
		}

		/* フォルダ階層を戻す */
		_chdir("../../");

	} while (0);

	return ret;
}

/***************************
 * 関数名 : Destroy関数    *
 * 内  容 : 破棄を行います *
 * 引  数 : なし           *
 * 戻り値 : なし           *
 ***************************/
VOID LayoutBase::Destroy(VOID)
{
	/* テクスチャインターフェイスの破棄 */
	Texture::Release(texture_);
	Texture::Release(texture2_);
}

/********************************************************
 * 関数名 : Count関数                                   *
 * 内  容 : もらってきた引数を使い数値の描画を行います  *
 * 引  数 : RECT        rect   描画用矩形               *
 *          D3DXVECTOR3 pos_   描画位置                 *
 *          INT         count_ 読み込みたい時間         *
 *          INT         loop_  桁数                     *
 * 戻り値 : なし                                        *
 ********************************************************/
VOID LayoutBase::CountSet(D3DXVECTOR3 pos, INT count, INT size, INT color, INT loop)
{
	/* ローカル変数 */
	INT         digit;
	INT         left;
	RECT        rect;
	D3DXVECTOR3 position = pos;

	/* 引数のサイズによって矩形を作成 */
	switch (size)
	{
		case SMALL:
		/* 切り取り矩形作成 */
		rect.left   = SmallScore.left;
		rect.top    = SmallScore.top + (color * SmallScore.bottom);  // 色の指定
		rect.right  = SmallScore.right;
		rect.bottom = SmallScore.bottom;
		break;

		case MIDDLE:
		/* 切り取り矩形作成 */
		rect.left   = MiddleScore.left;
		rect.top    = MiddleScore.top + (color * MiddleScore.bottom);  // 色の指定
		rect.right  = MiddleScore.right;
		rect.bottom = MiddleScore.bottom;
		break;

		case LAGE:
		/* 切り取り矩形作成 */
		rect.left   = LageScore.left;
		rect.top    = LageScore.top + (color * LageScore.bottom);  // 色の指定
		rect.right  = LageScore.right;
		rect.bottom = LageScore.bottom;
		break;

		default: break;
	}


	/* 回数分ループを行う */
	for (INT i = 0; i < loop; i++)
	{
		digit = (count % 10);

		left = rect.left;

		/* 切り取り範囲を決める */
		left += (digit * rect.right);

		RECT sprite
		{
			left,
			rect.top,
			rect.right,
			rect.bottom
		};

		/* 描画 */
		Sprite::DrawSprite(texture_, &position, sprite);

		/* 次の桁で移動する */
		count /= 10;
		position.x -= rect.right;
	}
}

/********************************************
 * 関数名 : DrawSet関数                     *
 * 内  容 : 描画の色を指定します            *
 * 引  数 : D3DXVECTOR3 pos  表示座標       *
 *          INT player_no    プレイヤー番号 *
 * 戻り値 : なし                            *
 ********************************************/
VOID LayoutBase::DrawSet(D3DXVECTOR3 pos, RECT rect, INT player_no, INT texture)
{
	/* 描画用矩形の作成 */
	RECT my_rect =
	{
		rect.left + (rect.right * player_no),
		rect.top,
		rect.right,
		rect.bottom
	};

	/* 指定が0なら1枚目のテクスチャで描画を行う */
	if (texture == 0) Sprite::DrawSprite(texture_, &pos, my_rect);
	else Sprite::DrawSprite(texture2_, &pos, my_rect);
}