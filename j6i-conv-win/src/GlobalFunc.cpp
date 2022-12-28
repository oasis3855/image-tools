#include "stdafx.h"
#include "GlobalFunc.h"
// ************************************************
// �O���[�o���֐�
// ************************************************

// BCD ���l��^����ƒʏ�̐��l�ɒ����ĕԂ�
unsigned int BCDtoINT(unsigned char bcd)
{
	unsigned int i,j;
	i = bcd;
	j = ((i & 0xf0) >> 4) * 10;
	j += bcd & 0x0f;
	return j;
}


// J6I �� JPG �ɃR���o�[�g����֐�
//
//
// SrcFname : ���̓t�@�C�� (J6I)
// DstPath : �o�̓t�H���_�BNULL�̂Ƃ��͓��̓t�@�C���Ɠ����t�H���_�ƂȂ�
//
//
// SrcFname : ���̓t�@�C����
// DstPath : �o�̓p�X�i�Ō�� \ �ŏI����Ă��邱�Ɓj
// Msg : ���b�Z�[�W�p�̃o�b�t�@�i���256�o�C�g�ő����j
// HWnd : �㏑���m�F�_�C�A���O�\���p�̐e�E�C���h�E�n���h�� �i�܂��� NULL�j
// year : �N���ϊ����Ȃ��Ƃ���0.
// ovwr : �㏑���x�����邩�ǂ���

