// j6icnvDlg.cpp : インプリメンテーション ファイル
// メインダイアログ

#include "stdafx.h"
#include "j6icnv.h"
#include "j6icnvDlg.h"
#include "j6iProfDlg.h"
#include <stdio.h>
#include <process.h>	// spawn
#include <io.h>			// findfirst

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// ************************************************
// グローバル関数
// ************************************************

// BCD 数値を与えると通常の数値に直して返す
unsigned int BCDtoINT(unsigned char bcd)
{
	unsigned int i,j;
	i = bcd;
	j = ((i & 0xf0) >> 4) * 10;
	j += bcd & 0x0f;
	return j;
}

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
		// メモ: この位置に ClassWizard によってメンバの初期化が追加されます。
	//}}AFX_DATA_INIT
	// メモ: LoadIcon は Win32 の DestroyIcon のサブシーケンスを要求しません。
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CJ6icnvDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CJ6icnvDlg)
		// メモ: この場所には ClassWizard によって DDX と DDV の呼び出しが追加されます。
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
	ON_BN_CLICKED(IDC_BTN_SEQ, OnBtnSeq)
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

	// レジストリからデータを読み込む
	is_confirm_overwrite = AfxGetApp()->GetProfileInt("Settings","cnf_ovwr",1);
	is_use_ext_viewer = AfxGetApp()->GetProfileInt("Settings","use_extvw",0);
	is_param_vw = AfxGetApp()->GetProfileInt("Settings","param_vw",1);
	is_outpath = AfxGetApp()->GetProfileInt("Settings","sw_outpath",0);
	ext_viewer = AfxGetApp()->GetProfileString("Settings","extvw","");
	OutPath = AfxGetApp()->GetProfileString("Settings","outpath","");

	// コマンドラインの解析
	if(AfxGetApp()->m_lpCmdLine[0] != (char)NULL)
	{	// コマンドラインの１つめを入力ファイルとして扱う
		SrcFname = AfxGetApp()->m_lpCmdLine;	// argv[1]
		// ﾌｧｲﾙ名テキストボックスにファイル名をセット
		this->SetDlgItemText(IDC_TXT_FNAME, AfxGetApp()->m_lpCmdLine);

		if(is_param_vw)
		{	// 画像をコンバートし、表示する
			SetDlgItemText(IDC_TXT_MES, "");	// ﾒｯｾｰｼﾞｴﾘｱのｸﾘｱ
			CnvMain();	// コンバート
			// 画像を表示する外部プログラムを実行
			if(_spawnl(_P_NOWAIT, ext_viewer, ext_viewer, DstFname, NULL)<0)
				DspMes("外部 Viewer が実行できない");
			// コマンドラインを消去
			AfxGetApp()->m_lpCmdLine[0] = (char)NULL;
		}
	}

	return TRUE;  // TRUE を返すとコントロールに設定したフォーカスは失われません。
}

// システムメニューの処理
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

// J6I ファイルの選択
void CJ6icnvDlg::OnBtnSel() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	CFileDialog dlg(TRUE, "J6I", NULL, OFN_HIDEREADONLY|OFN_FILEMUSTEXIST|OFN_EXPLORER,
		"J6I ﾌｧｲﾙ|*.J6I||");
	dlg.m_ofn.lpstrTitle = "変換元の J6I ｲﾒｰｼﾞﾌｧｲﾙを選んでください";

	if(dlg.DoModal() != IDOK) return;

	SrcFname = dlg.GetPathName();
	// ﾌｧｲﾙ名テキストボックスにファイル名をセット
	this->SetDlgItemText(IDC_TXT_FNAME,dlg.GetPathName());

}

// コンバートボタンが押された時
void CJ6icnvDlg::OnBtnCnv() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	SetDlgItemText(IDC_TXT_MES, "");	// ﾒｯｾｰｼﾞｴﾘｱのｸﾘｱ
	CnvMain();	// コンバート
}

