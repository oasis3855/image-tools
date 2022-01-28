// ThumbHtmlDirDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "ThumbHtmlDir.h"
#include "ThumbHtmlDirDlg.h"
#include "HelpDocViewer.h"
#include <io.h>			// findfirst


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// **********************************
// グローバル変数・定数
// **********************************
#define	MAX_PATH_ARRAY	400			// 子フォルダの最大個数
#define MAX_EXT			125			// EXT 指定文字列の最大長さ


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
// CThumbHtmlDirDlg ダイアログ

CThumbHtmlDirDlg::CThumbHtmlDirDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CThumbHtmlDirDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CThumbHtmlDirDlg)
	//}}AFX_DATA_INIT
	// メモ: LoadIcon は Win32 の DestroyIcon のサブシーケンスを要求しません。
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CThumbHtmlDirDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CThumbHtmlDirDlg)
	DDX_Control(pDX, IDC_CMB_IMGOPT, m_cmb_imgopt_ctrl);
	DDX_Control(pDX, IDC_SPIN_COLUMN, m_spin_col_ctrl);
	DDX_Control(pDX, IDC_CMB_PICSEXT, m_cmb_picsext_ctrl);
	DDX_Control(pDX, IDC_CMB_PHOTOEXT, m_cmb_photoext_ctrl);
	DDX_Control(pDX, IDC_BTN_BROWSE_ADDPATH, m_btn_browse_addpath_ctrl);
	DDX_Control(pDX, IDC_EDIT_ADDPATH, m_edit_addpath_ctrl);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CThumbHtmlDirDlg, CDialog)
	//{{AFX_MSG_MAP(CThumbHtmlDirDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_BROWSE, OnBtnBrowse)
	ON_BN_CLICKED(IDC_BTN_EXEC, OnBtnExec)
	ON_BN_CLICKED(IDC_CHECK_ADDPATH, OnCheckAddpath)
	ON_BN_CLICKED(IDC_BTN_BROWSE_ADDPATH, OnBtnBrowseAddpath)
	ON_BN_CLICKED(IDC_BTN_ABOUT, OnBtnAbout)
	ON_BN_CLICKED(IDC_BTN_PICS_BROWSE, OnBtnPicsBrowse)
	ON_BN_CLICKED(IDC_BTN_HELP, OnBtnHelp)
	ON_WM_HELPINFO()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CThumbHtmlDirDlg メッセージ ハンドラ

BOOL CThumbHtmlDirDlg::OnInitDialog()
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

	////////////////////////
	// ダイアログの初期値を設定
	////////////////////////
	this->SetDlgItemText(IDC_EDIT_PHOTO,"*");
	this->SetDlgItemText(IDC_EDIT_PICS,"pics");
	m_cmb_photoext_ctrl.SetCurSel(1);	// 「*.jp*」
	m_cmb_picsext_ctrl.SetCurSel(0);	// 「*.jpg」
	this->SetDlgItemInt(IDC_EDIT_COL, 5);
	m_spin_col_ctrl.SetRange(1,10);		// カラム数スピンコントロールの範囲指定
	this->SetDlgItemText(IDC_EDIT_HTML, "index.html");
	m_cmb_imgopt_ctrl.SetCurSel(0);		// 「border=0」を選択
	this->CheckRadioButton(IDC_RADIO_FORCEWR, IDC_RADIO_FORCEWR2, IDC_RADIO_FORCEWR2);

	// 「既存フォルダ追加モード｣のコントロール無効化措置
	OnCheckAddpath();

	return TRUE;  // TRUE を返すとコントロールに設定したフォーカスは失われません。
}

void CThumbHtmlDirDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CThumbHtmlDirDlg::OnPaint() 
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
HCURSOR CThumbHtmlDirDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

// **********************************
// 親フォルダの参照
// **********************************
void CThumbHtmlDirDlg::OnBtnBrowse() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	char strPathname[MAX_PATH];
	LPITEMIDLIST pidl;
	BROWSEINFO bi;

	// テキストボックスに入力されている親パスを得る
	GetDlgItemText(IDC_EDIT_PASS, strPathname, MAX_PATH-1);

	// Win95 対策 （最後に \ が付いたフォルダ名は受け付けない）
	if(strlen(strPathname)>3)		// ルートフォルダと空白は除く
	{
		if(strPathname[strlen(strPathname)-1] == '\\')
			strPathname[strlen(strPathname)-1] = (char)NULL;
	}


	// BROWSEINFO 構造体の初期化
	ZeroMemory(&bi,sizeof(bi));
	bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_STATUSTEXT;
	bi.lpszTitle = (LPCSTR)"親フォルダの選択";
	bi.hwndOwner = m_hWnd;
	bi.lpfn = &myBrowseCallbackProc;	// 初期フォルダ設定のためコールバック関数設定
	bi.lParam = (LPARAM)strPathname;	// 初期フォルダをパラメータとして設定

	pidl = ::SHBrowseForFolder(&bi);
	if(pidl == NULL) return;
	SHGetPathFromIDList(pidl, strPathname);

	////////////////////////
	// pidl を明示的に解放する
	////////////////////////
	IMalloc *imalloc;
	// Shellのメモリ管理オブジェクトを取得
	if( ::SHGetMalloc( &imalloc) != NOERROR) imalloc = NULL;
	// オブジェクト取得に成功した場合に開放する
	if( imalloc != NULL) imalloc->Free((void *)pidl); // ITEMIDLISTを開放

	// ルートフォルダの扱い。最後に \ がついているかどうか
	if(strPathname[strlen(strPathname)-1] != '\\')
		strcat(strPathname, "\\");
	// ファイル入力ボックスをアップデートする
	SetDlgItemText(IDC_EDIT_PASS, strPathname);

}

