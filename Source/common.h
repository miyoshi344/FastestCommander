/**************
 * 共通クラス *
 **************/

/* 多重Include防止 */
#ifndef RACE_COMMON_H_
#define RACE_COMMON_H_

#define	DIRECTINPUT_VERSION	0x0800
#pragma warning(disable:4996)

/***********
 * Include *
 ***********/
/* Windows */
#include <windows.h> 
#include <mmsystem.h>
#include <stdio.h>
#include <crtdbg.h>
#include <direct.h>

/* STL */
#include <vector>
#include <list>
#include <map>
#include <bitset>
#include <string>
#include <iostream>

/* DirectX */
#include <d3d9.h>
#include <d3dx9.h>
#include <dinput.h>
#include <DxErr.h>

/***********************
 * 内部変数/マクロ定義 *
 ***********************/
#define	WIN32_LEAN_AND_MEAN	 // Windowsヘッダから使用しないヘッダ情報を省略
#ifndef STRICT               // 型チェック厳密定義
#define STRICT
#endif
#ifndef DIRECTINPUT_VERSION  // DirectInputバージョン指定
#define DIRECTINPUT_VERSION 0x800
#endif

#define _CRTDBG_MAP_ALLOC
#define METRIC_MODE

/* ウィンドウサイズ */
//#define FULLSCREEN_MODE  // フルスクリーンモード

#ifndef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>
#define DEBUG_CLIENTBLOCK new (_CLIENT_BLOCK, __FILE__, __LINE__)
#define new DEBUG_CLIENTBLOCK
#endif



CONST INT  kWindowWidth   = 1280;  // ウィンドウ横幅
CONST INT  kWindowHeight  = 720;   // ウィンドウ縦幅

CONST LPSTR kWindowName   = "FastestCommander";  // ウィンドウタイトル
CONST LPSTR kClassName    = "FastestCommander";  // ウィンドウクラスネーム

CONST INT TEXTURE_NUM     = 30;      // 最大テクスチャ管理枚数
CONST INT MESH_NUM        = 30;      // 最大メッシュ管理数
CONST UINT NAME_LEN		  = 32767;   // 文字列長
CONST INT MAX_ALPHA       = 255;     // 最大アルファ値

CONST INT   kAllPlayer = 4;      // プレイヤー最大数
CONST FLOAT kFieldDraw = 35.0F;  // 描画範囲

/* 画面分割用 */
CONST D3DXVECTOR3 kMultiWindow = D3DXVECTOR3((kWindowWidth / 2), (kWindowHeight / 2), 0.0F);

/********
 * 列挙 *
 ********/

/* 地面のブロック属性 */
enum
{
	THORN,   // 黒地面
	GROUND,  // 通常地面 
	SLOPE,   // 坂

	/* 壁のブロック属性 */
	WALL,    // 壁 
	DUMMY,   // ダミー壁
	CHECK,   // チェックポイント

	/* 色の付いた地面のブロック属性 */
	RED_BLOCK,     // 赤ブロック
	GREEN_BLOCK,   // 緑ブロック
	YELLOW_BLOCK,  // 黄ブロック
	BLUE_BLOCK,    // 青ブロック

	BLOCK_MAX,  // ブロックの最大数
};

/* フィールドの種類 */
enum
{
	GRASS,     // 草原
	SNOW,      // 雪原
	MOUNTAIN,  // 山脈
	BEACH,     // 浜辺
	MAX_FIELD, 
};

/* プレイヤーの種類 */
enum
{
	RED_TANK,     // 赤タンク
	GREEN_TANK,   // 緑タンク
	YELLOW_TANK,  // 黄タンク
	BLUE_TANK,    // 青タンク
	TANK_ALL,
};

/**********************************
 * グローバル関数プロトタイプ宣言 *
 **********************************/
VOID ErrorDialog( LPCTSTR Message ); // メッセージボックスの表示

#endif