// j6iProfDlg.cpp : �C���v�������e�[�V���� �t�@�C��
//
// ���ݒ�_�C�A���O

#include "stdafx.h"
#include "j6icnv.h"
#include "j6icnvDlg.h"
#include "j6iProfDlg.h"
#include <shlobj.h>		// �t�H���_�I���_�C�A���O�p

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CProfDlg �_�C�A���O


CProfDlg::CProfDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CProfDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CProfDlg)
	m_chk_ext = FALSE;
	m_chk_ovwr = FALSE;
	m_txt_extviewer = _T("");
	m_paramvw = FALSE;
	m_txt_outpath = _T("");
	m_chk_delete = FALSE;
	m_chk_nodlg = FALSE;
	m_outpath = -1;
	m_chk_year = FALSE;
	m_txt_year = 0;
	//}}AFX_DATA_INIT
}


void CProfDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProfDlg)
	DDX_Control(pDX, IDC_CHK_DELETE, m_chk_delete_ctrl);
	DDX_Control(pDX, IDC_TXT_YEAR, m_txt_year_ctrl);
	DDX_Control(pDX, IDC_BTN_EXT, m_btn_extview);
	DDX_Control(pDX, IDC_BTN_OUTPUT, m_btn_output);
	DDX_Check(pDX, IDC_CHK_EXT, m_chk_ext);
	DDX_Check(pDX, IDC_CHK_OVWR, m_chk_ovwr);
	DDX_Text(pDX, IDC_TXT_EXT, m_txt_extviewer);
	DDX_Check(pDX, IDC_CHK_PARAMVW, m_paramvw);
	DDX_Text(pDX, IDC_TXT_OUTPUT, m_txt_outpath);
	DDX_Check(pDX, IDC_CHK_DELETE, m_chk_delete);
	DDX_Check(pDX, IDC_CHK_NODLG, m_chk_nodlg);
	DDX_Radio(pDX, IDC_RADIO_OUTPATH, m_outpath);
	DDX_Check(pDX, IDC_CHK_YEAR, m_chk_year);
	DDX_Text(pDX, IDC_TXT_YEAR, m_txt_year);
	DDV_MinMaxUInt(pDX, m_txt_year, 1900, 2030);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CProfDlg, CDialog)
	//{{AFX_MSG_MAP(CProfDlg)
	ON_BN_CLICKED(IDC_BTN_EXT, OnBtnExt)
	ON_BN_CLICKED(IDC_BTN_OUTPUT, OnBtnOutput)
	ON_BN_CLICKED(IDC_ABOUT, OnAbout)
	ON_BN_CLICKED(IDC_RADIO_OUTPATH, OnRadioOutpath)
	ON_BN_CLICKED(IDC_RADIO_OUTPATH2, OnRadioOutpath2)
	ON_BN_CLICKED(IDC_RADIO_OUTPATH3, OnRadioOutpath3)
	ON_BN_CLICKED(IDC_CHK_EXT, OnChkExt)
	ON_BN_CLICKED(IDC_CHK_YEAR, OnChkYear)
	ON_WM_VSCROLL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CProfDlg ���b�Z�[�W �n���h��


// �_�C�A���O�\���O�̏���
//
// �f�t�H���g�{�^���̏�Ԃ�ݒ肷��
BOOL CProfDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: ���̈ʒu�ɏ������̕⑫������ǉ����Ă�������

	// �o�̓p�X�̎�ނɂ��A�{�^���Ȃǂ�ω�������
	char s_full[_MAX_PATH];
	switch(GetCheckedRadioButton(IDC_RADIO_OUTPATH,IDC_RADIO_OUTPATH3))
	{
		case IDC_RADIO_OUTPATH :
			// �\�[�X�t�@�C���Ɠ����t�H���_
			m_btn_output.EnableWindow(FALSE);
			SetDlgItemText(IDC_TXT_OUTPUT, "");	
			break;
		case IDC_RADIO_OUTPATH2 :
			m_btn_output.EnableWindow(FALSE);
			::GetTempPath(_MAX_PATH-1, s_full);
			SetDlgItemText(IDC_TXT_OUTPUT, s_full);	
			break;
		case IDC_RADIO_OUTPATH3 :
			break;
	}

	// �O���r�����[�̐ݒ�̗L�薳��
	if(!IsDlgButtonChecked(IDC_CHK_EXT))
	{
		m_btn_extview.EnableWindow(FALSE);
		SetDlgItemText(IDC_TXT_EXT, "");	
		m_chk_delete_ctrl.EnableWindow(FALSE);	// �V�F�����s���ɂ́A�I����������ł��Ȃ�
		m_chk_delete_ctrl.SetCheck(0);
	}
	else
	{
		m_btn_extview.EnableWindow(TRUE);
		m_chk_delete_ctrl.EnableWindow(TRUE);
	}		
	// �N���ϊ��̗L�薳��
	if(!IsDlgButtonChecked(IDC_CHK_YEAR))
	{
		m_txt_year_ctrl.EnableWindow(FALSE);
	}
	else
	{
		m_txt_year_ctrl.EnableWindow(TRUE);
	}	
	
	return TRUE;  // �R���g���[���Ƀt�H�[�J�X��ݒ肵�Ȃ��Ƃ��A�߂�l�� TRUE �ƂȂ�܂�
	              // ��O: OCX �v���p�e�B �y�[�W�̖߂�l�� FALSE �ƂȂ�܂�
}

