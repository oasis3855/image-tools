// j6icnv.cpp : �A�v���P�[�V�����p�N���X�̒�`���s���܂��B
//
// �A�v���P�[�V�����{��
// ��������A���C���_�C�A���O��\��������A�C���X�g�[�������������肷��
// ���ׂẴ��W�X�g���A�N�Z�X���A�����ōs���B

#include "stdafx.h"
#include "j6icnv.h"
#include "j6icnvDlg.h"
#include "InstDlg.h"
#include "ShellLink.h"
#include "GlobalFunc.h"
#include <process.h>	// spawn


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



/////////////////////////////////////////////////////////////////////////////
// CJ6icnvApp

BEGIN_MESSAGE_MAP(CJ6icnvApp, CWinApp)
	//{{AFX_MSG_MAP(CJ6icnvApp)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ}�b�s���O�p�̃}�N����ǉ��܂��͍폜���܂��B
		//        ���̈ʒu�ɐ��������R�[�h��ҏW���Ȃ��ł��������B
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CJ6icnvApp �N���X�̍\�z

CJ6icnvApp::CJ6icnvApp()
{
	// TODO: ���̈ʒu�ɍ\�z�p�̃R�[�h��ǉ����Ă��������B
	// ������ InitInstance ���̏d�v�ȏ��������������ׂċL�q���Ă��������B
}

/////////////////////////////////////////////////////////////////////////////
// �B��� CJ6icnvApp �I�u�W�F�N�g

CJ6icnvApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CJ6icnvApp �N���X�̏�����

BOOL CJ6icnvApp::InitInstance()
{

#ifdef _AFXDLL
	Enable3dControls();			// ���L DLL ���� MFC ���g���ꍇ�͂������R�[�����Ă��������B
#else
	Enable3dControlsStatic();	// MFC �ƐÓI�Ƀ����N����ꍇ�͂������R�[�����Ă��������B
#endif

	CString tmpStr, tmpStr2;

	// �V�X�e�����W�X�g���̎g�p�ʒu��ݒ肷��
	// \HKEY_USERS\.Default\Software\hi_soft\AFS_IDS_APP_TITLE\Settings
	SetRegistryKey((LPCTSTR)"hi soft");
	LoadStdProfileSettings(0);	// MRU���X�g�Ǘ�(0:���Ȃ�)

	m_shell_j6i_fname = "";		// �����̃t�@�C�����́ANULL�Ƃ���

	// �R�}���h���C���̉��
	if(!strcmpi(m_lpCmdLine, "/delete"))
	{	// �A���C���X�g�[��
		UnInstallSeq();
		return FALSE;
	}
	else if(!strcmpi(m_lpCmdLine, "/inst"))
	{	// �C���X�g�[��
		if(!InstallSeq()) return FALSE;
		// �C���X�g�[���������Ƃ́A�v���O�����_�C�A���O��\������
	}
	else if(m_lpCmdLine[0] != NULL)
	{	// ���̑��̃p�����[�^�[�̓t�@�C�����Ƃ��Ĉ���
		m_shell_j6i_fname = m_lpCmdLine;
	}
	else
	{	// �����p�����[�^�[���Ȃ��Ƃ��ŁA�܂��C���X�g�[������Ă��Ȃ���
		if(!GetProfileInt("Global","inst",0))
		{	// �C���X�g�[���_�C�A���O�̕\���ƃC���X�g�[������
			if(!InstallSeq())return FALSE;
		}
	}

	RegReadAll();	// ���W�X�g������ݒ�l��ǂݍ���

	if(m_shell_j6i_fname != "" && m_directview && m_nodialog)
	{
		CString DstPath, DstFname;
		char Msg[512];

		if(m_outpath)
		{	// �o�̓p�X�Œ�̎�
			DstPath = m_outpath_fname;
		}
		else DstPath = "";

		if(m_year)
			::ConvertJ6I((LPCSTR)m_shell_j6i_fname, (LPCSTR)DstPath, Msg, NULL, m_year_data, m_confirm_ovwr);
		else
			::ConvertJ6I((LPCSTR)m_shell_j6i_fname, (LPCSTR)DstPath, Msg, NULL, 0, m_confirm_ovwr);
		// �摜��\������O���v���O���������s
		DstFname = ::GetJ6iName(m_shell_j6i_fname, DstPath);
		if(m_extvwer)
		{
			if(m_delete)
			{	// �o�̓t�@�C�����������郂�[�h�̂Ƃ��́A���s���삵�Ȃ�
				if(_spawnl(_P_WAIT, m_extvwer_fname, m_extvwer_fname, DstFname, NULL)<0)
					::MessageBox(NULL,"�O�� Viewer �����s�ł��Ȃ�","J6I �C���[�W �R���o�[�^", MB_OK);
			}
			else
			{
				if(_spawnl(_P_NOWAIT, m_extvwer_fname, m_extvwer_fname, DstFname, NULL)<0)
					::MessageBox(NULL,"�O�� Viewer �����s�ł��Ȃ�","J6I �C���[�W �R���o�[�^", MB_OK);
			}
		}
		else
		{
			if(m_delete)
					::MessageBox(NULL,"�V�F�����s�Ńt�@�C�������͂ł��Ȃ�","J6I �C���[�W �R���o�[�^", MB_OK);

			::ShellExecute(NULL, NULL, DstFname, NULL, NULL, SW_SHOW);
		}
		// �v���r���[��JPG�t�@�C������������
		if(m_delete) ::remove(DstFname);

	}
	else
	{
		// ���C���_�C�A���O�̐����ƕ\��
		CJ6icnvDlg dlg;
		m_pMainWnd = &dlg;
		dlg.m_chk_year = m_year;
		dlg.m_txt_year = m_year_data;

		dlg.DoModal();

		m_year = dlg.m_chk_year;
		m_year_data = dlg.m_txt_year;
	}

	RegWriteAll();	// �ݒ�l�����W�X�g���ɕۑ�����

	// �_�C�A���O�������Ă���A�v���P�[�V�����̃��b�Z�[�W �|���v���J�n������́A
	// �A�v���P�[�V�������I�����邽�߂� FALSE ��Ԃ��Ă��������B
	return FALSE;
}

