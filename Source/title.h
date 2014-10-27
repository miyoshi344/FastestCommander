/******************
 * タイトルクラス *
 ******************/

/* 多重Include防止 */
#ifndef RACE_TITLE_H_
#define RACE_TITLE_H_

/***********
 * Include *
 ***********/
#include "flow_base.h"
#include "game_base.h"
#include "demoplay.h"

/********
 * 定数 *
 ********/
/* テクスチャ名 */
CONST LPCSTR kTitleTexture = "Layout.png";

/* スタート用テクスチャ情報 */
CONST D3DXVECTOR3 kStartPosition       = D3DXVECTOR3(440.0F, 450.0F, 0.0F);
CONST D3DXVECTOR3 kTitlePlayerPosition = D3DXVECTOR3(250.0F, 500.0F, 0.0F);

/* タイトルテクスチャ情報 */
CONST D3DXVECTOR3 kTitlePosition = D3DXVECTOR3(100.0F, 50.0F, 0.0F);

/**************************************************
 * タイトルの管理を行うクラスです                 *
 * GameBaseとLayoutBaseクラスを継承して作成します *
 **************************************************/
class Title : public GameBase, public LayoutBase
{
private:

	DemoPlay demo_play_;

	/********
	 * 変数 *
	 ********/
	D3DXVECTOR3 position_;  // キャラクター座標
	
	BOOL start_button_;     // スタートフラグ
	BOOL button_flag_;      // ボタンが押されたかのフラグ

	INT count;              // 安定用カウント

	INT flag_count;         // 入力可能になる待機時間
	BOOL flag_;             // 入力可能フラグ

	/************
	 * 内部関数 *
	 ************/
	BOOL TextureInit(VOID);       // テクスチャ初期化
	BOOL ButtonUpDate(VOID);      // ボタン更新処理
	BOOL StartButton(VOID);       // スタート表示処理
	INT  PlayerSelect(VOID);      // プレイヤー人数選択処理
	VOID SpriteDraw(VOID);        // スプライト描画処理
	
public:
	Title(VOID);            // コンストラクタ
	~Title(VOID);           // デストラクタ

	BOOL Init(VOID);        // 初期設定
	VOID ReSet(VOID);       // 初期化処理
	BOOL UpDate(VOID);      // 更新処理
	VOID Draw(VOID);        // 描画処理
	VOID Destroy(VOID);     // 破棄処理
};

#endif