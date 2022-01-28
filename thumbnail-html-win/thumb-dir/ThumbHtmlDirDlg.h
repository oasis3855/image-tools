// SumbHtmlDirDlg.h : ヘッダー ファイル
//

#if !defined(AFX_SUMBHTMLDIRDLG_H__6596E9C6_C3AB_11D4_AD9A_0090CCA1066A__INCLUDED_)
#define AFX_SUMBHTMLDIRDLG_H__6596E9C6_C3AB_11D4_AD9A_0090CCA1066A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CSumbHtmlDirDlg ダイアログ

typedef struct
{
	char str[_MAX_PATH];
} _fstr;

class CSumbHtmlDirDlg : public CDialog
{
// 構築
public:
	LPSTR GetSumbFname(LPCSTR Fpath);
	BOOL CopyFile(LPCSTR FpathIn, LPCSTR FpathOut);
	void SortArray(_fstr *Pass_Array, int pass_count);
	CSumbHtmlDirDlg(CWnd* pParent = NULL);	// 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CSumbHtmlDirDlg)
	enum { IDD = IDD_SUMBHTMLDIR_DIALOG };
	CButton	m_btn_browse_addpath_ctrl;
	CEdit	m_edit_addpath_ctrl;
	//}}AFX_DATA

	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CSumbHtmlDirDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV のサポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	HICON m_hIcon;

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CSumbHtmlDirDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBtnBrowse();
	afx_msg void OnBtnExec();
	afx_msg void OnCheckAddpath();
	afx_msg void OnBtnBrowseAddpath();
	afx_msg void OnBtnAbout();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_SUMBHTMLDIRDLG_H__6596E9C6_C3AB_11D4_AD9A_0090CCA1066A__INCLUDED_)
