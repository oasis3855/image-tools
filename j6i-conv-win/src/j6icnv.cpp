// j6icnv.cpp : �A�v���P�[�V�����p�N���X�̒�`���s���܂��B
//

#include "stdafx.h"
#include "j6icnv.h"
#include "j6icnvDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// �O���[�o���f�[�^
int is_confirm_overwrite, is_use_ext_viewer, is_param_vw, is_outpath;
CString ext_viewer;


/////////////////////////////////////////////////////////////////////////////
// CJ6icnvApp

BEGIN_MESSAGE_MAP(CJ6icnvApp, CWinApp)
	//{{AFX_MSG_MAP(CJ6icnvApp)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ}�b�s���O�p�̃}�N����ǉ��܂��͍폜���܂��B
		//        ���̈ʒu�ɐ��������R�[�h��ҏW���Ȃ��ł��������B
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CJ6icnvApp �N���X�̍\�z

CJ6icnvApp::CJ6icnvApp()
{
	// TODO: ���̈ʒu�ɍ\�z�p�̃R�[�h��ǉ����Ă��������B
	// ������ InitInstance ���̏d�v�ȏ��������������ׂċL�q���Ă��������B
}

/////////////////////////////////////////////////////////////////////////////
// �B��� CJ6icnvApp �I�u�W�F�N�g

CJ6icnvApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CJ6icnvApp �N���X�̏�����

BOOL CJ6icnvApp::InitInstance()
{

#ifdef _AFXDLL
	Enable3dControls();			// ���L DLL ���� MFC ���g���ꍇ�͂������R�[�����Ă��������B
#else
	Enable3dControlsStatic();	// MFC �ƐÓI�Ƀ����N����ꍇ�͂������R�[�����Ă��������B
#endif

	// �V�X�e�����W�X�g���̎g�p�ʒu��ݒ肷��
	// \HKEY_USERS\.Default\Software\hi_soft\Settings
	m_pszAppName = "j6i_cnv";
	SetRegistryKey((LPCTSTR)"hi soft");
	// ���C���_�C�A���O�̐����ƕ\��
	CJ6icnvDlg dlg;
	m_pMainWnd = &dlg;
	dlg.DoModal();

	// �_�C�A���O�������Ă���A�v���P�[�V�����̃��b�Z�[�W �|���v���J�n������́A
	// �A�v���P�[�V�������I�����邽�߂� FALSE ��Ԃ��Ă��������B
	return FALSE;
}
