/***********
 * Include *
 ***********/
#include "demoplay.h"

/******************
 * コンストラクタ *
 ******************/
DemoPlay::DemoPlay(VOID)
{
	/* 初期化 */
	for (INT i = 0; i < kAllPlayer; i++ )
	{
		player_[i] = NULL;          // 空にする
		player_[i] = new PlayerAI;  // プレイヤーの格納
	}

	player_count_ = 2;      // プレイヤー人数
}

/****************
 * デストラクタ *
 ****************/
DemoPlay::~DemoPlay(VOID)
{
	/* フィールドの解放 */
	for (INT i = 0; i < MAX_FIELD; i++)
	{
		check_point_[i].Destroy();  // チェックポイントの解放
		field_[i].Destroy();        // フィールドの解放
	}

	/* プレイヤーの解放 */
	for (INT i = 0; i < kAllPlayer; i++)
	{
		player_[i]->Destroy();   // プレイヤーの解放
		SafeDelete(player_[i]);  // メモリを解放する
	}
}

/*********************************************
 * 関数名 : Init関数                         *
 * 内  容 : 初期化処理を行う                 *
 * 引  数 : なし                             *
 * 戻り値 : 初期化処理に成功したらTRUEを返す *
 *********************************************/
BOOL DemoPlay::Init(VOID)
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
VOID DemoPlay::ReSet(VOID)
{
	field_map_ = kStartMap;
	field_[field_map_].ReSet();

	/* プレイヤーの初期化 */
	for (INT i = 0; i < kAllPlayer; i++)
	{
		player_[i]->ReSet();  // プレイヤーの初期化
		camera_[i].Init();    // カメラの初期化
		total_point_[i] = 0;  // ポイントの初期化

		/* プレイヤーの初期配置 */
		player_[i]->StartPosition(check_point_[field_map_].ReturnStartPos(i));

		/* プレイヤー番号取得 */
		player_[i]->SetPlayerNo(i);
	}
}

/*******************************
 * 関数名 : UpDate関数         *
 * 内  容 : 更新処理を行います *
 * 引  数 : なし               *
 * 戻り値 : なし               *
 *******************************/
BOOL DemoPlay::UpDate(VOID)
{
	/* ゲーム画面の更新 */
	for (INT i = 0; i < kAllPlayer; i++)
	{
		GameSet(i);
		GameUpDate(i);
	}

	return TRUE;
}

/*******************************
 * 関数名 : Draw関数           *
 * 内  容 : 描画処理を行います *
 * 引  数 : なし               *
 * 戻り値 : なし               *
 *******************************/
VOID DemoPlay::Draw(VOID)
{
	BackGameDraw();  // ゲーム画面の描画
}

/***************************
 * 関数名 : Destroy関数    *
 * 内  容 : 破棄処理を行う *
 * 引  数 : なし           *
 * 戻り値 : なし           *
 ***************************/
VOID DemoPlay::Destroy(VOID)
{
	/* フィールドの解放 */
	for (INT i = 0; i < MAX_FIELD; i++)
	{
		check_point_[i].Destroy();  // チェックポイントの破棄
		field_[i].Destroy();        // フィールドの破棄
	}

	/* プレイヤーの破棄 */
	for (INT i = 0; i < kMaxPad; i++)
	{
		player_[i]->Destroy();   // プレイヤーの破棄
		SafeDelete(player_[i]);  // プレイヤーの破棄
	}
}

/*******************************************
 * 関数名 : GameSet関数                    *
 * 内  容 : ゲーム中の情報の更新を行います *
 * 引  数 : INT player プレイヤー番号      *
 * 戻り値 : なし                           *
 *******************************************/
