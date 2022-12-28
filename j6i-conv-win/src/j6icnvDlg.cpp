// j6icnvDlg.cpp : �C���v�������e�[�V���� �t�@�C��
// ���C���_�C�A���O

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
// �O���[�o���֐�
// ************************************************

// BCD ���l��^����ƒʏ�̐��l�ɒ����ĕԂ�
unsigned int BCDtoINT(unsigned char bcd)
{
	unsigned int i,j;
	i = bcd;
	j = ((i & 0xf0) >> 4) * 10;
	j += bcd & 0x0f;
	return j;
}

// ************************************************
// �o�[�W�������_�C�A���O
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
		// ���b�Z�[�W �n���h��������܂���B
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CJ6icnvDlg �_�C�A���O
// ���C���_�C�A���O

CJ6icnvDlg::CJ6icnvDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CJ6icnvDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CJ6icnvDlg)
		// ����: ���̈ʒu�� ClassWizard �ɂ���ă����o�̏��������ǉ�����܂��B
	//}}AFX_DATA_INIT
	// ����: LoadIcon �� Win32 �� DestroyIcon �̃T�u�V�[�P���X��v�����܂���B
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CJ6icnvDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CJ6icnvDlg)
		// ����: ���̏ꏊ�ɂ� ClassWizard �ɂ���� DDX �� DDV �̌Ăяo�����ǉ�����܂��B
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
// CJ6icnvDlg ���b�Z�[�W �n���h��

BOOL CJ6icnvDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// "�o�[�W�������..." ���j���[���ڂ��V�X�e�� ���j���[�֒ǉ����܂��B

	// IDM_ABOUTBOX �̓R�}���h ���j���[�͈̔͂łȂ���΂Ȃ�܂���B
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
	
	// ���̃_�C�A���O�p�̃A�C�R����ݒ肵�܂��B�t���[�����[�N�̓A�v���P�[�V�����̃��C��
	// �E�B���h�E���_�C�A���O�łȂ����͎����I�ɐݒ肵�܂���B
	SetIcon(m_hIcon, TRUE);			// �傫���A�C�R����ݒ�
	SetIcon(m_hIcon, FALSE);		// �������A�C�R����ݒ�
	
	// TODO: ���ʂȏ��������s�����͂��̏ꏊ�ɒǉ����Ă��������B

	// ���W�X�g������f�[�^��ǂݍ���
	is_confirm_overwrite = AfxGetApp()->GetProfileInt("Settings","cnf_ovwr",1);
	is_use_ext_viewer = AfxGetApp()->GetProfileInt("Settings","use_extvw",0);
	is_param_vw = AfxGetApp()->GetProfileInt("Settings","param_vw",1);
	is_outpath = AfxGetApp()->GetProfileInt("Settings","sw_outpath",0);
	ext_viewer = AfxGetApp()->GetProfileString("Settings","extvw","");
	OutPath = AfxGetApp()->GetProfileString("Settings","outpath","");

	// �R�}���h���C���̉��
	if(AfxGetApp()->m_lpCmdLine[0] != (char)NULL)
	{	// �R�}���h���C���̂P�߂���̓t�@�C���Ƃ��Ĉ���
		SrcFname = AfxGetApp()->m_lpCmdLine;	// argv[1]
		// ̧�ٖ��e�L�X�g�{�b�N�X�Ƀt�@�C�������Z�b�g
		this->SetDlgItemText(IDC_TXT_FNAME, AfxGetApp()->m_lpCmdLine);

		if(is_param_vw)
		{	// �摜���R���o�[�g���A�\������
			SetDlgItemText(IDC_TXT_MES, "");	// ү���޴ر�̸ر
			CnvMain();	// �R���o�[�g
			// �摜��\������O���v���O���������s
			if(_spawnl(_P_NOWAIT, ext_viewer, ext_viewer, DstFname, NULL)<0)
				DspMes("�O�� Viewer �����s�ł��Ȃ�");
			// �R�}���h���C��������
			AfxGetApp()->m_lpCmdLine[0] = (char)NULL;
		}
	}

	return TRUE;  // TRUE ��Ԃ��ƃR���g���[���ɐݒ肵���t�H�[�J�X�͎����܂���B
}

