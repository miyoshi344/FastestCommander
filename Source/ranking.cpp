/***********
 * Include *
 ***********/
#include "ranking.h"

/******************
 * コンストラクタ *
 ******************/
Ranking::Ranking(VOID)
{
	/* スコアの初期化 */
	for (INT i = 0; i < kAllPlayer; i++)
	{
		race_point_[i]    = 0;
		race_ranking_[i]  = 0;
	}
}

/****************
 * デストラクタ *
 ****************/
Ranking::~Ranking(VOID)
{
	/* レイアウトの破棄 */
	LayoutBase::Destroy();
}

/*******************************
 * 関数名 : Init関数           *
 * 内  容 : 初期処理を行います *
 * 引  数 : なし               *
 * 戻り値 : なし               *
 *******************************/
BOOL Ranking::Init(VOID)
{
	BOOL ret = TRUE;

	do
	{
		/* レイアウトの初期化 */
		if (!LayoutBase::Init())
		{
			/* エラー */
			ErrorDialog("テクスチャの読み込みに失敗");
			ret = FALSE;
			break;
		}

	} while (0);

	return ret;
}

/*****************************
 * 関数名 : ReSet関数        *
 * 内  容 : 初期化を行います *
 * 引  数 : なし             *
 * 戻り値 : なし             *
 *****************************/
VOID Ranking::ReSet(VOID)
{
	/* スコアの初期化 */
	for (INT i = 0; i < kAllPlayer; i++)
	{
		race_point_[i]    = 0;
		race_ranking_[i]  = 0;
	}
}

/********************************************
 * 関数名 : UpDate関数                      *
 * 内  容 : 更新処理を行います              *
 * 引  数 : INT player_no  プレイヤー番号   *
 *          INT ranking    プレイヤーの順位 *
 * 戻り値 : なし                            *
 ********************************************/
VOID Ranking::UpDate(INT player_no, INT ranking)
{
	/* 順位によって数字を格納 */
	switch (ranking)
	{
		case 1: race_point_[player_no] = 10; break;
		case 2: race_point_[player_no] = 8; break;
		case 3: race_point_[player_no] = 6; break;
		case 4: race_point_[player_no] = 4; break;
		default:break;
	}

	/* 順位を格納 */
	race_ranking_[player_no] = ranking;
}

/*********************************************
 * 関数名 : SoloDraw関数                     *
 * 内  容 : レースの結果のスコアを描画します *
 * 引  数 : なし                             *
 * 戻り値 : なし                             *
 *********************************************/
VOID Ranking::SoloDraw(VOID)
{
}

/*********************************************
 * 関数名 : TwoDraw関数                      *
 * 内  容 : レースの結果のスコアを描画します *
 * 引  数 : INT player_no プレイヤー番号     *
 * 戻り値 : なし                             *
 *********************************************/
VOID Ranking::TwoDraw(INT player_no)
{
	/* ランキング座標 */
	D3DXVECTOR3 race_ranking[2] =
	{
		D3DXVECTOR3(kRaceRankingPosition.x, kRaceRankingPosition.y, kRaceRankingPosition.z),  // プレイヤー1
		D3DXVECTOR3(kRaceRankingPosition.x, kRaceRankingPosition.y + kMultiWindow.y, kRaceRankingPosition.z),  // プレイヤー2
	};

	/* スコア座標 */
	D3DXVECTOR3 race_point[2] =
	{
		D3DXVECTOR3(kRacePointPosition.x, kRacePointPosition.y                 , kRacePointPosition.z),  // プレイヤー1
		D3DXVECTOR3(kRacePointPosition.x, kRacePointPosition.y + kMultiWindow.y, kRacePointPosition.z),  // プレイヤー3
	};

	/* 配置用座標 */
	D3DXVECTOR3 pos;

	/* 数字の座標を設定 */
	pos = race_ranking[player_no];
	pos.x += kCountPosition;

	/* レースランキングを描画 */
	DrawSet(race_ranking[player_no], kRaceRanking, player_no);

	/* ランキングを描画 */
	CountSet(pos, race_ranking_[player_no], MIDDLE, player_no, 1);

	pos.x += 50.0F;
	pos.y += 15.0F;

	DrawSet(pos, kRankingRect, player_no);

	/* 数字の座標を設定 */
	pos = race_point[player_no];
	pos.x += kCountPosition;

	/* レースポイントを描画 */
	DrawSet(race_point[player_no], kGetPoint, player_no);

	/* ポイントを描画 */
	CountSet(pos, race_point_[player_no], MIDDLE, player_no);

	pos.x += 50.0F;
	pos.y += 15.0F;
	DrawSet(pos, kPoint, player_no);
}