// **********************************
// 追加フォルダの参照
// **********************************
void CThumbHtmlDirDlg::OnBtnBrowseAddpath() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	char strPathname[MAX_PATH];
	LPITEMIDLIST pidl;
	BROWSEINFO bi;

	// テキストボックスに入力されている親パスを得る
	GetDlgItemText(IDC_EDIT_ADDPATH, strPathname, MAX_PATH-1);

	// Win95 対策 （最後に \ が付いたフォルダ名は受け付けない）
	if(strlen(strPathname)>3)			// ルートフォルダと空白は除く
	{
		if(strPathname[strlen(strPathname)-1] == '\\')
			strPathname[strlen(strPathname)-1] = (char)NULL;
	}

	// BROWSEINFO 構造体の初期化
	ZeroMemory(&bi,sizeof(bi));
	bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_STATUSTEXT;
	bi.lpszTitle = (LPCSTR)"追加対象フォルダの選択";
	bi.hwndOwner = m_hWnd;
	bi.lpfn = &myBrowseCallbackProc;	// 初期フォルダ設定のためコールバック関数設定
	bi.lParam = (LPARAM)strPathname;	// 初期フォルダをパラメータとして設定

	pidl = ::SHBrowseForFolder(&bi);
	if(pidl == NULL) return;
	SHGetPathFromIDList(pidl, strPathname);

	////////////////////////
	// pidl を明示的に解放する
	////////////////////////
	IMalloc *imalloc;
	// Shellのメモリ管理オブジェクトを取得
	if( ::SHGetMalloc( &imalloc) != NOERROR) imalloc = NULL;
	// オブジェクト取得に成功した場合に開放する
	if( imalloc != NULL) imalloc->Free((void *)pidl); // ITEMIDLISTを開放

	// ルートフォルダの扱い。最後に \ がついているかどうか
	if(strPathname[strlen(strPathname)-1] != '\\')
		strcat(strPathname, "\\");
	// ファイル入力ボックスをアップデートする
	SetDlgItemText(IDC_EDIT_ADDPATH, strPathname);

}	

// **********************************
// PICSフォルダの参照
// **********************************
void CThumbHtmlDirDlg::OnBtnPicsBrowse() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	BrowseSubPath(IDC_EDIT_PICS);	
	
}

// **********************************
// PHOTO,PICSフォルダの参照のダイアログ選択処理
// **********************************
void CThumbHtmlDirDlg::BrowseSubPath(int nID)
{
	char strPathname[MAX_PATH];
	char strRootPathname[MAX_PATH];
	LPITEMIDLIST pidl, pidlRoot;
	BROWSEINFO bi;

	// テキストボックスに入力されている親パスを得る
	GetDlgItemText(IDC_EDIT_PASS, strPathname, MAX_PATH-1);
	if(strlen(strPathname) <= 0) return;	// 何も入力されていないとき 
	// ルートフォルダの扱い。最後に \ がついているかどうか
	if(strPathname[strlen(strPathname)-1] != '\\')
		strcat(strPathname, "\\");
	strcpy(strRootPathname , strPathname);

	// BROWSEINFO 構造体の初期化
	ZeroMemory(&bi,sizeof(bi));
	bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_STATUSTEXT;
	bi.lpszTitle = (LPCSTR)"親フォルダの選択";
	bi.hwndOwner = m_hWnd;
	bi.lpfn = &myBrowseCallbackProc;	// 初期フォルダ設定のためコールバック関数設定
	bi.lParam = (LPARAM)strPathname;	// 初期フォルダをパラメータとして設定
	pidlRoot = ConvertPIDL(strRootPathname);
	bi.pidlRoot = pidlRoot;

	pidl = ::SHBrowseForFolder(&bi);
	if(pidl == NULL) return;			// キャンセルボタンが押された
	SHGetPathFromIDList(pidl, strPathname);

	////////////////////////
	// pidlRoot, pidl を明示的に解放する
	////////////////////////
	IMalloc *imalloc;
	// Shellのメモリ管理オブジェクトを取得
	if( ::SHGetMalloc( &imalloc) != NOERROR) imalloc = NULL;
	// オブジェクト取得に成功した場合に開放する
	if( imalloc != NULL) imalloc->Free((void *)pidlRoot); // ITEMIDLISTを開放

	// Shellのメモリ管理オブジェクトを取得
	if( ::SHGetMalloc( &imalloc) != NOERROR) imalloc = NULL;
	// オブジェクト取得に成功した場合に開放する
	if( imalloc != NULL) imalloc->Free((void *)pidl); // ITEMIDLISTを開放


	// ルートパスを取り除き、相対パスのみを取り出す
	strcpy(strRootPathname, strPathname+strlen(strRootPathname));
	// ファイル入力ボックスをアップデートする
	SetDlgItemText(nID, strRootPathname);
}

	
// **********************************
// フォルダ選択 SDK ダイアログ ::SHBrowseForFolder 用の コールバック関数
// **********************************
int CALLBACK myBrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
{
	// ダイアログが初期化されるとき （BFFM_INITIALIZED）
    if(uMsg == BFFM_INITIALIZED){
		// フォルダを選択する （BFFM_SETSELECTION）メッセージ
		if(lpData != NULL)
	        SendMessage(hwnd, BFFM_SETSELECTION, (WPARAM)TRUE, lpData);
    }
    return 0;
}

// **********************************
// フォルダ選択 SDK ダイアログ ::SHBrowseForFolder 用
// ルートフォルダの指定のためのフォルダ名コンバート
// **********************************
LPITEMIDLIST ConvertPIDL(char *lpszPath)
{
	ULONG			nDummy;
	ULONG			dwAttributes;
	OLECHAR			olePath[MAX_PATH];
	LPSHELLFOLDER	pDesktopFolder;
	LPITEMIDLIST	pidl;

	if(SUCCEEDED(::SHGetDesktopFolder(&pDesktopFolder))) {
		MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, lpszPath, -1, olePath, MAX_PATH);
		if(FAILED(pDesktopFolder->ParseDisplayName(NULL, NULL, olePath, &nDummy, &pidl, &dwAttributes)))
		{
			pidl = NULL;
		}
	}
	return(pidl);
}

