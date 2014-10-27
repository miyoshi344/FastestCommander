/****************
 * Spriteクラス *
 ****************/

/* 多重Include防止 */
#ifndef RACE_SPRITE_H_
#define RACE_SPRITE_H_

/***********
 * Include *
 ***********/
#include "common.h"    // 共通クラス
#include "direct3d.h"  // Direct3Dクラス

/*******************
 * 定数/マクロ宣言 *
 *******************/
#define SPRITE  (Sprite::GetInstance())	// Spriteインターフェイス呼び出し簡易版

/**************
 * クラス宣言 *
 **************/
class Sprite
{
private:
	static LPD3DXSPRITE sprite_;  // スプライトインタフェース

public:
	Sprite(VOID);				// コンストラクタ
	static BOOL Init(VOID);		// 初期化
	static VOID Destroy(VOID);	// 破棄

	/* LPD3DXSPRITEインターフェイスを返す */
	static CONST LPD3DXSPRITE GetSprite(VOID) { return sprite_; }

	static VOID	DrawSprite(
			CONST LPDIRECT3DTEXTURE9 pTexture,                        // テクスチャインターフェイス
			CONST D3DXVECTOR3* pPos = &D3DXVECTOR3(0.0F, 0.0F, 0.0F), // 行列
			CONST RECT         rect = { 0, 0, 1280, 720 },            // 切り取り矩形
			CONST INT          Alpha = 255,                           // アルファ値
			CONST D3DXVECTOR3* pCenter = NULL);                       // 中心座標	
};

#endif