/***********************************************
 * 関数名 : MultiDraw関数                      *
 * 内  容 : レース結果のスコアの描画を行います *
 * 引  数 : INT player_no プレイヤー番号       *
 * 戻り値 : なし                               *
 ***********************************************/
VOID Ranking::MultiDraw(INT player_no)
{
	/* ランキング座標 */
	D3DXVECTOR3 race_ranking[kAllPlayer] =
	{
		D3DXVECTOR3(kRaceRankingPosition.x,                  kRaceRankingPosition.y,                  kRaceRankingPosition.z),  // プレイヤー1
		D3DXVECTOR3(kRaceRankingPosition.x + kMultiWindow.x, kRaceRankingPosition.y,                  kRaceRankingPosition.z),  // プレイヤー2
		D3DXVECTOR3(kRaceRankingPosition.x,                  kRaceRankingPosition.y + kMultiWindow.y, kRaceRankingPosition.z),  // プレイヤー3
		D3DXVECTOR3(kRaceRankingPosition.x + kMultiWindow.x, kRaceRankingPosition.y + kMultiWindow.y, kRaceRankingPosition.z)   // プレイヤー4
	};

	/* トータルスコア座標 */
	D3DXVECTOR3 race_point[kAllPlayer] =
	{
		D3DXVECTOR3(kRacePointPosition.x,                  kRacePointPosition.y,                  kRacePointPosition.z),  // プレイヤー1
		D3DXVECTOR3(kRacePointPosition.x + kMultiWindow.x, kRacePointPosition.y,                  kRacePointPosition.z),  // プレイヤー2
		D3DXVECTOR3(kRacePointPosition.x,                  kRacePointPosition.y + kMultiWindow.y, kRacePointPosition.z),  // プレイヤー3
		D3DXVECTOR3(kRacePointPosition.x + kMultiWindow.x, kRacePointPosition.y + kMultiWindow.y, kRacePointPosition.z),  // プレイヤー4
	};

	/* 配置用座標 */
	D3DXVECTOR3 pos;

	/* 数字の座標を設定 */
	pos = race_ranking[player_no];
	pos.x += kCountPosition;

	/* レースランキングを描画 */
	DrawSet(race_ranking[player_no], kRaceRanking, player_no);

	/* ランキングを描画 */
	CountSet(pos, race_ranking_[player_no], MIDDLE, player_no, 1);
	
	pos.x += 50.0F;
	pos.y += 15.0F;

	DrawSet(pos, kRankingRect, player_no);

	/* 数字の座標を設定 */
	pos = race_point[player_no];
	pos.x += kCountPosition;

	/* レースポイントを描画 */
	DrawSet(race_point[player_no], kGetPoint, player_no);

	/* ポイントを描画 */
	CountSet(pos, race_point_[player_no], MIDDLE, player_no);

	pos.x += 50.0F;
	pos.y += 15.0F;
	DrawSet(pos, kPoint, player_no);
}

/*******************************
 * 関数名 : Destroy関数        *
 * 内  容 : 破棄処理を行います *
 * 引  数 : なし               *
 * 戻り値 : なし               *
 *******************************/
VOID Ranking::Destroy(VOID)
{
	/* レイアウト破棄 */
	LayoutBase::Destroy();
}