// **********************************
// 処理実行
// **********************************
void CThumbHtmlDirDlg::OnBtnExec() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	CFile fileOutHtml;							// ファイルハンドラ
	_fstr fstrPathArray[MAX_PATH_ARRAY];	// 子フォルダをソートして格納するための配列
	int nFolderCount;						// 格納している子フォルダ数 1～
	CString strFnameBaseFolder;				// 親パス（フルパス）
	CString strFnameImgSearch;				// 親パス（findfirst用ワイルドカードつき）
	CString strFnameHtmlPath;				// HTMLフルパス
	CString strFnamePicsRelativePath;		// pics 相対パス (サムネイル用画像 IMG SRC用)
	CString strFnamePicsExt;				// pics へ格納する画像拡張子
	CString strFnameImgSearchExt;			// photo の対象拡張子（複数指定はセミコロン区切り）
	CString strFnameAddBaseFolder;			// 既存フォルダ追加用、既存パス（フルパス）
	CString strFnameAddSearch;				// 既存フォルダパス（findfirst用ワイルドカードつき）
	CString strTagImgOpt;					// IMG タグオプション文字列
	CString strTagLinkOpt;					// A LINK タグオプション文字列
	CString strALink;						// A LINK 先のファイル名
	int nHtmlColumn;						// HTML カラム数
	DWORD _fattr;							// ファイル・フォルダの存在をチェックする時の変数
	CString strTmp, strTmp2;
	int i, j;

	////////////////////////
	// 各変数に初期値を代入  (これらの値は実行途中に変化することは無い)
	////////////////////////
	GetDlgItemText(IDC_EDIT_PASS, strFnameBaseFolder);			// 親パスを得る
	GetDlgItemText(IDC_EDIT_HTML, strFnameHtmlPath);			// HTML名を得る
	GetDlgItemText(IDC_EDIT_PICS, strFnamePicsRelativePath);	// PICSサブパスを得る
	GetDlgItemText(IDC_CMB_PHOTOEXT, strFnameImgSearchExt);		// PHOTO 対象拡張子
	GetDlgItemText(IDC_CMB_PICSEXT, strFnamePicsExt);			// PICS 対象拡張子
	GetDlgItemText(IDC_CMB_IMGOPT, strTagImgOpt);				// IMG タグオプション
	GetDlgItemText(IDC_CMB_LINKOPT, strTagLinkOpt);				// A LINK タグオプション
	nHtmlColumn = GetDlgItemInt(IDC_EDIT_COL);
	GetDlgItemText(IDC_EDIT_ADDPATH, strFnameAddBaseFolder);	// 既存フォルダパスを得る
	GetDlgItemText(IDC_CMB_LINKFILE, strALink);					// A LINK ファイル名

	////////////////////////
	// ダイアログ入力項目の正当性のチェック
	////////////////////////
	strTmp.LoadString(IDS_STR_ERRTITLE);
	if(strFnameBaseFolder == "")
	{
		strTmp2.LoadString(STR_MESDLG_BASEDIR);		// 基準パスが設定されていません
		MessageBox(strTmp2, strTmp, MB_OK|MB_ICONEXCLAMATION);
		return ;
	}
	if(strFnameHtmlPath == "")
	{
		strTmp2.LoadString(STR_MESDLG_HTMLNAME);	// HTML ファイル名が設定されていません
		MessageBox(strTmp2, strTmp, MB_OK|MB_ICONEXCLAMATION);
		return ;
	}
	if(strFnamePicsRelativePath == "")
	{
		strTmp2.LoadString(STR_MESDLG_THUMBDIR);	// サムネイル画像フォルダが設定されていません
		MessageBox(strTmp2, strTmp, MB_OK|MB_ICONEXCLAMATION);
		return ;
	}
	if(strFnameImgSearchExt == "")
	{
		strTmp2.LoadString(STR_MESDLG_EXTPHOTO);	// 画像の拡張子（検索パターン）が設定されていません
		MessageBox(strTmp2, strTmp, MB_OK|MB_ICONEXCLAMATION);
		return ;
	}
	if(strFnamePicsExt == "")
	{
		strTmp2.LoadString(STR_MESDLG_EXTTHUMBNO);	// サムネイル画像の拡張子が設定されていません
		MessageBox(strTmp2, strTmp, MB_OK|MB_ICONEXCLAMATION);
		return ;
	}
	if(IsDlgButtonChecked(IDC_CHECK_ADDPATH) && strFnameAddBaseFolder == "")
	{
		strTmp2.LoadString(STR_MESDLG_ADDPATH);		// 既存フォルダ名が設定されていません
		MessageBox(strTmp2, strTmp, MB_OK|MB_ICONEXCLAMATION);
		return;
	}

	////////////////////////
	// 各種文字列のプリプロセッシング
	////////////////////////

	// 親パスのチェック （最後に \ 記号がついていない場合は付加する）
	if(strFnameBaseFolder[strFnameBaseFolder.GetLength()-1] != '\\')
		strFnameBaseFolder = strFnameBaseFolder + "\\";

	// 追加パスのチェック （最後に \ 記号がついていない場合は付加する）
	if(strFnameAddBaseFolder != "")
	{
		if(strFnameAddBaseFolder[strFnameAddBaseFolder.GetLength()-1] != '\\')
			strFnameAddBaseFolder = strFnameAddBaseFolder + "\\";
	}

	// HTML フルパスの作成
	strFnameHtmlPath = strFnameBaseFolder + strFnameHtmlPath;

	// 追加用 検索パス（フルパス）の作成
	strFnameAddSearch = strFnameAddBaseFolder + "*";		// findfirst 用ワイルドカード付加


	// HTML カラム数のチェック
	if(nHtmlColumn <= 0 || nHtmlColumn >= 11)
	{
		strTmp2.LoadString(STR_MESDLG_COLUMN);	// 列数は 1 ～ 10 の間で設定してください
		MessageBox(strTmp2, strTmp, MB_OK|MB_ICONEXCLAMATION);
		return ;
	}

	// PICS 拡張子無効チェック
	if(strFnamePicsExt[0] != '.' || strFnamePicsExt.Find("*",0) >= 0)
	{
		strTmp2.LoadString(STR_MESDLG_EXTTHUMB);	// サムネイル画像の拡張子は、ピリオドで始まる文字列を入力してください
		MessageBox(strTmp2, strTmp, MB_OK|MB_ICONEXCLAMATION);
		return ;
	}

	// A LINK ファイル名の先頭に スラッシュをつける
	if(strALink != "")
	{
		strALink = "/" + strALink;
	}

	////////////////////////
	// 子フォルダを検索して、フォルダ名を配列に格納する
	////////////////////////
	struct _finddata_t finddata;
	long hFind;
	strFnameImgSearch = strFnameBaseFolder + "*";		// findfirst 用ワイルドカード付加
	nFolderCount = 0;

    if( (hFind = _findfirst((LPCSTR)strFnameImgSearch, &finddata )) == -1L )
	{	// 参照下ファイルが見つからない場合
	}
	else{
		if(finddata.attrib & _A_SUBDIR)
		{	// 一つ目のフォルダの発見
			if(finddata.name[0] != '.' && stricmp(finddata.name, strFnamePicsRelativePath) != 0)
			{	// "." , ".." や PICS フォルダの除外
				strcpy(fstrPathArray[nFolderCount].str, finddata.name);
				nFolderCount++;
			}
		}
		while(_findnext( hFind, &finddata ) == 0)
		{	// 続くフォルダを連続発見
			if(finddata.attrib & _A_SUBDIR)
			{
				if(finddata.name[0] != '.' && stricmp(finddata.name, strFnamePicsRelativePath) != 0)
				{	// "." , ".." や PICS フォルダの除外
					strcpy(fstrPathArray[nFolderCount].str, finddata.name);
					nFolderCount++;
					if(nFolderCount >= MAX_PATH_ARRAY) break;
				}
			}
		}
	}
	if(nFolderCount >= MAX_PATH_ARRAY)
	{
		strTmp.LoadString(STR_MESDLG_EXCEEDLIM);	// フォルダ数が処理限界を超えました\r\n途中まで処理します
		MessageBox(strTmp);
	}

	if(!nFolderCount)
	{	// 子フォルダが見つからないとき
		strTmp.LoadString(STR_MESDLG_NODIR);	// フォルダが存在しません。処理を中止します
		MessageBox(strTmp);
		return ;
	}


	////////////////////////
	// フォルダ配列のソート
	////////////////////////
	SortArray(fstrPathArray, nFolderCount);



	////////////////////////
	// pics フォルダの作成
	////////////////////////
	strTmp = strFnameBaseFolder + strFnamePicsRelativePath;	// フルパス作成
	_fattr = ::GetFileAttributes(strTmp);		// フォルダの存在の確認
	if(_fattr == -1)
	{	// 同名のフォルダもファイルも存在しないと思われる
		if(!::CreateDirectory(strTmp, NULL))
		{
			strTmp.LoadString(STR_ERR_MAKEPICDIR);	// サムネイル フォルダの作成に失敗しました
			MessageBox(strTmp);
			return ;
		}
	}
	else if(_fattr & FILE_ATTRIBUTE_DIRECTORY)
	{
		strTmp.Format(STR_MESDLG_OVERWRITE, strFnamePicsRelativePath);	// サムネイルフォルダ \"" %s "\" がすでに存在します\nこのフォルダを利用しますか？
		strTmp2.LoadString(STR_MESDLG_TITLE);	// ThumbHtmlDir メッセージ
		if(MessageBox(strTmp, strTmp2, MB_ICONQUESTION|MB_YESNO) == IDNO)
			return;

	}
	else
	{
		strTmp.Format(STR_MESDLG_SAMEPICDIR, strFnamePicsRelativePath);	// 同名のファイル \" %s \" があるため、フォルダを作成できません。\r\n処理を中止します
		MessageBox(strTmp);
		return ;
	}


	////////////////////////
	// サムネイル用ファイルをPICSフォルダにコピー
	////////////////////////
	// strtok (トークン切り出し) 専用の文字列宣言
	char str_ext_raw[MAX_EXT];	// 切り出し元の文字列。切り出しにしたがって破壊される
	char str_delimit[] = ";";	// 区切り文字
	char *str_strtok;			// 切り出された結果文字列
	CString StrInName, StrOutName;

