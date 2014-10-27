/***********
 * Include *
 ***********/
#include "bullet.h"

/******************
 * コンストラクタ *
 ******************/
Bullet::Bullet(VOID)
{
	speed_ = kBurretSpeed;  // 弾のスピード初期化
}

/****************
 * デストラクタ *
 ****************/
Bullet::~Bullet(VOID)
{
	Mesh::Release(my_mesh_);  // メッシュ破棄
}

/*************************************
 * 関数名 : Init関数                 *
 * 内  容 : 初期化処理を行います     *
 * 引  数 : なし                     *
 * 戻り値 : 成功したらTRUEを返します *
 *************************************/
BOOL Bullet::Init(VOID)
{
	BOOL ret = TRUE;  // 戻り値用

	do
	{
		/* 弾のメッシュ情報取得 */
		if (!BulletMeshInit())
		{
			ErrorDialog("弾の初期化に失敗");
			ret = FALSE;
			break;
		}

	} while (0);

	return ret;
}

/***********************************
 * 関数名 : ReSet関数              *
 * 内  容 : 数値の初期化を行います *
 * 引  数 : なし                   *
 * 戻り値 : なし                   *
 ***********************************/
VOID Bullet::ReSet(VOID)
{
	position_ = D3DXVECTOR3(0.0F, 0.0F, 0.0F);
}

/*******************************
 * 関数名 : UpDate関数         *
 * 内  容 : 更新処理を行います *
 * 引  数 : なし               *
 * 戻り値 : なし               *
 *******************************/
VOID Bullet::UpDate(VOID)
{
	/* 弾の移動処理 */
	BulletMove();
}

/*******************************
 * 関数名 : Draw関数           *
 * 内  容 : 描画処理を行います *
 * 引  数 : なし               *
 * 戻り値 : なし               *
 *******************************/
VOID Bullet::Draw(VOID)
{
	/* 弾の描画処理 */
	BulletDraw();
}

/*******************************
 * 関数名 : Destroy関数        *
 * 内  容 : 破棄処理を行います *
 * 引  数 : なし               *
 * 戻り値 : なし               *
 *******************************/
VOID Bullet::Destroy(VOID)
{
	/* メッシュの解放 */
	Mesh::Release(my_mesh_);
}

/***************************************************
 * 関数名 : BulletSet関数                          *
 * 内  容 : 弾の設定を行います                     *
 * 引  数 : D3DXVECTOR3 pos       弾を発射する座標 *
 *          FLOAT       direction 向き             *
 *          FLOAT       move      加速度           *
 ***************************************************/
VOID Bullet::BulletSet(D3DXVECTOR3 pos, FLOAT direction, FLOAT move)
{
	/* 弾の座標を設定する */
	position_    = pos;
	position_.x += sin(direction) * 1.0F;
	position_.z += cos(direction) * 1.0F;
	position_.y += kBurretHeight;

	/* 高さを設定する */
	jump_ = kBurretJump;

	/* 向きと加速度を設定する */
	direction_ = direction;
	move_ = move;
}

/*******************************************
 * 関数名 : BulletMeshInit関数             *
 * 内  容 : 弾のメッシュ情報を取得します   *
 * 引  数 : なし                           *
 * 戻り値 : 取得に成功したらTRUEを返します *
 *******************************************/
BOOL Bullet::BulletMeshInit(VOID)
{
	BOOL ret = TRUE;

	/* フォルダ階層を移動する */
	_chdir("Resource_fast/Mesh/Bullet");

	/* 弾薬メッシュの取得 */
	if ((my_mesh_ = Mesh::Load(bullet_name_)) == NULL)
	{
		/* プレイヤー用弾幕メッシュの情報取得に失敗 */
		ErrorDialog("弾薬メッシュの情報取得に失敗しました");
		ret = FALSE;
	}

	radius_ = collision_.SphereCreate(my_mesh_);

	/* フォルダ階層を戻す */
	_chdir("../../../");

	return ret;
}

/***********************************
 * 関数名 : BulletMove関数         *
 * 内  容 : 弾の移動処理を行います *
 * 引  数 : なし                   *
 * 戻り値 : なし                   *
 ***********************************/
VOID Bullet::BulletMove(VOID)
{
	origin_pos_ = position_;

	/* 向いている方向に移動 */
	position_.x += sin(direction_) * (move_ + speed_);
	position_.z += cos(direction_) * (move_ + speed_);

	/* 高さを調整する */
	position_.y += jump_;
	jump_ -= kBurretGravity;
}

/***********************************
 * 関数名 : BulletDraw関数         *
 * 内  容 : 弾の描画処理を行います *
 * 引  数 : なし                   *
 * 戻り値 : なし                   *
 ***********************************/
VOID Bullet::BulletDraw(VOID)
{
	/* 座標と角度を設定 */
	D3DXMATRIX pos_, rota_;

	/* ワールド座標系を作成 */
	D3DXMatrixIdentity(&world_);
	D3DXMatrixRotationY(&rota_, direction_);
	D3DXMatrixTranslation(&pos_, position_.x, position_.y, position_.z);

	/* 行列を掛け合わせる */
	D3DXMatrixMultiply(&world_, &rota_, &pos_);

	/* 描画 */
	Mesh::Draw(world_, my_mesh_);


}