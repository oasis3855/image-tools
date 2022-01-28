// SumbHtmlDirDlg.cpp : �C���v�������e�[�V���� �t�@�C��
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
// �A�v���P�[�V�����̃o�[�W�������Ŏg���Ă��� CAboutDlg �_�C�A���O

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂�
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �̃T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
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
		// ���b�Z�[�W �n���h��������܂���B
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSumbHtmlDirDlg �_�C�A���O

CSumbHtmlDirDlg::CSumbHtmlDirDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSumbHtmlDirDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSumbHtmlDirDlg)
	//}}AFX_DATA_INIT
	// ����: LoadIcon �� Win32 �� DestroyIcon �̃T�u�V�[�P���X��v�����܂���B
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
// CSumbHtmlDirDlg ���b�Z�[�W �n���h��

BOOL CSumbHtmlDirDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// "�o�[�W�������..." ���j���[���ڂ��V�X�e�� ���j���[�֒ǉ����܂��B

	// IDM_ABOUTBOX �̓R�}���h ���j���[�͈̔͂łȂ���΂Ȃ�܂���B
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

	// ���̃_�C�A���O�p�̃A�C�R����ݒ肵�܂��B�t���[�����[�N�̓A�v���P�[�V�����̃��C��
	// �E�B���h�E���_�C�A���O�łȂ����͎����I�ɐݒ肵�܂���B
	SetIcon(m_hIcon, TRUE);			// �傫���A�C�R����ݒ�
	SetIcon(m_hIcon, FALSE);		// �������A�C�R����ݒ�
	
	// TODO: ���ʂȏ��������s�����͂��̏ꏊ�ɒǉ����Ă��������B
	this->SetDlgItemText(IDC_EDIT_PICS,"pics");
	this->SetDlgItemInt(IDC_EDIT_COL, 5);
	this->SetDlgItemText(IDC_EDIT_HTML, "index.html");
	this->CheckRadioButton(IDC_RADIO_FORCEWR, IDC_RADIO_FORCEWR2, IDC_RADIO_FORCEWR2);

	// �u�����t�H���_�ǉ����[�h��̃R���g���[���������[�u
	OnCheckAddpath();

	return TRUE;  // TRUE ��Ԃ��ƃR���g���[���ɐݒ肵���t�H�[�J�X�͎����܂���B
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

// �����_�C�A���O�{�b�N�X�ɍŏ����{�^����ǉ�����Ȃ�΁A�A�C�R����`�悷��
// �R�[�h���ȉ��ɋL�q����K�v������܂��BMFC �A�v���P�[�V������ document/view
// ���f�����g���Ă���̂ŁA���̏����̓t���[�����[�N�ɂ�莩���I�ɏ�������܂��B

void CSumbHtmlDirDlg::OnPaint() 
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
HCURSOR CSumbHtmlDirDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

// �t�H���_�̎Q��
void CSumbHtmlDirDlg::OnBtnBrowse() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	char s_full[MAX_PATH];
	LPITEMIDLIST pidl;
	BROWSEINFO bi;
	ZeroMemory(&bi,sizeof(bi));
	bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_STATUSTEXT;
	bi.lpszTitle = (LPCSTR)"�e�t�H���_�̑I��";
	bi.hwndOwner = m_hWnd;

	pidl = ::SHBrowseForFolder(&bi);
	if(pidl == NULL) return;
	SHGetPathFromIDList(pidl, s_full);

	// ���[�g�t�H���_�̈����B�Ō�� \ �����Ă��邩�ǂ���
	if(s_full[strlen(s_full)-1] != '\\')
		strcat(s_full, "\\");
	// �t�@�C�����̓{�b�N�X���A�b�v�f�[�g����
	SetDlgItemText(IDC_EDIT_PATH, s_full);
	
}
// �t�H���_�̎Q��
void CSumbHtmlDirDlg::OnBtnBrowseAddpath() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	char s_full[MAX_PATH];
	LPITEMIDLIST pidl;
	BROWSEINFO bi;
	ZeroMemory(&bi,sizeof(bi));
	bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_STATUSTEXT;
	bi.lpszTitle = (LPCSTR)"�e�t�H���_�̑I��";
	bi.hwndOwner = m_hWnd;

	pidl = ::SHBrowseForFolder(&bi);
	if(pidl == NULL) return;
	SHGetPathFromIDList(pidl, s_full);

	// ���[�g�t�H���_�̈����B�Ō�� \ �����Ă��邩�ǂ���
	if(s_full[strlen(s_full)-1] != '\\')
		strcat(s_full, "\\");
	// �t�@�C�����̓{�b�N�X���A�b�v�f�[�g����
	SetDlgItemText(IDC_EDIT_ADDPATH, s_full);
}	
	

