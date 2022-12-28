// j6iProfDlg.cpp : インプリメンテーション ファイル
//
// 環境設定ダイアログ

#include "stdafx.h"
#include "j6icnv.h"
#include "j6icnvDlg.h"
#include "j6iProfDlg.h"
#include <shlobj.h>		// フォルダ選択ダイアログ用

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CProfDlg ダイアログ


CProfDlg::CProfDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CProfDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CProfDlg)
	m_chk_ext = FALSE;
	m_chk_ovwr = FALSE;
	m_txt_extviewer = _T("");
	m_paramvw = FALSE;
	m_txt_outpath = _T("");
	m_chk_delete = FALSE;
	m_chk_nodlg = FALSE;
	m_outpath = -1;
	m_chk_year = FALSE;
	m_txt_year = 0;
	//}}AFX_DATA_INIT
}


void CProfDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProfDlg)
	DDX_Control(pDX, IDC_CHK_DELETE, m_chk_delete_ctrl);
	DDX_Control(pDX, IDC_TXT_YEAR, m_txt_year_ctrl);
	DDX_Control(pDX, IDC_BTN_EXT, m_btn_extview);
	DDX_Control(pDX, IDC_BTN_OUTPUT, m_btn_output);
	DDX_Check(pDX, IDC_CHK_EXT, m_chk_ext);
	DDX_Check(pDX, IDC_CHK_OVWR, m_chk_ovwr);
	DDX_Text(pDX, IDC_TXT_EXT, m_txt_extviewer);
	DDX_Check(pDX, IDC_CHK_PARAMVW, m_paramvw);
	DDX_Text(pDX, IDC_TXT_OUTPUT, m_txt_outpath);
	DDX_Check(pDX, IDC_CHK_DELETE, m_chk_delete);
	DDX_Check(pDX, IDC_CHK_NODLG, m_chk_nodlg);
	DDX_Radio(pDX, IDC_RADIO_OUTPATH, m_outpath);
	DDX_Check(pDX, IDC_CHK_YEAR, m_chk_year);
	DDX_Text(pDX, IDC_TXT_YEAR, m_txt_year);
	DDV_MinMaxUInt(pDX, m_txt_year, 1900, 2030);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CProfDlg, CDialog)
	//{{AFX_MSG_MAP(CProfDlg)
	ON_BN_CLICKED(IDC_BTN_EXT, OnBtnExt)
	ON_BN_CLICKED(IDC_BTN_OUTPUT, OnBtnOutput)
	ON_BN_CLICKED(IDC_ABOUT, OnAbout)
	ON_BN_CLICKED(IDC_RADIO_OUTPATH, OnRadioOutpath)
	ON_BN_CLICKED(IDC_RADIO_OUTPATH2, OnRadioOutpath2)
	ON_BN_CLICKED(IDC_RADIO_OUTPATH3, OnRadioOutpath3)
	ON_BN_CLICKED(IDC_CHK_EXT, OnChkExt)
	ON_BN_CLICKED(IDC_CHK_YEAR, OnChkYear)
	ON_WM_VSCROLL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CProfDlg メッセージ ハンドラ


// ダイアログ表示前の処理
//
// デフォルトボタンの状態を設定する
BOOL CProfDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: この位置に初期化の補足処理を追加してください

	// 出力パスの種類により、ボタンなどを変化させる
	char s_full[_MAX_PATH];
	switch(GetCheckedRadioButton(IDC_RADIO_OUTPATH,IDC_RADIO_OUTPATH3))
	{
		case IDC_RADIO_OUTPATH :
			// ソースファイルと同じフォルダ
			m_btn_output.EnableWindow(FALSE);
			SetDlgItemText(IDC_TXT_OUTPUT, "");	
			break;
		case IDC_RADIO_OUTPATH2 :
			m_btn_output.EnableWindow(FALSE);
			::GetTempPath(_MAX_PATH-1, s_full);
			SetDlgItemText(IDC_TXT_OUTPUT, s_full);	
			break;
		case IDC_RADIO_OUTPATH3 :
			break;
	}

	// 外部ビュワーの設定の有り無し
	if(!IsDlgButtonChecked(IDC_CHK_EXT))
	{
		m_btn_extview.EnableWindow(FALSE);
		SetDlgItemText(IDC_TXT_EXT, "");	
		m_chk_delete_ctrl.EnableWindow(FALSE);	// シェル実行時には、終了後消去ができない
		m_chk_delete_ctrl.SetCheck(0);
	}
	else
	{
		m_btn_extview.EnableWindow(TRUE);
		m_chk_delete_ctrl.EnableWindow(TRUE);
	}		
	// 年号変換の有り無し
	if(!IsDlgButtonChecked(IDC_CHK_YEAR))
	{
		m_txt_year_ctrl.EnableWindow(FALSE);
	}
	else
	{
		m_txt_year_ctrl.EnableWindow(TRUE);
	}	
	
	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

