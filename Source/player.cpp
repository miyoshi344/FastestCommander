/***********
 * Include *
 ***********/
#include "player.h"

/******************
 * コンストラクタ *
 ******************/
Player::Player(VOID)
{
}

/****************
 * デストラクタ *
 ****************/
Player::~Player(VOID)
{
}

/*********************************
 * 関数名 : Init関数             *
 * 内  容 : 初期化処理を行います *
 * 引  数 : なし                 *
 * 戻り値 : なし                 *
 *********************************/
BOOL Player::Init(VOID)
{
	/* 戻り値用 */
	BOOL ret = TRUE;

	do
	{
		/* プレイヤーのメッシュ情報を取得 */
		if (!PlayerMeshInit())
		{
			/* エラー */
			ErrorDialog("プレイヤー情報の取得に失敗");
			ret = FALSE;
			break;
		}

		player_radius_ = collision_.SphereCreate(turret_mesh_);
		player_radius_ /= 1.5F;

		bullet_radius_ = bullet_.ReturnRadius();
		bullet_radius_ *= 1.5F;
	} while (0);

	return ret;
}

/*******************************
 * 関数名 : UpDate             *
 * 内  容 : 更新処理を行います *
 * 引  数 : なし               *
 * 戻り値 : なし               *
 *******************************/
VOID Player::UpDate(VOID)
{
	button_[player_no_] = XInput::ReturnPushButton(player_no_);

	Jump();             // ジャンプ
	TurretDirection();  // タレット旋回
	Direction();        // キャラクター旋回
	Accel();            // アクセル

	/* プレイヤーの描画範囲の設定 */
	draw_pos_ = body_position_;
	draw_pos_.x += sinf(body_direction_) * kDrawPosition;
	draw_pos_.z += cosf(body_direction_) * kDrawPosition;

	bullet_.UpDate();  // 弾の更新
	if (bullet_flag_)bullet_pos_ = bullet_.ReturnPosition();
	bullet_pos_.y += 0.5F;

	if (body_position_.y <= -20.0F)
	{
		body_position_.y = 0.0F;
		body_position_ = save_pos_;
		origin_pos_ = save_pos_;
	}
}

/***********************************************
 * 関数名 : Jump関数                           *
 * 内  容 : プレイヤーのジャンプ処理を行います *
 * 引  数 : なし                               *
 * 戻り値 : なし                               *
 ***********************************************/
VOID Player::Jump(VOID)
{
	/* 地面にいてボタンが押されたら */
	if ((button_[player_no_] & XINPUT_GAMEPAD_A) && !jump_flag_)
	{
		DSound::Restart(JUMP_SE);
		DSound::Play(JUMP_SE);

		jump_list_.push_back(XINPUT_GAMEPAD_A);

		jump_        = performance_.Jump;  // ジャンプ値を代入
		jump_flag_   = TRUE;               // ジャンプフラグを立てる

		bullet_flag_ = TRUE;               // 弾を撃てる状態にする
		bullet_.BulletSet(body_position_, turret_direction_, move_);  // 弾の設定
	}

	/* 座標の更新 */
	body_position_.y += jump_;      // 座標の更新
	origin_pos_ = body_position_;   // 移動前の座標を設定
}

/***********************************************
 * 関数名 : Direction                          *
 * 内  容 : キャラクターの向きの管理を行います *
 * 引  数 : なし                               *
 * 戻り値 : なし                               *
 ***********************************************/
VOID Player::Direction(VOID)
{
	/* 左スティックの値を取得 */
	INT thumb = XInput::AdditionLeftX(player_no_);

	/* 右に入力されたら */
	if (thumb > 0)
	{
		/* 右を向く */
		body_slope_ += performance_.Direction;  // 角度

		/* 最大値になったら固定する */
		if (body_slope_ >= performance_.MaxDirection)
		{
			body_slope_ = performance_.MaxDirection;
		}
	}

	/* 左に入力されたら */
	if (thumb < 0)
	{
		/* 左を向く */
		body_slope_ -= performance_.Direction;  // 角度

		/* 最大値になったら固定する */
		if (body_slope_ <= -performance_.MaxDirection)
		{
			body_slope_ = -performance_.MaxDirection;
		}
	}

	/* 入力がなかったなら初期化する */
	if (thumb == 0)
	{
		body_slope_ = 0.0F;
		
	}

	left_stick_list_.push_back(thumb);

	/* 向きを設定する */
	body_direction_ += body_slope_;
}

