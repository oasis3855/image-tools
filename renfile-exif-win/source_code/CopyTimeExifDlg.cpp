// CopyTimeExifDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "CopyTimeExif.h"
#include "CopyTimeExifDlg.h"
#include "DialogMessage.h"
#include "GlobalFunc.h"
#include <io.h>			// findfirst

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// **********************************
// グローバル変数・定数
// **********************************
#define	MAX_PATH_ARRAY	1000		// ソートできるファイル数の最大値

CString strMsg;


/////////////////////////////////////////////////////////////////////////////
// CCopyTimeExifDlg ダイアログ

CCopyTimeExifDlg::CCopyTimeExifDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCopyTimeExifDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCopyTimeExifDlg)
		// メモ: この位置に ClassWizard によってメンバの初期化が追加されます。
	//}}AFX_DATA_INIT
	// メモ: LoadIcon は Win32 の DestroyIcon のサブシーケンスを要求しません。
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCopyTimeExifDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCopyTimeExifDlg)
	DDX_Control(pDX, IDC_CMB_FILENAME_APPEND, m_cmb_filename_append_ctrl);
	DDX_Control(pDX, IDC_CMB_FILENAME, m_cmb_filename_ctrl);
	DDX_Control(pDX, IDC_CMB_MODE, m_cmb_mode_ctrl);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCopyTimeExifDlg, CDialog)
	//{{AFX_MSG_MAP(CCopyTimeExifDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_PICKFILE, OnBtnPickfile)
	ON_BN_CLICKED(IDC_BTN_COPY, OnBtnCopy)
	ON_BN_CLICKED(IDC_BTN_PICKFOLDER, OnBtnPickfolder)
	ON_BN_CLICKED(IDC_BTN_ABOUT, OnBtnAbout)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCopyTimeExifDlg メッセージ ハンドラ

BOOL CCopyTimeExifDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// このダイアログ用のアイコンを設定します。フレームワークはアプリケーションのメイン
	// ウィンドウがダイアログでない時は自動的に設定しません。
	SetIcon(m_hIcon, TRUE);			// 大きいアイコンを設定
	SetIcon(m_hIcon, FALSE);		// 小さいアイコンを設定
	
	// TODO: 特別な初期化を行う時はこの場所に追加してください。

	////////////////////////
	// ダイアログの初期値を設定
	////////////////////////
	m_cmb_mode_ctrl.SetCurSel(0);
	m_cmb_filename_ctrl.SetCurSel(1);
	m_cmb_filename_append_ctrl.SetCurSel(0);

	
	return TRUE;  // TRUE を返すとコントロールに設定したフォーカスは失われません。
}

// もしダイアログボックスに最小化ボタンを追加するならば、アイコンを描画する
// コードを以下に記述する必要があります。MFC アプリケーションは document/view
// モデルを使っているので、この処理はフレームワークにより自動的に処理されます。

void CCopyTimeExifDlg::OnPaint() 
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
HCURSOR CCopyTimeExifDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

// **********************************
// ファイル参照のボタンを押したとき
// **********************************
void CCopyTimeExifDlg::OnBtnPickfile() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	CString strTmp1;
	strTmp1.LoadString(STR_BTN_PICKFILE);
	CFileDialog dlg(TRUE, NULL, NULL, OFN_FILEMUSTEXIST|OFN_EXPLORER,strTmp1);
	if(dlg.DoModal()==TRUE)
		SetDlgItemText(IDC_EDIT_FILENAME, dlg.GetPathName());
	
}

int CALLBACK myBrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData);

