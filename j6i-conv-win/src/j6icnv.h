// j6icnv.h : J6ICNV �A�v���P�[�V�����̃��C�� �w�b�_�[ �t�@�C���ł��B
//

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// ���C�� �V���{��

extern int is_confirm_overwrite, is_use_ext_viewer, is_param_vw, is_outpath;
extern CString ext_viewer;

/////////////////////////////////////////////////////////////////////////////
// CJ6icnvApp:
// ���̃N���X�̓���̒�`�Ɋւ��Ă� j6icnv.cpp �t�@�C�����Q�Ƃ��Ă��������B
//

class CJ6icnvApp : public CWinApp
{
public:
	CJ6icnvApp();

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��𐶐����I�[�o�[���C�h���܂��B
	//{{AFX_VIRTUAL(CJ6icnvApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����

	//{{AFX_MSG(CJ6icnvApp)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ����o�֐���ǉ��܂��͍폜���܂��B
		//        ���̈ʒu�ɐ��������R�[�h��ҏW���Ȃ��ł��������B
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

