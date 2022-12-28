// j6iProfDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "j6icnv.h"
#include "j6iProfDlg.h"

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
	m_outpath = FALSE;
	m_txt_outpath = _T("");
	//}}AFX_DATA_INIT
}


void CProfDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProfDlg)
	DDX_Check(pDX, IDC_CHK_EXT, m_chk_ext);
	DDX_Check(pDX, IDC_CHK_OVWR, m_chk_ovwr);
	DDX_Text(pDX, IDC_TXT_EXT, m_txt_extviewer);
	DDX_Check(pDX, IDC_CHK_PARAMVW, m_paramvw);
	DDX_Check(pDX, IDC_CHK_OUTPUT, m_outpath);
	DDX_Text(pDX, IDC_TXT_OUTPUT, m_txt_outpath);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CProfDlg, CDialog)
	//{{AFX_MSG_MAP(CProfDlg)
	ON_BN_CLICKED(IDC_BTN_EXT, OnBtnExt)
	ON_BN_CLICKED(IDC_BTN_OUTPUT, OnBtnOutput)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CProfDlg メッセージ ハンドラ

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

void CProfDlg::OnBtnOutput() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	CString output_path;
	GetDlgItemText(IDC_TXT_OUTPUT, output_path);
	output_path += "出力ﾌｧｲﾙ.jpg";
	
	CFileDialog dlg(TRUE, NULL, (LPCSTR)output_path, OFN_HIDEREADONLY|OFN_PATHMUSTEXIST|OFN_EXPLORER,
		"すべてのﾌｧｲﾙ|*.*||");
	dlg.m_ofn.lpstrTitle = "JPGﾌｧｲﾙを出力するするﾌｫﾙﾀﾞを選んでください";

	if(dlg.DoModal() != IDOK) return;

	output_path = dlg.GetPathName();

	char s_drv[_MAX_DRIVE], s_dir[_MAX_DIR], s_body[_MAX_FNAME], s_full[_MAX_DRIVE+_MAX_DIR+_MAX_FNAME+4];
	_splitpath((LPCSTR)output_path, s_drv, s_dir, s_body, NULL);
	sprintf(s_full, "%s%s", s_drv, s_dir);
	output_path = s_full;

	SetDlgItemText(IDC_TXT_OUTPUT, output_path);
	
	
}
