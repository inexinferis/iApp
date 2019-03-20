
#include "main.h"

#define IDB_ADVANCEDOPTIONS   0x501
#define IDB_DEFAULTENTRYPOINT 0x502
#define IDB_PATCH             0x503
#define IDB_RESTORE           0x504

#define ID_LOADERNAME         0x505
#define ID_KERNELNAME         0x506

/**************************************************************\
  About Dialog...
\**************************************************************/

BEGIN_EVENT_TABLE(PatchguardDlg,XSkinnedDlg)
  EVENT_CREATE(PatchguardDlg::OnCreate)
  EVENT_CLOSE(PatchguardDlg::OnClose)
  EVENT_BN_CLICKED(IDOK,PatchguardDlg::OnOk)
  EVENT_STN_CLICKED(IDB_ADVANCEDOPTIONS,PatchguardDlg::OnAdvancedOptionsClicked)
  EVENT_STN_CLICKED(IDB_PATCH,PatchguardDlg::OnPatchClicked)
  EVENT_STN_CLICKED(IDB_RESTORE,PatchguardDlg::OnRestoreClicked)
END_EVENT_TABLE()

PatchguardDlg::PatchguardDlg(IApp *app):pApp(app){
  pMainFrame=&app->mainFrame;
}

INT PatchguardDlg::OnCreate(LPCREATESTRUCT /*pcs*/){
  LPTSTR tmpmsg;
  BOOL bIsPatched=pApp->pgd.IsPatched();
  //set icon...
  SetIcon(LoadIcon(XGetInstanceHandle(),MAKEINTRESOURCE(IDI_IAPPICON)),FALSE);
  //center window
  CenterWindow();

  bClose.Create(*this,STR_CLOSE,IDOK,XRect(200,50,100,24),XDEFSTATICSTYLE|SS_VCENTER|WS_TABSTOP);
  bPatch.Create(*this,STR_PATCH,IDB_PATCH,XRect(200,10,100,24),XDEFSTATICSTYLE|SS_VCENTER|WS_TABSTOP);
  bRestore.Create(*this,STR_RESTORE,IDB_RESTORE,XRect(200,10,100,24),XDEFSTATICSTYLE|SS_VCENTER|WS_TABSTOP);

  if(bIsPatched){
    bPatch.ShowWindow(SW_HIDE);
  }else{
    bRestore.ShowWindow(SW_HIDE);
  }

  tmpmsg=FormatMsgEx(NULL,STR_IAPP_STATUS,LoadStringStatic(0,bIsPatched?STR_PATCHED:STR_NOTPATCHED));
  if(tmpmsg){
    sStatus.Create(*this,tmpmsg,50,XRect(10,10,200,24),XDEFSTATICSTYLE|SS_VCENTER);
    LocalFree(tmpmsg);
  }

  bAdvancedOptions.Create(*this,STR_ADVANCEDOPTIONS,IDB_ADVANCEDOPTIONS,XRect(8,50,150,20),
    XDEFBUTTONSTYLE|WS_TABSTOP|BS_AUTOCHECKBOX);

  bGroup.Create(*this,STR_ADVANCEDOPTIONS,IDC_STATIC,XRECT(0,80,304,110),WS_CHILD|BS_GROUPBOX);

  bDefaultEntryPoint.Create(bGroup,STR_DEFAULTENTRYE,IDB_DEFAULTENTRYPOINT,XRect(8,20,200,20),
    XDEFBUTTONSTYLE|WS_TABSTOP|BS_AUTOCHECKBOX);

  uLoaderName.Create(bGroup,NULL,ID_LOADERNAME,XRect(110,45,180,22),XDEFEDITSTYLE|WS_TABSTOP|ES_WANTRETURN);
  sLoaderName.Create(bGroup,STR_LOADERNAME,600,XRect(10,45,95,22),XDEFSTATICSTYLE|SS_NOTIFY|SS_LEFT|SS_VCENTER,&uLoaderName);
  uKernelName.Create(bGroup,NULL,ID_KERNELNAME,XRect(110,75,180,22),XDEFEDITSTYLE|WS_TABSTOP|ES_WANTRETURN);
  sKernelName.Create(bGroup,STR_KERNELNAME,602,XRect(10,75,95,22),XDEFSTATICSTYLE|SS_NOTIFY|SS_LEFT|SS_VCENTER,&uKernelName);
  uLoaderName.LimitText(8);
  uKernelName.LimitText(8);

  uKernelName.SetWindowText("ntkernel");
  uLoaderName.SetWindowText("osloader");

  bClose.SetFocus();
  return 0;
}

