/***********
 * Include *
 ***********/
#include "title.h"

/******************
 * コンストラクタ *
 ******************/
Title::Title(VOID)
{
	/* 変数初期化 */
	start_button_ = FALSE;  // スタートの表示フラグ
	button_flag_  = TRUE;   // ボタンのフラグ初期化
	count         = 0;      // 暗転用カウント
	flag_count    = 0;      // 入力受付カウント初期化
	flag_         = FALSE;  // 入力受付フラグ初期化
}

/****************
 * デストラクタ *
 ****************/
Title::~Title(VOID)
{
	/* テクスチャの解放 */
	LayoutBase::Destroy();

	/* デモプレイの破棄 */
	demo_play_.Destroy();
}

/*********************************
 * 関数名 : Init関数             *
 * 内  容 : 初期化処理を行います *
 * 引  数 : なし                 *
 * 戻り値 : 成功したらTRUEを返す *
 *********************************/
BOOL Title::Init(VOID)
{
	BOOL ret = TRUE;

	do
	{
		/* デモプレイの初期化 */
		if (!demo_play_.Init())
		{
			/* エラー */
			ErrorDialog("デモプレイの作成に失敗しました");
			ret = FALSE;
			break;
		}

		/* タイトルテクスチャの初期化 */
		if (!TextureInit())
		{
			/* エラー */
			ErrorDialog("テクスチャの作成に失敗しました");
			ret = FALSE;
			break;
		}

		/* 初期化 */
		ReSet();

	} while (0);

	return ret;
}

/***********************************
 * 関数名 : ReSet関数              *
 * 内  容 : 数値の初期化を行います *
 * 引  数 : なし                   *
 * 戻り値 : なし                   *
 ***********************************/
VOID Title::ReSet(VOID)
{
	/* 変数初期化 */ 
	player_count_ = 2;           // プレイヤー人数

	start_button_ = FALSE;       // スタートフラグ
	button_flag_  = TRUE;        // ボタンフラグ
	count         = 0;           // 暗転用カウント

	flag_         = FALSE;       // 入力受付フラグ
	flag_count    = 0;           // 入力受付用カウント

	/* デモプレイの状態を初期化 */
	demo_play_.ReSet();

	/* サウンドの再生位置を初期化 */
	DSound::Restart(TITLE_BGM);
}

/*******************************
 * 関数名 : UpDate関数         *
 * 内  容 : 更新処理を行います *
 * 引  数 : なし               *
 * 戻り値 : なし               *
 *******************************/
INT Title::UpDate(VOID)
{
	/* サウンドの再生 */
	DSound::Play(TITLE_BGM, DSBPLAY_LOOPING);

	/* デモ画面を更新 */
	demo_play_.UpDate();

	/* ボタンが押されたら遷移を変える */
	return (ButtonUpDate()) ? MULTI_GAME : TITLE;
}

/*******************************
 * 関数名 : Draw関数           *
 * 内  容 : 描画処理を行います *
 * 引  数 : なし               *
 * 戻り値 : なし               *
 *******************************/
VOID Title::Draw(VOID)
{
	SpriteDraw();    // スプライトの描画
	demo_play_.Draw();
}

/*******************************
 * 関数名 : Destroy関数        *
 * 内  容 : 破棄処理を行います *
 * 引  数 : なし               *
 * 戻り値 : なし               *
 *******************************/
VOID Title::Destroy(VOID)
{
	/* テクスチャの解放 */
	LayoutBase::Destroy();

	demo_play_.Destroy();
}

/*********************************************
 * 関数名 : TextureInit関数                  *
 * 内  容 : プレイヤーの初期化を行います     *
 * 引  数 : なし                             *
 * 戻り値 : 初期化に成功したらTRUEを返します *
 *********************************************/
BOOL Title::TextureInit(VOID)
{
	BOOL ret = TRUE;

	/* フォルダ階層を移動 */
	_chdir("Resource_fast/Texture");

	do
	{
		/* テクスチャインターフェイスの取得 */
		if (!(texture_ = Texture::LoadTexture(kTitleTexture)))
		{
			/* エラー */
			ErrorDialog("タイトル画面の読み込みに失敗しました");
			ret = FALSE;
			break;
		}

	} while (0);

	/* フォルダ階層を戻す */
	_chdir("../../");

	return ret;
}

/*******************************************
 * 関数名 : ButtonUpDate関数               *
 * 内  容 : ボタン入力処理の更新を行います *
 * 引  数 : なし                           *
 * 戻り値 : なし                           *
 *******************************************/