// **********************************
// フォルダ参照のボタンを押したとき
// **********************************
void CCopyTimeExifDlg::OnBtnPickfolder() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	char strPathname[MAX_PATH];
	LPITEMIDLIST pidl;
	BROWSEINFO bi;

	// テキストボックスに入力されている親パスを得る
	GetDlgItemText(IDC_EDIT_FILENAME, strPathname, MAX_PATH-1);

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

	// 最後に *.* を追加する
	strcat(strPathname, "*.*");

	// ファイル入力ボックスをアップデートする
	SetDlgItemText(IDC_EDIT_FILENAME, strPathname);
	
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
// 実行ボタンを押したとき
// **********************************
void CCopyTimeExifDlg::OnBtnCopy() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください

	CString strTmp1, strTmp2;
	struct _fstr fstrPathArray[MAX_PATH_ARRAY];		// ファイル名をソートして格納するための配列
	int nFileCount = 0;								// 対象のファイル数 1～
	char strPathname[MAX_PATH], strFilename[MAX_PATH], strPathOnly[MAX_PATH];
	int i;

	// 情報バッファの初期化
	strMsg = "";

	// ファイル名入力ボックスからファイル名を読み込む
	GetDlgItemText(IDC_EDIT_FILENAME, strPathname, MAX_PATH-1);
	if(!strlen(strPathname))
	{	// ファイル名が指定されていないとき
		strTmp1.LoadString(STR_DLG_NOFILE);
		strTmp2.LoadString(STR_DLG_DLGTITLE);
		MessageBox(strTmp1, strTmp2, MB_OK|MB_ICONINFORMATION);
		return;
	}

	// パス名のみの格納 （C:\xxx\xxx\　← 最終のルート記号含む）
	strcpy(strPathOnly, "");
	for(i=strlen(strPathname)-1; i>=0; i--)
	{
		if(strPathname[i] == '\\')
		{
			strncpy(strPathOnly, strPathname, i+1);
			strPathOnly[i+1] = (char)0;
			break;
		}
		if(strPathname[i] == ':')
		{
			strncpy(strPathOnly, strPathname, i+1);
			strPathOnly[i+1] = (char)0;
			break;
		}

	}

	// ファイル名を検索して、振り分けルーチンを呼び出す
	struct _finddata_t finddata;
	long hFind;

	if( (hFind = _findfirst(strPathname, &finddata )) == -1L )
	{	// 参照したファイルが見つからない場合
		strTmp1.LoadString(STR_NO_FILE);
		strMsg = strMsg + strTmp1 + "\r\n";
	}
	else
	{
		if(!(finddata.attrib & _A_SUBDIR))
		{	// 一つ目のファイルの発見
			strncpy(fstrPathArray[nFileCount].str, finddata.name, MAX_PATH-1);
			nFileCount++;
		}
		while(_findnext( hFind, &finddata ) == 0)
		{	// 続くファイルを連続発見
			if(!(finddata.attrib & _A_SUBDIR))
			{
				strncpy(fstrPathArray[nFileCount].str, finddata.name, MAX_PATH-1);
				nFileCount++;
			}
			if(nFileCount >= MAX_PATH_ARRAY) break;
		}
	
		if(nFileCount >= MAX_PATH_ARRAY)
		{	// ファイル数が上限を超えた場合
			strTmp1.LoadString(STR_ERR_EXCEED_FILE);	// 画像ファイル数が処理限界を超えました\r\n途中まで処理します
			strMsg = strMsg + strTmp1 + "\r\n";
		}

		// ファイル名のソート
		SortArray(fstrPathArray, nFileCount, 0);

		for(i=0; i<nFileCount; i++)
		{
			strncpy(strFilename, fstrPathArray[i].str, MAX_PATH-1);
			CopyFuncDispatch(strPathname, strFilename, strPathOnly);
		}

	}

	CDialogMessage dlg;
	dlg.m_edit_main_text= strMsg;
	dlg.DoModal();

}

// **********************************
// 実行モード（ドロップダウンボックス）にしたがって、機能別の関数に振り分ける
// **********************************
void CCopyTimeExifDlg::CopyFuncDispatch(char *strPathname, char *strFilename, char *strPathOnly)
{
	CString strTmp1, strTmp2;

	strTmp2 = strFilename;

	switch(m_cmb_mode_ctrl.GetCurSel())
	{
	case 0 : // 日付の変更
		ChangeDate(strPathname, strFilename, strPathOnly);
		break;
	case 1 : // ファイル名の変更
		ChangeFname(strPathname, strFilename, strPathOnly);
		break;
	case 2 : // EXIFデータの表示
		PreviewExif(strPathname, strFilename, strPathOnly);
		break;
	default :
		strTmp1.LoadString(STR_FUNC_NOTINST);
		strMsg = strTmp2 + " : " + strTmp1 + "\n";
		break;
	}

}




