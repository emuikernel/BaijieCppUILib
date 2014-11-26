# Microsoft Developer Studio Generated NMAKE File, Based on png.dsp
!IF "$(CFG)" == ""
CFG=png - Win32 Release
!MESSAGE No configuration specified. Defaulting to png - Win32 Release.
!ENDIF 

!IF "$(CFG)" != "png - Win32 Release" && "$(CFG)" != "png - Win32 Debug" && "$(CFG)" != "png - Win32 ReleaseRetail" && "$(CFG)" != "png - Win32 ReleaseMinSize"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "png.mak" CFG="png - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "png - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "png - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "png - Win32 ReleaseRetail" (based on "Win32 (x86) Static Library")
!MESSAGE "png - Win32 ReleaseMinSize" (based on "Win32 (x86) Static Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "png - Win32 Release"

OUTDIR=.\..\..\Release
INTDIR=.\..\..\Release
# Begin Custom Macros
OutDir=.\..\..\Release
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\png.lib"

!ELSE 

ALL : "zlib - Win32 Release" "$(OUTDIR)\png.lib"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"zlib - Win32 ReleaseCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\png.obj"
	-@erase "$(INTDIR)\pngerror.obj"
	-@erase "$(INTDIR)\pnggccrd.obj"
	-@erase "$(INTDIR)\pngget.obj"
	-@erase "$(INTDIR)\pngmem.obj"
	-@erase "$(INTDIR)\pngpread.obj"
	-@erase "$(INTDIR)\pngread.obj"
	-@erase "$(INTDIR)\pngrio.obj"
	-@erase "$(INTDIR)\pngrtran.obj"
	-@erase "$(INTDIR)\pngrutil.obj"
	-@erase "$(INTDIR)\pngset.obj"
	-@erase "$(INTDIR)\pngtrans.obj"
	-@erase "$(INTDIR)\pngvcrd.obj"
	-@erase "$(INTDIR)\pngwio.obj"
	-@erase "$(INTDIR)\pngwrite.obj"
	-@erase "$(INTDIR)\pngwtran.obj"
	-@erase "$(INTDIR)\pngwutil.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\png.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MT /W3 /GX /O1 /I "..\zlib" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /Fp"$(INTDIR)\png.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\png.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\png.lib" 
LIB32_OBJS= \
	"$(INTDIR)\png.obj" \
	"$(INTDIR)\pngerror.obj" \
	"$(INTDIR)\pnggccrd.obj" \
	"$(INTDIR)\pngget.obj" \
	"$(INTDIR)\pngmem.obj" \
	"$(INTDIR)\pngpread.obj" \
	"$(INTDIR)\pngread.obj" \
	"$(INTDIR)\pngrio.obj" \
	"$(INTDIR)\pngrtran.obj" \
	"$(INTDIR)\pngrutil.obj" \
	"$(INTDIR)\pngset.obj" \
	"$(INTDIR)\pngtrans.obj" \
	"$(INTDIR)\pngvcrd.obj" \
	"$(INTDIR)\pngwio.obj" \
	"$(INTDIR)\pngwrite.obj" \
	"$(INTDIR)\pngwtran.obj" \
	"$(INTDIR)\pngwutil.obj" \
	"$(OUTDIR)\zlib.lib"

"$(OUTDIR)\png.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "png - Win32 Debug"

OUTDIR=.\..\..\Debug
INTDIR=.\..\..\Debug
# Begin Custom Macros
OutDir=.\..\..\Debug
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\png.lib"

!ELSE 

ALL : "zlib - Win32 Debug" "$(OUTDIR)\png.lib"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"zlib - Win32 DebugCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\png.obj"
	-@erase "$(INTDIR)\pngerror.obj"
	-@erase "$(INTDIR)\pnggccrd.obj"
	-@erase "$(INTDIR)\pngget.obj"
	-@erase "$(INTDIR)\pngmem.obj"
	-@erase "$(INTDIR)\pngpread.obj"
	-@erase "$(INTDIR)\pngread.obj"
	-@erase "$(INTDIR)\pngrio.obj"
	-@erase "$(INTDIR)\pngrtran.obj"
	-@erase "$(INTDIR)\pngrutil.obj"
	-@erase "$(INTDIR)\pngset.obj"
	-@erase "$(INTDIR)\pngtrans.obj"
	-@erase "$(INTDIR)\pngvcrd.obj"
	-@erase "$(INTDIR)\pngwio.obj"
	-@erase "$(INTDIR)\pngwrite.obj"
	-@erase "$(INTDIR)\pngwtran.obj"
	-@erase "$(INTDIR)\pngwutil.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\png.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MTd /W3 /Gm /GX /ZI /Od /I "..\zlib" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\png.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\png.lib" 
LIB32_OBJS= \
	"$(INTDIR)\png.obj" \
	"$(INTDIR)\pngerror.obj" \
	"$(INTDIR)\pnggccrd.obj" \
	"$(INTDIR)\pngget.obj" \
	"$(INTDIR)\pngmem.obj" \
	"$(INTDIR)\pngpread.obj" \
	"$(INTDIR)\pngread.obj" \
	"$(INTDIR)\pngrio.obj" \
	"$(INTDIR)\pngrtran.obj" \
	"$(INTDIR)\pngrutil.obj" \
	"$(INTDIR)\pngset.obj" \
	"$(INTDIR)\pngtrans.obj" \
	"$(INTDIR)\pngvcrd.obj" \
	"$(INTDIR)\pngwio.obj" \
	"$(INTDIR)\pngwrite.obj" \
	"$(INTDIR)\pngwtran.obj" \
	"$(INTDIR)\pngwutil.obj" \
	"$(OUTDIR)\zlib.lib"

"$(OUTDIR)\png.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "png - Win32 ReleaseRetail"

OUTDIR=.\..\..\ReleaseRetail
INTDIR=.\..\..\ReleaseRetail
# Begin Custom Macros
OutDir=.\..\..\ReleaseRetail
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\png.lib"

!ELSE 

ALL : "zlib - Win32 ReleaseRetail" "$(OUTDIR)\png.lib"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"zlib - Win32 ReleaseRetailCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\png.obj"
	-@erase "$(INTDIR)\pngerror.obj"
	-@erase "$(INTDIR)\pnggccrd.obj"
	-@erase "$(INTDIR)\pngget.obj"
	-@erase "$(INTDIR)\pngmem.obj"
	-@erase "$(INTDIR)\pngpread.obj"
	-@erase "$(INTDIR)\pngread.obj"
	-@erase "$(INTDIR)\pngrio.obj"
	-@erase "$(INTDIR)\pngrtran.obj"
	-@erase "$(INTDIR)\pngrutil.obj"
	-@erase "$(INTDIR)\pngset.obj"
	-@erase "$(INTDIR)\pngtrans.obj"
	-@erase "$(INTDIR)\pngvcrd.obj"
	-@erase "$(INTDIR)\pngwio.obj"
	-@erase "$(INTDIR)\pngwrite.obj"
	-@erase "$(INTDIR)\pngwtran.obj"
	-@erase "$(INTDIR)\pngwutil.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\png.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MT /W3 /GX /O1 /I "..\zlib" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /Fp"$(INTDIR)\png.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\png.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\png.lib" 
LIB32_OBJS= \
	"$(INTDIR)\png.obj" \
	"$(INTDIR)\pngerror.obj" \
	"$(INTDIR)\pnggccrd.obj" \
	"$(INTDIR)\pngget.obj" \
	"$(INTDIR)\pngmem.obj" \
	"$(INTDIR)\pngpread.obj" \
	"$(INTDIR)\pngread.obj" \
	"$(INTDIR)\pngrio.obj" \
	"$(INTDIR)\pngrtran.obj" \
	"$(INTDIR)\pngrutil.obj" \
	"$(INTDIR)\pngset.obj" \
	"$(INTDIR)\pngtrans.obj" \
	"$(INTDIR)\pngvcrd.obj" \
	"$(INTDIR)\pngwio.obj" \
	"$(INTDIR)\pngwrite.obj" \
	"$(INTDIR)\pngwtran.obj" \
	"$(INTDIR)\pngwutil.obj" \
	"$(OUTDIR)\zlib.lib"

"$(OUTDIR)\png.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "png - Win32 ReleaseMinSize"

OUTDIR=.\png___Win32_ReleaseMinSize
INTDIR=.\png___Win32_ReleaseMinSize
# Begin Custom Macros
OutDir=.\png___Win32_ReleaseMinSize
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\png.lib"

!ELSE 

ALL : "zlib - Win32 ReleaseMinSize" "$(OUTDIR)\png.lib"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"zlib - Win32 ReleaseMinSizeCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\png.obj"
	-@erase "$(INTDIR)\pngerror.obj"
	-@erase "$(INTDIR)\pnggccrd.obj"
	-@erase "$(INTDIR)\pngget.obj"
	-@erase "$(INTDIR)\pngmem.obj"
	-@erase "$(INTDIR)\pngpread.obj"
	-@erase "$(INTDIR)\pngread.obj"
	-@erase "$(INTDIR)\pngrio.obj"
	-@erase "$(INTDIR)\pngrtran.obj"
	-@erase "$(INTDIR)\pngrutil.obj"
	-@erase "$(INTDIR)\pngset.obj"
	-@erase "$(INTDIR)\pngtrans.obj"
	-@erase "$(INTDIR)\pngvcrd.obj"
	-@erase "$(INTDIR)\pngwio.obj"
	-@erase "$(INTDIR)\pngwrite.obj"
	-@erase "$(INTDIR)\pngwtran.obj"
	-@erase "$(INTDIR)\pngwutil.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\png.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MT /W3 /GX /O1 /I "..\zlib" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /Fp"$(INTDIR)\png.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\png.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\png.lib" 
LIB32_OBJS= \
	"$(INTDIR)\png.obj" \
	"$(INTDIR)\pngerror.obj" \
	"$(INTDIR)\pnggccrd.obj" \
	"$(INTDIR)\pngget.obj" \
	"$(INTDIR)\pngmem.obj" \
	"$(INTDIR)\pngpread.obj" \
	"$(INTDIR)\pngread.obj" \
	"$(INTDIR)\pngrio.obj" \
	"$(INTDIR)\pngrtran.obj" \
	"$(INTDIR)\pngrutil.obj" \
	"$(INTDIR)\pngset.obj" \
	"$(INTDIR)\pngtrans.obj" \
	"$(INTDIR)\pngvcrd.obj" \
	"$(INTDIR)\pngwio.obj" \
	"$(INTDIR)\pngwrite.obj" \
	"$(INTDIR)\pngwtran.obj" \
	"$(INTDIR)\pngwutil.obj" \
	"..\ZLIB\zlib___Win32_ReleaseMinSize\zlib.lib"

"$(OUTDIR)\png.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ENDIF 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("png.dep")
!INCLUDE "png.dep"
!ELSE 
!MESSAGE Warning: cannot find "png.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "png - Win32 Release" || "$(CFG)" == "png - Win32 Debug" || "$(CFG)" == "png - Win32 ReleaseRetail" || "$(CFG)" == "png - Win32 ReleaseMinSize"
SOURCE=.\png.c

"$(INTDIR)\png.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\pngerror.c

"$(INTDIR)\pngerror.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\pnggccrd.c

"$(INTDIR)\pnggccrd.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\pngget.c

"$(INTDIR)\pngget.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\pngmem.c

"$(INTDIR)\pngmem.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\pngpread.c

"$(INTDIR)\pngpread.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\pngread.c

"$(INTDIR)\pngread.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\pngrio.c

"$(INTDIR)\pngrio.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\pngrtran.c

"$(INTDIR)\pngrtran.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\pngrutil.c

"$(INTDIR)\pngrutil.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\pngset.c

"$(INTDIR)\pngset.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\pngtrans.c

"$(INTDIR)\pngtrans.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\pngvcrd.c

"$(INTDIR)\pngvcrd.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\pngwio.c

"$(INTDIR)\pngwio.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\pngwrite.c

"$(INTDIR)\pngwrite.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\pngwtran.c

"$(INTDIR)\pngwtran.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\pngwutil.c

"$(INTDIR)\pngwutil.obj" : $(SOURCE) "$(INTDIR)"


!IF  "$(CFG)" == "png - Win32 Release"

"zlib - Win32 Release" : 
   cd "\layout\hsmile.v3\external\ZLIB"
   $(MAKE) /$(MAKEFLAGS) /F .\zlib.mak CFG="zlib - Win32 Release" 
   cd "..\PNG"

"zlib - Win32 ReleaseCLEAN" : 
   cd "\layout\hsmile.v3\external\ZLIB"
   $(MAKE) /$(MAKEFLAGS) /F .\zlib.mak CFG="zlib - Win32 Release" RECURSE=1 CLEAN 
   cd "..\PNG"

!ELSEIF  "$(CFG)" == "png - Win32 Debug"

"zlib - Win32 Debug" : 
   cd "\layout\hsmile.v3\external\ZLIB"
   $(MAKE) /$(MAKEFLAGS) /F .\zlib.mak CFG="zlib - Win32 Debug" 
   cd "..\PNG"

"zlib - Win32 DebugCLEAN" : 
   cd "\layout\hsmile.v3\external\ZLIB"
   $(MAKE) /$(MAKEFLAGS) /F .\zlib.mak CFG="zlib - Win32 Debug" RECURSE=1 CLEAN 
   cd "..\PNG"

!ELSEIF  "$(CFG)" == "png - Win32 ReleaseRetail"

"zlib - Win32 ReleaseRetail" : 
   cd "\layout\hsmile.v3\external\ZLIB"
   $(MAKE) /$(MAKEFLAGS) /F .\zlib.mak CFG="zlib - Win32 ReleaseRetail" 
   cd "..\PNG"

"zlib - Win32 ReleaseRetailCLEAN" : 
   cd "\layout\hsmile.v3\external\ZLIB"
   $(MAKE) /$(MAKEFLAGS) /F .\zlib.mak CFG="zlib - Win32 ReleaseRetail" RECURSE=1 CLEAN 
   cd "..\PNG"

!ELSEIF  "$(CFG)" == "png - Win32 ReleaseMinSize"

"zlib - Win32 ReleaseMinSize" : 
   cd "\layout\hsmile.v3\external\ZLIB"
   $(MAKE) /$(MAKEFLAGS) /F .\zlib.mak CFG="zlib - Win32 ReleaseMinSize" 
   cd "..\PNG"

"zlib - Win32 ReleaseMinSizeCLEAN" : 
   cd "\layout\hsmile.v3\external\ZLIB"
   $(MAKE) /$(MAKEFLAGS) /F .\zlib.mak CFG="zlib - Win32 ReleaseMinSize" RECURSE=1 CLEAN 
   cd "..\PNG"

!ENDIF 


!ENDIF 

