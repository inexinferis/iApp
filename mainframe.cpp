#include "main.h"

/**************************************************************\
  Main Frame
\**************************************************************/

BEGIN_EVENT_TABLE(MainFrame,XSkinnedFrame)
  EVENT_CREATE(MainFrame::OnCreate)
  EVENT_CLOSE(MainFrame::OnClose)
  EVENT_SIZE(MainFrame::OnSize)
  EVENT_GETMINMAXINFO(MainFrame::OnGetMaxMinInfo)
  EVENT_DESTROY(MainFrame::OnDestroy)
  EVENT_BN_CLICKED(IDM_IAPPLOGOUT,MainFrame::OnLogout)
  EVENT_BN_CLICKED(IDM_IAPPUPDATE,MainFrame::OnCheckUpdate)
  EVENT_BN_CLICKED(IDM_IAPPSETTINGS,MainFrame::OnSettings)
  EVENT_BN_CLICKED(IDM_IAPPEXIT,MainFrame::OnExit)
  EVENT_BN_CLICKED(IDM_HELPCONTENTS,MainFrame::OnHelpContents)
  EVENT_BN_CLICKED(IDM_HELPABOUT,MainFrame::OnAbout)
  EVENT_BN_CLICKED(IDM_TOOLPATCHGUARD,MainFrame::OnPatchguard)
  EVENT_UPDATESTATE(MainFrame::OnUpdateState)
  EVENT_XNOTIFYICON(MainFrame::OnSystrayNotify)
  EVENT_XTVSELCHANGED(MainFrame::OnTVSelChanged)
  EVENT_TIMER_ID(IDT_CHECKSTATUS,MainFrame::OnUpdateStatusTimer)
END_EVENT_TABLE()

DWORD MainFrame::LogoutThread(LPVOID /*lpParameter*/){
  XASSERT(pApp!=NULL)
  pApp->SetThreadLocale();
  switch(pApp->con.Logout()){
    case 0://IPROT_RESPONSE_OK:
      pApp->conDlg.EndDialog(TRUE);
      return 0;
    break;
    case 1://IPROT_OPERATION_CANCELLED://do nothing...
    break;
    default://IPROT_INVALID_RESPONSE,IPROT_INVALID_ARGS
      MessageBox(STR_COULDNOTCONNECT,0,MB_ICONEXCLAMATION);
    break;
  }
  pApp->conDlg.EndDialog(0);
  return 0;
}

DWORD MainFrame::CUpdateThread(LPVOID /*lpParameter*/){
  XASSERT(pApp!=NULL)
  pApp->SetThreadLocale();
  switch(pApp->con.GetStatus()){
    case 0://IPROT_RESPONSE_OK:
      //should be in the same thread...
      SendMessage(WM_UPDATESTATE);
      pApp->conDlg.EndDialog(TRUE);
      return 0;
    break;
    case 1://IPROT_OPERATION_CANCELLED:break;//do nothing...
    case 2://IPROT_CONNECTION_FAILURE:case IPROT_SYSTEM_ERROR:
      pApp->conDlg.MessageBox(pApp->con.GetLastErrorMsg(),0,MB_ICONEXCLAMATION);
    break;
    default://IPROT_INVALID_RESPONSE,IPROT_INVALID_ARGS
      MessageBox(STR_COULDNOTCONNECT,0,MB_ICONEXCLAMATION);
    break;
  }
  pApp->conDlg.EndDialog(0);
  return 0;
}

DWORD MainFrame::UpdateStatusThread(LPVOID /*lpParameter*/){
  XASSERT(pApp!=NULL)
  pApp->SetThreadLocale();
  switch(pApp->con.GetStatus()){
    case 0://IPROT_RESPONSE_OK:
      //should be in the same thread...
      SendMessage(WM_UPDATESTATE);
    break;
    default://IPROT_OPERATION_CANCELLED,IPROT_CONNECTION_FAILURE,IPROT_SYSTEM_ERROR,IPROT_INVALID_RESPONSE,IPROT_INVALID_ARGS
      bForceExit=TRUE;
      PostMessage(WM_CLOSE);
    break;
  }
  return 0;
}

