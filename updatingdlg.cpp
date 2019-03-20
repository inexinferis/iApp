#include "main.h"

/**************************************************************\
  Updating Dlg
\**************************************************************/

BEGIN_EVENT_TABLE(UpdateDlg,XSkinnedDlg)
  EVENT_CREATE(UpdateDlg::OnCreate)
  EVENT_BN_CLICKED(IDCANCEL,UpdateDlg::OnCancel)
  EVENT_DESTROY(UpdateDlg::OnDestroy)
END_EVENT_TABLE()

INT UpdateDlg::OnCreate(LPCREATESTRUCT /*pcs*/){
  //set icon...
  SetIcon(LoadIcon(XGetInstanceHandle(),MAKEINTRESOURCE(IDI_IAPPICON)),FALSE);
  //center wnd
  CenterWindow();
  // Create progress bar
  pProgress.Create(*this,NULL,IDP_UPDPROGRESS,XRECT(10,45,350,24),WS_CHILD|WS_VISIBLE|WS_DISABLED);
  pProgress.SetBarColor(XCOLOR_BACKGROUND_ACTIVE);
  pProgress.SetBkColor(XCOLOR_DARKBACKGROUND);
  //create static
  sStatus.Create(*this,STR_CONNECTING,50,XRect(10,5,250,30),XDEFSTATICSTYLE|SS_VCENTER);
  bCancel.Create(*this,STR_CANCEL,IDCANCEL,XRect(240,85,120,24),XDEFSTATICSTYLE|WS_TABSTOP|SS_VCENTER);
	//set status callback...
  pApp->con.SetStatusCallbackEx(this,LPXINTERNET_STATUS_CALLBACK(LPXWND_INTERNET_STATUS_CALLBACK(&UpdateDlg::OnStatusCallback)));
  //crash if called before...
  pProgress.DisableWindowTheme();
  return 0;
}

BOOL UpdateDlg::OnStatusCallback(HINTERNET /*hInternet*/,DWORD_PTR /*dwContext*/,DWORD dwInternetStatus,
  LPVOID /*lpvStatusInformation*/,DWORD /*dwStatusInformationLength*/){
  static DWORD dwLastStatus=0;
  switch(dwInternetStatus){
    case INTERNET_STATUS_RESOLVING_NAME:case INTERNET_STATUS_NAME_RESOLVED:
      if(dwLastStatus!=1){
        sStatus.SetWindowTextEx(XGetInstanceHandle(),STR_RESOLVING);
        dwLastStatus=1;
      }
		break;
    case INTERNET_STATUS_CONNECTING_TO_SERVER:case INTERNET_STATUS_CONNECTED_TO_SERVER:
      if(dwLastStatus!=2){
        sStatus.SetWindowTextEx(XGetInstanceHandle(),STR_CONNECTING);
        dwLastStatus=2;
      }
		break;
		case INTERNET_STATUS_SENDING_REQUEST:case INTERNET_STATUS_REQUEST_SENT:
		  if(dwLastStatus!=3){
        sStatus.SetWindowTextEx(XGetInstanceHandle(),STR_SENDINGREQUEST);
        dwLastStatus=3;
      }
		break;
    case INTERNET_STATUS_RECEIVING_RESPONSE:case INTERNET_STATUS_RESPONSE_RECEIVED:
      DWORD total,current;float rate;
      pApp->con.ConStatus(&total,&current,&rate);
      #if defined(_DEBUG) && !defined(VM_PROTECT)
        DbgPrint("--- data %d %d\n",(int)total,(int)current);
      #endif
      if(total&&current)
        pProgress.SetPos((int)((float)current/(float)total)*100);
      if(dwLastStatus!=4){
        sStatus.SetWindowTextEx(XGetInstanceHandle(),STR_RECEIVINGRESPONSE);
        dwLastStatus=4;
      }
		break;
    case INTERNET_STATUS_CLOSING_CONNECTION:case INTERNET_STATUS_CONNECTION_CLOSED:
      if(dwLastStatus!=5){
        sStatus.SetWindowTextEx(XGetInstanceHandle(),STR_CLOSINGCONNECTION);
        dwLastStatus=5;
      }
		break;
    default:break;
	}
  return TRUE;
}

VOID UpdateDlg::OnCancel(WPARAM /*wParam*/,LPARAM /*lParam*/){
  pApp->con.Disconnect();
}

VOID UpdateDlg::OnDestroy(){
  pApp->con.RemoveStatusCallbackEx();
}
