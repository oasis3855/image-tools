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
	CButton	m_chk_delete_ctrl;
	CEdit	m_txt_year_ctrl;
	CButton	m_btn_extview;
	CButton	m_btn_output;
	BOOL	m_chk_ext;
	BOOL	m_chk_ovwr;
	CString	m_txt_extviewer;
	BOOL	m_paramvw;
	CString	m_txt_outpath;
	BOOL	m_chk_delete;
	BOOL	m_chk_nodlg;
	int		m_outpath;
	BOOL	m_chk_year;
	UINT	m_txt_year;
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
	afx_msg void OnAbout();
	afx_msg void OnRadioOutpath();
	afx_msg void OnRadioOutpath2();
	afx_msg void OnRadioOutpath3();
	afx_msg void OnChkExt();
	afx_msg void OnChkYear();
	virtual BOOL OnInitDialog();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
