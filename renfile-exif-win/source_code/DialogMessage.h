#if !defined(AFX_DIALOGMESSAGE_H__3C31B7F7_301F_4092_8282_2ED7507933B8__INCLUDED_)
#define AFX_DIALOGMESSAGE_H__3C31B7F7_301F_4092_8282_2ED7507933B8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DialogMessage.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CDialogMessage �_�C�A���O

class CDialogMessage : public CDialog
{
// �R���X�g���N�V����
public:
	CDialogMessage(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CDialogMessage)
	enum { IDD = IDD_DLG_PREVIEW };
	CEdit	m_edit_main_ctrl;
	CString	m_edit_main_text;
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CDialogMessage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CDialogMessage)
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_DIALOGMESSAGE_H__3C31B7F7_301F_4092_8282_2ED7507933B8__INCLUDED_)
