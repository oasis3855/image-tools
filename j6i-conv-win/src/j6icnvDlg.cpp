// j6icnvDlg.cpp : �C���v�������e�[�V���� �t�@�C��
// ���C���_�C�A���O ����� �o�[�W�������_�C�A���O

#include "stdafx.h"
#include "j6icnv.h"
#include "j6icnvDlg.h"
#include "j6iProfDlg.h"
#include "GlobalFunc.h"
#include <stdio.h>
#include <process.h>	// spawn
#include <io.h>			// findfirst
#include <shlobj.h>		// �t�H���_���ʃ_�C�A���O

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


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
	m_chk_year = FALSE;
	m_txt_year = 0;
	//}}AFX_DATA_INIT
	// ����: LoadIcon �� Win32 �� DestroyIcon �̃T�u�V�[�P���X��v�����܂���B
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
	CString SrcFname;	// ���̓t�@�C�����i�_�C�A���O���ڂ���擾����j
	CString DstPath;	// �o�̓t�@�C�����i�p�X�j
	CString DstFname;	// �o�̓t�@�C�����i�p�X�j
	CString FnameBody;	// �g���q�Ȃ��̃t�@�C����

	// �_�C�A���O���ڂ̏����ݒ�
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

	// �t�@�C�������N���I�v�V�����Ƃ��ēn���ꂽ�Ƃ��A�t�@�C�����R���o�[�g����
	if(theApp->m_shell_j6i_fname != (char)NULL)
	{	// �����Ƃ��ăt�@�C�������w�肳��Ă���ꍇ
		SrcFname = theApp->m_shell_j6i_fname;
		// ̧�ٖ��e�L�X�g�{�b�N�X�Ƀt�@�C�������Z�b�g
		SetDlgItemText(IDC_TXT_FNAME, SrcFname);

		// �o�̓p�X�̎w��
		if(theApp->m_outpath)
		{	// �o�̓p�X�Œ�̎�
			DstPath = theApp->m_outpath_fname;
		}
		else DstPath = "";


		if(theApp->m_directview)
		{	// �摜���R���o�[�g���A�\������
			SetDlgItemText(IDC_TXT_MES, "");	// ү���޴ر�̸ر
			// �t�@�C���̃R���o�[�g
			if(theApp->m_year)
				::ConvertJ6I((LPCSTR)SrcFname, (LPCSTR)DstPath, m_tmpMsg, m_hWnd, theApp->m_year_data, theApp->m_confirm_ovwr);
			else
				::ConvertJ6I((LPCSTR)SrcFname, (LPCSTR)DstPath, m_tmpMsg, m_hWnd, 0, theApp->m_confirm_ovwr);
			m_last_j6i_file = SrcFname;
			// �摜��\������O���v���O���������s
			OnBtnShw();
			// �v���r���[��JPG�t�@�C������������
			if(theApp->m_delete) 
			{
				DstFname = ::GetJ6iName(SrcFname, DstPath);
				::remove(DstFname);
			}
			DspMes(m_tmpMsg);

		}
	}

	return TRUE;  // TRUE ��Ԃ��ƃR���g���[���ɐݒ肵���t�H�[�J�X�͎����܂���B
}

// �V�X�e�����j���[�̏���
// �����ł́A�uAbout�v�_�C�A���O�̕\�����j���[�̏���������
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

// �_�C�A���O���j�������Ƃ��ɂ́A�w���v�t�@�C�������
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

// �u�I���v�{�^���������ꂽ�Ƃ��BJ6I �t�@�C���̑I��
void CJ6icnvDlg::OnBtnSel() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������

	if(GetCheckedRadioButton(IDC_RADIO_SELMODE_1, IDC_RADIO_SELMODE_2) == 
		IDC_RADIO_SELMODE_1)
	{	// �t�@�C����I������
	
		CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY|OFN_FILEMUSTEXIST|OFN_EXPLORER,
			"J6I �t�@�C��|*.J6I|���ׂẴt�@�C��|*.*||");
		dlg.m_ofn.lpstrTitle = "�ϊ����� J6I �C���[�W�t�@�C���̑I��";

		if(dlg.DoModal() != IDOK) return;

		// ̧�ٖ��e�L�X�g�{�b�N�X�Ƀt�@�C�������Z�b�g
		SetDlgItemText(IDC_TXT_FNAME,dlg.GetPathName());
	}
	else
	{	// �t�H���_��I������
		char s_full[MAX_PATH];
		char sTitle[] = "�ϊ����t�H���_�̑I��";
		LPITEMIDLIST pidl;
		BROWSEINFO bi;
		ZeroMemory(&bi,sizeof(bi));
		bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_STATUSTEXT;
		bi.lpszTitle = sTitle;
		bi.hwndOwner = m_hWnd;

		pidl = ::SHBrowseForFolder(&bi);
		if(pidl == NULL) return;
		SHGetPathFromIDList(pidl, s_full);


		// ���[�g�t�H���_�̈����B�Ō�� \ �����Ă��邩�ǂ���
		if(s_full[strlen(s_full)-1] != '\\')
			strcat(s_full, "\\");
		// �t�@�C�����̓{�b�N�X���A�b�v�f�[�g����
		SetDlgItemText(IDC_TXT_FNAME, s_full);
	}
}

