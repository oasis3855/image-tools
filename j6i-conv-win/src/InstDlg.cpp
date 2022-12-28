// InstDlg.cpp : �C���v�������e�[�V���� �t�@�C��
//
// �C���X�g�[���_�C�A���O
//
#include "stdafx.h"
#include "j6icnv.h"
#include "InstDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInstDlg �_�C�A���O


CInstDlg::CInstDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CInstDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInstDlg)
	m_chk_start = FALSE;
	m_chk_associate = FALSE;
	m_radio_uninst = -1;
	//}}AFX_DATA_INIT
}


void CInstDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInstDlg)
	DDX_Check(pDX, IDC_CHK_START, m_chk_start);
	DDX_Check(pDX, IDC_CHK_ASSOCIATE, m_chk_associate);
	DDX_Radio(pDX, IDC_RADIO_UNINST1, m_radio_uninst);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CInstDlg, CDialog)
	//{{AFX_MSG_MAP(CInstDlg)
	ON_BN_CLICKED(IDC_HELP_INST, OnHelpInst)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInstDlg ���b�Z�[�W �n���h��


void CInstDlg::OnHelpInst() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	OnHelp();
}

// �E�C���h�E���j�������Ƃ��ɂ́A�w���v�����
void CInstDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����Ă�������
	WinHelp(0L, HELP_QUIT);
	
}
