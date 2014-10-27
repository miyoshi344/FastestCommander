/******************************
 * フィールドに表示するクラス *
 ******************************/

/* 多重Include防止 */
#ifndef RACE_FIELD_H_
#define RACE_FIELD_H_

/***********
 * Include *
 ***********/
#include "objectbase.h"
#include "fieldobject.h"
#include "collision.h"

/********
 * 定数 *
 ********/

/* メッシュ情報 */
CONST LPSTR kFieldMap = "Field.csv";  // ステージファイル
CONST LPSTR kSkyBall  = "skyball_morning.x";  // スカイボール

/* フィールド関係の数字 */
CONST FLOAT kFieldCollision = 1.0F;   // 当たり判定取得範囲
CONST FLOAT kSlopeDirection = 90.0F;  // 坂の角度

/* 壁の高さ */
CONST INT kWallLoop  = 5;  // 壁の高さ
CONST INT kDammyLoop = 5;  // ダミー壁の高さ

/******************************************
 * フィールドの描画や管理を行うクラスです *
 * ObjectBaseを継承して作成します         *
 ******************************************/
class Field : public ObjectBase
{
private:

	/**************
	 * クラス定義 *
	 **************/
	Collision    collision_;  // 当たり判定クラス

	FieldObject* block[BLOCK_MAX];  // ブロック格納用クラスポインタ
	D3DXVECTOR3 min_, max_, len_;

	/********
	 * 変数 *
	 ********/
	LPCSTR map_no_;    // マップの名前
	INT    player_;    // プレイヤー番号
	INT    field_no_;  // フィールド番号

	/* プレイヤー座標 */
	D3DXVECTOR3 origin_pos_[kAllPlayer];      // プレイヤーの移動前座標
	D3DXVECTOR3 player_pos_[kAllPlayer];      // プレイヤーの移動後座標
	D3DXVECTOR3 player_draw_pos_[kAllPlayer]; // 描画用プレイヤーの前方座標
	FLOAT       player_radius_[kAllPlayer];

	/* 弾の座標 */
	D3DXVECTOR3 origin_bullet_[kAllPlayer];  // 弾の移動前座標
	D3DXVECTOR3 bullet_pos_[kAllPlayer];     // 弾の移動後座標
	FLOAT       bullet_radius_[kAllPlayer];

	/**********
	 * リスト *
	 **********/

	/* マップの情報 */
	std::list<D3DXMATRIX>  matrix_list_;  // ワールドリスト
	std::list<D3DXVECTOR3> pos_list_;     // 座標リスト
	std::list<INT> information_list_;     // ブロック番号

	/* 状態保存用 */
	std::list<INT> info_list_;            // フィールド状態保存用リスト

	/* 描画用 */
	std::list<D3DXMATRIX>  draw_matrix_[kAllPlayer];       // 描画用ワールドリスト
	std::list<D3DXVECTOR3> draw_pos_[kAllPlayer];          // 描画範囲内判定座標リスト
	std::list<INT>         draw_information_[kAllPlayer];  // 描画範囲内ブロック番号

	/* 当たり判定用 */
	std::list<D3DXVECTOR3> player_pos_list_;
	std::list<D3DXMATRIX>  player_list_;         // プレイヤー付近の判定用リスト
	std::list<INT>         player_information_;  // プレイヤー付近の判定用ブロック番号
	std::list<FLOAT>       player_radius_list_;
	std::list<D3DXVECTOR3> player_len_list_;

	/**************
	 * イテレータ *
	 **************/

	/* マップの情報 */
	std::list<D3DXMATRIX>::iterator  it_matrix_;       // 全体のワールド座標
	std::list<D3DXVECTOR3>::iterator it_position_;     // 全体の座標
	std::list<INT>::iterator         it_infomation_;   // 全体のリスト
	std::list<D3DXVECTOR3>::iterator it_end_position_; // 全体の終了用

