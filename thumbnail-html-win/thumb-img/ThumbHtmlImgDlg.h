// ThumbHtmlImgDlg.h : ヘッダー ファイル
//

#if !defined(AFX_SUMBHTMLDIRDLG_H__6596E9C6_C3AB_11D4_AD9A_0090CCA1066A__INCLUDED_)
#define AFX_SUMBHTMLDIRDLG_H__6596E9C6_C3AB_11D4_AD9A_0090CCA1066A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CThumbHtmlImgDlg ダイアログ

typedef struct
{
	char str[_MAX_PATH];
	time_t tm_write;
} _fstr;

class CThumbHtmlImgDlg : public CDialog
{
// 構築
public:
	void BrowseSubPath(int nID);
	void SortArray(_fstr *Pass_Array, int pass_count, int nMode);
	CThumbHtmlImgDlg(CWnd* pParent = NULL);	// 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CThumbHtmlImgDlg)
	enum { IDD = IDD_THUMB_HTML_IMG_DLG };
	CComboBox	m_cmb_sort_ctrl;
	CSpinButtonCtrl	m_spin_col_ctrl;
	CComboBox	m_cmb_picsext_ctrl;
	CComboBox	m_cmb_photoext_ctrl;
	CComboBox	m_cmb_imgopt_ctrl;
	CComboBox	m_cmb_style_ctrl;
	//}}AFX_DATA

	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CThumbHtmlImgDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV のサポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	HICON m_hIcon;

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CThumbHtmlImgDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBtnBrowse();
	afx_msg void OnBtnExec();
	afx_msg void OnBtnAbout();
	afx_msg void OnBtnPhotoBrowse();
	afx_msg void OnBtnPicsBrowse();
	afx_msg void OnBtnHelp();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

// **********************************
// フォルダ選択 SDK ダイアログ ::SHBrowseForFolder 用
// **********************************
int CALLBACK myBrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData);
LPITEMIDLIST ConvertPIDL(char *lpszPath);


#endif // !defined(AFX_SUMBHTMLDIRDLG_H__6596E9C6_C3AB_11D4_AD9A_0090CCA1066A__INCLUDED_)
