// j6icnv.cpp : アプリケーション用クラスの定義を行います。
//
// アプリケーション本体
// ここから、メインダイアログを表示したり、インストール処理をしたりする
// すべてのレジストリアクセスも、ここで行う。

#include "stdafx.h"
#include "j6icnv.h"
#include "j6icnvDlg.h"
#include "InstDlg.h"
#include "ShellLink.h"
#include "GlobalFunc.h"
#include <process.h>	// spawn


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



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

	CString tmpStr, tmpStr2;

	// システムレジストリの使用位置を設定する
	// \HKEY_USERS\.Default\Software\hi_soft\AFS_IDS_APP_TITLE\Settings
	SetRegistryKey((LPCTSTR)"hi soft");
	LoadStdProfileSettings(0);	// MRUリスト管理(0:しない)

	m_shell_j6i_fname = "";		// 引数のファイル名は、NULLとする

	// コマンドラインの解析
	if(!strcmpi(m_lpCmdLine, "/delete"))
	{	// アンインストール
		UnInstallSeq();
		return FALSE;
	}
	else if(!strcmpi(m_lpCmdLine, "/inst"))
	{	// インストール
		if(!InstallSeq()) return FALSE;
		// インストールしたあとは、プログラムダイアログを表示する
	}
	else if(m_lpCmdLine[0] != NULL)
	{	// その他のパラメーターはファイル名として扱う
		m_shell_j6i_fname = m_lpCmdLine;
	}
	else
	{	// 何もパラメーターがないときで、まだインストールされていない時
		if(!GetProfileInt("Global","inst",0))
		{	// インストールダイアログの表示とインストール処理
			if(!InstallSeq())return FALSE;
		}
	}

	RegReadAll();	// レジストリから設定値を読み込む

	if(m_shell_j6i_fname != "" && m_directview && m_nodialog)
	{
		CString DstPath, DstFname;
		char Msg[512];

		if(m_outpath)
		{	// 出力パス固定の時
			DstPath = m_outpath_fname;
		}
		else DstPath = "";

		if(m_year)
			::ConvertJ6I((LPCSTR)m_shell_j6i_fname, (LPCSTR)DstPath, Msg, NULL, m_year_data, m_confirm_ovwr);
		else
			::ConvertJ6I((LPCSTR)m_shell_j6i_fname, (LPCSTR)DstPath, Msg, NULL, 0, m_confirm_ovwr);
		// 画像を表示する外部プログラムを実行
		DstFname = ::GetJ6iName(m_shell_j6i_fname, DstPath);
		if(m_extvwer)
		{
			if(m_delete)
			{	// 出力ファイルを消去するモードのときは、並行動作しない
				if(_spawnl(_P_WAIT, m_extvwer_fname, m_extvwer_fname, DstFname, NULL)<0)
					::MessageBox(NULL,"外部 Viewer が実行できない","J6I イメージ コンバータ", MB_OK);
			}
			else
			{
				if(_spawnl(_P_NOWAIT, m_extvwer_fname, m_extvwer_fname, DstFname, NULL)<0)
					::MessageBox(NULL,"外部 Viewer が実行できない","J6I イメージ コンバータ", MB_OK);
			}
		}
		else
		{
			if(m_delete)
					::MessageBox(NULL,"シェル実行でファイル消去はできない","J6I イメージ コンバータ", MB_OK);

			::ShellExecute(NULL, NULL, DstFname, NULL, NULL, SW_SHOW);
		}
		// プレビュー後JPGファイルを消去する
		if(m_delete) ::remove(DstFname);

	}
	else
	{
		// メインダイアログの生成と表示
		CJ6icnvDlg dlg;
		m_pMainWnd = &dlg;
		dlg.m_chk_year = m_year;
		dlg.m_txt_year = m_year_data;

		dlg.DoModal();

		m_year = dlg.m_chk_year;
		m_year_data = dlg.m_txt_year;
	}

	RegWriteAll();	// 設定値をレジストリに保存する

	// ダイアログが閉じられてからアプリケーションのメッセージ ポンプを開始するよりは、
	// アプリケーションを終了するために FALSE を返してください。
	return FALSE;
}

// レジストリから設定値を読み込む
void CJ6icnvApp::RegReadAll()
{
	m_confirm_ovwr = GetProfileInt("Settings","cnf_ovwr",1);
	m_extvwer = GetProfileInt("Settings","use_extvw",0);
	m_directview = GetProfileInt("Settings","param_vw",1);
	m_outpath = GetProfileInt("Settings","sw_outpath",0);
	m_delete =  GetProfileInt("Settings","sw_delete",0);
	m_nodialog =  GetProfileInt("Settings","sw_nodlg",0);
	m_year = GetProfileInt("Settings","sw_year",0);
	m_extvwer_fname = GetProfileString("Settings","extvw","");
	m_outpath_fname = GetProfileString("Settings","outpath","");
	m_year_data = GetProfileInt("Settings","sw_year_data",2000);
}

