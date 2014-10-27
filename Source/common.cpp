/**************
 * 共通クラス *
 **************/
#include "common.h"

/**********************************************
 * 関数名 : ErrorDialog関数                   *
 * 内  容 : メッセージボックスを表示          *
 * 引  数 : LPCTSTR Message：表示するテキスト *
 * 戻り値：なし                               *
 **********************************************/
VOID ErrorDialog( LPCTSTR Message )
{
    /* ダイアログタイプ */
    INT type = MB_OK | MB_ICONERROR | MB_ICONSTOP | MB_TASKMODAL;

#ifdef _DEBUG
    /* ダイアログを表示 */
    MessageBox(	NULL,     // ウィンドウハンドル
				Message,  // 表示するメッセージ
				"ERROR",  // タイトルに表示する文字列
				type );	  // ダイアログタイプ8
#endif
}