// �R���o�[�g�{�^���������ꂽ��
void CJ6icnvDlg::OnBtnCnv() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	CString SrcFname;	// ���̓t�@�C�����i�_�C�A���O���ڂ���擾����j
	CString SrcPath;	// ���̓t�@�C���̃p�X��
	CString DstPath;	// �o�̓t�@�C�����i�p�X���j
	CString FnameBody;	// �g���q�Ȃ��̃t�@�C����
	CString tmpStr;

	SetDlgItemText(IDC_TXT_MES, "");	// ү���޴ر�̸ر
	strcpy(m_tmpMsg, "");	// ���b�Z�[�W�p�o�b�t�@�̃N���A

	// �t�@�C�����̓{�b�N�X�ɁA���͂���Ă��邩�̃`�F�b�N
	GetDlgItemText(IDC_TXT_FNAME, SrcFname);	// �_�C�A���O������̓t�@�C�������擾
	if(SrcFname == "" || SrcFname.GetLength() > (_MAX_DRIVE+_MAX_DIR+_MAX_FNAME))
	{	//	���̓t�@�C�������������A��������ꍇ
		tmpStr.LoadString(IDS_ERR_NOFIL_OR_LONG);
		SetDlgItemText(IDC_TXT_MES, tmpStr);
		return ;
	}

	// �o�̓p�X�̎w��
	if(theApp->m_outpath)
	{	// �o�̓p�X�Œ�̎�
		DstPath = theApp->m_outpath_fname;
	}
	else DstPath = "";

	// �ϊ��`���𔻕ʂ���
	if(GetCheckedRadioButton(IDC_RADIO_SELMODE_1, IDC_RADIO_SELMODE_2) == 
		IDC_RADIO_SELMODE_1)
	{	// �t�@�C���ЂƂ�ϊ�����
		// �t�@�C������ SrcFname = "X:\XXX\ZZZZ.J6I" �̂悤�Ɋ��S�ɓ��͂���Ă���
		if(theApp->m_year)
			::ConvertJ6I((LPCSTR)SrcFname, (LPCSTR)DstPath, m_tmpMsg, m_hWnd, theApp->m_year_data, theApp->m_confirm_ovwr);
		else
			::ConvertJ6I((LPCSTR)SrcFname, (LPCSTR)DstPath, m_tmpMsg, m_hWnd, 0, theApp->m_confirm_ovwr);
		m_last_j6i_file = SrcFname;
		tmpStr = m_tmpMsg;
	}
	else
	{	// �t�H���_���A���ϊ�
		// �t�@�C�������̓{�b�N�X�̓p�X�����̂��߁A"*.J6I"��ǉ�����
		CString WildcardFname = SrcFname + "*.J6I";
		SrcPath = SrcFname;
		struct _finddata_t finddata;
		long hFind;

	    if( (hFind = _findfirst((LPCSTR)WildcardFname, &finddata )) == -1L )
		{
			DspMes("���ްđΏ�̧�ق�������Ȃ�\r\n");
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
			strcpy(m_tmpMsg, "");	// ���b�Z�[�W�p�o�b�t�@�̃N���A
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
				// ��ɍŏI�s��\�����Ă���
				m_txt_mes_ctrl.LineScroll(m_txt_mes_ctrl.GetLineCount());
			}
		}
	}
	// ���b�Z�[�W�G���A���X�V�i���̕\���j
	DspMes(tmpStr);
	// ��ɍŏI�s��\�����Ă���
	m_txt_mes_ctrl.LineScroll(m_txt_mes_ctrl.GetLineCount());
}


