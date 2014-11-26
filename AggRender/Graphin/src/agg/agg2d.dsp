# Microsoft Developer Studio Project File - Name="agg2d" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=agg2d - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "agg2d.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "agg2d.mak" CFG="agg2d - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "agg2d - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "agg2d - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "agg2d - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\build\release\agg2d"
# PROP Intermediate_Dir "..\..\build\release\agg2d"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /G6 /MT /W3 /GX /Ox /Ot /Oa /Og /Oi /Op /Ob2 /I "./include" /I "./font_win32_tt" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D "AGG_BMP_ALPHA_BLEND" /YX /FD /c
# SUBTRACT CPP /Ow /Os
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "agg2d - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\build\debug\agg2d"
# PROP Intermediate_Dir "..\..\build\debug\agg2d"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "./include" /I "./font_win32_tt" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "AGG_BMP_ALPHA_BLEND" /FD /GZ /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "agg2d - Win32 Release"
# Name "agg2d - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\src\agg2D.cpp
# End Source File
# Begin Source File

SOURCE=.\src\agg_arc.cpp
# End Source File
# Begin Source File

SOURCE=.\src\agg_bezier_arc.cpp
# End Source File
# Begin Source File

SOURCE=.\src\ctrl\agg_cbox_ctrl.cpp
# End Source File
# Begin Source File

SOURCE=.\src\agg_curves.cpp
# End Source File
# Begin Source File

SOURCE=.\font_win32_tt\agg_font_win32_tt.cpp
# End Source File
# Begin Source File

SOURCE=.\src\agg_gsv_text.cpp
# End Source File
# Begin Source File

SOURCE=.\src\agg_image_filters.cpp
# End Source File
# Begin Source File

SOURCE=.\src\agg_line_aa_basics.cpp
# End Source File
# Begin Source File

SOURCE=.\src\agg_line_profile_aa.cpp
# End Source File
# Begin Source File

SOURCE=.\src\platform\win32\agg_platform_support.cpp
# End Source File
# Begin Source File

SOURCE=.\src\agg_rounded_rect.cpp
# End Source File
# Begin Source File

SOURCE=.\src\ctrl\agg_slider_ctrl.cpp
# End Source File
# Begin Source File

SOURCE=.\src\agg_sqrt_tables.cpp
# End Source File
# Begin Source File

SOURCE=.\src\agg_trans_affine.cpp
# End Source File
# Begin Source File

SOURCE=.\src\agg_vcgen_stroke.cpp
# End Source File
# Begin Source File

SOURCE=.\src\platform\win32\agg_win32_bmp.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\include\agg2D.h
# End Source File
# Begin Source File

SOURCE=.\include\platform\agg_platform_support.h
# End Source File
# Begin Source File

SOURCE=.\include\platform\win32\agg_win32_bmp.h
# End Source File
# End Group
# End Target
# End Project
