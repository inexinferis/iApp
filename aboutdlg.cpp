#include "main.h"

/**************************************************************\
  About Dialog...
\**************************************************************/

BEGIN_EVENT_TABLE(AboutDlg,XSkinnedDlg)
  EVENT_CREATE(AboutDlg::OnCreate)
  EVENT_CLOSE(AboutDlg::OnClose)
  EVENT_BN_CLICKED(IDOK,AboutDlg::OnOk)
  EVENT_STN_CLICKED(IDS_LINK,AboutDlg::OnLinkClicked)
END_EVENT_TABLE()

INT AboutDlg::OnCreate(LPCREATESTRUCT /*pcs*/){
  //set icon...
  SetIcon(LoadIcon(XGetInstanceHandle(),MAKEINTRESOURCE(IDI_IAPPICON)),FALSE);
  //center window
  CenterWindow();
  bClose.Create(*this,STR_OK,IDOK,XRect(100,170,100,24),XDEFSTATICSTYLE|SS_VCENTER|WS_TABSTOP);
  sLink.Create(*this,STR_WEB_URL,IDS_LINK,XRect(10,60,150,20),XDEFSTATICSTYLE|WS_TABSTOP);
  bClose.SetFocus();
  return 0;
}

VOID AboutDlg::OnLinkClicked(WPARAM /*dwParams*/,LPARAM /*lParam*/){
  ShellExecute(*this,_T("open"),LoadStringStatic(NULL,STR_WEB_URL),0,0,SW_SHOWNORMAL);
}

VOID AboutDlg::OnOk(WPARAM /*wParam*/,LPARAM /*lParam*/){
  OnClose();
}

VOID AboutDlg::OnClose(){
  EndDialog(0);
}
