#include "main.h"

TabPage *TabPage::pPages=NULL;

BOOL TabPage::LoadCheat(){
  return TRUE;
}

BOOL TabPage::CheatIsLoaded(){
  return TRUE;
}

BOOL TabPage::InstallCheat(){
  return TRUE;
}

VOID TabPage::UninstallCheat(){
  return;
}

BOOL TabPage::UnloadCheat(){
  return TRUE;
}

BOOL TabPage::OnStatusCallback(HINTERNET /*hInternet*/,DWORD_PTR /*dwContext*/,DWORD dwInternetStatus,
  LPVOID /*lpvStatusInformation*/,DWORD /*dwStatusInformationLength*/){
  static DWORD dwLastStatus=0;
  switch(dwInternetStatus){
    case INTERNET_STATUS_RESOLVING_NAME:case INTERNET_STATUS_NAME_RESOLVED:
      if(dwLastStatus!=1){
        if(pMainFrame)
          pMainFrame->UpdateState(LoadStringStatic(0,STR_RESOLVING));
        dwLastStatus=1;
      }
		break;
    case INTERNET_STATUS_CONNECTING_TO_SERVER:case INTERNET_STATUS_CONNECTED_TO_SERVER:
      if(dwLastStatus!=2){
        if(pMainFrame)
          pMainFrame->UpdateState(LoadStringStatic(0,STR_CONNECTING));
        dwLastStatus=2;
      }
		break;
		case INTERNET_STATUS_SENDING_REQUEST:case INTERNET_STATUS_REQUEST_SENT:
		  if(dwLastStatus!=3){
        if(pMainFrame)
          pMainFrame->UpdateState(LoadStringStatic(0,STR_SENDINGREQUEST));
        dwLastStatus=3;
      }
		break;
    case INTERNET_STATUS_RECEIVING_RESPONSE:case INTERNET_STATUS_RESPONSE_RECEIVED:
      if(dwLastStatus!=4){
        if(pMainFrame)
          pMainFrame->UpdateState(LoadStringStatic(0,STR_RECEIVINGRESPONSE));
        dwLastStatus=4;
      }
		break;
    case INTERNET_STATUS_CLOSING_CONNECTION:case INTERNET_STATUS_CONNECTION_CLOSED:
      if(dwLastStatus!=5){
        if(pMainFrame)
          pMainFrame->UpdateState(LoadStringStatic(0,STR_CLOSINGCONNECTION));
        dwLastStatus=5;
      }
		break;
    default:break;
	}
  return TRUE;
}

BOOL TabPage::Uninstall(){
  return TRUE;
}

BOOL TabPage::CheckFiles(){
  return TRUE;
}

DWORD TabPage::GetState(){
  return 0;
}

VOID TabPage::UpdateState(){
}

BEGIN_EVENT_TABLE(TabPage,XSkinnedDlg)
  EVENT_CREATE(TabPage::OnCreate)
  EVENT_SIZE(TabPage::OnSize)
  EVENT_BN_CLICKED(IDB_START,TabPage::OnStart)
  EVENT_BN_CLICKED(IDB_EXTRA,TabPage::OnExtra)
END_EVENT_TABLE()

VOID TabPage::OnStart(WPARAM /*wParam*/,LPARAM /*lParam*/){
}

VOID TabPage::OnExtra(WPARAM /*wParam*/,LPARAM /*lParam*/){
}

