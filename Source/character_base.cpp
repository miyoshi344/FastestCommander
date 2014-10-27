/***********
 * Include *
 ***********/
#include "character_base.h"

/******************
 * コンストラクタ *
 ******************/
CharacterBase::CharacterBase(VOID)
{
	/* 変数初期化 */
	player_radius_ = 0.0F;
	bullet_radius_ = 0.0F;

	/* 向き値初期化 */
	body_direction_   = 0.0F;
	turret_direction_ = 0.0F;
	body_slope_       = 0.0F;
	turret_slope_     = 0.0F;

	/* 状態初期化 */
	move_ = 0.0F;
	jump_ = 0.0F;

	/* スピードの初期化 */
	max_speed_ = performance_.MaxSpeed;  // 最高速
	brake_     = performance_.Brake;     // ブレーキ値

	/* フラグ管理 */
	jump_flag_   = FALSE;
	bullet_flag_ = FALSE;
	hold_flag_   = FALSE;
}

/****************
 * デストラクタ *
 ****************/
CharacterBase::~CharacterBase(VOID)
{
	/* メッシュの解放 */
	Mesh::Release(body_mesh_);
	Mesh::Release(turret_mesh_);

	/* 弾の破棄 */
	bullet_.Destroy();
}


/***********************************
 * 関数名 : ReSet関数              *
 * 内  容 : 状態の初期化を行います *
 * 引  数 : なし                   *
 * 戻り値 : なし                   *
 ***********************************/
VOID CharacterBase::ReSet(VOID)
{
	/* スタート位置を設定 */
	body_position_ = start_pos_;
	draw_pos_ = body_position_;

	/* 変数初期化 */

	/* 向き値初期化 */
	body_direction_   = 0.0F;
	turret_direction_ = 0.0F;
	body_slope_       = 0.0F;
	turret_slope_     = 0.0F;

	/* ステータス初期化 */
	move_ = 0.0F;
	jump_ = 0.0F;

	/* 速度の初期化 */
	max_speed_ = performance_.MaxSpeed;
	brake_     = performance_.Brake;

	/* フラグ管理 */
	jump_flag_   = FALSE;
	bullet_flag_ = FALSE;
	hold_flag_   = FALSE;

	/* 弾の初期化 */
	bullet_.ReSet();

	DSound::Pause(ENGINE_SE);
}

/*******************************
 * 関数名 : Draw関数           *
 * 内  容 : 描画処理を行います *
 * 引  数 : なし               *
 * 戻り値 : なし               *
 *******************************/
VOID CharacterBase::Draw(VOID)
{
	BodyDraw();    // ボディの描画
	TurretDraw();  // タレットの描画

	/* 弾の描画 */
	if (bullet_flag_) bullet_.Draw();

	//D3DXVECTOR3 pos = turret_position_;
	//pos.y += 0.5F;

	//collision_.SphereMesh(turret_mesh_, pos, radius_);
}

/*******************************
 * 関数名 : Destroy関数        *
 * 内  容 : 破棄処理を行います *
 * 引  数 : なし               *
 * 戻り値 : なし               *
 *******************************/
VOID CharacterBase::Destroy(VOID)
{
	/* メッシュの解放 */
	Mesh::Release(body_mesh_);
	Mesh::Release(turret_mesh_);

	/* 弾の破棄 */
	bullet_.Destroy();
}

/*******************************************
 * 関数名 : GroundCheck関数                *
 * 内  容 : 地面のブロック情報を取得します *
 * 引  数 : なし                           *
 * 戻り値 : なし                           *
 *******************************************/
VOID CharacterBase::BlockCheck(INT block_)
{
	/* 当たったブロックごとの処理を行う */
	switch (block_)
	{
		case GROUND       : GroundHit();              break;  // 地面
		case THORN        : Thorn();                  break;  // 棘
		case SLOPE        : SlopeHit();               break;  // 坂
		case WALL         : WallHit();                break;  // 壁
		case RED_BLOCK    : ColorBlock(RED_BLOCK);    break;  // 赤地面
		case GREEN_BLOCK  : ColorBlock(GREEN_BLOCK);  break;  // 緑地面
		case YELLOW_BLOCK : ColorBlock(YELLOW_BLOCK); break;  // 黄地面
		case BLUE_BLOCK   : ColorBlock(BLUE_BLOCK);   break;  // 青地面
		case DUMMY        : break;
		default           : jump_flag_ = TRUE;       
							jump_ -= performance_.Gravity;  // ジャンプ値を徐々に下げる
							break;  // 地面にいない状態
	}
}

