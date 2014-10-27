/****************************
 * キャラクターの基礎クラス *
 ****************************/

/* 多重Include防止 */
#ifndef RACE_CHARACTER_BASE_H_
#define RACE_CHARACTER_BASE_H_

/***********
 * Include *
 ***********/
#include "objectbase.h"
#include "collision.h"
#include "bullet.h"

/********
 * 定数 *
 ********/
CONST FLOAT kDrawPosition = 22.0F;  // プレイヤー回りの描画範囲

/**********
 * 構造体 *
 **********/

/* プレイヤーのステータスに関する構造体 */
CONST struct TankPerformance
{
	/************
	 * スピード *
	 ************/
	FLOAT MaxSpeed       = 0.41F;  // 最高速
	FLOAT DownSpeed      = 0.01F;  // 棘地面での減速値
	FLOAT DownColorSpeed = 0.05F;  // 他のフィールドを踏んだ時の減速値
	FLOAT DownMaxSpeed   = 0.11F;  // 棘地面での最高速
	
	FLOAT Brake        = 0.005F; // ブレーキ

	FLOAT Inertia      = 0.003F;   // 慣性値
	FLOAT Accel        = 0.00001F; // 加速度

	/********
	 * ギア *
	 ********/
	FLOAT GearOne      = 0.2F;   // 2速切り替え
	FLOAT GearTwo      = 0.3F;   // 3速切り替え
	FLOAT GearThree    = 0.35F;  // 4速切り替え

	FLOAT GearOneSpeed   = 0.000005F;  // 1速加速値
	FLOAT GearTwoSpeed   = 0.000003F;  // 2速加速値
	FLOAT GearThreeSpeed = 0.000002F;  // 3速加速値
	FLOAT GearForSpeed   = 0.0000008F;  // 4速加速値

	/************
	 * ジャンプ *
	 ************/
	FLOAT Jump    = 0.07F;   // ジャンプ値
	FLOAT Gravity = 0.005F; // 重力値

	/********
	 * 旋回 *
	 ********/
	FLOAT Direction    = 0.0003F;  // 向きの入力値
	FLOAT MaxDirection = 0.015F;   // 向きの入力最大値 
};

/* プレイヤーに関する構造体 */
CONST struct Tank
{
	/* 赤戦車 */
	LPCSTR red_body       = "tank_red_body.x";        // ボディメッシュ名
	LPCSTR red_turret     = "tank_red_turret.x";      // タレットメッシュ名
	LPCSTR red_burret     = "bullet_red.x";           // バレットメッシュ名
	LPCSTR red_ai_name    = "Resource_fast/AI/red_ai";     // AIテキスト場所

	/* 緑戦車 */
	LPCSTR green_body     = "tank_green_body.x";      // ボディメッシュ名
	LPCSTR green_turret   = "tank_green_turret.x";    // タレットメッシュ名
	LPCSTR green_burret   = "bullet_green.x";         // バレットメッシュ名
	LPCSTR green_ai_name  = "Resource_fast/AI/green_ai";    // AIテキスト場所

	/* 黄戦車 */
	LPCSTR yellow_body    = "tank_yellow_body.x";     // ボディメッシュ名
	LPCSTR yellow_turret  = "tank_yellow_turret.x";   // タレットメッシュ名
	LPCSTR yellow_bullet  = "bullet_yellow.x";        // バレットメッシュ名
	LPCSTR yellow_ai_name = "Resource_fast/AI/yellow_ai";  // AIテキスト場所

	/* 青戦車 */
	LPCSTR blue_body      = "tank_blue_body.x";       // ボディメッシュ名
	LPCSTR blue_turret    = "tank_blue_turret.x";     // タレットメッシュ名
	LPCSTR blue_burret    = "bullet_blue.x";          // バレットメッシュ名
	LPCSTR blue_ai_name   = "Resource_fast/AI/blue_ai";    // AIテキスト場所
};

/************************************************
 * キャラクター用の基礎クラスです               *
 * キャラクターはこのクラスを継承して作成します *
 ************************************************/
class CharacterBase
{
protected:
	/**************
	 * クラス定義 *
	 **************/
	Collision collision_;
	Bullet    bullet_;  // 弾のクラス

	/**************
	 * 構造体定義 *
	 **************/
	TankPerformance performance_;  // 戦車のステータス情報
	Tank tank_;                    // 戦車情報

	/* メッシュ */
	MeshTable* body_mesh_;   // ボディ用メッシュ
	MeshTable* turret_mesh_; // タレット用メッシュ  

	/********
	 * 変数 *
	 ********/
	FLOAT player_radius_;
	FLOAT bullet_radius_;

	/* 座標 */
	D3DXVECTOR3 body_position_;    // ボディ用座標
	D3DXVECTOR3 turret_position_;  // タレット用座標

	D3DXVECTOR3 start_pos_;      // スタート位置
	D3DXVECTOR3 origin_pos_;     // 移動する前の座標
	D3DXVECTOR3 draw_pos_;       // マップの描画する座標
	D3DXVECTOR3 save_pos_;       // 復活刷る座標位置
	D3DXVECTOR3 bullet_pos_;     // 弾の座標保存位置
	D3DXVECTOR3 origin_bullet_;  // 移動する前の弾の座標

