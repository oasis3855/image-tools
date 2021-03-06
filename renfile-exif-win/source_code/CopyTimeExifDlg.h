// CopyTimeExifDlg.h : ヘッダー ファイル
//

#if !defined(AFX_COPYTIMEEXIFDLG_H__6E68A8F2_8C4E_4FD4_98FA_1CF470D41185__INCLUDED_)
#define AFX_COPYTIMEEXIFDLG_H__6E68A8F2_8C4E_4FD4_98FA_1CF470D41185__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CCopyTimeExifDlg ダイアログ

class CCopyTimeExifDlg : public CDialog
{
// 構築
public:
	BOOL ChangeFname(char *strPathname, char *strFilename, char *strPathOnly);
	void CopyFuncDispatch(char *strPathname, char *strFilename, char *strPathOnly);
	void PreviewExif(char *strPathname, char *strFilename, char *strPathOnly);
	BOOL ChangeDate(char *strPathname, char *strFilename, char *strPathOnly);
	CCopyTimeExifDlg(CWnd* pParent = NULL);	// 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CCopyTimeExifDlg)
	enum { IDD = IDD_COPYTIMEEXIF_DIALOG };
	CComboBox	m_cmb_filename_append_ctrl;
	CComboBox	m_cmb_filename_ctrl;
	CComboBox	m_cmb_mode_ctrl;
	//}}AFX_DATA

	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CCopyTimeExifDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV のサポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	HICON m_hIcon;

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CCopyTimeExifDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBtnPickfile();
	afx_msg void OnBtnCopy();
	afx_msg void OnBtnPickfolder();
	afx_msg void OnBtnAbout();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。



#endif // !defined(AFX_COPYTIMEEXIFDLG_H__6E68A8F2_8C4E_4FD4_98FA_1CF470D41185__INCLUDED_)

