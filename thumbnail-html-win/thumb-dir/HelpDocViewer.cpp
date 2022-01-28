// HelpDocViewer.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include "thumbhtmldir.h"
#include "HelpDocViewer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHelpDocViewer �_�C�A���O


CHelpDocViewer::CHelpDocViewer(CWnd* pParent /*=NULL*/)
	: CDialog(CHelpDocViewer::IDD, pParent)
{
	//{{AFX_DATA_INIT(CHelpDocViewer)
	m_edit = _T("");
	//}}AFX_DATA_INIT
}


void CHelpDocViewer::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHelpDocViewer)
	DDX_Control(pDX, IDC_EDIT, m_edit_ctrl);
	DDX_Text(pDX, IDC_EDIT, m_edit);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CHelpDocViewer, CDialog)
	//{{AFX_MSG_MAP(CHelpDocViewer)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHelpDocViewer ���b�Z�[�W �n���h��

BOOL CHelpDocViewer::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: ���̈ʒu�ɏ������̕⑫������ǉ����Ă�������
	LOGFONT fntTmpLogfnt;
	CString strTmp;

	// ���{����̂Ƃ��̂݁A�t�H���g�𓙕��t�H���g�ɐ؂�ւ���
	strTmp.LoadString(STR_SW_LANG);
	if(strTmp == "JPN")
	{

		CFont *fntTmp = m_edit_ctrl.GetFont();
		if(m_edit_ctrl.GetFont()->GetLogFont(&fntTmpLogfnt))
		{
	//		fntTmpLogfnt.lfHeight = 0;
			strcpy(fntTmpLogfnt.lfFaceName, "�l�r �S�V�b�N");
			fntEditbox.DeleteObject();
			if(fntEditbox.CreateFontIndirect(&fntTmpLogfnt))
			{
				m_edit_ctrl.SetFont(&fntEditbox);
			}
		}
	}
	
	
	return TRUE;  // �R���g���[���Ƀt�H�[�J�X��ݒ肵�Ȃ��Ƃ��A�߂�l�� TRUE �ƂȂ�܂�
	              // ��O: OCX �v���p�e�B �y�[�W�̖߂�l�� FALSE �ƂȂ�܂�
}