	/* ワールド座標 */
	D3DXMATRIX body_world_;    // ボディ用ワールド座標
	D3DXMATRIX turret_world_;  // タレット用ワールド座標

	/* ステータス */
	FLOAT move_;              // 速度
	FLOAT jump_;              // ジャンプ値 
	FLOAT brake_;             // ブレーキ
	FLOAT max_speed_;         // 最大速度
	FLOAT body_direction_;    // ボディ旋回
	FLOAT body_slope_;        // ボディ向きの入力保存
	FLOAT turret_direction_;  // タレット旋回
	FLOAT turret_slope_;      // タレット向きの入力保存

	BOOL jump_flag_;    // ジャンプ出来るかのフラグ
	BOOL bullet_flag_;  // 弾を撃てるかのフラグ
	BOOL hold_flag_;

	INT player_no_;     // プレイヤー番号
	INT player_color_;  // プレイヤーカラー

	ULONG button_[kMaxPad];  // ボタン入力保存用

	/**********
	 * リスト *
	 **********/
	std::list<INT> left_stick_list_;   // 左スティック入力保存用
	std::list<INT> right_stick_list_;  // 右スティック入力保存用
	std::list<INT> accel_list_;        // アクセル入力保存用
	std::list<INT> brake_list_;        // ブレーキ入力保存用
	std::list<WORD> jump_list_;        // ジャンプ入力保存用

	/**************
	 * イテレータ *
	 **************/
	std::list<INT>::iterator it_left_stick_;  // 左スティック用イテレータ
	std::list<INT>::iterator it_right_stick_; // 右スティック用イテレータ
	std::list<INT>::iterator it_accel_;       // アクセル用イテレータ
	std::list<INT>::iterator it_brake_;       // ブレーキ用イテレータ
	std::list<WORD>::iterator it_jump_;       // ジャンプ用イテレータ

	/************
	 * 内部関数 *
	 ************/

	/* 純粋仮想関数 */
	virtual VOID Jump(VOID)            = 0;  // ジャンプ処理
	virtual VOID Direction(VOID)       = 0;  // 移動処理
	virtual VOID TurretDirection(VOID) = 0;  // 砲塔の視点移動
	virtual VOID Accel(VOID)           = 0;  // アクセル処理

	/* ブロックに当たった時の処理 */
	VOID Thorn(VOID);       // 棘に当たった時の処理
	VOID GroundHit(VOID);   // 地面に当たった時の処理
	VOID WallHit(VOID);     // 壁にぶつかった処理
	VOID SlopeHit(VOID);    // 坂に当たった時の処理

	/* 情報の取得や描画 */
	BOOL PlayerMeshInit(VOID);         // メッシュ情報の取得処理
	VOID BodyDraw(VOID);               // ボディの描画処理
	VOID TurretDraw(VOID);             // タレットの描画処理
	VOID ColorBlock(INT block_color);  // 色の付いた地面の処理

public:
	CharacterBase(VOID);   // コンストラクタ
	~CharacterBase(VOID);  // デストラクタ

	virtual BOOL Init(VOID)    = 0;  // 初期設定
	virtual VOID UpDate(VOID)  = 0;  // 更新処理

	VOID ReSet(VOID);      // 初期化
	VOID Draw(VOID);       // 描画処理
	VOID Destroy(VOID);    // 破棄
	VOID PlayerHit(VOID);  // プレイヤー同士の当たり判定

	VOID BlockCheck(INT block_);    // 当たり判定チェック用関数

	/* 値を渡す関数 */ 
	D3DXVECTOR3 ReturnOrigin(VOID)       { return origin_pos_; }     // 更新する前の座標
	D3DXVECTOR3 ReturnPosition(VOID)     { return body_position_; }  // 更新した後の座標
	D3DXVECTOR3 ReturnDrawPosition(VOID) { return draw_pos_; }       // マップの描画位置座標
	D3DXVECTOR3 ReturnOriginBullet(VOID) { return origin_bullet_; }  // 弾の更新する前の座標
	D3DXVECTOR3 ReturnBullet(VOID)       { return bullet_pos_; }     // 弾の更新した後の座標
	MeshTable*  ReturnMesh(VOID)         { return body_mesh_; }

	FLOAT ReturnDirection(VOID)    { return body_direction_; }          // 旋回値
	FLOAT ReturnMove(VOID)         { return move_; }                    // 速度
	FLOAT ReturnPlayerRadius(VOID) { return player_radius_; }
	FLOAT ReturnBulletRadius(VOID) { return bullet_radius_; }

	/* 値をもらう関数 */
	VOID StartPosition(D3DXVECTOR3 pos)   { start_pos_ = pos; }              // スタート位置
	VOID BulletFlag(VOID)                 { bullet_flag_ = FALSE; }          // 弾が当たっているかのフラグ
	VOID SetPlayerColor(INT player_color) { player_color_ = player_color; }  // プレイヤーの色情報
	VOID SetPlayerNo(INT player_no)       { player_no_ = player_no; }        // プレイヤーの番号情報

	VOID ReturnPosition(D3DXVECTOR3 pos) { save_pos_ = pos; }
};

#endif
