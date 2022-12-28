// j6icnv.h : J6ICNV アプリケーションのメイン ヘッダー ファイルです。
//

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// メイン シンボル


/////////////////////////////////////////////////////////////////////////////
// CJ6icnvApp:
// このクラスの動作の定義に関しては j6icnv.cpp ファイルを参照してください。
//

class CJ6icnvApp : public CWinApp
{
public:
	UINT m_year_data;
	BOOL m_year;
	BOOL m_delete;
	BOOL m_nodialog;
	void UnInstallSeq(void);
	BOOL InstallSeq(void);
	CString m_shell_j6i_fname;
	BOOL m_directview;
	CString m_outpath_fname;
	CString m_extvwer_fname;
	int m_outpath;
	BOOL m_extvwer;
	BOOL m_confirm_ovwr;
	void RegWriteAll(void);
	void RegReadAll(void);
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