// J6I を JPG にコンバートする関数
void CJ6icnvDlg::CnvMain(void)
{
	CFile in,out;		// 入出力ファイルハンドル
	CFileException fileException;	// ファイルアクセスのエラートラップ関数
	void *buf;			// ファイルコピーの時のバッファ
	header hd;			// ヘッダ領域を読み込むバッファ
	int i, is_enable_date;
	unsigned long j;	// ファイルポインタ

	if(SrcFname == "") return;		// 入力ファイル名がない場合

	// パスを分割して出力ファイル名を作成する
	char s_drv[_MAX_DRIVE], s_dir[_MAX_DIR], s_body[_MAX_FNAME], s_full[_MAX_DRIVE+_MAX_DIR+_MAX_FNAME+4];
	_splitpath((LPCSTR)SrcFname, s_drv, s_dir, s_body, NULL);
	// 出力パスの指定
	if(is_outpath)
	{	// 出力パス固定の時
		sprintf(s_full, "%s%s.jpg", (LPCSTR)OutPath, s_body);
	}
	else sprintf(s_full, "%s%s%s.jpg", s_drv, s_dir, s_body);
	DstFname = s_full;

	FnameBody = s_body;


	DspMes("%s の ｺﾝﾊﾞｰﾄを開始します", FnameBody);
	// 上書きの確認
	if(is_confirm_overwrite)
	{
		CFileStatus status;
		if(CFile::GetStatus(DstFname, status))
		{	// JPG ファイルが存在する時
			CString tmpStr = FnameBody + ".jpg ﾌｧｲﾙが存在します\n上書きしますか";
			if(MessageBox(tmpStr, "上書きの確認", MB_YESNO|MB_ICONQUESTION) != IDYES)
			{
				DspMes("ｺﾝﾊﾞｰﾄしませんでした");
				return;
			}
			DspMes("ﾌｧｲﾙ %s.jpg を上書きします", FnameBody);
		}
	}

	// 入出力双方のファイルを開く
	if(!in.Open( SrcFname, CFile::modeRead, &fileException))
	{
		DspMes("J6I ﾌｧｲﾙ %s が読み込めない", SrcFname);
		return;
	}
	if(!out.Open( DstFname, CFile::modeWrite|CFile::modeCreate, &fileException))
	{
		DspMes("JPG ﾌｧｲﾙ %s が作成できない", DstFname);
		return;
	}
	// J6I ﾌｧｲﾙからヘッダを読み出す
	if(in.Read(&hd, sizeof(header)) < sizeof(header))
	{
		DspMes("J6I ﾌｧｲﾙ ﾍｯﾀﾞ領域ｴﾗｰ");
		out.Close();
		in.Close();
		return;
	}
	// ﾊﾞｲﾄ並びの入れ替え
	if(!hd.endian)
	{
		j = hd.startaddr;
		hd.startaddr = ((j & 0xff000000) >> 24) | ((j & 0x00ff0000) >> 8) | 
			((j & 0x0000ff00) << 8) | ((j & 0x000000ff) << 24);
		j = hd.endaddr;
		hd.endaddr = ((j & 0xff000000) >> 24) | ((j & 0x00ff0000) >> 8) | 
			((j & 0x0000ff00) << 8) | ((j & 0x000000ff) << 24);
	}
	// ヘッダ領域の検査
	if(hd.head != (unsigned char)0x80 || hd.dummy1[0] != (unsigned char)0x3e)
	{
		DspMes("J6I ﾌｧｲﾙﾍｯﾀﾞが JPG ﾃﾞｰﾀの長さを示していない");
		out.Close();
		in.Close();
		return;
	}
	// ヘッダ領域の検査(時刻が格納されているかどうか)
	if(hd.enabledate == (unsigned char)0x81) is_enable_date = 1;
	else
	{
		is_enable_date = 0;
		DspMes("J6I ﾌｧｲﾙﾍｯﾀﾞに時刻情報が無い");
	}

	// 情報の表示
	CString str;
	if(is_enable_date)
	{
		str.Format("　　%02u年%02u月%02u日 %02u時%02u分%02u秒\r\n　　%Lu ﾊﾞｲﾄ (ﾍｯﾀﾞ %Lu ﾊﾞｲﾄ) ﾊﾞｰｼﾞｮﾝ %02d.%02d 属性 %02X",
			BCDtoINT(hd.date[0]),
			BCDtoINT(hd.date[1]),
			BCDtoINT(hd.date[2]),
			BCDtoINT(hd.date[3]),
			BCDtoINT(hd.date[4]),
			BCDtoINT(hd.date[5]),
			hd.endaddr - hd.startaddr + 1,
			hd.startaddr,
			hd.ver_h, hd.ver_l, hd.attr);
	}
	else
	{
		str.Format("　　%Lu ﾊﾞｲﾄ (ﾍｯﾀﾞ %Lu ﾊﾞｲﾄ) ﾊﾞｰｼﾞｮﾝ %02d.%02d 属性 %02X",
			hd.endaddr - hd.startaddr + 1,
			hd.startaddr,
			hd.ver_h, hd.ver_l, hd.attr);
	}
	DspMes(str);
	// J6I ファイルのデータのある場所までのシーク
	if((ULONG)in.Seek(hd.startaddr, CFile::begin) < hd.startaddr)
	{
		DspMes("J6I ﾌｧｲﾙ ﾍｯﾀﾞ領域ｴﾗｰ");
		out.Close();
		in.Close();
		return;
	}

	// コピーするバッファの確保
	if((buf = malloc(1024*10+1)) == NULL)
	{
		DspMes("ｺﾋﾟｰ用ﾊﾞｯﾌｧ領域 10 kBytes が確保できない");
		return;
	}
	
	j = hd.endaddr - hd.startaddr + 1;	// JPG データ長の計算
	// データのコピー
	do{
		try
		{
			i = in.Read(buf, 1024*10 < j ? 1024*10 : j);
		}
		catch(CFileException *e)
		{	// ファイルアクセスのエラーをトラップ
			if(e->m_cause == CFileException::accessDenied) DspMes("J6Iﾌｧｲﾙのｱｸｾｽ禁止");
			DspMes("J6I ﾌｧｲﾙ 読み込みｴﾗｰ");
			break;
		}
		try
		{
			out.Write(buf, i);
		}
		catch(CFileException *e)
		{	// ファイルアクセスのエラーをトラップ
			if(e->m_cause == CFileException::diskFull) DspMes("ﾃﾞｨｽｸがいっぱい");
			DspMes("JPG ﾌｧｲﾙ 書き込みｴﾗｰ");
			break;
		}
		j -= i;
	}while(i && j>0);

	out.Close();
	in.Close();

	if(is_enable_date)
	{
		// JPG ﾌｧｲﾙの日付設定
		CFileStatus status;
		if(!CFile::GetStatus(DstFname, status))
		{
			DspMes("JPG ﾌｧｲﾙ %s の日付読み込みｴﾗｰ", DstFname);
			return;
		}
		int yearhundred = 2000;
		if(BCDtoINT(hd.date[0])>80) yearhundred = 1900; 
		CTime DstTime(yearhundred+BCDtoINT(hd.date[0]), BCDtoINT(hd.date[1]), BCDtoINT(hd.date[2]),
				BCDtoINT(hd.date[3]), BCDtoINT(hd.date[4]), BCDtoINT(hd.date[5]));

		status.m_ctime = DstTime;
		status.m_mtime = DstTime;
		try{ CFile::SetStatus(DstFname, status); }
		catch(CFileException *e)
		{
			if(e->m_cause == CFileException::accessDenied)
				DspMes("JPG ﾌｧｲﾙ %s にｱｸｾｽ不能", DstFname);
			DspMes("JPG ﾌｧｲﾙ %s の日付書き込みｴﾗｰ", DstFname);
			return;
		}
	}

	DspMes("%s 処理完了", FnameBody);

	free(buf);

	return;
}