// ���W�X�g������ݒ�l��ǂݍ���
void CJ6icnvApp::RegReadAll()
{
	m_confirm_ovwr = GetProfileInt("Settings","cnf_ovwr",1);
	m_extvwer = GetProfileInt("Settings","use_extvw",0);
	m_directview = GetProfileInt("Settings","param_vw",1);
	m_outpath = GetProfileInt("Settings","sw_outpath",0);
	m_delete =  GetProfileInt("Settings","sw_delete",0);
	m_nodialog =  GetProfileInt("Settings","sw_nodlg",0);
	m_year = GetProfileInt("Settings","sw_year",0);
	m_extvwer_fname = GetProfileString("Settings","extvw","");
	m_outpath_fname = GetProfileString("Settings","outpath","");
	m_year_data = GetProfileInt("Settings","sw_year_data",2000);
}

// ���W�X�g���ɐݒ�l����������
void CJ6icnvApp::RegWriteAll()
{
	if(m_confirm_ovwr != (int)GetProfileInt("Settings","cnf_ovwr",1))
		WriteProfileInt("Settings","cnf_ovwr",m_confirm_ovwr);
	if(m_extvwer != (int)GetProfileInt("Settings","use_extvw",0))
		WriteProfileInt("Settings","use_extvw",m_extvwer);
	if(m_directview != (int)GetProfileInt("Settings","param_vw",1))
		WriteProfileInt("Settings","param_vw",m_directview);
	if(m_outpath != (int)GetProfileInt("Settings","sw_outpath",0))
		WriteProfileInt("Settings","sw_outpath",m_outpath);
	if(m_delete !=  (int)GetProfileInt("Settings","sw_delete",0))
		WriteProfileInt("Settings","sw_delete",m_delete);
	if(m_nodialog !=  (int)GetProfileInt("Settings","sw_nodlg",0))
		WriteProfileInt("Settings","sw_nodlg",m_nodialog);
	if(m_year != (int)GetProfileInt("Settings","sw_year",0))
		WriteProfileInt("Settings","sw_year",m_year);
	if(m_extvwer_fname != GetProfileString("Settings","extvw",""))
		WriteProfileString("Settings","extvw",m_extvwer_fname);
	if(m_outpath_fname != GetProfileString("Settings","outpath",""))
		WriteProfileString("Settings","outpath",m_outpath_fname);
	if(m_year != (int)GetProfileInt("Settings","sw_year_data",2000))
		WriteProfileInt("Settings","sw_year_data",m_year_data);
}

