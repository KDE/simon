
  
  
  !define MUI_FINISHPAGE_RUN "$INSTDIR\bin\simon.exe"  
  direkt vor
  !insertmacro MUI_PAGE_FINISH
  

ganz unten:
  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
  
Function SysLang
  Push $0
 
  System::Alloc "${NSIS_MAX_STRLEN}"
  Pop $0
 
  Var /GLOBAL file
  
  FileOpen $file "$INSTDIR\share\config\kdeglobals" "w"
  FileWrite $file "[Locale]$\n"
  
  ;uses LOCALE_SYSTEM_DEFAULT and LOCALE_SLANGUAGE defines
 
  System::Call "Kernel32::GetLocaleInfo(i,i,t,i)i(2048,0x5a,.r0,${NSIS_MAX_STRLEN})i"
  FileWrite $file "Country="
  FileWrite $file "$0"
  FileWrite $file "$\n"
  
  
  System::Call "Kernel32::GetLocaleInfo(i,i,t,i)i(2048,0x59,.r0,${NSIS_MAX_STRLEN})i"
  FileWrite $file "Language="
  FileWrite $file "$0"
  FileWrite $file "$\n"
  
  FileClose $file

  Exch $0
FunctionEnd

Function .onInstSuccess
Call SysLang 
Exec "$INSTDIR\bin\kbuildsycoca4.exe"
FunctionEnd

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
  

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
add the following...
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
		
	!insertmacro MUI_INSTALLOPTIONS_WRITE "NSIS.InstallOptions.ini" "Settings"  "NumFields" "4"

	!insertmacro MUI_INSTALLOPTIONS_WRITE "NSIS.InstallOptions.ini" "Field 1"  "Type" "label"
	!insertmacro MUI_INSTALLOPTIONS_WRITE "NSIS.InstallOptions.ini" "Field 1"  "Text" "$(PATH_OPTIONS_BODY_DESC)"
	!insertmacro MUI_INSTALLOPTIONS_WRITE "NSIS.InstallOptions.ini" "Field 1"  "Left" "0"
	!insertmacro MUI_INSTALLOPTIONS_WRITE "NSIS.InstallOptions.ini" "Field 1"  "Right" "-1"
	!insertmacro MUI_INSTALLOPTIONS_WRITE "NSIS.InstallOptions.ini" "Field 1"  "Top" "0"
	!insertmacro MUI_INSTALLOPTIONS_WRITE "NSIS.InstallOptions.ini" "Field 1"  "Bottom" "20"

	!insertmacro MUI_INSTALLOPTIONS_WRITE "NSIS.InstallOptions.ini" "Field 2"  "Type" "label"
	!insertmacro MUI_INSTALLOPTIONS_WRITE "NSIS.InstallOptions.ini" "Field 2"  "Text" " "
	!insertmacro MUI_INSTALLOPTIONS_WRITE "NSIS.InstallOptions.ini" "Field 2"  "Left" "0"
	!insertmacro MUI_INSTALLOPTIONS_WRITE "NSIS.InstallOptions.ini" "Field 2"  "Right" "-1"
	!insertmacro MUI_INSTALLOPTIONS_WRITE "NSIS.InstallOptions.ini" "Field 2"  "Top" "30"
	!insertmacro MUI_INSTALLOPTIONS_WRITE "NSIS.InstallOptions.ini" "Field 2"  "Bottom" "40"
	!insertmacro MUI_INSTALLOPTIONS_WRITE "NSIS.InstallOptions.ini" "Field 2"  "State" "0"

	!insertmacro MUI_INSTALLOPTIONS_WRITE "NSIS.InstallOptions.ini" "Field 3"  "Type" "radiobutton"
	!insertmacro MUI_INSTALLOPTIONS_WRITE "NSIS.InstallOptions.ini" "Field 3"  "Text" "$(PATH_OPTIONS_BODY_ALL)"
	!insertmacro MUI_INSTALLOPTIONS_WRITE "NSIS.InstallOptions.ini" "Field 3"  "Left" "0"
	!insertmacro MUI_INSTALLOPTIONS_WRITE "NSIS.InstallOptions.ini" "Field 3"  "Right" "-1"
	!insertmacro MUI_INSTALLOPTIONS_WRITE "NSIS.InstallOptions.ini" "Field 3"  "Top" "40"
	!insertmacro MUI_INSTALLOPTIONS_WRITE "NSIS.InstallOptions.ini" "Field 3"  "Bottom" "50"
	!insertmacro MUI_INSTALLOPTIONS_WRITE "NSIS.InstallOptions.ini" "Field 3"  "State" "1"

	!insertmacro MUI_INSTALLOPTIONS_WRITE "NSIS.InstallOptions.ini" "Field 4"  "Type" "radiobutton"
	!insertmacro MUI_INSTALLOPTIONS_WRITE "NSIS.InstallOptions.ini" "Field 4"  "Text" "$(PATH_OPTIONS_BODY_CURRENT)"
	!insertmacro MUI_INSTALLOPTIONS_WRITE "NSIS.InstallOptions.ini" "Field 4"  "Left" "0"
	!insertmacro MUI_INSTALLOPTIONS_WRITE "NSIS.InstallOptions.ini" "Field 4"  "Right" "-1"
	!insertmacro MUI_INSTALLOPTIONS_WRITE "NSIS.InstallOptions.ini" "Field 4"  "Top" "50"
	!insertmacro MUI_INSTALLOPTIONS_WRITE "NSIS.InstallOptions.ini" "Field 4"  "Bottom" "60"
	!insertmacro MUI_INSTALLOPTIONS_WRITE "NSIS.InstallOptions.ini" "Field 4"  "State" "0"
	
	!insertmacro MUI_INSTALLOPTIONS_WRITE "NSIS.InstallOptions.ini" "Field 5"  "Type" ""
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
right after this:
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	
Function .onInit
  ; Reads components status for registry
  !insertmacro SectionList "InitSection"

  ; check to see if /D has been used to change 
  ; the install directory by comparing it to the 
  ; install directory that is expected to be the
  ; default
  StrCpy $IS_DEFAULT_INSTALLDIR 0
  StrCmp "$INSTDIR" "$PROGRAMFILES\simon 0.3" 0 +2
    StrCpy $IS_DEFAULT_INSTALLDIR 1
  
  StrCpy $SV_ALLUSERS "JustMe"
  ; if default install dir then change the default
  ; if it is installed for JustMe
  StrCmp "$IS_DEFAULT_INSTALLDIR" "1" 0 +2
    StrCpy $INSTDIR "$DOCUMENTS\simon 0.3"

  ClearErrors
  UserInfo::GetName
  IfErrors noLM
  Pop $0
  UserInfo::GetAccountType
  Pop $1
  StrCmp $1 "Admin" 0 +3
    SetShellVarContext all
    ;MessageBox MB_OK 'User "$0" is in the Admin group'
    StrCpy $SV_ALLUSERS "AllUsers"
    Goto done
  StrCmp $1 "Power" 0 +3
    SetShellVarContext all
    ;MessageBox MB_OK 'User "$0" is in the Power Users group'
    StrCpy $SV_ALLUSERS "AllUsers"
    Goto done
    
  noLM:
    StrCpy $SV_ALLUSERS "AllUsers"
    ;Get installation folder from registry if available

  done:
  StrCmp $SV_ALLUSERS "AllUsers" 0 +3
    StrCmp "$IS_DEFAULT_INSTALLDIR" "1" 0 +2
      StrCpy $INSTDIR "$PROGRAMFILES\simon 0.3"

  StrCmp "ON" "ON" 0 noOptionsPage
    !insertmacro MUI_INSTALLOPTIONS_EXTRACT "NSIS.InstallOptions.ini"
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;







