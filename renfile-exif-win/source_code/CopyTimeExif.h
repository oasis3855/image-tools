// CopyTimeExif.h : COPYTIMEEXIF アプリケーションのメイン ヘッダー ファイルです。
//

#if !defined(AFX_COPYTIMEEXIF_H__F86CF922_4741_47F6_95E9_4298C9469434__INCLUDED_)
#define AFX_COPYTIMEEXIF_H__F86CF922_4741_47F6_95E9_4298C9469434__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// メイン シンボル

/////////////////////////////////////////////////////////////////////////////
// CCopyTimeExifApp:
// このクラスの動作の定義に関しては CopyTimeExif.cpp ファイルを参照してください。
//

class CCopyTimeExifApp : public CWinApp
{
public:
	CCopyTimeExifApp();

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CCopyTimeExifApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// インプリメンテーション

	//{{AFX_MSG(CCopyTimeExifApp)
		// メモ - ClassWizard はこの位置にメンバ関数を追加または削除します。
		//        この位置に生成されるコードを編集しないでください。
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_COPYTIMEEXIF_H__F86CF922_4741_47F6_95E9_4298C9469434__INCLUDED_)
