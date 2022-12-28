// j6icnvDlg.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CJ6icnvDlg dialog

class CJ6icnvDlg : public CDialog
{
// �\�z
public:
	CString m_last_j6i_file;
	CJ6icnvDlg(CWnd* pParent = NULL);	// �W���̃R���X�g���N�^
	void DspMes(CString str, CString str2="");	// ���b�Z�[�W�G���A�ւ̕\��


// Dialog Data
	//{{AFX_DATA(CJ6icnvDlg)
	enum { IDD = IDD_J6ICNV_DIALOG };
	CEdit	m_txt_mes_ctrl;
	CButton	m_chk_year_ctrl;
	CEdit	m_txt_year_ctrl;
	BOOL	m_chk_year;
	UINT	m_txt_year;
	//}}AFX_DATA

	// ClassWizard �͉��z�֐��𐶐����I�[�o�[���C�h���܂��B
	//{{AFX_VIRTUAL(CJ6icnvDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �̃T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:
	HICON m_hIcon;
	CJ6icnvApp *theApp;

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CJ6icnvDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBtnSel();
	afx_msg void OnBtnCnv();
	afx_msg void OnBtnProf();
	afx_msg void OnBtnShw();
	afx_msg void OnChkYear();
	afx_msg void OnChangeTxtYear();
	afx_msg void OnRadioSelmode1();
	afx_msg void OnRadioSelmode2();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	char m_tmpMsg[256];
};


/////////////////////////////////////////////////////////////////////////////
// �A�v���P�[�V�����̃o�[�W�������Ŏg���Ă��� CAboutDlg �_�C�A���O
class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard �͉��z�֐��𐶐����I�[�o�[���C�h���܂�
	//{{AFX_VIRTUAL(CAboutDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �̃T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

