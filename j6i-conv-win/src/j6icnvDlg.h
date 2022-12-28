// j6icnvDlg.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CJ6icnvDlg dialog

class CJ6icnvDlg : public CDialog
{
// 構築
public:
	CString m_last_j6i_file;
	CJ6icnvDlg(CWnd* pParent = NULL);	// 標準のコンストラクタ
	void DspMes(CString str, CString str2="");	// メッセージエリアへの表示


// Dialog Data
	//{{AFX_DATA(CJ6icnvDlg)
	enum { IDD = IDD_J6ICNV_DIALOG };
	CEdit	m_txt_mes_ctrl;
	CButton	m_chk_year_ctrl;
	CEdit	m_txt_year_ctrl;
	BOOL	m_chk_year;
	UINT	m_txt_year;
	//}}AFX_DATA

	// ClassWizard は仮想関数を生成しオーバーライドします。
	//{{AFX_VIRTUAL(CJ6icnvDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV のサポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	HICON m_hIcon;
	CJ6icnvApp *theApp;

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CJ6icnvDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBtnSel();
	afx_msg void OnBtnCnv();
	afx_msg void OnBtnProf();
	afx_msg void OnBtnShw();
	afx_msg void OnChkYear();
	afx_msg void OnChangeTxtYear();
	afx_msg void OnRadioSelmode1();
	afx_msg void OnRadioSelmode2();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	char m_tmpMsg[256];
};


/////////////////////////////////////////////////////////////////////////////
// アプリケーションのバージョン情報で使われている CAboutDlg ダイアログ
class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// ダイアログ データ
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard は仮想関数を生成しオーバーライドします
	//{{AFX_VIRTUAL(CAboutDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV のサポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