// 外部画像表示ソフトの選択ボタンを押したとき
void CProfDlg::OnBtnExt() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	CString extprog;
	GetDlgItemText(IDC_TXT_EXT, extprog);
	
	CFileDialog dlg(TRUE, "EXE", (LPCSTR)extprog, OFN_HIDEREADONLY|OFN_FILEMUSTEXIST|OFN_EXPLORER,
		"ﾌﾟﾛｸﾞﾗﾑ|*.exe;*.com;*.pif|すべてのﾌｧｲﾙ|*.*||");
	dlg.m_ofn.lpstrTitle = "JPG を表示するﾌﾟﾛｸﾞﾗﾑを選んでください";

	if(dlg.DoModal() != IDOK) return;

	SetDlgItemText(IDC_TXT_EXT, dlg.GetPathName());
	
}

// 出力パスの選択ボタンを押したとき
void CProfDlg::OnBtnOutput() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください

	char s_path[MAX_PATH];
	char sTitle[] = "変換元フォルダの選択";
	LPITEMIDLIST pidl;
	BROWSEINFO bi;
	ZeroMemory(&bi,sizeof(bi));
	bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_STATUSTEXT;
	bi.lpszTitle = sTitle;
	bi.hwndOwner = m_hWnd;

	pidl = ::SHBrowseForFolder(&bi);
	if(pidl == NULL) return;
	SHGetPathFromIDList(pidl, s_path);

	// フォルダ名は最後が \ で終わるようにする。なお、ルートフォルダは元から \ で終わっている
	if(s_path[strlen(s_path)-1] != '\\') strcat(s_path, "\\");

	SetDlgItemText(IDC_TXT_OUTPUT, s_path);	
}

// 著作権表示ダイアログを表示する
void CProfDlg::OnAbout() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	CAboutDlg dlgAbout;
	dlgAbout.DoModal();
	
}

// ソースファイルと同じフォルダ を押した場合
void CProfDlg::OnRadioOutpath() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	m_btn_output.EnableWindow(FALSE);
	SetDlgItemText(IDC_TXT_OUTPUT, "");
}

// テンポラリフォルダ を押した場合
void CProfDlg::OnRadioOutpath2() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	char s_full[_MAX_PATH];
	m_btn_output.EnableWindow(FALSE);
	::GetTempPath(_MAX_PATH-1, s_full);
	SetDlgItemText(IDC_TXT_OUTPUT, s_full);	
}

// 任意のフォルダを押した場合
void CProfDlg::OnRadioOutpath3() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	m_btn_output.EnableWindow(TRUE);	
}

// 外部ビュワーの設定の有り無し
void CProfDlg::OnChkExt() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	if(!IsDlgButtonChecked(IDC_CHK_EXT))
	{
		m_btn_extview.EnableWindow(FALSE);
		SetDlgItemText(IDC_TXT_EXT, "");
		m_chk_delete_ctrl.EnableWindow(FALSE);	// シェル実行時には、終了後消去ができない
		m_chk_delete_ctrl.SetCheck(0);
	}
	else
	{
		m_btn_extview.EnableWindow(TRUE);
		m_chk_delete_ctrl.EnableWindow(TRUE);
	}		
}

// 年号変換の有り無し
void CProfDlg::OnChkYear() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	if(!IsDlgButtonChecked(IDC_CHK_YEAR))
	{
		m_txt_year_ctrl.EnableWindow(FALSE);
	}
	else
	{
		m_txt_year_ctrl.EnableWindow(TRUE);
	}	
}


// 年変更のスクロールボタンが押された場合
void CProfDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください
	UINT year;

	year = GetDlgItemInt(IDC_TXT_YEAR);
	if(year < 1980 || year > 2030)
	{
		CTime t = CTime::GetCurrentTime();
		year = t.GetYear();
	}

	if(pScrollBar->GetDlgCtrlID() == IDC_SCROLL_YEAR)
	{
		switch(nSBCode)
		{
			case SB_LINEUP :
				if(year<2030)year++; break;
			case SB_LINEDOWN :
				if(year>1980) year--;
				break;
			default : break;
		}
		SetDlgItemInt(IDC_TXT_YEAR, year);
	}


	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}
