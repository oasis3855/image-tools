// CopyTimeExif.cpp : アプリケーション用クラスの定義を行います。
//

#include "stdafx.h"
#include "CopyTimeExif.h"
#include "CopyTimeExifDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCopyTimeExifApp

BEGIN_MESSAGE_MAP(CCopyTimeExifApp, CWinApp)
	//{{AFX_MSG_MAP(CCopyTimeExifApp)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
		//        この位置に生成されるコードを編集しないでください。
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCopyTimeExifApp クラスの構築

CCopyTimeExifApp::CCopyTimeExifApp()
{
	// TODO: この位置に構築用のコードを追加してください。
	// ここに InitInstance 中の重要な初期化処理をすべて記述してください。
}

/////////////////////////////////////////////////////////////////////////////
// 唯一の CCopyTimeExifApp オブジェクト

CCopyTimeExifApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CCopyTimeExifApp クラスの初期化

BOOL CCopyTimeExifApp::InitInstance()
{
	// 標準的な初期化処理
	// もしこれらの機能を使用せず、実行ファイルのサイズを小さくしたけ
	//  れば以下の特定の初期化ルーチンの中から不必要なものを削除して
	//  ください。

	InitCommonControls();

	CWinApp::InitInstance();


#ifdef _AFXDLL
//	Enable3dControls();			// 共有 DLL 内で MFC を使う場合はここをコールしてください。
#else
//	Enable3dControlsStatic();	// MFC と静的にリンクする場合はここをコールしてください。
#endif

	CCopyTimeExifDlg dlg;
	m_pMainWnd = &dlg;
	dlg.DoModal();

	// ダイアログが閉じられてからアプリケーションのメッセージ ポンプを開始するよりは、
	// アプリケーションを終了するために FALSE を返してください。
	return FALSE;
}