// �V�X�e�����j���[�̏���
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

// �����_�C�A���O�{�b�N�X�ɍŏ����{�^����ǉ�����Ȃ�΁A�A�C�R����`�悷��
// �R�[�h���ȉ��ɋL�q����K�v������܂��BMFC �A�v���P�[�V������ document/view
// ���f�����g���Ă���̂ŁA���̏����̓t���[�����[�N�ɂ�莩���I�ɏ�������܂��B

void CJ6icnvDlg::OnPaint() 
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
HCURSOR CJ6icnvDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

// J6I �t�@�C���̑I��
void CJ6icnvDlg::OnBtnSel() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	CFileDialog dlg(TRUE, "J6I", NULL, OFN_HIDEREADONLY|OFN_FILEMUSTEXIST|OFN_EXPLORER,
		"J6I ̧��|*.J6I||");
	dlg.m_ofn.lpstrTitle = "�ϊ����� J6I �Ұ��̧�ق�I��ł�������";

	if(dlg.DoModal() != IDOK) return;

	SrcFname = dlg.GetPathName();
	// ̧�ٖ��e�L�X�g�{�b�N�X�Ƀt�@�C�������Z�b�g
	this->SetDlgItemText(IDC_TXT_FNAME,dlg.GetPathName());

}

// �R���o�[�g�{�^���������ꂽ��
void CJ6icnvDlg::OnBtnCnv() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	SetDlgItemText(IDC_TXT_MES, "");	// ү���޴ر�̸ر
	CnvMain();	// �R���o�[�g
}

