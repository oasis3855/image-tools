@echo off
REM -- 最初に Microsoft Visual C++ で作成した resource.h からマップ ファイルを作成します
echo // MAKEHELP.BAT が生成したヘルプ マップ ファイル は J6ICNV.HPJ で使われます。 >"hlp\j6icnv.hm"
echo. >>"hlp\j6icnv.hm"
echo // コマンド (ID_* and IDM_*) >>"hlp\j6icnv.hm"
makehm ID_,HID_,0x10000 IDM_,HIDM_,0x10000 resource.h >>"hlp\j6icnv.hm"
echo. >>"hlp\j6icnv.hm"
echo // プロンプト (IDP_*) >>"hlp\j6icnv.hm"
makehm IDP_,HIDP_,0x30000 resource.h >>"hlp\j6icnv.hm"
echo. >>"hlp\j6icnv.hm"
echo // リソース (IDR_*) >>"hlp\j6icnv.hm"
makehm IDR_,HIDR_,0x20000 resource.h >>"hlp\j6icnv.hm"
echo. >>"hlp\j6icnv.hm"
echo // ダイアログ (IDD_*) >>"hlp\j6icnv.hm"
makehm IDD_,HIDD_,0x20000 resource.h >>"hlp\j6icnv.hm"
echo. >>"hlp\j6icnv.hm"
echo // フレーム コントロール (IDW_*) >>"hlp\j6icnv.hm"
makehm IDW_,HIDW_,0x50000 resource.h >>"hlp\j6icnv.hm"
REM -- プロジェクト J6ICNV のヘルプを作成


echo Win32 ﾍﾙﾌﾟ ﾌｧｲﾙのﾋﾞﾙﾄﾞ中
start /wait hcrtf -x "hlp\j6icnv.hpj"
echo.
if exist Debug\nul copy "hlp\j6icnv.hlp" Debug
if exist Debug\nul copy "hlp\j6icnv.cnt" Debug
if exist Release\nul copy "hlp\j6icnv.hlp" Release
if exist Release\nul copy "hlp\j6icnv.cnt" Release
echo.


