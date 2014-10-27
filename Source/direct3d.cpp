/***********
 * Include *
 ***********/
#include "direct3d.h"


/**********************
 * 静的メンバ変数宣言 *
 **********************/
LPDIRECT3D9            Direct3D::d3d_;         // Direct3Dインターフェイス
LPDIRECT3DDEVICE9      Direct3D::d3d_device_;  // デバイスのIDirect3Device9インターフェイスのポインタ
D3DPRESENT_PARAMETERS  Direct3D::present_;     // プレゼンテーションパラメータ
D3DCAPS9               Direct3D::caps_;        // ハードウェア能力格納構造体


/******************
 * コンストラクタ *
 ******************/
Direct3D::Direct3D( VOID )
{
	/* メンバ変数初期化 */
	d3d_        = NULL;
	d3d_device_ = NULL;
	ZeroMemory( &caps_, sizeof(caps_) );
	ZeroMemory( &present_, sizeof(present_) );
}

/*****************************************************
 * 関数名 : Init関数                                 *
 * 内  容 : ウィンドウの作成と各種ライブラリの初期化 *
 * 引  数 : CONST HWND hWnd：ウィンドウハンドル      *
 * 戻り値 : 初期化に成功するとTRUEを返す             *
 *****************************************************/
BOOL Direct3D::Init( CONST HWND hWnd )
{
    BOOL ret = TRUE;    // 戻り値用

	/* 既にデバイスが取得済みなら処理を抜ける */
	if( d3d_device_ ) return ret;
    
    do
    {
        /* Direct3Dの初期化 */
	    if( !(d3d_ = Direct3DCreate9( D3D_SDK_VERSION )) )
	    {
		    /* エラー */
            ErrorDialog( "Direct3Dインターフェイスの取得に失敗" );
            ret = FALSE;
            break;
	    }

        /* 各種パラメータの設定 */
        SetPresentParameters(hWnd);
        SetMultiSamples();

        /* D3Dデバイスの作成 */
        ret = CreateD3DDevice( hWnd );
        
    }while( 0 );

    return ret;
}

/***************************
 * 関数名 : Destroy関数    *
 * 内  容 : Direct3Dを破棄 *
 * 引  数 : なし           *
 * 戻り値 : なし           *
 ***************************/
VOID Direct3D::Destroy( VOID )
{
    SafeRelease( d3d_device_ );  // Direct3DDeviceインタフェース
    SafeRelease( d3d_ );         // Direct3Dインタフェース
}

/***********************************
 * 関数名 : CreateD3DDevice関数    *
 * 内  容 : Direct3Dデバイスを作成 * 
 * 引 数：なし                     *
 * 戻り値：なし                    *
 ***********************************/
BOOL Direct3D::CreateD3DDevice( CONST HWND hWnd )
{
    BOOL ret = TRUE;    // 戻り値用

    /* Direct3Dデバイスの作成 */
    if( FAILED(d3d_->CreateDevice(
            D3DADAPTER_DEFAULT,                   // アダプタの選択
            D3DDEVTYPE_HAL,                       // ハードウェアモード
            hWnd,                                 // 描画するウィンドウのハンドル
            D3DCREATE_HARDWARE_VERTEXPROCESSING,  // ハードによる頂点処理を指定
            &present_,                            // 初期化パラメータのポインタ
            &d3d_device_ )))                      // 取得したインタフェースの格納先ポインタ
	    {
            /* 失敗したらSOFTWAREで作成を試みる */
		    if( FAILED(d3d_->CreateDevice(
                D3DADAPTER_DEFAULT,                   // アダプタの選択
                D3DDEVTYPE_HAL,                       // ハードウェアモード
                hWnd,                                 // 描画するウィンドウのハンドル
                D3DCREATE_SOFTWARE_VERTEXPROCESSING,  // ソフトによる頂点処理を指定
                &present_,                            // 初期化パラメータのポインタ
                &d3d_device_ )))                      // 取得したインタフェースの格納先ポインタ
		    {
			    /* エラー（必要ならREFでの動作） */
                ErrorDialog( "D3Dデバイスの作成に失敗" );
                ret = FALSE;
		    }	
	    }   

    return ret;
}

/*************************************************
 * 関数名 : SetPresentParameters関数             *
 * 内  容 : 固定機能のレンダリングステートを設定 *
 * 引  数 : なし                                 *
 * 戻り値 : なし                                 *
 *************************************************/
