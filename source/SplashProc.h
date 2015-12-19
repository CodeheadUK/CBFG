
BOOL CALLBACK SplashWinProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
 {
  extern HWND hMain;

   switch(msg)
    {
     case WM_INITDIALOG:
      SetTimer(hDlg,246,3000,NULL);
      return true;

     case WM_MOUSEACTIVATE:
      KillTimer(hDlg,246);
      EndDialog(hDlg,0);
      InvalidateRgn(hMain,NULL,true);
      return 0;

     case WM_TIMER:
      KillTimer(hDlg,246);
      EndDialog(hDlg,0);
      InvalidateRgn(hMain,NULL,true);      
      return 0;

      default:
       return 0;
    }

 }