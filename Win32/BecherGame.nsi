!define PRODUCT_NAME "Alcominance"
!define PRODUCT_VERSION "1.0"
!define PRODUCT_PUBLISHER "Heimdall"
!define PRODUCT_WEB_SITE "http://www.gejza.net"
!define PRODUCT_DIR_REGKEY "Software\Microsoft\Windows\CurrentVersion\App Paths\Alcominance.exe"
!define PRODUCT_UNINST_KEY "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}"
!define PRODUCT_UNINST_ROOT_KEY "HKLM"
!define PRODUCT_STARTMENU_REGVAL "NSIS:StartMenuDir"


; MUI 1.67 compatible ------
!include "MUI.nsh"

!include "E:\Program Files\NSIS\Contrib\Language files\czech.nsh"

; MUI Settings
!define MUI_ABORTWARNING
!define MUI_ICON "..\resource\logo.ico"
!define MUI_UNICON "..\resource\logo.ico"

; Language Selection Dialog Settings
!define MUI_LANGDLL_REGISTRY_ROOT "${PRODUCT_UNINST_ROOT_KEY}"
!define MUI_LANGDLL_REGISTRY_KEY "${PRODUCT_UNINST_KEY}"
;!define MUI_LANGDLL_REGISTRY_VALUENAME "NSIS:Language"
 !define MUI_LANGDLL_REGISTRY_VALUENAME "Installer Language"


; Welcome page
!insertmacro MUI_PAGE_WELCOME
; License page
!insertmacro MUI_PAGE_LICENSE "license.txt"
; Components page
!insertmacro MUI_PAGE_COMPONENTS
; Directory page
!insertmacro MUI_PAGE_DIRECTORY
; Start menu page
var ICONS_GROUP
!define MUI_STARTMENUPAGE_NODISABLE
!define MUI_STARTMENUPAGE_DEFAULTFOLDER "Alcominance"
!define MUI_STARTMENUPAGE_REGISTRY_ROOT "${PRODUCT_UNINST_ROOT_KEY}"
!define MUI_STARTMENUPAGE_REGISTRY_KEY "${PRODUCT_UNINST_KEY}"
!define MUI_STARTMENUPAGE_REGISTRY_VALUENAME "${PRODUCT_STARTMENU_REGVAL}"
!insertmacro MUI_PAGE_STARTMENU Application $ICONS_GROUP
; Instfiles page
!insertmacro MUI_PAGE_INSTFILES
; Finish page
!define MUI_FINISHPAGE_RUN "$INSTDIR\Alcominance.exe"
!define MUI_FINISHPAGE_SHOWREADME "$INSTDIR\Napoveda_en.doc"
!insertmacro MUI_PAGE_FINISH

; Uninstaller pages
!insertmacro MUI_UNPAGE_INSTFILES

; Language files
!insertmacro MUI_LANGUAGE "Czech"
!insertmacro MUI_LANGUAGE "English"

; MUI end ------

Name "${PRODUCT_NAME} ${PRODUCT_VERSION}"
OutFile "Setup.exe"
InstallDir "$PROGRAMFILES\Alcominance"
InstallDirRegKey HKLM "${PRODUCT_DIR_REGKEY}" ""
ShowInstDetails show
ShowUnInstDetails show

Function .onInit
  !insertmacro MUI_LANGDLL_DISPLAY
FunctionEnd

Section "game" SEC01
  SetOutPath "$INSTDIR"
  SetOverwrite try
  File "install\Napoveda.doc"
  File "install\Napoveda_en.doc"
  File "install\Alcominance.exe"
  File "install\Alcominance_d.exe"
  File "install\becher.conf"
  File "install\Hoe3D_GL.dll"
  File "install\Hoe3Dd_D3D8.dll"
  File "install\Hoe3Dd_D3D9.dll"
  File "install\Hoe3Dd_GL.dll"
  File "install\BASSMOD.dll"
  File "install\OpenAL32.dll"
  File "install\wrap_oal.dll"
  SetOverwrite ifnewer
  File "license.txt"

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


; Shortcuts
  !insertmacro MUI_STARTMENU_WRITE_BEGIN Application
  CreateDirectory "$SMPROGRAMS\$ICONS_GROUP"
  CreateShortCut "$SMPROGRAMS\$ICONS_GROUP\Alcominance.lnk" "$INSTDIR\Alcominance.exe"
  CreateShortCut "$DESKTOP\Alcominance.lnk" "$INSTDIR\Alcominance.exe"
  !insertmacro MUI_STARTMENU_WRITE_END

  ; Write the installation path into the registry
;  WriteRegStr HKLM SOFTWARE\Alcominance "Install_Dir" "$INSTDIR"
;  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Alcominance" "NoModify" 1
;  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Alcominance" "NoRepair" 1

SectionEnd

