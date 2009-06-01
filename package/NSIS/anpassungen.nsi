
  
  
  !define MUI_FINISHPAGE_RUN "$INSTDIR\bin\simon.exe"  
  direkt vor
  !insertmacro MUI_PAGE_FINISH
  

ganz unten
  
Function .onInstSuccess
Exec "$INSTDIR\bin\kbuildsycoca4.exe"
FunctionEnd