//	strncpy(str_ext_raw, strFnameImgSearchExt, MAX_EXT-1);	// 破壊に備えてコピー

	for(i=0; i<nFolderCount; i++)
	{
		// サムネイルファイル名の取得
		strncpy(str_ext_raw, strFnameImgSearchExt, MAX_EXT-1);	// 破壊に備えてコピー（アドレスは変更されないが、文字列は切り捨てられる）
		str_strtok = strtok(str_ext_raw, str_delimit);	// 最初の拡張子を切り出す
		strTmp2 = "";	// 最も小さいファイル名を格納する
		while(str_strtok != NULL)	// 拡張子の数だけ実行する
		{
			// 「X:\\Folder .. \\Rel Folder\\*.EXT」 形式の検索フルパスの作成
			strTmp.Format("%s%s\\%s", strFnameBaseFolder, fstrPathArray[i].str, str_strtok);
			strTmp = GetThumbFname((LPCSTR)strTmp);	// フォルダ内の「最小の｣名前のファイル名を得る（パスなし）「XXXXX.EXT」 形式

			if(strTmp != "")
			{
				// 拡張子別の最小のファイル名を strTmp2 に格納する（パスなし）「XXXXX.EXT」 形式
				if(strTmp2 == "") strTmp2 = strTmp;		// 最初の検索時
				if(strcmpi(strTmp, strTmp2) < 0) strTmp2 = strTmp;
			}

			str_strtok = strtok(NULL, str_delimit);	// 次の拡張子を切り出す
		}
		
		if(strTmp2 == "") continue;

		// コピー元ファイルのフルパスの作成
		StrInName.Format("%s%s\\%s", strFnameBaseFolder, fstrPathArray[i].str, strTmp2);
		// コピー先ファイルのフルパスの作成
		j = strTmp2.ReverseFind('.');	// 最後の '.' の位置
		if(j>=0) strTmp = strTmp2.Right(strTmp2.GetLength()-j);	// 拡張子のみを抽出
		else strTmp = "";	// 拡張子無し
		StrOutName.Format("%s%s\\%s%s", strFnameBaseFolder, strFnamePicsRelativePath, fstrPathArray[i].str, strTmp);
		// ファイルのコピー
		CopyFile((LPCSTR)StrInName, (LPCSTR)StrOutName);
	}

	////////////////////////
	// 既存フォルダに追加モード、既存フォルダのフォルダ名を配列に追加格納
	////////////////////////
	if(IsDlgButtonChecked(IDC_CHECK_ADDPATH))
	{
	    if( (hFind = _findfirst((LPCSTR)strFnameAddSearch, &finddata )) == -1L )
		{	// 参照下ファイルが見つからない場合
		}
		else{
			if(finddata.attrib & _A_SUBDIR)
			{	// 一つ目のフォルダの発見
				if(finddata.name[0] != '.' && stricmp(finddata.name, strFnamePicsRelativePath) != 0)
				{	// "." , ".." や PICS フォルダの除外
					strcpy(fstrPathArray[nFolderCount].str, finddata.name);
					nFolderCount++;
				}
			}
			while(_findnext( hFind, &finddata ) == 0)
			{	// 続くフォルダを連続発見
				if(finddata.attrib & _A_SUBDIR)
				{
					if(finddata.name[0] != '.' && stricmp(finddata.name, strFnamePicsRelativePath) != 0)
					{	// "." , ".." や PICS フォルダの除外
						strcpy(fstrPathArray[nFolderCount].str, finddata.name);
						nFolderCount++;
						if(nFolderCount >= MAX_PATH_ARRAY) break;
					}
				}
			}
		}
		if(nFolderCount >= MAX_PATH_ARRAY)
		{
			strTmp.LoadString(STR_MESDLG_EXCEEDLIM);	// フォルダ数が処理限界を超えました\r\n途中まで処理します
			MessageBox(strTmp);
		}

		////////////////////////
		// フォルダ配列の再度ソート
		////////////////////////
		SortArray(fstrPathArray, nFolderCount);

		////////////////////////
		// 既存フォルダに追加モード時の、フォルダ重複確認
		////////////////////////
		j = 0;
		strTmp = "";
		for(i=0; i<nFolderCount-1; i++)
		{
			if(!_stricmp(fstrPathArray[i].str,fstrPathArray[i+1].str))
			{
				strTmp = strTmp + fstrPathArray[i].str + " ,   ";
				j++;
			}
		}
		if(j)
		{
			strTmp2.Format(STR_MESDLG_SAMEDIR, strTmp);	// 重複フォルダが存在します\n処理を続行しますか？\n\n（重複フォルダは次のとおりです）\n\n %s
			strTmp.LoadString(STR_MESDLG_TITLE);	// ThumbHtmlDir メッセージ
			if(MessageBox(strTmp2, strTmp, MB_ICONQUESTION|MB_YESNO) == IDNO)
				return;
		}
	}


	////////////////////////
	// HTML ファイルの作成
	////////////////////////

	// HTML ファイルの存在をチェックする
	if(GetCheckedRadioButton(IDC_RADIO_FORCEWR, IDC_RADIO_FORCEWR2) == IDC_RADIO_FORCEWR2)
	{
		_fattr = ::GetFileAttributes(strFnameHtmlPath);
		if(_fattr != -1)
		{
			strTmp.Format(STR_MESDLG_HTMLOVERWRITE, strFnameHtmlPath);	// HTMLファイル  %s がすでに存在します。上書きしますか？
			strTmp2.LoadString(STR_MESDLG_TITLE);	// ThumbHtmlImg メッセージ
			if(MessageBox(strTmp, strTmp2, MB_ICONQUESTION|MB_YESNO) != IDYES)
				return;		// 上書きせずに終了する
		}
	}
	// HTML ファイルを開く（新規作成する）
	if(!fileOutHtml.Open(strFnameHtmlPath,CFile::modeCreate|CFile::modeWrite))
	{
		strTmp.LoadString(IDS_STR_ERRTITLE);
		strTmp2.LoadString(STR_ERR_OPEN_HTML);		// HTML ファイルが開けません
		MessageBox(strTmp2, strTmp, MB_OK|MB_ICONSTOP);
		return ;
	}


	// ***********
	// HTML に書き込む
	// ***********
	int nCounter;	// 画像ファイル数カウンタ
	// ヘッダ情報を書き込む
	strTmp = "<html>\r\n<body>\r\n";
	fileOutHtml.Write((LPCSTR)strTmp, strlen(strTmp));
	// 情報を書き込む
	strTmp.Format("<p>ThumbHtmlDir Ver 1.5<br>%d folders</p>\r\n", nFolderCount);
	fileOutHtml.Write((LPCSTR)strTmp, strlen(strTmp));

	// テーブル開始
	strTmp = "<table border=\"1\">\r\n";
	fileOutHtml.Write((LPCSTR)strTmp, strlen(strTmp));


	// 子フォルダ情報を書き込む
	nCounter = 0;	// フォルダカウンタ
	for(i=0; i<nFolderCount/nHtmlColumn+1; i++)
	{
		strTmp = "<tr>\r\n";
		fileOutHtml.Write((LPCSTR)strTmp, strlen(strTmp));
		for(j=0; j<nHtmlColumn; j++)
		{	// 列の定義
			if(nCounter < nFolderCount)
			{
				strTmp.Format("\t<td><a href=\"%s%s\" %s><img src=\"%s/%s%s\" %s><br>%s</a></td>\r\n",
					fstrPathArray[nCounter].str, strALink, strTagLinkOpt, strFnamePicsRelativePath, fstrPathArray[nCounter].str, strFnamePicsExt, strTagImgOpt, fstrPathArray[nCounter].str);
				fileOutHtml.Write((LPCSTR)strTmp, strlen(strTmp));
				nCounter++;
			}
			else
			{
				strTmp = "\t<td></td>\r\n";
				fileOutHtml.Write((LPCSTR)strTmp, strlen(strTmp));
			}
		}
		strTmp = "</tr>\r\n";
		fileOutHtml.Write((LPCSTR)strTmp, strlen(strTmp));
	}

	// フッタ情報を書き込む
	strTmp = "</table>\r\n</body>\r\n</html>\r\n";
	fileOutHtml.Write((LPCSTR)strTmp, strlen(strTmp));

	// HTML ファイルを閉じる
	fileOutHtml.Close();

	// 終了メッセージの表示
	strTmp.Format(STR_MES_SUCCESS, strFnameHtmlPath);	// HTMLファイル %s の作成が正常に終了しました
	strTmp2.LoadString(STR_MESDLG_TITLE);	// ThumbHtmlDir メッセージ
	MessageBox(strTmp, strTmp2);

}