// �C���X�g�[��
BOOL CJ6icnvApp::InstallSeq(void)
{
	CInstDlg dlg;
	
	dlg.m_chk_associate = TRUE;
	dlg.m_chk_start = TRUE;
	dlg.m_radio_uninst = 0;

	if(dlg.DoModal() != IDOK) return FALSE;		// ��ݾك{�^���������ꂽ�� �������Ȃ�

	WriteProfileInt("Global","inst",1);
	char szProg[MAX_PATH];
	char szLink[MAX_PATH];
	CString tmpStr;

	// �A���C���X�g�[�����ɂ���
	if(dlg.m_radio_uninst == 0)
	{	// �A���C���X�g�[���p�X���u�A�v���P�[�V�����̒ǉ��ƍ폜�v�ɏ�������
		if(!::GetModuleFileName(NULL, szProg, MAX_PATH)) return FALSE;
		strcat(szProg, " /delete");
		HKEY hCU;
		DWORD dw;
		// �S���[�U�[���ʐݒ�̈�ɁA�A���C���X�g�[��������������
		if (RegCreateKeyEx(HKEY_LOCAL_MACHINE, "Software\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\J6ICNV",
			0, REG_NONE, REG_OPTION_NON_VOLATILE, KEY_WRITE|KEY_READ, NULL,
			&hCU, &dw) == ERROR_SUCCESS)
		{
			RegSetValueEx( hCU, "UninstallString", 0, REG_SZ, (const unsigned char *)szProg, strlen(szProg));
			strcpy(szProg, "J6I �C���[�W �R���o�[�^");
			RegSetValueEx( hCU, "DisplayName", 0, REG_SZ, (const unsigned char *)szProg, strlen(szProg));
			RegCloseKey(hCU);
		}
	}
	else
	{	//�X�^�[�g���j���[�ɃA���C���X�g�[���V���[�g�J�b�g���쐬����
		::GetModuleFileName(NULL, szProg, MAX_PATH);
		HKEY hCU;
		DWORD lpType;
		ULONG ulSize = MAX_PATH;
		if (RegOpenKeyEx(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders",
			0, KEY_QUERY_VALUE, &hCU) == ERROR_SUCCESS)
		{
			RegQueryValueEx( hCU, "Programs", NULL, &lpType, (unsigned char *)&szLink, &ulSize);
			RegCloseKey(hCU);
		}
		strcat(szLink, "\\J6I�Ұ�޺��ް��̍폜.LNK");
		::CreateShellLink(szProg, szLink, "J6ICNV", "/delete");
		WriteProfileInt("Global","inst_local",1);
	}

	if(dlg.m_chk_start)
	{	// �X�^�[�g���j���[�ɓo�^����
		::GetModuleFileName(NULL, szProg, MAX_PATH);
		HKEY hCU;
		DWORD lpType;
		ULONG ulSize = MAX_PATH;
		if (RegOpenKeyEx(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders",
			0, KEY_QUERY_VALUE, &hCU) == ERROR_SUCCESS)
		{
			RegQueryValueEx( hCU, "Programs", NULL, &lpType, (unsigned char *)&szLink, &ulSize);
			RegCloseKey(hCU);
		}
		strcat(szLink, "\\J6I�Ұ�޺��ް�.LNK");
		::CreateShellLink(szProg, szLink, "J6ICNV", "");
	}

	if(dlg.m_chk_associate)
	{	// �֘A�t�����s��
		HKEY hCU;
		DWORD dw;
		// HKEY_CLASSES_ROOT\\.j6i �� �uj6ifile�v ����������
		if (RegCreateKeyEx(HKEY_CLASSES_ROOT, ".j6i",
			0, REG_NONE, REG_OPTION_NON_VOLATILE, KEY_WRITE|KEY_READ, NULL,
			&hCU, &dw) == ERROR_SUCCESS)
		{
			strcpy(szProg, "j6ifile");
			RegSetValueEx( hCU, "", 0, REG_SZ, (unsigned char *)&szProg, strlen(szProg));
			RegCloseKey(hCU);
		}
		else return FALSE;
		// HKEY_CLASSES_ROOT\\j6ifile �� �֘A�t���f�[�^ ����������
		if(!::GetModuleFileName(NULL, szProg, MAX_PATH)) return FALSE;
		strcat(szProg, " %1");
		if (RegCreateKeyEx(HKEY_CLASSES_ROOT, "j6ifile\\shell\\open\\command",
			0, REG_NONE, REG_OPTION_NON_VOLATILE, KEY_WRITE|KEY_READ, NULL,
			&hCU, &dw) == ERROR_SUCCESS)
		{
			RegSetValueEx(hCU, "", 0, REG_SZ, (unsigned char *)&szProg, strlen(szProg));
			RegCloseKey(hCU);
		}
		else return FALSE;

	}

	return TRUE;
}

