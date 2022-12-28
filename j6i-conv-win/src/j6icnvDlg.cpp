// j6icnvDlg.cpp : インプリメンテーション ファイル
// メインダイアログ および バージョン情報ダイアログ

#include "stdafx.h"
#include "j6icnv.h"
#include "j6icnvDlg.h"
#include "j6iProfDlg.h"
#include "GlobalFunc.h"
#include <stdio.h>
#include <process.h>	// spawn
#include <io.h>			// findfirst
#include <shlobj.h>		// フォルダ共通ダイアログ

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// ************************************************
// バージョン情報ダイアログ
// ************************************************

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// メッセージ ハンドラがありません。
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CJ6icnvDlg ダイアログ
// メインダイアログ

CJ6icnvDlg::CJ6icnvDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CJ6icnvDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CJ6icnvDlg)
	m_chk_year = FALSE;
	m_txt_year = 0;
	//}}AFX_DATA_INIT
	// メモ: LoadIcon は Win32 の DestroyIcon のサブシーケンスを要求しません。
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	theApp = (CJ6icnvApp *)AfxGetApp();
	m_last_j6i_file = "";
}

void CJ6icnvDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CJ6icnvDlg)
	DDX_Control(pDX, IDC_TXT_MES, m_txt_mes_ctrl);
	DDX_Control(pDX, IDC_CHK_YEAR, m_chk_year_ctrl);
	DDX_Control(pDX, IDC_TXT_YEAR, m_txt_year_ctrl);
	DDX_Check(pDX, IDC_CHK_YEAR, m_chk_year);
	DDX_Text(pDX, IDC_TXT_YEAR, m_txt_year);
	DDV_MinMaxUInt(pDX, m_txt_year, 1900, 2030);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CJ6icnvDlg, CDialog)
	//{{AFX_MSG_MAP(CJ6icnvDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_SEL, OnBtnSel)
	ON_BN_CLICKED(IDC_BTN_CNV, OnBtnCnv)
	ON_BN_CLICKED(IDC_BTN_PROF, OnBtnProf)
	ON_BN_CLICKED(IDC_BTN_SHW, OnBtnShw)
	ON_BN_CLICKED(IDC_CHK_YEAR, OnChkYear)
	ON_EN_CHANGE(IDC_TXT_YEAR, OnChangeTxtYear)
	ON_BN_CLICKED(IDC_RADIO_SELMODE_1, OnRadioSelmode1)
	ON_BN_CLICKED(IDC_RADIO_SELMODE_2, OnRadioSelmode2)
	ON_WM_VSCROLL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CJ6icnvDlg メッセージ ハンドラ

BOOL CJ6icnvDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// "バージョン情報..." メニュー項目をシステム メニューへ追加します。

	// IDM_ABOUTBOX はコマンド メニューの範囲でなければなりません。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	CString strAboutMenu;
	strAboutMenu.LoadString(IDS_ABOUTBOX);
	if (!strAboutMenu.IsEmpty())
	{
		pSysMenu->AppendMenu(MF_SEPARATOR);
		pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
	}
	
	// このダイアログ用のアイコンを設定します。フレームワークはアプリケーションのメイン
	// ウィンドウがダイアログでない時は自動的に設定しません。
	SetIcon(m_hIcon, TRUE);			// 大きいアイコンを設定
	SetIcon(m_hIcon, FALSE);		// 小さいアイコンを設定
	
	// TODO: 特別な初期化を行う時はこの場所に追加してください。
	CString SrcFname;	// 入力ファイル名（ダイアログ項目から取得する）
	CString DstPath;	// 出力ファイル名（パス）
	CString DstFname;	// 出力ファイル名（パス）
	CString FnameBody;	// 拡張子なしのファイル名

	// ダイアログ項目の初期設定
	if(theApp->m_year)
	{
		m_chk_year_ctrl.SetCheck(TRUE);
		m_txt_year_ctrl.EnableWindow(TRUE);
	}
	else
	{
		m_chk_year_ctrl.SetCheck(FALSE);
		m_txt_year_ctrl.EnableWindow(FALSE);
	}

	CheckRadioButton(IDC_RADIO_SELMODE_1, IDC_RADIO_SELMODE_2, IDC_RADIO_SELMODE_1);

	// ファイル名が起動オプションとして渡されたとき、ファイルをコンバートする
	if(theApp->m_shell_j6i_fname != (char)NULL)
	{	// 引数としてファイル名が指定されている場合
		SrcFname = theApp->m_shell_j6i_fname;
		// ﾌｧｲﾙ名テキストボックスにファイル名をセット
		SetDlgItemText(IDC_TXT_FNAME, SrcFname);

		// 出力パスの指定
		if(theApp->m_outpath)
		{	// 出力パス固定の時
			DstPath = theApp->m_outpath_fname;
		}
		else DstPath = "";


		if(theApp->m_directview)
		{	// 画像をコンバートし、表示する
			SetDlgItemText(IDC_TXT_MES, "");	// ﾒｯｾｰｼﾞｴﾘｱのｸﾘｱ
			// ファイルのコンバート
			if(theApp->m_year)
				::ConvertJ6I((LPCSTR)SrcFname, (LPCSTR)DstPath, m_tmpMsg, m_hWnd, theApp->m_year_data, theApp->m_confirm_ovwr);
			else
				::ConvertJ6I((LPCSTR)SrcFname, (LPCSTR)DstPath, m_tmpMsg, m_hWnd, 0, theApp->m_confirm_ovwr);
			m_last_j6i_file = SrcFname;
			// 画像を表示する外部プログラムを実行
			OnBtnShw();
			// プレビュー後JPGファイルを消去する
			if(theApp->m_delete) 
			{
				DstFname = ::GetJ6iName(SrcFname, DstPath);
				::remove(DstFname);
			}
			DspMes(m_tmpMsg);

		}
	}

	return TRUE;  // TRUE を返すとコントロールに設定したフォーカスは失われません。
}