// レジストリに設定値を書き込む
void CJ6icnvApp::RegWriteAll()
{
	if(m_confirm_ovwr != (int)GetProfileInt("Settings","cnf_ovwr",1))
		WriteProfileInt("Settings","cnf_ovwr",m_confirm_ovwr);
	if(m_extvwer != (int)GetProfileInt("Settings","use_extvw",0))
		WriteProfileInt("Settings","use_extvw",m_extvwer);
	if(m_directview != (int)GetProfileInt("Settings","param_vw",1))
		WriteProfileInt("Settings","param_vw",m_directview);
	if(m_outpath != (int)GetProfileInt("Settings","sw_outpath",0))
		WriteProfileInt("Settings","sw_outpath",m_outpath);
	if(m_delete !=  (int)GetProfileInt("Settings","sw_delete",0))
		WriteProfileInt("Settings","sw_delete",m_delete);
	if(m_nodialog !=  (int)GetProfileInt("Settings","sw_nodlg",0))
		WriteProfileInt("Settings","sw_nodlg",m_nodialog);
	if(m_year != (int)GetProfileInt("Settings","sw_year",0))
		WriteProfileInt("Settings","sw_year",m_year);
	if(m_extvwer_fname != GetProfileString("Settings","extvw",""))
		WriteProfileString("Settings","extvw",m_extvwer_fname);
	if(m_outpath_fname != GetProfileString("Settings","outpath",""))
		WriteProfileString("Settings","outpath",m_outpath_fname);
	if(m_year != (int)GetProfileInt("Settings","sw_year_data",2000))
		WriteProfileInt("Settings","sw_year_data",m_year_data);
}

// インストール
BOOL CJ6icnvApp::InstallSeq(void)
{
	CInstDlg dlg;
	
	dlg.m_chk_associate = TRUE;
	dlg.m_chk_start = TRUE;
	dlg.m_radio_uninst = 0;

	if(dlg.DoModal() != IDOK) return FALSE;		// ｷｬﾝｾﾙボタンが押されたら 何もしない

	WriteProfileInt("Global","inst",1);
	char szProg[MAX_PATH];
	char szLink[MAX_PATH];
	CString tmpStr;

	// アンインストール情報について
	if(dlg.m_radio_uninst == 0)
	{	// アンインストールパスを「アプリケーションの追加と削除」に書き込み
		if(!::GetModuleFileName(NULL, szProg, MAX_PATH)) return FALSE;
		strcat(szProg, " /delete");
		HKEY hCU;
		DWORD dw;
		// 全ユーザー共通設定領域に、アンインストール情報を書き込む
		if (RegCreateKeyEx(HKEY_LOCAL_MACHINE, "Software\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\J6ICNV",
			0, REG_NONE, REG_OPTION_NON_VOLATILE, KEY_WRITE|KEY_READ, NULL,
			&hCU, &dw) == ERROR_SUCCESS)
		{
			RegSetValueEx( hCU, "UninstallString", 0, REG_SZ, (const unsigned char *)szProg, strlen(szProg));
			strcpy(szProg, "J6I イメージ コンバータ");
			RegSetValueEx( hCU, "DisplayName", 0, REG_SZ, (const unsigned char *)szProg, strlen(szProg));
			RegCloseKey(hCU);
		}
	}
	else
	{	//スタートメニューにアンインストールショートカットを作成する
		::GetModuleFileName(NULL, szProg, MAX_PATH);
		HKEY hCU;
		DWORD lpType;
		ULONG ulSize = MAX_PATH;
		if (RegOpenKeyEx(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders",
			0, KEY_QUERY_VALUE, &hCU) == ERROR_SUCCESS)
		{
			RegQueryValueEx( hCU, "Programs", NULL, &lpType, (unsigned char *)&szLink, &ulSize);
			RegCloseKey(hCU);
		}
		strcat(szLink, "\\J6Iｲﾒｰｼﾞｺﾝﾊﾞｰﾀの削除.LNK");
		::CreateShellLink(szProg, szLink, "J6ICNV", "/delete");
		WriteProfileInt("Global","inst_local",1);
	}

	if(dlg.m_chk_start)
	{	// スタートメニューに登録する
		::GetModuleFileName(NULL, szProg, MAX_PATH);
		HKEY hCU;
		DWORD lpType;
		ULONG ulSize = MAX_PATH;
		if (RegOpenKeyEx(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders",
			0, KEY_QUERY_VALUE, &hCU) == ERROR_SUCCESS)
		{
			RegQueryValueEx( hCU, "Programs", NULL, &lpType, (unsigned char *)&szLink, &ulSize);
			RegCloseKey(hCU);
		}
		strcat(szLink, "\\J6Iｲﾒｰｼﾞｺﾝﾊﾞｰﾀ.LNK");
		::CreateShellLink(szProg, szLink, "J6ICNV", "");
	}

	if(dlg.m_chk_associate)
	{	// 関連付けを行う
		HKEY hCU;
		DWORD dw;
		// HKEY_CLASSES_ROOT\\.j6i に 「j6ifile」 を書き込む
		if (RegCreateKeyEx(HKEY_CLASSES_ROOT, ".j6i",
			0, REG_NONE, REG_OPTION_NON_VOLATILE, KEY_WRITE|KEY_READ, NULL,
			&hCU, &dw) == ERROR_SUCCESS)
		{
			strcpy(szProg, "j6ifile");
			RegSetValueEx( hCU, "", 0, REG_SZ, (unsigned char *)&szProg, strlen(szProg));
			RegCloseKey(hCU);
		}
		else return FALSE;
		// HKEY_CLASSES_ROOT\\j6ifile に 関連付けデータ を書き込む
		if(!::GetModuleFileName(NULL, szProg, MAX_PATH)) return FALSE;
		strcat(szProg, " %1");
		if (RegCreateKeyEx(HKEY_CLASSES_ROOT, "j6ifile\\shell\\open\\command",
			0, REG_NONE, REG_OPTION_NON_VOLATILE, KEY_WRITE|KEY_READ, NULL,
			&hCU, &dw) == ERROR_SUCCESS)
		{
			RegSetValueEx(hCU, "", 0, REG_SZ, (unsigned char *)&szProg, strlen(szProg));
			RegCloseKey(hCU);
		}
		else return FALSE;

	}

	return TRUE;
}