BOOL Title::ButtonUpDate(VOID)
{
	for (INT i = 0; i < kMaxPad; i++)
	{
		/* 入力を取得 */
		push_button_[i] = XInput::ReturnPushButton(i);  // ボタン入力
		push_stick_[i] = XInput::ReturnPushStick(i);    // スティック入力

		/* Aボタンが押されたかの確認 */
		if (push_button_[i] & XINPUT_GAMEPAD_A)
		{
			DSound::Play(PUSH_SE);  // サウンドの再生
			start_button_ = TRUE;   // プレイヤー人数選択のフラグをオンにする
			button_flag_ = FALSE;   // ボタンの表示をオフにする
		}
	}

	/* Aボタンが押されたら */
	if (start_button_)
	{
		if (PlayerSelect())
		{
			DSound::Restart(PUSH_SE);  // サウンドの再生位置を戻す
			DSound::Play(PUSH_SE);     // サウンドの再生

			DSound::Pause(TITLE_BGM);  // BGMの停止
			return TRUE;
		}
	}
	else StartButton();

	return FALSE;
}

/*************************************************
 * 関数名 : StartButton関数                      *
 * 内  容 : カウントを数えて表示の管理を行います *
 * 引  数 : なし                                 *
 * 戻り値 : 表示するかどうかのフラグを返します   *
 *************************************************/
BOOL Title::StartButton(VOID)
{
	/* カウントが60以上になったら*/
	if (count++ >= 60)
	{
		/* ボタンフラグを反転させる */
		if (button_flag_) button_flag_ = FALSE;
		else button_flag_ = TRUE;

		count = 0;
	}

	return button_flag_;
}

/*********************************************
 * 関数名 : PlayerSelect関数                 *
 * 内  容 : プレイヤー人数の選択を管理します *
 * 引  数 : なし                             *
 * 戻り値 : 選択された状態を返します         *
 *********************************************/
INT Title::PlayerSelect(VOID)
{
	/* ボタン入力取得のフラグ */
	if (flag_count++ >= 30)
	{
		flag_ = TRUE;
		flag_count = 30;
	}

	/* 人数分のボタン入力を監視します */
	for (INT i = 0; i < kMaxPad; i++)
	{
		/* スティックが右に倒されたら */
		if (push_stick_[i] & LEFT_STICK_RIGHT && (player_count_ < 4))
		{
			DSound::Restart(CURSOR_SE); // サウンドの再生位置を戻す
			DSound::Play(CURSOR_SE);    // サウンドの再生
			player_count_++;            // プレイヤーカウントを増やす
		}

		/* スティックが左に倒されたら */
		else if (push_stick_[i] & LEFT_STICK_LEFT && (player_count_ > 2))
		{
			DSound::Restart(CURSOR_SE);  // サウンドの再生位置を戻す
			DSound::Play(CURSOR_SE);     // サウンドの再生
			player_count_--;             // プレイヤーカウントを減らす
		}

		/* Bボタンが押されたらフラグをオフにする */
		else if (push_button_[i] == XINPUT_GAMEPAD_B)
		{
			start_button_ = FALSE;  // ボタンフラグをオフにする
			flag_         = FALSE;  // 入力フラグをオフにする
			flag_count    = 0;      // フラグカウントをリセットする
		}

		/* Aボタンが押されたら */
		if (flag_ && push_button_[i] == XINPUT_GAMEPAD_A) return TRUE;
	}

	return FALSE;
}

/*******************************************
 * 関数名 : SpriteDraw関数                 *
 * 内  容 : スプライトの描画処理を行います *
 * 引  数 : なし                           *
 * 戻り値 : なし                           *
 *******************************************/
VOID Title::SpriteDraw(VOID)
{
	Sprite::GetSprite()->Begin(D3DXSPRITE_ALPHABLEND);

	/* タイトルの描画 */
	Sprite::DrawSprite(texture_, &kTitlePosition, kTitleRect);

	/* ボタンが押されていないならPushStartを描画 */
	if (button_flag_) Sprite::DrawSprite(texture_, &kStartPosition, kStartRect);

	/* 押されていたなら1人用か多人数かを選択 */
	if (start_button_)
	{
		/* プレイ人数の描画 */
		D3DXVECTOR3 pos = kTitlePlayerPosition;
		pos.x = kTitlePlayerPosition.x + (275.0F * (player_count_ - 2));  // 選択されているプレイヤー数の描画

		RECT rect = kPlayer;
		rect.left = kPlayer.left + (kPlayer.right * (player_count_ - 1));

		Sprite::DrawSprite(texture_, &pos, rect);
	}

	Sprite::GetSprite()->End();
}