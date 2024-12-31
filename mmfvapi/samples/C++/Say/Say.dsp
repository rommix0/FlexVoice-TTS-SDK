# Microsoft Developer Studio Project File - Name="Say" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=Say - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Say.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Say.mak" CFG="Say - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Say - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "Say - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
CPP=xicl6.exe
RSC=rc.exe

!IF  "$(CFG)" == "Say - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Say___Win32_Debug"
# PROP BASE Intermediate_Dir "Say___Win32_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GR /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /FD /GZ /c
# SUBTRACT BASE CPP /YX
# ADD CPP /nologo /MTd /W3 /Gm /GR /GX /Zi /Od /I "..\..\..\include\stl-patch" /I "..\..\..\include" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /FD /GZ /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=xilink6.exe
# ADD BASE LINK32 FlexVoiced.lib /nologo /subsystem:console /debug /machine:I386 /out:"Debug/t2w.exe" /pdbtype:sept
# ADD LINK32 FlexVoice_3_01_001.lib advapi32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept /libpath:"..\..\..\lib"

!ELSEIF  "$(CFG)" == "Say - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Say___Win32_Release"
# PROP BASE Intermediate_Dir "Say___Win32_Release"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GR /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /FD /c
# SUBTRACT BASE CPP /YX
# ADD CPP /nologo /MT /W3 /GR /GX /O2 /I "..\..\..\include\stl-patch" /I "..\..\..\include" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=xilink6.exe
# ADD BASE LINK32 FlexVoice.lib /nologo /subsystem:console /machine:I386 /out:"Release/t2w.exe"
# ADD LINK32 FlexVoice_3_01_001.lib advapi32.lib /nologo /subsystem:console /machine:I386 /libpath:"..\..\..\lib"

!ENDIF 

# Begin Target

# Name "Say - Win32 Debug"
# Name "Say - Win32 Release"
# Begin Source File

SOURCE=.\Description.txt
# End Source File
# Begin Source File

SOURCE=.\main.cpp
# End Source File
# Begin Source File

SOURCE=.\PhonemeFileWriter.cpp
# End Source File
# Begin Source File

SOURCE=.\PhonemeFileWriter.h
# End Source File
# Begin Source File

SOURCE=.\SpeechParams.h
# End Source File
# Begin Source File

SOURCE=.\TextToWav.cpp
# End Source File
# Begin Source File

SOURCE=.\TextToWav.h
# End Source File
# Begin Source File

SOURCE=.\Tools.h
# End Source File
# Begin Source File

SOURCE=.\WavFileWriter.cpp
# End Source File
# Begin Source File

SOURCE=.\WavFileWriter.h
# End Source File
# End Target
# End Project
