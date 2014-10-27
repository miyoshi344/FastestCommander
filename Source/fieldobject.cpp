/***********
 * Include *
 ***********/
#include "fieldobject.h"

/*******************************************
 * 関数名 : Init関数                       *
 * 内  容 : メッシュの初期化を行います     *
 * 引  数 : なし                           *
 * 戻り値 : 取得に成功したらTRUEを返します *
 *******************************************/
BOOL FieldObject::Init(VOID)
{
	BOOL ret = TRUE;

	do
	{
		/* ブロックの初期化 */
		if (!ObjectInit())
		{
			/* エラー */
			ErrorDialog("ブロックの初期化に失敗しました");
			ret = FALSE;
			break;
		}

	} while (0);

	return ret;
}

/*******************************
 * 関数名 : UpDadate関数       *
 * 内  容 : 更新処理を行います *
 * 引  数 : なし               *
 * 戻り値 : なし               *
 *******************************/
VOID FieldObject::UpDate(VOID)
{
}

/*********************************************
 * 関数名 : Draw関数                         *
 * 内  容 : 描画処理を行います               *
 * 引  数 : D3DXMATRIX matrix ワールド座標系 *
 * 戻り値 : なし                             *
 *********************************************/
VOID FieldObject::Draw(VOID)
{
	/* メッシュの描画 */
	Mesh::Draw(world_, my_mesh_);
	//collision_.Sphere(my_mesh_, position_);
}

/*******************************
 * 関数名 : Destroy関数        *
 * 内  容 : 破棄処理を行います *
 * 引  数 : なし               *
 * 戻り値 : なし               *
 *******************************/
VOID FieldObject::Destroy(VOID)
{
	/* メッシュの破棄 */
	Mesh:: Release(my_mesh_);
}

/*******************************************
 * 関数名 : ObjectInit関数                 *
 * 内  容 : ブロックのメッシュを取得します *
 * 引  数 : なし                           *
 * 戻り値 : なし                           *
 *******************************************/
BOOL FieldObject::ObjectInit(VOID)
{
	BOOL ret = TRUE;

	/* フォルダ階層を移動する */
	_chdir("Resource_fast/Mesh/Block");

	do
	{
		/* メッシュの読み込み */
		if ((my_mesh_ = Mesh::Load(mesh_name_)) == NULL)
		{
			/* エラー */
			ErrorDialog("ブロックのメッシュ情報の取得に失敗");
			ret = FALSE;
			break;
		}

	} while (0);

	/* フォルダ階層を戻す */
	_chdir("../../../");

	return ret;
}