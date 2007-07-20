; example2.nsi
;
; This script is based on example1.nsi, but it remember the directory, 
; has uninstall support and (optionally) installs start menu shortcuts.
;
; It will install example2.nsi into a directory that the user selects,

;--------------------------------

;--------------------------------
;Include Modern UI

  !include "MUI.nsh"

;--------------------------------
;General

; The name of the installer
Name "Alcominance Game"

; The file to write
OutFile "Alcominance.exe"

; The default installation directory
InstallDir $PROGRAMFILES\Alcominance

; Registry key to check for directory (so if you install again, it will 
; overwrite the old one automatically)
InstallDirRegKey HKLM "Software\Alcominance" "Install_Dir"

;--------------------------------
;Interface Configuration

  !define MUI_HEADERIMAGE
  !define MUI_HEADERIMAGE_BITMAP "..\resource\nsis.bmp" ; optional
  !define MUI_ABORTWARNING
  !define MUI_ICON "..\resource\install.ico"
  !define MUI_UNICON "${NSISDIR}\Contrib\Graphics\Icons\orange-uninstall.ico"

;--------------------------------
;Pages

  !insertmacro MUI_PAGE_LICENSE "${NSISDIR}\Docs\Modern UI\License.txt"
  !insertmacro MUI_PAGE_COMPONENTS
  !insertmacro MUI_PAGE_DIRECTORY
  !insertmacro MUI_PAGE_INSTFILES
  
  !insertmacro MUI_UNPAGE_CONFIRM
  !insertmacro MUI_UNPAGE_INSTFILES
  
;--------------------------------
;Languages
  ;!insertmacro MUI_LANGUAGE "English"
  !insertmacro MUI_LANGUAGE "Czech"
;--------------------------------
;Language Selection Dialog Settings

  ;Remember the installer language
  !define MUI_LANGDLL_REGISTRY_ROOT "HKCU" 
  !define MUI_LANGDLL_REGISTRY_KEY "Software\Alcominance" 
  !define MUI_LANGDLL_REGISTRY_VALUENAME "Installer Language"


;--------------------------------

; The stuff to install
Section "Game files"

  SectionIn RO
  
  ; Set output path to the installation directory.
  SetOutPath $INSTDIR
  
  ; exe files
  ; File "..\Alcominance_d.exe"
  File "..\Alcominance.exe"
  ; File "..\BecherEditor_d.exe"
  File "..\BecherEditor.exe"
  File "..\..\Common\bin\BASSMOD.dll"
  File "..\..\Common\bin\OpenAL32.dll"
  File "..\..\Common\bin\wrap_oal.dll"
  ; File "..\..\Hoe\bin\Hoe3Dd_GL.dll"
  File "..\..\Hoe\bin\Hoe3D_D3D9.dll"
  File "..\..\Hoe\bin\Hoe3D_D3D8.dll"
  File "..\..\Hoe\bin\Hoe3D_GL.dll"
  
  File "install\becher.conf"
  File "..\docs\Readme.txt"
  File "..\docs\Napoveda.doc"
  File "..\docs\Napoveda_en.doc"
  
  SetOutPath "$INSTDIR\data"
  File "..\data\*"
  SetOutPath "$INSTDIR\data_old"
  File "..\data_old\*"
  SetOutPath "$INSTDIR\maps"
  File "..\maps\*"
  SetOutPath "$INSTDIR\resource"
  File "..\resource\*"
  SetOutPath "$INSTDIR\scripts"
  File "..\scripts\*"
  SetOutPath "$INSTDIR\sound"
  File "..\sound\*.ogg"
  File "..\sound\music\*"
  SetOutPath "$INSTDIR\save"
  SetOutPath $INSTDIR
  
  ; Write the installation path into the registry
  WriteRegStr HKLM SOFTWARE\Alcominance "Install_Dir" "$INSTDIR"
  
  ; Write the uninstall keys for Windows
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Alcominance" "DisplayName" "Alcominance Game"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Alcominance" "UninstallString" '"$INSTDIR\uninstall.exe"'
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Alcominance" "NoModify" 1
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Alcominance" "NoRepair" 1
  WriteUninstaller "uninstall.exe"
  
SectionEnd

; Optional section (can be disabled by the user)
Section "Start Menu Shortcuts"

  CreateDirectory "$SMPROGRAMS\Alcominance"
  CreateShortCut "$SMPROGRAMS\Alcominance\Uninstall.lnk" "$INSTDIR\uninstall.exe" "" "$INSTDIR\uninstall.exe" 0
  CreateShortCut "$SMPROGRAMS\Alcominance\Alcominance.lnk" "$INSTDIR\Alcominance.exe" "" "$INSTDIR\Alcominance_d.exe" 0
  CreateShortCut "$SMPROGRAMS\Alcominance\Editor.lnk" "$INSTDIR\BecherEditor.exe" "" "$INSTDIR\BecherEditor_d.exe" 0
  CreateShortCut "$SMPROGRAMS\Alcominance\Nastaveni.lnk" "$INSTDIR\Alcominance_d.exe -h" "" "$INSTDIR\Alcominance_d.exe -h" 0
  CreateShortCut "$SMPROGRAMS\Alcominance\Readme.lnk" "$INSTDIR\readme.txt" "" "$INSTDIR\readme.txt" 0
  CreateShortCut "$SMPROGRAMS\Alcominance\Heimdall on web.lnk" "http://www.gejza.net" "" "http://www.gejza.net" 0
  
SectionEnd


;--------------------------------
;Installer Functions

Function .onInit

  !insertmacro MUI_LANGDLL_DISPLAY

FunctionEnd


;--------------------------------

; Uninstaller

Section "Uninstall"
  
  ; Remove registry keys
  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Alcominance"
  DeleteRegKey HKLM SOFTWARE\Alcominance

  ; odebrat data files
  Delete "$INSTDIR\data\*.*"
  Delete "$INSTDIR\resource\*.*"
  Delete "$INSTDIR\scripts\*.*"
  Delete "$INSTDIR\sound\*.*"
	
  ; Remove files and uninstaller
  Delete $INSTDIR\*.exe
  ;Delete $INSTDIR\Uninstall.exe
  Delete $INSTDIR\*.dll
  ;Delete $INSTDIR\OpenAL32.dll
  ;Delete $INSTDIR\wrap_oal.dll
  ;Delete $INSTDIR\Hoe3Dd_GL.dll
  ;Delete $INSTDIR\Hoe3Dd_D3D9.dll

  ; Remove shortcuts, if any
  Delete "$SMPROGRAMS\Alcominance\*.*"

  ; Remove directories used
  RMDir "$SMPROGRAMS\Alcominance"
  RMDir "$INSTDIR"
  RMDir "$INSTDIR\data"
  RMDir "$INSTDIR\resource"
  RMDir "$INSTDIR\scripts"
  RMDir "$INSTDIR\sound"
  RMDir "$INSTDIR\maps"


SectionEnd

;--------------------------------
;Uninstaller Functions

Function un.onInit

  !insertmacro MUI_UNGETLANGUAGE
  
FunctionEnd