// メッセージエリアに文字列を表示する
void CJ6icnvDlg::DspMes(CString str, CString str2)
{
	CString strStack, strTemp;
	if(str2 != "") strTemp.Format(str, str2);
	else strTemp = str;
	GetDlgItemText(IDC_TXT_MES, strStack);
	if(strStack != "") strStack = strStack + "\r\n" + strTemp;
	else strStack = strTemp;

	SetDlgItemText(IDC_TXT_MES, strStack);
	return;
}

// 機能設定ダイアログを開く
void CJ6icnvDlg::OnBtnProf() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	CProfDlg profDlg;

	// ダイアログボックスの項目を設定
	profDlg.m_chk_ovwr = (BOOL)is_confirm_overwrite;
	profDlg.m_chk_ext = (BOOL)is_use_ext_viewer;
	profDlg.m_paramvw = (BOOL)is_param_vw;
	profDlg.m_outpath = (BOOL)is_outpath;
	profDlg.m_txt_extviewer = ext_viewer;
	profDlg.m_txt_outpath = OutPath;

	if (profDlg.DoModal() == IDOK)
	{
		// ダイアログの内容をレジストリに保存
		if(profDlg.m_chk_ovwr) AfxGetApp()->WriteProfileInt("Settings","cnf_ovwr",1);
		else AfxGetApp()->WriteProfileInt("Settings","cnf_ovwr",0);
		if(profDlg.m_chk_ext) AfxGetApp()->WriteProfileInt("Settings","use_extvw",1);
		else AfxGetApp()->WriteProfileInt("Settings","use_extvw",0);
		if(profDlg.m_paramvw) AfxGetApp()->WriteProfileInt("Settings","param_vw",1);
		else AfxGetApp()->WriteProfileInt("Settings","param_vw",0);
		if(profDlg.m_outpath) AfxGetApp()->WriteProfileInt("Settings","sw_outpath",1);
		else AfxGetApp()->WriteProfileInt("Settings","sw_outpath",0);
		AfxGetApp()->WriteProfileString("Settings","extvw", profDlg.m_txt_extviewer);
		AfxGetApp()->WriteProfileString("Settings","outpath", profDlg.m_txt_outpath);
		// レジストリを内部変数に格納し直す
		is_confirm_overwrite = AfxGetApp()->GetProfileInt("Settings","cnf_ovwr",1);
		is_use_ext_viewer = AfxGetApp()->GetProfileInt("Settings","use_extvw",0);
		is_param_vw = AfxGetApp()->GetProfileInt("Settings","param_vw",1);
		is_outpath = AfxGetApp()->GetProfileInt("Settings","sw_outpath",0);
		ext_viewer = AfxGetApp()->GetProfileString("Settings","extvw","");
		OutPath = AfxGetApp()->GetProfileString("Settings","outpath","");
	}
}

