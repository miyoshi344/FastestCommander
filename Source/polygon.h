/******************
 * ポリゴンクラス *
 ******************/

/***********
 * Include *
 ***********/
#include "common.h"
#include "direct3d.h"

/* 多重Include防止 */
#ifndef RACE_POLYGON_H_
#define RACE_POLYGON_H_

/**********
 * 構造体 *
 **********/
struct COSTOMVERTEX
{
	D3DXVECTOR3 pos_;
	D3DCOLOR    color_;
	D3DXVECTOR2 uv;
};

/**********************************
 * 板ポリゴンを作成するクラスです *
 **********************************/
class PolygonPlate
{
private:
	/* 変数 */
	LPDIRECT3DVERTEXBUFFER9		 vertex_buffer_; // 頂点バッファ
	LPDIRECT3DINDEXBUFFER9       index_buffer_;  // インデックスバッファ
	LPDIRECT3DVERTEXDECLARATION9 declaration_;	 // 頂点デクラレーション

	LPDIRECT3DTEXTURE9 p_texture_;                  // テクスチャ
	LPDIRECT3DSURFACE9 p_texture_render_target_;    // レンダーターゲット
	LPDIRECT3DSURFACE9 p_texture_stencil_surface_;  // ステンシルサーフェイス

	/* ゲーム処理 */
	LPDIRECT3DSURFACE9	p_before_render_target,
		                p_before_stencil_surface;
public:
	PolygonPlate(VOID);    // コンストラクタ
	~PolygonPlate(VOID);   // デストラクタ

	BOOL Init(VOID);  // 初期化処理
	VOID TextureDraw(VOID);
	VOID ReturnDraw(VOID);
	VOID Draw(VOID);
	VOID Destroy(VOID);
};

#endif