/***********
 * Include *
 ***********/
#include "game_base.h"

/******************
 * コンストラクタ *
 ******************/
GameBase::GameBase(VOID)
{
	/* 初期化 */
	for (INT i = 0; i < kAllPlayer; i++)
	{
		player_[i]       = NULL;        // 一旦空にする
		player_[i]       = new Player;  // プレイヤーの情報を取得
		ranking_flag_[i] = FALSE;       // ランキングフラグの初期化
	}

	result_count_ = 0;                       // リザルト画面移行プレイヤー数の初期化
	pause_        = FALSE;                   // ポーズフラグ

	ranking_.GetPlayerCount(player_count_);  // プレイヤー人数の取得

	/* マップファイル読み込み */
	field_[GRASS].SetMapNo(map_.FieldMap1);
	field_[SNOW].SetMapNo(map_.FieldMap2);
	field_[MOUNTAIN].SetMapNo(map_.FieldMap3);
	field_[BEACH].SetMapNo(map_.FieldMap4);

	/* チェックポイントファイル読み込み */
	check_point_[GRASS].SetMapNo(map_.FieldMap1);
	check_point_[SNOW].SetMapNo(map_.FieldMap2);
	check_point_[MOUNTAIN].SetMapNo(map_.FieldMap3);
	check_point_[BEACH].SetMapNo(map_.FieldMap4);
}

/****************
 * デストラクタ *
 ****************/
GameBase::~GameBase(VOID)
{
	/* フィールドの破棄 */
	for (INT i = 0; i < MAX_FIELD; i++)
	{
		check_point_[i].Destroy();  // チェックポイントの破棄
		field_[i].Destroy();        // フィールドの破棄
	}

	/* プレイヤーの破棄 */
	for (INT i = 0; i < TANK_ALL; i++)
	{
		player_[i]->Destroy();  // プレイヤーの破棄
		speed_[i].Destroy(); // スピードの破棄
	}

	time_.Destroy();  // タイムの破棄

	DSound::Pause(bgm_);
}

/***********************************
 * 関数名 : ReSet関数              *
 * 内  容 : 数値の初期化を行います *
 * 引  数 : なし                   *
 * 戻り値 : なし                   *
 ***********************************/
VOID GameBase::ReSet(VOID)
{
	/* プレイヤーのリセット */
	for (INT i = 0; i < player_count_; i++)
	{
		player_[i]->ReSet();       // プレイヤーの初期化
		camera_[i].Init();         // カメラの初期化
		ranking_flag_[i] = FALSE;  // ランキングフラグの初期化

		/* プレイヤーの初期配置 */
		player_[i]->StartPosition(check_point_[field_map_].ReturnStartPos(i));

		/* プレイヤー番号取得 */
		player_[i]->SetPlayerNo(i);
	}

	/* レイアウトのリセット */
	ranking_.ReSet();  // ランキングを初期化
	time_.ReSet();     // タイムを初期化

	/* マップのリセット */
	field_[field_map_].ReSet();        // フィールドの初期化
	check_point_[field_map_].ReSet();  // チェックポイントの初期化

	/* 変数の初期化 */
	pause_        = FALSE;  // ポーズフラグの初期化
	result_count_ = 0;      // リザルト画面移行プレイヤー数の初期化

	/* カウントダウン用 */
	count_      = 0;        // カウント初期化
	count_down_ = 3;        // カウント初期化
}

/*************************************
 * 関数名 : Init関数                 *
 * 内  容 : 初期化処理を行います     *
 * 引  数 : なし                     *
 * 戻り値 : 成功したらTRUEを返します * 
 *************************************/
BOOL GameBase::Init(VOID)
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
			/* エラー */
			ErrorDialog("テクスチャの取得に失敗");
			ret = FALSE;
			break;
		}

		/* ランキングの初期化 */
		if (!ranking_.Init())
		{
			/* エラー */
			ErrorDialog("テクスチャの取得に失敗");
			ret = FALSE;
			break;
		}

	} while (0);

	return ret;
}

/*******************************
 * 関数名 : Destroy関数        *
 * 内  容 : 破棄処理を行います *
 * 引  数 : なし               *
 * 戻り値 : なし               *
 *******************************/
VOID GameBase::Destroy(VOID)
{
	time_.Destroy(); // タイムの破棄

	/* フィールド情報の破棄 */
	for (INT i = 0; i < MAX_FIELD; i++)
	{
		check_point_[i].Destroy();  // チェックポイントの破棄
		field_[i].Destroy();        // フィールドの破棄
	}

	/* プレイヤーの破棄 */
	for (INT i = 0; i < player_count_; i++)
	{
		player_[i]->Destroy();  // プレイヤーの破棄
		speed_[i].Destroy();    // スピードの破棄
	}
}

/*********************************************
 * 関数名 : MapInit関数                      *
 * 内  容 : マップ関係の初期化処理を行います *
 * 引  数 : なし                             *
 * 戻り値 : 初期化に成功したらTRUEを返します *
 *********************************************/
