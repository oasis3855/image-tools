#if !defined(AFX_HELPDOCVIEWER_H__59E54E85_F25A_4948_9232_5760D51FD2FD__INCLUDED_)
#define AFX_HELPDOCVIEWER_H__59E54E85_F25A_4948_9232_5760D51FD2FD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HelpDocViewer.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CHelpDocViewer �_�C�A���O

class CHelpDocViewer : public CDialog
{
// �R���X�g���N�V����
public:
	CHelpDocViewer(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^
	CFont fntEditbox;

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CHelpDocViewer)
	enum { IDD = IDD_HELPDOC };
	CEdit	m_edit_ctrl;
	CString	m_edit;
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CHelpDocViewer)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CHelpDocViewer)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_HELPDOCVIEWER_H__59E54E85_F25A_4948_9232_5760D51FD2FD__INCLUDED_)
