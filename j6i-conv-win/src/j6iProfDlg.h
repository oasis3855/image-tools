// j6iProfDlg.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CProfDlg �_�C�A���O

class CProfDlg : public CDialog
{
// �R���X�g���N�V����
public:
	CProfDlg(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CProfDlg)
	enum { IDD = IDD_DLG_PROF };
	BOOL	m_chk_ext;
	BOOL	m_chk_ovwr;
	CString	m_txt_extviewer;
	BOOL	m_paramvw;
	BOOL	m_outpath;
	CString	m_txt_outpath;
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��𐶐����I�[�o�[���C�h���܂��B
	//{{AFX_VIRTUAL(CProfDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CProfDlg)
	afx_msg void OnBtnExt();
	afx_msg void OnBtnOutput();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