/***********************************************
 * 関数名 : Thorn関数                          *
 * 内  容 : 悪地形に当たった時の処理を行います *
 * 引  数 : なし                               *
 * 戻り値 : なし                               *
 ***********************************************/
VOID CharacterBase::Thorn(VOID)
{
	/* 減速中のSEを再生 */
	DSound::Play(DOWN_SE);

	/* 速度を落とす */
	max_speed_ = performance_.DownMaxSpeed;  // 最高速を下げる
	if (max_speed_ < move_) move_ -= performance_.DownSpeed;  // 最高速超えていた場合速度を下げる

	jump_      = 0.0F;  // ジャンプ値を0にする
	jump_flag_ = TRUE;  // ジャンプができない状態にする
}

/*********************************************
 * 関数名 : GroundHit関数                    *
 * 内  容 : 地面に当たった時の処理を行います *
 * 引  数 : なし                             *
 * 戻り値 : なし                             *
 *********************************************/
VOID CharacterBase::GroundHit(VOID)
{
	jump_ = 0.0F;        // ジャンプ値を0にする
	jump_flag_ = FALSE;  // ジャンプができる状態にする

	body_position_.y += 0.005F;

	/* 最高速を戻す */
	max_speed_ = performance_.MaxSpeed;
}

/***********************************************
 * 関数名 : Wallhit関数                        *
 * 内  容 : 壁に当たった時の停止処理を行います *
 * 引  数 : なし                               *
 * 戻り値 : なし                               *
 ***********************************************/
VOID CharacterBase::WallHit(VOID)
{
	/* 壁にぶつかったSEを再生 */
	DSound::Restart(WALL_SE);
	DSound::Play(WALL_SE);

	/* 向いている方向の逆に移動 */
	body_position_.x += sin(body_direction_) * -(move_ + 0.5F);
	body_position_.z += cos(body_direction_) * -(move_ + 0.5F);

	/* 加速度を下げる */
	move_ /= 10.0F;

	/* 加速度が0以下になりそうなら固定する */
	if (move_ <= 0.0F) move_ = 0.0F;
}

/*************************************************
 * 関数名 : RedBlock関数                         *
 * 内  容 : 色ブロックに触れた時の処理を行います *
 * 引  数 : INT color_block 触れたブロックの色   *
 * 戻り値 : なし                                 *
 *************************************************/
VOID CharacterBase::ColorBlock(INT color_block)
{
	/* 自分の色かどうか比較を行う */
	if (player_color_ == color_block)
	{
		/* 自分の色と一致したら加速を行う */
		DSound::Pause(DASH_SE);
		DSound::Play(DASH_SE);

		move_     += 0.05F;
		jump_      = 0.0F;
		jump_flag_ = TRUE;
	}
	/* 自分以外の色なら */
	else
	{
		/* 減速中のSEを再生 */
		DSound::Play(DOWN_SE);

		/* 速度を落とす */
		max_speed_ = performance_.DownMaxSpeed;  // 最高速を下げる
		if (max_speed_ < move_) move_ -= performance_.DownColorSpeed;     // 最高速超えていた場合速度を下げる

		jump_ = 0.0F;  // ジャンプ値を0にする
		jump_flag_ = TRUE;  // ジャンプができない状態にする
	}
}

/*******************************************
 * 関数名 : SlopeHit関数                   *
 * 内  容 : 坂に当たった時の処理を行います *
 * 引  数 : なし                           *
 * 戻り値 : なし                           *
 *******************************************/
VOID CharacterBase::SlopeHit(VOID)
{
	body_position_.y += sin(move_ * 1.3F);  // キャラクターの位置を上げる
}

/*************************************************
 * 関数名 : PlayerMeshInit関数                   *
 * 内  容 : プレイヤーのメッシュ情報を取得します *
 * 引  数 : なし                                 *
 * 戻り値 : なし                                 *
 *************************************************/
