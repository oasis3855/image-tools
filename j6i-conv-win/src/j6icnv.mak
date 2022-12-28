# Microsoft Developer Studio Generated NMAKE File, Format Version 4.00
# ** 編集しないでください **

# TARGTYPE "Win32 (x86) Application" 0x0101

!IF "$(CFG)" == ""
CFG=j6icnv - Win32 Debug
!MESSAGE 構成が指定されていません。ﾃﾞﾌｫﾙﾄの j6icnv - Win32 Debug を設定します。
!ENDIF 

!IF "$(CFG)" != "j6icnv - Win32 Release" && "$(CFG)" != "j6icnv - Win32 Debug"
!MESSAGE 指定された ﾋﾞﾙﾄﾞ ﾓｰﾄﾞ "$(CFG)" は正しくありません。
!MESSAGE ｺﾏﾝﾄﾞ ﾗｲﾝ上でﾏｸﾛ 'CFG' を定義することによって
!MESSAGE NMAKE 実行時にﾋﾞﾙﾄﾞ ﾓｰﾄﾞを指定できます。例えば:
!MESSAGE 
!MESSAGE NMAKE /f "j6icnv.mak" CFG="j6icnv - Win32 Debug"
!MESSAGE 
!MESSAGE 選択可能なﾋﾞﾙﾄﾞ ﾓｰﾄﾞ:
!MESSAGE 
!MESSAGE "j6icnv - Win32 Release" ("Win32 (x86) Application" 用)
!MESSAGE "j6icnv - Win32 Debug" ("Win32 (x86) Application" 用)
!MESSAGE 
!ERROR 無効な構成が指定されています。
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 
################################################################################
# Begin Project
# PROP Target_Last_Scanned "j6icnv - Win32 Debug"
RSC=rc.exe
CPP=cl.exe
MTL=mktyplib.exe

!IF  "$(CFG)" == "j6icnv - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
OUTDIR=.\Release
INTDIR=.\Release

ALL : "$(OUTDIR)\j6icnv.exe" "$(OUTDIR)\j6icnv.hlp"

CLEAN : 
	-@erase ".\Release\j6icnv.exe"
	-@erase ".\Release\StdAfx.obj"
	-@erase ".\Release\j6icnv.obj"
	-@erase ".\Release\j6icnv.pch"
	-@erase ".\Release\j6icnvDlg.obj"
	-@erase ".\Release\j6iProfDlg.obj"
	-@erase ".\Release\j6icnv.res"
	-@erase ".\Release\j6icnv.hlp"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /MD /W3 /GX /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /c
# ADD CPP /nologo /MD /W3 /GX /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /c
CPP_PROJ=/nologo /MD /W3 /GX /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL"\
 /D "_MBCS" /Fp"$(INTDIR)/j6icnv.pch" /Yu"stdafx.h" /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\Release/
CPP_SBRS=
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /win32
MTL_PROJ=/nologo /D "NDEBUG" /win32 
# ADD BASE RSC /l 0x411 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x411 /d "NDEBUG" /d "_AFXDLL"
RSC_PROJ=/l 0x411 /fo"$(INTDIR)/j6icnv.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/j6icnv.bsc" 
BSC32_SBRS=
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386
LINK32_FLAGS=/nologo /subsystem:windows /incremental:no\
 /pdb:"$(OUTDIR)/j6icnv.pdb" /machine:I386 /out:"$(OUTDIR)/j6icnv.exe" 
LINK32_OBJS= \
	"$(INTDIR)/StdAfx.obj" \
	"$(INTDIR)/j6icnv.obj" \
	"$(INTDIR)/j6icnvDlg.obj" \
	"$(INTDIR)/j6iProfDlg.obj" \
	"$(INTDIR)/j6icnv.res"

"$(OUTDIR)\j6icnv.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "j6icnv - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
OUTDIR=.\Debug
INTDIR=.\Debug

ALL : "$(OUTDIR)\j6icnv.exe" "$(OUTDIR)\j6icnv.hlp"

CLEAN : 
	-@erase ".\Debug\vc40.pdb"
	-@erase ".\Debug\j6icnv.pch"
	-@erase ".\Debug\vc40.idb"
	-@erase ".\Debug\j6icnv.exe"
	-@erase ".\Debug\StdAfx.obj"
	-@erase ".\Debug\j6icnvDlg.obj"
	-@erase ".\Debug\j6icnv.obj"
	-@erase ".\Debug\j6iProfDlg.obj"
	-@erase ".\Debug\j6icnv.res"
	-@erase ".\Debug\j6icnv.ilk"
	-@erase ".\Debug\j6icnv.pdb"
	-@erase ".\Debug\j6icnv.hlp"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /c
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)/j6icnv.pch" /Yu"stdafx.h" /Fo"$(INTDIR)/"\
 /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\Debug/
CPP_SBRS=
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /win32
MTL_PROJ=/nologo /D "_DEBUG" /win32 
# ADD BASE RSC /l 0x411 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x411 /d "_DEBUG" /d "_AFXDLL"
RSC_PROJ=/l 0x411 /fo"$(INTDIR)/j6icnv.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/j6icnv.bsc" 
BSC32_SBRS=
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386
LINK32_FLAGS=/nologo /subsystem:windows /incremental:yes\
 /pdb:"$(OUTDIR)/j6icnv.pdb" /debug /machine:I386 /out:"$(OUTDIR)/j6icnv.exe" 
