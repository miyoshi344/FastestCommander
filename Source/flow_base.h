/**********************
 * 状態遷移基礎クラス *
 **********************/

/* 多重Include防止 */
#ifndef RACE_FLOW_BASE_H_
#define RACE_FLOW_BASE_H_

/***********
 * Include *
 ***********/
#include "common.h"
#include "sprite.h"
#include "texture.h"
#include "xinput.h"

/********
 * 列挙 *
 ********/

/* 遷移用列挙 */
enum
{
	TITLE,       // タイトル
	SOLO_GAME,   // ソロゲーム
	MULTI_GAME,  // マルチゲーム
	ENDING,      // エンディング
	GAME_OVER,   // ゲームオーバー
	MAX_FLOW,    // 遷移最大数
};

/**********************************
 * 状態遷移の基礎クラスになります *
 **********************************/
class FlowBase
{
protected:

	/************
	 * 静的変数 *
	 ************/
	static INT player_count_;             // プレイヤー数
	static INT field_map_;                // フィールド番号
	static INT total_point_[kAllPlayer];  // プレイヤーごとのスコアの合計値

	/********
	 * 変数 *
	 ********/
	ULONG push_stick_[kMaxPad];   // スティック入力保存用
	ULONG push_button_[kMaxPad];  // ボタン入力保存用

	/************
	 * 内部関数 *
	 ************/
	virtual	VOID SpriteDraw(VOID) = 0;  // スプライト描画処理

public:
	FlowBase(VOID);   // コンストラクタ
	~FlowBase(VOID);  // デストラクタ

	virtual BOOL Init(VOID)       = 0;  // 初期化処理
	virtual VOID ReSet(VOID)      = 0;  // 数値の初期化
	virtual INT  UpDate(VOID)     = 0;  // 更新処理
	virtual VOID Draw(VOID)       = 0;  // 描画処理
	virtual VOID Destroy(VOID)    = 0;  // 破棄処理

	/* スコアを受け渡す関数 */
	INT SetTotalRanking(INT player_no) { return total_point_[player_no]; }

	VOID GetTotalRanking(INT player_no, INT total_point) { total_point_[player_no] = total_point; }
};

#endif