	/* 描画用 */
	std::list<D3DXVECTOR3>::iterator it_draw_pos_;
	std::list<D3DXMATRIX>::iterator  it_draw_matrix_;      // 描画用ワールド座標
	std::list<INT>::iterator         it_draw_infomation_;  // 描画用ブロック番号
	std::list<D3DXMATRIX>::iterator  it_draw_end_;         // 描画終了用

	/* プレイヤー当たり判定用 */
	std::list<D3DXVECTOR3>::iterator it_player_pos_;
	std::list<D3DXMATRIX>::iterator  it_player_matrix_;      // プレイヤー近くのブロックの座標
	std::list<INT>::iterator         it_player_infomation_;  // プレイヤー近くのブロックの情報
	std::list<FLOAT>::iterator       it_radius_;             
	std::list<D3DXVECTOR3>::iterator it_len_list_;
	std::list<D3DXMATRIX>::iterator  it_player_end_;         // プレイヤー近く判定の終了用

	/* 弾用当たり判定 */
	std::list<D3DXMATRIX>::iterator  it_bullet_matrix_;      // 弾の近くのブロックのワールド座標
	std::list<D3DXVECTOR3>::iterator it_bullet_position_;    // 弾の近くのブロック座標
	std::list<INT>::iterator         it_bullet_infomation_;  // 弾の近くのブロック情報
	std::list<D3DXMATRIX>::iterator  it_bullet_end_;         // 弾の判定の終了用

	/************
	 * 内部関数 *
	 ************/
	BOOL SkyInit(VOID);         // スカイボール初期読み込み
	BOOL BlockInit(VOID);       // ブロック初期読み込み
	BOOL MapFileSet(VOID);      // マップ初期読み込み
	 
	VOID PlayerHitCheck(VOID);  // プレイヤーの当たり判定チェック
	VOID FieldDraw(VOID);       // フィールドの描画
	VOID SkyDraw(VOID);         // スカイボールの描画

public:
	Field(VOID);         // コンストラクタ
	~Field(VOID);        // デストラクタ

	BOOL Init(VOID);     // 初期処理
	VOID ReSet(VOID);    // 初期化処理
	VOID UpDate(VOID);   // 更新処理
	VOID Draw(VOID);     // 描画処理
	VOID Destroy(VOID);  // 破棄処理
	INT  BlockCheck(VOID);              // プレイヤーのブロック判定
	BOOL BulletCheck(INT PlayerColor);  // 弾のブロック判定

	/**********************
	 * 変数を受け取る関数 *
	 **********************/
	VOID SetPlayerPos(D3DXVECTOR3 player_pos)       { player_pos_[player_] = player_pos; }        // プレイヤーごとの更新前座標をもらう
	VOID SetOriginPos(D3DXVECTOR3 origin_pos)       { origin_pos_[player_] = origin_pos; }        // プレイヤーごとの更新後の座標をもらう
	VOID SetDrawPos(D3DXVECTOR3 draw_pos)           { player_draw_pos_[player_] = draw_pos; }     // 描画する位置の座標をもらう
	VOID SetOriginBullet(D3DXVECTOR3 origin_bullet) { origin_bullet_[player_] = origin_bullet; }  // 弾の更新前の座標をもらう
	VOID SetBulletPos(D3DXVECTOR3 bullet_pos)       { bullet_pos_[player_] = bullet_pos; }        // 弾の更新後の座標をもらう 
	VOID SetPlayerRadius(FLOAT radius)              { player_radius_[player_] = radius; }
	VOID SetBulletRadius(FLOAT radius)              { bullet_radius_[player_] = radius; }

	VOID SetPlayerNo(INT player_no) { player_   = player_no; }  // プレイヤーの番号
	VOID SetMapNo(LPCSTR MapNo)     { map_no_   = MapNo; }      // 読み込むマップのファイル名
	VOID SetFieldNo(INT field_no)   { field_no_ = field_no; }   // 現在のマップ番号
};

#endif