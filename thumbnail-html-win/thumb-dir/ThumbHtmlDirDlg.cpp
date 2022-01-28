// SumbHtmlDirDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "SumbHtmlDir.h"
#include "SumbHtmlDirDlg.h"
#include <io.h>			// findfirst


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

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

	// ClassWizard は仮想関数のオーバーライドを生成します
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV のサポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

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
// CSumbHtmlDirDlg ダイアログ

CSumbHtmlDirDlg::CSumbHtmlDirDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSumbHtmlDirDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSumbHtmlDirDlg)
	//}}AFX_DATA_INIT
	// メモ: LoadIcon は Win32 の DestroyIcon のサブシーケンスを要求しません。
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSumbHtmlDirDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSumbHtmlDirDlg)
	DDX_Control(pDX, IDC_BTN_BROWSE_ADDPATH, m_btn_browse_addpath_ctrl);
	DDX_Control(pDX, IDC_EDIT_ADDPATH, m_edit_addpath_ctrl);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSumbHtmlDirDlg, CDialog)
	//{{AFX_MSG_MAP(CSumbHtmlDirDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_BROWSE, OnBtnBrowse)
	ON_BN_CLICKED(IDC_BTN_EXEC, OnBtnExec)
	ON_BN_CLICKED(IDC_CHECK_ADDPATH, OnCheckAddpath)
	ON_BN_CLICKED(IDC_BTN_BROWSE_ADDPATH, OnBtnBrowseAddpath)
	ON_BN_CLICKED(IDC_BTN_ABOUT, OnBtnAbout)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSumbHtmlDirDlg メッセージ ハンドラ

BOOL CSumbHtmlDirDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// "バージョン情報..." メニュー項目をシステム メニューへ追加します。

	// IDM_ABOUTBOX はコマンド メニューの範囲でなければなりません。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// このダイアログ用のアイコンを設定します。フレームワークはアプリケーションのメイン
	// ウィンドウがダイアログでない時は自動的に設定しません。
	SetIcon(m_hIcon, TRUE);			// 大きいアイコンを設定
	SetIcon(m_hIcon, FALSE);		// 小さいアイコンを設定
	
	// TODO: 特別な初期化を行う時はこの場所に追加してください。
	this->SetDlgItemText(IDC_EDIT_PICS,"pics");
	this->SetDlgItemInt(IDC_EDIT_COL, 5);
	this->SetDlgItemText(IDC_EDIT_HTML, "index.html");
	this->CheckRadioButton(IDC_RADIO_FORCEWR, IDC_RADIO_FORCEWR2, IDC_RADIO_FORCEWR2);

	// 「既存フォルダ追加モード｣のコントロール無効化措置
	OnCheckAddpath();

	return TRUE;  // TRUE を返すとコントロールに設定したフォーカスは失われません。
}

void CSumbHtmlDirDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// もしダイアログボックスに最小化ボタンを追加するならば、アイコンを描画する
// コードを以下に記述する必要があります。MFC アプリケーションは document/view
// モデルを使っているので、この処理はフレームワークにより自動的に処理されます。

void CSumbHtmlDirDlg::OnPaint() 
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
HCURSOR CSumbHtmlDirDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

// フォルダの参照
void CSumbHtmlDirDlg::OnBtnBrowse() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	char s_full[MAX_PATH];
	LPITEMIDLIST pidl;
	BROWSEINFO bi;
	ZeroMemory(&bi,sizeof(bi));
	bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_STATUSTEXT;
	bi.lpszTitle = (LPCSTR)"親フォルダの選択";
	bi.hwndOwner = m_hWnd;

	pidl = ::SHBrowseForFolder(&bi);
	if(pidl == NULL) return;
	SHGetPathFromIDList(pidl, s_full);

	// ルートフォルダの扱い。最後に \ がついているかどうか
	if(s_full[strlen(s_full)-1] != '\\')
		strcat(s_full, "\\");
	// ファイル入力ボックスをアップデートする
	SetDlgItemText(IDC_EDIT_PATH, s_full);
	
}
// フォルダの参照
void CSumbHtmlDirDlg::OnBtnBrowseAddpath() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	char s_full[MAX_PATH];
	LPITEMIDLIST pidl;
	BROWSEINFO bi;
	ZeroMemory(&bi,sizeof(bi));
	bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_STATUSTEXT;
	bi.lpszTitle = (LPCSTR)"親フォルダの選択";
	bi.hwndOwner = m_hWnd;

	pidl = ::SHBrowseForFolder(&bi);
	if(pidl == NULL) return;
	SHGetPathFromIDList(pidl, s_full);

	// ルートフォルダの扱い。最後に \ がついているかどうか
	if(s_full[strlen(s_full)-1] != '\\')
		strcat(s_full, "\\");
	// ファイル入力ボックスをアップデートする
	SetDlgItemText(IDC_EDIT_ADDPATH, s_full);
}	
	

