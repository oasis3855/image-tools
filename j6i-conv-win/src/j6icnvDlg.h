// j6icnvDlg.h : ヘッダー ファイル
//

typedef struct headertag
{

	unsigned char head;				/* 00 */
	unsigned char dummy1[7-1+1];
	unsigned char ver_h, ver_l;		/* 08,09 */
	char dummy2[0x0f-0x0a+1];
	char spec[0x2f-0x10+1];			/* 10 - 2F */
	unsigned char attr;				/* 30 */
	unsigned char gamma;			/* 31 */
	unsigned char endian;			/* 32 */
	unsigned char dummy3[0x37-0x33+1];
	unsigned long startaddr;		/* 38,39,3A,3B */
	unsigned long endaddr;			/* 3C,3D,3E,3F */
	unsigned char enabledate;		/* 40 */
	unsigned char dummy4;
	unsigned char tz;				/* 42 */
	char dummy5;
	unsigned char date[0x49-0x44+1];/* 44-49 */
	char dummy6[0xab-0x4a+1];
}header;

unsigned int BCDtoINT(unsigned char bcd);


/////////////////////////////////////////////////////////////////////////////
// CJ6icnvDlg dialog

class CJ6icnvDlg : public CDialog
{
// 構築
public:
	CJ6icnvDlg(CWnd* pParent = NULL);	// 標準のコンストラクタ
	void CnvMain(void);		// ｺﾝﾊﾞｰﾀ本体
	void DspMes(CString str, CString str2="");	// メッセージエリアへの表示

	CString SrcFname, DstFname;		// 変換元と変換先の完全なパス名
	CString FnameBody;				// 拡張子無しのファイル名
	CString OutPath;				// 出力パス

// Dialog Data
	//{{AFX_DATA(CJ6icnvDlg)
	enum { IDD = IDD_J6ICNV_DIALOG };
		// メモ: この位置に ClassWizard によってデータ メンバが追加されます。
	//}}AFX_DATA

	// ClassWizard は仮想関数を生成しオーバーライドします。
	//{{AFX_VIRTUAL(CJ6icnvDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV のサポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	HICON m_hIcon;

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
	afx_msg void OnBtnSeq();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
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

