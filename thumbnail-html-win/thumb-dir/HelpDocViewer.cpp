// HelpDocViewer.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "thumbhtmldir.h"
#include "HelpDocViewer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHelpDocViewer ダイアログ


CHelpDocViewer::CHelpDocViewer(CWnd* pParent /*=NULL*/)
	: CDialog(CHelpDocViewer::IDD, pParent)
{
	//{{AFX_DATA_INIT(CHelpDocViewer)
	m_edit = _T("");
	//}}AFX_DATA_INIT
}


void CHelpDocViewer::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHelpDocViewer)
	DDX_Control(pDX, IDC_EDIT, m_edit_ctrl);
	DDX_Text(pDX, IDC_EDIT, m_edit);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CHelpDocViewer, CDialog)
	//{{AFX_MSG_MAP(CHelpDocViewer)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHelpDocViewer メッセージ ハンドラ

BOOL CHelpDocViewer::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: この位置に初期化の補足処理を追加してください
	LOGFONT fntTmpLogfnt;
	CString strTmp;

	// 日本語環境のときのみ、フォントを等幅フォントに切り替える
	strTmp.LoadString(STR_SW_LANG);
	if(strTmp == "JPN")
	{

		CFont *fntTmp = m_edit_ctrl.GetFont();
		if(m_edit_ctrl.GetFont()->GetLogFont(&fntTmpLogfnt))
		{
	//		fntTmpLogfnt.lfHeight = 0;
			strcpy(fntTmpLogfnt.lfFaceName, "ＭＳ ゴシック");
			fntEditbox.DeleteObject();
			if(fntEditbox.CreateFontIndirect(&fntTmpLogfnt))
			{
				m_edit_ctrl.SetFont(&fntEditbox);
			}
		}
	}
	
	
	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}