// J6I �� JPG �ɃR���o�[�g����֐�
void CJ6icnvDlg::CnvMain(void)
{
	CFile in,out;		// ���o�̓t�@�C���n���h��
	CFileException fileException;	// �t�@�C���A�N�Z�X�̃G���[�g���b�v�֐�
	void *buf;			// �t�@�C���R�s�[�̎��̃o�b�t�@
	header hd;			// �w�b�_�̈��ǂݍ��ރo�b�t�@
	int i, is_enable_date;
	unsigned long j;	// �t�@�C���|�C���^

	if(SrcFname == "") return;		// ���̓t�@�C�������Ȃ��ꍇ

	// �p�X�𕪊����ďo�̓t�@�C�������쐬����
	char s_drv[_MAX_DRIVE], s_dir[_MAX_DIR], s_body[_MAX_FNAME], s_full[_MAX_DRIVE+_MAX_DIR+_MAX_FNAME+4];
	_splitpath((LPCSTR)SrcFname, s_drv, s_dir, s_body, NULL);
	// �o�̓p�X�̎w��
	if(is_outpath)
	{	// �o�̓p�X�Œ�̎�
		sprintf(s_full, "%s%s.jpg", (LPCSTR)OutPath, s_body);
	}
	else sprintf(s_full, "%s%s%s.jpg", s_drv, s_dir, s_body);
	DstFname = s_full;

	FnameBody = s_body;


	DspMes("%s �� ���ްĂ��J�n���܂�", FnameBody);
	// �㏑���̊m�F
	if(is_confirm_overwrite)
	{
		CFileStatus status;
		if(CFile::GetStatus(DstFname, status))
		{	// JPG �t�@�C�������݂��鎞
			CString tmpStr = FnameBody + ".jpg ̧�ق����݂��܂�\n�㏑�����܂���";
			if(MessageBox(tmpStr, "�㏑���̊m�F", MB_YESNO|MB_ICONQUESTION) != IDYES)
			{
				DspMes("���ްĂ��܂���ł���");
				return;
			}
			DspMes("̧�� %s.jpg ���㏑�����܂�", FnameBody);
		}
	}

	// ���o�͑o���̃t�@�C�����J��
	if(!in.Open( SrcFname, CFile::modeRead, &fileException))
	{
		DspMes("J6I ̧�� %s ���ǂݍ��߂Ȃ�", SrcFname);
		return;
	}
	if(!out.Open( DstFname, CFile::modeWrite|CFile::modeCreate, &fileException))
	{
		DspMes("JPG ̧�� %s ���쐬�ł��Ȃ�", DstFname);
		return;
	}
	// J6I ̧�ق���w�b�_��ǂݏo��
	if(in.Read(&hd, sizeof(header)) < sizeof(header))
	{
		DspMes("J6I ̧�� ͯ�ޗ̈�װ");
		out.Close();
		in.Close();
		return;
	}
	// �޲ĕ��т̓���ւ�
	if(!hd.endian)
	{
		j = hd.startaddr;
		hd.startaddr = ((j & 0xff000000) >> 24) | ((j & 0x00ff0000) >> 8) | 
			((j & 0x0000ff00) << 8) | ((j & 0x000000ff) << 24);
		j = hd.endaddr;
		hd.endaddr = ((j & 0xff000000) >> 24) | ((j & 0x00ff0000) >> 8) | 
			((j & 0x0000ff00) << 8) | ((j & 0x000000ff) << 24);
	}
	// �w�b�_�̈�̌���
	if(hd.head != (unsigned char)0x80 || hd.dummy1[0] != (unsigned char)0x3e)
	{
		DspMes("J6I ̧��ͯ�ނ� JPG �ް��̒����������Ă��Ȃ�");
		out.Close();
		in.Close();
		return;
	}
	// �w�b�_�̈�̌���(�������i�[����Ă��邩�ǂ���)
	if(hd.enabledate == (unsigned char)0x81) is_enable_date = 1;
	else
	{
		is_enable_date = 0;
		DspMes("J6I ̧��ͯ�ނɎ�����񂪖���");
	}

	// ���̕\��
	CString str;
	if(is_enable_date)
	{
		str.Format("�@�@%02u�N%02u��%02u�� %02u��%02u��%02u�b\r\n�@�@%Lu �޲� (ͯ�� %Lu �޲�) �ް�ޮ� %02d.%02d ���� %02X",
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
		str.Format("�@�@%Lu �޲� (ͯ�� %Lu �޲�) �ް�ޮ� %02d.%02d ���� %02X",
			hd.endaddr - hd.startaddr + 1,
			hd.startaddr,
			hd.ver_h, hd.ver_l, hd.attr);
	}
	DspMes(str);
	// J6I �t�@�C���̃f�[�^�̂���ꏊ�܂ł̃V�[�N
	if((ULONG)in.Seek(hd.startaddr, CFile::begin) < hd.startaddr)
	{
		DspMes("J6I ̧�� ͯ�ޗ̈�װ");
		out.Close();
		in.Close();
		return;
	}

	// �R�s�[����o�b�t�@�̊m��
	if((buf = malloc(1024*10+1)) == NULL)
	{
		DspMes("��߰�p�ޯ̧�̈� 10 kBytes ���m�ۂł��Ȃ�");
		return;
	}
	
	j = hd.endaddr - hd.startaddr + 1;	// JPG �f�[�^���̌v�Z
	// �f�[�^�̃R�s�[
	do{
		try
		{
			i = in.Read(buf, 1024*10 < j ? 1024*10 : j);
		}
		catch(CFileException *e)
		{	// �t�@�C���A�N�Z�X�̃G���[���g���b�v
			if(e->m_cause == CFileException::accessDenied) DspMes("J6I̧�ق̱����֎~");
			DspMes("J6I ̧�� �ǂݍ��ݴװ");
			break;
		}
		try
		{
			out.Write(buf, i);
		}
		catch(CFileException *e)
		{	// �t�@�C���A�N�Z�X�̃G���[���g���b�v
			if(e->m_cause == CFileException::diskFull) DspMes("�ި���������ς�");
			DspMes("JPG ̧�� �������ݴװ");
			break;
		}
		j -= i;
	}while(i && j>0);

	out.Close();
	in.Close();

	if(is_enable_date)
	{
		// JPG ̧�ق̓��t�ݒ�
		CFileStatus status;
		if(!CFile::GetStatus(DstFname, status))
		{
			DspMes("JPG ̧�� %s �̓��t�ǂݍ��ݴװ", DstFname);
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
				DspMes("JPG ̧�� %s �ɱ����s�\", DstFname);
			DspMes("JPG ̧�� %s �̓��t�������ݴװ", DstFname);
			return;
		}
	}

	DspMes("%s ��������", FnameBody);

	free(buf);

	return;
}

// ���b�Z�[�W�G���A�ɕ������\������
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

// �@�\�ݒ�_�C�A���O���J��
void CJ6icnvDlg::OnBtnProf() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	CProfDlg profDlg;

	// �_�C�A���O�{�b�N�X�̍��ڂ�ݒ�
	profDlg.m_chk_ovwr = (BOOL)is_confirm_overwrite;
	profDlg.m_chk_ext = (BOOL)is_use_ext_viewer;
	profDlg.m_paramvw = (BOOL)is_param_vw;
	profDlg.m_outpath = (BOOL)is_outpath;
	profDlg.m_txt_extviewer = ext_viewer;
	profDlg.m_txt_outpath = OutPath;

	if (profDlg.DoModal() == IDOK)
	{
		// �_�C�A���O�̓��e�����W�X�g���ɕۑ�
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
		// ���W�X�g��������ϐ��Ɋi�[������
		is_confirm_overwrite = AfxGetApp()->GetProfileInt("Settings","cnf_ovwr",1);
		is_use_ext_viewer = AfxGetApp()->GetProfileInt("Settings","use_extvw",0);
		is_param_vw = AfxGetApp()->GetProfileInt("Settings","param_vw",1);
		is_outpath = AfxGetApp()->GetProfileInt("Settings","sw_outpath",0);
		ext_viewer = AfxGetApp()->GetProfileString("Settings","extvw","");
		OutPath = AfxGetApp()->GetProfileString("Settings","outpath","");
	}
}

// �ϊ���̉摜��\������
void CJ6icnvDlg::OnBtnShw() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	if(SrcFname =="")
	{
		DspMes("�܂��A�Ұ��̧�ق�I�����Ă�������");
		return;
	}
	CFileStatus status;
	if(!CFile::GetStatus(DstFname, status))
	{	// JPG �t�@�C�������݂��Ȃ���
		OnBtnCnv();
	}
	if(_spawnl(_P_NOWAIT, ext_viewer, ext_viewer, DstFname, NULL)<0)
		DspMes("�O�� Viewer �����s�ł��Ȃ�");
}

