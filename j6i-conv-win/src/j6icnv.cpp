// j6icnv.cpp : アプリケーション用クラスの定義を行います。
//

#include "stdafx.h"
#include "j6icnv.h"
#include "j6icnvDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// グローバルデータ
int is_confirm_overwrite, is_use_ext_viewer, is_param_vw, is_outpath;
CString ext_viewer;


/////////////////////////////////////////////////////////////////////////////
// CJ6icnvApp

BEGIN_MESSAGE_MAP(CJ6icnvApp, CWinApp)
	//{{AFX_MSG_MAP(CJ6icnvApp)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
		//        この位置に生成されるコードを編集しないでください。
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CJ6icnvApp クラスの構築

CJ6icnvApp::CJ6icnvApp()
{
	// TODO: この位置に構築用のコードを追加してください。
	// ここに InitInstance 中の重要な初期化処理をすべて記述してください。
}

/////////////////////////////////////////////////////////////////////////////
// 唯一の CJ6icnvApp オブジェクト

CJ6icnvApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CJ6icnvApp クラスの初期化

BOOL CJ6icnvApp::InitInstance()
{

#ifdef _AFXDLL
	Enable3dControls();			// 共有 DLL 内で MFC を使う場合はここをコールしてください。
#else
	Enable3dControlsStatic();	// MFC と静的にリンクする場合はここをコールしてください。
#endif

	// システムレジストリの使用位置を設定する
	// \HKEY_USERS\.Default\Software\hi_soft\Settings
	m_pszAppName = "j6i_cnv";
	SetRegistryKey((LPCTSTR)"hi soft");
	// メインダイアログの生成と表示
	CJ6icnvDlg dlg;
	m_pMainWnd = &dlg;
	dlg.DoModal();

	// ダイアログが閉じられてからアプリケーションのメッセージ ポンプを開始するよりは、
	// アプリケーションを終了するために FALSE を返してください。
	return FALSE;
}
