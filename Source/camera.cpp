/***********
 * Include *
 ***********/
#include "camera.h"

/******************
 * コンストラクタ *
 ******************/
Camera::Camera(VOID)
{
}

/****************
 * デストラクタ *
 ****************/
Camera::~Camera(VOID)
{
}

/*******************************
 * 関数名 : Init関数           *
 * 内  容 : カメラの設定を行う *
 * 引  数 : なし               *
 * 戻り値 : なし               *
 *******************************/
BOOL Camera::Init(VOID)
{
	/* 単位行列で初期化 */
	D3DXMatrixIdentity(&proj_);

	/* 射影行列を作成 */
	D3DXMatrixPerspectiveFovLH(&proj_,
		D3DXToRadian(45.0F),                           // 画角
		((FLOAT)kWindowWidth / (FLOAT)kWindowHeight),  // 画面比率
		Near,                                          // nearクリップ面
		Far);                                          // farクリップ面

	/* 作成した射影行列を設定する */
	D3DDEV->SetTransform(D3DTS_PROJECTION, &proj_);

	/* カメラの座標、視点、向きを決めるビュー行列を作成 */
	D3DXMatrixLookAtLH(&view_,
		&kCameraPosition,    // カメラ座標
		&kCameraPrev,        // 注視点座標
		&kCameraDirection);  // カメラの向き状態（上）

	/* 作成したビュー行列を設定する */
	D3DDEV->SetTransform(D3DTS_VIEW, &view_);

	return TRUE;
}

/*******************************
 * 関数名 : UpDate関数         *
 * 内  容 : 更新処理を行います *
 * 引  数 : なし               *
 * 戻り値 : なし               *
 *******************************/
VOID Camera::UpDate(VOID)
{
	Light();

	D3DXVECTOR3 camera_position_;
	D3DXVECTOR3 look_point_;

	D3DXMatrixIdentity(&view_);

	camera_position_ = D3DXVECTOR3(
		position_.x + sinf(direction_) * -kCameraPosition.x,
		kCameraPosition.y + position_.y,
		position_.z + cosf(direction_) * -kCameraPosition.z
		);

	look_point_ = D3DXVECTOR3(
		position_.x + sinf(direction_) * kCameraPosition.x,
		kCameraPosition.y + position_.y,
		position_.z + cosf(direction_) * kCameraPosition.z
		);


	// ポジションと焦点を求める
	D3DXVECTOR3 prev_position = pos_,
		new_position = prev_position + (camera_position_ - prev_position),
		new_look_at_pos = point_ + (look_point_ - point_);

	/* 新しく作った座標に更新 */
	pos_   = new_position;
	point_ = new_look_at_pos;

	/* カメラの座標、視点、向きを決めるビュー行列を作成 */
	D3DXMatrixLookAtLH(&view_,
		&new_position,       // カメラ座標
		&new_look_at_pos,    // 注視点座標
		&kCameraDirection);  // カメラの向き状態（上）

	/* 作成したビュー行列を設定する */
	D3DDEV->SetTransform(D3DTS_VIEW, &view_);
}

/************************************************************
 * 関数名 : Light関数                                       *
 * 内  容 : 光の当たり方の設定                              *
 * 引  数 : なし                                            *
 * 戻り値 : なし                                            *
 ************************************************************/
VOID Camera::Light(VOID)
{
	// CONST D3DXCOLOR Diffuse(光の当たり部分の色)
	// CONST D3DXCOLOR Ambient(光の当たらない部分の色)
	// CONST D3DXVECTOR3 Direction(光の当たる向き)

	/* ライティングの設定 */
	D3DLIGHT9 light_;
	ZeroMemory(&light_, sizeof(light_));  // 初期化
	light_.Type = D3DLIGHT_DIRECTIONAL;   // 平行光源

	/* 光が当たる部分の色 */
	light_.Diffuse.r = LightDiffuse.r;  // 赤
	light_.Diffuse.g = LightDiffuse.g;  // 緑
	light_.Diffuse.b = LightDiffuse.b;  // 青
	light_.Diffuse.a = LightDiffuse.a;  // α値(透明度)

	/* 光が当たらない部分の色 */
	light_.Ambient.r = Ambient.r;  // 赤
	light_.Ambient.g = Ambient.g;  // 緑
	light_.Ambient.b = Ambient.b;  // 青
	light_.Ambient.a = Ambient.a;  // α値(透明度)

	/* 光の当たる向きを設定 */
	light_.Direction.x = ColorDirection.x;
	light_.Direction.y = ColorDirection.y;
	light_.Direction.z = ColorDirection.z;

	/* 作った設定を入れる */
	D3DDEV->SetLight(0, &light_);  // 引数には個数を入力
	D3DDEV->LightEnable(0, TRUE);  // 引数で0　TRUEを入れる

	/* レンダリングステートの設定 */
	D3DDEV->SetRenderState(D3DRS_LIGHTING, TRUE);         // ライティング無効
	D3DDEV->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW );  // カリングモード
}

/*********************************************
 * 関数名 : Aspect関数                       *
 * 内  容 : アスペクト比を設定しなおします   *
 * 引  数 : D3DXVECTOR3 aspect　アスペクト比 *
 * 戻り値 : なし                             *
 *********************************************/
VOID Camera::Aspect(D3DXVECTOR3 aspect)
{
	/* 単位行列で初期化 */
	D3DXMatrixIdentity(&proj_);

	/* 射影行列を作成 */
	D3DXMatrixPerspectiveFovLH(&proj_,
		D3DXToRadian(45.0F),   // 画角
		(aspect.x / aspect.y), // 画面比率
		Near,                  // nearクリップ面
		Far);                  // farクリップ面

	/* 作成した射影行列を設定する */
	D3DDEV->SetTransform(D3DTS_PROJECTION, &proj_);
}