// システムメニューの処理
// ここでは、「About」ダイアログの表示メニューの処理をする
void CJ6icnvDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// ダイアログが破棄されるときには、ヘルプファイルを閉じる
void CJ6icnvDlg::OnDestroy()
{
	WinHelp(0L, HELP_QUIT);
	CDialog::OnDestroy();
}

// もしダイアログボックスに最小化ボタンを追加するならば、アイコンを描画する
// コードを以下に記述する必要があります。MFC アプリケーションは document/view
// モデルを使っているので、この処理はフレームワークにより自動的に処理されます。

void CJ6icnvDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 描画用のデバイス コンテキスト

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// クライアントの矩形領域内の中央
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// アイコンを描画します。
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// システムは、ユーザーが最小化ウィンドウをドラッグしている間、
// カーソルを表示するためにここを呼び出します。
HCURSOR CJ6icnvDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

// 「選択」ボタンが押されたとき。J6I ファイルの選択
void CJ6icnvDlg::OnBtnSel() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください

	if(GetCheckedRadioButton(IDC_RADIO_SELMODE_1, IDC_RADIO_SELMODE_2) == 
		IDC_RADIO_SELMODE_1)
	{	// ファイルを選択する
	
		CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY|OFN_FILEMUSTEXIST|OFN_EXPLORER,
			"J6I ファイル|*.J6I|すべてのファイル|*.*||");
		dlg.m_ofn.lpstrTitle = "変換元の J6I イメージファイルの選択";

		if(dlg.DoModal() != IDOK) return;

		// ﾌｧｲﾙ名テキストボックスにファイル名をセット
		SetDlgItemText(IDC_TXT_FNAME,dlg.GetPathName());
	}
	else
	{	// フォルダを選択する
		char s_full[MAX_PATH];
		char sTitle[] = "変換元フォルダの選択";
		LPITEMIDLIST pidl;
		BROWSEINFO bi;
		ZeroMemory(&bi,sizeof(bi));
		bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_STATUSTEXT;
		bi.lpszTitle = sTitle;
		bi.hwndOwner = m_hWnd;

		pidl = ::SHBrowseForFolder(&bi);
		if(pidl == NULL) return;
		SHGetPathFromIDList(pidl, s_full);


		// ルートフォルダの扱い。最後に \ がついているかどうか
		if(s_full[strlen(s_full)-1] != '\\')
			strcat(s_full, "\\");
		// ファイル入力ボックスをアップデートする
		SetDlgItemText(IDC_TXT_FNAME, s_full);
	}
}

