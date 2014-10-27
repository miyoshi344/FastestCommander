/******************************
* ゲーム部分を管理するクラス *
******************************/

/* 多重Include防止 */
#ifndef RACE_PROGRESS_H_
#define RACE_PROGRESS_H_

/***********
* Include *
***********/
#include "flow_base.h"
#include "title.h"
#include "game_base.h"
#include "solo_game.h"
#include "multi_game.h"
#include "ending.h"

/**********************************
* ゲーム部分を管理するクラスです *
**********************************/
class Progress
{
private:

	/**************
	 * クラス定義 *
	 **************/
	FlowBase* flow_[MAX_FLOW];  // 作業ごとのクラス格納用ポインタ

	/********
	 * 変数 *
	 ********/
	INT flow_no_;     // 作業中の遷移

public:
	Progress(VOID);    // コンストラクタ
	~Progress(VOID);   // デストラクタ

	BOOL Init(VOID);     // 初期化処理
	VOID UpDate(VOID);   // 更新処理
	VOID Draw(VOID);     // 描画処理
	VOID Destroy(VOID);  // 破棄処理
};

#endif