VOID Direct3D::SetPresentParameters(CONST HWND hWnd)
{
    /* D3DPRESENT_PARAMETERS共通項目 */
	present_.SwapEffect              = D3DSWAPEFFECT_DISCARD;  // スワップエフェクト
	present_.EnableAutoDepthStencil	 = TRUE;                   // 深度：ステンシルバッファの有無
	present_.AutoDepthStencilFormat	 = D3DFMT_D24S8;           // 深度：ステンシルバッファのフォーマット）
	present_.MultiSampleType         = D3DMULTISAMPLE_NONE;    // アンチエイリアスレベル
#ifndef FULLSCREEN_MODE
	/* ウィンドウモードの設定 */
	present_.Windowed				= TRUE;                         // ウィンドウモードの指定
	present_.BackBufferFormat		= D3DFMT_UNKNOWN;                   // バックバッファのフォーマット
	present_.PresentationInterval	= D3DPRESENT_INTERVAL_DEFAULT;  // テアリング防止
#else
	/* フルスクリーン時の設定 */
	present_.Windowed					= FALSE;                        // ウィンドウモードの指定	
	present_.FullScreen_RefreshRateInHz	= D3DPRESENT_RATE_DEFAULT;      // リフレッシュレート
	present_.BackBufferWidth			= kWindowWidth;                // バックバッファ横幅
	present_.BackBufferHeight			= kWindowHeight;               // ウィンドウの縦幅
	present_.BackBufferFormat			= D3DFMT_X8R8G8B8;               // バックバッファフォーマット
	present_.BackBufferCount            = 1;                            // バックバッファの数
	present_.hDeviceWindow              = hWnd;                         // デバイスウィンドウ
	present_.PresentationInterval       = D3DPRESENT_INTERVAL_DEFAULT;  // テアリング防止
#endif
}

/***************************************
 * 関数名 : SetMultiSamples関数        *
 * 内  容 : アンチエイリアスを行います *
 * 引  数 : なし　                     *
 * 戻り値 : なし                       *
 ***************************************/
VOID Direct3D::SetMultiSamples( VOID )
{
    /* ディスプレイモードの取得 */
	D3DDISPLAYMODE display;
	if( FAILED( d3d_->GetAdapterDisplayMode( D3DADAPTER_DEFAULT, &display ) ))
	{
        /* エラー */
        ErrorDialog( "ディスプレイモードの取得に失敗" );
        return;
	}

    /* マルチサンプリングチェック */
	DWORD quality_buf = 0L;
	DWORD quality_z   = 0L;

    /* アンチエイリアスの設定 */
    for( INT i = D3DMULTISAMPLE_16_SAMPLES; i >= D3DMULTISAMPLE_NONE; i-- )
    {
        /* レンダリングターゲットアンチエイリアシングがサポートされているか調べる */
        if( SUCCEEDED( d3d_->CheckDeviceMultiSampleType(
            D3DADAPTER_DEFAULT,      // 問い合わせるディスプレイアダプターを示す序数
            D3DDEVTYPE_HAL,          // デバイスの種類
            display.Format,          // サーフェスのフォーマット
            present_.Windowed,       // フルウィンドウマルチサンプリング：TRUE, または、フルスクリーンアンチエイリアシング：FALSE, を問い合わせる 
            (D3DMULTISAMPLE_TYPE)i,	 // テストするマルチサンプリングテクニック
            &quality_buf ) ) )
		{
			/* 深度ステンシルでアンチエイリアシングがサポートされているか調べる */
			if( SUCCEEDED( d3d_->CheckDeviceMultiSampleType(
                D3DADAPTER_DEFAULT,      // 問い合わせるディスプレイアダプターを示す序数
                D3DDEVTYPE_HAL,          // デバイスの種類
                display.Format,          // サーフェスのフォーマット
                present_.Windowed,       // フルウィンドウマルチサンプリング：TRUE, または、フルスクリーンアンチエイリアシング：FALSE, を問い合わせる 
                (D3DMULTISAMPLE_TYPE)i,  // テストするマルチサンプリングテクニック
                &quality_z ) ) )
			{
				/* サポートされているレベルを設定 */
				present_.MultiSampleType = (D3DMULTISAMPLE_TYPE)i;

				/* MultiSampleQualityはレンダリングターゲットと深度ステンシルで値の小さい方を採用する。*/
				if( quality_buf < quality_z ) present_.MultiSampleQuality = quality_buf - 1L;
				else present_.MultiSampleQuality = quality_z - 1L;
				break;
			}
		}
	}
}