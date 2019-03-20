#include "main.h"

/**************************************************************\
  Main App
\**************************************************************/

IMPLEMENT_APP(IApp)

const WCHAR sBrowserStyle[]=L"BODY {\
  background-color:#000000;\
  color:#FFFFFF;\
  font: 0.8em Verdana,sans-serif,Arial,Helvetica;\
  margin: 0;\
  padding: 5px;\
  scrollbar-base-color:#404040;\
  scrollbar-track-color: #101010;\
  scrollbar-arrow-color: #909090;\
  scrollbar-highlight-color:#303030\
}";

const TCHAR *g_StrMap[]={
  _T("Inexinferis App"),
  _T("IAPP Main Frame"),
  _T("iAppInstaller.exe"),
  _T("iApp.chm"),
  _T("iApp"),
  _T("-silent")
};

VOID IApp::SetThreadLocale(){
  // Set the thread locale based on the primary language
  // (control panel --> regional settings --> language)
  //if(!m_lang){
    switch(PRIMARYLANGID(::GetUserDefaultLCID())){
      case LANG_SPANISH:
        ::SetThreadLocalSettings(LANG_SPANISH,SUBLANG_DEFAULT);
      break;
      default:
        ::SetThreadLocalSettings(LANG_ENGLISH,SUBLANG_DEFAULT);
      break;
    }
  /*}else{
    if(m_lang==1)
      ::SetThreadLocalSettings(LANG_SPANISH,SUBLANG_DEFAULT);
    else
      ::SetThreadLocalSettings(LANG_ENGLISH,SUBLANG_DEFAULT);
  }*/
}

LPTSTR IApp::GetLastErrorMsg(){
  static TCHAR szMsgBuf[SYS_ERROR_BUFFER_SIZE];
  if(::FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM,NULL,
    ::GetLastError(),0,szMsgBuf,SYS_ERROR_BUFFER_SIZE,NULL))
    return szMsgBuf;
  return NULL;
}

VOID IApp::DisableAppTheme(){
  HMODULE uxtheme=GetModuleHandle(_T("uxtheme.dll"));
  if(uxtheme){
    typedef VOID (WINAPI *tSetThemeAppProperties)(DWORD);
    tSetThemeAppProperties pSetThemeAppProperties=(tSetThemeAppProperties)GetProcAddress(uxtheme,"SetThemeAppProperties");
    if(pSetThemeAppProperties)
      pSetThemeAppProperties(0);
  }
}

BOOL IApp::OnInit(HINSTANCE hInst,LPTSTR pcArgs,INT nShow){

  //check only one instance
  hMutex=CreateMutex(NULL,TRUE,g_StrMap[IAppMainStr]);
  if(!hMutex)return FALSE;
  if(GetLastError()==ERROR_ALREADY_EXISTS){
    FindAndShowWindow(hInst);
    CloseHandle(hMutex);
    return FALSE;
  }

  //Set Lang...
  SetThreadLocale();

  //Disable Themes...
  DisableAppTheme();

  //check if has access
  if(!GetAdminRights()){//You Need Admin Rigths...
    MessageBox(NULL,LoadStringStatic(NULL,STR_ADMINREQUIRED),0,MB_OK|MB_ICONSTOP);
    return FALSE;
  }

  //make paths
  GetModuleFileName(hInst,sAppFile,MAX_PATH);
  GetModulePath(hInst,sAppPath,MAX_PATH);

  //check argument...
  if(_tcsstr(pcArgs,g_StrMap[IAppHide]))
    nShow=SW_MINIMIZE;

  return StartApp(nShow);
}

BOOL IApp::StartApp(INT nShow){
  //Check Updates...
  if(bCheckUpdate){
    //find updates
    if(chkupdThread.Create(this,LPXTHREAD_START_ROUTINE(&IApp::CheckUpdateThread))){
      BOOL bNeedUpdate=conDlg.CreateDlgBoxEx(WS_EX_DLGMODALFRAME,STR_CHECKINGUPDATES,WS_VISIBLE,XRECT(CW_USEDEFAULT,CW_USEDEFAULT,380,105));
      chkupdThread.Close();
      switch(bNeedUpdate){
        case -1://outdated version
          if(updThread.Create(this,LPXTHREAD_START_ROUTINE(&IApp::UpdateThread))){
            updDlg.CreateDlgBoxEx(WS_EX_DLGMODALFRAME,STR_UPDATING,WS_VISIBLE,XRECT(CW_USEDEFAULT,CW_USEDEFAULT,380,150));
            updThread.Close();
          }else
            MessageBox(NULL,LoadStringStatic(NULL,STR_INTERNAL_ERROR),0,MB_OK|MB_ICONSTOP);
          return FALSE;
        break;
        case 1:break;//all OK
        default://error...
          return FALSE;
        break;
      }
    }else{
      MessageBox(NULL,LoadStringStatic(NULL,STR_INTERNAL_ERROR),0,MB_OK|MB_ICONSTOP);
      return FALSE;
    }
    bCheckUpdate=FALSE;
  }

  if(bFirst){
    //read data & try login
    if(1){
      if(logThread.Create(this,LPXTHREAD_START_ROUTINE(&IApp::LoginThread))){
        bConnected=conDlg.CreateDlgBoxEx(WS_EX_DLGMODALFRAME,STR_APP_NAME,WS_VISIBLE,XRECT(CW_USEDEFAULT,CW_USEDEFAULT,380,105));
        logThread.Close();
      }else{
        MessageBox(NULL,LoadStringStatic(NULL,STR_INTERNAL_ERROR),0,MB_OK|MB_ICONSTOP);
        return FALSE;
      }
    }
    bFirst=FALSE;
  }

  //try login...
  if(!bConnected&&!logDlg.CreateDlgBoxEx(0,STR_INIT_SESSION,WS_EX_TOOLWINDOW|WS_MINIMIZEBOX|WS_SYSMENU|WS_VISIBLE,XRECT(CW_USEDEFAULT,CW_USEDEFAULT,400,240))){
    return FALSE;//finaliza el programa...
  }

  mainFrame.Create(_T("Inexinferis App"),_T("IAPP Main Frame"),WS_OVERLAPPEDWINDOW,XRECT(CW_USEDEFAULT,CW_USEDEFAULT,MINWINSIZEX,MINWINSIZEY));
  if(mainFrame.IsValid()){
    //la mostramos...
    mainFrame.ShowWindow(nShow);
    mainFrame.UpdateWindow();
    return TRUE;//empieza el programa...
  }
  return FALSE;//finaliza el programa...
}

