/******************
 * Direct3Dクラス *
 ******************/

/* 多重Include防止 */
#ifndef RACE_DIRECT3D_H_
#define RACE_DIRECT3D_H_

/***********
 * Include *
 ***********/
#include "common.h"
#include "release.h"

/***********************
 * 内部定数/マクロ定義 *
 ***********************/
#define D3DDEV (Direct3D::d3d_device()) // D3DDeviceメンバ呼び出し簡易版

/********************************
 * Direct3Dを管理するクラスです *
 ********************************/
class Direct3D
{
private:
    static VOID SetMultiSamples( VOID );             // アンチエイリアスの設定を行います
	static VOID SetPresentParameters(CONST HWND hWnd );        // D3DPRESENT_PARAMETERS構造体の設定を行います
    static BOOL CreateD3DDevice( CONST HWND hWnd );  // DirectDデバイスを作成します

	static LPDIRECT3D9            d3d_;              // Direct3Dインターフェイス
	static LPDIRECT3DDEVICE9      d3d_device_;       // デバイスのIDirect3Device9インターフェイスのポインタ
	static D3DPRESENT_PARAMETERS  present_;          // プレゼンテーションパラメータ
	static D3DCAPS9               caps_;             // ハードウェア能力格納構造体

public:
    Direct3D( VOID );                                // コンストラクタ
	static BOOL Init( CONST HWND hWnd );             // 初期化
	static VOID Destroy( VOID );                     // 破棄

	static CONST LPDIRECT3DDEVICE9 d3d_device( VOID ) { return d3d_device_; }  // D3DDeviceの取得
    static CONST LPDIRECT3D9 d3d( VOID ) { return d3d_; }                      // D3Dオブジェクトの取得
};

#endif