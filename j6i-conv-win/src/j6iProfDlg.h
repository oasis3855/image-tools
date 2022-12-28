// j6iProfDlg.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CProfDlg ダイアログ

class CProfDlg : public CDialog
{
// コンストラクション
public:
	CProfDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CProfDlg)
	enum { IDD = IDD_DLG_PROF };
	BOOL	m_chk_ext;
	BOOL	m_chk_ovwr;
	CString	m_txt_extviewer;
	BOOL	m_paramvw;
	BOOL	m_outpath;
	CString	m_txt_outpath;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数を生成しオーバーライドします。
	//{{AFX_VIRTUAL(CProfDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CProfDlg)
	afx_msg void OnBtnExt();
	afx_msg void OnBtnOutput();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