void CCopyTimeExifDlg::PreviewExif(char *strPathname, char *strFilename, char *strPathOnly)
{
	CString strTmp;
	struct exif_data exifData;
	char strExifErrMessage[MAX_EXIF_ERR_MESSAGE_SIZE+1];

	init_exif_data_struct(&exifData);

	if(strMsg.GetLength() > 1000000) return;	// 1MBytes以上は表示しない

	strTmp = strFilename;
	strMsg += strTmp + "\r\n";

	::read_exif_data(strPathOnly, strFilename, &exifData, strExifErrMessage);

	if(strlen(strExifErrMessage))
	{	// エラーメッセージがある場合
		strMsg = strMsg + strExifErrMessage + "\r\n";
	}


	strTmp.Format("  ImageDescription : %s\r\n"
					"  Make : %s\r\n"
					"  Model : %s  ,Software : %s  ,ExifVersion : %s\r\n"
					"  Date (IFD0) %s  , (Original) %s , (Digitized) %s\r\n"
					"  ExifImageWidth : %ld , ExifImageHeight : %ld\r\n"
					"  ISOSpeedRatings : %d , ExposureTime : 1/%.0lf , FNumber : %.2lf\r\n",
					exifData.ImageDescription,
					exifData.Make,
					exifData.Model, exifData.Software, exifData.ExifVersion,
					exifData.DateTime, exifData.DateTimeOriginal, exifData.DateTimeDigitized,
					exifData.ExifImageWidth, exifData.ExifImageHeight,
					exifData.ISOSpeedRatings, exifData.ExposureTime, exifData.FNumber);

	strMsg += strTmp;

}


BOOL CCopyTimeExifDlg::ChangeDate(char *strPathname, char *strFilename, char *strPathOnly)
{
	CString strTmp;
	struct exif_data exifData;
	char strExifErrMessage[MAX_EXIF_ERR_MESSAGE_SIZE+1];
	CFileStatus Fstatus;
	CString strFullPath;

	// strtok (トークン切り出し) 専用の文字列宣言
	char str_ext_raw[21];		// 切り出し元の文字列。切り出しにしたがって破壊される
	char str_delimit[] = ": ";	// 区切り文字 （コロン および 空白）
	char *str_strtok;			// 切り出された結果文字列

	struct tm tmTime;

	// 情報メッセージの設定 （ファイル名）
	strTmp = strFilename;
	strMsg += strTmp + " : ";

	init_exif_data_struct(&exifData);

	// EXIF 情報を読み込む
	if(!::read_exif_data(strPathOnly, strFilename, &exifData, strExifErrMessage))
	{
		if(strlen(strExifErrMessage))
		{	// エラーメッセージがある場合
			strMsg = strMsg + strExifErrMessage + "\r\n";
		}

		return FALSE;
	}

	// EXIF より時間情報を tm 構造体に代入する
	strncpy(str_ext_raw, exifData.DateTime, 20);	// 破壊に備えてコピー

	str_strtok = strtok(str_ext_raw, str_delimit);
	tmTime.tm_year = atoi(str_strtok);
	if(tmTime.tm_year < 1970 || tmTime.tm_year > 2032)
	{
		if(tmTime.tm_year < 0 || tmTime.tm_year > 99) return FALSE;
		if(tmTime.tm_year <= 99 && tmTime.tm_year > 50) tmTime.tm_year += 1900;
		else tmTime.tm_year += 2000;
	}
	tmTime.tm_year -= 1900;						// tm_year は 1900年からの年数

	str_strtok = strtok(NULL, str_delimit);
	tmTime.tm_mon = atoi(str_strtok) - 1;		// tm_mon は 0 ～ 11
	if(tmTime.tm_mon < 0 || tmTime.tm_mon > 12) return FALSE;

	str_strtok = strtok(NULL, str_delimit);
	tmTime.tm_mday = atoi(str_strtok);
	if(tmTime.tm_mday < 0 || tmTime.tm_mday > 31) return FALSE;

	str_strtok = strtok(NULL, str_delimit);
	tmTime.tm_hour = atoi(str_strtok);
	if(tmTime.tm_hour < 0 || tmTime.tm_hour > 23) return FALSE;

	str_strtok = strtok(NULL, str_delimit);
	tmTime.tm_min = atoi(str_strtok);
	if(tmTime.tm_min < 0 || tmTime.tm_min > 60) return FALSE;

	str_strtok = strtok(NULL, str_delimit);
	if(!strlen(str_strtok)) return FALSE;
	tmTime.tm_sec = atoi(str_strtok);
	if(tmTime.tm_sec < 0 || tmTime.tm_sec > 60) return FALSE;

	tmTime.tm_isdst = 0;	// 夏時間でない
	tmTime.tm_wday = 0;		// ダミーをとりあえず入れておく
	tmTime.tm_yday = 0; 	// ダミーをとりあえず入れておく

	strFullPath.Format("%s%s", strPathOnly, strFilename);

	// ファイルの情報を取得する
	if(!CFile::GetStatus(strFullPath, Fstatus))
	{
		if(strMsg.GetLength() < 1000000)
		{
			strTmp.LoadString(STR_GETSTATUS_ERR);
			strMsg = strMsg + strTmp + "\r\n";
		}
		return FALSE;
	}

	// 情報メッセージの設定 （元のタイムスタンプ）
	strTmp.Format("%04d:%02d:%02d %02d:%02d:%02d ", Fstatus.m_mtime.GetYear(), Fstatus.m_mtime.GetMonth(),
					Fstatus.m_mtime.GetDay(), Fstatus.m_mtime.GetHour(), Fstatus.m_mtime.GetMinute(), Fstatus.m_mtime.GetSecond());
	strMsg += strTmp;

	// 時刻情報バッファを更新
	Fstatus.m_ctime = mktime(&tmTime);
	Fstatus.m_atime = mktime(&tmTime);
	Fstatus.m_mtime = mktime(&tmTime);

	// ファイルの情報を書き込む
	try{ CFile::SetStatus(strFullPath, Fstatus); }
	catch(CFileException *e)
	{	// コピー先ファイルに書き込めないとき
		long dummy = e->m_lOsError;	// 「e が一回も使われていない」警告を回避するためのダミー
		strTmp.LoadString(STR_SETSTATUS_ERR);
		strMsg = strMsg + strTmp + "\r\n";
		return FALSE;
	}

	// 情報メッセージの設定 （新タイムスタンプ）
	strTmp.Format("-> %04d:%02d:%02d %02d:%02d:%02d ", Fstatus.m_mtime.GetYear(), Fstatus.m_mtime.GetMonth(),
					Fstatus.m_mtime.GetDay(), Fstatus.m_mtime.GetHour(), Fstatus.m_mtime.GetMinute(), Fstatus.m_mtime.GetSecond());
	strMsg = strMsg + strTmp + "\r\n";

	return TRUE;
}

