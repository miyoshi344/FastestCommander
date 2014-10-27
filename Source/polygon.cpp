/***********
 * Include *
 ***********/
#include "polygon.h"

/******************
 * コンストラクタ *
 ******************/
PolygonPlate::PolygonPlate(VOID)
{
}

/****************
 * デストラクタ *
 ****************/
PolygonPlate::~PolygonPlate(VOID)
{
}

/*********************************************
 * 関数名 : Init関数                         *
 * 内  容 : 初期化処理を行います             *
 * 引  数 : なし                             *
 * 戻り値 : 初期化に成功したらTRUEを返します *
 *********************************************/
BOOL PolygonPlate::Init(VOID)
{
	BOOL ret = TRUE;

	do
	{
		/* 頂点配列 */
		COSTOMVERTEX vertex[] = {
			{ D3DXVECTOR3(0.0F, 0.0F, 0.0F), D3DCOLOR_RGBA(255,   0,   0, 255), D3DXVECTOR2(0.0F, 1.0F)},
			{ D3DXVECTOR3(0.0F, 1.0F, 0.0F), D3DCOLOR_RGBA(  0, 255,   0, 255), D3DXVECTOR2(0.0F, 0.0F)},
			{ D3DXVECTOR3(1.0F, 1.0F, 0.0F), D3DCOLOR_RGBA(  0,   0, 255, 255), D3DXVECTOR2(1.0F, 0.0F)},
			{ D3DXVECTOR3(1.0F, 0.0F, 0.0F), D3DCOLOR_RGBA(255, 255, 255, 255), D3DXVECTOR2(1.0F, 1.0F)},
		};

		/* レンダーターゲット用テクスチャの作成	*/
		if (FAILED(D3DXCreateTexture(
			D3DDEV,                 // Direct3DDevice
			kWindowWidth,           // テクスチャの幅
			kWindowHeight,          // テクスチャの高さ
			0,                      // 0で固定
			D3DUSAGE_RENDERTARGET,  // 使用用途の設定)
			D3DFMT_A8R8G8B8,        // 式数フォーマットの設定
			D3DPOOL_DEFAULT,        // D3DPOOL_MANAGED固定
			&p_texture_             // 格納先ポインタ
			)))
		{
			/* エラー */
			ErrorDialog("レンダーターゲット用テクスチャの作成に失敗");
			ret = FALSE;
			break;
		}

		/* テクスチャ情報を取得 */
		D3DSURFACE_DESC desc;

		p_texture_->GetLevelDesc(0, &desc);
		p_texture_->GetSurfaceLevel(0, &p_texture_render_target_);

		/* 深度バッファの設定 */
		if (FAILED(D3DDEV->CreateDepthStencilSurface(
			desc.Width,                   // テクスチャの幅
			desc.Height,                  // テクスチャの高さ
			D3DFMT_D16,                   // D3DFMT_D16で固定
			D3DMULTISAMPLE_NONE,          // D3DMULTISAMPLE_NONEで固定
			0,                            // 0で固定
			TRUE,                         // TRUEで固定
			&p_texture_stencil_surface_,  // 格納先サーフェイスポインタ
			NULL
			)))
		{
			/* エラー */
			ErrorDialog("深度バッファの作成に失敗");
			ret = FALSE;
			break;
		}

		/* 頂点ストリームの作成 */
		D3DVERTEXELEMENT9 elements[] =
		{
			{
				0,                      // ストリーム番号
				0,                      // オフセット値
				D3DDECLTYPE_FLOAT3,     // データ型
				D3DDECLMETHOD_DEFAULT,  // 頂点処理タイプ
				D3DDECLUSAGE_POSITION,  // データタイプ
				0                       // 0
			},
			{
				0,                      // ストリーム番号
				12,                     // オフセット値
				D3DDECLTYPE_FLOAT2,     // データ型
				D3DDECLMETHOD_DEFAULT,  // 頂点処理タイプ
				D3DDECLUSAGE_COLOR,     // データタイプ
				0,
			},
			{
				0,                      // ストリーム番号
				16,                     // オフセット値
				D3DDECLTYPE_FLOAT2,     // データ型
				D3DDECLMETHOD_DEFAULT,  // 頂点処理タイプ
				D3DDECLUSAGE_TEXCOORD,  // データタイプ
				0,
			},

			D3DDECL_END()
		};

		/* 頂点デクラレーションの作成 */
		D3DDEV->CreateVertexDeclaration(elements, &declaration_);

		/* 頂点バッファの作成 */
		if (FAILED(D3DDEV->CreateVertexBuffer(
			4 * sizeof(COSTOMVERTEX),  // 頂点のバッファサイズ
			D3DUSAGE_WRITEONLY,        // 頂点バッファの使用方法
			0,                         // 0で固定
			D3DPOOL_MANAGED,           // D3DPOOL_MANAGEDで固定
			&vertex_buffer_,           // 頂点バッファの格納先ポインタ
			NULL
			)))
		{
			/* エラー */
			ErrorDialog("頂点バッファの作成に失敗");
			ret = FALSE;
			break;
		}

		/* 頂点バッファをロックしてメモリへのポインタを取得する */
		COSTOMVERTEX* p_vertex;
		vertex_buffer_->Lock(0, 0, (VOID**)&p_vertex, 0);

		/* 内容をコピーする */
		memcpy(p_vertex, vertex, sizeof(vertex));

		/* ロック解除 */
		vertex_buffer_->Unlock();

		/* インデックスを配列定義 */
		WORD indeices[] = { 0, 1, 2, 3 };

		/* 頂点インデックスバッファの作成 */
		if (FAILED(D3DDEV->CreateIndexBuffer(
			4 * sizeof(WORD),    // インデックスの数を定義
			D3DUSAGE_WRITEONLY,  // D3DUSAGE_WRITEONLYで固定
			D3DFMT_INDEX16,      // D3DFMT_INDEX16で固定
			D3DPOOL_MANAGED,     // D3DPOOL_MANAGEDで固定
			&index_buffer_,      // インデックスバッファの格納先を指定
			NULL                 // NULLで固定
			)))
		{
			/* エラー */
			ErrorDialog("頂点インデックスの作成に失敗");
			ret = FALSE;
			break;
		}

		/* 頂点バッファをロックしてメモリへのポインタを取得する */
		COSTOMVERTEX* p_index;
		index_buffer_->Lock(0, 0, (VOID**)&p_index, 0);

		/* 内容をコピーする */
		memcpy(p_index, indeices, sizeof(indeices));

		/* ロック解除 */
		index_buffer_->Unlock();

	} while (0);

	return ret;
}

