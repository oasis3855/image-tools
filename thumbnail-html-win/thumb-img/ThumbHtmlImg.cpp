// ThumbHtmlImg.cpp : �A�v���P�[�V�����p�N���X�̒�`���s���܂��B
// (C) 2000-2003 INOUE. Hirokazu

#include "stdafx.h"
#include "ThumbHtmlImg.h"
#include "ThumbHtmlImgDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CThumbHtmlImgApp

BEGIN_MESSAGE_MAP(CThumbHtmlImgApp, CWinApp)
	//{{AFX_MSG_MAP(CThumbHtmlImgApp)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ}�b�s���O�p�̃}�N����ǉ��܂��͍폜���܂��B
		//        ���̈ʒu�ɐ��������R�[�h��ҏW���Ȃ��ł��������B
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CThumbHtmlImgApp �N���X�̍\�z

CThumbHtmlImgApp::CThumbHtmlImgApp()
{
	// TODO: ���̈ʒu�ɍ\�z�p�̃R�[�h��ǉ����Ă��������B
	// ������ InitInstance ���̏d�v�ȏ��������������ׂċL�q���Ă��������B
}

/////////////////////////////////////////////////////////////////////////////
// �B��� CThumbHtmlImgApp �I�u�W�F�N�g

CThumbHtmlImgApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CThumbHtmlImgApp �N���X�̏�����

BOOL CThumbHtmlImgApp::InitInstance()
{
	// �W���I�ȏ���������
	// ���������̋@�\���g�p�����A���s�t�@�C���̃T�C�Y��������������
	//  ��Έȉ��̓���̏��������[�`���̒�����s�K�v�Ȃ��̂��폜����
	//  ���������B

#ifdef _AFXDLL
	Enable3dControls();			// ���L DLL ���� MFC ���g���ꍇ�͂������R�[�����Ă��������B
#else
	Enable3dControlsStatic();	// MFC �ƐÓI�Ƀ����N����ꍇ�͂������R�[�����Ă��������B
#endif

	CThumbHtmlImgDlg dlg;
	m_pMainWnd = &dlg;
	dlg.DoModal();

	// �_�C�A���O�������Ă���A�v���P�[�V�����̃��b�Z�[�W �|���v���J�n������́A
	// �A�v���P�[�V�������I�����邽�߂� FALSE ��Ԃ��Ă��������B
	return FALSE;
}