BOOL GameBase::MapInit(VOID)
{
	BOOL ret = TRUE;

	/* フィールド情報の初期化 */
	for (INT i = 0; i < MAX_FIELD; i++)
	{
		/* チェックポイントの初期化 */
		if (!check_point_[i].Init())
		{
			ErrorDialog("チェックポイントの取得に失敗");
			ret = FALSE;
			break;
		}

		/* ファイル番号を取得 */
		field_[i].SetFieldNo(i);

		/* フィールドの初期化 */
		if (!field_[i].Init())
		{
			ErrorDialog("ブロックのメッシュ取得に失敗");
			ret = FALSE;
			break;
		}
	}

	return ret;
}

/*************************************************
 * 関数名 : PlayerInit関数                       *
 * 内  容 : プレイヤー関係の初期化処理を行います *
 * 引  数 : なし                                 *
 * 戻り値 : なし                                 *
 *************************************************/
BOOL GameBase::PlayerInit(VOID)
{
	BOOL ret = TRUE;

	/* プレイヤーの初期化 */
	for (INT i = 0; i < kAllPlayer; i++)
	{
		camera_[i].Init();           // カメラの初期化

		player_[i]->SetPlayerNo(i);  // プレイヤー番号の取得

		/* プレイヤーの初期配置 */
		player_[i]->StartPosition(check_point_[field_map_].ReturnStartPos(i));

		/* プレイヤーの初期化 */
		if (!player_[i]->Init())
		{
			/* エラー */
			ErrorDialog("プレイヤーのメッシュ取得に失敗");
			ret = FALSE;
			break;
		}

		/* スピードの初期化 */
		if (!speed_[i].Init())
		{
			/* エラー */
			ErrorDialog("テクスチャの取得に失敗");
			ret = FALSE;
			break;
		}
	}

	return ret;
}

/*************************************************
 * 関数名 : DrawPlayer関数                       *
 * 内  容 : プレイヤー同士の描画範囲を設定します *
 * 引  数 : INT player プレイヤー番号            *
 * 戻り値 : なし                                 *
 *************************************************/
VOID GameBase::DrawPlayer(INT player)
{
	/* プレイヤーの描画範囲を設定 */
	for (INT i = 0; i < player_count_; i++)
	{
		/* プレイヤー同士が範囲内にいるのなら */
		if (D3DXVec3Length(&(D3DXVECTOR3(player_[player]->ReturnDrawPosition() - player_[i]->ReturnPosition()))) < kFieldDraw)
		{
			player_[i]->Draw();  // そのプレイヤーの画面に他のプレイヤーを描画する
		}
	}
}

/***********************************************
 * 関数名 : CountDown関数                      *
 * 内  容 : カウントダウンを行います           *
 * 引  数 : なし                               *
 * 戻り値 : カウントが終了したらTRUEを返します *
 ***********************************************/
BOOL GameBase::CountDowm(VOID)
{
	/* カウントが指定数に達したら */
	if (count_++ >= 180)
	{
		/* カウントリセット */
		count_ = 0;

		/* 秒がまだ残っているなら */
		if (count_down_ > 0)
		{
			count_down_--;  // 秒を下げる
		}
	}

	/* 描画無くなったらTRUEを返す */
	return (count_down_ <= 0) ? TRUE : FALSE;
}

/*****************************************
 * 関数名 : Pause関数                    *
 * 内  容 : ゲーム中の一時停止用関数     *
 * 引  数 : なし                         *
 * 戻り値 : 一時停止を行うならTRUEを返す *
 *****************************************/
BOOL GameBase::Pause(INT player_no)
{
	/* スタートボタンが押されたら更新を止める */
	if (time_.ReturnFlag() && (!pause_ && (push_button_[player_no] == XINPUT_GAMEPAD_START)))
	{
		DSound::Pause(bgm_);  // BGMの停止
		pause_ = TRUE;        // ポーズフラグをオンにする
	}
	/* ポーズフラグがオンの時に押されたら */
	else if (push_button_[player_no] == XINPUT_GAMEPAD_START)
	{
		DSound::Play(bgm_);  // BGMの再生
		pause_ = FALSE;      // ポーズフラグをオフにする
	}

	/* ポーズフラグを返す */
	return pause_;
}

/*******************************************
 * 関数名 : GameSet関数                    *
 * 内  容 : ゲーム中の情報の更新を行います *
 * 引  数 : INT player  プレイヤー番号     *
 * 戻り値 : なし                           *
 *******************************************/
