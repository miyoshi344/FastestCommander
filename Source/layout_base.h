/****************************
* レイアウトのコモンクラス *
**************************** /

/* 多重Include防止 */
#ifndef RACE_LAYOUT_H_
#define RACE_LAYOUT_H_

/***********
* Include *
***********/
#include "common.h"
#include "objectbase.h"

/********
* 列挙 *
********/

/* 文字の大きさ */
enum
{
	SMALL,   // 小さい文字
	MIDDLE,  // 中の文字
	LAGE,    // 大きい文字
};

/* 文字カラー */
enum
{
	RED,     // 赤
	GREEN,   // 緑
	YELLOW,  // 黄
	BLUE,    // 青
};

/********
 * 定数 *
 ********/
CONST LPCSTR kScore  = "score.png";
CONST LPCSTR kLayout = "Layout.png";

/* 数字用矩形 */
CONST RECT SmallScore  = { 0, 160, 25, 25 };
CONST RECT MiddleScore = { 0, 268, 50, 60 };
CONST RECT LageScore   = { 0, 519, 80, 100 };

/* 切り取り矩形 */
CONST RECT kStartRect    = {   0,  85,  425,  75 };  // スタートボタン
CONST RECT kPlayer       = {   0,   0,  200,  80 };  // プレイヤー人数選択
CONST RECT kTitleRect    = {   0, 280, 1000, 150 };  // タイトル

CONST RECT kLapRect      = {   0,  50,   90,  40 };  // ラップ文字
CONST RECT kBarRect      = { 600, 268,   50,  60 };  // バー
CONST RECT kRankRect     = { 600, 212,   60,  28 };  // ランク
CONST RECT kRankingRect  = {   0,  0,   70,  38 };  // ランキング

CONST RECT kCheckRect    = { 700, 350,  350, 100 };  // チェックポイント
CONST RECT kStageRect    = { 450, 338,  200,  95 };  // ステージ数

CONST RECT kTotalRanking = {   0,  940, 330,  60 };  // 合計順位
CONST RECT kTotalScore   = {   0, 1010, 330,  50 };  // 合計スコア
CONST RECT kRaceRanking  = {   0, 1060, 330,  59 };  // レース中ランキング
CONST RECT kGetPoint     = {   0, 1120, 330,  60 };  // レース中ポイント

CONST RECT kPoint        = { 300,   0,  69,  40 };  // PT文字
CONST RECT MeterRect     = {   0,   100, 150, 60 };  // km/h表示


/**************************
* レイアウト用基礎クラス *
**************************/
class LayoutBase
{
protected:

	/********
	 * 変数 *
	 ********/
	LPDIRECT3DTEXTURE9 texture_;   // テクスチャ
	LPDIRECT3DTEXTURE9 texture2_;  // テクスチャ

	INT player_no_;

	/************
	 * 内部関数 *
	 ************/

	/* 描画用関数 */
	VOID CountSet(D3DXVECTOR3 pos, INT count = 0, INT size = SMALL, INT color = RED, INT loop = 2);
	VOID DrawSet(D3DXVECTOR3 pos, RECT rect, INT player_no = 0, INT texture = 0);

public:
	LayoutBase(VOID);    // コンストラクタ
	~LayoutBase(VOID);   // デストラクタ

	BOOL Init(VOID);
	VOID Destroy(VOID);  // 破棄

	VOID SetPlayerNo(INT player_no) { player_no_ = player_no; }
};
#endif