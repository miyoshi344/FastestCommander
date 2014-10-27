/**********************
 * エンディングクラス *
 **********************/

/* 多重Include防止 */
#ifndef RACE_ENDING_H_
#define RACE_ENDING_H_

/***********
 * Include *
 ***********/
#include "flow_base.h"
#include "layout_base.h"
#include "demoplay.h"

/********
 * 定数 *
 ********/
CONST D3DXVECTOR3 kEnding = D3DXVECTOR3(400, 110.0F, 0.0F);  // エンディング画面描画位置

CONST LPCSTR Result = "result.png";

/********************************************************
 * エンディングの管理を行うクラスです                   *
 * FlowBaseクラスとLayoutBaseクラスを継承して作成します *
 ********************************************************/
class Ending : public FlowBase, public LayoutBase
{
private:

	DemoPlay demo_play_;

	LPDIRECT3DTEXTURE9 texture_; // テクスチャ

	/* 変数 */
	INT total_point_[kAllPlayer];    // プレイヤーの総合ポイント
	INT total_ranking_[kAllPlayer];  // プレイヤーの総合ランキング

	/* 内部関数 */
	VOID RankingSort(INT player_no);  // 順位をソート
	VOID RankingDraw(VOID);           // ランキングの描画
	VOID SpriteDraw(VOID);            // スプライト描画処理

public:
	Ending(VOID);           // コンストラクタ
	~Ending(VOID);          // デストラクタ

	BOOL Init(VOID);        // 初期設定
	VOID ReSet(VOID);       // 初期化処理
	INT  UpDate(VOID);      // 更新処理
	VOID Draw(VOID);        // 描画処理
	VOID Destroy(VOID);     // 破棄処理


};

#endif