;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

  
  
  
  
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Replace the following with the latter
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;--------------------------------
; Create custom pages
Function InstallOptionsPage
  !insertmacro MUI_HEADER_TEXT "Install Options" "Choose options for installing simon 0.3 simon"
  !insertmacro MUI_INSTALLOPTIONS_DISPLAY "NSIS.InstallOptions.ini"

FunctionEnd
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

LangString PATH_OPTIONS_HEADER_TEXT ${LANG_ENGLISH} "Install Options"
LangString PATH_OPTIONS_HEADER_SUBTEXT ${LANG_ENGLISH} "Choose options for installing simon 0.3"
LangString PATH_OPTIONS_BODY_DESC ${LANG_ENGLISH} "simons installation directory needs to be added to the system path."
LangString PATH_OPTIONS_BODY_ALL ${LANG_ENGLISH} "Add simon to the system path for all users"
LangString PATH_OPTIONS_BODY_CURRENT ${LANG_ENGLISH} "Add simon to the system path for current user"

LangString PATH_OPTIONS_HEADER_TEXT ${LANG_GERMAN} "Installationsoptionen"
LangString PATH_OPTIONS_HEADER_SUBTEXT ${LANG_GERMAN} "Konfigurieren Sie Ihre simon Installation"
LangString PATH_OPTIONS_BODY_DESC ${LANG_GERMAN} "simons Installationsordner muss zum Systempfad hinzugefügt werden."
LangString PATH_OPTIONS_BODY_ALL ${LANG_GERMAN} "simon für alle Anwender zum Systempfad hinzufügen"
LangString PATH_OPTIONS_BODY_CURRENT ${LANG_GERMAN} "simon für den aktuellen Anwender zum Systempfad hinhzufügen"

;--------------------------------
; Create custom pages
Function InstallOptionsPage
  !insertmacro MUI_HEADER_TEXT "$(PATH_OPTIONS_HEADER_TEXT)" "$(PATH_OPTIONS_HEADER_SUBTEXT)"
  !insertmacro MUI_INSTALLOPTIONS_DISPLAY "NSIS.InstallOptions.ini"
FunctionEnd
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;



;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; For release packages strip all files with these commands:
; find . -iname "*.dll" -exec strip {} ; 
; find . -iname "*.dll" -exec strip {} ; 
;
; After restore the unstripped versions of these files:
; libexpat.dll
; libxml2.dll
; libxslt.dll
; portaudio_x86.dll
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