// **********************************
// ソーティング
// **********************************
void CThumbHtmlDirDlg::SortArray(_fstr *fstrPathArray, int nFolderCount)
{
	int i, j;
	CString strTmp;

	for(i=0; i<nFolderCount; i++)
	{
		for(j=i; j<nFolderCount; j++)
		{
			if(stricmp((const char*)fstrPathArray[i].str, (const char*)fstrPathArray[j].str) > 0)
			{
				strTmp = fstrPathArray[i].str;
				strcpy(fstrPathArray[i].str, fstrPathArray[j].str);
				strcpy(fstrPathArray[j].str, strTmp);
			}
		}
	}
}

// **********************************
// ファイルコピー
// **********************************
BOOL CThumbHtmlDirDlg::CopyFile(LPCSTR FpathIn, LPCSTR FpathOut)
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

// **********************************
// フォルダ内の「もっとも小さい｣名前のファイル名を返す（パス名は付けないで返す）
// **********************************
LPSTR CThumbHtmlDirDlg::GetThumbFname(LPCSTR Fpath)
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

// **********************************
// 「既存フォルダ追加モード｣のチェックが入れられたら、インプットボックスを有効にする。
// **********************************
void CThumbHtmlDirDlg::OnCheckAddpath() 
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

// **********************************
// バージョン表示ボタン クリック
// **********************************
void CThumbHtmlDirDlg::OnBtnAbout() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	CAboutDlg dlg;

	dlg.DoModal();
}

