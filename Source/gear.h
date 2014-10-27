/************************
 * ギアを表示するクラス *
 ************************/
#ifndef RACE_GEAR_H_
#define RACE_GEAR_H_

/***********
 * Include *
 ***********/
#include "layout_base.h"

/* 定数 */

/************
 * 文字情報 *
 ************/
CONST INT kGearSize  = Small;
CONST INT kGearColor = Red;

CONST INT kNumberCount = 6;

/********
 * 配置 *
 ********/
CONST D3DXVECTOR3 kGearPos = D3DXVECTOR3(920.0F, 540.0F, 0.0F);

CONST D3DXVECTOR3 kGearNamePos = D3DXVECTOR3(11.0F, 12.0F, 0.0);

/****************
 * 切り取り矩形 *
 ****************/
CONST INT kGearStartX = 12,  kGearStartY = 200,
		  kGearEndX   = 114, kGearEndY   = 95;

/****************************************
 * ギアのスプライト表示を行うクラスです *
 ****************************************/
class Gear : public LayoutBase
{
private:
	INT gear_;

	VOID NameSet(VOID);
	VOID DrawSet(VOID);
public:
	Gear(VOID);         // コンストラクタ
	~Gear(VOID);        // デストラクタ

	VOID UpDate(VOID);  // 更新処理
	VOID Draw(VOID);    // 描画処理

	VOID SetGearCount(INT set_gear_) { gear_ = set_gear_; }
};

#endif