// 処理実行
#define	MAX_PASS_ARRAY	400		// 子フォルダの最大個数

void CSumbHtmlDirDlg::OnBtnExec() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	CFile f_html;						// ファイルハンドラ
	_fstr Pass_Array[MAX_PASS_ARRAY];	// 子フォルダをソートして格納するための配列
	int pass_count = 0;					// 格納している子フォルダ数 1～
	CString FpathParent;				// 親パス（フルパス）
	CString FpathParentSearch;			// 親パス（findfirst用ワイルドカードつき）
	CString FpathHTML;					// HTMLフルパス
	CString FpathPICS;					// pic フルパス
	CString FpathSPICS;					// pic パス
	CString FpathAddPath;				// 既存フォルダ追加用、既存パス（フルパス）
	CString FpathAddPathSearch;			// 既存フォルダパス（findfirst用ワイルドカードつき）
	DWORD _fattr;						// ファイル・フォルダの存在をチェックする時の変数
	CString StrTmp;
	int i, j;

	GetDlgItemText(IDC_EDIT_PASS, FpathParent);	// 親パスを得る
	GetDlgItemText(IDC_EDIT_HTML, FpathHTML);	// HTML名を得る
	FpathHTML = FpathParent + FpathHTML;		// HTML フルパスの作成
	GetDlgItemText(IDC_EDIT_PICS, FpathSPICS);	// PICSサブパスを得る
	FpathPICS = FpathParent + FpathSPICS;		// PICSフルパス
	FpathParentSearch = FpathParent + "*";		// findfirst 用ワイルドカード付加

	GetDlgItemText(IDC_EDIT_ADDPATH, FpathAddPath);	// 既存フォルダパスを得る
	FpathAddPathSearch = FpathAddPath + "*";		// findfirst 用ワイルドカード付加

	// フォルダ名入力済みのチェック
	if(!strlen(FpathParent))
	{
		MessageBox("親フォルダ名が設定されていません");
		return;
	}
	if(IsDlgButtonChecked(IDC_CHECK_ADDPATH) && !strlen(FpathAddPath))
	{
		MessageBox("既存フォルダ名が設定されていません");
		return;
	}

	// ***********
	// 子フォルダを検索して、フォルダ名を配列に格納する
	// ***********
	struct _finddata_t finddata;
	long hFind;

    if( (hFind = _findfirst((LPCSTR)FpathParentSearch, &finddata )) == -1L )
	{	// 参照下ファイルが見つからない場合
	}
	else{
		if(finddata.attrib & _A_SUBDIR)
		{	// 一つ目のフォルダの発見
			if(finddata.name[0] != '.' && stricmp(finddata.name, FpathSPICS) != 0)
			{	// "." , ".." や PICS フォルダの除外
				strcpy(Pass_Array[pass_count].str, finddata.name);
				pass_count++;
			}
		}
		while(_findnext( hFind, &finddata ) == 0)
		{	// 続くフォルダを連続発見
			if(finddata.attrib & _A_SUBDIR)
			{
				if(finddata.name[0] != '.' && stricmp(finddata.name, FpathSPICS) != 0)
				{	// "." , ".." や PICS フォルダの除外
					strcpy(Pass_Array[pass_count].str, finddata.name);
					pass_count++;
					if(pass_count >= MAX_PASS_ARRAY) break;
				}
			}
		}
	}
	if(pass_count >= MAX_PASS_ARRAY)
		MessageBox("子フォルダ数が処理限界を超えました\r\n途中まで処理します");

	if(!pass_count)
	{	// 子フォルダが見つからないとき
		MessageBox("子フォルダが存在しません。処理を中止します。");
		return ;
	}

	// フォルダ配列のソート
	SortArray(Pass_Array, pass_count);

	// ***********
	// pics フォルダの作成
	// ***********
	_fattr = ::GetFileAttributes(FpathPICS);		// フォルダの存在の確認
	if(_fattr == -1)
	{	// 同名のフォルダもファイルも存在しないと思われる
		if(!::CreateDirectory(FpathPICS, NULL))
		{
			MessageBox("サムネイル フォルダの作成に失敗しました");
			return ;
		}
	}
	else if(_fattr & FILE_ATTRIBUTE_DIRECTORY)
	{
		StrTmp = "サムネイルフォルダ \"" + FpathSPICS + "\" がすでに存在します\nこのフォルダを利用しますか？";
		if(MessageBox(StrTmp, "重複フォルダの確認", MB_ICONQUESTION|MB_YESNO) == IDNO)
			return;

	}
	else
	{
		MessageBox("同名のファイルがあるため、サムネイル フォルダが作成できません");
		return ;
	}


	// ***********
	// サムネイル用ファイルをPICSフォルダにコピー
	// ***********
	CString StrInName, StrOutName;
	for(i=0; i<pass_count; i++)
	{
		// サムネイルファイル名の取得
		StrTmp.Format("%s%s\\*.jp*", FpathParent, Pass_Array[i].str);	// フォルダのフルパス
		StrTmp = GetSumbFname((LPCSTR)StrTmp);	// フォルダ内の「最小の｣名前のファイル名を得る

		// ファイルのコピー
		StrInName.Format("%s%s\\%s", FpathParent, Pass_Array[i].str, StrTmp);
		StrOutName.Format("%s\\%s.jpg", FpathPICS, Pass_Array[i].str);
		CopyFile((LPCSTR)StrInName, (LPCSTR)StrOutName);
	}

	// ***********
	// 既存フォルダに追加モード
	// ***********
	if(IsDlgButtonChecked(IDC_CHECK_ADDPATH))
	{
	    if( (hFind = _findfirst((LPCSTR)FpathAddPathSearch, &finddata )) == -1L )
		{	// 参照下ファイルが見つからない場合
		}
		else{
			if(finddata.attrib & _A_SUBDIR)
			{	// 一つ目のフォルダの発見
				if(finddata.name[0] != '.' && stricmp(finddata.name, FpathSPICS) != 0)
				{	// "." , ".." や PICS フォルダの除外
					strcpy(Pass_Array[pass_count].str, finddata.name);
					pass_count++;
				}
			}
			while(_findnext( hFind, &finddata ) == 0)
			{	// 続くフォルダを連続発見
				if(finddata.attrib & _A_SUBDIR)
				{
					if(finddata.name[0] != '.' && stricmp(finddata.name, FpathSPICS) != 0)
					{	// "." , ".." や PICS フォルダの除外
						strcpy(Pass_Array[pass_count].str, finddata.name);
						pass_count++;
						if(pass_count >= MAX_PASS_ARRAY) break;
					}
				}
			}
		}
		if(pass_count >= MAX_PASS_ARRAY)
			MessageBox("子フォルダ数が処理限界を超えました\r\n途中まで処理します");
		// フォルダ配列のソート
		SortArray(Pass_Array, pass_count);
	}

	// ***********
	// 既存フォルダに追加モード時の、フォルダ重複確認
	// ***********
	if(IsDlgButtonChecked(IDC_CHECK_ADDPATH))
	{
		j = 0;
		StrTmp = "";
		for(i=0; i<pass_count-1; i++)
		{
			if(!_stricmp(Pass_Array[i].str,Pass_Array[i+1].str))
			{
				StrTmp = StrTmp + Pass_Array[i].str + " ,   ";
				j++;
			}
		}
		if(j)
		{
			StrTmp = "重複フォルダが存在します\n処理を続行しますか？\n（重複フォルダは次のとおりです）\n\n" + StrTmp;
			if(MessageBox(StrTmp, "重複フォルダの確認", MB_ICONQUESTION|MB_YESNO) == IDNO)
				return;
		}

	}


	// ***********
	// HTML ファイルの作成
	// ***********

	// HTML ファイルの存在をチェックする
	if(GetCheckedRadioButton(IDC_RADIO_FORCEWR, IDC_RADIO_FORCEWR2) == IDC_RADIO_FORCEWR2)
	{
		_fattr = ::GetFileAttributes(FpathHTML);
		if(_fattr != -1)
		{
			StrTmp = "HTMLファイル \"" + FpathHTML + " \"がすでに存在します。上書きしますか？";
			if(MessageBox(StrTmp, "HTMLファイル上書きの確認", MB_ICONQUESTION|MB_YESNO) != IDYES)
				return;		// 上書きせずに終了する
		}
	}
	// HTML ファイルを開く（新規作成する）
	if(!f_html.Open(FpathHTML,CFile::modeCreate|CFile::modeWrite))
	{
		MessageBox("HTML ファイルが開けません");
		return ;
	}


	// ***********
	// HTML に書き込む
	// ***********
	int colmn, cntr;
	colmn = this->GetDlgItemInt(IDC_EDIT_COL);
	if(colmn <= 0) colmn = 5;	// 0カラムはありえない
	// ヘッダ情報を書き込む
	StrTmp = "<html>\r\n<body>\r\n<table border=\"1\"\r\n>";
	f_html.Write((LPCSTR)StrTmp, strlen(StrTmp));

	// ALINK の行き先を index.html とするための文字列設定
	CString StrAlink;
	if(IsDlgButtonChecked(IDC_CHECK_ALINK)) StrAlink = "/index.html";
	else StrAlink = "";
	// 子フォルダ情報を書き込む
	cntr = 0;	// フォルダカウンタ
	for(i=0; i<pass_count/colmn+1; i++)
	{
		StrTmp = "<tr>\r\n";
		f_html.Write((LPCSTR)StrTmp, strlen(StrTmp));
		for(j=0; j<colmn; j++)
		{	// 列の定義
			if(cntr < pass_count)
			{
				StrTmp.Format("<td><a href=\"./%s%s\"><img src=\"./%s/%s.jpg\"><br>%s</a></td>\r\n",
					Pass_Array[cntr].str,StrAlink,FpathSPICS,Pass_Array[cntr].str,Pass_Array[cntr].str);
				f_html.Write((LPCSTR)StrTmp, strlen(StrTmp));
				cntr++;
			}
			else
			{
				StrTmp = "<td></td>\r\n";
				f_html.Write((LPCSTR)StrTmp, strlen(StrTmp));
			}
		}
		StrTmp = "</tr>\r\n";
		f_html.Write((LPCSTR)StrTmp, strlen(StrTmp));
	}

	// フッタ情報を書き込む
	StrTmp = "</table>\r\n</body>\r\n</html>\r\n";
	f_html.Write((LPCSTR)StrTmp, strlen(StrTmp));

	f_html.Close();

	// 終了メッセージの表示
	StrTmp = "HTMLファイル \"" + FpathHTML + " \"の作成が正常に終了しました";
	MessageBox(StrTmp, "処理完了");
}