// **********************************
// ヘルプボタン クリック
// **********************************
void CThumbHtmlDirDlg::OnBtnHelp() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください

	// エディットボックスの文字列が選択状態になるのを防ぐには
	// タブオーダーでエディットボックスの前に閉じるボタン（デフォルトボタン）を配置する。

	CHelpDocViewer dlg;
	CString strTmp;

	strTmp.LoadString(STR_SW_LANG);
	if(strTmp == "JPN")
	{	// 日本語リソースのとき

		dlg.m_edit = "\r\n"
		"ThumbHtmlDir - サムネイル HTML 作成 （フォルダ整理）\r\n"
		" Version 1.5   (Free Software)\r\n"
		"\r\n\r\n"
		" (C) 2000-2003 INOUE. Hirokazu 井上 博計\r\n"
		"\r\n\r\n\r\n"
		"【はじめに】\r\n"
		"フォルダにまとめられた画像ファイルのサムネイルHTMLを作成できます\r\n"
		"撮影した写真の整理に、HTML編集ソフトで編集する下準備として利用できます\r\n"
		"\r\n"
		"ThumbHtmlDir は 複数のフォルダの（フォルダ選択のための）サムネイルを作ります\r\n"
		"ThumbHtmlImg は 一つのフォルダ内の画像ファイルのサムネイルを作ります\r\n"
		"\r\n\r\n"
		"【動作環境】\r\n"
		"Windows 95/98/Me/NT/2000/XP (日本語版および各国語版)\r\n"
		"MFC42.DLL および MSVCRT.DLL がシステムフォルダに存在すること（Visual C++ 6.0 対応版） 古いバージョンのDLLの場合、起動できません\r\n"
		"\r\n"
		"このプログラムには、日本語と英語の２種類の設定が含まれています。日本語以外のWindowsで起動した場合、自動的に英語モードで動作します\r\n"
		"\r\n\r\n"
		"【インストール】\r\n"
		"この画面を見ているということは、すでにインストール完了です\r\n"
		"\r\n\r\n"
		"【アンインストール】\r\n"
		"このプログラムは、レジストリおよび設定ファイルなどを利用しません\r\n"
		"プログラム本体を消去すればそれで終わりです\r\n"
		"\r\n\r\n"
		"【利用法】\r\n"
		"「基準フォルダ」\r\n"
		"   このフォルダに HTML ファイルを作成します。画像はこのフォルダの下の\r\n"
		"   サブフォルダに格納されている必要があります\r\n"
		"   末尾に「￥」記号がつく形で指定します\r\n"
		"   (例) C:\\My Documents\\\r\n"
		"\r\n"
		"「画像フォルダ」\r\n"
		"   基準フォルダ以下の全てのフォルダを対象とします。設定変更不可\r\n"
		"\r\n"
		"「画像フォルダの対象拡張子」\r\n"
		"   対象ファイルの検索パターンを入力します\r\n"
		"   複数のパターンを検索する場合は、セミコロンで区切ります\r\n"
		"   (例) *.jpg;*.gif\r\n"
		"\r\n"
		"「サムネイル画像フォルダ」\r\n"
		"   基準フォルダの下にあるサムネイル画像（縮小）ファイルのあるフォルダ\r\n"
		"   相対パスで指定します\r\n"
		"   画像フォルダの中の一番『若い名前』のファイルのコピー先となります\r\n"
		"   ※ 本プログラムではこのフォルダは検査しません\r\n"
		"      ブラウザで表示するまでに、縮小画像を用意してください\r\n"
		"\r\n"
		"「サムネイル画像フォルダの対象拡張子」\r\n"
		"   対象ファイルの拡張子を入力します\r\n"
		"   複数のパターンや、検索フィルタは設定できません\r\n"
		"   本プログラム実行後、サムネイルフォルダの画像を縮小し、この拡張子に\r\n"
		"   統一してください\r\n"
		"   (例) .jpg\r\n"
		"\r\n"
		"「HTML ファイル名」\r\n"
		"   出力する ＨＴＭＬ ファイル名\r\n"
		"   (例) index.html\r\n"
		"\r\n"
		"「HTML 表の列数」\r\n"
		"   横何列に画像を並べるか指定します （1 ～ 10 列）\r\n"
		"\r\n"
		"「IMG タグのオプション」\r\n"
		"   <img src=\"...\" XXXXXX> の 「XXXXX」の指定が出来ます\r\n"
		"   (例) border=0\r\n"
		"\r\n"
		"「A LINK タグのオプション」\r\n"
		"   <a href=\"...\" XXXXXX> の 「XXXXX」の指定が出来ます\r\n"
		"   (例) target=_new\r\n"
		"\r\n"
		"「リンク先のファイル名」\r\n"
		"   <a href=\"..../index.html\"> のように\r\n"
		"   フォルダ内のファイルをリンク先を指定できます\r\n"
		"   (例) index.html\r\n"
		"\r\n"
		"「上書きモード」\r\n"
		"   既存のファイルに上書きする場合に警告するかどうかを設定します\r\n"
		"\r\n"
		"「既存フォルダ追加モード」\r\n"
		"   CD-ROMなどに退避した画像集に追加書き込みする場合に指定します\r\n"
		"   (例)\r\n"
		"      既存フォルダ以下に - [Folder 1], [Folder 2], [Folder 3]\r\n"
		"      親フォルダ以下に - [Folder 3], [Folder 4], [Folder 5]\r\n"
		"      となっていた場合、[Folder 3] は重複していますのでエラーとなります\r\n"
		"\r\n"
		"「About」\r\n"
		"   バージョンダイアログを表示します\r\n"
		"\r\n"
		"「実行」\r\n"
		"   HTML を作成します\r\n"
		"\r\n\r\n"
		"【具体例】\r\n"
		"    [C:]\r\n"
		"      |\r\n"
		"      +--[Folder1]\r\n"
		"      |\r\n"
		"      +--[Folder2]\r\n"
		"      |         |\r\n"
		"      |         +--[Folder-X1]\r\n"
		"      |         |       +--DSCF001.JPG  : 1600x1200のデジカメ画像\r\n"
		"      |         |       +--DSCF002.JPG  : 1600x1200のデジカメ画像\r\n"
		"      |         |\r\n"
		"      |         +--[Folder-X2]\r\n"
		"      |         |       +--DSCF001.JPG  : 1600x1200のデジカメ画像\r\n"
		"      |         |       +--DSCF002.JPG  : 1600x1200のデジカメ画像\r\n"
		"      |         |\r\n"
		"      |         +--[Folder-X3]\r\n"
		"      |         |       +--DSCF001.JPG  : 1600x1200のデジカメ画像\r\n"
		"      |         |       +--DSCF002.JPG  : 1600x1200のデジカメ画像\r\n"
		"      |         |\r\n"
		"      |         +--[pics]\r\n"
		"      |         |       +--Folder-X1.JPG  : ※160x120のサムネイル画像\r\n"
		"      |         |       +--Folder-X2.JPG  : ※160x120のサムネイル画像\r\n"
		"      |         |       +--Folder-X3.JPG  : ※160x120のサムネイル画像\r\n"
		"      |         +--index.html             : ※HTMLファイル\r\n"
		"\r\n"
		"  基準フォルダ           ： C:\\Folder2\\\r\n"
		"  画像フォルダ           ： *\r\n"
		"  (対象拡張子）          ： *.jpg\r\n"
		"  サムネイル画像フォルダ ： pics\r\n"
		"  (対象拡張子）          ： .jpg\r\n"
		"  HTML ファイル名        ： index.html\r\n"
		"\r\n"
		"  ※のファイルは、各フォルダの中の先頭のファイルがコピーされる\r\n"
		"\r\n"
		"  作成される HTML ファイルの例\r\n"
		"\r\n"
		"        +-----------+-----------+-----------+\r\n"
		"        |           |           |           |\r\n"
		"        | 画像      | 画像      | 画像      |\r\n"
		"        | Folder-X1 | Folder-X2 | Folder-X3 |\r\n"
		"        +-----------+-----------+-----------+\r\n"
		"        |           |           |           |\r\n"
		"\r\n"
		"  ※ 基準フォルダは絶対パス、画像・サムネイルフォルダは相対パス\r\n"
		"\r\n\r\n"
		"【あとがき・編集履歴】\r\n"
		"Version 1.0  2001/09/15\r\n"
		"  当初バージョン\r\n"
		"Version 1.5  2003/05/11\r\n"
		"  標準設定のドロップダウンリスト\r\n"
		"  フォルダ選択の記憶化\r\n"
		"  ヘルプファイル\r\n"
		"\r\n\r\n"
		"【使用許諾条件】\r\n"
		"プログラム名 ： ThumbHtmlDir サムネイルHTML作成\r\n"
		"バージョン   ： 1.5\r\n"
		"著作権保有者 ： (C)2000-2003 INOUE. Hirokazu (井上 博計)\r\n"
		"\r\n"
		"このプログラムとドキュメント（以下、プログラムと記述）はフリーソフトウエアです。\r\n"
		"このプログラムおよび実行時の画面の著作権はすべて井上 博計が有します。\r\n"
		"\r\n"
		"著作者はこのプログラム及びドキュメントに関するいかなる保証も行いません。\r\n"
		"本製品の動作（正常動作、異常動作）によって被るすべての結果についての全責任は、本ソフトの使用者にあります。\r\n"
		"\r\n"
		"このプログラムの再配布を、無償・無制限に行う限り著作者からの許諾は必要ありません。\r\n"
		"このプログラムを有償で再配布、または販売する場合は著作者の許可が必要です。\r\n"
		"再配布時に本プログラムに付属したドキュメント以外の情報（著作者の個人情報等）を付加することを厳禁します。\r\n"
		"このプログラムを無断で有料再配布した場合は、再配布者および幇助者は著作者に対して連帯して賠償する義務を負います。\r\n"
		"\r\n"
		"このプログラムのサポートは著作者宛電子メールを使用して行いますが、著作者がサポートを行うのはボランティアであって義務・責務ではありません。\r\n"
		"\r\n"
		"※ フリーメールから発信されたメールは、無視します。 ＨＴＭＬメールや添付ファイルも無視し、該当ユーザからのメールは２度と受信ません。ご注意ください。\r\n"
		"\r\n"
		"\r\n"
		"【ユーザー・サポート連絡先】\r\n"
		"電子メール\r\n"
		"  下記のホームページでメールアドレスを公開しています。\r\n"
		"  （ウイルスメール対策のための自衛手段にご協力願います）\r\n"
		"ホームページ\r\n"
		"  http://inoue-h.connect.to/\r\n"
		"  http://www.vector.co.jp/authors/VA001911/index.html\r\n"
		"  http://www.ne.jp/asahi/oasis/inoue-h/index.html\r\n"
		"\r\n"
		"上記のアドレスが消滅している場合、検索エンジンで「井上博計」で検索してください。\r\n"
		"My Journey - 旅のページ が私のページです\r\n";

	}
	else
	{
		dlg.m_edit = "\r\n"
		"ThumbHtmlImg - Thumbnail HTML maker (Folder List)\r\n"
		" Version 1.5   (Free Software)\r\n"
		"\r\n\r\n"
		" (C) 2000-2003 INOUE. Hirokazu\r\n"
		"\r\n\r\n\r\n"
		"[Introduction]\r\n"
		"This program can make index(list) HTML file of folders containing images.\r\n"
		"This program is designed for indexing photographs, and prepare source for HTML editor\r\n"
		"\r\n\r\n"
		"[Target System]\r\n"
		"Windows 95/98/Me/NT/2000/XP (Japanese and Other Language)\r\n"
		"This program requires MFC42.DLL and MSVCRT.DLL (designed for Visual C++ 6.0) .\r\n"
		"\r\n"
		"This program contains Japanese and English text resource, and Resource is automatically selected according to your OS .\r\n"
		"\r\n\r\n"
		"[Install]\r\n"
		"No need to install .\r\n"
		"\r\n\r\n"
		"[Uninstall]\r\n"
		"Only delete program file .\r\n"
		"This program do not use system registry\r\n"
		"\r\n\r\n"
		"[How to Use]\r\n"
		" Base Folder :\r\n"
		"   Designate folder with browse button , or input path string end with '\\' .\r\n"
		"\r\n"
		" Image Folder , Thumbnail Folder\r\n"
		"   Designate relative path from Base Folder .\r\n"
		"\r\n"
		" Scan Pat\r\n"
		"   Designate image file match pattern . like '*.jpg'\r\n"
		"\r\n\r\n"
		"[Licence agreement]\r\n"
		"This program is free software. And all copyrights is reserved by INOUE. Hirokazu (author).\r\n"
		"\r\n"
		"There is no guarantee of this program functioning correctly, and author is not responsible to every result of this program. Other condition follows on this document in japanese resource (Japanese Language only, sorry).\r\n"
		"\r\n"
		"[User Support]\r\n"
		"E-Mail\r\n"
		"  Please find mail address from following HOMEPAGE !\r\n"
		"Homepage\r\n"
		"  http://inoue-h.connect.to/\r\n"
		"  http://www.vector.co.jp/authors/VA001911/index.html\r\n"
		"  http://www.ne.jp/asahi/oasis/inoue-h/index.html\r\n";
	}

	dlg.DoModal();

}

// *****************************************
// F1 キーを押したときの処理
// *****************************************
BOOL CThumbHtmlDirDlg::OnHelpInfo(HELPINFO* pHelpInfo) 
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください
	OnBtnHelp();	
	return CDialog::OnHelpInfo(pHelpInfo);
}