// コンバートボタンが押された時
void CJ6icnvDlg::OnBtnCnv() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	CString SrcFname;	// 入力ファイル名（ダイアログ項目から取得する）
	CString SrcPath;	// 入力ファイルのパス名
	CString DstPath;	// 出力ファイル名（パスつき）
	CString FnameBody;	// 拡張子なしのファイル名
	CString tmpStr;

	SetDlgItemText(IDC_TXT_MES, "");	// ﾒｯｾｰｼﾞｴﾘｱのｸﾘｱ
	strcpy(m_tmpMsg, "");	// メッセージ用バッファのクリア

	// ファイル入力ボックスに、入力されているかのチェック
	GetDlgItemText(IDC_TXT_FNAME, SrcFname);	// ダイアログから入力ファイル名を取得
	if(SrcFname == "" || SrcFname.GetLength() > (_MAX_DRIVE+_MAX_DIR+_MAX_FNAME))
	{	//	入力ファイル名が無いか、長すぎる場合
		tmpStr.LoadString(IDS_ERR_NOFIL_OR_LONG);
		SetDlgItemText(IDC_TXT_MES, tmpStr);
		return ;
	}

	// 出力パスの指定
	if(theApp->m_outpath)
	{	// 出力パス固定の時
		DstPath = theApp->m_outpath_fname;
	}
	else DstPath = "";

	// 変換形式を判別する
	if(GetCheckedRadioButton(IDC_RADIO_SELMODE_1, IDC_RADIO_SELMODE_2) == 
		IDC_RADIO_SELMODE_1)
	{	// ファイルひとつを変換する
		// ファイル名は SrcFname = "X:\XXX\ZZZZ.J6I" のように完全に入力されている
		if(theApp->m_year)
			::ConvertJ6I((LPCSTR)SrcFname, (LPCSTR)DstPath, m_tmpMsg, m_hWnd, theApp->m_year_data, theApp->m_confirm_ovwr);
		else
			::ConvertJ6I((LPCSTR)SrcFname, (LPCSTR)DstPath, m_tmpMsg, m_hWnd, 0, theApp->m_confirm_ovwr);
		m_last_j6i_file = SrcFname;
		tmpStr = m_tmpMsg;
	}
	else
	{	// フォルダ内連続変換
		// ファイル名入力ボックスはパスだけのため、"*.J6I"を追加する
		CString WildcardFname = SrcFname + "*.J6I";
		SrcPath = SrcFname;
		struct _finddata_t finddata;
		long hFind;

	    if( (hFind = _findfirst((LPCSTR)WildcardFname, &finddata )) == -1L )
		{
			DspMes("ｺﾝﾊﾞｰﾄ対象ﾌｧｲﾙが見つからない\r\n");
			return;
		}
		if(!(finddata.attrib & _A_SUBDIR))
		{
			SrcFname = SrcPath + finddata.name;
			if(theApp->m_year)
				::ConvertJ6I((LPCSTR)SrcFname, (LPCSTR)DstPath, m_tmpMsg, m_hWnd, theApp->m_year_data, theApp->m_confirm_ovwr);
			else
				::ConvertJ6I((LPCSTR)SrcFname, (LPCSTR)DstPath, m_tmpMsg, m_hWnd, 0, theApp->m_confirm_ovwr);
			m_last_j6i_file = SrcFname;
			tmpStr = m_tmpMsg;
			DspMes(tmpStr);
		}
		while(_findnext( hFind, &finddata ) == 0)
		{
			strcpy(m_tmpMsg, "");	// メッセージ用バッファのクリア
			if(!(finddata.attrib & _A_SUBDIR))
			{
				SrcFname = SrcPath + finddata.name;
				if(theApp->m_year)
					::ConvertJ6I((LPCSTR)SrcFname, (LPCSTR)DstPath, m_tmpMsg, m_hWnd, theApp->m_year_data, theApp->m_confirm_ovwr);
				else
					::ConvertJ6I((LPCSTR)SrcFname, (LPCSTR)DstPath, m_tmpMsg, m_hWnd, 0, theApp->m_confirm_ovwr);
				m_last_j6i_file = SrcFname;
				tmpStr += m_tmpMsg;
				DspMes(tmpStr);
				// 常に最終行を表示しておく
				m_txt_mes_ctrl.LineScroll(m_txt_mes_ctrl.GetLineCount());
			}
		}
	}
	// メッセージエリアを更新（情報の表示）
	DspMes(tmpStr);
	// 常に最終行を表示しておく
	m_txt_mes_ctrl.LineScroll(m_txt_mes_ctrl.GetLineCount());
}


// メッセージエリアに文字列を表示する
//
// str2に値が入れられた場合は、いわゆる printf("...%s", str) のように扱う
void CJ6icnvDlg::DspMes(CString str, CString str2)
{
	CString strTemp;

	if(str2 != "") strTemp.Format(str, str2);
	else strTemp = str;

	SetDlgItemText(IDC_TXT_MES, strTemp);
	return;
}

