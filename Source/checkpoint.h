/* 多重Include防止 */
#ifndef RACE_CHECKPOINT_H_
#define RACE_CHECKPOINT_H_

/***********
* Include *
***********/
#include "layout_base.h"
#include "collision.h"

/**********
 * 列挙型 *
 **********/
enum
{
	WeekPoint,    // 1週
	MiddlePoint,  // チェックポイント
	NoPoint,      // なし
};

/**********
 * 構造体 *
 **********/

/* プレイヤーの座標に関数構造体 */
struct PlayerPos
{
	D3DXVECTOR3 start_position_;  // プレイヤーの開始座標
	D3DXVECTOR3 player_pos_;      // プレイヤー座標
	D3DXVECTOR3 origin_pos_;      // プレイヤーの動く前の座標

	/******************
	 * コンストラクタ *
	 ******************/
	PlayerPos(VOID)
	{
		start_position_ = D3DXVECTOR3(0.0F, 0.0F, 0.0F);
		player_pos_     = D3DXVECTOR3(0.0F, 0.0F, 0.0F);
		origin_pos_     = D3DXVECTOR3(0.0F, 0.0F, 0.0F);
	}
};

/* プレイヤーの情報に関する構造体 */
struct PlayerCheck
{
	INT   check_no;  // 現在のチェックポイント到達数
	INT   lap;       // 周回数
	INT   ranking;   // 順位
	FLOAT vector;    // 次のチェックポイントまでの距離

	/* 初期化 */
	VOID ReSet(VOID)
	{
		check_no = 0;
		lap      = 1;
		ranking  = 0;
		vector   = 0.0F;
	}
};

/********
 * 定数 *
 ********/
CONST FLOAT kMultiY = 60.0F;
CONST FLOAT kMultiX = 30.0F;

/* 配置情報 */
CONST D3DXVECTOR3 kMultiRankingPos   = { 10.0F, 50.0F, 0.0F };  // 画面との間隔
CONST D3DXVECTOR3 kMultiRankPos      = { 50.0F, 15.0F, 0.0F };  // 画面との間隔

/* 表示間隔 */
CONST FLOAT kLapPosition    = 80.0F;  // チェックポイントの配置間隔
CONST FLOAT kFienalPosition = 200.0F;

/****************
 * ファイル情報 *
 ****************/
CONST LPSTR kCheckCsv = "checkpoint.csv";   // チェックポイント用CSV
CONST LPSTR kHitCheck = "box_obj.x";	    // プレイヤー用メッシュ

/* 周回情報 */
CONST INT   kFinalLap       = 3;     // 周回数
CONST FLOAT kCheckCollision = 20.0F;  // チェックポイントの当たり判定大きさ

/****************************************
 * チェックポイントに関数クラスです     *
 * LayoutBaseクラスを継承して作成します *
 ****************************************/
class CheckPoint : public LayoutBase
{
private:
	D3DXVECTOR3 return_position_[kAllPlayer];

	/**************
	 * 構造体定義 *
	 **************/
	PlayerPos   player_pos_[kAllPlayer];    // 配置座標
	PlayerCheck player_check_[kAllPlayer];  // プレイヤー情報

	/**************
	 * クラス定義 *
	 **************/
	MeshTable*  my_mesh_;    // メッシュ情報
	Collision   collision_;  // 当たり判定

	/********
	 * 変数 *
	 ********/
	LPCSTR check_no_;   // マップ番号
	INT    check_max_;  // チェックポイント数保存用

	/**********
	 * リスト *
	 **********/
	std::list<D3DXMATRIX>  check_matrix_;           // チェックポイント用ワールド座標
	std::list<D3DXVECTOR3> check_position_;         // チェックポイント用座標
	std::list<INT>         check_information_;      // チェックポイント用ブロック情報

	/****************
	 * イテレーター *
	 ****************/
	std::list<D3DXMATRIX>::iterator  it_matrix_;     // ワールド座標用イテレータ
	std::list<D3DXVECTOR3>::iterator it_position_;   // 座標用イテレータ
	std::list<INT>::iterator         it_infomation;  // ブロック情報用イテレータ

	std::list<D3DXMATRIX>::iterator end_matrix_;

	/************
	 * 内部関数 *
	 ************/
	INT  LapCheck(VOID);                   // プレイヤーの周回をチェックする関数
	BOOL FileCheck(VOID);                  // マップを読み込む関数
	VOID TwoLapDraw(INT player_no);        // 2人用ラップ数描画処理
	VOID TwoRankingDraw(INT player_no);    // 2人用レース結果を描画する関数
	VOID MultiLapDraw(INT player_no);      // 多人数用ラップ数描画処理 
	VOID MultiRankingDraw(INT player_no);  // レース結果を描画する関数

public:
	CheckPoint(VOID);    // コンストラクタ
	~CheckPoint(VOID);   // デストラクタ

	BOOL Init(VOID);                // 初期設定
	VOID ReSet(VOID);               // 初期化処理
	INT  UpDate(INT player_no);     // 更新処理
	VOID Draw(INT player_no);       // 描画処理
	VOID TwoDraw(INT player_no);    // 2人用描画処理
	VOID MultiDraw(INT player_no);  // 多人数用描画処理
	VOID Destroy(VOID);             // 破棄処理

	VOID RankingSort(INT player_no, INT player_count);  // ランキング描画処理
	VOID CheckDraw(INT player_no);                      // 現在のチェックポイント到達数を描画

	/* ゴールした時の処理 */
	BOOL Goal(INT player_no)
	{
		/* プレイヤーのラップ数が規定数を超えたなら */
		if (player_check_[player_no].lap > kFinalLap) return TRUE;
		else return FALSE;
	}

	/* 引数をもらう関数 */
	VOID SetPlayerPosition(D3DXVECTOR3 position) { player_pos_[player_no_].player_pos_ = position; }
	VOID SetPlayerOrigin(D3DXVECTOR3 origin) { player_pos_[player_no_].origin_pos_ = origin; }
	VOID SetMapNo(LPCSTR MapNo) { check_no_ = MapNo; }

	/* 変数を渡す関数 */
	D3DXVECTOR3 ReturnStartPos(INT no) { return player_pos_[no].start_position_; }
	INT ReturnRanking(INT player_no) { return player_check_[player_no].ranking; }

	D3DXVECTOR3 ReturnPosition(INT player_no) { return return_position_[player_no]; }
};
#endif