// �I�񂾃t�H���_�̉��̂��ׂẴt�@�C����A�����ĕϊ�����
void CJ6icnvDlg::OnBtnSeq() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	CFileDialog dlg(TRUE, "J6I", "����̧��.J6I", OFN_HIDEREADONLY|OFN_PATHMUSTEXIST|OFN_EXPLORER,
		"J6I ̧��|*.J6I|���ׂĂ�̧��|*.*||");
	dlg.m_ofn.lpstrTitle = "�ϊ����� J6I �Ұ��̧�ق��܂�ł���̫��ނ��w�肵�ĉ�����";

	if(dlg.DoModal() != IDOK) return;

	SetDlgItemText(IDC_TXT_MES, "");	// ү���޴ر�̸ر

	CString SrcFinfPath = dlg.GetPathName();

	// ���̓t�@�C����(���C���h�J�[�h)�̐���
	char s_drv[_MAX_DRIVE], s_dir[_MAX_DIR], s_body[_MAX_FNAME], s_full[_MAX_DRIVE+_MAX_DIR+_MAX_FNAME+4];
	_splitpath((LPCSTR)SrcFinfPath, s_drv, s_dir, s_body, NULL);
	sprintf(s_full, "%s%s*.j6i", s_drv, s_dir);

	struct _finddata_t finddata;
	long hFind;
    if( (hFind = _findfirst(s_full, &finddata )) == -1L )
	{
		DspMes("���ްđΏ�̧�ق�������Ȃ�");
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