DWORD IApp::LoginThread(LPVOID /*lpParameter*/){
  SetThreadLocale();
  switch(con.Login()){
    case 0://IPROT_RESPONSE_OK:
      conDlg.EndDialog(TRUE);
      return 0;
    break;
    case 1://IPROT_USER_NOT_ACTIVE:
      conDlg.MessageBox(STR_USERNOTACTIVE,0,MB_ICONEXCLAMATION);
    break;
    case 2://IPROT_INVALID_USER_DATA:
      conDlg.MessageBox(STR_IVALIDUSERDATA,0,MB_ICONEXCLAMATION);
    break;
    case 3://IPROT_USER_BANNED:
      conDlg.MessageBox(STR_BANNEDACCOUNT,0,MB_ICONEXCLAMATION);
    break;
    case 4://IPROT_CONNECTION_FAILURE:case IPROT_SYSTEM_ERROR:
      conDlg.MessageBox(con.GetLastErrorMsg(),0,MB_ICONEXCLAMATION);
    break;
    case 5://IPROT_OPERATION_CANCELLED:break;//do nothing...
    default://IPROT_INVALID_RESPONSE,IPROT_INVALID_ARGS
      conDlg.MessageBox(STR_COULDNOTCONNECT,0,MB_ICONEXCLAMATION);
    break;
  }
  conDlg.EndDialog(FALSE);
  return 0;
}

DWORD IApp::CheckUpdateThread(LPVOID /*lpParameter*/){
  SetThreadLocale();
  switch(con.CheckUpdate()){
    case 0://IPROT_RESPONSE_OK:
      conDlg.EndDialog(TRUE);
      return 0;
    break;
    case 1://IPROT_CONNECTION_FAILURE:case IPROT_SYSTEM_ERROR:
      conDlg.MessageBox(con.GetLastErrorMsg(),0,MB_ICONEXCLAMATION);
    break;
    case 2://IPROT_OPERATION_CANCELLED:break;//do nothing...
    default://IPROT_INVALID_RESPONSE,IPROT_INVALID_ARGS
      conDlg.MessageBox(STR_COULDNOTCONNECT,0,MB_ICONEXCLAMATION);
    break;
  }
  conDlg.EndDialog(FALSE);
  return 0;
}

DWORD IApp::UpdateThread(LPVOID /*lpParameter*/){
  SetThreadLocale();
  switch(con.Update()){
    case 0://IPROT_RESPONSE_OK:
      ShellExecute(NULL,_T("open"),"",NULL,NULL,SW_SHOWDEFAULT);
      //run installer
      updDlg.EndDialog(0);
      return 0;
    break;
    case 1://IPROT_CONNECTION_FAILURE:case IPROT_SYSTEM_ERROR:
      updDlg.MessageBox(con.GetLastErrorMsg(),0,MB_ICONEXCLAMATION);
    break;
    case 2://IPROT_FILE_NOTFOUND:
      updDlg.MessageBox(STR_FILENOTFOUND,0,MB_ICONEXCLAMATION);
    break;
    case 3://IPROT_OPERATION_CANCELLED:break;//do nothing...
    default://IPROT_INVALID_RESPONSE,IPROT_INVALID_ARGS
      updDlg.MessageBox(STR_COULDNOTCONNECT,0,MB_ICONEXCLAMATION);
    break;
  }
  updDlg.EndDialog(FALSE);
  return 0;
}

INT IApp::OnExit(INT loopret){
  while(bRestart){
    bRestart=FALSE;
    if(StartApp(SW_NORMAL))
      loopret=MsgLoop();
  }
  if(hMutex)
    CloseHandle(hMutex);
  return loopret;
}

BEGIN_EVENT_TABLE(IApp,XWinApp)
  //EVENT_THREAD_MESSAGE(WM_APP+1,IApp::ThreadMsg)
  //PostThreadMessage(GetCurrentThreadId(),WM_APP+1,0,0);
END_EVENT_TABLE()