INT TabPage::OnCreate(LPCREATESTRUCT /*pcs*/){
  //create groupbox
  m_GroupTopLeft.Create(*this,NULL,IDC_STATIC,XRECT(0,0,250,120),WS_CHILD|WS_VISIBLE|BS_GROUPBOX);
  m_GroupTopRight.Create(*this,NULL,IDC_STATIC,XRECT(0,0,0,0),WS_CHILD|WS_VISIBLE|BS_GROUPBOX);

  //load data...
  ICONINFO iiIconInfo;
  HICON hIcon=pIconLst->GetIcon(2);
  if(::GetIconInfo(hIcon,&iiIconInfo)){
    // Create a handle to access the bitmap information of the icon.
    BITMAP bmIcon;
    // Retrieve the bitmap information of the icon.
    ::GetObject((HGDIOBJ)iiIconInfo.hbmColor,sizeof(bmIcon),&bmIcon);
    // Create the control for the icon.
    m_CheatIcon.Create(*this,NULL,IDC_STATIC,XRECT(10,74,bmIcon.bmWidth,bmIcon.bmHeight),
      WS_CHILD|WS_VISIBLE|SS_ICON);
    // Set the icon of the control.
    m_CheatIcon.SetIcon(hIcon);
  }

  // extract bitmap from imagelist (getbitmap don't work...)
  XClientDC dcClient(*this);
  HBITMAP hBmp=pBmpLst->ExtractBitmap(dcClient,0);
  if(hBmp){
    // Create a handle to access the bitmap information of the icon.
    BITMAP bmIcon;
    // Retrieve the bitmap information of the icon.
    ::GetObject((HGDIOBJ)hBmp,sizeof(bmIcon),&bmIcon);
    // Create the control for the bmp.
    m_CheatBmp.Create(*this,NULL,IDC_STATIC,XRECT(1,1,bmIcon.bmWidth-2,bmIcon.bmHeight-2),
      WS_CHILD|WS_VISIBLE|SS_BITMAP);
    // Set the bmp to the control.
    m_CheatBmp.SetBitmap(hBmp);
  }

  // Create Status
  m_CheatStatus.Create(*this,NULL,IDC_STATIC,XRECT(0,0,0,0),
    WS_CHILD|WS_VISIBLE|SS_NOPREFIX|SS_EDITCONTROL);
  m_CheatStatus.SetFontSize(24);

  // Status Description...
  m_CheatDesc.Create(*this,NULL,IDC_STATIC,XRECT(0,0,0,0),
    WS_CHILD|WS_VISIBLE|SS_NOPREFIX|SS_EDITCONTROL);
  m_CheatDesc.SetFontSize(14);

  // Create buttons
  m_Start.Create(*this,NULL,IDB_START,XRECT(50,78,90,24),WS_CHILD|WS_GROUP|WS_VISIBLE);
  m_Extra.Create(*this,NULL,IDB_EXTRA,XRECT(150,78,90,24),WS_CHILD|WS_VISIBLE);

  // Create progress bar
  m_Progress.Create(*this,NULL,IDP_PROGRESS,XRECT(0,0,0,0),WS_CHILD|WS_VISIBLE);//WS_DISABLED
  m_Progress.SetBarColor(XCOLOR_BACKGROUND_ACTIVE);
  m_Progress.SetBkColor(XCOLOR_DARKBACKGROUND);

  // Create Description
  m_Html.Create(*this,NULL,156,XRECT(0,0,0,0),WS_CHILD|WS_VISIBLE);
  //m_Html.SetHtmlContent(description);

  //set status
  UpdateState();

  //crash if called before...
  m_Progress.DisableWindowTheme();
  return 0;
}

VOID TabPage::OnSize(UINT nType,POINTS dwSize){
  if(nType!=SIZE_MINIMIZED){
    if(m_GroupTopRight.IsValid())
      m_GroupTopRight.SetWindowPos(NULL,255,0,dwSize.x-255,120,SWP_NOZORDER);
    if(m_CheatStatus.IsValid())
      m_CheatStatus.SetWindowPos(NULL,265,15,dwSize.x-260,32,SWP_NOZORDER);
    if(m_CheatDesc.IsValid())
      m_CheatDesc.SetWindowPos(NULL,266,50,dwSize.x-260,16,SWP_NOZORDER);
    if(m_Progress.IsValid())
      m_Progress.SetWindowPos(NULL,265,80,dwSize.x-275,20,SWP_NOZORDER);
    if(m_Html.IsValid())
			m_Html.SetWindowPos(NULL,0,125,dwSize.x,dwSize.y-120,SWP_NOZORDER);
  }
}