// 機能設定ダイアログを開く
void CJ6icnvDlg::OnBtnProf() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	CProfDlg profDlg;

	// ダイアログボックスの項目を設定
	profDlg.m_chk_ovwr = theApp->m_confirm_ovwr;
	profDlg.m_chk_ext = theApp->m_extvwer;
	profDlg.m_paramvw = theApp->m_directview;
	profDlg.m_outpath = theApp->m_outpath;
	profDlg.m_chk_delete = theApp->m_delete;
	profDlg.m_chk_nodlg = theApp->m_nodialog;
	profDlg.m_chk_year = theApp->m_year;
	profDlg.m_txt_extviewer = theApp->m_extvwer_fname;
	profDlg.m_txt_outpath = theApp->m_outpath_fname;
	profDlg.m_txt_year = theApp->m_year_data;

	if (profDlg.DoModal() == IDOK)
	{
		// レジストリを内部変数に格納し直す
		theApp->m_confirm_ovwr = profDlg.m_chk_ovwr;
		theApp->m_extvwer = profDlg.m_chk_ext;
		theApp->m_directview = profDlg.m_paramvw;
		theApp->m_outpath = profDlg.m_outpath;
		theApp->m_delete = profDlg.m_chk_delete;
		theApp->m_nodialog = profDlg.m_chk_nodlg;
		theApp->m_year = profDlg.m_chk_year;
		theApp->m_extvwer_fname = profDlg.m_txt_extviewer;
		theApp->m_outpath_fname = profDlg.m_txt_outpath;
		theApp->m_year_data = profDlg.m_txt_year;
		m_chk_year_ctrl.SetCheck(profDlg.m_chk_year);
		// 年号変換については、メインダイアログの同じ項目をアップデートする
		SetDlgItemInt(IDC_TXT_YEAR, profDlg.m_txt_year);
		if(profDlg.m_chk_year) m_txt_year_ctrl.EnableWindow(TRUE);
		else m_txt_year_ctrl.EnableWindow(FALSE);
	}
}

// 変換後の画像を表示する
void CJ6icnvDlg::OnBtnShw() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	CString DstPath;
	CString DstFname;

	if(m_last_j6i_file =="")
	{
		DspMes("まず、J6Iファイルを変換してください");
		return;
	}

	// 出力パスの指定
	if(theApp->m_outpath)
	{	// 出力パス固定の時
		DstPath = theApp->m_outpath_fname;
	}
	else DstPath = "";

	DstFname = ::GetJ6iName(m_last_j6i_file, DstPath);
	if(theApp->m_extvwer)
	{
		if(theApp->m_delete) 
		{	// 出力ファイル消去モードのときは、プロセス終了を待つ
			if(_spawnl(_P_WAIT, theApp->m_extvwer_fname, theApp->m_extvwer_fname, DstFname, NULL)<0)
				DspMes("外部 Viewer が実行できない");
		}
		else
		{
			if(_spawnl(_P_NOWAIT, theApp->m_extvwer_fname, theApp->m_extvwer_fname, DstFname, NULL)<0)
				DspMes("外部 Viewer が実行できない");
		}
	}
	else
	{
		::ShellExecute(m_hWnd, NULL, DstFname, NULL, NULL, SW_SHOW);
	}
}

// 年号変換のチェックボックスで、年号入力領域の「有効・無効」の切り替えをする
void CJ6icnvDlg::OnChkYear() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	if(!IsDlgButtonChecked(IDC_CHK_YEAR))
	{
		m_txt_year_ctrl.EnableWindow(FALSE);
		theApp->m_year = FALSE;
	}
	else
	{
		m_txt_year_ctrl.EnableWindow(TRUE);
		theApp->m_year = TRUE;
	}	
}

// 年号入力ボックスに新しい年号が入力された場合
// 共通領域のデータをアップデートする
void CJ6icnvDlg::OnChangeTxtYear() 
{
	// TODO: これが RICHEDIT コントロールの場合、コントロールは、 lParam マスク
	// 内での論理和の ENM_CHANGE フラグ付きで CRichEditCrtl().SetEventMask()
	// メッセージをコントロールへ送るために CDialog::OnInitDialog() 関数をオーバー
	// ライドしない限りこの通知を送りません。
	
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	theApp->m_year_data = GetDlgItemInt(IDC_TXT_YEAR);
}

// ラジオボタン「ひとつのファイルを処理」をクリック
void CJ6icnvDlg::OnRadioSelmode1() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	SetDlgItemText(IDC_TXT_FNAME, "");	// ファイル入力エリアをクリア
}

// ラジオボタン 「連続」をクリック
void CJ6icnvDlg::OnRadioSelmode2() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	SetDlgItemText(IDC_TXT_FNAME, "");	// ファイル入力エリアをクリア	
}

// 年号入力ボックスの右にある、スクロールボタンを押した場合の処理
void CJ6icnvDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// このメッセージ関数には、すべてのスクロールボタンのメッセージが入ってくる。
	// したがって、特定のスクロールボタンを判定してから処理する
	//
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