Section "editor" SEC02
  File "install\BecherEditor.exe"
  File "install\BecherEditor_d.exe"

; Shortcuts
  !insertmacro MUI_STARTMENU_WRITE_BEGIN Application
 CreateShortCut "$ICONS_GROUP.lnk" "$INSTDIR\BecherEditor.exe"
  !insertmacro MUI_STARTMENU_WRITE_END
SectionEnd

Section -AdditionalIcons
  !insertmacro MUI_STARTMENU_WRITE_BEGIN Application
  WriteIniStr "$INSTDIR\${PRODUCT_NAME}.url" "InternetShortcut" "URL" "${PRODUCT_WEB_SITE}"
  CreateShortCut "$SMPROGRAMS\$ICONS_GROUP\Heimdall on web.lnk" "$INSTDIR\${PRODUCT_NAME}.url"
  CreateShortCut "$SMPROGRAMS\$ICONS_GROUP\Uninstall.lnk" "$INSTDIR\uninst.exe"
  !insertmacro MUI_STARTMENU_WRITE_END
SectionEnd

Section -Post
  WriteUninstaller "$INSTDIR\uninst.exe"
  WriteRegStr HKLM "${PRODUCT_DIR_REGKEY}" "" "$INSTDIR\Alcominance.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayName" "$(^Name)"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "UninstallString" "$INSTDIR\uninst.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayIcon" "$INSTDIR\Alcominance.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayVersion" "${PRODUCT_VERSION}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "URLInfoAbout" "${PRODUCT_WEB_SITE}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "Publisher" "${PRODUCT_PUBLISHER}"
SectionEnd

; Section descriptions
!insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
  !insertmacro MUI_DESCRIPTION_TEXT ${SEC01} ""
  !insertmacro MUI_DESCRIPTION_TEXT ${SEC02} ""
!insertmacro MUI_FUNCTION_DESCRIPTION_END


Function un.onUninstSuccess
  HideWindow
  MessageBox MB_ICONINFORMATION|MB_OK "Alcominance was successfully removed from your computer."
FunctionEnd

Function un.onInit
!insertmacro MUI_UNGETLANGUAGE
  MessageBox MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2 "Are you sure you want to completely remove Alcominance and all of its components?" IDYES +2
  Abort

FunctionEnd


Section Uninstall
  !insertmacro MUI_STARTMENU_GETFOLDER "Application" $ICONS_GROUP
  Delete "$INSTDIR\${PRODUCT_NAME}.url"
  Delete "$INSTDIR\uninst.exe"
  Delete "$INSTDIR\BecherEditor_d.exe"
  Delete "$INSTDIR\BecherEditor.exe"
  Delete "$INSTDIR\license.txt"
  Delete "$INSTDIR\wrap_oal.dll"
  Delete "$INSTDIR\OpenAL32.dll"
  Delete "$INSTDIR\BASSMOD.dll"
  Delete "$INSTDIR\Hoe3Dd_GL.dll"
  Delete "$INSTDIR\Hoe3Dd_D3D9.dll"
  Delete "$INSTDIR\Hoe3Dd_D3D8.dll"
  Delete "$INSTDIR\Hoe3D_GL.dll"
  Delete "$INSTDIR\becher.conf"
  Delete "$INSTDIR\Alcominance_d.exe"
  Delete "$INSTDIR\Alcominance.exe"
  Delete "$INSTDIR\Napoveda_en.doc"
  Delete "$INSTDIR\Napoveda.doc"
  Delete "$INSTDIR\Alcominance.lnk"
  Delete "$INSTDIR\becher.log"

  Delete "$INSTDIR\data\*.*"
  Delete "$INSTDIR\resource\*.*"
  Delete "$INSTDIR\scripts\*.*"
  Delete "$INSTDIR\sound\*.*"
  Delete "$INSTDIR\data_old\*.*"
  Delete "$INSTDIR\maps\*.*"

  RMDir "$INSTDIR\data"
  RMDir "$INSTDIR\data_old"
  RMDir "$INSTDIR\resource"
  RMDir "$INSTDIR\scripts"
  RMDir "$INSTDIR\sound"
  RMDir "$INSTDIR\maps"
  RMDir "$INSTDIR"  

  Delete "$SMPROGRAMS\$ICONS_GROUP\Uninstall.lnk"
  Delete "$SMPROGRAMS\$ICONS_GROUP\Heimdall on web.lnk"
  Delete "$ICONS_GROUP.lnk"
  Delete "$DESKTOP\Alcominance.lnk"
  Delete "$SMPROGRAMS\$ICONS_GROUP\Alcominance.lnk"

  RMDir "$SMPROGRAMS\$ICONS_GROUP"

  DeleteRegKey ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}"
  DeleteRegKey HKLM "${PRODUCT_DIR_REGKEY}"
  SetAutoClose true
SectionEnd