// �O���摜�\���\�t�g�̑I���{�^�����������Ƃ�
void CProfDlg::OnBtnExt() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	CString extprog;
	GetDlgItemText(IDC_TXT_EXT, extprog);
	
	CFileDialog dlg(TRUE, "EXE", (LPCSTR)extprog, OFN_HIDEREADONLY|OFN_FILEMUSTEXIST|OFN_EXPLORER,
		"��۸���|*.exe;*.com;*.pif|���ׂĂ�̧��|*.*||");
	dlg.m_ofn.lpstrTitle = "JPG ��\��������۸��т�I��ł�������";

	if(dlg.DoModal() != IDOK) return;

	SetDlgItemText(IDC_TXT_EXT, dlg.GetPathName());
	
}

// �o�̓p�X�̑I���{�^�����������Ƃ�
void CProfDlg::OnBtnOutput() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������

	char s_path[MAX_PATH];
	char sTitle[] = "�ϊ����t�H���_�̑I��";
	LPITEMIDLIST pidl;
	BROWSEINFO bi;
	ZeroMemory(&bi,sizeof(bi));
	bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_STATUSTEXT;
	bi.lpszTitle = sTitle;
	bi.hwndOwner = m_hWnd;

	pidl = ::SHBrowseForFolder(&bi);
	if(pidl == NULL) return;
	SHGetPathFromIDList(pidl, s_path);

	// �t�H���_���͍Ōオ \ �ŏI���悤�ɂ���B�Ȃ��A���[�g�t�H���_�͌����� \ �ŏI����Ă���
	if(s_path[strlen(s_path)-1] != '\\') strcat(s_path, "\\");

	SetDlgItemText(IDC_TXT_OUTPUT, s_path);	
}

// ���쌠�\���_�C�A���O��\������
void CProfDlg::OnAbout() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	CAboutDlg dlgAbout;
	dlgAbout.DoModal();
	
}

// �\�[�X�t�@�C���Ɠ����t�H���_ ���������ꍇ
void CProfDlg::OnRadioOutpath() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	m_btn_output.EnableWindow(FALSE);
	SetDlgItemText(IDC_TXT_OUTPUT, "");
}

// �e���|�����t�H���_ ���������ꍇ
void CProfDlg::OnRadioOutpath2() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	char s_full[_MAX_PATH];
	m_btn_output.EnableWindow(FALSE);
	::GetTempPath(_MAX_PATH-1, s_full);
	SetDlgItemText(IDC_TXT_OUTPUT, s_full);	
}

// �C�ӂ̃t�H���_���������ꍇ
void CProfDlg::OnRadioOutpath3() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	m_btn_output.EnableWindow(TRUE);	
}

// �O���r�����[�̐ݒ�̗L�薳��
void CProfDlg::OnChkExt() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	if(!IsDlgButtonChecked(IDC_CHK_EXT))
	{
		m_btn_extview.EnableWindow(FALSE);
		SetDlgItemText(IDC_TXT_EXT, "");
		m_chk_delete_ctrl.EnableWindow(FALSE);	// �V�F�����s���ɂ́A�I����������ł��Ȃ�
		m_chk_delete_ctrl.SetCheck(0);
	}
	else
	{
		m_btn_extview.EnableWindow(TRUE);
		m_chk_delete_ctrl.EnableWindow(TRUE);
	}		
}

// �N���ϊ��̗L�薳��
void CProfDlg::OnChkYear() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	if(!IsDlgButtonChecked(IDC_CHK_YEAR))
	{
		m_txt_year_ctrl.EnableWindow(FALSE);
	}
	else
	{
		m_txt_year_ctrl.EnableWindow(TRUE);
	}	
}


// �N�ύX�̃X�N���[���{�^���������ꂽ�ꍇ
void CProfDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
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
