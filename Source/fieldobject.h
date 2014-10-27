/**************************************
 * フィールドに表示するブロッククラス *
 **************************************/

/* 多重Include防止 */
#ifndef RACE_FIELDOBJECT_H_
#define RACE_FIELDOBJECT_H_

/***********
 * Include *
 ***********/
#include "objectbase.h"

/********
 * 定数 *
 ********/

/* 共通のメッシュ */
CONST LPCSTR kThorn       = "boxhalf_obj.x";
CONST LPSTR  kSlope       = "slope_a_obj.x";
CONST LPCSTR kWall        = "kibako.x";
CONST LPCSTR kCheck       = "check.x";

/* ステージごとのメッシュ */
CONST LPCSTR kGrassGround = "kusahara.x";
CONST LPCSTR kSnowGround  = "snow.x";
CONST LPCSTR kRockyGround = "iwaba.x";
CONST LPCSTR kSandGround  = "sand.x";

/* 各色のメッシュ */
CONST LPCSTR kRedBlock    = "boxhalf_red.x";
CONST LPCSTR kGreenBlock  = "boxhalf_green.x";
CONST LPCSTR kBlueBlock   = "boxhalf_blue.x";
CONST LPCSTR kYellowBlock = "boxhalf_yellow.x";

/****************************************************
 * フィールドに描画するブロックを管理するクラスです *
 ****************************************************/
class FieldObject : public ObjectBase
{
protected:
	LPCSTR mesh_name_;

	BOOL ObjectInit(VOID);
public:
	BOOL Init(VOID); // 初期化
	VOID UpDate(VOID);  // 更新処理
	VOID Draw(VOID);    // 描画処理
	VOID Destroy(VOID); // 破棄

	VOID SetMeshName(LPCSTR mesh_name){ mesh_name_ = mesh_name; }
};

#endif