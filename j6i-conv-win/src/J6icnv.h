// j6icnv.h : J6ICNV �A�v���P�[�V�����̃��C�� �w�b�_�[ �t�@�C���ł��B
//

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// ���C�� �V���{��


/////////////////////////////////////////////////////////////////////////////
// CJ6icnvApp:
// ���̃N���X�̓���̒�`�Ɋւ��Ă� j6icnv.cpp �t�@�C�����Q�Ƃ��Ă��������B
//

class CJ6icnvApp : public CWinApp
{
public:
	UINT m_year_data;
	BOOL m_year;
	BOOL m_delete;
	BOOL m_nodialog;
	void UnInstallSeq(void);
	BOOL InstallSeq(void);
	CString m_shell_j6i_fname;
	BOOL m_directview;
	CString m_outpath_fname;
	CString m_extvwer_fname;
	int m_outpath;
	BOOL m_extvwer;
	BOOL m_confirm_ovwr;
	void RegWriteAll(void);
	void RegReadAll(void);
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