VOID GameBase::GameSet(INT player)
{
	/* マップごとのBGM設定 */
	switch (field_map_)
	{
		case GRASS    : bgm_ = GRASS_BGM;    break;
		case SNOW     : bgm_ = SNOW_BGM;     break;
		case MOUNTAIN : bgm_ = MOUNTAIN_BGM; break;
		case BEACH    : bgm_ = BEACH_BGM;    break;
		default:break;
	}

	/* BGMの再生 */
	DSound::Play(bgm_, DSBPLAY_LOOPING);

	/* プレイヤーの設定 */
	player_[player]->SetPlayerColor(RED_BLOCK + player);
	player_[player]->SetPlayerNo(player);

	/* フィールドとの当たり判定の更新 */
	field_[field_map_].SetPlayerNo(player);
	field_[field_map_].SetOriginPos(player_[player]->ReturnOrigin());
	field_[field_map_].SetPlayerPos(player_[player]->ReturnPosition());
	field_[field_map_].SetDrawPos(player_[player]->ReturnDrawPosition());
	field_[field_map_].SetPlayerRadius(player_[player]->ReturnPlayerRadius());
	field_[field_map_].SetBulletRadius(player_[player]->ReturnBulletRadius());

	// フィールドの更新*/
	field_[field_map_].UpDate();

	/* カメラの設定 */
	camera_[player].SetPosition(player_[player]->ReturnPosition());
	camera_[player].Direction(player_[player]->ReturnDirection());

	/* スピードメーターの設定 */
	speed_[player].SetPlayerSpeed(player_[player]->ReturnMove());
	speed_[player].UpDate();
}

/***************************************
 * 関数名 : GameUpDate関数             *
 * 内  容 : ゲーム中の更新を行います   *
 * 引  数 : INT player  プレイヤー番号 *
 * 戻り値 : なし                       *
 ***************************************/
VOID GameBase::GameUpDate(INT player)
{
	/* プレイヤーの更新 */
	player_[player]->BlockCheck(field_[field_map_].BlockCheck());  // プレイヤーの当たっているブロックの判定
	player_[player]->UpDate();                                     // プレイヤーの更新
	
	for (INT i = 0; i < kAllPlayer; i++)
	{
		if (!(i == player) && 
			collision_.SphereHitCheck(
			player_[player]->ReturnPosition(),
			player_[i]->ReturnPosition(),
			player_[player]->ReturnPlayerRadius(),
			player_[i]->ReturnBulletRadius()))
		{
			player_[player]->PlayerHit();
		}
	}

	player_[player]->ReturnPosition(check_point_[field_map_].ReturnPosition(player));

	/* 弾の判定を行う */
	field_[field_map_].SetBulletPos(player_[player]->ReturnBullet());
	field_[field_map_].SetOriginBullet(player_[player]->ReturnOriginBullet());
	if (field_[field_map_].BulletCheck(RED_BLOCK + player)) player_[player]->BulletFlag();

	/* チェックポイントの更新 */
	check_point_[field_map_].SetPlayerNo(player);
	check_point_[field_map_].SetPlayerOrigin(player_[player]->ReturnOrigin());
	check_point_[field_map_].SetPlayerPosition(player_[player]->ReturnPosition());
	check_point_[field_map_].RankingSort(player, player_count_);

	// チェックポイントの更新
	check_point_[field_map_].UpDate(player);

	/* ゴール判定 */
	if (check_point_[field_map_].Goal(player))
	{
		GameResult(player);
	}
}

/*****************************************************
 * 関数名 : GameResult関数                           *
 * 内  容 : ゲーム中のマップでのリザルトを表示します *
 * 引  数 : INT player  プレイヤー番号               *
 * 戻り値 : なし                                     *
 *****************************************************/
VOID GameBase::GameResult(INT player)
{
	/* ランキングフラグがTRUEになっていたら */
	if (!ranking_flag_[player])
	{
		/* ゴールSEの再生 */
		DSound::Restart(GOAL_SE);
		DSound::Play(GOAL_SE);

		/* ランキングの更新 */
		ranking_.UpDate(player, check_point_[field_map_].ReturnRanking(player));
		ranking_flag_[player] = TRUE;  // フラグをTRUEにする

		/* リザルトカウントを増やす */
		result_count_++;
		count_down_ = 3;
	}
}

/***************************************************
 * 関数名 : NextStage関数                          *
 * 内  容 : 次のマップへ移動する間の処理を行います *
 * 引  数 : なし                                   *
 * 戻り値 : なし                                   *
 ***************************************************/
BOOL GameBase::NextStage(VOID)
{
	/* リザルトカウントがプレイヤー数に達したら */
	if (result_count_ >= player_count_)
	{
		/* カウントフラグがオンになったら */
		if (CountDowm())
		{
			/* プレイヤー全員 */
			for (INT i = 0; i < player_count_; i++)
			{
				ranking_flag_[i] = FALSE;  // ランキングフラグをオフにする
				total_point_[i] += ranking_.ReturnPoint(i);  // ポイントをトータルに加算する
			}

			/* フィールド番号を増やす */
			field_map_++;

			/* BGMを止める */
			DSound::Pause(bgm_);

			/* フィールドを全部回ったなら */
			if (field_map_ >= MAX_FIELD) return ENDING;  // エンディングに移行
			else ReSet();  // まだの場合は状態を初期化する
		}
	}

	return MULTI_GAME;
}


