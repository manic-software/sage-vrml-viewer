OutFile "sage-setup.exe"
Name "SAGE"
CRCCheck on
SetCompressor /SOLID lzma
RequestExecutionLevel admin
InstallDirRegKey HKLM "Software\SAGE\SAGE" "Install_Dir"
InstallDir "$PROGRAMFILES\SAGE\"

Section "SAGE (required)" core
        SectionIn RO
        SetOutPath "$INSTDIR"
        File "sage.exe"
	File "sage.pdf"
;registry stuff for uninstall and uninstall itself
        WriteRegStr HKLM "Software\SAGE" "Install_Dir" "$INSTDIR"
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\SAGE" "DisplayName" "SAGE (remove only)"
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\SAGE" "UninstallString" "$INSTDIR\Uninstall.exe"
	WriteUninstaller "$INSTDIR\Uninstall.exe"
	WriteRegStr HKCR ".wrl" "" "SAGE.Document"
	WriteRegStr HKCR "SAGE.Document" "" "VRML File"
	WriteRegStr HKCR "SAGE.Document\DefaultIcon" "" "$INSTDIR/sage.exe,1"
	WriteRegStr HKCR "SAGE.Document\shell\open\command" "" '"$INSTDIR\sage.exe" "%1"' 
SectionEnd

Section "MingW Runtime" mingw
        SetOutPath "$SYSDIR"
	SetOverwrite ifnewer
        File "C:\mingw32\bin\libgcc_s_dw2-1.dll"
        File "C:\mingw32\bin\libstdc++-6.dll"
SectionEnd

Section "Qt Binaries" qt
        SetOutPath "$SYSDIR"
	SetOverwrite ifnewer
        File "C:\Qt\4.8.6\bin\QtCore4.dll"
        File "C:\Qt\4.8.6\bin\QtGui4.dll"
        File "C:\Qt\4.8.6\bin\QtOpenGL4.dll"
        File "C:\Qt\4.8.6\bin\Qt3Support4.dll"
        File "C:\Qt\4.8.6\bin\QtNetwork4.dll"
        File "C:\Qt\4.8.6\bin\QtSql4.dll"
        File "C:\Qt\4.8.6\bin\QtXml4.dll"
SectionEnd

Section "Start Menu Shortcuts" startmenu
        CreateShortCut "$SMPROGRAMS\SAGE.lnk" "$INSTDIR\sage.exe" "" "$INSTDIR\sage.exe" 0
SectionEnd

Section "Desktop Shortcut" desktop
        CreateShortCut "$DESKTOP\SAGE.lnk" "$INSTDIR\sage.exe" "" "$INSTDIR\sage.exe" 0
SectionEnd

Section "Uninstall"
	RMDir /r "$INSTDIR\*.*"
	RMDIR "$INSTDIR"
	Delete "$DESKTOP\SAGE.lnk"
	Delete "$SMPROGRAMS\SAGE.lnk"
        DeleteRegKey HKLM "Software\SAGE"
	DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\SAGE"
	DeleteRegKey HKCR ".wrl"
	DeleteRegKey HKCR "SAGE.Document"
	DeleteRegKey HKCR "SAGE.Document\DefaultIcon"
	DeleteRegKey HKCR "SAGE.Document\shell\open\command"
SectionEnd
