// j6icnv.h : J6ICNV アプリケーションのメイン ヘッダー ファイルです。
//

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// メイン シンボル

extern int is_confirm_overwrite, is_use_ext_viewer, is_param_vw, is_outpath;
extern CString ext_viewer;

/////////////////////////////////////////////////////////////////////////////
// CJ6icnvApp:
// このクラスの動作の定義に関しては j6icnv.cpp ファイルを参照してください。
//

class CJ6icnvApp : public CWinApp
{
public:
	CJ6icnvApp();

// オーバーライド
	// ClassWizard は仮想関数を生成しオーバーライドします。
	//{{AFX_VIRTUAL(CJ6icnvApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// インプリメンテーション

	//{{AFX_MSG(CJ6icnvApp)
		// メモ - ClassWizard はこの位置にメンバ関数を追加または削除します。
		//        この位置に生成されるコードを編集しないでください。
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