BOOL ConvertJ6I(LPCSTR SrcFname, LPCSTR DstPath, LPSTR Msg, HWND HWnd, int year, BOOL ovwr)
{

	CFile in,out;		// ���o�̓t�@�C���n���h��
	CFileException fileException;	// �t�@�C���A�N�Z�X�̃G���[�g���b�v�֐�
	void *buf;			// �t�@�C���R�s�[�̎��̃o�b�t�@
	header hd;			// �w�b�_�̈��ǂݍ��ރo�b�t�@
	int i, is_enable_date;
	unsigned long j;	// �t�@�C���|�C���^
	CString DstFname;	// �o�̓t�@�C�����i�p�X���j
	CString FnameBody;	// �g���q�Ȃ��̃t�@�C����

	if(SrcFname == "") return FALSE;		// ���̓t�@�C�������Ȃ��ꍇ

	// �p�X�𕪊����ďo�̓t�@�C�������쐬����
	char s_drv[_MAX_DRIVE], s_dir[_MAX_DIR], s_body[_MAX_FNAME], s_full[_MAX_DRIVE+_MAX_DIR+_MAX_FNAME+4];
	_splitpath((LPCSTR)SrcFname, s_drv, s_dir, s_body, NULL);
	// �o�̓p�X�̎w��
	if(strcmp(DstPath, ""))
	{	// �o�̓p�X�Œ�̎�
		sprintf(s_full, "%s%s.jpg", DstPath, s_body);
	}
	else sprintf(s_full, "%s%s%s.jpg", s_drv, s_dir, s_body);
	DstFname = s_full;

	FnameBody = s_body;

	sprintf(Msg, "%s �̃R���o�[�g\r\n", FnameBody);
	// �㏑���̊m�F
	CFileStatus status;
	if(CFile::GetStatus(DstFname, status))
	{	// JPG �t�@�C�������݂��鎞
		if(ovwr)
		{
			CString tmpStr = FnameBody + "�������O�� jpeg �t�@�C�������݂��܂�\n�㏑�����܂���";
			if(::MessageBox(HWnd, tmpStr, "J6I �t�@�C�� �R���o�[�^", MB_YESNO|MB_ICONQUESTION) != IDYES)
			{
				strcat(Msg, "  �R���o�[�g���~ (����t�@�C������)\r\n");
				return FALSE;
			}
		}
		strcat(Msg, "  �t�@�C�����㏑��\r\n");
	}

	// ���o�͑o���̃t�@�C�����J��
	if(!in.Open( SrcFname, CFile::modeRead, &fileException))
	{
		strcat(Msg, "  J6I�t�@�C�����ǂݍ��߂Ȃ�\r\n");
		return FALSE;
	}
	if(!out.Open( DstFname, CFile::modeWrite|CFile::modeCreate, &fileException))
	{
		strcat(Msg, "  JPG�t�@�C�����쐬�ł��Ȃ�\r\n");
		return FALSE;
	}
	// J6I ̧�ق���w�b�_��ǂݏo��
	if(in.Read(&hd, sizeof(header)) < sizeof(header))
	{
		strcat(Msg, "  J6I�t�@�C���w�b�_�̈�G���[ (�F���s��)\r\n");
		out.Close();
		in.Close();
		return FALSE;
	}
	// �޲ĕ��т̓���ւ�
	if(!hd.endian)
	{
		j = hd.startaddr;
		hd.startaddr = ((j & 0xff000000) >> 24) | ((j & 0x00ff0000) >> 8) | 
			((j & 0x0000ff00) << 8) | ((j & 0x000000ff) << 24);
		j = hd.endaddr;
		hd.endaddr = ((j & 0xff000000) >> 24) | ((j & 0x00ff0000) >> 8) | 
			((j & 0x0000ff00) << 8) | ((j & 0x000000ff) << 24);
	}
	// �w�b�_�̈�̌���
	if(hd.head != (unsigned char)0x80 || hd.dummy1[0] != (unsigned char)0x3e)
	{
		strcat(Msg, "  J6I�t�@�C�������Ă��܂� (�t�@�C�����s��)\r\n");
		out.Close();
		in.Close();
		return FALSE;
	}
	// �w�b�_�̈�̌���(�������i�[����Ă��邩�ǂ���)
	if(hd.enabledate == (unsigned char)0x81) is_enable_date = 1;
	else
	{
		is_enable_date = 0;
		strcat(Msg, "  ������񂪊i�[����Ă��܂��� (�������s)\r\n");
	}

	// ���̕\��
	CString str;
	if(is_enable_date)
	{
		// �N���Œ肩�ǂ����𔻒f���āA���\������
		if(year == 0)
			str.Format("  %02u�N%02u��%02u�� %02u��%02u��%02u�b\r\n  %Lu �޲� (ͯ�� %Lu �޲�) ver%d.%d attr%X\r\n",
				BCDtoINT(hd.date[0]),BCDtoINT(hd.date[1]),
				BCDtoINT(hd.date[2]),BCDtoINT(hd.date[3]),
				BCDtoINT(hd.date[4]),BCDtoINT(hd.date[5]),
				hd.endaddr - hd.startaddr + 1,
				hd.startaddr,
				hd.ver_h, hd.ver_l, hd.attr);
		else
			str.Format("  %02u�N%02u��%02u�� %02u��%02u��%02u�b\r\n  %Lu �޲� (ͯ�� %Lu �޲�) ver%d.%d attr%X\r\n",
				year,BCDtoINT(hd.date[1]),
				BCDtoINT(hd.date[2]),BCDtoINT(hd.date[3]),
				BCDtoINT(hd.date[4]),BCDtoINT(hd.date[5]),
				hd.endaddr - hd.startaddr + 1,
				hd.startaddr,
				hd.ver_h, hd.ver_l, hd.attr);
	}
	else
	{
		str.Format("  %Lu �޲� (ͯ�� %Lu �޲�) �ް�ޮ� %02d.%02d ���� %02X\r\n",
			hd.endaddr - hd.startaddr + 1,
			hd.startaddr,
			hd.ver_h, hd.ver_l, hd.attr);
	}
	strcat(Msg, (LPCSTR)str);
	// J6I �t�@�C���̃f�[�^�̂���ꏊ�܂ł̃V�[�N
	if((ULONG)in.Seek(hd.startaddr, CFile::begin) < hd.startaddr)
	{
		strcat(Msg, "  J6I�t�@�C���ُ�\r\n");
		out.Close();
		in.Close();
		return FALSE;
	}

	// �R�s�[����o�b�t�@�̊m��
	if((buf = malloc(1024*10+1)) == NULL)
	{
		strcat(Msg, "  �������[�s�� (�����I��)\r\n");
		return FALSE;
	}
	
	j = hd.endaddr - hd.startaddr + 1;	// JPG �f�[�^���̌v�Z
	// �f�[�^�̃R�s�[
	do{
		try
		{
			i = in.Read(buf, 1024*10 < j ? 1024*10 : j);
		}
		catch(CFileException *e)
		{	// �t�@�C���A�N�Z�X�̃G���[���g���b�v
			long dummy = e->m_lOsError;	// �ue �������g���Ă��Ȃ��v�x����������邽�߂̃_�~�[
			strcat(Msg, "  J6I�ǂݍ��݃G���[ (���f)\r\n");
			break;
		}
		try
		{
			out.Write(buf, i);
		}
		catch(CFileException *e)
		{	// �t�@�C���A�N�Z�X�̃G���[���g���b�v
			if(e->m_cause == CFileException::diskFull)
			{
				strcat(Msg, "  �f�B�X�N�̎c��̈悪�s�����Ă��܂�\r\n");
				return FALSE;
			}
			strcat(Msg, "  JPG�������݃G���[ (���f)\r\n");
			break;
		}
		j -= i;
	}while(i && j>0);

	out.Close();
	in.Close();

	if(is_enable_date)
	{
		// JPG ̧�ق̓��t�ݒ�
		CFileStatus status;
		CTime FileTime;
		if(!CFile::GetStatus(DstFname, status))
		{
			strcat(Msg, "  JPG�o�̓t�@�C���ُ�\r\n");
			return FALSE;
		}
		if(year == 0)
		{
			int yearhundred = 2000;
			if(BCDtoINT(hd.date[0])>80) yearhundred = 1900; 
			CTime DstTime(yearhundred+BCDtoINT(hd.date[0]), BCDtoINT(hd.date[1]), BCDtoINT(hd.date[2]),
				BCDtoINT(hd.date[3]), BCDtoINT(hd.date[4]), BCDtoINT(hd.date[5]));
			FileTime = DstTime;
		}
		else
		{
			CTime DstTime(year, BCDtoINT(hd.date[1]), BCDtoINT(hd.date[2]),
				BCDtoINT(hd.date[3]), BCDtoINT(hd.date[4]), BCDtoINT(hd.date[5]));
			FileTime = DstTime;
		}

		status.m_ctime = FileTime;
		status.m_mtime = FileTime;
		try{ CFile::SetStatus(DstFname, status); }
		catch(CFileException *e)
		{
			long dummy = e->m_lOsError;	// �ue �������g���Ă��Ȃ��v�x����������邽�߂̃_�~�[
			strcat(Msg, "  JPG�o�̓t�@�C���A�N�Z�X�s�\\r\n");
			return FALSE;
		}
	}

	free(buf);
	return TRUE;
}

// J6I�t�@�C���̃t���p�X��^����ƁA�o�̓t�@�C�������t���p�X���ŕԂ�
// �摜�v���r���[�̂Ƃ��Ɏg�p����
CString GetJ6iName(LPCSTR SrcFname, LPCSTR DstPath)
{
	CString DstFname;	// �o�̓t�@�C�����i�p�X���j

	if(SrcFname == "") DstFname = "";		// ���̓t�@�C�������Ȃ��ꍇ

	// �p�X�𕪊����ďo�̓t�@�C�������쐬����
	char s_drv[_MAX_DRIVE], s_dir[_MAX_DIR], s_body[_MAX_FNAME], s_full[_MAX_DRIVE+_MAX_DIR+_MAX_FNAME+4];
	_splitpath((LPCSTR)SrcFname, s_drv, s_dir, s_body, NULL);
	// �o�̓p�X�̎w��
	if(strcmp(DstPath, ""))
	{	// �o�̓p�X�Œ�̎�
		sprintf(s_full, "%s%s.jpg", DstPath, s_body);
	}
	else sprintf(s_full, "%s%s%s.jpg", s_drv, s_dir, s_body);
	DstFname = s_full;

	return DstFname;
}