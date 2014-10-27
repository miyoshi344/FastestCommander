/**********************
 * ゲームの基礎クラス *
 **********************/

/* 多重Include防止 */
#ifndef RACE_GAME_BASE_H_
#define RACE_GAME_BASE_H_

/***********
 * Include *
 ***********/
#include "flow_base.h"
#include "directsound.h"
#include "field.h"
#include "character_base.h"
#include "player.h"
#include "player_ai.h"
#include "collision.h"
#include "camera.h"
#include "time.h"
#include "speed.h"
#include "checkPoint.h"
#include "camera.h"
#include "ranking.h"

/********
 * 定数 *
 ********/

/**********
 * 構造体 *
 **********/
CONST struct FieldMap
{
	LPCSTR FieldMap1 = "Resource_fast/Csv/Map1";  // マップ1のファイル保管場所
	LPCSTR FieldMap2 = "Resource_fast/Csv/Map2";  // マップ2のファイル保管場所
	LPCSTR FieldMap3 = "Resource_fast/Csv/Map3";  // マップ3のファイル保管場所
	LPCSTR FieldMap4 = "Resource_fast/Csv/Map4";  // マップ4のファイル保管場所
};

/******************************************
* ゲームのメイン部分を管理するクラスです *
******************************************/
class GameBase : public FlowBase
{
protected:

	/**************
	 * クラス定義 *
	 **************/

	/* フィールド関係 */
	FieldMap       map_;                     // マップのファイル保管場所
	Collision      collision_;               // 当たり判定クラス
	Field          field_[MAX_FIELD];        // フィールドクラス
	CheckPoint     check_point_[MAX_FIELD];  // チェックポイントクラス

	/* キャラクター関係 */
	CharacterBase* player_[kAllPlayer];  // プレイヤークラス
	Camera         camera_[kAllPlayer];  // カメラクラス

	/* レイアウト関係 */
	Speed          speed_[kAllPlayer];   // スピードカウンタクラス
	Time           time_;                // タイムクラス
	Ranking        ranking_;             // ランキングクラス

	/************
	 * 変数宣言 *
	 ************/
	BOOL pause_;                     // ポーズフラグ
	BOOL time_count_;                // カウントダウンフラグ
	BOOL ranking_flag_[kAllPlayer];  // ランキング表示フラグ

	INT count_;         // カウントダウン用ミリ秒
	INT count_down_;    // カウントダウン用秒
	INT result_count_;  // リザルト移行人数保存用

	INT bgm_;  // 再生するBGM番号保存用

	/************
	 * 内部関数 *
	 ************/
	BOOL MapInit(VOID);                // マップの初期化
	BOOL PlayerInit(VOID);             // プレイヤーの初期化
	VOID DrawPlayer(INT player);       // プレイヤー同士の描画範囲処理
	VOID GameSet(INT player);          // ゲーム内の状態更新処理
	VOID GameUpDate(INT player);       // ゲーム内の更新処理
	VOID GameResult(INT player);       // ゲーム内のリザルト処理
	VOID ResultUpDate(INT player_no);  // リザルトの更新処理
	VOID ResultDraw(INT player_no);    // リザルトの描画処理
	INT NextStage(VOID);               // ステージ移行処理
	BOOL CountDowm(VOID);              // カウントダウン処理
	BOOL Pause(INT player_no);         // ポーズの処理 

	/* 純粋仮想関数 */
	virtual VOID SpriteDraw(VOID) = 0; // スプライト描画処理

public:
	GameBase(VOID);   // コンストラクタ
	~GameBase(VOID);  // デストラクタ

	BOOL Init(VOID);     // 初期設定
	VOID ReSet(VOID);    // 初期化処理
	VOID Destroy(VOID);  // 破棄処理

	/* 純粋仮想関数 */
	virtual INT UpDate(VOID) = 0; // 更新処理
	virtual VOID Draw(VOID)  = 0; // 描画処理
};

#endif