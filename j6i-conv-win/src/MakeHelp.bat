@echo off
REM -- �ŏ��� Microsoft Visual C++ �ō쐬���� resource.h ����}�b�v �t�@�C�����쐬���܂�
echo // MAKEHELP.BAT �����������w���v �}�b�v �t�@�C�� �� J6ICNV.HPJ �Ŏg���܂��B >"hlp\j6icnv.hm"
echo. >>"hlp\j6icnv.hm"
echo // �R�}���h (ID_* and IDM_*) >>"hlp\j6icnv.hm"
makehm ID_,HID_,0x10000 IDM_,HIDM_,0x10000 resource.h >>"hlp\j6icnv.hm"
echo. >>"hlp\j6icnv.hm"
echo // �v�����v�g (IDP_*) >>"hlp\j6icnv.hm"
makehm IDP_,HIDP_,0x30000 resource.h >>"hlp\j6icnv.hm"
echo. >>"hlp\j6icnv.hm"
echo // ���\�[�X (IDR_*) >>"hlp\j6icnv.hm"
makehm IDR_,HIDR_,0x20000 resource.h >>"hlp\j6icnv.hm"
echo. >>"hlp\j6icnv.hm"
echo // �_�C�A���O (IDD_*) >>"hlp\j6icnv.hm"
makehm IDD_,HIDD_,0x20000 resource.h >>"hlp\j6icnv.hm"
echo. >>"hlp\j6icnv.hm"
echo // �t���[�� �R���g���[�� (IDW_*) >>"hlp\j6icnv.hm"
makehm IDW_,HIDW_,0x50000 resource.h >>"hlp\j6icnv.hm"
REM -- �v���W�F�N�g J6ICNV �̃w���v���쐬


echo Win32 ���� ̧�ق�����ޒ�
start /wait hcrtf -x "hlp\j6icnv.hpj"
echo.
if exist Debug\nul copy "hlp\j6icnv.hlp" Debug
if exist Debug\nul copy "hlp\j6icnv.cnt" Debug
if exist Release\nul copy "hlp\j6icnv.hlp" Release
if exist Release\nul copy "hlp\j6icnv.cnt" Release
echo.