VOID PatchguardDlg::OnAdvancedOptionsClicked(WPARAM /*dwParams*/,LPARAM /*lParam*/){
  if(bAdvancedOptions.GetCheck()==BST_CHECKED){
    if(MessageBox(STR_ADV_WARNING,0,MB_OKCANCEL|MB_ICONWARNING)==IDOK){
      ResizeWindow(0,110);
      bGroup.ShowWindow();
    }else{
      bAdvancedOptions.SetCheck(FALSE);
    }
  }else{
    ResizeWindow(0,-110);
    bGroup.HideWindow();
  }
}

VOID PatchguardDlg::OnPatchClicked(WPARAM /*wParam*/,LPARAM /*lParam*/){
  CHAR sKernelName[16],sLoaderName[16];LPTSTR tmpmsg;
  DWORD wVr=GetWindowsVr();
  if(!wVr){
    pMainFrame->MessageBox(STR_INCOMPATIBLEWVR,0,MB_OK|MB_ICONSTOP);
    return;
  }
  if(!pApp->pgd.IsPatched()){
    uKernelName.GetWindowText(sKernelName,sizeof(sKernelName));
    uLoaderName.GetWindowText(sLoaderName,sizeof(sLoaderName));
    if(sKernelName[0]&&sLoaderName[0]){
      if(pApp->pgd.Init(wVr,sKernelName,sLoaderName)){
        if(pApp->pgd.CanBePatched()){
          if(pApp->pgd.Patch(bDefaultEntryPoint.GetCheck())){
            bPatch.ShowWindow(SW_HIDE);
            bRestore.ShowWindow(SW_SHOW);
            tmpmsg=FormatMsgEx(NULL,STR_IAPP_STATUS,LoadStringStatic(0,STR_PATCHED));
            if(tmpmsg){
              sStatus.SetWindowText(tmpmsg);
              LocalFree(tmpmsg);
            }
            return;
          }else{
            pMainFrame->MessageBox(STR_PATCHFAIL,0,MB_OK|MB_ICONSTOP);
          }
        }else{
          pMainFrame->MessageBox(STR_FILESEXISTS,0,MB_OK|MB_ICONSTOP);
        }
      }else{
        pMainFrame->MessageBox(STR_INITFAIL,0,MB_OK|MB_ICONSTOP);
      }
    }else{
      pMainFrame->MessageBox(STR_INVALIDNAMES,0,MB_OK|MB_ICONSTOP);
    }
  }
}

VOID PatchguardDlg::OnRestoreClicked(WPARAM /*wParam*/,LPARAM /*lParam*/){
  LPTSTR tmpmsg;
  if(pApp->pgd.IsPatched()){
    if(pApp->pgd.Restore()){
      bPatch.ShowWindow(SW_SHOW);
      bRestore.ShowWindow(SW_HIDE);
      tmpmsg=FormatMsgEx(NULL,STR_IAPP_STATUS,LoadStringStatic(0,STR_NOTPATCHED));
      if(tmpmsg){
        sStatus.SetWindowText(tmpmsg);
        LocalFree(tmpmsg);
      }
    }else{
      pMainFrame->MessageBox(STR_RESTOREFAIL,0,MB_OK|MB_ICONSTOP);
    }
  }
}

VOID PatchguardDlg::OnOk(WPARAM /*wParam*/,LPARAM /*lParam*/){
  CHAR sKernelName[16],sLoaderName[16];
  uKernelName.GetWindowText(sKernelName,sizeof(sKernelName));
  uLoaderName.GetWindowText(sLoaderName,sizeof(sLoaderName));
  OnClose();
}

VOID PatchguardDlg::OnClose(){
  EndDialog(0);
}