BOOL CharacterBase::PlayerMeshInit(VOID)
{
	BOOL ret = TRUE;
	LPCSTR body,
		   turret,
		   bullet;

	/* プレイヤーの色に合わせる */
	switch (player_no_)
	{
		/* 赤戦車 */
		case RED_TANK:
			body     = tank_.red_body;      // ボディメッシュ
			turret   = tank_.red_turret;    // タレットメッシュ
			bullet   = tank_.red_burret;    // バレットメッシュ
			break;

		/* 緑戦車 */
		case GREEN_TANK:
			body     = tank_.green_body;     // ボディメッシュ
			turret   = tank_.green_turret;   // タレットメッシュ
			bullet   = tank_.green_burret;   // バレットメッシュ
			break;

		/* 黄戦車 */
		case YELLOW_TANK:
			body     = tank_.yellow_body;     // ボディメッシュ
			turret   = tank_.yellow_turret;   // タレットメッシュ
			bullet   = tank_.yellow_bullet;   // バレットメッシュ
			break;

		/* 青戦車 */
		case BLUE_TANK:
			body     = tank_.blue_body;       // ボディメッシュ
			turret   = tank_.blue_turret;     // タレットメッシュ
			bullet   = tank_.blue_burret;     // バレットメッシュ
			break;

		default: break;
	}

	/* 読み込むメッシュを選択 */
	bullet_.BulletColor(bullet);

	do
	{
		/* 弾の初期化 */
		if (!bullet_.Init())
		{
			/* エラー */
			ErrorDialog("弾のメッシュ取得に失敗");
			ret = FALSE;
			break;
		}

		/* フォルダ階層を移動する */
		_chdir("Resource_fast/Mesh/Tank");

		/* メッシュの読み込み */
		if ((body_mesh_ = Mesh::Load(body)) == NULL)
		{
			/* エラー */
			ErrorDialog("ボディのメッシュ取得に失敗");
			ret = FALSE;
			break;
		}

		/* メッシュの読み込み */
		if ((turret_mesh_ = Mesh::Load(turret)) == NULL)
		{
			/* エラー */
			ErrorDialog("タレットのメッシュ取得に失敗");
			ret = FALSE;
			break;
		}

	} while (0);

	/* フォルダ階層を戻す */
	_chdir("../../../");

	return ret;
}

/***************************************
 * 関数名 : BodyDraw関数               *
 * 内  容 : ボディの描画処理を行います *
 * 引  数 : なし                       *
 * 戻り値 : なし                       *
 ***************************************/
VOID CharacterBase::BodyDraw(VOID)
{
	/* 座標と角度を設定 */
	D3DXMATRIX body_pos, body_rota;

	/* 回転の設定 */
	D3DXMatrixRotationY(&body_rota, body_direction_);

	/* 移動の設定 */
	D3DXMatrixTranslation(&body_pos, body_position_.x, body_position_.y, body_position_.z);

	/* ワールド座標系を作成 */
	D3DXMatrixIdentity(&body_world_);

	/* 行列を掛け合わせる */
	D3DXMatrixMultiply(&body_world_, &body_rota, &body_pos);

	/* 描画 */
	Mesh::Draw(body_world_, body_mesh_);
}

/*****************************************
 * 関数名 : TurretDraw関数               *
 * 内  容 : タレットの描画処理を行います *
 * 引  数 : なし                         *
 * 戻り値 : なし                         *
 *****************************************/
VOID CharacterBase::TurretDraw(VOID)
{
	/* 座標をボディと同期する */
	turret_position_ = body_position_;

	/* 座標と角度を設定 */
	D3DXMATRIX turret_pos, turret_rota;

	/* 回転の設定 */
	D3DXMatrixRotationY(&turret_rota, turret_direction_);

	/* 移動の設定 */
	D3DXMatrixTranslation(&turret_pos, turret_position_.x, turret_position_.y, turret_position_.z);

	/* ワールド座標系を作成 */
	D3DXMatrixIdentity(&turret_world_);

	/* 行列を掛け合わせる */
	D3DXMatrixMultiply(&turret_world_, &turret_rota, &turret_pos);

	/* 描画 */
	Mesh::Draw(turret_world_, turret_mesh_);
}

VOID CharacterBase::PlayerHit(VOID)
{
	DSound::Play(WALL_SE);

	/* 向いている方向の逆に移動 */
	body_position_.x += sin(body_direction_) * -(move_ * 5.0F);
	body_position_.z += cos(body_direction_) * -(move_ * 5.0F);

	/* 加速度を下げる */
	move_ /= 10.0F;

	/* 加速度が0以下になりそうなら固定する */
	if (move_ <= 0.0F) move_ = 0.0F;
}