// ソーティング
void CSumbHtmlDirDlg::SortArray(_fstr *Pass_Array, int pass_count)
{
	int i, j;
	CString StrTmp;

	for(i=0; i<pass_count; i++)
	{
		for(j=i; j<pass_count; j++)
		{
			if(stricmp((const char*)Pass_Array[i].str, (const char*)Pass_Array[j].str) > 0)
			{
				StrTmp = Pass_Array[i].str;
				strcpy(Pass_Array[i].str, Pass_Array[j].str);
				strcpy(Pass_Array[j].str, StrTmp);
			}
		}
	}
}

// ファイルコピー
BOOL CSumbHtmlDirDlg::CopyFile(LPCSTR FpathIn, LPCSTR FpathOut)
{
	CFile f_in, f_out;
	char buf[1024];
	long i;
	int size;

	if(!f_in.Open(FpathIn, CFile::modeRead)) return FALSE;
	if(!f_out.Open(FpathOut, CFile::modeCreate|CFile::modeWrite))
	{
		f_in.Close();
		return FALSE;
	}

	for(i=0; i<(long)(f_in.GetLength()/1024L+1L); i++)
	{
		size = f_in.Read(buf,1024);
		if(size == 0) break;
		f_out.Write(buf, size);
	}

	f_out.Close();
	f_in.Close();

	return TRUE;
}

