// j6icnvDlg.h : �w�b�_�[ �t�@�C��
//

typedef struct headertag
{

	unsigned char head;				/* 00 */
	unsigned char dummy1[7-1+1];
	unsigned char ver_h, ver_l;		/* 08,09 */
	char dummy2[0x0f-0x0a+1];
	char spec[0x2f-0x10+1];			/* 10 - 2F */
	unsigned char attr;				/* 30 */
	unsigned char gamma;			/* 31 */
	unsigned char endian;			/* 32 */
	unsigned char dummy3[0x37-0x33+1];
	unsigned long startaddr;		/* 38,39,3A,3B */
	unsigned long endaddr;			/* 3C,3D,3E,3F */
	unsigned char enabledate;		/* 40 */
	unsigned char dummy4;
	unsigned char tz;				/* 42 */
	char dummy5;
	unsigned char date[0x49-0x44+1];/* 44-49 */
	char dummy6[0xab-0x4a+1];
}header;

unsigned int BCDtoINT(unsigned char bcd);


/////////////////////////////////////////////////////////////////////////////
// CJ6icnvDlg dialog

class CJ6icnvDlg : public CDialog
{
// �\�z
public:
	CJ6icnvDlg(CWnd* pParent = NULL);	// �W���̃R���X�g���N�^
	void CnvMain(void);		// ���ް��{��
	void DspMes(CString str, CString str2="");	// ���b�Z�[�W�G���A�ւ̕\��

	CString SrcFname, DstFname;		// �ϊ����ƕϊ���̊��S�ȃp�X��
	CString FnameBody;				// �g���q�����̃t�@�C����
	CString OutPath;				// �o�̓p�X

// Dialog Data
	//{{AFX_DATA(CJ6icnvDlg)
	enum { IDD = IDD_J6ICNV_DIALOG };
		// ����: ���̈ʒu�� ClassWizard �ɂ���ăf�[�^ �����o���ǉ�����܂��B
	//}}AFX_DATA

	// ClassWizard �͉��z�֐��𐶐����I�[�o�[���C�h���܂��B
	//{{AFX_VIRTUAL(CJ6icnvDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �̃T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:
	HICON m_hIcon;

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
	afx_msg void OnBtnSeq();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
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