// �A���C���X�g�[��
void CJ6icnvApp::UnInstallSeq(void)
{
	CString tmpStr, tmpStr2;	// �X�g�����O ���\�[�X �ǂݍ��ݗp
	
	tmpStr.LoadString(IDS_APPNAME);
	tmpStr2.LoadString(IDS_Q_UNINST);
	if(::MessageBox(NULL, tmpStr2, tmpStr, MB_ICONQUESTION|MB_YESNO|MB_SYSTEMMODAL) != IDYES)
		return;

	HKEY hCU;
	char szLink[MAX_PATH];
	DWORD lpType;
	ULONG ulSize = MAX_PATH;

	if(GetProfileInt("Global","inst_local",0))
	{	// �X�^�[�g���j���[�� �폜�A�C�R��������ꍇ
		if (RegOpenKeyEx(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders",
				0, KEY_QUERY_VALUE, &hCU) == ERROR_SUCCESS)
		{
			RegQueryValueEx( hCU, "Programs", NULL, &lpType, (unsigned char *)&szLink, &ulSize);
			RegCloseKey(hCU);
		}
		strcat(szLink, "\\J6I�Ұ�޺��ް��̍폜.LNK");
		::remove(szLink);
	}
	else
	{	// ���W�X�g����ɍ폜�V���[�g�J�b�g������ꍇ
		// �R���g���[���p�l���p�폜�f�[�^�̍폜
		if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, "Software\\Microsoft\\Windows\\CurrentVersion\\Uninstall",
				0, KEY_QUERY_VALUE, &hCU) == ERROR_SUCCESS)
		{
			RegDeleteKey( hCU, "J6ICNV");
			RegCloseKey(hCU);
		}
	}
	// ���W�X�g���̐ݒ�������� ���݂̃��[�U�[��񂩂�폜����
	if (RegOpenKeyEx(HKEY_CURRENT_USER, "Software\\hi soft",
			0, KEY_QUERY_VALUE, &hCU) == ERROR_SUCCESS)
	{
		RegDeleteKey( hCU, "j6i_conv");
		RegCloseKey(hCU);
	}
	// �X�^�[�g���j���[�̐ݒ�A�C�R���𖕎E
	if (RegOpenKeyEx(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders",
			0, KEY_QUERY_VALUE, &hCU) == ERROR_SUCCESS)
	{
		RegQueryValueEx( hCU, "Programs", NULL, &lpType, (unsigned char *)&szLink, &ulSize);
		RegCloseKey(hCU);
	}
	strcat(szLink, "\\J6I�Ұ�޺��ް�.LNK");
	::remove(szLink);
	// �t�@�C���̊֘A�t�����폜
	RegDeleteKey(HKEY_CLASSES_ROOT, ".j6i");
	RegDeleteKey(HKEY_CLASSES_ROOT, "j6ifile");

	// ���̕\��
	tmpStr.LoadString(IDS_APPNAME);
	tmpStr2.LoadString(IDS_END_UNINST);
	::MessageBox(NULL, tmpStr2, tmpStr, MB_ICONINFORMATION|MB_OK|MB_SYSTEMMODAL);

}