// アンインストール
void CJ6icnvApp::UnInstallSeq(void)
{
	CString tmpStr, tmpStr2;	// ストリング リソース 読み込み用
	
	tmpStr.LoadString(IDS_APPNAME);
	tmpStr2.LoadString(IDS_Q_UNINST);
	if(::MessageBox(NULL, tmpStr2, tmpStr, MB_ICONQUESTION|MB_YESNO|MB_SYSTEMMODAL) != IDYES)
		return;

	HKEY hCU;
	char szLink[MAX_PATH];
	DWORD lpType;
	ULONG ulSize = MAX_PATH;

	if(GetProfileInt("Global","inst_local",0))
	{	// スタートメニューに 削除アイコンがある場合
		if (RegOpenKeyEx(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders",
				0, KEY_QUERY_VALUE, &hCU) == ERROR_SUCCESS)
		{
			RegQueryValueEx( hCU, "Programs", NULL, &lpType, (unsigned char *)&szLink, &ulSize);
			RegCloseKey(hCU);
		}
		strcat(szLink, "\\J6Iｲﾒｰｼﾞｺﾝﾊﾞｰﾀの削除.LNK");
		::remove(szLink);
	}
	else
	{	// レジストリ上に削除ショートカットがある場合
		// コントロールパネル用削除データの削除
		if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, "Software\\Microsoft\\Windows\\CurrentVersion\\Uninstall",
				0, KEY_QUERY_VALUE, &hCU) == ERROR_SUCCESS)
		{
			RegDeleteKey( hCU, "J6ICNV");
			RegCloseKey(hCU);
		}
	}
	// レジストリの設定情報を消去 現在のユーザー情報から削除する
	if (RegOpenKeyEx(HKEY_CURRENT_USER, "Software\\hi soft",
			0, KEY_QUERY_VALUE, &hCU) == ERROR_SUCCESS)
	{
		RegDeleteKey( hCU, "j6i_conv");
		RegCloseKey(hCU);
	}
	// スタートメニューの設定アイコンを抹殺
	if (RegOpenKeyEx(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders",
			0, KEY_QUERY_VALUE, &hCU) == ERROR_SUCCESS)
	{
		RegQueryValueEx( hCU, "Programs", NULL, &lpType, (unsigned char *)&szLink, &ulSize);
		RegCloseKey(hCU);
	}
	strcat(szLink, "\\J6Iｲﾒｰｼﾞｺﾝﾊﾞｰﾀ.LNK");
	::remove(szLink);
	// ファイルの関連付けを削除
	RegDeleteKey(HKEY_CLASSES_ROOT, ".j6i");
	RegDeleteKey(HKEY_CLASSES_ROOT, "j6ifile");

	// 情報の表示
	tmpStr.LoadString(IDS_APPNAME);
	tmpStr2.LoadString(IDS_END_UNINST);
	::MessageBox(NULL, tmpStr2, tmpStr, MB_ICONINFORMATION|MB_OK|MB_SYSTEMMODAL);

}


