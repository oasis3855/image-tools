// ThumbHtmlImgDlg.cpp : インプリメンテーション ファイル
// (C) 2000-2003 INOUE. Hirokazu

#include "stdafx.h"
#include "ThumbHtmlImg.h"
#include "ThumbHtmlImgDlg.h"
#include "HelpDocViewer.h"
#include <io.h>			// findfirst
#include <string.h>		// strtok


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// **********************************
// グローバル変数・定数
// **********************************
#define	MAX_PATH_ARRAY	1000		// 子フォルダの最大個数
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
// CThumbHtmlImgDlg ダイアログ

CThumbHtmlImgDlg::CThumbHtmlImgDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CThumbHtmlImgDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CThumbHtmlImgDlg)
		// メモ: この位置に ClassWizard によってメンバの初期化が追加されます。
	//}}AFX_DATA_INIT
	// メモ: LoadIcon は Win32 の DestroyIcon のサブシーケンスを要求しません。
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CThumbHtmlImgDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CThumbHtmlImgDlg)
	DDX_Control(pDX, IDC_CMB_SORT, m_cmb_sort_ctrl);
	DDX_Control(pDX, IDC_SPIN_COL, m_spin_col_ctrl);
	DDX_Control(pDX, IDC_CMB_PICSEXT, m_cmb_picsext_ctrl);
	DDX_Control(pDX, IDC_CMB_PHOTOEXT, m_cmb_photoext_ctrl);
	DDX_Control(pDX, IDC_CMB_IMGOPT, m_cmb_imgopt_ctrl);
	DDX_Control(pDX, IDC_CMB_STYLE, m_cmb_style_ctrl);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CThumbHtmlImgDlg, CDialog)
	//{{AFX_MSG_MAP(CThumbHtmlImgDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_BROWSE, OnBtnBrowse)
	ON_BN_CLICKED(IDC_BTN_EXEC, OnBtnExec)
	ON_BN_CLICKED(IDC_BTN_ABOUT, OnBtnAbout)
	ON_BN_CLICKED(IDC_BTN_PHOTO_BROWSE, OnBtnPhotoBrowse)
	ON_BN_CLICKED(IDC_BTN_PICS_BROWSE, OnBtnPicsBrowse)
	ON_BN_CLICKED(IDC_BTN_HELP, OnBtnHelp)
	ON_WM_HELPINFO()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CThumbHtmlImgDlg メッセージ ハンドラ

BOOL CThumbHtmlImgDlg::OnInitDialog()
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
	this->SetDlgItemText(IDC_EDIT_PHOTO,"photo");
	this->SetDlgItemText(IDC_EDIT_PICS,"pics");
	m_cmb_photoext_ctrl.SetCurSel(1);	// 「*.jp*」
	m_cmb_picsext_ctrl.SetCurSel(0);	// 「*.jpg」
	this->SetDlgItemInt(IDC_EDIT_COL, 2);
	m_spin_col_ctrl.SetRange(1,10);		// カラム数スピンコントロールの範囲指定
	this->SetDlgItemText(IDC_EDIT_HTML, "index.html");
	m_cmb_imgopt_ctrl.SetCurSel(0);		// 「border=0」を選択
	m_cmb_style_ctrl.SetCurSel(3);		// 「ファイル名（画像の左） 日時（画像の右）」を選択
	m_cmb_sort_ctrl.SetCurSel(0);		// 「日時でソート（昇順）」
	this->CheckRadioButton(IDC_RADIO_FORCEWR, IDC_RADIO_FORCEWR2, IDC_RADIO_FORCEWR2);
	
	return TRUE;  // TRUE を返すとコントロールに設定したフォーカスは失われません。
}

void CThumbHtmlImgDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CThumbHtmlImgDlg::OnPaint() 
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
HCURSOR CThumbHtmlImgDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

// **********************************
// 親フォルダの参照
// **********************************
void CThumbHtmlImgDlg::OnBtnBrowse() 
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
	if(pidl == NULL) return;			// キャンセルボタンが押された
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
// PHOTOフォルダの参照
// **********************************
void CThumbHtmlImgDlg::OnBtnPhotoBrowse() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	BrowseSubPath(IDC_EDIT_PHOTO);
}

