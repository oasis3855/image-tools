// CopyTimeExifDlg.cpp : �C���v�������e�[�V���� �t�@�C��
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
	char strExifErrMessage[MAX_EXIF_ERR_MESSAGE_SIZE+1];

	init_exif_data_struct(&exifData);

	if(strMsg.GetLength() > 1000000) return;	// 1MBytes�ȏ�͕\�����Ȃ�

	strTmp = strFilename;
	strMsg += strTmp + "\r\n";

	::read_exif_data(strPathOnly, strFilename, &exifData, strExifErrMessage);

	if(strlen(strExifErrMessage))
	{	// �G���[���b�Z�[�W������ꍇ
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
	if(!::read_exif_data(strPathOnly, strFilename, &exifData, strExifErrMessage))
	{
		if(strlen(strExifErrMessage))
		{	// �G���[���b�Z�[�W������ꍇ
			strMsg = strMsg + strExifErrMessage + "\r\n";
		}

		return FALSE;
	}

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
	char strExifErrMessage[MAX_EXIF_ERR_MESSAGE_SIZE+1];
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
	if(!::read_exif_data(strPathOnly, strFilename, &exifData, strExifErrMessage))
	{
		if(strlen(strExifErrMessage))
		{	// �G���[���b�Z�[�W������ꍇ
			strMsg = strMsg + strExifErrMessage + "\r\n";
		}

		return FALSE;
	}

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

void CCopyTimeExifDlg::OnBtnAbout() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	CString strTmp1, strTmp2;

	strTmp1.LoadString(STR_DLG_DLGTITLE);
	strTmp2.LoadString(STR_DLG_ABOUTBOX);

	MessageBox(strTmp2, strTmp1, MB_OK|MB_ICONINFORMATION);
}

