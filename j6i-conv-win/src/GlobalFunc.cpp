#include "stdafx.h"
#include "GlobalFunc.h"
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


// J6I を JPG にコンバートする関数
//
//
// SrcFname : 入力ファイル (J6I)
// DstPath : 出力フォルダ。NULLのときは入力ファイルと同じフォルダとなる
//
//
// SrcFname : 入力ファイル名
// DstPath : 出力パス（最後は \ で終わっていること）
// Msg : メッセージ用のバッファ（大体256バイトで足りる）
// HWnd : 上書き確認ダイアログ表示用の親ウインドウハンドル （または NULL）
// year : 年号変換しないときは0.
// ovwr : 上書き警告するかどうか

BOOL ConvertJ6I(LPCSTR SrcFname, LPCSTR DstPath, LPSTR Msg, HWND HWnd, int year, BOOL ovwr)
{

	CFile in,out;		// 入出力ファイルハンドル
	CFileException fileException;	// ファイルアクセスのエラートラップ関数
	void *buf;			// ファイルコピーの時のバッファ
	header hd;			// ヘッダ領域を読み込むバッファ
	int i, is_enable_date;
	unsigned long j;	// ファイルポインタ
	CString DstFname;	// 出力ファイル名（パスつき）
	CString FnameBody;	// 拡張子なしのファイル名

	if(SrcFname == "") return FALSE;		// 入力ファイル名がない場合

	// パスを分割して出力ファイル名を作成する
	char s_drv[_MAX_DRIVE], s_dir[_MAX_DIR], s_body[_MAX_FNAME], s_full[_MAX_DRIVE+_MAX_DIR+_MAX_FNAME+4];
	_splitpath((LPCSTR)SrcFname, s_drv, s_dir, s_body, NULL);
	// 出力パスの指定
	if(strcmp(DstPath, ""))
	{	// 出力パス固定の時
		sprintf(s_full, "%s%s.jpg", DstPath, s_body);
	}
	else sprintf(s_full, "%s%s%s.jpg", s_drv, s_dir, s_body);
	DstFname = s_full;

	FnameBody = s_body;

	sprintf(Msg, "%s のコンバート\r\n", FnameBody);
	// 上書きの確認
	CFileStatus status;
	if(CFile::GetStatus(DstFname, status))
	{	// JPG ファイルが存在する時
		if(ovwr)
		{
			CString tmpStr = FnameBody + "同じ名前の jpeg ファイルが存在します\n上書きしますか";
			if(::MessageBox(HWnd, tmpStr, "J6I ファイル コンバータ", MB_YESNO|MB_ICONQUESTION) != IDYES)
			{
				strcat(Msg, "  コンバート中止 (同一ファイル存在)\r\n");
				return FALSE;
			}
		}
		strcat(Msg, "  ファイルを上書き\r\n");
	}

	// 入出力双方のファイルを開く
	if(!in.Open( SrcFname, CFile::modeRead, &fileException))
	{
		strcat(Msg, "  J6Iファイルが読み込めない\r\n");
		return FALSE;
	}
	if(!out.Open( DstFname, CFile::modeWrite|CFile::modeCreate, &fileException))
	{
		strcat(Msg, "  JPGファイルが作成できない\r\n");
		return FALSE;
	}
	// J6I ﾌｧｲﾙからヘッダを読み出す
	if(in.Read(&hd, sizeof(header)) < sizeof(header))
	{
		strcat(Msg, "  J6Iファイルヘッダ領域エラー (認識不可)\r\n");
		out.Close();
		in.Close();
		return FALSE;
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
		strcat(Msg, "  J6Iファイルが壊れています (ファイル長不足)\r\n");
		out.Close();
		in.Close();
		return FALSE;
	}
	// ヘッダ領域の検査(時刻が格納されているかどうか)
	if(hd.enabledate == (unsigned char)0x81) is_enable_date = 1;
	else
	{
		is_enable_date = 0;
		strcat(Msg, "  時刻情報が格納されていません (強制続行)\r\n");
	}

	// 情報の表示
	CString str;
	if(is_enable_date)
	{
		// 年号固定かどうかを判断して、情報表示する
		if(year == 0)
			str.Format("  %02u年%02u月%02u日 %02u時%02u分%02u秒\r\n  %Lu ﾊﾞｲﾄ (ﾍｯﾀﾞ %Lu ﾊﾞｲﾄ) ver%d.%d attr%X\r\n",
				BCDtoINT(hd.date[0]),BCDtoINT(hd.date[1]),
				BCDtoINT(hd.date[2]),BCDtoINT(hd.date[3]),
				BCDtoINT(hd.date[4]),BCDtoINT(hd.date[5]),
				hd.endaddr - hd.startaddr + 1,
				hd.startaddr,
				hd.ver_h, hd.ver_l, hd.attr);
		else
			str.Format("  %02u年%02u月%02u日 %02u時%02u分%02u秒\r\n  %Lu ﾊﾞｲﾄ (ﾍｯﾀﾞ %Lu ﾊﾞｲﾄ) ver%d.%d attr%X\r\n",
				year,BCDtoINT(hd.date[1]),
				BCDtoINT(hd.date[2]),BCDtoINT(hd.date[3]),
				BCDtoINT(hd.date[4]),BCDtoINT(hd.date[5]),
				hd.endaddr - hd.startaddr + 1,
				hd.startaddr,
				hd.ver_h, hd.ver_l, hd.attr);
	}
	else
	{
		str.Format("  %Lu ﾊﾞｲﾄ (ﾍｯﾀﾞ %Lu ﾊﾞｲﾄ) ﾊﾞｰｼﾞｮﾝ %02d.%02d 属性 %02X\r\n",
			hd.endaddr - hd.startaddr + 1,
			hd.startaddr,
			hd.ver_h, hd.ver_l, hd.attr);
	}
	strcat(Msg, (LPCSTR)str);
	// J6I ファイルのデータのある場所までのシーク
	if((ULONG)in.Seek(hd.startaddr, CFile::begin) < hd.startaddr)
	{
		strcat(Msg, "  J6Iファイル異常\r\n");
		out.Close();
		in.Close();
		return FALSE;
	}

	// コピーするバッファの確保
	if((buf = malloc(1024*10+1)) == NULL)
	{
		strcat(Msg, "  メモリー不足 (強制終了)\r\n");
		return FALSE;
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
			long dummy = e->m_lOsError;	// 「e が一回も使われていない」警告を回避するためのダミー
			strcat(Msg, "  J6I読み込みエラー (中断)\r\n");
			break;
		}
		try
		{
			out.Write(buf, i);
		}
		catch(CFileException *e)
		{	// ファイルアクセスのエラーをトラップ
			if(e->m_cause == CFileException::diskFull)
			{
				strcat(Msg, "  ディスクの残り領域が不足しています\r\n");
				return FALSE;
			}
			strcat(Msg, "  JPG書き込みエラー (中断)\r\n");
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
		CTime FileTime;
		if(!CFile::GetStatus(DstFname, status))
		{
			strcat(Msg, "  JPG出力ファイル異常\r\n");
			return FALSE;
		}
		if(year == 0)
		{
			int yearhundred = 2000;
			if(BCDtoINT(hd.date[0])>80) yearhundred = 1900; 
			CTime DstTime(yearhundred+BCDtoINT(hd.date[0]), BCDtoINT(hd.date[1]), BCDtoINT(hd.date[2]),
				BCDtoINT(hd.date[3]), BCDtoINT(hd.date[4]), BCDtoINT(hd.date[5]));
			FileTime = DstTime;
		}
		else
		{
			CTime DstTime(year, BCDtoINT(hd.date[1]), BCDtoINT(hd.date[2]),
				BCDtoINT(hd.date[3]), BCDtoINT(hd.date[4]), BCDtoINT(hd.date[5]));
			FileTime = DstTime;
		}

		status.m_ctime = FileTime;
		status.m_mtime = FileTime;
		try{ CFile::SetStatus(DstFname, status); }
		catch(CFileException *e)
		{
			long dummy = e->m_lOsError;	// 「e が一回も使われていない」警告を回避するためのダミー
			strcat(Msg, "  JPG出力ファイルアクセス不能\r\n");
			return FALSE;
		}
	}

	free(buf);
	return TRUE;
}

// J6Iファイルのフルパスを与えると、出力ファイル名をフルパス名で返す
// 画像プレビューのときに使用する
CString GetJ6iName(LPCSTR SrcFname, LPCSTR DstPath)
{
	CString DstFname;	// 出力ファイル名（パスつき）

	if(SrcFname == "") DstFname = "";		// 入力ファイル名がない場合

	// パスを分割して出力ファイル名を作成する
	char s_drv[_MAX_DRIVE], s_dir[_MAX_DIR], s_body[_MAX_FNAME], s_full[_MAX_DRIVE+_MAX_DIR+_MAX_FNAME+4];
	_splitpath((LPCSTR)SrcFname, s_drv, s_dir, s_body, NULL);
	// 出力パスの指定
	if(strcmp(DstPath, ""))
	{	// 出力パス固定の時
		sprintf(s_full, "%s%s.jpg", DstPath, s_body);
	}
	else sprintf(s_full, "%s%s%s.jpg", s_drv, s_dir, s_body);
	DstFname = s_full;

	return DstFname;
}