INT MainFrame::OnCreate(LPCREATESTRUCT /*pcs*/){
  //set icon...
  SetIcon(LoadIcon(XGetInstanceHandle(),MAKEINTRESOURCE(IDI_IAPPICON)),FALSE);

  // Menus...
  XMenuEx *File=Menu.Append(0,STR_IAPP_MENU);
  File->Append(IDM_IAPPLOGOUT,STR_LOGOUT_MENU);
  File->AppendSeparator();
  File->Append(IDM_IAPPUPDATE,STR_UPDATE_MENU);
  File->Append(IDM_IAPPSETTINGS,STR_SETTINGS_MENU);
  File->AppendSeparator();
  File->Append(IDM_IAPPEXIT,STR_EXIT_MENU);

  if(IsWow64()){
    XMenuEx *Tools=Menu.Append(0,STR_TOOLS_MENU);
    Tools->Append(IDM_TOOLPATCHGUARD,STR_PATCHGUARD_MENU);
  }

  XMenuEx *Help=Menu.Append(0,STR_HELP_MENU);
  Help->Append(IDM_HELPCONTENTS,STR_CONTENTS_MENU);
  Help->AppendSeparator();
  Help->Append(IDM_HELPABOUT,STR_ABOUT_MENU);

  SetMenu(Menu);

  REdit.Create(*this,NULL,300,XRect(0,0,0,0),XDEFRICHEDITSTYLE|WS_TABSTOP|WS_VSCROLL|ES_MULTILINE|ES_LEFT|ES_AUTOVSCROLL|ES_READONLY);
  if(!REdit.IsValid()){
    MessageBox(pApp->con.GetLastErrorMsg(),0,MB_ICONEXCLAMATION);
    ExitProcess(0);
  }

  REdit.SetFont((HFONT)GetStockObject(DEFAULT_GUI_FONT),TRUE);
  REdit.SetBackgroundColor(FALSE,RGB(0x00,0x00,0x00));
  REdit.SetTextColor(FALSE,RGB(0x90,0x90,0x90));

  /*LPTSTR tmpmsg=FormatMsgEx(NULL,STR_APP_CREDITS,FULLVERSION_STRING);
  if(tmpmsg){
    REdit.PutString(tmpmsg);
    LocalFree(tmpmsg);
  }

  IconLst.Create(32,32,ILC_COLOR16|ILC_MASK,MAX_ICONINDEX+2);
  IconLst.Add(LoadIcon(XGetInstanceHandle(),MAKEINTRESOURCE(IDI_IAPPICON)));
  IconLst.Add(LoadIcon(XGetInstanceHandle(),MAKEINTRESOURCE(IDI_DWICON)));

  //public
  IconLst.Add(LoadIcon(XGetInstanceHandle(),MAKEINTRESOURCE(IDI_HLICON)));
  IconLst.Add(LoadIcon(XGetInstanceHandle(),MAKEINTRESOURCE(IDI_CSICON)));
  IconLst.Add(LoadIcon(XGetInstanceHandle(),MAKEINTRESOURCE(IDI_HL2ICON)));
  IconLst.Add(LoadIcon(XGetInstanceHandle(),MAKEINTRESOURCE(IDI_CSSICON)));
  IconLst.Add(LoadIcon(XGetInstanceHandle(),MAKEINTRESOURCE(IDI_CSGOICON)));

  //private
  IconLst.Add(LoadIcon(XGetInstanceHandle(),MAKEINTRESOURCE(IDI_IRICON)));
  IconLst.Add(LoadIcon(XGetInstanceHandle(),MAKEINTRESOURCE(IDI_SEICON)));
  IconLst.Add(LoadIcon(XGetInstanceHandle(),MAKEINTRESOURCE(IDI_IRSICON)));
  IconLst.Add(LoadIcon(XGetInstanceHandle(),MAKEINTRESOURCE(IDI_IRGOICON)));
  IconLst.Add(LoadIcon(XGetInstanceHandle(),MAKEINTRESOURCE(IDI_HDICON)));

  BmpLst.Create(250,60,ILC_COLOR32,MAX_BGINDEX);
  BmpLst.Add(LoadBitmap(XGetInstanceHandle(),MAKEINTRESOURCE(IDB_CSBMP)));
  BmpLst.Add(LoadBitmap(XGetInstanceHandle(),MAKEINTRESOURCE(IDB_IRBMP)));
  BmpLst.Add(LoadBitmap(XGetInstanceHandle(),MAKEINTRESOURCE(IDB_SEBMP)));

  Tree.CreateEx(0,*this,NULL,100,XRect(0,0,0,0),XDEFLISTVIEWSTYLE|TVS_DISABLEDRAGDROP|TVS_SINGLEEXPAND|TVS_FULLROWSELECT|TVS_NOHSCROLL|WS_BORDER);
  Tree.SetImageList(IconLst,TVSIL_NORMAL);*/

  StatusBar.CreateEx(0,*this,NULL,101,XRect(0,0,0,0),XDEFSTATUSBARSTYLE);
  UpdateState(LoadStringStatic(0,STR_CONNECTIONOK));

  CenterWindow();
  //move to front...
  SetForegroundWindow();
  return 0;
}