// **********************************
// PICSフォルダの参照
// **********************************
void CThumbHtmlImgDlg::OnBtnPicsBrowse() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	BrowseSubPath(IDC_EDIT_PICS);	
}


// **********************************
// PHOTO,PICSフォルダの参照のダイアログ選択処理
// **********************************
void CThumbHtmlImgDlg::BrowseSubPath(int nID)
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
void CThumbHtmlImgDlg::OnBtnExec() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	CFile fileOutHtml;						// ファイルハンドラ （HTML出力）
	_fstr fstrPathArray[MAX_PATH_ARRAY];	// ファイル名をソートして格納するための配列
	int nImagesCount;						// 対象のイメージファイル数 1～
	CString strFnameBaseFolder;				// 親パス（フルパス）
	CString strFnameImgSearch;				// photo フルパス（findfirst用ワイルドカードつき）
	CString strFnameImgBody;				// ファイルのボディー名（拡張子抜き）
	CString strFnameHtmlPath;				// HTMLフルパス
	CString strFnamePicsRelativePath;		// pics 相対パス (サムネイル用画像 IMG SRC用)
	CString strFnameImgRelativePath;		// photo 相対パス (実体画像 A LINK用)
	CString strFnamePicsExt;				// pics へ格納する画像拡張子
	CString strFnameImgSearchExt;			// photo の対象拡張子（複数指定はセミコロン区切り）
	CString strTagImgOpt;					// IMG タグオプション文字列
	CString strTagLinkOpt;					// A LINK タグオプション文字列
	struct tm *timeData;					// time_t を分解して日時データに切り分ける
	int nHtmlColumn;						// HTML カラム数
	CString strTmp, strTmp2;
	DWORD _fattr;							// ファイル・フォルダの存在をチェックする時の変数
	int i, j;								// 一時的カウンター用（汎用）


	////////////////////////
	// 各変数に初期値を代入  (これらの値は実行途中に変化することは無い)
	////////////////////////
	GetDlgItemText(IDC_EDIT_PASS, strFnameBaseFolder);			// 親パスを得る
	GetDlgItemText(IDC_EDIT_HTML, strFnameHtmlPath);			// HTML名を得る
	GetDlgItemText(IDC_EDIT_PHOTO, strFnameImgRelativePath);	// PHOTOサブパスを得る
	GetDlgItemText(IDC_EDIT_PICS, strFnamePicsRelativePath);	// PICSサブパスを得る
	GetDlgItemText(IDC_CMB_PHOTOEXT, strFnameImgSearchExt);		// PHOTO 対象拡張子
	GetDlgItemText(IDC_CMB_PICSEXT, strFnamePicsExt);			// PICS 対象拡張子
	GetDlgItemText(IDC_CMB_IMGOPT, strTagImgOpt);				// IMG タグオプション
	GetDlgItemText(IDC_CMB_LINKOPT, strTagLinkOpt);				// A LINK タグオプション
	nHtmlColumn = GetDlgItemInt(IDC_EDIT_COL);

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
	if(strFnameImgRelativePath == "")
	{
		strTmp2.LoadString(STR_MESDLG_PHOTODIR);	// 画像フォルダが設定されていません
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

	// 親パスのチェック （最後に \ 記号がついていない場合は付加する）
	if(strFnameBaseFolder[strFnameBaseFolder.GetLength()-1] != '\\')
		strFnameBaseFolder = strFnameBaseFolder + "\\";

	// HTML フルパスの作成
	strFnameHtmlPath = strFnameBaseFolder + strFnameHtmlPath;

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

	////////////////////////
	// PHOTO フォルダからファイル名一覧を読み込んで、ソートする
	////////////////////////

	// strtok (トークン切り出し) 専用の文字列宣言
	char str_ext_raw[MAX_EXT];	// 切り出し元の文字列。切り出しにしたがって破壊される
	char str_delimit[] = ";";	// 区切り文字
	char *str_strtok;			// 切り出された結果文字列
	strncpy(str_ext_raw, strFnameImgSearchExt, MAX_EXT-1);	// 破壊に備えてコピー

	// 子フォルダを検索して、フォルダ名を配列に格納する
	struct _finddata_t finddata;
	long hFind;
	nImagesCount = 0;

	str_strtok = strtok(str_ext_raw, str_delimit);	// 最初の拡張子を切り出す
	while(str_strtok != NULL)	// 拡張子の数だけ実行する
	{
		// 検索パスの組み立て
		strFnameImgSearch = strFnameBaseFolder + strFnameImgRelativePath + "\\" + str_strtok;

		if( (hFind = _findfirst((LPCSTR)strFnameImgSearch, &finddata )) == -1L )
		{	// 参照下ファイルが見つからない場合
		}
		else{
			if(!(finddata.attrib & _A_SUBDIR))
			{	// 一つ目のファイルの発見
				strcpy(fstrPathArray[nImagesCount].str, finddata.name);
				fstrPathArray[nImagesCount].tm_write = finddata.time_write;
				nImagesCount++;
			}
			while(_findnext( hFind, &finddata ) == 0)
			{	// 続くファイルを連続発見
				if(!(finddata.attrib & _A_SUBDIR))
				{
					strcpy(fstrPathArray[nImagesCount].str, finddata.name);
					fstrPathArray[nImagesCount].tm_write = finddata.time_write;
					nImagesCount++;
					if(nImagesCount >= MAX_PATH_ARRAY) break;
				}
			}
		}
		if(nImagesCount >= MAX_PATH_ARRAY) break;
		str_strtok = strtok(NULL, str_delimit);	// 次の拡張子を切り出す
	}

	if(nImagesCount >= MAX_PATH_ARRAY)
	{
		strTmp.LoadString(STR_MESDLG_EXCEEDLIM);	// 画像ファイル数が処理限界を超えました\r\n途中まで処理します
		MessageBox(strTmp);
	}

	if(!nImagesCount)
	{	// PHOTO フォルダ内に対象ファイルが存在しないとき
		strTmp.LoadString(STR_MESDLG_NOIMAGE);	// 画像ファイルが存在しません。処理を中止します
		MessageBox(strTmp);
		return ;
	}

	// ファイル名のソート
	SortArray(fstrPathArray, nImagesCount, m_cmb_sort_ctrl.GetCurSel());


	////////////////////////
	// HTML を新規作成する
	////////////////////////

	// HTML ファイルの存在をチェックする
	if(GetCheckedRadioButton(IDC_RADIO_FORCEWR, IDC_RADIO_FORCEWR2) == IDC_RADIO_FORCEWR2)
	{
		_fattr = ::GetFileAttributes(strFnameHtmlPath);
		if(_fattr != -1)
		{
			strTmp.Format(STR_MESDLG_OVERWRITE, strFnameHtmlPath);	// HTMLファイル  %s がすでに存在します。上書きしますか？
			strTmp2.LoadString(STR_MESDLG_TITLE);	// ThumbHtmlImg メッセージ
			if(MessageBox(strTmp, strTmp2, MB_ICONQUESTION|MB_YESNO) != IDYES)
				return;		// 上書きせずに終了する
		}
	}
	// HTML ファイルを開く
	if(!fileOutHtml.Open(strFnameHtmlPath,CFile::modeCreate|CFile::modeWrite))
	{
		strTmp.LoadString(IDS_STR_ERRTITLE);
		strTmp2.LoadString(STR_ERR_OPEN_HTML);		// HTML ファイルが開けません
		MessageBox(strTmp2, strTmp, MB_OK|MB_ICONSTOP);
		return ;
	}

	////////////////////////
	// HTML ファイルに出力する
	////////////////////////

	int nCounter;	// 画像ファイル数カウンタ
	// ヘッダ情報を書き込む
	strTmp = "<html>\r\n<body>\r\n";
	fileOutHtml.Write((LPCSTR)strTmp, strlen(strTmp));
	// 情報を書き込む
	strTmp.Format("<p>ThumbHtmlImg Ver 1.1<br>%d images</p>\r\n", nImagesCount);
	fileOutHtml.Write((LPCSTR)strTmp, strlen(strTmp));

	// テーブル開始
	strTmp = "<table border=\"1\">\r\n";
	fileOutHtml.Write((LPCSTR)strTmp, strlen(strTmp));

	// 各サムネイルとリンクを表形式で書き込む
	nCounter = 0;	// 画像ファイル数カウンタ
	while(1)
	{
		if(nCounter >= nImagesCount) break;	// ファイル数処理したら、ループ終了

		strTmp = "<tr>\r\n";
		fileOutHtml.Write((LPCSTR)strTmp, strlen(strTmp));
		for(j=0; j<nHtmlColumn; j++)
		{	// 列の定義
			if(nCounter < nImagesCount)
			{
				// PHOTO ファイル名から拡張子を抜き、PICS拡張子を付ける処理
				for(i=strlen(fstrPathArray[nCounter].str)-1; i>0; i--)
				{	// １文字目までスキャンして… と細かいことは無視する
					if(fstrPathArray[nCounter].str[i] == '.') break;
				}
				strTmp2 = fstrPathArray[nCounter].str;
				strFnameImgBody = strTmp2.Left(i);
				strTmp2 = strFnameImgBody + strFnamePicsExt;

				// HTML記述の作成と書き込み
				switch(m_cmb_style_ctrl.GetCurSel())
				{
				case 0 : // 「無し」
					// 画像
					strTmp.Format("\t<td><a href=\"%s/%s\" %s><img src=\"%s/%s\" %s></a></td>\r\n",
						strFnameImgRelativePath, fstrPathArray[nCounter].str, strTagLinkOpt, strFnamePicsRelativePath, strTmp2, strTagImgOpt);
					fileOutHtml.Write((LPCSTR)strTmp, strlen(strTmp));
					break;
				case 1 : // 「ファイル名（画像の下）」
					// 画像 & ファイル名（下）
					strTmp.Format("\t<td><p><a href=\"%s/%s\" %s><img src=\"%s/%s\" %s></a></p><p>%s</p></td>\r\n",
						strFnameImgRelativePath, fstrPathArray[nCounter].str, strTagLinkOpt, strFnamePicsRelativePath, strTmp2, strTagImgOpt, strFnameImgBody);
					fileOutHtml.Write((LPCSTR)strTmp, strlen(strTmp));
					break;
				case 2 : // 「ファイル名（画像の下） 日時（画像の下）」
					// 画像 & ファイル名（下）& 日時（下）
					timeData = localtime(&fstrPathArray[nCounter].tm_write);
					strTmp.Format("\t<td><p><a href=\"%s/%s\" %s><img src=\"%s/%s\" %s></a></p><p>%s<br>%04d/%02d/%02d<br>%02d:%02d</p></td>\r\n",
						strFnameImgRelativePath, fstrPathArray[nCounter].str, strTagLinkOpt, strFnamePicsRelativePath, strTmp2, strTagImgOpt, strFnameImgBody,
						timeData->tm_year+1900, timeData->tm_mon+1, timeData->tm_mday, timeData->tm_hour, timeData->tm_min);
					fileOutHtml.Write((LPCSTR)strTmp, strlen(strTmp));
					break;
				case 3 : // 「ファイル名（画像の左） 日時（画像の右）」
					// ファイル名
					strTmp.Format("\t<td>%s</td>\r\n", strFnameImgBody);
					fileOutHtml.Write((LPCSTR)strTmp, strlen(strTmp));
					// 画像
					strTmp.Format("\t<td><a href=\"%s/%s\" %s><img src=\"%s/%s\" %s></a></td>\r\n",
						strFnameImgRelativePath, fstrPathArray[nCounter].str, strTagLinkOpt, strFnamePicsRelativePath, strTmp2, strTagImgOpt);
					fileOutHtml.Write((LPCSTR)strTmp, strlen(strTmp));
					// 日時
					timeData = localtime(&fstrPathArray[nCounter].tm_write);
					strTmp.Format("\t<td>%04d/%02d/%02d<br>%02d:%02d</td>\r\n",
						timeData->tm_year+1900, timeData->tm_mon+1, timeData->tm_mday, timeData->tm_hour, timeData->tm_min);
					fileOutHtml.Write((LPCSTR)strTmp, strlen(strTmp));
					break;
				case 4 : // 「ファイル名（画像の右） 日時（画像の右）」
					// 画像
					strTmp.Format("\t<td><a href=\"%s/%s\" %s><img src=\"%s/%s\" %s></a></td>\r\n",
						strFnameImgRelativePath, fstrPathArray[nCounter].str, strTagLinkOpt, strFnamePicsRelativePath, strTmp2, strTagImgOpt);
					fileOutHtml.Write((LPCSTR)strTmp, strlen(strTmp));
					// ファイル名
					strTmp.Format("\t<td>%s</td>\r\n", strFnameImgBody);
					fileOutHtml.Write((LPCSTR)strTmp, strlen(strTmp));
					// 日時
					timeData = localtime(&fstrPathArray[nCounter].tm_write);
					strTmp.Format("\t<td>%04d/%02d/%02d<br>%02d:%02d</td>\r\n",
						timeData->tm_year+1900, timeData->tm_mon+1, timeData->tm_mday, timeData->tm_hour, timeData->tm_min);
					fileOutHtml.Write((LPCSTR)strTmp, strlen(strTmp));
					break;
				case 5 : // 「ファイル名（画像の左） 日（画像の右） 時（画像の右）」
					// ファイル名
					strTmp.Format("\t<td>%s</td>\r\n", strFnameImgBody);
					fileOutHtml.Write((LPCSTR)strTmp, strlen(strTmp));
					// 画像
					strTmp.Format("\t<td><a href=\"%s/%s\" %s><img src=\"%s/%s\" %s></a></td>\r\n",
						strFnameImgRelativePath, fstrPathArray[nCounter].str, strTagLinkOpt, strFnamePicsRelativePath, strTmp2, strTagImgOpt);
					fileOutHtml.Write((LPCSTR)strTmp, strlen(strTmp));
					// 日
					timeData = localtime(&fstrPathArray[nCounter].tm_write);
					strTmp.Format("\t<td>%04d/%02d/%02d</td>\r\n",
						timeData->tm_year+1900, timeData->tm_mon+1, timeData->tm_mday);
					fileOutHtml.Write((LPCSTR)strTmp, strlen(strTmp));
					// 時
					timeData = localtime(&fstrPathArray[nCounter].tm_write);
					strTmp.Format("\t<td>%02d:%02d</td>\r\n",
						timeData->tm_hour, timeData->tm_min);
					fileOutHtml.Write((LPCSTR)strTmp, strlen(strTmp));
					break;
				case 6 : // 「ファイル名（画像の右） 日（画像の右）時（画像の右）」
					// 画像
					strTmp.Format("\t<td><a href=\"%s/%s\" %s><img src=\"%s/%s\" %s></a></td>\r\n",
						strFnameImgRelativePath, fstrPathArray[nCounter].str, strTagLinkOpt, strFnamePicsRelativePath, strTmp2, strTagImgOpt);
					fileOutHtml.Write((LPCSTR)strTmp, strlen(strTmp));
					// ファイル名
					strTmp.Format("\t<td>%s</td>\r\n", strFnameImgBody);
					fileOutHtml.Write((LPCSTR)strTmp, strlen(strTmp));
					// 日
					timeData = localtime(&fstrPathArray[nCounter].tm_write);
					strTmp.Format("\t<td>%04d/%02d/%02d</td>\r\n",
						timeData->tm_year+1900, timeData->tm_mon+1, timeData->tm_mday);
					fileOutHtml.Write((LPCSTR)strTmp, strlen(strTmp));
					// 時
					timeData = localtime(&fstrPathArray[nCounter].tm_write);
					strTmp.Format("\t<td>%02d:%02d</td>\r\n",
						timeData->tm_hour, timeData->tm_min);
					fileOutHtml.Write((LPCSTR)strTmp, strlen(strTmp));
					break;
				default :
					break;
				}
				// パディング（空白カラム）
				strTmp = "\t<td></td>\r\n";
				fileOutHtml.Write((LPCSTR)strTmp, strlen(strTmp));
				
				// カウントアップ
				nCounter++;
			}
			else
			{
				// データがないときは、空白セルをHTMLに書き込む
				switch(m_cmb_style_ctrl.GetCurSel())
				{
				case 0:
				case 1:
				case 2:
					strTmp = "\t<td></td>\r\n\t<td></td>\r\n";
					break;
				case 3:
				case 4:
					strTmp = "\t<td></td>\r\n\t<td></td>\r\n\t<td></td>\r\n\t<td></td>\r\n";
					break;
				case 5:
				case 6:
				default:
					strTmp = "\t<td></td>\r\n\t<td></td>\r\n\t<td></td>\r\n\t<td></td>\r\n\t<td></td>\r\n";
					break;
				}
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

	strTmp.Format(STR_MES_SUCCESS, strFnameHtmlPath);	// HTMLファイル %s の作成が正常に終了しました
	strTmp2.LoadString(STR_MESDLG_TITLE);	// ThumbHtmlImg メッセージ
	MessageBox(strTmp, strTmp2);
}

// **********************************
// ソーティング
// **********************************
void CThumbHtmlImgDlg::SortArray(_fstr *fstrPathArray, int nImagesCount, int nMode)
{
	int i, j;
	CString strTmp;
	time_t tmTmp;

	////////////////////////
	// ソートモードを切り替える
	////////////////////////
	if(nMode == 0)
	{	// ファイル名でソート（昇順）
		for(i=0; i<nImagesCount; i++)
		{
			for(j=i; j<nImagesCount; j++)
			{
				if(stricmp((const char*)fstrPathArray[i].str, (const char*)fstrPathArray[j].str) > 0)
				{
					// ファイル名の交換
					strTmp = fstrPathArray[i].str;
					strcpy(fstrPathArray[i].str, fstrPathArray[j].str);
					strcpy(fstrPathArray[j].str, strTmp);
					// 日時の交換
					tmTmp = fstrPathArray[i].tm_write;
					fstrPathArray[i].tm_write = fstrPathArray[j].tm_write;
					fstrPathArray[j].tm_write = tmTmp;
				}
			}
		}
	}
	else if(nMode == 1)
	{	// ファイル名でソート（降順）
		for(i=0; i<nImagesCount; i++)
		{
			for(j=i; j<nImagesCount; j++)
			{
				if(stricmp((const char*)fstrPathArray[i].str, (const char*)fstrPathArray[j].str) < 0)
				{
					// ファイル名の交換
					strTmp = fstrPathArray[i].str;
					strcpy(fstrPathArray[i].str, fstrPathArray[j].str);
					strcpy(fstrPathArray[j].str, strTmp);
					// 日時の交換
					tmTmp = fstrPathArray[i].tm_write;
					fstrPathArray[i].tm_write = fstrPathArray[j].tm_write;
					fstrPathArray[j].tm_write = tmTmp;
				}
			}
		}
	}
	else if(nMode == 2)
	{	// 日時でソート（昇順）
		for(i=0; i<nImagesCount; i++)
		{
			for(j=i; j<nImagesCount; j++)
			{
				if(fstrPathArray[i].tm_write > fstrPathArray[j].tm_write)
				{
					// ファイル名の交換
					strTmp = fstrPathArray[i].str;
					strcpy(fstrPathArray[i].str, fstrPathArray[j].str);
					strcpy(fstrPathArray[j].str, strTmp);
					// 日時の交換
					tmTmp = fstrPathArray[i].tm_write;
					fstrPathArray[i].tm_write = fstrPathArray[j].tm_write;
					fstrPathArray[j].tm_write = tmTmp;
				}
			}
		}
	}
	else if(nMode == 3)
	{	// 日時でソート（降順）
		for(i=0; i<nImagesCount; i++)
		{
			for(j=i; j<nImagesCount; j++)
			{
				if(fstrPathArray[i].tm_write < fstrPathArray[j].tm_write)
				{
					// ファイル名の交換
					strTmp = fstrPathArray[i].str;
					strcpy(fstrPathArray[i].str, fstrPathArray[j].str);
					strcpy(fstrPathArray[j].str, strTmp);
					// 日時の交換
					tmTmp = fstrPathArray[i].tm_write;
					fstrPathArray[i].tm_write = fstrPathArray[j].tm_write;
					fstrPathArray[j].tm_write = tmTmp;
				}
			}
		}
	}


}

// **********************************
// バージョン表示ボタン クリック
// **********************************
void CThumbHtmlImgDlg::OnBtnAbout() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	CAboutDlg dlg;
	dlg.DoModal();

}


// **********************************
// ヘルプボタン クリック
// **********************************
void CThumbHtmlImgDlg::OnBtnHelp() 
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
		"ThumbHtmlImg - サムネイル HTML 作成 （画像整理）\r\n"
		" Version 1.5   (freeware)\r\n"
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
		"   基準フォルダの下にある画像（実体）ファイルのあるフォルダ\r\n"
		"   相対パスで指定します\r\n"
		"   ※ このフォルダ内のリストをHTML化します\r\n"
		"   ※ 最大ファイル数は 1000 個 です\r\n"
		"\r\n"
		"「画像フォルダの対象拡張子」\r\n"
		"   対象ファイルの検索パターンを入力します\r\n"
		"   複数のパターンを検索する場合は、セミコロンで区切ります\r\n"
		"   (例) *.jpg;*.gif\r\n"
		"\r\n"
		"「サムネイル画像フォルダ」\r\n"
		"   基準フォルダの下にあるサムネイル画像（縮小）ファイルのあるフォルダ\r\n"
		"   相対パスで指定します\r\n"
		"   ※ 本プログラムではこのフォルダは検査しません\r\n"
		"      ブラウザで表示するまでに、縮小画像を用意してください\r\n"
		"\r\n"
		"「サムネイル画像フォルダの対象拡張子」\r\n"
		"   対象ファイルの拡張子を入力します\r\n"
		"   複数のパターンや、検索フィルタは設定できません\r\n"
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
		"「ファイル名・日時の追加」\r\n"
		"   ファイル名、年月日、時分の書き込み方法を指定します\r\n"
		"   (例) 「ファイル名（画像の左）、日時（画像の右）」を指定すると\r\n"
		"        +---------+----- +------------+--------+\r\n"
		"        |         |      |            |        |\r\n"
		"        | DSCF001 | 画像 | 2001/05/16 |        |\r\n"
		"        |         |      |   10:56    |        |\r\n"
		"        +---------+----- +------------+--------+\r\n"
		"        |         |      |            |        |\r\n"
		"\r\n"
		"        のような HTML が作成されます\r\n"
		"\r\n"
		"「ファイルの並べ替え」\r\n"
		"   ファイルの並べ替え（ソーティング）方法を指定します\r\n"
		"\r\n"
		"「上書きモード」\r\n"
		"   既存のファイルに上書きする場合に警告するかどうかを設定します\r\n"
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
		"      |         +--[photo]\r\n"
		"      |         |       +--DSCF001.JPG  : 1600x1200のデジカメ画像\r\n"
		"      |         |       +--DSCF002.JPG  : 1600x1200のデジカメ画像\r\n"
		"      |         |\r\n"
		"      |         +--[pics]\r\n"
		"      |         |       +--DSCF001.JPG  : 160x120のサムネイル画像\r\n"
		"      |         |       +--DSCF002.JPG  : 160x120のサムネイル画像\r\n"
		"      |         +--index.html           : HTMLファイル\r\n"
		"\r\n"
		"  基準フォルダ           ： C:\\Folder2\\\r\n"
		"  画像フォルダ           ： photo\r\n"
		"  (対象拡張子）          ： *.jpg\r\n"
		"  サムネイル画像フォルダ ： pics\r\n"
		"  (対象拡張子）          ： .jpg\r\n"
		"  HTML ファイル名        ： index.html\r\n"
		"\r\n"
		"  ※ 基準フォルダは絶対パス、画像・サムネイルフォルダは相対パス\r\n"
		"\r\n\r\n"
		"【あとがき・編集履歴】\r\n"
		"Version 1.0  2001/09/15\r\n"
		"  当初バージョン\r\n"
		"Version 1.5  2003/05/11\r\n"
		"  ファイル名・日時書き込み\r\n"
		"  ソーティング方法追加\r\n"
		"  標準設定のドロップダウンリスト\r\n"
		"  フォルダ選択の記憶化\r\n"
		"  ヘルプファイル\r\n"
		"\r\n\r\n"
		"【使用許諾条件】\r\n"
		"プログラム名 ： ThumbHtmlImg サムネイルHTML作成\r\n"
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
		"ThumbHtmlImg - Thumbnail HTML maker (Image List)\r\n"
		" Version 1.5   (Free Software)\r\n"
		"\r\n\r\n"
		" (C) 2000-2003 INOUE. Hirokazu\r\n"
		"\r\n\r\n\r\n"
		"[Introduction]\r\n"
		"This program can make index(list) HTML file of images in specific folder.\r\n"
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
BOOL CThumbHtmlImgDlg::OnHelpInfo(HELPINFO* pHelpInfo) 
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください
	OnBtnHelp();
	return TRUE;
}
