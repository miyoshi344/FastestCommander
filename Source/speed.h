/***************
 * speedクラス *
 ***************/

/* 多重Include防止 */
#ifndef RACE_SPEED_H_
#define RACE_SPEED_H_

/***********
 * Include *
 ***********/
#include "layout_base.h"

/********
 * 定数 *
 ********/

/* 配置座標 */
CONST D3DXVECTOR3 kSpeedPos      = D3DXVECTOR3(160.0F, 70.0F, 0.0F);  // 数字の座標調節用
CONST D3DXVECTOR3 kMultiSpeedPos = D3DXVECTOR3(-50.0F, 50.0F, 0.0F);  // 文字の座標調節用

/****************************************
 * スピードを管理するクラスです         *
 * LayoutBaseクラスを継承して作成します *
 ****************************************/
class Speed : public LayoutBase
{
private:

	/********
	 * 変数 *
	 ********/
	FLOAT speed_;

	/************
	 * 内部関数 *
	 ************/
	VOID SpeedCount(VOID);  // スピード表示

public:
	Speed(VOID);                    // コンストラクタ
	~Speed(VOID);                   // デストラクタ

	VOID UpDate(VOID);              // 更新処理
	VOID SoloDraw(VOID);            // 描画処理
	VOID TwoDraw(INT player_no);    // 2人用描画処理
	VOID MultiDraw(INT player_no);  // 多人数用描画処理

	/* プレイヤーの速度を表示 */
	VOID SetPlayerSpeed(FLOAT move) { speed_ = move; }
};

#endif