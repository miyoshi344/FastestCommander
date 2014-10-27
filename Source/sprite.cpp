/***********
 * Include *
 ***********/
#include "Sprite.h"

/**********************
 * 静的メンバ変数宣言 *
 **********************/
LPD3DXSPRITE Sprite::sprite_;

/******************
 * コンストラクタ *
 ******************/
Sprite::Sprite(VOID)
{
	/* 初期化 */
	sprite_ = NULL;
}

/*******************************************
 * 関数名 : Init関数                       *
 * 内  容 : スプライトクラスを初期化します *
 * 引  数 : なし　                         *
 * 戻り値 : 成功したらTRUEを返します       *
 *******************************************/
BOOL Sprite::Init(VOID)
{
	BOOL ret = TRUE;

	do
	{
		/* スプライトインターフェイスの作成 */
		if (FAILED(D3DXCreateSprite(D3DDEV, &sprite_)))
		{
			/* エラー */
			ErrorDialog("スプライトインターフェイスの作成に失敗");
			ret = FALSE;
			break;
		}
	} while (0);

	return ret;
};

/************************************************************
 * 関数名 : DrawSprite関数                                  *
 * 内  容 : スプライトを描画します                          *
 * 引  数 : CONST LPDIRECT3DTEXTURE9 pTexture  テクスチャ   *
 *          CONST D3DXVECTOR3* pPos,           描画座標     *
 *          CONST RECT         rect            切り取り矩形 *
 *          CONT  INT          Alpha           透過         *
 *          CONST D3DXVECTOR3* pCenter　       中心座標     *
 * 戻り値 : 成功したらTRUEを返します                        *
 ************************************************************/
VOID Sprite::DrawSprite(CONST LPDIRECT3DTEXTURE9 pTexture,
						CONST D3DXVECTOR3* pPos,
						CONST RECT rect,
						CONST INT Alpha,
						CONST D3DXVECTOR3* pCenter)
{
	RECT my_rect;  // テクスチャの切り出し範囲

	/* テクスチャ使用範囲設定 */
	my_rect.left   = rect.left;
	my_rect.top    = rect.top;
	my_rect.right  = rect.left + rect.right;
	my_rect.bottom = rect.top + rect.bottom;

	/* 描画 */
	sprite_->Draw(pTexture,						   // テクスチャインタフェースポインタ
				  &my_rect,                        // テクスチャ切り出し範囲
				  pCenter,                         // 回転中心座標
				  pPos,                            // 描画開始位置
				  D3DCOLOR_ARGB(Alpha, 255, 255, 255));  // アルファチャネル、カラーチャネル
}

/*****************************************
 * 関数名 : Destroy関数                  *
 * 内  容 : スプライトクラスを破棄します *
 * 引  数 : なし　                       *
 * 戻り値 : なし                         *
//****************************************/
VOID Sprite::Destroy(VOID)
{
	/* スプライトインターフェイスの破棄 */
	SafeRelease(sprite_);
}