/*********************************************
 * 関数名 : TextureDraw関数                  *
 * 内  容 : 作った板ポリゴンに描画を行います *
 * 引  数 : なし                             *
 * 戻り値 : なし                             *
 *********************************************/
VOID PolygonPlate::TextureDraw(VOID)
{
	/* バックバッファを取得 */
	D3DDEV->GetRenderTarget(0, &p_before_render_target);
	D3DDEV->GetDepthStencilSurface(&p_before_stencil_surface);

	D3DDEV->SetRenderTarget(0, p_texture_render_target_);
	D3DDEV->SetDepthStencilSurface(p_texture_stencil_surface_);

	D3DDEV->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(255, 255, 255), 1.0F, 0);
	D3DDEV->SetTexture(0, NULL);
}

/*********************************************
 * 関数名 : ReturnDraw関数                   *
 * 内  容 : 描画先をバックバッファに戻します *
 * 引  数 : なし                             *
 * 戻り値 : なし                             *
 *********************************************/
VOID PolygonPlate::ReturnDraw(VOID)
{
	D3DDEV->SetRenderTarget(0, p_before_render_target);
	D3DDEV->SetDepthStencilSurface(p_before_stencil_surface);
}

/*****************************************************
 * 関数名 : Draw関数                                 *
 * 内  容 : テクスチャ設定した板ポリゴンを描画します *
 * 引  数 : なし                                     *
 * 戻り値 : なし                                     *
 *****************************************************/
VOID PolygonPlate::Draw(VOID)
{
	/* レンダリングターゲット初期化 */
	D3DDEV->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0F, 0);

	/* ワールド行列を作成 */
	D3DXMATRIX world, work;
	D3DXMatrixIdentity(&world);

	/* ポリゴンのワールド行列を作成して設定 */
	D3DXMatrixIdentity(&world);
	D3DXMatrixIdentity(&work);
	D3DXMatrixTranslation(&work, -1.0F, -0.5F, 1.0F);
	D3DXMatrixMultiply(&world, &world, &work);

	D3DDEV->SetTransform(D3DTS_WORLD, &world);

	/* ポリゴン描画 */
	D3DDEV->SetStreamSource(0, vertex_buffer_, 0, sizeof(COSTOMVERTEX));  // 頂点バッファの設定(デバイスにセット)
	D3DDEV->SetVertexDeclaration(declaration_);				              // 頂点バッファのフォーマットの設定
	D3DDEV->SetIndices(index_buffer_);
	D3DDEV->SetTexture(0, p_texture_);
	D3DDEV->DrawIndexedPrimitive(D3DPT_TRIANGLEFAN, 0, 0, 3, 0, 2);
}

/*****************************************
 * 関数名 : Destroy関数                  *
 * 内  容 : 設定したバッファを破棄します *
 * 引  数 : なし                         *
 * 戻り値 : なし                         *
 *****************************************/
VOID PolygonPlate::Destroy(VOID)
{
	declaration_->Release();
	index_buffer_->Release();
	vertex_buffer_->Release();

	p_texture_->Release();
	p_texture_render_target_->Release();
	p_texture_stencil_surface_->Release();

	p_before_render_target->Release();
	p_before_stencil_surface->Release();
}