VOID DemoPlay::GameSet(INT player)
{
	/* プレイヤーの設定 */
	player_[player]->SetPlayerColor(RED_BLOCK + player);  // プレイヤーの色情報
	player_[player]->SetPlayerNo(player);                 // プレイヤー番号

	/* フィールドとの当たり判定の更新 */
	field_[field_map_].SetPlayerNo(player);                                // プレイヤー番号を取得
	field_[field_map_].SetOriginPos(player_[player]->ReturnOrigin());      // プレイヤーの更新前の座標を取得
	field_[field_map_].SetPlayerPos(player_[player]->ReturnPosition());    // プレイヤーの更新後の座標を取得
	field_[field_map_].SetDrawPos(player_[player]->ReturnDrawPosition());  // プレイヤーの描画座標を取得

	/* フィールドの更新 */
	field_[field_map_].UpDate();

	/* カメラの設定 */
	camera_[player].SetPosition(player_[player]->ReturnPosition());  // プレイヤーの座標を取得
	camera_[player].Direction(player_[player]->ReturnDirection());  // プレイヤーの旋回値を取得
}

/***************************************
 * 関数名 : GameUpDate関数             *
 * 内  容 : ゲーム中の更新を行います   *
 * 引  数 : INT player  プレイヤー番号 *
 * 戻り値 : なし                       *
 ***************************************/
VOID DemoPlay::GameUpDate(INT player)
{
	/* プレイヤーの更新 */
	player_[player]->BlockCheck(field_[field_map_].BlockCheck());  // プレイヤーのブロックの判定
	player_[player]->UpDate();                                     // プレイヤーの更新

	/* 弾の判定を行う */
	field_[field_map_].SetOriginBullet(player_[player]->ReturnOriginBullet());              // 弾の更新前の座標を取得
	field_[field_map_].SetBulletPos(player_[player]->ReturnBullet());                       // 弾の更新後の座標を取得
	if (field_[field_map_].BulletCheck(RED_BLOCK + player)) player_[player]->BulletFlag();  // 弾の色の情報を取得

	/* チェックポイントの更新 */
	check_point_[field_map_].SetPlayerNo(player);                                   // プレイヤーの番号を取得
	check_point_[field_map_].SetPlayerOrigin(player_[player]->ReturnOrigin());      // プレイヤーの更新前の座標を取得
	check_point_[field_map_].SetPlayerPosition(player_[player]->ReturnPosition());  // プレイヤーの更新後の座標を取得              

	// チェックポイントの更新
	check_point_[field_map_].UpDate(player);
}

/*******************************
 * 関数名 : BackGameDraw関数   *
 * 内  容 : 描画処理を行います *
 * 引  数 : なし               *
 * 戻り値 : なし               *
 *******************************/
VOID DemoPlay::BackGameDraw(VOID)
{
	/* 画面の描画 */
	camera_[0].UpDate();  // カメラの更新
	//player_[0]->Draw();   // プレイヤーの描画

	/* 画面の更新 */
	field_[field_map_].SetPlayerNo(0);
	field_[field_map_].Draw();
}

/*********************************************
 * 関数名 : MapInit関数                      *
 * 内  容 : マップ関係の初期化処理を行います *
 * 引  数 : なし                             *
 * 戻り値 : 初期化に成功したらTRUEを返します *
 *********************************************/
BOOL DemoPlay::MapInit(VOID)
{
	BOOL ret = TRUE;

	for (INT i = 0; i < MAX_FIELD; i++)
	{
		/* チェックポイントの初期化 */
		if (!check_point_[i].Init())
		{
			/* エラー */
			ErrorDialog("チェックポイントの取得に失敗");
			ret = FALSE;
			break;
		}

		field_[i].SetFieldNo(i);

		/* フィールドの初期化 */
		if (!field_[i].Init())
		{
			/* エラー */
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
BOOL DemoPlay::PlayerInit(VOID)
{
	BOOL ret = TRUE;

	/* プレイヤーの初期化 */
	for (INT i = 0; i < kMaxPad; i++)
	{
		/* プレイヤーの初期配置 */
		player_[i]->StartPosition(check_point_[field_map_].ReturnStartPos(i));

		/* プレイヤー番号取得 */
		player_[i]->SetPlayerNo(i);

		/* プレイヤーの初期化 */
		if (!player_[i]->Init())
		{
			ErrorDialog("プレイヤーのメッシュ取得に失敗");
			ret = FALSE;
			break;
		}
	}

	return ret;
}

VOID DemoPlay::SpriteDraw(VOID)
{

}