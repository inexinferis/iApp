#include "main.h"

/**************************************************************\
  Login Wnd
\**************************************************************/

BEGIN_EVENT_TABLE(LoginDlg,XSkinnedDlg)
  EVENT_CREATE(LoginDlg::OnCreate)
  EVENT_CLOSE(LoginDlg::OnClose)
  EVENT_DESTROY(LoginDlg::OnDestroy)
  EVENT_BN_CLICKED(IDCANCEL,LoginDlg::OnCancel)
  EVENT_BN_CLICKED(IDOK,LoginDlg::OnLogin)
  EVENT_EN_CHANGE(ID_USERNAME,LoginDlg::OnEditChange)
  EVENT_EN_CHANGE(ID_PASSWORD,LoginDlg::OnEditChange)
END_EVENT_TABLE()

INT LoginDlg::OnCreate(LPCREATESTRUCT /*pcs*/){
  //set icon...
  SetIcon(LoadIcon(XGetInstanceHandle(),MAKEINTRESOURCE(IDI_IAPPICON)),FALSE);
  //center wnd
  CenterWindow();
  //create edits
  sWelcome.Create(*this,STR_WELCOME,50,XRect(10,10,380,20),XDEFSTATICSTYLE|SS_VCENTER);
  uName.Create(*this,NULL,ID_USERNAME,XRect(110,50,220,22),XDEFEDITSTYLE|WS_TABSTOP|ES_WANTRETURN);
  sName.Create(*this,STR_USERNAME,100,XRect(10,50,95,22),XDEFSTATICSTYLE|SS_NOTIFY|SS_RIGHT|SS_VCENTER,&uName);
  uPass.Create(*this,NULL,ID_PASSWORD,XRect(110,85,220,22),XDEFEDITSTYLE|WS_TABSTOP|ES_PASSWORD|ES_WANTRETURN);
  sPass.Create(*this,STR_PASSWORD,102,XRect(10,85,95,22),XDEFSTATICSTYLE|SS_NOTIFY|SS_RIGHT|SS_VCENTER,&uPass);
  bRemember.Create(*this,STR_REMEMBER,104,XRect(110,115,120,20),XDEFBUTTONSTYLE|WS_TABSTOP|BS_AUTOCHECKBOX);
  bLogin.Create(*this,STR_LOGIN,IDOK,XRect(60,145,120,24),XDEFBUTTONSTYLE|WS_TABSTOP|WS_DISABLED);
  bCancel.Create(*this,STR_CANCEL,IDCANCEL,XRect(210,145,120,24),XDEFBUTTONSTYLE|WS_TABSTOP);
  uName.SetFocus();
  //move to front...
  SetForegroundWindow();
  return 0;
}

VOID LoginDlg::OnClose(){
  EndDialog(0);
}

VOID LoginDlg::OnDestroy(){
}

VOID LoginDlg::OnEditChange(WPARAM /*dwParam*/,LPARAM /*lParam*/){
  if(uName.GetWindowTextLength()&&uPass.GetWindowTextLength()){
    if(!bLogin.IsEnabled()){
      bLogin.EnableWindow(TRUE);
      bLogin.RedrawWindow();
    }
  }else{
    if(bLogin.IsEnabled()){
      bLogin.EnableWindow(FALSE);
      bLogin.RedrawWindow();
    }
  }
}

VOID LoginDlg::OnLogin(WPARAM /*wParam*/,LPARAM /*lParam*/){
  if(logThread.Create(this,LPXTHREAD_START_ROUTINE(LPXWND_THREAD_ROUTINE(&LoginDlg::LoginThread)))){
    if(pApp->conDlg.CreateDlgBoxEx(WS_EX_DLGMODALFRAME,STR_STARTINGSESSION,WS_VISIBLE,XRECT(CW_USEDEFAULT,CW_USEDEFAULT,380,105),*this)){
      bLoggedIn=TRUE;
      EndDialog(TRUE);
    }
    logThread.Close();
  }else
    MessageBox(STR_INTERNAL_ERROR);
}

VOID LoginDlg::OnCancel(WPARAM /*wParam*/,LPARAM /*lParam*/){
	OnClose();
}

DWORD LoginDlg::LoginThread(LPVOID /*lpParameter*/){
  XASSERT(pApp!=NULL)
  pApp->SetThreadLocale();
  if(uName.GetWindowTextLength()&&uPass.GetWindowTextLength()){
    UINT uRet=pApp->con.Login();
    switch(uRet){
      case 0://IPROT_RESPONSE_OK:
        pApp->conDlg.EndDialog(TRUE);
        return 0;
      break;
      case 1://IPROT_USER_NOT_ACTIVE:
        MessageBox(STR_USERNOTACTIVE,0,MB_ICONEXCLAMATION);
      break;
      case 2://IPROT_INVALID_USER_DATA:
        MessageBox(STR_IVALIDUSERDATA,0,MB_ICONEXCLAMATION);
      break;
      case 3://IPROT_USER_BANNED:
        MessageBox(STR_BANNEDACCOUNT,0,MB_ICONEXCLAMATION);
      break;
      case 4://IPROT_CONNECTION_FAILURE:case IPROT_SYSTEM_ERROR:
        MessageBox(pApp->con.GetLastErrorMsg(),0,MB_ICONEXCLAMATION);
      break;
      case 5://IPROT_OPERATION_CANCELLED:break;//do nothing...
        MessageBox(STR_INVALIDPROTOCOL,0,MB_ICONEXCLAMATION);
      break;
      default://IPROT_INVALID_RESPONSE,IPROT_INVALID_ARGS
        MessageBox(STR_COULDNOTCONNECT,0,MB_ICONEXCLAMATION);
      break;
    }
  }
  pApp->conDlg.EndDialog(0);
  return 0;
}