VOID MainFrame::OnGetMaxMinInfo(LPMINMAXINFO pmmi){
  pmmi->ptMinTrackSize.x=MINWINSIZEX;
  pmmi->ptMinTrackSize.y=MINWINSIZEY;
}

VOID MainFrame::OnSize(UINT nType,POINTS dwSize){
  if(nType==SIZE_MINIMIZED){
    if(!SysTray.IsValid()&&SysTray.Add(*this,0x500,LoadIcon(XGetInstanceHandle(),MAKEINTRESOURCE(IDI_IAPPICON)),LoadStringStatic(NULL,STR_APP_NAME)))
      ShowWindow(SW_HIDE);
  }else{
    if(SysTray.IsValid())
      SysTray.Delete();
    Tree.MoveWindow(0,0,LISTWINWIDTH,dwSize.y-STATUSBARHEIGHT,TRUE);
    Tree.UpdateClient(LISTWINWIDTH+5,0,dwSize.x-LISTWINWIDTH-5,dwSize.y-REDITWINHEIGHT-5-STATUSBARHEIGHT);
    REdit.MoveWindow(LISTWINWIDTH+5,dwSize.y-REDITWINHEIGHT-STATUSBARHEIGHT,dwSize.x-LISTWINWIDTH-5,REDITWINHEIGHT,TRUE);
    StatusBar.SendMessage(WM_SIZE,0,0);
  }
}

VOID MainFrame::OnClose(){
  //there is a cheat running?
  pApp->bClosing=TRUE;
  //force disconnect any active connection...
  if(pApp->con.IsBusy()){
    pApp->con.Disconnect();
    while(pApp->con.IsBusy())
      Sleep(100);
  }
  //kill status thread if active...
  KillTimer(IDT_CHECKSTATUS);
  if(getstatusThread.IsValid()){
    DWORD status;
    if(getstatusThread.GetExitCode(&status)&&status==STILL_ACTIVE){
      getstatusThread.WaitForExit(10000);
      getstatusThread.Close();
    }
  }
  //check if we need disconnect from server...
  if(!bForceExit){
    if(logoutThread.Create(this,LPXTHREAD_START_ROUTINE(LPXWND_THREAD_ROUTINE(&MainFrame::LogoutThread)))){
      if(pApp->conDlg.CreateDlgBoxEx(WS_EX_DLGMODALFRAME,STR_CLOSINGSESSION,WS_VISIBLE,XRECT(CW_USEDEFAULT,CW_USEDEFAULT,380,105),*this))
        pApp->bConnected=FALSE;
      logoutThread.Close();
    }else
      MessageBox(STR_INTERNAL_ERROR);
  }
  bForceExit=FALSE;
  pApp->bClosing=FALSE;
  DestroyWindow();
}

VOID MainFrame::OnDestroy(){
  Menu.Destroy();
  IconLst.Destroy();
  BmpLst.Destroy();
  PostQuitMessage(0);
}

VOID MainFrame::OnExit(WPARAM /*wParam*/,LPARAM /*lParam*/){
  OnClose();
}

VOID MainFrame::OnCheckUpdate(WPARAM /*wParam*/,LPARAM /*lParam*/){
  while(pApp->con.IsBusy())
    Sleep(100);
  if(cupdateThread.Create(this,LPXTHREAD_START_ROUTINE(LPXWND_THREAD_ROUTINE(&MainFrame::CUpdateThread)))){
    BOOL bNeedUpdate=pApp->conDlg.CreateDlgBoxEx(WS_EX_DLGMODALFRAME,STR_CHECKINGUPDATES,WS_VISIBLE,XRECT(CW_USEDEFAULT,CW_USEDEFAULT,380,105),*this);
    switch(bNeedUpdate){
      case -1://outdated version, restart!
        pApp->bCheckUpdate=TRUE;
        pApp->bConnected=FALSE;
        pApp->bRestart=TRUE;
        DestroyWindow();
      break;
      case 1:
        MessageBox(LoadStringStatic(NULL,STR_NOUPDATES),NULL,MB_OK|MB_ICONINFORMATION);
      break;//all OK
      default:break;
    }
    cupdateThread.Close();
  }else
    MessageBox(STR_INTERNAL_ERROR);
}

