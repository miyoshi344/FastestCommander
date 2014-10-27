/****************************
 * プレイヤーセレクトクラス *
 ****************************/

/* 多重Include防止 */
#ifndef RACE_PLAYER_SELECT_H_
#define RACE_PLAYER_SELECT_H_

/***********
 * Include *
 ***********/
#include "common.h"
#include "sprite.h"
#include "xinput.h"
#include "flow_base.h"

/********
 * 定数 *
 ********/

/****************************************
 * プレイヤーの人数を管理するクラスです *
 ****************************************/
class PlayerSelect : public FlowBase
{
private:

	VOID SpriteDraw(VOID);
public:
	PlayerSelect(VOID);
	~PlayerSelect(VOID);

	BOOL Init(VOID);
	VOID ReSet(VOID);
	INT UpDate(VOID);
	VOID Draw(VOID);
	VOID Destroy(VOID);
};

#endif