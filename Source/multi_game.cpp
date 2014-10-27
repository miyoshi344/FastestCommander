/***********
* Include *
***********/
#include "multi_game.h"

/******************
* コンストラクタ *
******************/
MultiGame::MultiGame(VOID)
{
}

/****************
* デストラクタ *
****************/
MultiGame::~MultiGame(VOID)
{
}

/*****************************
* 関数名 : Init関数         *
* 内  容 : 初期化を行います *
* 引  数 : なし             *
* 戻り値 : なし             *
*****************************/
BOOL MultiGame::Init(VOID)
{
	BOOL ret = TRUE;

	do
	{
		/* マップの初期化 */
		if (!MapInit())
		{
			/* エラー */
			ErrorDialog("マップの初期化に失敗しました");
			ret = FALSE;
			break;
		}

		/* プレイヤーの初期化 */
		if (!PlayerInit())
		{
			/* エラー */
			ErrorDialog("プレイヤーの初期化に失敗しました");
			ret = FALSE;
			break;
		}

		/* タイムの初期化 */
		if (!time_.Init())
		{
			ErrorDialog("テクスチャの取得に失敗");
			ret = FALSE;
			break;
		}

		if (!ranking_.Init())
		{
			ErrorDialog("テクスチャの取得に失敗");
			ret = FALSE;
			break;
		}

		if (!TextureInit())
		{
			/* エラー */
			ErrorDialog("縁の取得に失敗");
			ret = FALSE;
			break;
		}

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
VOID MultiGame::ReSet(VOID)
{
	/* プレイヤーのリセット */
	for (INT i = 0; i < player_count_; i++)
	{
		player_[i]->ReSet();       // プレイヤー初期化
		camera_[i].Init();         // カメラ初期化
		ranking_flag_[i] = FALSE;  // ランキングフラグの設定オフ

		/* プレイヤーの初期配置 */
		player_[i]->StartPosition(check_point_[field_map_].ReturnStartPos(i));

		/* プレイヤー番号取得 */
		player_[i]->SetPlayerNo(i);
	}

	/* BGMの再生位置初期化 */
	for (INT i = GRASS_BGM; i < BEACH_BGM + 1; i++)
	{
		DSound::Restart(i);  // 再生位置の初期化
	}

	/* ビューの設定 */
	ViewPort();

	/* 状態の初期化 */
	ranking_.ReSet();  // ランキングの初期化
	time_.ReSet();     // タイムの初期化

	/* フィールドの初期化 */
	field_[field_map_].ReSet();        // フィールド初期化
	check_point_[field_map_].ReSet();  // チェックポイント初期化

	/* 変数初期化 */
	pause_        = FALSE;  // ポーズフラグの設定オフ
	result_count_ = 0;      // リザルトカウントを0
}

/*******************************
 * 関数名 : UpDate関数         *
 * 内  容 : 更新処理を行います *
 * 引  数 : なし               *
 * 戻り値 : なし               *
 *******************************/
INT MultiGame::UpDate(VOID)
{
	/* プレイヤーの更新 */
	for (INT i = 0; i < player_count_; i++)
	{
		/* 入力情報の取得 */
		push_button_[i] = XInput::ReturnPushButton(i);

		/* ポーズが実行されていなければ実行 */
		if (!Pause(i))
		{
			/* 情報の更新 */
			GameSet(i);

			/* プレイヤーの更新 */
			if (time_.ReturnFlag()) GameUpDate(i);
		}
		/* ポーズ中にバックボタンが押されたらタイトルに戻る */
		else if (pause_ && (push_button_[i] == XINPUT_GAMEPAD_BACK)) return TITLE;
	}

	/* ステージの処理 */
	return NextStage();
}

/*******************************
 * 関数名 : Draw関数           *
 * 内  容 : 描画処理を行います *
 * 引  数 : なし               *
 * 戻り値 : なし               *
 *******************************/
VOID MultiGame::Draw(VOID)
{
	/* ビューポートを確保しておく */
	D3DVIEWPORT9 view;
	D3DDEV->GetViewport(&view);

	/* ゲーム画面の描画 */
	MultiDraw();

	//collision_.Draw();

	/* ビューを戻す */
	D3DDEV->SetViewport(&view);

	/* スプライトの描画 */
	SpriteDraw();
}

/*********************************************
 * 関数名 : TextureInit関数                  *
 * 内  容 : テクスチャの初期化を行います     *
 * 引  数 : なし                             *
 * 戻り値 : 初期化に成功したらTRUEを返します *
 *********************************************/
BOOL MultiGame::TextureInit(VOID)
{
	BOOL ret = TRUE;

	/* フォルダ階層を移動 */
	_chdir("Resource_fast/Texture");

	do
	{
		/* テクスチャインターフェイスの取得 */
		if (!(player2_edge_ = Texture::LoadTexture(k2PlayEdge)))
		{
			/* エラー */
			ErrorDialog("枠縁の読み込みに失敗しました");
			ret = FALSE;
			break;
		}

		/* テクスチャインターフェイスの取得 */
		if (!(player4_edge_ = Texture::LoadTexture(k4PlayEdge)))
		{
			/* エラー */
			ErrorDialog("枠縁の読み込みに失敗しました");
			ret = FALSE;
			break;
		}

	} while (0);

	/* フォルダ階層を戻す */
	_chdir("../../");

	return ret;
}

/*************************************
 * 関数名 : ViewPort関数             *
 * 内  容 : 画面の分割設定を行います *
 * 引  数 : なし                     *
 * 戻り値 : なし                     *
 *************************************/
VOID MultiGame::ViewPort(VOID)
{
	/* 選択されたプレイヤーが2人以上なら */
	if (player_count_ > 2)
	{
		/* 画面を4分割する */
		port_[0] = {                0,                 0, kWindowWidth / 2, kWindowHeight / 2, 0.0f, 1.0f };
		port_[1] = { kWindowWidth / 2,                 0, kWindowWidth / 2, kWindowHeight / 2, 0.0f, 1.0f };
		port_[2] = {                0, kWindowHeight / 2, kWindowWidth / 2, kWindowHeight / 2, 0.0f, 1.0f };
		port_[3] = { kWindowWidth / 2, kWindowHeight / 2, kWindowWidth / 2, kWindowHeight / 2, 0.0f, 1.0f };

		/* アスペクト比の設定 */
		aspect_ = kMultiWindow;
	}
	/* 2人なら */
	else
	{
		/* 画面を2分割する */
		port_[0] = { 0,                     0, kWindowWidth, (DWORD)kMultiWindow.y, 0.0f, 1.0f };
		port_[1] = { 0, (DWORD)kMultiWindow.y, kWindowWidth, (DWORD)kMultiWindow.y, 0.0f, 1.0f };

		/* アスペクト比の設定 */
		aspect_ = D3DXVECTOR3((FLOAT)kWindowWidth, kMultiWindow.y, 0.0F);
	}
}

/***************************************
 * 関数名 : MultiDraw関数              *
 * 内  容 : 多人数用の画面を描画します *
 * 引  数 : なし                       *
 * 戻り値 : なし                       *
 ***************************************/
VOID MultiGame::MultiDraw(VOID)
{
	time_.TimeCount();

	/* プレイヤー人数分分割する */
	for (int i = 0; i < player_count_; i++)
	{
		//描画領域を変更（ビューポート行列）
		D3DDEV->SetViewport(&port_[i]);
		D3DDEV->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0F, 0);

		/* カメラのアスペクト比設定 */
		camera_[i].Aspect(aspect_);

		//カメラの座標を変更（ビュー行列）
		camera_[i].UpDate();

		/* 画面の更新 */
		field_[field_map_].SetPlayerNo(i);  // プレイヤー番号の取得
		field_[field_map_].Draw();          // フィールドの描画

		/* プレイヤーの描画 */
		DrawPlayer(i);

		/* チェックポイントの判定を表示 */
		check_point_[field_map_].Draw(i);

		/* スプライトの描画 */
		Sprite::GetSprite()->Begin(D3DXSPRITE_ALPHABLEND);

		/* ランキングフラグが立っていないなら */
		if (!ranking_flag_[i])
		{
			/* カウントダウンが終わったら */
			if (time_.ReturnFlag())
			{
				/* プレイヤー数が2人以上なら */
				if (player_count_ > 2)
				{
					speed_[i].MultiDraw(i);                // 4人用スピードの描画
					check_point_[field_map_].MultiDraw(i); // チェックポイントの描画
				}
				/* 2人なら */
				else
				{
					speed_[i].TwoDraw(i);                // 2人用スピードの描画 
					check_point_[field_map_].TwoDraw(i); // 周回数の描画
				}
			}
		}
		/* ランキングフラグが立っていたら */
		else
		{
			/* プレイヤー数が2人以上なら */
			if (player_count_ > 2) ranking_.MultiDraw(i);  // 4人用ランキングの描画

			/* 2人なら */
			else ranking_.TwoDraw(i);                      // 2人用ランキングの描画
		}

		Sprite::GetSprite()->End();
	}
}

/*****************************************
 * 関数名 : SpriteDraw関数               *
 * 内  容 : スプライト描画処理を行います *
 * 引  数 : なし                         *
 * 戻り値 : なし                         *
 *****************************************/
VOID MultiGame::SpriteDraw(VOID)
{
	Sprite::GetSprite()->Begin(D3DXSPRITE_ALPHABLEND);

	/* プレイヤーが2人以上なら */
	if (player_count_ > 2) Sprite::DrawSprite(player4_edge_);  // 4人用フレームの描画

	/* プレイヤーが2人なら */ 
	else Sprite::DrawSprite(player2_edge_);                    // 2人用フレームの描画

	/* タイムカウントの描画を行う */
	if (!time_.ReturnFlag()) time_.TimeCountDraw();

	Sprite::GetSprite()->End();
}