BOOL CCopyTimeExifDlg::ChangeFname(char *strPathname, char *strFilename, char *strPathOnly)
{
	CString strTmp;
	struct exif_data exifData;
	char strExifErrMessage[MAX_EXIF_ERR_MESSAGE_SIZE+1];
	CFileStatus Fstatus;
	CString strFullPath, strNewFilename, strNewFullPath;

	// strtok (トークン切り出し) 専用の文字列宣言
	char str_ext_raw[21];		// 切り出し元の文字列。切り出しにしたがって破壊される
	char str_delimit[] = ": ";	// 区切り文字 （コロン および 空白）
	char *str_strtok;			// 切り出された結果文字列

	struct tm tmTime;
	CTime tmCtime;

	int i;

	// 情報メッセージの設定 （ファイル名）
	strTmp = strFilename;
	strMsg += strTmp + " : ";

	init_exif_data_struct(&exifData);

	// EXIF 情報を読み込む
	if(!::read_exif_data(strPathOnly, strFilename, &exifData, strExifErrMessage))
	{
		if(strlen(strExifErrMessage))
		{	// エラーメッセージがある場合
			strMsg = strMsg + strExifErrMessage + "\r\n";
		}

		return FALSE;
	}

	// EXIF より時間情報を tm 構造体に代入する
	strncpy(str_ext_raw, exifData.DateTime, 20);	// 破壊に備えてコピー

	str_strtok = strtok(str_ext_raw, str_delimit);
	tmTime.tm_year = atoi(str_strtok);
	if(tmTime.tm_year < 1970 || tmTime.tm_year > 2032)
	{
		if(tmTime.tm_year < 0 || tmTime.tm_year > 99) return FALSE;
		if(tmTime.tm_year <= 99 && tmTime.tm_year > 50) tmTime.tm_year += 1900;
		else tmTime.tm_year += 2000;
	}
	tmTime.tm_year -= 1900;						// tm_year は 1900年からの年数

	str_strtok = strtok(NULL, str_delimit);
	tmTime.tm_mon = atoi(str_strtok) - 1;		// tm_mon は 0 ～ 11
	if(tmTime.tm_mon < 0 || tmTime.tm_mon > 12) return FALSE;

	str_strtok = strtok(NULL, str_delimit);
	tmTime.tm_mday = atoi(str_strtok);
	if(tmTime.tm_mday < 0 || tmTime.tm_mday > 31) return FALSE;

	str_strtok = strtok(NULL, str_delimit);
	tmTime.tm_hour = atoi(str_strtok);
	if(tmTime.tm_hour < 0 || tmTime.tm_hour > 23) return FALSE;

	str_strtok = strtok(NULL, str_delimit);
	tmTime.tm_min = atoi(str_strtok);
	if(tmTime.tm_min < 0 || tmTime.tm_min > 60) return FALSE;

	str_strtok = strtok(NULL, str_delimit);
	if(!strlen(str_strtok)) return FALSE;
	tmTime.tm_sec = atoi(str_strtok);
	if(tmTime.tm_sec < 0 || tmTime.tm_sec > 60) return FALSE;

	tmTime.tm_isdst = 0;	// 夏時間でない
	tmTime.tm_wday = 0;		// ダミーをとりあえず入れておく
	tmTime.tm_yday = 0; 	// ダミーをとりあえず入れておく

	strFullPath.Format("%s%s", strPathOnly, strFilename);

	// ファイルの情報を取得する
	if(!CFile::GetStatus(strFullPath, Fstatus))
	{
		if(strMsg.GetLength() < 1000000)
		{
			strTmp.LoadString(STR_GETSTATUS_ERR);
			strMsg = strMsg + strTmp + "\r\n";
		}
		return FALSE;
	}

	tmCtime = mktime(&tmTime);

	// 新ファイル名の作成
	switch(m_cmb_filename_ctrl.GetCurSel())
	{
	case 0 : // yy-mm-dd hh-mm
		strNewFilename.Format("%02d-%02d-%02d %02d-%02d", tmCtime.GetYear() >= 2000 ? tmCtime.GetYear()-2000:tmCtime.GetYear()-1900, tmCtime.GetMonth(),
					tmCtime.GetDay(), tmCtime.GetHour(), tmCtime.GetMinute());
		break;
	case 1 : // yy-mm-dd hh-mm-ss
		strNewFilename.Format("%02d-%02d-%02d %02d-%02d-%02d", tmCtime.GetYear() >= 2000 ? tmCtime.GetYear()-2000:tmCtime.GetYear()-1900, tmCtime.GetMonth(),
					tmCtime.GetDay(), tmCtime.GetHour(), tmCtime.GetMinute(), tmCtime.GetSecond());
		break;
	case 2 : // yymmdd hhmmss
		strNewFilename.Format("%02d%02d%02d %02d%02d%02d", tmCtime.GetYear() >= 2000 ? tmCtime.GetYear()-2000:tmCtime.GetYear()-1900, tmCtime.GetMonth(),
					tmCtime.GetDay(), tmCtime.GetHour(), tmCtime.GetMinute(), tmCtime.GetSecond());
		break;
	case 3 : // yyyy-mm-dd hh-mm
		strNewFilename.Format("%04d-%02d-%02d %02d-%02d", tmCtime.GetYear(), tmCtime.GetMonth(),
					tmCtime.GetDay(), tmCtime.GetHour(), tmCtime.GetMinute());
		break;
	case 4 : // yyyy-mm-dd hh-mm-ss
		strNewFilename.Format("%04d-%02d-%02d %02d-%02d-%02d", tmCtime.GetYear(), tmCtime.GetMonth(),
					tmCtime.GetDay(), tmCtime.GetHour(), tmCtime.GetMinute(), tmCtime.GetSecond());
		break;
	case 5 : // yyyymmdd hhmmss
		strNewFilename.Format("%04d%02d%02d %02d%02d%02d", tmCtime.GetYear(), tmCtime.GetMonth(),
					tmCtime.GetDay(), tmCtime.GetHour(), tmCtime.GetMinute(), tmCtime.GetSecond());
		break;
	case 6 : // mm-dd hh-mm
		strNewFilename.Format("%02d-%02d %02d-%02d", tmCtime.GetMonth(),
					tmCtime.GetDay(), tmCtime.GetHour(), tmCtime.GetMinute());
		break;
	case 7 : // mm-dd hh-mm-ss
		strNewFilename.Format("%02d-%02d %02d-%02d-%02d", tmCtime.GetMonth(),
					tmCtime.GetDay(), tmCtime.GetHour(), tmCtime.GetMinute(), tmCtime.GetSecond());
		break;
	case 8 : // yy-mm-dd
		strNewFilename.Format("%02d-%02d-%02d", tmCtime.GetYear() >= 2000 ? tmCtime.GetYear()-2000:tmCtime.GetYear()-1900, tmCtime.GetMonth(),
					tmCtime.GetDay());
		break;
	case 9 : // yyyy-mm-dd
		strNewFilename.Format("%04d-%02d-%02d", tmCtime.GetYear(), tmCtime.GetMonth(),
					tmCtime.GetDay());
		break;
	case 10 : // hh-mm
		strNewFilename.Format("%02d-%02d",
					tmCtime.GetHour(), tmCtime.GetMinute());
		break;
	case 11 : // hh-mm-ss
		strNewFilename.Format("%02d-%02d-%02d", 
					tmCtime.GetHour(), tmCtime.GetMinute(), tmCtime.GetSecond());
		break;
	case 12 : // OLYMPUS 形式
	case 13 : // 日時情報なし
		strNewFilename = "";
		break;
	default :
		break;
	}

	switch(m_cmb_filename_append_ctrl.GetCurSel())
	{
	case 0 : // 何も付加しない
		break;
	case 1 : // F値 - シャッター速度
		strTmp.Format(" %02.2lf-%04.0lf", exifData.FNumber, exifData.ExposureTime);
		strNewFilename += strTmp;
		break;
	case 2 : // ISO - F値 - シャッター速度
		strTmp.Format(" %04d-%02.2lf-%04.0lf", exifData.ISOSpeedRatings, exifData.FNumber, exifData.ExposureTime);
		strNewFilename += strTmp;
		break;
	case 3 : // F値  シャッター速度
		strTmp.Format(" %02.2lf %04.0lf", exifData.FNumber, exifData.ExposureTime);
		strNewFilename += strTmp;
		break;
	case 4 : // ISO  F値  シャッター速度
		strTmp.Format(" %04d %02.2lf %04.0lf", exifData.ISOSpeedRatings, exifData.FNumber, exifData.ExposureTime);
		strNewFilename += strTmp;
		break;
	}

	if(strNewFilename == "") strNewFilename = "No Title";

	// 新ファイル名が使われていないか検査し、既存の場合 "(n)" を加える
	strTmp = strNewFilename + ".jpg";
	for(i=1; i<10000; i++)
	{
		strNewFullPath.Format("%s%s", strPathOnly, strTmp);
		if(!CFile::GetStatus(strNewFullPath, Fstatus)) break;		// ファイルが存在しない場合、抜ける
		strTmp.Format("%s(%d).jpg", strNewFilename, i);
	}

	strNewFilename = strTmp;	// 新ファイル名

	// 新ファイル名 (フルパス)
	strNewFullPath.Format("%s%s", strPathOnly, strTmp);

	// ファイルの情報を書き込む
	try{ CFile::Rename(strFullPath, strNewFullPath); }
	catch(CFileException *e)
	{	// コピー先ファイルに書き込めないとき
		long dummy = e->m_lOsError;	// 「e が一回も使われていない」警告を回避するためのダミー
		strTmp.LoadString(STR_SETSTATUS_ERR);
		strMsg = strMsg + strTmp + "\r\n";
		return FALSE;
	}

	// 情報メッセージの設定 （ファイル名）
	strMsg += "->" + strNewFilename + "\r\n";

	return TRUE;
}

void CCopyTimeExifDlg::OnBtnAbout() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	CString strTmp1, strTmp2;

	strTmp1.LoadString(STR_DLG_DLGTITLE);
	strTmp2.LoadString(STR_DLG_ABOUTBOX);

	MessageBox(strTmp2, strTmp1, MB_OK|MB_ICONINFORMATION);
}

