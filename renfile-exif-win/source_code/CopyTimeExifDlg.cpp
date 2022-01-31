// CopyTimeExifDlg.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include "CopyTimeExif.h"
#include "CopyTimeExifDlg.h"
#include "DialogMessage.h"
#include <io.h>			// findfirst

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// **********************************
// �O���[�o���ϐ��E�萔
// **********************************
#define	MAX_PATH_ARRAY	1000		// �\�[�g�ł���t�@�C�����̍ő�l

CString strMsg;


/////////////////////////////////////////////////////////////////////////////
// CCopyTimeExifDlg �_�C�A���O

CCopyTimeExifDlg::CCopyTimeExifDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCopyTimeExifDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCopyTimeExifDlg)
		// ����: ���̈ʒu�� ClassWizard �ɂ���ă����o�̏��������ǉ�����܂��B
	//}}AFX_DATA_INIT
	// ����: LoadIcon �� Win32 �� DestroyIcon �̃T�u�V�[�P���X��v�����܂���B
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
// CCopyTimeExifDlg ���b�Z�[�W �n���h��

BOOL CCopyTimeExifDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ���̃_�C�A���O�p�̃A�C�R����ݒ肵�܂��B�t���[�����[�N�̓A�v���P�[�V�����̃��C��
	// �E�B���h�E���_�C�A���O�łȂ����͎����I�ɐݒ肵�܂���B
	SetIcon(m_hIcon, TRUE);			// �傫���A�C�R����ݒ�
	SetIcon(m_hIcon, FALSE);		// �������A�C�R����ݒ�
	
	// TODO: ���ʂȏ��������s�����͂��̏ꏊ�ɒǉ����Ă��������B

	////////////////////////
	// �_�C�A���O�̏����l��ݒ�
	////////////////////////
	m_cmb_mode_ctrl.SetCurSel(0);
	m_cmb_filename_ctrl.SetCurSel(1);
	m_cmb_filename_append_ctrl.SetCurSel(0);

	
	return TRUE;  // TRUE ��Ԃ��ƃR���g���[���ɐݒ肵���t�H�[�J�X�͎����܂���B
}

// �����_�C�A���O�{�b�N�X�ɍŏ����{�^����ǉ�����Ȃ�΁A�A�C�R����`�悷��
// �R�[�h���ȉ��ɋL�q����K�v������܂��BMFC �A�v���P�[�V������ document/view
// ���f�����g���Ă���̂ŁA���̏����̓t���[�����[�N�ɂ�莩���I�ɏ�������܂��B

void CCopyTimeExifDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �`��p�̃f�o�C�X �R���e�L�X�g

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// �N���C�A���g�̋�`�̈���̒���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �A�C�R����`�悵�܂��B
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// �V�X�e���́A���[�U�[���ŏ����E�B���h�E���h���b�O���Ă���ԁA
// �J�[�\����\�����邽�߂ɂ������Ăяo���܂��B
HCURSOR CCopyTimeExifDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

// **********************************
// �t�@�C���Q�Ƃ̃{�^�����������Ƃ�
// **********************************
void CCopyTimeExifDlg::OnBtnPickfile() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	CString strTmp1;
	strTmp1.LoadString(STR_BTN_PICKFILE);
	CFileDialog dlg(TRUE, NULL, NULL, OFN_FILEMUSTEXIST|OFN_EXPLORER,strTmp1);
	if(dlg.DoModal()==TRUE)
		SetDlgItemText(IDC_EDIT_FILENAME, dlg.GetPathName());
	
}

int CALLBACK myBrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData);

// **********************************
// �t�H���_�Q�Ƃ̃{�^�����������Ƃ�
// **********************************
void CCopyTimeExifDlg::OnBtnPickfolder() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	char strPathname[MAX_PATH];
	LPITEMIDLIST pidl;
	BROWSEINFO bi;

	// �e�L�X�g�{�b�N�X�ɓ��͂���Ă���e�p�X�𓾂�
	GetDlgItemText(IDC_EDIT_FILENAME, strPathname, MAX_PATH-1);

	// Win95 �΍� �i�Ō�� \ ���t�����t�H���_���͎󂯕t���Ȃ��j
	if(strlen(strPathname)>3)		// ���[�g�t�H���_�Ƌ󔒂͏���
	{
		if(strPathname[strlen(strPathname)-1] == '\\')
			strPathname[strlen(strPathname)-1] = (char)NULL;
	}

	// BROWSEINFO �\���̂̏�����
	ZeroMemory(&bi,sizeof(bi));
	bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_STATUSTEXT;
	bi.lpszTitle = (LPCSTR)"�e�t�H���_�̑I��";
	bi.hwndOwner = m_hWnd;
	bi.lpfn = &myBrowseCallbackProc;	// �����t�H���_�ݒ�̂��߃R�[���o�b�N�֐��ݒ�
	bi.lParam = (LPARAM)strPathname;	// �����t�H���_���p�����[�^�Ƃ��Đݒ�

	pidl = ::SHBrowseForFolder(&bi);
	if(pidl == NULL) return;			// �L�����Z���{�^���������ꂽ
	SHGetPathFromIDList(pidl, strPathname);

	////////////////////////
	// pidl �𖾎��I�ɉ������
	////////////////////////
	IMalloc *imalloc;
	// Shell�̃������Ǘ��I�u�W�F�N�g���擾
	if( ::SHGetMalloc( &imalloc) != NOERROR) imalloc = NULL;
	// �I�u�W�F�N�g�擾�ɐ��������ꍇ�ɊJ������
	if( imalloc != NULL) imalloc->Free((void *)pidl); // ITEMIDLIST���J��

	// ���[�g�t�H���_�̈����B�Ō�� \ �����Ă��邩�ǂ���
	if(strPathname[strlen(strPathname)-1] != '\\')
		strcat(strPathname, "\\");

	// �Ō�� *.* ��ǉ�����
	strcat(strPathname, "*.*");

	// �t�@�C�����̓{�b�N�X���A�b�v�f�[�g����
	SetDlgItemText(IDC_EDIT_FILENAME, strPathname);
	
}

// **********************************
// �t�H���_�I�� SDK �_�C�A���O ::SHBrowseForFolder �p�� �R�[���o�b�N�֐�
// **********************************
int CALLBACK myBrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
{
	// �_�C�A���O�������������Ƃ� �iBFFM_INITIALIZED�j
    if(uMsg == BFFM_INITIALIZED){
		// �t�H���_��I������ �iBFFM_SETSELECTION�j���b�Z�[�W
		if(lpData != NULL)
	        SendMessage(hwnd, BFFM_SETSELECTION, (WPARAM)TRUE, lpData);
    }
    return 0;
}

// **********************************
// ���s�{�^�����������Ƃ�
// **********************************
void CCopyTimeExifDlg::OnBtnCopy() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������

	CString strTmp1, strTmp2;
	struct _fstr fstrPathArray[MAX_PATH_ARRAY];		// �t�@�C�������\�[�g���Ċi�[���邽�߂̔z��
	int nFileCount = 0;								// �Ώۂ̃t�@�C���� 1�`
	char strPathname[MAX_PATH], strFilename[MAX_PATH], strPathOnly[MAX_PATH];
	int i;

	// ���o�b�t�@�̏�����
	strMsg = "";

	// �t�@�C�������̓{�b�N�X����t�@�C������ǂݍ���
	GetDlgItemText(IDC_EDIT_FILENAME, strPathname, MAX_PATH-1);
	if(!strlen(strPathname))
	{	// �t�@�C�������w�肳��Ă��Ȃ��Ƃ�
		strTmp1.LoadString(STR_DLG_NOFILE);
		strTmp2.LoadString(STR_DLG_DLGTITLE);
		MessageBox(strTmp1, strTmp2, MB_OK|MB_ICONINFORMATION);
		return;
	}

	// �p�X���݂̂̊i�[ �iC:\xxx\xxx\�@�� �ŏI�̃��[�g�L���܂ށj
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

	// �t�@�C�������������āA�U�蕪�����[�`�����Ăяo��
	struct _finddata_t finddata;
	long hFind;

	if( (hFind = _findfirst(strPathname, &finddata )) == -1L )
	{	// �Q�Ƃ����t�@�C����������Ȃ��ꍇ
		strTmp1.LoadString(STR_NO_FILE);
		strMsg = strMsg + strTmp1 + "\r\n";
	}
	else
	{
		if(!(finddata.attrib & _A_SUBDIR))
		{	// ��ڂ̃t�@�C���̔���
			strncpy(fstrPathArray[nFileCount].str, finddata.name, MAX_PATH-1);
			nFileCount++;
		}
		while(_findnext( hFind, &finddata ) == 0)
		{	// �����t�@�C����A������
			if(!(finddata.attrib & _A_SUBDIR))
			{
				strncpy(fstrPathArray[nFileCount].str, finddata.name, MAX_PATH-1);
				nFileCount++;
			}
			if(nFileCount >= MAX_PATH_ARRAY) break;
		}
	
		if(nFileCount >= MAX_PATH_ARRAY)
		{	// �t�@�C����������𒴂����ꍇ
			strTmp1.LoadString(STR_ERR_EXCEED_FILE);	// �摜�t�@�C�������������E�𒴂��܂���\r\n�r���܂ŏ������܂�
			strMsg = strMsg + strTmp1 + "\r\n";
		}

		// �t�@�C�����̃\�[�g
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
// ���s���[�h�i�h���b�v�_�E���{�b�N�X�j�ɂ��������āA�@�\�ʂ̊֐��ɐU�蕪����
// **********************************
void CCopyTimeExifDlg::CopyFuncDispatch(char *strPathname, char *strFilename, char *strPathOnly)
{
	CString strTmp1, strTmp2;

	strTmp2 = strFilename;

	switch(m_cmb_mode_ctrl.GetCurSel())
	{
	case 0 : // ���t�̕ύX
		ChangeDate(strPathname, strFilename, strPathOnly);
		break;
	case 1 : // �t�@�C�����̕ύX
		ChangeFname(strPathname, strFilename, strPathOnly);
		break;
	case 2 : // EXIF�f�[�^�̕\��
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

	init_exif_data_struct(&exifData);

	if(strMsg.GetLength() > 1000000) return;	// 1MBytes�ȏ�͕\�����Ȃ�

	strTmp = strFilename;
	strMsg += strTmp + "\r\n";

	::read_exif_data(strPathOnly, strFilename, &exifData);

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
	CFileStatus Fstatus;
	CString strFullPath;

	// strtok (�g�[�N���؂�o��) ��p�̕�����錾
	char str_ext_raw[21];		// �؂�o�����̕�����B�؂�o���ɂ��������Ĕj�󂳂��
	char str_delimit[] = ": ";	// ��؂蕶�� �i�R���� ����� �󔒁j
	char *str_strtok;			// �؂�o���ꂽ���ʕ�����

	struct tm tmTime;

	// ��񃁃b�Z�[�W�̐ݒ� �i�t�@�C�����j
	strTmp = strFilename;
	strMsg += strTmp + " : ";

	init_exif_data_struct(&exifData);

	// EXIF ����ǂݍ���
	if(!::read_exif_data(strPathOnly, strFilename, &exifData))
		return FALSE;

	// EXIF ��莞�ԏ��� tm �\���̂ɑ������
	strncpy(str_ext_raw, exifData.DateTime, 20);	// �j��ɔ����ăR�s�[

	str_strtok = strtok(str_ext_raw, str_delimit);
	tmTime.tm_year = atoi(str_strtok);
	if(tmTime.tm_year < 1970 || tmTime.tm_year > 2032)
	{
		if(tmTime.tm_year < 0 || tmTime.tm_year > 99) return FALSE;
		if(tmTime.tm_year <= 99 && tmTime.tm_year > 50) tmTime.tm_year += 1900;
		else tmTime.tm_year += 2000;
	}
	tmTime.tm_year -= 1900;						// tm_year �� 1900�N����̔N��

	str_strtok = strtok(NULL, str_delimit);
	tmTime.tm_mon = atoi(str_strtok) - 1;		// tm_mon �� 0 �` 11
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

	tmTime.tm_isdst = 0;	// �Ď��ԂłȂ�
	tmTime.tm_wday = 0;		// �_�~�[���Ƃ肠��������Ă���
	tmTime.tm_yday = 0; 	// �_�~�[���Ƃ肠��������Ă���

	strFullPath.Format("%s%s", strPathOnly, strFilename);

	// �t�@�C���̏����擾����
	if(!CFile::GetStatus(strFullPath, Fstatus))
	{
		if(strMsg.GetLength() < 1000000)
		{
			strTmp.LoadString(STR_GETSTATUS_ERR);
			strMsg = strMsg + strTmp + "\r\n";
		}
		return FALSE;
	}

	// ��񃁃b�Z�[�W�̐ݒ� �i���̃^�C���X�^���v�j
	strTmp.Format("%04d:%02d:%02d %02d:%02d:%02d ", Fstatus.m_mtime.GetYear(), Fstatus.m_mtime.GetMonth(),
					Fstatus.m_mtime.GetDay(), Fstatus.m_mtime.GetHour(), Fstatus.m_mtime.GetMinute(), Fstatus.m_mtime.GetSecond());
	strMsg += strTmp;

	// �������o�b�t�@���X�V
	Fstatus.m_ctime = mktime(&tmTime);
	Fstatus.m_atime = mktime(&tmTime);
	Fstatus.m_mtime = mktime(&tmTime);

	// �t�@�C���̏�����������
	try{ CFile::SetStatus(strFullPath, Fstatus); }
	catch(CFileException *e)
	{	// �R�s�[��t�@�C���ɏ������߂Ȃ��Ƃ�
		long dummy = e->m_lOsError;	// �ue �������g���Ă��Ȃ��v�x����������邽�߂̃_�~�[
		strTmp.LoadString(STR_SETSTATUS_ERR);
		strMsg = strMsg + strTmp + "\r\n";
		return FALSE;
	}

	// ��񃁃b�Z�[�W�̐ݒ� �i�V�^�C���X�^���v�j
	strTmp.Format("-> %04d:%02d:%02d %02d:%02d:%02d ", Fstatus.m_mtime.GetYear(), Fstatus.m_mtime.GetMonth(),
					Fstatus.m_mtime.GetDay(), Fstatus.m_mtime.GetHour(), Fstatus.m_mtime.GetMinute(), Fstatus.m_mtime.GetSecond());
	strMsg = strMsg + strTmp + "\r\n";

	return TRUE;
}

BOOL CCopyTimeExifDlg::ChangeFname(char *strPathname, char *strFilename, char *strPathOnly)
{
	CString strTmp;
	struct exif_data exifData;
	CFileStatus Fstatus;
	CString strFullPath, strNewFilename, strNewFullPath;

	// strtok (�g�[�N���؂�o��) ��p�̕�����錾
	char str_ext_raw[21];		// �؂�o�����̕�����B�؂�o���ɂ��������Ĕj�󂳂��
	char str_delimit[] = ": ";	// ��؂蕶�� �i�R���� ����� �󔒁j
	char *str_strtok;			// �؂�o���ꂽ���ʕ�����

	struct tm tmTime;
	CTime tmCtime;

	int i;

	// ��񃁃b�Z�[�W�̐ݒ� �i�t�@�C�����j
	strTmp = strFilename;
	strMsg += strTmp + " : ";

	init_exif_data_struct(&exifData);

	// EXIF ����ǂݍ���
	if(!::read_exif_data(strPathOnly, strFilename, &exifData))
		return FALSE;

	// EXIF ��莞�ԏ��� tm �\���̂ɑ������
	strncpy(str_ext_raw, exifData.DateTime, 20);	// �j��ɔ����ăR�s�[

	str_strtok = strtok(str_ext_raw, str_delimit);
	tmTime.tm_year = atoi(str_strtok);
	if(tmTime.tm_year < 1970 || tmTime.tm_year > 2032)
	{
		if(tmTime.tm_year < 0 || tmTime.tm_year > 99) return FALSE;
		if(tmTime.tm_year <= 99 && tmTime.tm_year > 50) tmTime.tm_year += 1900;
		else tmTime.tm_year += 2000;
	}
	tmTime.tm_year -= 1900;						// tm_year �� 1900�N����̔N��

	str_strtok = strtok(NULL, str_delimit);
	tmTime.tm_mon = atoi(str_strtok) - 1;		// tm_mon �� 0 �` 11
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

	tmTime.tm_isdst = 0;	// �Ď��ԂłȂ�
	tmTime.tm_wday = 0;		// �_�~�[���Ƃ肠��������Ă���
	tmTime.tm_yday = 0; 	// �_�~�[���Ƃ肠��������Ă���

	strFullPath.Format("%s%s", strPathOnly, strFilename);

	// �t�@�C���̏����擾����
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

	// �V�t�@�C�����̍쐬
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
	case 12 : // OLYMPUS �`��
	case 13 : // �������Ȃ�
		strNewFilename = "";
		break;
	default :
		break;
	}

	switch(m_cmb_filename_append_ctrl.GetCurSel())
	{
	case 0 : // �����t�����Ȃ�
		break;
	case 1 : // F�l - �V���b�^�[���x
		strTmp.Format(" %02.2lf-%04.0lf", exifData.FNumber, exifData.ExposureTime);
		strNewFilename += strTmp;
		break;
	case 2 : // ISO - F�l - �V���b�^�[���x
		strTmp.Format(" %04d-%02.2lf-%04.0lf", exifData.ISOSpeedRatings, exifData.FNumber, exifData.ExposureTime);
		strNewFilename += strTmp;
		break;
	case 3 : // F�l  �V���b�^�[���x
		strTmp.Format(" %02.2lf %04.0lf", exifData.FNumber, exifData.ExposureTime);
		strNewFilename += strTmp;
		break;
	case 4 : // ISO  F�l  �V���b�^�[���x
		strTmp.Format(" %04d %02.2lf %04.0lf", exifData.ISOSpeedRatings, exifData.FNumber, exifData.ExposureTime);
		strNewFilename += strTmp;
		break;
	}

	if(strNewFilename == "") strNewFilename = "No Title";

	// �V�t�@�C�������g���Ă��Ȃ����������A�����̏ꍇ "(n)" ��������
	strTmp = strNewFilename + ".jpg";
	for(i=1; i<10000; i++)
	{
		strNewFullPath.Format("%s%s", strPathOnly, strTmp);
		if(!CFile::GetStatus(strNewFullPath, Fstatus)) break;		// �t�@�C�������݂��Ȃ��ꍇ�A������
		strTmp.Format("%s(%d).jpg", strNewFilename, i);
	}

	strNewFilename = strTmp;	// �V�t�@�C����

	// �V�t�@�C���� (�t���p�X)
	strNewFullPath.Format("%s%s", strPathOnly, strTmp);

	// �t�@�C���̏�����������
	try{ CFile::Rename(strFullPath, strNewFullPath); }
	catch(CFileException *e)
	{	// �R�s�[��t�@�C���ɏ������߂Ȃ��Ƃ�
		long dummy = e->m_lOsError;	// �ue �������g���Ă��Ȃ��v�x����������邽�߂̃_�~�[
		strTmp.LoadString(STR_SETSTATUS_ERR);
		strMsg = strMsg + strTmp + "\r\n";
		return FALSE;
	}

	// ��񃁃b�Z�[�W�̐ݒ� �i�t�@�C�����j
	strMsg += "->" + strNewFilename + "\r\n";

	return TRUE;
}


// exif_data �̏�����
void init_exif_data_struct(struct exif_data *dat)
{
	dat->App1Size = 0;
	dat->IfdDirCount = 0;
	dat->SubIfdDirCount = 0;
	// IFD
	strcpy(dat->ImageDescription, "");
	strcpy(dat->Make, "");
	strcpy(dat->Model, "");
	dat->Orientation = 0;
	strcpy(dat->Software, "");
	strcpy(dat->DateTime, "");
	strcpy(dat->Copyright, "");
	//Sub IFD
	dat->ExposureTime = 0;
	dat->FNumber = 0;
	dat->ISOSpeedRatings = 0;
	dat->ExposureProgram = 0;
	strcpy(dat->ExifVersion, "");
	strcpy(dat->DateTimeOriginal, "");
	strcpy(dat->DateTimeDigitized, "");
	dat->ExifImageWidth = 0;
	dat->ExifImageHeight = 0;
}

// �t�@�C������ EXIF �f�[�^��ǂݍ���
BOOL read_exif_data(CString strPathOnly, CString strFilename, struct exif_data *dat)
{
	CString strFullPath;
	CString strTmp1;
	char datData[2048], datData2[2048];
	unsigned short int wordData;
	unsigned int dwordData, dwordData2;
	FILE *in;
	int i;

	strFullPath = strPathOnly + strFilename;

	if((in = fopen((LPCSTR)strFullPath, "rb")) == NULL)
	{	// �t�@�C�����J���Ȃ��ꍇ
		if(strMsg.GetLength() < 1000000)
		{
			strTmp1.LoadString(STR_FILE_CANNOT_OPEN_RB);
			strMsg = strMsg + "\r\n" + strTmp1 + "\r\n";
		}
		return FALSE;
	}

	// CHECK EXIF HEADER
	if(fread(datData, 1, 4, in) < 4)
	{
		if(strMsg.GetLength() < 1000000)
		{
			strTmp1.LoadString(STR_FILE_ABNORMAL_END);
			strMsg = strMsg + "\r\n" + strTmp1+ "\r\n";
		}
		fclose(in);
		return FALSE;
	}
	datData2[0] = (char)0xff;
	datData2[1] = (char)0xd8;
	datData2[2] = (char)0xff;
	datData2[3] = (char)0xe1;
	if(!CheckBinaryData(datData, datData2, 4))
	{	// EXIF �w�b�_�̊J�n�l���ُ�
		if(strMsg.GetLength() < 1000000)
		{
			strTmp1.LoadString(STR_EXIF_DATA_ERR_1);
			strMsg = strMsg + "\r\n" + strTmp1+ "\r\n";
		}
		fclose(in);
		return FALSE;
	}

	// APP1 SIZE
	if(fread(&wordData, 2, 1, in) < 1)
	{	// �ǂݍ��ݕs�\�̂Ƃ�
		if(strMsg.GetLength() < 1000000)
		{
			strTmp1.LoadString(STR_FILE_ABNORMAL_END);
			strMsg = strMsg + "\r\n" + strTmp1+ "\r\n";
		}
		fclose(in);
		return FALSE;
	}
	dat->App1Size = wordData;

	// CHECK EXIF HEADER
	if(fread(datData, 1, 6, in) < 6)
	{
		if(strMsg.GetLength() < 1000000)
		{
			strTmp1.LoadString(STR_FILE_ABNORMAL_END);
			strMsg = strMsg + "\r\n" + strTmp1+ "\r\n";
		}
		fclose(in);
		return FALSE;
	}
	datData2[0] = (char)0x45;	// E
	datData2[1] = (char)0x78;	// x
	datData2[2] = (char)0x69;	// i
	datData2[3] = (char)0x66;	// f
	datData2[4] = (char)0;
	datData2[5] = (char)0;
	if(!CheckBinaryData(datData, datData2, 6))
	{	// EXIF �}�[�J�[�l���ُ�
		if(strMsg.GetLength() < 1000000)
		{
			strTmp1.LoadString(STR_EXIF_DATA_ERR_2);
			strMsg = strMsg + "\r\n" + strTmp1+ "\r\n";
		}
		fclose(in);
		return FALSE;
	}

	// CHECK TIFF HEADER
	if(fread(datData, 1, 8, in) < 8)
	{
		if(strMsg.GetLength() < 1000000)
		{
			strTmp1.LoadString(STR_FILE_ABNORMAL_END);
			strMsg = strMsg + "\r\n" + strTmp1+ "\r\n";
		}
		fclose(in);
		return FALSE;
	}
	datData2[0] = (char)0x49;	// I (Intel) , (if Motorola 'M')
	datData2[1] = (char)0x49;	// I (Intel) , (if Motorola 'M')
	datData2[2] = (char)0x2a;	// (if Motorola 0x00)
	datData2[3] = (char)0x00;	// (if Motorola 0x2a)
	datData2[4] = (char)0x08;
	datData2[5] = (char)0;
	datData2[6] = (char)0;
	datData2[7] = (char)0;
	if(!CheckBinaryData(datData, datData2, 8))
	{	// TIFF �}�[�J�[�l���ُ�
		if(strMsg.GetLength() < 1000000)
		{
			strTmp1.LoadString(STR_EXIF_DATA_ERR_3);
			strMsg = strMsg + "\r\n" + strTmp1+ "\r\n";
		}
		fclose(in);
		return FALSE;
	}

	// Exif IFD Directry Entries Count
	if(fread(&wordData, 2, 1, in) < 1)
	{
		if(strMsg.GetLength() < 1000000)
		{
			strTmp1.LoadString(STR_FILE_ABNORMAL_END);
			strMsg = strMsg + "\r\n" + strTmp1+ "\r\n";
		}
		fclose(in);
		return FALSE;
	}
	dat->IfdDirCount = wordData;


	// Pharse IFD Data
	unsigned short int ifd_TagID, ifd_Format;
	unsigned int ifd_DataCount, ifd_Data;
	long cur_filepoint, ifd_SubPtr = 0;

	cur_filepoint = ftell(in);

	for(i=0; i<dat->IfdDirCount; i++)
	{
		if(fseek(in, cur_filepoint, SEEK_SET))
		{
			if(strMsg.GetLength() < 1000000)
			{
				strTmp1.LoadString(STR_FILE_ABNORMAL_END);
				strMsg = strMsg + "\r\n" + strTmp1+ "\r\n";
			}
			fclose(in);
			return FALSE;
		}

		fread(&wordData, 2, 1, in);
		ifd_TagID = wordData;
		fread(&wordData, 2, 1, in);
		ifd_Format = wordData;
		fread(&dwordData, 4, 1, in);
		ifd_DataCount = dwordData;
		if(fread(&dwordData, 4, 1, in) < 1)
		{	// �ǂݍ��݃G���[
			if(strMsg.GetLength() < 1000000)
			{
				strTmp1.LoadString(STR_FILE_ABNORMAL_END);
				strMsg = strMsg + "\r\n" + strTmp1+ "\r\n";
			}
			fclose(in);
			return FALSE;
		}
		ifd_Data = dwordData;

		cur_filepoint = ftell(in);		// �t�@�C���|�C���^�̈ړ��ɔ����ĕۑ�

		// TagID�ɂ��������āA�f�[�^��ǂݏo���B
		// �f�[�^�I�t�Z�b�g�� "II" �̈ʒu����B(0x0c)
		switch(ifd_TagID)
		{
		case 0x010e :
			if(ifd_Format != 2) break;
			fseek(in, 0x0c + ifd_Data, SEEK_SET);
			fread(datData, 1, ifd_DataCount, in);
			datData[ifd_DataCount] = (char)0;
			strncpy(dat->ImageDescription, datData, 254);
			break;
		case 0x010f :
			if(ifd_Format != 2) break;
			fseek(in, 0x0c + ifd_Data, SEEK_SET);
			fread(datData, 1, ifd_DataCount, in);
			datData[ifd_DataCount] = (char)0;
			strncpy(dat->Make, datData, 254);
			break;
		case 0x0110 :
			if(ifd_Format != 2) break;
			fseek(in, 0x0c + ifd_Data, SEEK_SET);
			fread(datData, 1, ifd_DataCount, in);
			datData[ifd_DataCount] = (char)0;
			strncpy(dat->Model, datData, 254);
			break;
		case 0x0112 :
			if(ifd_Format != 3) break;
			dat->Orientation = ifd_Data;
			break;
		case 0x0131 :
			if(ifd_Format != 2) break;
			fseek(in, 0x0c + ifd_Data, SEEK_SET);
			fread(datData, 1, ifd_DataCount, in);
			datData[ifd_DataCount] = (char)0;
			strncpy(dat->Software, datData, 254);
			break;
		case 0x0132 :
			if(ifd_Format != 2) break;
			fseek(in, 0x0c + ifd_Data, SEEK_SET);
			fread(datData, 1, ifd_DataCount, in);
			datData[ifd_DataCount] = (char)0;
			strncpy(dat->DateTime, datData, 20);
			break;
		case 0x8298 :
			if(ifd_Format != 2) break;
			fseek(in, 0x0c + ifd_Data, SEEK_SET);
			fread(datData, 1, ifd_DataCount, in);
			datData[ifd_DataCount] = (char)0;
			strncpy(dat->Copyright, datData, 20);
			break;
		case 0x8769 :		// Exif SUB IFD �����݂���ꍇ�A�|�C���^�𓾂�
			if(ifd_Format != 4) break;
			ifd_SubPtr = ifd_Data;
			break;
		}



	}

	// Sub IFD �̓ǂݍ���
	if(ifd_SubPtr)
	{
		if(fseek(in, 0x0c + ifd_SubPtr, SEEK_SET))
		{
			if(strMsg.GetLength() < 1000000)
			{
				strTmp1.LoadString(STR_FILE_ABNORMAL_END);
				strMsg = strMsg + "\r\n" + strTmp1+ "\r\n";
			}
			fclose(in);
			return FALSE;
		}

		// Exif IFD Directry Entries Count
		fread(&wordData, 2, 1, in);
		dat->SubIfdDirCount = wordData;

		cur_filepoint = ftell(in);

		for(i=0; i<dat->SubIfdDirCount; i++)
		{
			fseek(in, cur_filepoint, SEEK_SET);

			fread(&wordData, 2, 1, in);
			ifd_TagID = wordData;
			fread(&wordData, 2, 1, in);
			ifd_Format = wordData;
			fread(&dwordData, 4, 1, in);
			ifd_DataCount = dwordData;
			if(fread(&dwordData, 4, 1, in) < 1)
			{	// �ǂݍ��݃G���[
				if(strMsg.GetLength() < 1000000)
				{
					strTmp1.LoadString(STR_FILE_ABNORMAL_END);
					strMsg = strMsg + "\r\n" + strTmp1+ "\r\n";
				}
				fclose(in);
				return FALSE;
			}
			ifd_Data = dwordData;

			cur_filepoint = ftell(in);		// �t�@�C���|�C���^�̈ړ��ɔ����ĕۑ�

			// TagID�ɂ��������āA�f�[�^��ǂݏo���B
			// �f�[�^�I�t�Z�b�g�� "II" �̈ʒu����B(0x0c)

			switch(ifd_TagID)
			{
			case 0x829a :
				if(ifd_Format != 5) break;
				fseek(in, 0x0c + ifd_Data, SEEK_SET);
				fread(&dwordData, 4, 1, in);	// ���q
				fread(&dwordData2, 4, 1, in);	// ����
				if(dwordData == 0) break;	// �[�����Z���
				dat->ExposureTime = (double)dwordData2 / (double)dwordData; // 1/n �� n ���i�[
				break;
			case 0x829d :
				if(ifd_Format != 5) break;
				fseek(in, 0x0c + ifd_Data, SEEK_SET);
				fread(&dwordData, 4, 1, in);	// ���q
				fread(&dwordData2, 4, 1, in);	// ����
				if(dwordData2 == 0) break;	// �[�����Z���
				dat->FNumber = (double)dwordData / (double)dwordData2;
				break;
			case 0x8827 :
				if(ifd_Format != 3) break;
				dat->ISOSpeedRatings = ifd_Data;
				break;
			case 0x8822 :
				if(ifd_Format != 3) break;
				dat->ExposureProgram = ifd_Data;
				break;
			case 0x9000 :
				if(ifd_Format != 7) break;
				fseek(in, -4, SEEK_CUR);
				fread(datData, 1, 4, in);
				datData[4] = (char)0;
				strncpy(dat->ExifVersion, datData, 4);
				break;
			case 0x9003 :
				if(ifd_Format != 2) break;
				fseek(in, 0x0c + ifd_Data, SEEK_SET);
				fread(datData, 1, ifd_DataCount, in);
				datData[ifd_DataCount] = (char)0;
				strncpy(dat->DateTimeOriginal, datData, 20);
				break;
			case 0x9004 :
				if(ifd_Format != 2) break;
				fseek(in, 0x0c + ifd_Data, SEEK_SET);
				fread(datData, 1, ifd_DataCount, in);
				datData[ifd_DataCount] = (char)0;
				strncpy(dat->DateTimeDigitized, datData, 20);
				break;
			case 0xa002 :
				if(ifd_Format == 3) 
					dat->ExifImageWidth = (unsigned short)ifd_Data;
				if(ifd_Format == 4) 
					dat->ExifImageWidth = ifd_Data;
				break;
			case 0xa003 :
				if(ifd_Format == 3) 
					dat->ExifImageHeight = (unsigned short)ifd_Data;
				if(ifd_Format == 4) 
					dat->ExifImageHeight = ifd_Data;
				break;
			}
		}

	}

	fclose(in);

	return TRUE;
}

// �o�C�i���f�[�^�ɕs��v������ꍇ�AFALSE ��Ԃ�
BOOL CheckBinaryData(char *data1, char *data2, long size)
{
	long i;

	for(i=0; i<size; i++)
	{
		if(data1[i] != data2[i]) return FALSE;
	}
	return TRUE;
}


// **********************************
// �\�[�e�B���O
// **********************************
void SortArray(_fstr *fstrPathArray, int nFileCount, int nMode)
{
	int i, j;
	CString strTmp;

	////////////////////////
	// �\�[�g���[�h��؂�ւ���
	////////////////////////
	if(nMode == 0)
	{	// �t�@�C�����Ń\�[�g�i�����j
		for(i=0; i<nFileCount; i++)
		{
			for(j=i; j<nFileCount; j++)
			{
				if(stricmp((const char*)fstrPathArray[i].str, (const char*)fstrPathArray[j].str) > 0)
				{
					// �t�@�C�����̌���
					strTmp = fstrPathArray[i].str;
					strcpy(fstrPathArray[i].str, fstrPathArray[j].str);
					strcpy(fstrPathArray[j].str, strTmp);
				}
			}
		}
	}
	else if(nMode == 1)
	{	// �t�@�C�����Ń\�[�g�i�~���j
		for(i=0; i<nFileCount; i++)
		{
			for(j=i; j<nFileCount; j++)
			{
				if(stricmp((const char*)fstrPathArray[i].str, (const char*)fstrPathArray[j].str) < 0)
				{
					// �t�@�C�����̌���
					strTmp = fstrPathArray[i].str;
					strcpy(fstrPathArray[i].str, fstrPathArray[j].str);
					strcpy(fstrPathArray[j].str, strTmp);
				}
			}
		}
	}
}


void CCopyTimeExifDlg::OnBtnAbout() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	CString strTmp1, strTmp2;

	strTmp1.LoadString(STR_DLG_DLGTITLE);
	strTmp2.LoadString(STR_DLG_ABOUTBOX);

	MessageBox(strTmp2, strTmp1, MB_OK|MB_ICONINFORMATION);
}
