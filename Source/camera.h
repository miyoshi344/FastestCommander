/****************
 * カメラクラス *
 ****************/

/* 多重Include防止 */
#ifndef RACE_CAMERA_H_
#define RACE_CAMERA_H_

/***********
 * Include *
 ***********/
#include "common.h"
#include "direct3d.h"
#include "framerate.h"
#include "objectbase.h"

/*********************
 * 定数 / マクロ提議 *
 *********************/
CONST D3DXVECTOR3 kCameraPosition  = D3DXVECTOR3(8.5F, 2.7F, 8.5F);  // カメラの位置
CONST D3DXVECTOR3 kCameraPrev      = D3DXVECTOR3(0.0F, 0.0F, 0.0F);  // カメラの注視点
CONST D3DXVECTOR3 kCameraDirection = D3DXVECTOR3(0.0F, 1.0F, 0.0F);  // カメラの向き

CONST D3DXCOLOR   LightDiffuse     = D3DXCOLOR(1.0F, 1.0F, 1.0F, 1.0F);  // 光の当たる部分
CONST D3DXCOLOR   Ambient          = D3DXCOLOR(0.5F, 0.5F, 0.5F, 1.0F);  // 光が当たらない部分
CONST D3DXVECTOR3 ColorDirection   = D3DXVECTOR3(0.0F, -20.0F, 0.0F);    // 光の向き

CONST FLOAT Near = 1.0F;    // カメラのnear値
CONST FLOAT Far  = 1000.0F; // カメラのfar値

/********************************
 * カメラの管理を行うクラスです *
 ********************************/
class Camera
{
private:

	/********
	 * 変数 *
	 ********/
	D3DXVECTOR3 position_;     // カメラ座標変換用
	D3DXMATRIX  view_, proj_;  // ワールド座標変換用

	D3DXVECTOR3 pos_;    // カメラ座標
	D3DXVECTOR3 point_;  // カメラ注視点

	FLOAT direction_;    // プレイヤー角度格納用

	/************
	 * 内部関数 *
	 ************/
	VOID Light(VOID); // 光源
	
public:
	Camera(VOID);       // コンストラクタ 
	~Camera(VOID);      // デストラクタ

	BOOL Init(VOID);    // カメラの初期配置
	VOID UpDate(VOID);  // 更新処理 

	VOID Aspect(D3DXVECTOR3 aspect);  // アスペクト比設定
	
	/* プレイヤーからの情報取得 */
	VOID Direction(FLOAT direction) { direction_ = direction; }
	VOID SetPosition(D3DXVECTOR3 pos) { position_ = pos; }
};
#endif