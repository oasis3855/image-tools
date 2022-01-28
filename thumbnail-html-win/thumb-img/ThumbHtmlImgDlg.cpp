// ThumbHtmlImgDlg.cpp : �C���v�������e�[�V���� �t�@�C��
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
// �O���[�o���ϐ��E�萔
// **********************************
#define	MAX_PATH_ARRAY	1000		// �q�t�H���_�̍ő��
#define MAX_EXT			125			// EXT �w�蕶����̍ő咷��


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
// CThumbHtmlImgDlg �_�C�A���O

CThumbHtmlImgDlg::CThumbHtmlImgDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CThumbHtmlImgDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CThumbHtmlImgDlg)
		// ����: ���̈ʒu�� ClassWizard �ɂ���ă����o�̏��������ǉ�����܂��B
	//}}AFX_DATA_INIT
	// ����: LoadIcon �� Win32 �� DestroyIcon �̃T�u�V�[�P���X��v�����܂���B
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
// CThumbHtmlImgDlg ���b�Z�[�W �n���h��

BOOL CThumbHtmlImgDlg::OnInitDialog()
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

	////////////////////////
	// �_�C�A���O�̏����l��ݒ�
	////////////////////////
	this->SetDlgItemText(IDC_EDIT_PHOTO,"photo");
	this->SetDlgItemText(IDC_EDIT_PICS,"pics");
	m_cmb_photoext_ctrl.SetCurSel(1);	// �u*.jp*�v
	m_cmb_picsext_ctrl.SetCurSel(0);	// �u*.jpg�v
	this->SetDlgItemInt(IDC_EDIT_COL, 2);
	m_spin_col_ctrl.SetRange(1,10);		// �J�������X�s���R���g���[���͈͎̔w��
	this->SetDlgItemText(IDC_EDIT_HTML, "index.html");
	m_cmb_imgopt_ctrl.SetCurSel(0);		// �uborder=0�v��I��
	m_cmb_style_ctrl.SetCurSel(3);		// �u�t�@�C�����i�摜�̍��j �����i�摜�̉E�j�v��I��
	m_cmb_sort_ctrl.SetCurSel(0);		// �u�����Ń\�[�g�i�����j�v
	this->CheckRadioButton(IDC_RADIO_FORCEWR, IDC_RADIO_FORCEWR2, IDC_RADIO_FORCEWR2);
	
	return TRUE;  // TRUE ��Ԃ��ƃR���g���[���ɐݒ肵���t�H�[�J�X�͎����܂���B
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

// �����_�C�A���O�{�b�N�X�ɍŏ����{�^����ǉ�����Ȃ�΁A�A�C�R����`�悷��
// �R�[�h���ȉ��ɋL�q����K�v������܂��BMFC �A�v���P�[�V������ document/view
// ���f�����g���Ă���̂ŁA���̏����̓t���[�����[�N�ɂ�莩���I�ɏ�������܂��B

void CThumbHtmlImgDlg::OnPaint() 
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
HCURSOR CThumbHtmlImgDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

// **********************************
// �e�t�H���_�̎Q��
// **********************************
void CThumbHtmlImgDlg::OnBtnBrowse() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	char strPathname[MAX_PATH];
	LPITEMIDLIST pidl;
	BROWSEINFO bi;

	// �e�L�X�g�{�b�N�X�ɓ��͂���Ă���e�p�X�𓾂�
	GetDlgItemText(IDC_EDIT_PASS, strPathname, MAX_PATH-1);

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
	// �t�@�C�����̓{�b�N�X���A�b�v�f�[�g����
	SetDlgItemText(IDC_EDIT_PASS, strPathname);
	
}


// **********************************
// PHOTO�t�H���_�̎Q��
// **********************************
void CThumbHtmlImgDlg::OnBtnPhotoBrowse() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	BrowseSubPath(IDC_EDIT_PHOTO);
}

// **********************************
// PICS�t�H���_�̎Q��
// **********************************
void CThumbHtmlImgDlg::OnBtnPicsBrowse() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	BrowseSubPath(IDC_EDIT_PICS);	
}