// ���b�Z�[�W�G���A�ɕ������\������
//
// str2�ɒl�������ꂽ�ꍇ�́A������ printf("...%s", str) �̂悤�Ɉ���
void CJ6icnvDlg::DspMes(CString str, CString str2)
{
	CString strTemp;

	if(str2 != "") strTemp.Format(str, str2);
	else strTemp = str;

	SetDlgItemText(IDC_TXT_MES, strTemp);
	return;
}

// �@�\�ݒ�_�C�A���O���J��
void CJ6icnvDlg::OnBtnProf() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	CProfDlg profDlg;

	// �_�C�A���O�{�b�N�X�̍��ڂ�ݒ�
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
		// ���W�X�g��������ϐ��Ɋi�[������
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
		// �N���ϊ��ɂ��ẮA���C���_�C�A���O�̓������ڂ��A�b�v�f�[�g����
		SetDlgItemInt(IDC_TXT_YEAR, profDlg.m_txt_year);
		if(profDlg.m_chk_year) m_txt_year_ctrl.EnableWindow(TRUE);
		else m_txt_year_ctrl.EnableWindow(FALSE);
	}
}

// �ϊ���̉摜��\������
void CJ6icnvDlg::OnBtnShw() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	CString DstPath;
	CString DstFname;

	if(m_last_j6i_file =="")
	{
		DspMes("�܂��AJ6I�t�@�C����ϊ����Ă�������");
		return;
	}

	// �o�̓p�X�̎w��
	if(theApp->m_outpath)
	{	// �o�̓p�X�Œ�̎�
		DstPath = theApp->m_outpath_fname;
	}
	else DstPath = "";

	DstFname = ::GetJ6iName(m_last_j6i_file, DstPath);
	if(theApp->m_extvwer)
	{
		if(theApp->m_delete) 
		{	// �o�̓t�@�C���������[�h�̂Ƃ��́A�v���Z�X�I����҂�
			if(_spawnl(_P_WAIT, theApp->m_extvwer_fname, theApp->m_extvwer_fname, DstFname, NULL)<0)
				DspMes("�O�� Viewer �����s�ł��Ȃ�");
		}
		else
		{
			if(_spawnl(_P_NOWAIT, theApp->m_extvwer_fname, theApp->m_extvwer_fname, DstFname, NULL)<0)
				DspMes("�O�� Viewer �����s�ł��Ȃ�");
		}
	}
	else
	{
		::ShellExecute(m_hWnd, NULL, DstFname, NULL, NULL, SW_SHOW);
	}
}

// �N���ϊ��̃`�F�b�N�{�b�N�X�ŁA�N�����͗̈�́u�L���E�����v�̐؂�ւ�������
void CJ6icnvDlg::OnChkYear() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
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

// �N�����̓{�b�N�X�ɐV�����N�������͂��ꂽ�ꍇ
// ���ʗ̈�̃f�[�^���A�b�v�f�[�g����
void CJ6icnvDlg::OnChangeTxtYear() 
{
	// TODO: ���ꂪ RICHEDIT �R���g���[���̏ꍇ�A�R���g���[���́A lParam �}�X�N
	// ���ł̘_���a�� ENM_CHANGE �t���O�t���� CRichEditCrtl().SetEventMask()
	// ���b�Z�[�W���R���g���[���֑��邽�߂� CDialog::OnInitDialog() �֐����I�[�o�[
	// ���C�h���Ȃ����肱�̒ʒm�𑗂�܂���B
	
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	theApp->m_year_data = GetDlgItemInt(IDC_TXT_YEAR);
}

// ���W�I�{�^���u�ЂƂ̃t�@�C���������v���N���b�N
void CJ6icnvDlg::OnRadioSelmode1() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	SetDlgItemText(IDC_TXT_FNAME, "");	// �t�@�C�����̓G���A���N���A
}

// ���W�I�{�^�� �u�A���v���N���b�N
void CJ6icnvDlg::OnRadioSelmode2() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	SetDlgItemText(IDC_TXT_FNAME, "");	// �t�@�C�����̓G���A���N���A	
}

// �N�����̓{�b�N�X�̉E�ɂ���A�X�N���[���{�^�����������ꍇ�̏���
void CJ6icnvDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// ���̃��b�Z�[�W�֐��ɂ́A���ׂẴX�N���[���{�^���̃��b�Z�[�W�������Ă���B
	// ���������āA����̃X�N���[���{�^���𔻒肵�Ă��珈������
	//
	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����邩�܂��̓f�t�H���g�̏������Ăяo���Ă�������
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