VOID MainFrame::OnLogout(WPARAM /*wParam*/,LPARAM /*lParam*/){
  while(pApp->con.IsBusy())
    Sleep(100);
  if(logoutThread.Create(this,LPXTHREAD_START_ROUTINE(LPXWND_THREAD_ROUTINE(&MainFrame::LogoutThread)))){
    if(pApp->conDlg.CreateDlgBoxEx(WS_EX_DLGMODALFRAME,STR_CLOSINGSESSION,WS_VISIBLE,XRECT(CW_USEDEFAULT,CW_USEDEFAULT,380,105),*this)){
      pApp->bConnected=FALSE;
      pApp->bRestart=TRUE;
      DestroyWindow();
    }
    logoutThread.Close();
  }else
    MessageBox(STR_INTERNAL_ERROR);
}

VOID MainFrame::OnSettings(WPARAM /*wParam*/,LPARAM /*lParam*/){
  Settings.CreateDlgBoxEx(0,STR_IAPP_SETTINGS,WS_VISIBLE|WS_SYSMENU,XRECT(CW_USEDEFAULT,CW_USEDEFAULT,260,220),*this);
}

VOID MainFrame::OnAbout(WPARAM /*wParam*/,LPARAM /*lParam*/){
  About.CreateDlgBoxEx(0,STR_ABOUT_IAPP,WS_VISIBLE|WS_SYSMENU,XRECT(CW_USEDEFAULT,CW_USEDEFAULT,320,240),*this);
}

VOID MainFrame::OnPatchguard(WPARAM /*wParam*/,LPARAM /*lParam*/){
  Patchguard.CreateDlgBoxEx(0,STR_PATCHGUARD,WS_VISIBLE|WS_SYSMENU,XRECT(CW_USEDEFAULT,CW_USEDEFAULT,320,120),*this);
}

VOID MainFrame::OnHelpContents(WPARAM /*wParam*/,LPARAM /*lParam*/){
  CHAR sHelpFile[MAX_PATH];
  sprintf(sHelpFile,"%s\\%s",pApp->sAppPath,g_StrMap[IAppHelp]);
  ShellExecute(*this,"open",sHelpFile,NULL,NULL,SW_SHOWNORMAL);
}

VOID MainFrame::OnSystrayNotify(UINT /*uId*/,UINT uMouseMsg){
  switch(LOWORD(uMouseMsg)){
    case WM_LBUTTONUP:case WM_RBUTTONUP:
      ShowWindow(SW_RESTORE);
      SetForegroundWindow();
    break;
    default:break;
  }
}

VOID MainFrame::OnTVSelChanged(TabPage *pTab){
  pActiveTab=pTab;
}

VOID MainFrame::OnUpdateStatusTimer(TIMERPROC /*lpTimerFunc*/){
}

VOID MainFrame::OnUpdateState(VOID){
}

VOID MainFrame::LogEvent(LPCTSTR form,...){
  if(form){
    static TCHAR tmp[2048];
    va_list p;
    va_start(p,form);
    wvsprintf(tmp,form,p);
    REdit.PutString(tmp);
    va_end(p);
  }else
    REdit.ShowError();
}

VOID MainFrame::LogEventEx(HINSTANCE hInstance,LPCSTR nRsrcID,...){
  static TCHAR tmp[2048];
  va_list argList;
  va_start(argList,nRsrcID);
  LPTSTR lpszFormat=LoadStringBuffered(hInstance,nRsrcID);
  if(lpszFormat){
    vsprintf(tmp,lpszFormat,argList);
    REdit.PutString(tmp);
    LocalFree(lpszFormat);
  }
  va_end(argList);
}

VOID MainFrame::UpdateState(LPCTSTR state){
  static TCHAR nstate[512*sizeof(TCHAR)];
  //this need a static buffer...
  strcpy(nstate,state);
  StatusBar.SetText(nstate);
}