/***************************************
 * 関数名 : TurretDirection関数        *
 * 内  容 : タレットの向きを管理します *
 * 引  数 : なし                       *
 * 戻り値 : なし                       *
 ***************************************/
VOID Player::TurretDirection(VOID)
{
	/* 右スティックの値を取得 */
	INT thumb = XInput::AdditionRightX(player_no_);

	if (button_[player_no_] & XINPUT_GAMEPAD_RIGHT_THUMB) hold_flag_ = !hold_flag_;

	/* 右に入力されたら */
	if (thumb > 0)
	{
		/* 右を向く */
		turret_slope_ += performance_.Direction;  // 角度

		/* 最大値になったら固定する */
		if (turret_slope_ >= performance_.MaxDirection)
		{
			turret_slope_ = performance_.MaxDirection;
		}
	}

	/* 左に入力されたら */
	if (thumb < 0)
	{
		/* 左を向く */
		turret_slope_ -= performance_.Direction;  // 角度

		/* 最大値になったら固定する */
		if (turret_slope_ <= -performance_.MaxDirection)
		{
			turret_slope_ = -performance_.MaxDirection;
		}
	}

	/* 入力がなかったなら初期化する */
	if (thumb == 0)
	{

		turret_slope_ = 0.0F;        // 角度の初期化


		if (!hold_flag_)
		{
			/* 入力がなかったら砲塔の向きを戻す */
			if (turret_direction_ > body_direction_) turret_direction_ -= performance_.MaxDirection;
			else if (turret_direction_ < body_direction_) turret_direction_ += performance_.MaxDirection;
		}
	}

	right_stick_list_.push_back(thumb);

	/* タレットの向きを決定する */
	turret_direction_ += (body_slope_ + turret_slope_);
}
/*****************************************
 * 関数名 : Accel関数                    *
 * 内  容 : プレイヤーの移動を管理します *
 * 引  数 : なし                         *
 * 戻り値 : なし                         *
 *****************************************/
VOID Player::Accel(VOID)
{
	INT engine_se = ENGINE_SE + player_no_;

	/* ボタン入力 */
	INT right_trigger = XInput::DownRightTrigger(player_no_);
	INT left_trigger  = XInput::DownLeftTrigger(player_no_);

	/* アクセルボタンが押されていたら */
	if (right_trigger > 0)
	{
		/* エンジン音の再生 */
		DSound::Play(engine_se);

		/* 徐々に加速を下げる */
		if      (move_ <= performance_.GearOne)   move_ += right_trigger * performance_.GearOneSpeed;    // 1速 
		else if (move_ <= performance_.GearTwo)   move_ += right_trigger * performance_.GearTwoSpeed;    // 2速
		else if (move_ <= performance_.GearThree) move_ += right_trigger * performance_.GearThreeSpeed;  // 3速
		else                                      move_ += right_trigger * performance_.GearForSpeed;    // 4速

		/* 最大速度になったら */
		if (move_ > max_speed_)
		{
			move_ -= performance_.Inertia ;  // 最高速を維持する

			/* 0以下になったら固定する */
			if (move_ <= 0.0F) move_ = 0.0F;
		}
	}
	/* アクセルボタンが離されたら */
	else if (move_ >= 0.0F)
	{
		DSound::Pause(engine_se);

		/* 徐々に速度を下げる */
		move_ -= performance_.Inertia;

		/* 0になったら固定する */
		if (move_ < 0.0F)  move_ = 0.0F;
	}
	/* ブレーキ処理 */
	if (left_trigger > 0)
	{

		/* 加速が残っていたら */
		if (move_ > 0.0F)
		{
			move_ -= performance_.Brake;

			/* 加速度が0以下になったら固定する */
			if (move_ < 0.0F) move_ = 0.0F;
		}
	}

	/* 向いている方向に移動 */
	body_position_.x += sin(body_direction_) * move_;
	body_position_.z += cos(body_direction_) * move_;
}