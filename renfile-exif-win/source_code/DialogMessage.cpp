// DialogMessage.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "CopyTimeExif.h"
#include "DialogMessage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogMessage ダイアログ


CDialogMessage::CDialogMessage(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogMessage::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogMessage)
	m_edit_main_text = _T("");
	//}}AFX_DATA_INIT
}


void CDialogMessage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogMessage)
	DDX_Control(pDX, IDC_EDIT_MAIN, m_edit_main_ctrl);
	DDX_Text(pDX, IDC_EDIT_MAIN, m_edit_main_text);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogMessage, CDialog)
	//{{AFX_MSG_MAP(CDialogMessage)
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogMessage メッセージ ハンドラ

void CDialogMessage::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: この位置にメッセージ ハンドラ用のコードを追加してください

	// テキストが選択されているのを解除する
	m_edit_main_ctrl.SetSel(-1,0,FALSE);
	// カーソルを先頭行に持っていく
	m_edit_main_ctrl.LineScroll(0);
	
}
