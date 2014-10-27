/***********
 * Include *
 ***********/
#include "player_ai.h"

/******************
 * コンストラクタ *
 ******************/
PlayerAI::PlayerAI(VOID)
{
}

/****************
 * デストラクタ *
 ****************/
PlayerAI::~PlayerAI(VOID)
{
}

/*******************************************
 * 関数名 : Init関数                       *
 * 内  容 : 初期処理を行います             *
 * 引  数 : なし                           *
 * 戻り値 : 処理に成功したらTRUEを返します *
 *******************************************/
BOOL PlayerAI::Init(VOID)
{
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

		/* AI情報を取得 */
		//if (!AiInit())
		//{
		//	/* エラー */
		//	ErrorDialog("AI情報の取得に失敗");
		//	ret = FALSE;
		//	break;
		//}

	} while (0);

	return ret;
}

/*******************************
 * 関数名 : UpDate関数         *
 * 内  容 : 更新処理を行います *
 * 引  数 : なし               *
 * 戻り値 : なし               *
 *******************************/
VOID PlayerAI::UpDate(VOID)
{
	Jump();             // ジャンプ
	Direction();        // キャラクター旋回
	TurretDirection();  // タレット旋回
	Accel();            // アクセル

	/* プレイヤー回りの描画判定の設定 */
	draw_pos_ = body_position_;
	draw_pos_.x += sinf(body_direction_) * kDrawPosition;
	draw_pos_.z += cosf(body_direction_) * kDrawPosition;

	bullet_.UpDate();  // 弾の更新
	if (bullet_flag_)bullet_pos_ = bullet_.ReturnPosition();
}

/***********************************
 * 関数名 : Jump関数               *
 * 内  容 : ジャンプ処理を行います *
 * 引  数 : なし                   *
 * 戻り値 : なし                   *
 ***********************************/
VOID PlayerAI::Jump(VOID)
{
	/* 地面にいてボタンフラグが立っていれば */

	/* 座標の更新 */
	body_position_.y += jump_;      // 座標の更新
	jump_ -= performance_.Gravity;  // ジャンプ値を徐々に下げる
	origin_pos_ = body_position_;   // 移動前の座標を設定
}

/***********************************************
 * 関数名 : Direction関数                      *
 * 内  容 : キャラクターの向きの管理を行います *
 * 引  数 : なし                               *
 * 戻り値 : なし                               *
 ***********************************************/
VOID PlayerAI::Direction(VOID)
{
	/* 向きを設定する */
	body_direction_ += body_slope_;
}

/***************************************
 * 関数名 : TurretDirection関数        *
 * 内  容 : タレットの向きを管理します *
 * 引  数 : なし                       *
 * 戻り値 : なし                       *
 ***************************************/
VOID PlayerAI::TurretDirection(VOID)
{
	/* タレットの向きを決定する */
	turret_direction_ += (body_slope_ + turret_slope_);
}

/*********************************
 * 関数名 : Accel関数            *
 * 内  容 : 移動の管理を行います *
 * 引  数 : なし                 *
 * 戻り値 : なし                 *
 *********************************/
VOID PlayerAI::Accel(VOID)
{
	/* 向いている方向に移動 */
	body_position_.x += sin(body_direction_) * move_;
	body_position_.z += cos(body_direction_) * move_;
}

/***********************************************
 * 関数名 : AiInit関数                         *
 * 内  容 : AIテキストを読み込み保存します     *
 * 引  数 : なし                               *
 * 戻り値 : 読み込みに成功したらTRUEを返します *
 ***********************************************/
BOOL PlayerAI::AiInit(VOID)
{
	BOOL ret = TRUE;  // 戻り値用
	FILE* fp;         // ファイルポインタ
	CHAR file_name[NAME_LEN];

	INT left_stcik,
		right_stick,
		accel,
		brake,
		jump;

	_chdir("Resource_fast/AI/green_ai");

	do
	{
		/* マップ番号の指定 */
		sprintf_s(file_name, "MapAI%d.txt");

		/* ファイルの読み込み */
		fopen_s(&fp, file_name, "r");

		/* ファイルが読み込めたかの確認 */
		if (!fp)
		{
			/* エラー */
			ErrorDialog("AIのファイル読み込みに失敗しました");
			ret = FALSE;
			break;
		}

		while (fscanf_s(fp, "%d %d %d %d %d", &left_stcik, &right_stick, &accel, &brake, &jump) == EOF)
		{
			left_stick_list_.push_back(left_stcik);
			right_stick_list_.push_back(right_stick);
			accel_list_.push_back(accel);
			brake_list_.push_back(brake);
			jump_list_.push_back(jump);
		}

	} while (0);

	_chdir("../../../");

	return ret;
}