LINK32_OBJS= \
	"$(INTDIR)/StdAfx.obj" \
	"$(INTDIR)/j6icnvDlg.obj" \
	"$(INTDIR)/j6icnv.obj" \
	"$(INTDIR)/j6iProfDlg.obj" \
	"$(INTDIR)/j6icnv.res"

"$(OUTDIR)\j6icnv.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 

.c{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.c{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

################################################################################
# Begin Target

# Name "j6icnv - Win32 Release"
# Name "j6icnv - Win32 Debug"

!IF  "$(CFG)" == "j6icnv - Win32 Release"

!ELSEIF  "$(CFG)" == "j6icnv - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\ReadMe.txt

!IF  "$(CFG)" == "j6icnv - Win32 Release"

!ELSEIF  "$(CFG)" == "j6icnv - Win32 Debug"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\j6icnv.cpp
DEP_CPP_J6ICN=\
	".\StdAfx.h"\
	".\j6icnv.h"\
	".\j6icnvDlg.h"\
	

"$(INTDIR)\j6icnv.obj" : $(SOURCE) $(DEP_CPP_J6ICN) "$(INTDIR)"\
 "$(INTDIR)\j6icnv.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\j6icnvDlg.cpp

!IF  "$(CFG)" == "j6icnv - Win32 Release"

DEP_CPP_J6ICNV=\
	".\StdAfx.h"\
	".\j6icnv.h"\
	".\j6icnvDlg.h"\
	".\j6iProfDlg.h"\
	

"$(INTDIR)\j6icnvDlg.obj" : $(SOURCE) $(DEP_CPP_J6ICNV) "$(INTDIR)"\
 "$(INTDIR)\j6icnv.pch"


!ELSEIF  "$(CFG)" == "j6icnv - Win32 Debug"

DEP_CPP_J6ICNV=\
	".\StdAfx.h"\
	".\j6icnv.h"\
	".\j6icnvDlg.h"\
	".\j6iProfDlg.h"\
	
NODEP_CPP_J6ICNV=\
	".\is_param_vw"\
	

"$(INTDIR)\j6icnvDlg.obj" : $(SOURCE) $(DEP_CPP_J6ICNV) "$(INTDIR)"\
 "$(INTDIR)\j6icnv.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\StdAfx.cpp
DEP_CPP_STDAF=\
	".\StdAfx.h"\
	

!IF  "$(CFG)" == "j6icnv - Win32 Release"

# ADD CPP /Yc"stdafx.h"

BuildCmds= \
	$(CPP) /nologo /MD /W3 /GX /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL"\
 /D "_MBCS" /Fp"$(INTDIR)/j6icnv.pch" /Yc"stdafx.h" /Fo"$(INTDIR)/" /c $(SOURCE) \
	

"$(INTDIR)\StdAfx.obj" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\j6icnv.pch" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "j6icnv - Win32 Debug"

# ADD CPP /Yc"stdafx.h"

BuildCmds= \
	$(CPP) /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)/j6icnv.pch" /Yc"stdafx.h" /Fo"$(INTDIR)/"\
 /Fd"$(INTDIR)/" /c $(SOURCE) \
	

"$(INTDIR)\StdAfx.obj" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\j6icnv.pch" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\j6icnv.rc
DEP_RSC_J6ICNV_=\
	".\res\idr_main.ico"\
	".\res\j6icnv.rc2"\
	

"$(INTDIR)\j6icnv.res" : $(SOURCE) $(DEP_RSC_J6ICNV_) "$(INTDIR)"
   $(RSC) $(RSC_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\hlp\j6icnv.hpj

!IF  "$(CFG)" == "j6icnv - Win32 Release"

# Begin Custom Build - ﾍﾙﾌﾟ ﾌｧｲﾙを作成しています...
OutDir=.\Release
ProjDir=.
TargetName=j6icnv
InputPath=.\hlp\j6icnv.hpj

"$(OutDir)\$(TargetName).hlp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   "$(ProjDir)\makehelp.bat"

# End Custom Build

!ELSEIF  "$(CFG)" == "j6icnv - Win32 Debug"

# Begin Custom Build - ﾍﾙﾌﾟ ﾌｧｲﾙを作成しています...
OutDir=.\Debug
ProjDir=.
TargetName=j6icnv
InputPath=.\hlp\j6icnv.hpj

"$(OutDir)\$(TargetName).hlp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   "$(ProjDir)\makehelp.bat"

# End Custom Build

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\j6iProfDlg.cpp
DEP_CPP_J6IPR=\
	".\StdAfx.h"\
	".\j6icnv.h"\
	".\j6iProfDlg.h"\
	

"$(INTDIR)\j6iProfDlg.obj" : $(SOURCE) $(DEP_CPP_J6IPR) "$(INTDIR)"\
 "$(INTDIR)\j6icnv.pch"


# End Source File
# End Target
# End Project
################################################################################
