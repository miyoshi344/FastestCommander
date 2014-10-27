/********************
 * ランキングクラス *
 ********************/

/* 多重Include防止 */
#ifndef RACE_RANKING_H_
#define RACE_RANKING_H_

/***********
 * Include *
 ***********/
#include "layout_base.h"

/********
 * 定数 *
 ********/
CONST D3DXVECTOR3 kRaceRankingPosition  = D3DXVECTOR3(50.0F, 100.0F, 0.0F);
CONST D3DXVECTOR3 kRacePointPosition    = D3DXVECTOR3(50.0F, 200.0F, 0.0F);

CONST FLOAT kCountPosition = 400.0F;

/****************************************
 * ランキングを管理するクラスです       *
 * LayoutBaseクラスを継承して作成します *
 ****************************************/
class Ranking : public LayoutBase
{
private:

	/********
	 * 変数 *
	 ********/
	INT race_ranking_[kAllPlayer];  // レースランキング
	INT race_point_[kAllPlayer];    // レースポイント
	INT player_count_;              // プレイヤー数

	/************
	 * 内部関数 *
	 ************/
	VOID RaceDraw(INT player_no);  // レース結果描画

public:
	Ranking(VOID);   // コンストラクタ
	~Ranking(VOID);  // デストラクタ
	
	BOOL Init(VOID);   // 初期処理
	VOID ReSet(VOID);  // 初期化処理
	VOID UpDate(INT player_no, INT ranking);  // 更新処理

	VOID SoloDraw(VOID);           // 1人用描画処理
	VOID TwoDraw(INT player_no);   // 2人用描画処理
	VOID MultiDraw(INT player_no); // 描画処理
	VOID Destroy(VOID);            // 破棄処理

	VOID RankingSort(INT player_no);

	VOID GetPlayerCount(INT player) { player_count_ = player; }

	/* 値を返す関数 */
	INT ReturnPoint(INT player_no)   { return race_point_[player_no]; }
};

#endif