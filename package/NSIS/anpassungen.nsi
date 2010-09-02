
  
  
  !define MUI_FINISHPAGE_RUN "$INSTDIR\bin\simon.exe"  
  direkt vor
  !insertmacro MUI_PAGE_FINISH
  

ganz unten:
  
  
Function SysLang
  Push $0
 
  System::Alloc "${NSIS_MAX_STRLEN}"
  Pop $0
 
  Var /GLOBAL file
  
  FileOpen $file "$INSTDIR\share\config\kdeglobals" "w"
  FileWrite $file "[Locale]$\n"
  
  ;uses LOCALE_SYSTEM_DEFAULT and LOCALE_SLANGUAGE defines
 
  System::Call "Kernel32::GetLocaleInfo(i,i,t,i)i(2048,0x59,.r0,${NSIS_MAX_STRLEN})i"
  FileWrite $file "Country="
  FileWrite $file "$0"
  FileWrite $file "$\n"
  
  
  System::Call "Kernel32::GetLocaleInfo(i,i,t,i)i(2048,0x5a,.r0,${NSIS_MAX_STRLEN})i"
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
