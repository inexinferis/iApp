#include "main.h"

/**************************************************************\
  Settings Dialog...
\**************************************************************/

BEGIN_EVENT_TABLE(SettingsDlg,XSkinnedDlg)
  EVENT_CREATE(SettingsDlg::OnCreate)
  EVENT_CLOSE(SettingsDlg::OnClose)
  EVENT_BN_CLICKED(IDCANCEL,SettingsDlg::OnCancel)
  EVENT_STN_CLICKED(IDOK,SettingsDlg::OnSave)
END_EVENT_TABLE()

SettingsDlg::SettingsDlg(IApp *app):pApp(app){
  pMainFrame=&app->mainFrame;
}

INT SettingsDlg::OnCreate(LPCREATESTRUCT /*pcs*/){
  //set icon...
  SetIcon(LoadIcon(XGetInstanceHandle(),MAKEINTRESOURCE(IDI_IAPPICON)),FALSE);
  //center window
  CenterWindow();
  bGroup.Create(*this,STR_IAPP_SETTINGS,IDC_STATIC,XRECT(0,0,243,140),WS_CHILD|WS_VISIBLE|BS_GROUPBOX);
  sLang.Create(bGroup,STR_IAPP_LANG,IDC_STATIC,XRect(15,20,100,20),WS_CHILD|WS_VISIBLE|WS_GROUP);
  cLang.Create(bGroup,NULL,50,XRect(15,40,100,100),XDEFCOMBOBOXSTYLE|WS_TABSTOP|CBS_DROPDOWNLIST);
  cLang.AddString(LoadStringStatic(NULL,STR_IAPP_AUTOLANG));
  cLang.AddString(LoadStringStatic(NULL,STR_IAPP_ESPLANG));
  cLang.AddString(LoadStringStatic(NULL,STR_IAPP_ENGLANG));
  bAutoStart.Create(bGroup,STR_IAPP_AUTOSTART,104,XRect(15,70,160,20),XDEFBUTTONSTYLE|WS_TABSTOP|BS_AUTOCHECKBOX);
  bAutoStart.SetCheck(BST_CHECKED);
  bMinStart.Create(bGroup,STR_IAPP_MINSTART,105,XRect(15,90,160,20),XDEFBUTTONSTYLE|WS_TABSTOP|BS_AUTOCHECKBOX);
  bMinStart.SetCheck(BST_CHECKED);
  bForceLogout.Create(bGroup,STR_IAPP_FLOGOUT,105,XRect(15,110,160,20),XDEFBUTTONSTYLE|WS_TABSTOP|BS_AUTOCHECKBOX);
  bForceLogout.SetCheck(BST_CHECKED);
  bOk.Create(*this,STR_OK,IDOK,XRect(10,153,100,24),XDEFBUTTONSTYLE|SS_VCENTER|WS_TABSTOP);
  bCancel.Create(*this,STR_CANCEL,IDCANCEL,XRect(135,153,100,24),XDEFBUTTONSTYLE|SS_VCENTER|WS_TABSTOP);
  bOk.SetFocus();
  return 0;
}

VOID SettingsDlg::OnSave(WPARAM /*wParam*/,LPARAM /*lParam*/){
  EndDialog(0);
}

VOID SettingsDlg::OnCancel(WPARAM /*wParam*/,LPARAM /*lParam*/){
  OnClose();
}

VOID SettingsDlg::OnClose(){
  EndDialog(0);
}