// フォルダ内の「もっとも小さい｣名前のファイル名を返す（パス名は付けないで返す）
LPSTR CSumbHtmlDirDlg::GetSumbFname(LPCSTR Fpath)
{
	struct _finddata_t finddata;
	long hFind;
	char RetName[_MAX_FNAME];
	char *RetPtr;

	strcpy(RetName, "");

    if( (hFind = _findfirst((LPCSTR)Fpath, &finddata )) == -1L )
	{	// 参照下ファイルが見つからない場合
	}
	else{
		if(!(finddata.attrib & _A_SUBDIR))
		{	// 一つ目のファイルの発見
			strcpy(RetName, finddata.name);
		}
		while(_findnext( hFind, &finddata ) == 0)
		{	// 続くファイルを連続発見
			if(!(finddata.attrib & _A_SUBDIR))
			{
				// 一番「小さい｣名前のファイル名をRetNameに保持する
				if(stricmp(finddata.name, RetName) < 0) strcpy(RetName, finddata.name);
			}
		}
	}
	RetPtr = (LPSTR)RetName;
	return RetPtr;
}

// 「既存フォルダ追加モード｣のチェックが入れられたら、インプットボックスを有効にする。
void CSumbHtmlDirDlg::OnCheckAddpath() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	if(!IsDlgButtonChecked(IDC_CHECK_ADDPATH))
	{
		m_edit_addpath_ctrl.EnableWindow(FALSE);
		m_btn_browse_addpath_ctrl.EnableWindow(FALSE);
	}
	else
	{
		m_edit_addpath_ctrl.EnableWindow(TRUE);
		m_btn_browse_addpath_ctrl.EnableWindow(TRUE);
	}	
}

// 説明ボタンを押したときの処理
void CSumbHtmlDirDlg::OnBtnAbout() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	CAboutDlg dlg;

	dlg.DoModal();
}
