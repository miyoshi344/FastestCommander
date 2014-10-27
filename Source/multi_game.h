/****************
 * ゲームクラス *
 ****************/

/* 多重Include防止 */
#ifndef RACE_MULTI_GAME_H_
#define RACE_MULTI_GAME_H_

/***********
 * Include *
 ***********/
#include "game_base.h"

/********
 * 定数 *
 ********/
CONST LPCSTR k2PlayEdge = "2PlayEdge.png";  // 2人用フレーム
CONST LPCSTR k4PlayEdge = "4PlayEdge.png";  // 3人以上用フレーム

/******************************************
 * ゲームのメイン部分を管理するクラスです *
 * GameBaseを継承して作成します           *
 ******************************************/
class MultiGame : public GameBase
{
private:
	/********
	 * 変数 *
	 ********/

	/* 画面構成 */
	D3DVIEWPORT9 port_[kAllPlayer];  // ビューポート
	D3DXVECTOR3 aspect_;             // アスペクト

	/* フレーム用テクスチャ */
	LPDIRECT3DTEXTURE9 player2_edge_;  // 2人用フレーム
	LPDIRECT3DTEXTURE9 player4_edge_;  // 3人以上用フレーム

	/************
	 * 内部関数 *
	 ************/
	BOOL TextureInit(VOID);  // テクスチャ初期化
	VOID ViewPort(VOID);     // ビューの設定
	VOID MultiDraw(VOID);    // 描画処理
	VOID SpriteDraw(VOID);   // スプライト描画処理

public:
	MultiGame(VOID);   // コンストラクタ
	~MultiGame(VOID);  // デストラクタ

	BOOL Init(VOID);   // 初期処理
	VOID ReSet(VOID);  // 初期化処理
	INT  UpDate(VOID); // 更新処理
	VOID Draw(VOID);   // 描画処理
};

#endif