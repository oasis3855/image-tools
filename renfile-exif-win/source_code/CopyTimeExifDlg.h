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


// EXIF データ 格納用構造体
struct exif_data{
	UINT App1Size;
	unsigned short IfdDirCount;
	unsigned short SubIfdDirCount;
	// IFD
	char ImageDescription[256];		// 0x010e
	char Make[256];					// 0x010f
	char Model[256];				// 0x0110
	unsigned short Orientation;		// 0x0112
	char Software[256];				// 0x0131
	char DateTime[21];				// 0x0132
	char Copyright[256];			// 0x8298
	//Sub IFD
	double ExposureTime;			// 0x829a
	double FNumber;					// 0x829d
	unsigned short ISOSpeedRatings;	// 0x8827
	unsigned short ExposureProgram;	// 0x8822
	char ExifVersion[5];			// 0x9000
	char DateTimeOriginal[21];		// 0x9003
	char DateTimeDigitized[21];		// 0x9004
	unsigned long ExifImageWidth;	// 0xa002
	unsigned long ExifImageHeight;	// 0xa003
};

// ファイル名 データ 格納用構造体（ソート用）
struct _fstr{
	char str[_MAX_PATH];
};


void init_exif_data_struct(struct exif_data *dat);
BOOL read_exif_data(CString strPathOnly, CString strFilename, struct exif_data *dat);
BOOL CheckBinaryData(char *data1, char *data2, long size);
void SortArray(struct _fstr *fstrPathArray, int nFileCount, int nMode);

#endif // !defined(AFX_COPYTIMEEXIFDLG_H__6E68A8F2_8C4E_4FD4_98FA_1CF470D41185__INCLUDED_)