// �������s
#define	MAX_PASS_ARRAY	400		// �q�t�H���_�̍ő��

void CSumbHtmlDirDlg::OnBtnExec() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	CFile f_html;						// �t�@�C���n���h��
	_fstr Pass_Array[MAX_PASS_ARRAY];	// �q�t�H���_���\�[�g���Ċi�[���邽�߂̔z��
	int pass_count = 0;					// �i�[���Ă���q�t�H���_�� 1�`
	CString FpathParent;				// �e�p�X�i�t���p�X�j
	CString FpathParentSearch;			// �e�p�X�ifindfirst�p���C���h�J�[�h���j
	CString FpathHTML;					// HTML�t���p�X
	CString FpathPICS;					// pic �t���p�X
	CString FpathSPICS;					// pic �p�X
	CString FpathAddPath;				// �����t�H���_�ǉ��p�A�����p�X�i�t���p�X�j
	CString FpathAddPathSearch;			// �����t�H���_�p�X�ifindfirst�p���C���h�J�[�h���j
	DWORD _fattr;						// �t�@�C���E�t�H���_�̑��݂��`�F�b�N���鎞�̕ϐ�
	CString StrTmp;
	int i, j;

	GetDlgItemText(IDC_EDIT_PASS, FpathParent);	// �e�p�X�𓾂�
	GetDlgItemText(IDC_EDIT_HTML, FpathHTML);	// HTML���𓾂�
	FpathHTML = FpathParent + FpathHTML;		// HTML �t���p�X�̍쐬
	GetDlgItemText(IDC_EDIT_PICS, FpathSPICS);	// PICS�T�u�p�X�𓾂�
	FpathPICS = FpathParent + FpathSPICS;		// PICS�t���p�X
	FpathParentSearch = FpathParent + "*";		// findfirst �p���C���h�J�[�h�t��

	GetDlgItemText(IDC_EDIT_ADDPATH, FpathAddPath);	// �����t�H���_�p�X�𓾂�
	FpathAddPathSearch = FpathAddPath + "*";		// findfirst �p���C���h�J�[�h�t��

	// �t�H���_�����͍ς݂̃`�F�b�N
	if(!strlen(FpathParent))
	{
		MessageBox("�e�t�H���_�����ݒ肳��Ă��܂���");
		return;
	}
	if(IsDlgButtonChecked(IDC_CHECK_ADDPATH) && !strlen(FpathAddPath))
	{
		MessageBox("�����t�H���_�����ݒ肳��Ă��܂���");
		return;
	}

	// ***********
	// �q�t�H���_���������āA�t�H���_����z��Ɋi�[����
	// ***********
	struct _finddata_t finddata;
	long hFind;

    if( (hFind = _findfirst((LPCSTR)FpathParentSearch, &finddata )) == -1L )
	{	// �Q�Ɖ��t�@�C����������Ȃ��ꍇ
	}
	else{
		if(finddata.attrib & _A_SUBDIR)
		{	// ��ڂ̃t�H���_�̔���
			if(finddata.name[0] != '.' && stricmp(finddata.name, FpathSPICS) != 0)
			{	// "." , ".." �� PICS �t�H���_�̏��O
				strcpy(Pass_Array[pass_count].str, finddata.name);
				pass_count++;
			}
		}
		while(_findnext( hFind, &finddata ) == 0)
		{	// �����t�H���_��A������
			if(finddata.attrib & _A_SUBDIR)
			{
				if(finddata.name[0] != '.' && stricmp(finddata.name, FpathSPICS) != 0)
				{	// "." , ".." �� PICS �t�H���_�̏��O
					strcpy(Pass_Array[pass_count].str, finddata.name);
					pass_count++;
					if(pass_count >= MAX_PASS_ARRAY) break;
				}
			}
		}
	}
	if(pass_count >= MAX_PASS_ARRAY)
		MessageBox("�q�t�H���_�����������E�𒴂��܂���\r\n�r���܂ŏ������܂�");

	if(!pass_count)
	{	// �q�t�H���_��������Ȃ��Ƃ�
		MessageBox("�q�t�H���_�����݂��܂���B�����𒆎~���܂��B");
		return ;
	}

	// �t�H���_�z��̃\�[�g
	SortArray(Pass_Array, pass_count);

	// ***********
	// pics �t�H���_�̍쐬
	// ***********
	_fattr = ::GetFileAttributes(FpathPICS);		// �t�H���_�̑��݂̊m�F
	if(_fattr == -1)
	{	// �����̃t�H���_���t�@�C�������݂��Ȃ��Ǝv����
		if(!::CreateDirectory(FpathPICS, NULL))
		{
			MessageBox("�T���l�C�� �t�H���_�̍쐬�Ɏ��s���܂���");
			return ;
		}
	}
	else if(_fattr & FILE_ATTRIBUTE_DIRECTORY)
	{
		StrTmp = "�T���l�C���t�H���_ \"" + FpathSPICS + "\" �����łɑ��݂��܂�\n���̃t�H���_�𗘗p���܂����H";
		if(MessageBox(StrTmp, "�d���t�H���_�̊m�F", MB_ICONQUESTION|MB_YESNO) == IDNO)
			return;

	}
	else
	{
		MessageBox("�����̃t�@�C�������邽�߁A�T���l�C�� �t�H���_���쐬�ł��܂���");
		return ;
	}


	// ***********
	// �T���l�C���p�t�@�C����PICS�t�H���_�ɃR�s�[
	// ***********
	CString StrInName, StrOutName;
	for(i=0; i<pass_count; i++)
	{
		// �T���l�C���t�@�C�����̎擾
		StrTmp.Format("%s%s\\*.jp*", FpathParent, Pass_Array[i].str);	// �t�H���_�̃t���p�X
		StrTmp = GetSumbFname((LPCSTR)StrTmp);	// �t�H���_���́u�ŏ��̣���O�̃t�@�C�����𓾂�

		// �t�@�C���̃R�s�[
		StrInName.Format("%s%s\\%s", FpathParent, Pass_Array[i].str, StrTmp);
		StrOutName.Format("%s\\%s.jpg", FpathPICS, Pass_Array[i].str);
		CopyFile((LPCSTR)StrInName, (LPCSTR)StrOutName);
	}

	// ***********
	// �����t�H���_�ɒǉ����[�h
	// ***********
	if(IsDlgButtonChecked(IDC_CHECK_ADDPATH))
	{
	    if( (hFind = _findfirst((LPCSTR)FpathAddPathSearch, &finddata )) == -1L )
		{	// �Q�Ɖ��t�@�C����������Ȃ��ꍇ
		}
		else{
			if(finddata.attrib & _A_SUBDIR)
			{	// ��ڂ̃t�H���_�̔���
				if(finddata.name[0] != '.' && stricmp(finddata.name, FpathSPICS) != 0)
				{	// "." , ".." �� PICS �t�H���_�̏��O
					strcpy(Pass_Array[pass_count].str, finddata.name);
					pass_count++;
				}
			}
			while(_findnext( hFind, &finddata ) == 0)
			{	// �����t�H���_��A������
				if(finddata.attrib & _A_SUBDIR)
				{
					if(finddata.name[0] != '.' && stricmp(finddata.name, FpathSPICS) != 0)
					{	// "." , ".." �� PICS �t�H���_�̏��O
						strcpy(Pass_Array[pass_count].str, finddata.name);
						pass_count++;
						if(pass_count >= MAX_PASS_ARRAY) break;
					}
				}
			}
		}
		if(pass_count >= MAX_PASS_ARRAY)
			MessageBox("�q�t�H���_�����������E�𒴂��܂���\r\n�r���܂ŏ������܂�");
		// �t�H���_�z��̃\�[�g
		SortArray(Pass_Array, pass_count);
	}

	// ***********
	// �����t�H���_�ɒǉ����[�h���́A�t�H���_�d���m�F
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
			StrTmp = "�d���t�H���_�����݂��܂�\n�����𑱍s���܂����H\n�i�d���t�H���_�͎��̂Ƃ���ł��j\n\n" + StrTmp;
			if(MessageBox(StrTmp, "�d���t�H���_�̊m�F", MB_ICONQUESTION|MB_YESNO) == IDNO)
				return;
		}

	}


	// ***********
	// HTML �t�@�C���̍쐬
	// ***********

	// HTML �t�@�C���̑��݂��`�F�b�N����
	if(GetCheckedRadioButton(IDC_RADIO_FORCEWR, IDC_RADIO_FORCEWR2) == IDC_RADIO_FORCEWR2)
	{
		_fattr = ::GetFileAttributes(FpathHTML);
		if(_fattr != -1)
		{
			StrTmp = "HTML�t�@�C�� \"" + FpathHTML + " \"�����łɑ��݂��܂��B�㏑�����܂����H";
			if(MessageBox(StrTmp, "HTML�t�@�C���㏑���̊m�F", MB_ICONQUESTION|MB_YESNO) != IDYES)
				return;		// �㏑�������ɏI������
		}
	}
	// HTML �t�@�C�����J���i�V�K�쐬����j
	if(!f_html.Open(FpathHTML,CFile::modeCreate|CFile::modeWrite))
	{
		MessageBox("HTML �t�@�C�����J���܂���");
		return ;
	}


	// ***********
	// HTML �ɏ�������
	// ***********
	int colmn, cntr;
	colmn = this->GetDlgItemInt(IDC_EDIT_COL);
	if(colmn <= 0) colmn = 5;	// 0�J�����͂��肦�Ȃ�
	// �w�b�_������������
	StrTmp = "<html>\r\n<body>\r\n<table border=\"1\"\r\n>";
	f_html.Write((LPCSTR)StrTmp, strlen(StrTmp));

	// ALINK �̍s����� index.html �Ƃ��邽�߂̕�����ݒ�
	CString StrAlink;
	if(IsDlgButtonChecked(IDC_CHECK_ALINK)) StrAlink = "/index.html";
	else StrAlink = "";
	// �q�t�H���_������������
	cntr = 0;	// �t�H���_�J�E���^
	for(i=0; i<pass_count/colmn+1; i++)
	{
		StrTmp = "<tr>\r\n";
		f_html.Write((LPCSTR)StrTmp, strlen(StrTmp));
		for(j=0; j<colmn; j++)
		{	// ��̒�`
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

	// �t�b�^������������
	StrTmp = "</table>\r\n</body>\r\n</html>\r\n";
	f_html.Write((LPCSTR)StrTmp, strlen(StrTmp));

	f_html.Close();

	// �I�����b�Z�[�W�̕\��
	StrTmp = "HTML�t�@�C�� \"" + FpathHTML + " \"�̍쐬������ɏI�����܂���";
	MessageBox(StrTmp, "��������");
}

// �\�[�e�B���O
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

// �t�@�C���R�s�[
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

// �t�H���_���́u�����Ƃ�����������O�̃t�@�C������Ԃ��i�p�X���͕t���Ȃ��ŕԂ��j
LPSTR CSumbHtmlDirDlg::GetSumbFname(LPCSTR Fpath)
{
	struct _finddata_t finddata;
	long hFind;
	char RetName[_MAX_FNAME];
	char *RetPtr;

	strcpy(RetName, "");

    if( (hFind = _findfirst((LPCSTR)Fpath, &finddata )) == -1L )
	{	// �Q�Ɖ��t�@�C����������Ȃ��ꍇ
	}
	else{
		if(!(finddata.attrib & _A_SUBDIR))
		{	// ��ڂ̃t�@�C���̔���
			strcpy(RetName, finddata.name);
		}
		while(_findnext( hFind, &finddata ) == 0)
		{	// �����t�@�C����A������
			if(!(finddata.attrib & _A_SUBDIR))
			{
				// ��ԁu����������O�̃t�@�C������RetName�ɕێ�����
				if(stricmp(finddata.name, RetName) < 0) strcpy(RetName, finddata.name);
			}
		}
	}
	RetPtr = (LPSTR)RetName;
	return RetPtr;
}

// �u�����t�H���_�ǉ����[�h��̃`�F�b�N�������ꂽ��A�C���v�b�g�{�b�N�X��L���ɂ���B
void CSumbHtmlDirDlg::OnCheckAddpath() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
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

// �����{�^�����������Ƃ��̏���
void CSumbHtmlDirDlg::OnBtnAbout() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	CAboutDlg dlg;

	dlg.DoModal();
}
