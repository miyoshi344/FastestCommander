/*********** 
 * Include *
 ***********/
#include "progress.h"

/******************
 * コンストラクタ *
 ******************/
Progress::Progress(VOID)
{
	/* 開始の遷移 */
	flow_no_ = TITLE;

	/* 作業番号ごとのクラス格納 */
	flow_[TITLE]      = new Title;      // タイトルクラス
	flow_[SOLO_GAME]  = new SoloGame;   // ソロゲームクラス
	flow_[MULTI_GAME] = new MultiGame;  // マルチゲームクラス
	flow_[GAME_OVER]  = new Ending;     // ゲームオーバークラス
	flow_[ENDING]     = new Ending;     // エンディングクラス
}

/****************
 * デストラクタ *
 ****************/
Progress::~Progress(VOID)
{
	/* 作業番号の内容を破棄する */
	for (INT i = 0; i < MAX_FLOW; i++)
	{
		/* 中身が存在するなら */
		if (flow_[i] != NULL)
		{
			/* 中身の破棄 */
			flow_[i]->Destroy();
			SafeDelete(flow_[i]);
		}
	}
}

/*********************************************
 * 関数名 : Init関数                         *
 * 内  容 : 初期化処理を行います             *
 * 引  数 : なし                             *
 * 戻り値 : 初期化に成功したらTRUEを返します *
 *********************************************/
BOOL Progress::Init(VOID)
{
	BOOL ret = TRUE;
	do
	{
		/* 確保したクラスを初期化する */
		for (INT i = 0; i < MAX_FLOW; i++)
		{
			/* 初期処理 */
			if (!flow_[i]->Init())
			{
				/* エラー */
				ErrorDialog("初期化に失敗しました");
				ret = FALSE;
				break;
			}
		}

	} while (0);

	return ret;
}

/*******************************
 * 関数名 : UpDate関数         *
 * 内  容 : 更新処理を行います *
 * 引  数 : なし               *
 * 戻り値 : なし               *
 *******************************/
VOID Progress::UpDate(VOID)
{
	/* 現在の作業番号を取得 */
	INT progression = flow_[flow_no_]->UpDate();

	/* 違う番号が返されたら */
	if (flow_no_ != progression)
	{
		/* 作業番号を切り替える */
		flow_no_ = progression;

		/* 初期化 */
		flow_[flow_no_]->ReSet();
	}
}

/*******************************
 * 関数名 : Draw関数           *
 * 内  容 : 描画処理を行います *
 * 引  数 : なし               *
 * 戻り値 : なし               *
 *******************************/
VOID Progress::Draw(VOID)
{
	/* 現在の作業番号の描画を行う */
	flow_[flow_no_]->Draw();
}

/*******************************
 * 関数名 : Destroy関数        *
 * 内  容 : 破棄処理を行います *
 * 引  数 : なし               *
 * 戻り値 : なし               *
 *******************************/
VOID Progress::Destroy(VOID)
{
	/* 作業番号ごとの内容を破棄する */
	for (INT i = 0; i < MAX_FLOW; i++)
	{
		/* 中身が存在するなら */
		if (flow_ != NULL)
		{
			/* 中身を破棄する */
			flow_[i]->Destroy();
			SafeDelete(flow_[i]);
		}
	}
}