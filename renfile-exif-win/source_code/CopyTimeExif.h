// CopyTimeExif.h : COPYTIMEEXIF �A�v���P�[�V�����̃��C�� �w�b�_�[ �t�@�C���ł��B
//

#if !defined(AFX_COPYTIMEEXIF_H__F86CF922_4741_47F6_95E9_4298C9469434__INCLUDED_)
#define AFX_COPYTIMEEXIF_H__F86CF922_4741_47F6_95E9_4298C9469434__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// ���C�� �V���{��

/////////////////////////////////////////////////////////////////////////////
// CCopyTimeExifApp:
// ���̃N���X�̓���̒�`�Ɋւ��Ă� CopyTimeExif.cpp �t�@�C�����Q�Ƃ��Ă��������B
//

class CCopyTimeExifApp : public CWinApp
{
public:
	CCopyTimeExifApp();

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CCopyTimeExifApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����

	//{{AFX_MSG(CCopyTimeExifApp)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ����o�֐���ǉ��܂��͍폜���܂��B
		//        ���̈ʒu�ɐ��������R�[�h��ҏW���Ȃ��ł��������B
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_COPYTIMEEXIF_H__F86CF922_4741_47F6_95E9_4298C9469434__INCLUDED_)
