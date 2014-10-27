/***********
 * Include *
 ***********/
#include "ending.h"

/******************
 * コンストラクタ *
 ******************/
Ending::Ending(VOID)
{
	/* 初期化 */
	for (INT i = 0; i < kAllPlayer; i++)
	{
		total_point_[i]   = 0;
		total_ranking_[i] = 0;
	}
}

/****************
 * デストラクタ *
 ****************/
Ending::~Ending(VOID)
{
	/* レイアウトの解放 */
	LayoutBase::Destroy();

	demo_play_.Destroy();
}

/*********************************
 * 関数名 : Init関数             *
 * 内  容 : 初期化処理を行います *
 * 引  数 : なし                 *
 * 戻り値 : 成功したらTRUEを返す *
 *********************************/
BOOL Ending::Init(VOID)
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

		/* フォルダ階層を移動 */
		_chdir("Resource_fast/Texture");

		if (!(texture_ = Texture::LoadTexture(Result)))
		{
			/* エラー */
			break;
		}

		/* フォルダ階層を戻す */
		_chdir("../../");

		/* テクスチャ情報の取得 */
		if (!LayoutBase::Init())
		{
			/* エラー */
			ErrorDialog("テクスチャの取得に失敗");
			ret = FALSE;
			break;
		}

		/* 初期化 */
		for (INT i = 0; i < kAllPlayer; i++)
		{
			total_point_[i]   = 0;
			total_ranking_[i] = 0;
		}

	} while (0);

	return ret;
}

/***********************************
 * 関数名 : ReSet関数              *
 * 内  容 : 数値の初期化を行います *
 * 引  数 : なし                   *
 * 戻り値 : なし                   *
 ***********************************/
VOID Ending::ReSet(VOID)
{
	/* ポイントの設定を行う */
	for (INT i = 0; i < player_count_; i++)
	{
		total_point_[i] = SetTotalRanking(i);
	}

	/* デモプレイの状態を初期化する */
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
INT Ending::UpDate(VOID)
{
	/* プレイヤーの数だけ行う */
	for (INT i = 0; i < player_count_; i++)
	{
		RankingSort(i);  // ランキングソート
		push_button_[i] = XInput::ReturnPushButton(i);  // ボタン入力を取得

		/* スタートボタンが押されたら */
		if (push_button_[i] == XINPUT_GAMEPAD_START) return TITLE;
	}

	/* デモプレイ画面の更新 */
	demo_play_.UpDate();

	/* サウンドの再生 */
	DSound::Play(TITLE_BGM, DSBPLAY_LOOPING);

	return ENDING;
}

/*******************************
 * 関数名 : Draw関数           *
 * 内  容 : 描画処理を行います *
 * 引  数 : なし               *
 * 戻り値 : なし               *
 *******************************/
VOID Ending::Draw(VOID)
{
	/* スプライトの描画 */
	SpriteDraw();

	/* デモプレイ画面の描画 */
	demo_play_.Draw();
}

/*******************************
 * 関数名 : Destroy関数        *
 * 内  容 : 破棄処理を行います *
 * 引  数 : なし               *
 * 戻り値 : なし               *
 *******************************/
VOID Ending::Destroy(VOID)
{
	/* レイアウトの解放 */
	LayoutBase::Destroy();

	/* デモ画面の破棄 */
	demo_play_.Destroy();

}

/*****************************************
 * 関数名 : RankingSort関数              *
 * 内  容 : 順位のソートを行います       *
 * 引  数 : INT player_no プレイヤー番号 *
 * 戻り値 : なし                         *
 *****************************************/
VOID Ending::RankingSort(INT player_no)
{
	INT ranking = 0;

	/* 他のプレイヤーとポイントを比較 */
	for (INT i = 0; i < player_count_; i++)
	{
		if (total_point_[player_no] < total_point_[i]) ranking++;  // 相手より少ないなら順位を上げる
	}

	/* ランキングを格納 */
	total_ranking_[player_no] = ranking;
}

/*******************************************
 * 関数名 : SpriteDraw関数                 *
 * 内  容 : スプライトの描画処理を行います *
 * 引  数 : なし                           *
 * 戻り値 : なし                           *
 *******************************************/
VOID Ending::SpriteDraw(VOID)
{
	RECT rect;

	rect.left = 0;
	rect.top = 0;
	rect.right = 1280;
	rect.bottom = 720;

	Sprite::GetSprite()->Begin(D3DXSPRITE_ALPHABLEND);

	Sprite::DrawSprite(texture_, &D3DXVECTOR3(0.0F, 0.0F, 0.0F),rect);

	RankingDraw();  // ランキングの描画

	Sprite::GetSprite()->End();
}

/*******************************************
 * 関数名 : RankingDraw関数                *
 * 内  容 : ランキングの描画処理を行います *
 * 引  数 : なし                           *
 * 戻り値 : なし                           *
 *******************************************/
VOID Ending::RankingDraw(VOID)
{
	/* 初期座標の設定 */
	D3DXVECTOR3 pos = kEnding;

	/* プレイヤーの数だけ行う */
	for (INT i = 0; i < player_count_; i++)
	{
		INT player_no = total_ranking_[i];

		/* プレイヤー番号の描画 */
		DrawSet(pos, kPlayer, i, 1);

		/* 座標の移動 */
		pos.x += 230.0F;
		pos.y += 20.0F;

		/* 順位の描画 */
		CountSet(pos, i, MIDDLE, i, 1);

		/* 座標の移動 */
		pos.x += 75.0F;
		pos.y += 20.0F;

		/* 文字の描画 */
		DrawSet(pos, kRankingRect, i);

		/* 座標の移動 */
		pos.x -= 275.0F;
		pos.y += 40.0F;

		/* ポイントの描画 */
		CountSet(pos, total_point_[total_ranking_[i]], MIDDLE, i, 2);

		/* 座標を移動 */
		pos.x += 100.0F;

		/* 文字の描画 */
		DrawSet(pos, kTotalScore, i);

		/* 座標の移動 */
		pos.x = kEnding.x;
		pos.y += 60.0F;
	}
}