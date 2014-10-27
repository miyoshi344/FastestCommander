/***********
 * Include *
 ***********/
#include "player_select.h"

/******************
 * コンストラクタ *
 ******************/
PlayerSelect::PlayerSelect(VOID)
{

}

/****************
 * デストラクタ *
 ****************/
PlayerSelect::~PlayerSelect(VOID)
{
	Texture::Release(texture_);
}

/*********************************************
 * 関数名 : Init関数                         *
 * 内  容 : 初期処理を行います               *
 * 引  数 : なし                             *
 * 戻り値 : 初期化に成功したらTRUEを返します *
 *********************************************/
BOOL PlayerSelect::Init(VOID)
{
	BOOL ret = TRUE;

	do
	{
		/* フォルダ階層を移動 */
		_chdir("Resource/Texture");

		/* テクスチャインターフェイスの取得 */
		if (!(texture_ = Texture::LoadTexture("back.bmp")))
		{
			ErrorDialog("セレクト画面の読み込みに失敗しました");
			ret = FALSE;
			break;
		}

		/* フォルダ階層を戻す */
		_chdir("../../");

	} while (0);

	return ret;
}

/***********************************
 * 関数名 : ReSet関数              *
 * 内  容 : 状態の初期化を行います *
 * 引  数 : なし                   *
 * 戻り値 : なし                   *
 ***********************************/
VOID PlayerSelect::ReSet(VOID)
{

}

/*********************************
 * 関数名 : UpDate関数           *
 * 内  容 : 更新処理を行います   *
 * 引  数 : なし                 *
 * 戻り値 : 遷移の番号を返します *
 *********************************/
INT PlayerSelect::UpDate(VOID)
{
	for (INT i = 0; i < kMaxPad; i++)
	{
		XInput::PushButton(i);
		push_button = XInput::ReturnPushButton();

		if (push_button == XINPUT_GAMEPAD_B) return GAME;
	}

	return SELECT;
}

/*******************************
 * 関数名 : Draw関数           *
 * 内  容 : 描画処理を行います *
 * 引  数 : なし               *
 * 戻り値 : なし               *
 *******************************/
VOID PlayerSelect::Draw(VOID)
{
	SpriteDraw();
}

/*******************************
 * 関数名 : Destroy関数        *
 * 内  容 : 破棄処理を行います *
 * 引  数 : なし               *
 * 戻り値 : なし               *
 *******************************/
VOID PlayerSelect::Destroy(VOID)
{
	Texture::Release(texture_);
}

/*******************************************
 * 関数名 : SpriteDraw関数                 *
 * 内  容 : スプライトの描画処理を行います *
 * 引  数 : なし                           *
 * 戻り値 : なし                           *
 *******************************************/
VOID PlayerSelect::SpriteDraw(VOID)
{
	Sprite::GetSprite()->Begin(D3DXSPRITE_ALPHABLEND);

	Sprite::DrawSprite(texture_);

	Sprite::GetSprite()->End();
}