// **********************************
// PHOTO,PICS�t�H���_�̎Q�Ƃ̃_�C�A���O�I������
// **********************************
void CThumbHtmlImgDlg::BrowseSubPath(int nID)
{
	char strPathname[MAX_PATH];
	char strRootPathname[MAX_PATH];
	LPITEMIDLIST pidl, pidlRoot;
	BROWSEINFO bi;

	// �e�L�X�g�{�b�N�X�ɓ��͂���Ă���e�p�X�𓾂�
	GetDlgItemText(IDC_EDIT_PASS, strPathname, MAX_PATH-1);
	if(strlen(strPathname) <= 0) return;	// �������͂���Ă��Ȃ��Ƃ� 
	// ���[�g�t�H���_�̈����B�Ō�� \ �����Ă��邩�ǂ���
	if(strPathname[strlen(strPathname)-1] != '\\')
		strcat(strPathname, "\\");
	strcpy(strRootPathname , strPathname);

	// BROWSEINFO �\���̂̏�����
	ZeroMemory(&bi,sizeof(bi));
	bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_STATUSTEXT;
	bi.lpszTitle = (LPCSTR)"�e�t�H���_�̑I��";
	bi.hwndOwner = m_hWnd;
	bi.lpfn = &myBrowseCallbackProc;	// �����t�H���_�ݒ�̂��߃R�[���o�b�N�֐��ݒ�
	bi.lParam = (LPARAM)strPathname;	// �����t�H���_���p�����[�^�Ƃ��Đݒ�
	pidlRoot = ConvertPIDL(strRootPathname);
	bi.pidlRoot = pidlRoot;

	pidl = ::SHBrowseForFolder(&bi);
	if(pidl == NULL) return;			// �L�����Z���{�^���������ꂽ
	SHGetPathFromIDList(pidl, strPathname);

	////////////////////////
	// pidlRoot, pidl �𖾎��I�ɉ������
	////////////////////////
	IMalloc *imalloc;
	// Shell�̃������Ǘ��I�u�W�F�N�g���擾
	if( ::SHGetMalloc( &imalloc) != NOERROR) imalloc = NULL;
	// �I�u�W�F�N�g�擾�ɐ��������ꍇ�ɊJ������
	if( imalloc != NULL) imalloc->Free((void *)pidlRoot); // ITEMIDLIST���J��

	// Shell�̃������Ǘ��I�u�W�F�N�g���擾
	if( ::SHGetMalloc( &imalloc) != NOERROR) imalloc = NULL;
	// �I�u�W�F�N�g�擾�ɐ��������ꍇ�ɊJ������
	if( imalloc != NULL) imalloc->Free((void *)pidl); // ITEMIDLIST���J��


	// ���[�g�p�X����菜���A���΃p�X�݂̂����o��
	strcpy(strRootPathname, strPathname+strlen(strRootPathname));
	// �t�@�C�����̓{�b�N�X���A�b�v�f�[�g����
	SetDlgItemText(nID, strRootPathname);
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
// �t�H���_�I�� SDK �_�C�A���O ::SHBrowseForFolder �p
// ���[�g�t�H���_�̎w��̂��߂̃t�H���_���R���o�[�g
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
// �������s
// **********************************
void CThumbHtmlImgDlg::OnBtnExec() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	CFile fileOutHtml;						// �t�@�C���n���h�� �iHTML�o�́j
	_fstr fstrPathArray[MAX_PATH_ARRAY];	// �t�@�C�������\�[�g���Ċi�[���邽�߂̔z��
	int nImagesCount;						// �Ώۂ̃C���[�W�t�@�C���� 1�`
	CString strFnameBaseFolder;				// �e�p�X�i�t���p�X�j
	CString strFnameImgSearch;				// photo �t���p�X�ifindfirst�p���C���h�J�[�h���j
	CString strFnameImgBody;				// �t�@�C���̃{�f�B�[���i�g���q�����j
	CString strFnameHtmlPath;				// HTML�t���p�X
	CString strFnamePicsRelativePath;		// pics ���΃p�X (�T���l�C���p�摜 IMG SRC�p)
	CString strFnameImgRelativePath;		// photo ���΃p�X (���̉摜 A LINK�p)
	CString strFnamePicsExt;				// pics �֊i�[����摜�g���q
	CString strFnameImgSearchExt;			// photo �̑Ώۊg���q�i�����w��̓Z�~�R������؂�j
	CString strTagImgOpt;					// IMG �^�O�I�v�V����������
	CString strTagLinkOpt;					// A LINK �^�O�I�v�V����������
	struct tm *timeData;					// time_t �𕪉����ē����f�[�^�ɐ؂蕪����
	int nHtmlColumn;						// HTML �J������
	CString strTmp, strTmp2;
	DWORD _fattr;							// �t�@�C���E�t�H���_�̑��݂��`�F�b�N���鎞�̕ϐ�
	int i, j;								// �ꎞ�I�J�E���^�[�p�i�ėp�j


	////////////////////////
	// �e�ϐ��ɏ����l����  (�����̒l�͎��s�r���ɕω����邱�Ƃ͖���)
	////////////////////////
	GetDlgItemText(IDC_EDIT_PASS, strFnameBaseFolder);			// �e�p�X�𓾂�
	GetDlgItemText(IDC_EDIT_HTML, strFnameHtmlPath);			// HTML���𓾂�
	GetDlgItemText(IDC_EDIT_PHOTO, strFnameImgRelativePath);	// PHOTO�T�u�p�X�𓾂�
	GetDlgItemText(IDC_EDIT_PICS, strFnamePicsRelativePath);	// PICS�T�u�p�X�𓾂�
	GetDlgItemText(IDC_CMB_PHOTOEXT, strFnameImgSearchExt);		// PHOTO �Ώۊg���q
	GetDlgItemText(IDC_CMB_PICSEXT, strFnamePicsExt);			// PICS �Ώۊg���q
	GetDlgItemText(IDC_CMB_IMGOPT, strTagImgOpt);				// IMG �^�O�I�v�V����
	GetDlgItemText(IDC_CMB_LINKOPT, strTagLinkOpt);				// A LINK �^�O�I�v�V����
	nHtmlColumn = GetDlgItemInt(IDC_EDIT_COL);

	////////////////////////
	// �_�C�A���O���͍��ڂ̐������̃`�F�b�N
	////////////////////////
	strTmp.LoadString(IDS_STR_ERRTITLE);
	if(strFnameBaseFolder == "")
	{
		strTmp2.LoadString(STR_MESDLG_BASEDIR);		// ��p�X���ݒ肳��Ă��܂���
		MessageBox(strTmp2, strTmp, MB_OK|MB_ICONEXCLAMATION);
		return ;
	}
	if(strFnameHtmlPath == "")
	{
		strTmp2.LoadString(STR_MESDLG_HTMLNAME);	// HTML �t�@�C�������ݒ肳��Ă��܂���
		MessageBox(strTmp2, strTmp, MB_OK|MB_ICONEXCLAMATION);
		return ;
	}
	if(strFnameImgRelativePath == "")
	{
		strTmp2.LoadString(STR_MESDLG_PHOTODIR);	// �摜�t�H���_���ݒ肳��Ă��܂���
		MessageBox(strTmp2, strTmp, MB_OK|MB_ICONEXCLAMATION);
		return ;
	}
	if(strFnamePicsRelativePath == "")
	{
		strTmp2.LoadString(STR_MESDLG_THUMBDIR);	// �T���l�C���摜�t�H���_���ݒ肳��Ă��܂���
		MessageBox(strTmp2, strTmp, MB_OK|MB_ICONEXCLAMATION);
		return ;
	}
	if(strFnameImgSearchExt == "")
	{
		strTmp2.LoadString(STR_MESDLG_EXTPHOTO);	// �摜�̊g���q�i�����p�^�[���j���ݒ肳��Ă��܂���
		MessageBox(strTmp2, strTmp, MB_OK|MB_ICONEXCLAMATION);
		return ;
	}
	if(strFnamePicsExt == "")
	{
		strTmp2.LoadString(STR_MESDLG_EXTTHUMBNO);	// �T���l�C���摜�̊g���q���ݒ肳��Ă��܂���
		MessageBox(strTmp2, strTmp, MB_OK|MB_ICONEXCLAMATION);
		return ;
	}

	// �e�p�X�̃`�F�b�N �i�Ō�� \ �L�������Ă��Ȃ��ꍇ�͕t������j
	if(strFnameBaseFolder[strFnameBaseFolder.GetLength()-1] != '\\')
		strFnameBaseFolder = strFnameBaseFolder + "\\";

	// HTML �t���p�X�̍쐬
	strFnameHtmlPath = strFnameBaseFolder + strFnameHtmlPath;

	// HTML �J�������̃`�F�b�N
	if(nHtmlColumn <= 0 || nHtmlColumn >= 11)
	{
		strTmp2.LoadString(STR_MESDLG_COLUMN);	// �񐔂� 1 �` 10 �̊ԂŐݒ肵�Ă�������
		MessageBox(strTmp2, strTmp, MB_OK|MB_ICONEXCLAMATION);
		return ;
	}

	// PICS �g���q�����`�F�b�N
	if(strFnamePicsExt[0] != '.' || strFnamePicsExt.Find("*",0) >= 0)
	{
		strTmp2.LoadString(STR_MESDLG_EXTTHUMB);	// �T���l�C���摜�̊g���q�́A�s���I�h�Ŏn�܂镶�������͂��Ă�������
		MessageBox(strTmp2, strTmp, MB_OK|MB_ICONEXCLAMATION);
		return ;
	}

	////////////////////////
	// PHOTO �t�H���_����t�@�C�����ꗗ��ǂݍ���ŁA�\�[�g����
	////////////////////////

	// strtok (�g�[�N���؂�o��) ��p�̕�����錾
	char str_ext_raw[MAX_EXT];	// �؂�o�����̕�����B�؂�o���ɂ��������Ĕj�󂳂��
	char str_delimit[] = ";";	// ��؂蕶��
	char *str_strtok;			// �؂�o���ꂽ���ʕ�����
	strncpy(str_ext_raw, strFnameImgSearchExt, MAX_EXT-1);	// �j��ɔ����ăR�s�[

	// �q�t�H���_���������āA�t�H���_����z��Ɋi�[����
	struct _finddata_t finddata;
	long hFind;
	nImagesCount = 0;

	str_strtok = strtok(str_ext_raw, str_delimit);	// �ŏ��̊g���q��؂�o��
	while(str_strtok != NULL)	// �g���q�̐��������s����
	{
		// �����p�X�̑g�ݗ���
		strFnameImgSearch = strFnameBaseFolder + strFnameImgRelativePath + "\\" + str_strtok;

		if( (hFind = _findfirst((LPCSTR)strFnameImgSearch, &finddata )) == -1L )
		{	// �Q�Ɖ��t�@�C����������Ȃ��ꍇ
		}
		else{
			if(!(finddata.attrib & _A_SUBDIR))
			{	// ��ڂ̃t�@�C���̔���
				strcpy(fstrPathArray[nImagesCount].str, finddata.name);
				fstrPathArray[nImagesCount].tm_write = finddata.time_write;
				nImagesCount++;
			}
			while(_findnext( hFind, &finddata ) == 0)
			{	// �����t�@�C����A������
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
		str_strtok = strtok(NULL, str_delimit);	// ���̊g���q��؂�o��
	}

	if(nImagesCount >= MAX_PATH_ARRAY)
	{
		strTmp.LoadString(STR_MESDLG_EXCEEDLIM);	// �摜�t�@�C�������������E�𒴂��܂���\r\n�r���܂ŏ������܂�
		MessageBox(strTmp);
	}

	if(!nImagesCount)
	{	// PHOTO �t�H���_���ɑΏۃt�@�C�������݂��Ȃ��Ƃ�
		strTmp.LoadString(STR_MESDLG_NOIMAGE);	// �摜�t�@�C�������݂��܂���B�����𒆎~���܂�
		MessageBox(strTmp);
		return ;
	}

	// �t�@�C�����̃\�[�g
	SortArray(fstrPathArray, nImagesCount, m_cmb_sort_ctrl.GetCurSel());


	////////////////////////
	// HTML ��V�K�쐬����
	////////////////////////

	// HTML �t�@�C���̑��݂��`�F�b�N����
	if(GetCheckedRadioButton(IDC_RADIO_FORCEWR, IDC_RADIO_FORCEWR2) == IDC_RADIO_FORCEWR2)
	{
		_fattr = ::GetFileAttributes(strFnameHtmlPath);
		if(_fattr != -1)
		{
			strTmp.Format(STR_MESDLG_OVERWRITE, strFnameHtmlPath);	// HTML�t�@�C��  %s �����łɑ��݂��܂��B�㏑�����܂����H
			strTmp2.LoadString(STR_MESDLG_TITLE);	// ThumbHtmlImg ���b�Z�[�W
			if(MessageBox(strTmp, strTmp2, MB_ICONQUESTION|MB_YESNO) != IDYES)
				return;		// �㏑�������ɏI������
		}
	}
	// HTML �t�@�C�����J��
	if(!fileOutHtml.Open(strFnameHtmlPath,CFile::modeCreate|CFile::modeWrite))
	{
		strTmp.LoadString(IDS_STR_ERRTITLE);
		strTmp2.LoadString(STR_ERR_OPEN_HTML);		// HTML �t�@�C�����J���܂���
		MessageBox(strTmp2, strTmp, MB_OK|MB_ICONSTOP);
		return ;
	}

	////////////////////////
	// HTML �t�@�C���ɏo�͂���
	////////////////////////

	int nCounter;	// �摜�t�@�C�����J�E���^
	// �w�b�_������������
	strTmp = "<html>\r\n<body>\r\n";
	fileOutHtml.Write((LPCSTR)strTmp, strlen(strTmp));
	// ������������
	strTmp.Format("<p>ThumbHtmlImg Ver 1.1<br>%d images</p>\r\n", nImagesCount);
	fileOutHtml.Write((LPCSTR)strTmp, strlen(strTmp));

	// �e�[�u���J�n
	strTmp = "<table border=\"1\">\r\n";
	fileOutHtml.Write((LPCSTR)strTmp, strlen(strTmp));

	// �e�T���l�C���ƃ����N��\�`���ŏ�������
	nCounter = 0;	// �摜�t�@�C�����J�E���^
	while(1)
	{
		if(nCounter >= nImagesCount) break;	// �t�@�C��������������A���[�v�I��

		strTmp = "<tr>\r\n";
		fileOutHtml.Write((LPCSTR)strTmp, strlen(strTmp));
		for(j=0; j<nHtmlColumn; j++)
		{	// ��̒�`
			if(nCounter < nImagesCount)
			{
				// PHOTO �t�@�C��������g���q�𔲂��APICS�g���q��t���鏈��
				for(i=strlen(fstrPathArray[nCounter].str)-1; i>0; i--)
				{	// �P�����ڂ܂ŃX�L�������āc �ƍׂ������Ƃ͖�������
					if(fstrPathArray[nCounter].str[i] == '.') break;
				}
				strTmp2 = fstrPathArray[nCounter].str;
				strFnameImgBody = strTmp2.Left(i);
				strTmp2 = strFnameImgBody + strFnamePicsExt;

				// HTML�L�q�̍쐬�Ə�������
				switch(m_cmb_style_ctrl.GetCurSel())
				{
				case 0 : // �u�����v
					// �摜
					strTmp.Format("\t<td><a href=\"%s/%s\" %s><img src=\"%s/%s\" %s></a></td>\r\n",
						strFnameImgRelativePath, fstrPathArray[nCounter].str, strTagLinkOpt, strFnamePicsRelativePath, strTmp2, strTagImgOpt);
					fileOutHtml.Write((LPCSTR)strTmp, strlen(strTmp));
					break;
				case 1 : // �u�t�@�C�����i�摜�̉��j�v
					// �摜 & �t�@�C�����i���j
					strTmp.Format("\t<td><p><a href=\"%s/%s\" %s><img src=\"%s/%s\" %s></a></p><p>%s</p></td>\r\n",
						strFnameImgRelativePath, fstrPathArray[nCounter].str, strTagLinkOpt, strFnamePicsRelativePath, strTmp2, strTagImgOpt, strFnameImgBody);
					fileOutHtml.Write((LPCSTR)strTmp, strlen(strTmp));
					break;
				case 2 : // �u�t�@�C�����i�摜�̉��j �����i�摜�̉��j�v
					// �摜 & �t�@�C�����i���j& �����i���j
					timeData = localtime(&fstrPathArray[nCounter].tm_write);
					strTmp.Format("\t<td><p><a href=\"%s/%s\" %s><img src=\"%s/%s\" %s></a></p><p>%s<br>%04d/%02d/%02d<br>%02d:%02d</p></td>\r\n",
						strFnameImgRelativePath, fstrPathArray[nCounter].str, strTagLinkOpt, strFnamePicsRelativePath, strTmp2, strTagImgOpt, strFnameImgBody,
						timeData->tm_year+1900, timeData->tm_mon+1, timeData->tm_mday, timeData->tm_hour, timeData->tm_min);
					fileOutHtml.Write((LPCSTR)strTmp, strlen(strTmp));
					break;
				case 3 : // �u�t�@�C�����i�摜�̍��j �����i�摜�̉E�j�v
					// �t�@�C����
					strTmp.Format("\t<td>%s</td>\r\n", strFnameImgBody);
					fileOutHtml.Write((LPCSTR)strTmp, strlen(strTmp));
					// �摜
					strTmp.Format("\t<td><a href=\"%s/%s\" %s><img src=\"%s/%s\" %s></a></td>\r\n",
						strFnameImgRelativePath, fstrPathArray[nCounter].str, strTagLinkOpt, strFnamePicsRelativePath, strTmp2, strTagImgOpt);
					fileOutHtml.Write((LPCSTR)strTmp, strlen(strTmp));
					// ����
					timeData = localtime(&fstrPathArray[nCounter].tm_write);
					strTmp.Format("\t<td>%04d/%02d/%02d<br>%02d:%02d</td>\r\n",
						timeData->tm_year+1900, timeData->tm_mon+1, timeData->tm_mday, timeData->tm_hour, timeData->tm_min);
					fileOutHtml.Write((LPCSTR)strTmp, strlen(strTmp));
					break;
				case 4 : // �u�t�@�C�����i�摜�̉E�j �����i�摜�̉E�j�v
					// �摜
					strTmp.Format("\t<td><a href=\"%s/%s\" %s><img src=\"%s/%s\" %s></a></td>\r\n",
						strFnameImgRelativePath, fstrPathArray[nCounter].str, strTagLinkOpt, strFnamePicsRelativePath, strTmp2, strTagImgOpt);
					fileOutHtml.Write((LPCSTR)strTmp, strlen(strTmp));
					// �t�@�C����
					strTmp.Format("\t<td>%s</td>\r\n", strFnameImgBody);
					fileOutHtml.Write((LPCSTR)strTmp, strlen(strTmp));
					// ����
					timeData = localtime(&fstrPathArray[nCounter].tm_write);
					strTmp.Format("\t<td>%04d/%02d/%02d<br>%02d:%02d</td>\r\n",
						timeData->tm_year+1900, timeData->tm_mon+1, timeData->tm_mday, timeData->tm_hour, timeData->tm_min);
					fileOutHtml.Write((LPCSTR)strTmp, strlen(strTmp));
					break;
				case 5 : // �u�t�@�C�����i�摜�̍��j ���i�摜�̉E�j ���i�摜�̉E�j�v
					// �t�@�C����
					strTmp.Format("\t<td>%s</td>\r\n", strFnameImgBody);
					fileOutHtml.Write((LPCSTR)strTmp, strlen(strTmp));
					// �摜
					strTmp.Format("\t<td><a href=\"%s/%s\" %s><img src=\"%s/%s\" %s></a></td>\r\n",
						strFnameImgRelativePath, fstrPathArray[nCounter].str, strTagLinkOpt, strFnamePicsRelativePath, strTmp2, strTagImgOpt);
					fileOutHtml.Write((LPCSTR)strTmp, strlen(strTmp));
					// ��
					timeData = localtime(&fstrPathArray[nCounter].tm_write);
					strTmp.Format("\t<td>%04d/%02d/%02d</td>\r\n",
						timeData->tm_year+1900, timeData->tm_mon+1, timeData->tm_mday);
					fileOutHtml.Write((LPCSTR)strTmp, strlen(strTmp));
					// ��
					timeData = localtime(&fstrPathArray[nCounter].tm_write);
					strTmp.Format("\t<td>%02d:%02d</td>\r\n",
						timeData->tm_hour, timeData->tm_min);
					fileOutHtml.Write((LPCSTR)strTmp, strlen(strTmp));
					break;
				case 6 : // �u�t�@�C�����i�摜�̉E�j ���i�摜�̉E�j���i�摜�̉E�j�v
					// �摜
					strTmp.Format("\t<td><a href=\"%s/%s\" %s><img src=\"%s/%s\" %s></a></td>\r\n",
						strFnameImgRelativePath, fstrPathArray[nCounter].str, strTagLinkOpt, strFnamePicsRelativePath, strTmp2, strTagImgOpt);
					fileOutHtml.Write((LPCSTR)strTmp, strlen(strTmp));
					// �t�@�C����
					strTmp.Format("\t<td>%s</td>\r\n", strFnameImgBody);
					fileOutHtml.Write((LPCSTR)strTmp, strlen(strTmp));
					// ��
					timeData = localtime(&fstrPathArray[nCounter].tm_write);
					strTmp.Format("\t<td>%04d/%02d/%02d</td>\r\n",
						timeData->tm_year+1900, timeData->tm_mon+1, timeData->tm_mday);
					fileOutHtml.Write((LPCSTR)strTmp, strlen(strTmp));
					// ��
					timeData = localtime(&fstrPathArray[nCounter].tm_write);
					strTmp.Format("\t<td>%02d:%02d</td>\r\n",
						timeData->tm_hour, timeData->tm_min);
					fileOutHtml.Write((LPCSTR)strTmp, strlen(strTmp));
					break;
				default :
					break;
				}
				// �p�f�B���O�i�󔒃J�����j
				strTmp = "\t<td></td>\r\n";
				fileOutHtml.Write((LPCSTR)strTmp, strlen(strTmp));
				
				// �J�E���g�A�b�v
				nCounter++;
			}
			else
			{
				// �f�[�^���Ȃ��Ƃ��́A�󔒃Z����HTML�ɏ�������
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

	// �t�b�^������������
	strTmp = "</table>\r\n</body>\r\n</html>\r\n";
	fileOutHtml.Write((LPCSTR)strTmp, strlen(strTmp));

	// HTML �t�@�C�������
	fileOutHtml.Close();

	strTmp.Format(STR_MES_SUCCESS, strFnameHtmlPath);	// HTML�t�@�C�� %s �̍쐬������ɏI�����܂���
	strTmp2.LoadString(STR_MESDLG_TITLE);	// ThumbHtmlImg ���b�Z�[�W
	MessageBox(strTmp, strTmp2);
}

// **********************************
// �\�[�e�B���O
// **********************************
void CThumbHtmlImgDlg::SortArray(_fstr *fstrPathArray, int nImagesCount, int nMode)
{
	int i, j;
	CString strTmp;
	time_t tmTmp;

	////////////////////////
	// �\�[�g���[�h��؂�ւ���
	////////////////////////
	if(nMode == 0)
	{	// �t�@�C�����Ń\�[�g�i�����j
		for(i=0; i<nImagesCount; i++)
		{
			for(j=i; j<nImagesCount; j++)
			{
				if(stricmp((const char*)fstrPathArray[i].str, (const char*)fstrPathArray[j].str) > 0)
				{
					// �t�@�C�����̌���
					strTmp = fstrPathArray[i].str;
					strcpy(fstrPathArray[i].str, fstrPathArray[j].str);
					strcpy(fstrPathArray[j].str, strTmp);
					// �����̌���
					tmTmp = fstrPathArray[i].tm_write;
					fstrPathArray[i].tm_write = fstrPathArray[j].tm_write;
					fstrPathArray[j].tm_write = tmTmp;
				}
			}
		}
	}
	else if(nMode == 1)
	{	// �t�@�C�����Ń\�[�g�i�~���j
		for(i=0; i<nImagesCount; i++)
		{
			for(j=i; j<nImagesCount; j++)
			{
				if(stricmp((const char*)fstrPathArray[i].str, (const char*)fstrPathArray[j].str) < 0)
				{
					// �t�@�C�����̌���
					strTmp = fstrPathArray[i].str;
					strcpy(fstrPathArray[i].str, fstrPathArray[j].str);
					strcpy(fstrPathArray[j].str, strTmp);
					// �����̌���
					tmTmp = fstrPathArray[i].tm_write;
					fstrPathArray[i].tm_write = fstrPathArray[j].tm_write;
					fstrPathArray[j].tm_write = tmTmp;
				}
			}
		}
	}
	else if(nMode == 2)
	{	// �����Ń\�[�g�i�����j
		for(i=0; i<nImagesCount; i++)
		{
			for(j=i; j<nImagesCount; j++)
			{
				if(fstrPathArray[i].tm_write > fstrPathArray[j].tm_write)
				{
					// �t�@�C�����̌���
					strTmp = fstrPathArray[i].str;
					strcpy(fstrPathArray[i].str, fstrPathArray[j].str);
					strcpy(fstrPathArray[j].str, strTmp);
					// �����̌���
					tmTmp = fstrPathArray[i].tm_write;
					fstrPathArray[i].tm_write = fstrPathArray[j].tm_write;
					fstrPathArray[j].tm_write = tmTmp;
				}
			}
		}
	}
	else if(nMode == 3)
	{	// �����Ń\�[�g�i�~���j
		for(i=0; i<nImagesCount; i++)
		{
			for(j=i; j<nImagesCount; j++)
			{
				if(fstrPathArray[i].tm_write < fstrPathArray[j].tm_write)
				{
					// �t�@�C�����̌���
					strTmp = fstrPathArray[i].str;
					strcpy(fstrPathArray[i].str, fstrPathArray[j].str);
					strcpy(fstrPathArray[j].str, strTmp);
					// �����̌���
					tmTmp = fstrPathArray[i].tm_write;
					fstrPathArray[i].tm_write = fstrPathArray[j].tm_write;
					fstrPathArray[j].tm_write = tmTmp;
				}
			}
		}
	}


}

// **********************************
// �o�[�W�����\���{�^�� �N���b�N
// **********************************
void CThumbHtmlImgDlg::OnBtnAbout() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	CAboutDlg dlg;
	dlg.DoModal();

}


// **********************************
// �w���v�{�^�� �N���b�N
// **********************************
void CThumbHtmlImgDlg::OnBtnHelp() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������

	// �G�f�B�b�g�{�b�N�X�̕����񂪑I����ԂɂȂ�̂�h���ɂ�
	// �^�u�I�[�_�[�ŃG�f�B�b�g�{�b�N�X�̑O�ɕ���{�^���i�f�t�H���g�{�^���j��z�u����B

	CHelpDocViewer dlg;
	CString strTmp;

	strTmp.LoadString(STR_SW_LANG);
	if(strTmp == "JPN")
	{	// ���{�ꃊ�\�[�X�̂Ƃ�

		dlg.m_edit = "\r\n"
		"ThumbHtmlImg - �T���l�C�� HTML �쐬 �i�摜�����j\r\n"
		" Version 1.5   (freeware)\r\n"
		"\r\n\r\n"
		" (C) 2000-2003 INOUE. Hirokazu ��� ���v\r\n"
		"\r\n\r\n\r\n"
		"�y�͂��߂Ɂz\r\n"
		"�t�H���_�ɂ܂Ƃ߂�ꂽ�摜�t�@�C���̃T���l�C��HTML���쐬�ł��܂�\r\n"
		"�B�e�����ʐ^�̐����ɁAHTML�ҏW�\�t�g�ŕҏW���鉺�����Ƃ��ė��p�ł��܂�\r\n"
		"\r\n"
		"ThumbHtmlDir �� �����̃t�H���_�́i�t�H���_�I���̂��߂́j�T���l�C�������܂�\r\n"
		"ThumbHtmlImg �� ��̃t�H���_���̉摜�t�@�C���̃T���l�C�������܂�\r\n"
		"\r\n\r\n"
		"�y������z\r\n"
		"Windows 95/98/Me/NT/2000/XP (���{��ł���ъe�����)\r\n"
		"MFC42.DLL ����� MSVCRT.DLL ���V�X�e���t�H���_�ɑ��݂��邱�ƁiVisual C++ 6.0 �Ή��Łj �Â��o�[�W������DLL�̏ꍇ�A�N���ł��܂���\r\n"
		"\r\n"
		"���̃v���O�����ɂ́A���{��Ɖp��̂Q��ނ̐ݒ肪�܂܂�Ă��܂��B���{��ȊO��Windows�ŋN�������ꍇ�A�����I�ɉp�ꃂ�[�h�œ��삵�܂�\r\n"
		"\r\n\r\n"
		"�y�C���X�g�[���z\r\n"
		"���̉�ʂ����Ă���Ƃ������Ƃ́A���łɃC���X�g�[�������ł�\r\n"
		"\r\n\r\n"
		"�y�A���C���X�g�[���z\r\n"
		"���̃v���O�����́A���W�X�g������ѐݒ�t�@�C���Ȃǂ𗘗p���܂���\r\n"
		"�v���O�����{�̂���������΂���ŏI���ł�\r\n"
		"\r\n\r\n"
		"�y���p�@�z\r\n"
		"�u��t�H���_�v\r\n"
		"   ���̃t�H���_�� HTML �t�@�C�����쐬���܂��B�摜�͂��̃t�H���_�̉���\r\n"
		"   �T�u�t�H���_�Ɋi�[����Ă���K�v������܂�\r\n"
		"   �����Ɂu���v�L�������`�Ŏw�肵�܂�\r\n"
		"   (��) C:\\My Documents\\\r\n"
		"\r\n"
		"�u�摜�t�H���_�v\r\n"
		"   ��t�H���_�̉��ɂ���摜�i���́j�t�@�C���̂���t�H���_\r\n"
		"   ���΃p�X�Ŏw�肵�܂�\r\n"
		"   �� ���̃t�H���_���̃��X�g��HTML�����܂�\r\n"
		"   �� �ő�t�@�C������ 1000 �� �ł�\r\n"
		"\r\n"
		"�u�摜�t�H���_�̑Ώۊg���q�v\r\n"
		"   �Ώۃt�@�C���̌����p�^�[������͂��܂�\r\n"
		"   �����̃p�^�[������������ꍇ�́A�Z�~�R�����ŋ�؂�܂�\r\n"
		"   (��) *.jpg;*.gif\r\n"
		"\r\n"
		"�u�T���l�C���摜�t�H���_�v\r\n"
		"   ��t�H���_�̉��ɂ���T���l�C���摜�i�k���j�t�@�C���̂���t�H���_\r\n"
		"   ���΃p�X�Ŏw�肵�܂�\r\n"
		"   �� �{�v���O�����ł͂��̃t�H���_�͌������܂���\r\n"
		"      �u���E�U�ŕ\������܂łɁA�k���摜��p�ӂ��Ă�������\r\n"
		"\r\n"
		"�u�T���l�C���摜�t�H���_�̑Ώۊg���q�v\r\n"
		"   �Ώۃt�@�C���̊g���q����͂��܂�\r\n"
		"   �����̃p�^�[����A�����t�B���^�͐ݒ�ł��܂���\r\n"
		"   (��) .jpg\r\n"
		"\r\n"
		"�uHTML �t�@�C�����v\r\n"
		"   �o�͂��� �g�s�l�k �t�@�C����\r\n"
		"   (��) index.html\r\n"
		"\r\n"
		"�uHTML �\�̗񐔁v\r\n"
		"   ������ɉ摜����ׂ邩�w�肵�܂� �i1 �` 10 ��j\r\n"
		"\r\n"
		"�uIMG �^�O�̃I�v�V�����v\r\n"
		"   <img src=\"...\" XXXXXX> �� �uXXXXX�v�̎w�肪�o���܂�\r\n"
		"   (��) border=0\r\n"
		"\r\n"
		"�uA LINK �^�O�̃I�v�V�����v\r\n"
		"   <a href=\"...\" XXXXXX> �� �uXXXXX�v�̎w�肪�o���܂�\r\n"
		"   (��) target=_new\r\n"
		"\r\n"
		"�u�t�@�C�����E�����̒ǉ��v\r\n"
		"   �t�@�C�����A�N�����A�����̏������ݕ��@���w�肵�܂�\r\n"
		"   (��) �u�t�@�C�����i�摜�̍��j�A�����i�摜�̉E�j�v���w�肷���\r\n"
		"        +---------+----- +------------+--------+\r\n"
		"        |         |      |            |        |\r\n"
		"        | DSCF001 | �摜 | 2001/05/16 |        |\r\n"
		"        |         |      |   10:56    |        |\r\n"
		"        +---------+----- +------------+--------+\r\n"
		"        |         |      |            |        |\r\n"
		"\r\n"
		"        �̂悤�� HTML ���쐬����܂�\r\n"
		"\r\n"
		"�u�t�@�C���̕��בւ��v\r\n"
		"   �t�@�C���̕��בւ��i�\�[�e�B���O�j���@���w�肵�܂�\r\n"
		"\r\n"
		"�u�㏑�����[�h�v\r\n"
		"   �����̃t�@�C���ɏ㏑������ꍇ�Ɍx�����邩�ǂ�����ݒ肵�܂�\r\n"
		"\r\n"
		"�uAbout�v\r\n"
		"   �o�[�W�����_�C�A���O��\�����܂�\r\n"
		"\r\n"
		"�u���s�v\r\n"
		"   HTML ���쐬���܂�\r\n"
		"\r\n\r\n"
		"�y��̗�z\r\n"
		"    [C:]\r\n"
		"      |\r\n"
		"      +--[Folder1]\r\n"
		"      |\r\n"
		"      +--[Folder2]\r\n"
		"      |         |\r\n"
		"      |         +--[photo]\r\n"
		"      |         |       +--DSCF001.JPG  : 1600x1200�̃f�W�J���摜\r\n"
		"      |         |       +--DSCF002.JPG  : 1600x1200�̃f�W�J���摜\r\n"
		"      |         |\r\n"
		"      |         +--[pics]\r\n"
		"      |         |       +--DSCF001.JPG  : 160x120�̃T���l�C���摜\r\n"
		"      |         |       +--DSCF002.JPG  : 160x120�̃T���l�C���摜\r\n"
		"      |         +--index.html           : HTML�t�@�C��\r\n"
		"\r\n"
		"  ��t�H���_           �F C:\\Folder2\\\r\n"
		"  �摜�t�H���_           �F photo\r\n"
		"  (�Ώۊg���q�j          �F *.jpg\r\n"
		"  �T���l�C���摜�t�H���_ �F pics\r\n"
		"  (�Ώۊg���q�j          �F .jpg\r\n"
		"  HTML �t�@�C����        �F index.html\r\n"
		"\r\n"
		"  �� ��t�H���_�͐�΃p�X�A�摜�E�T���l�C���t�H���_�͑��΃p�X\r\n"
		"\r\n\r\n"
		"�y���Ƃ����E�ҏW�����z\r\n"
		"Version 1.0  2001/09/15\r\n"
		"  �����o�[�W����\r\n"
		"Version 1.5  2003/05/11\r\n"
		"  �t�@�C�����E������������\r\n"
		"  �\�[�e�B���O���@�ǉ�\r\n"
		"  �W���ݒ�̃h���b�v�_�E�����X�g\r\n"
		"  �t�H���_�I���̋L����\r\n"
		"  �w���v�t�@�C��\r\n"
		"\r\n\r\n"
		"�y�g�p���������z\r\n"
		"�v���O������ �F ThumbHtmlImg �T���l�C��HTML�쐬\r\n"
		"�o�[�W����   �F 1.5\r\n"
		"���쌠�ۗL�� �F (C)2000-2003 INOUE. Hirokazu (��� ���v)\r\n"
		"\r\n"
		"���̃v���O�����ƃh�L�������g�i�ȉ��A�v���O�����ƋL�q�j�̓t���[�\�t�g�E�G�A�ł��B\r\n"
		"���̃v���O��������ю��s���̉�ʂ̒��쌠�͂��ׂĈ�� ���v���L���܂��B\r\n"
		"\r\n"
		"����҂͂��̃v���O�����y�уh�L�������g�Ɋւ��邢���Ȃ�ۏ؂��s���܂���B\r\n"
		"�{���i�̓���i���퓮��A�ُ퓮��j�ɂ���Ĕ�邷�ׂĂ̌��ʂɂ��Ă̑S�ӔC�́A�{�\�t�g�̎g�p�҂ɂ���܂��B\r\n"
		"\r\n"
		"���̃v���O�����̍Ĕz�z���A�����E�������ɍs�����蒘��҂���̋����͕K�v����܂���B\r\n"
		"���̃v���O������L���ōĔz�z�A�܂��͔̔�����ꍇ�͒���҂̋����K�v�ł��B\r\n"
		"�Ĕz�z���ɖ{�v���O�����ɕt�������h�L�������g�ȊO�̏��i����҂̌l��񓙁j��t�����邱�Ƃ����ւ��܂��B\r\n"
		"���̃v���O�����𖳒f�ŗL���Ĕz�z�����ꍇ�́A�Ĕz�z�҂���ћ񏕎҂͒���҂ɑ΂��ĘA�т��Ĕ�������`���𕉂��܂��B\r\n"
		"\r\n"
		"���̃v���O�����̃T�|�[�g�͒���҈��d�q���[�����g�p���čs���܂����A����҂��T�|�[�g���s���̂̓{�����e�B�A�ł����ċ`���E�Ӗ��ł͂���܂���B\r\n"
		"\r\n"
		"�� �t���[���[�����甭�M���ꂽ���[���́A�������܂��B �g�s�l�k���[����Y�t�t�@�C�����������A�Y�����[�U����̃��[���͂Q�x�Ǝ�M�܂���B�����ӂ��������B\r\n"
		"\r\n"
		"\r\n"
		"�y���[�U�[�E�T�|�[�g�A����z\r\n"
		"�d�q���[��\r\n"
		"  ���L�̃z�[���y�[�W�Ń��[���A�h���X�����J���Ă��܂��B\r\n"
		"  �i�E�C���X���[���΍�̂��߂̎��q��i�ɂ����͊肢�܂��j\r\n"
		"�z�[���y�[�W\r\n"
		"  http://inoue-h.connect.to/\r\n"
		"  http://www.vector.co.jp/authors/VA001911/index.html\r\n"
		"  http://www.ne.jp/asahi/oasis/inoue-h/index.html\r\n"
		"\r\n"
		"��L�̃A�h���X�����ł��Ă���ꍇ�A�����G���W���Łu��㔎�v�v�Ō������Ă��������B\r\n"
		"My Journey - ���̃y�[�W �����̃y�[�W�ł�\r\n";

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
// F1 �L�[���������Ƃ��̏���
// *****************************************
BOOL CThumbHtmlImgDlg::OnHelpInfo(HELPINFO* pHelpInfo) 
{
	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����邩�܂��̓f�t�H���g�̏������Ăяo���Ă�������
	OnBtnHelp();
	return TRUE;
}