// 変換後の画像を表示する
void CJ6icnvDlg::OnBtnShw() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	if(SrcFname =="")
	{
		DspMes("まず、ｲﾒｰｼﾞﾌｧｲﾙを選択してください");
		return;
	}
	CFileStatus status;
	if(!CFile::GetStatus(DstFname, status))
	{	// JPG ファイルが存在しない時
		OnBtnCnv();
	}
	if(_spawnl(_P_NOWAIT, ext_viewer, ext_viewer, DstFname, NULL)<0)
		DspMes("外部 Viewer が実行できない");
}

// 選んだフォルダの下のすべてのファイルを連続して変換する
void CJ6icnvDlg::OnBtnSeq() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	CFileDialog dlg(TRUE, "J6I", "入力ﾌｧｲﾙ.J6I", OFN_HIDEREADONLY|OFN_PATHMUSTEXIST|OFN_EXPLORER,
		"J6I ﾌｧｲﾙ|*.J6I|すべてのﾌｧｲﾙ|*.*||");
	dlg.m_ofn.lpstrTitle = "変換元の J6I ｲﾒｰｼﾞﾌｧｲﾙを含んでいるﾌｫﾙﾀﾞを指定して下さい";

	if(dlg.DoModal() != IDOK) return;

	SetDlgItemText(IDC_TXT_MES, "");	// ﾒｯｾｰｼﾞｴﾘｱのｸﾘｱ

	CString SrcFinfPath = dlg.GetPathName();

	// 入力ファイル名(ワイルドカード)の生成
	char s_drv[_MAX_DRIVE], s_dir[_MAX_DIR], s_body[_MAX_FNAME], s_full[_MAX_DRIVE+_MAX_DIR+_MAX_FNAME+4];
	_splitpath((LPCSTR)SrcFinfPath, s_drv, s_dir, s_body, NULL);
	sprintf(s_full, "%s%s*.j6i", s_drv, s_dir);

	struct _finddata_t finddata;
	long hFind;
    if( (hFind = _findfirst(s_full, &finddata )) == -1L )
	{
		DspMes("ｺﾝﾊﾞｰﾄ対象ﾌｧｲﾙが見つからない");
		return;
	}
	if(!(finddata.attrib & _A_SUBDIR))
	{
		strcpy(s_full, finddata.name);
		SrcFname = s_full;
		CnvMain();
	}
	while(_findnext( hFind, &finddata ) == 0)
	{
		if(!(finddata.attrib == _A_SUBDIR))
		{
			strcpy(s_full, finddata.name);
			SrcFname = s_full;
			CnvMain();
		}
	}
}
