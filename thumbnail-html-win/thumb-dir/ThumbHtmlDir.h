// ThumbHtmlDir.h : SUMBHTMLDIR アプリケーションのメイン ヘッダー ファイルです。
//

#if !defined(AFX_SUMBHTMLDIR_H__6596E9C4_C3AB_11D4_AD9A_0090CCA1066A__INCLUDED_)
#define AFX_SUMBHTMLDIR_H__6596E9C4_C3AB_11D4_AD9A_0090CCA1066A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// メイン シンボル

/////////////////////////////////////////////////////////////////////////////
// CThumbHtmlDirApp:
// このクラスの動作の定義に関しては ThumbHtmlDir.cpp ファイルを参照してください。
//

class CThumbHtmlDirApp : public CWinApp
{
public:
	CThumbHtmlDirApp();

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CThumbHtmlDirApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// インプリメンテーション

	//{{AFX_MSG(CThumbHtmlDirApp)
		// メモ - ClassWizard はこの位置にメンバ関数を追加または削除します。
		//        この位置に生成されるコードを編集しないでください。
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_SUMBHTMLDIR_H__6596E9C4_C3AB_11D4_AD9A_0090CCA1066A__INCLUDED_)
