#include "xmsgbox.h"

//Definición de Eventos a Interceptar...
BEGIN_EVENT_TABLE(XMsgBox,XDialog)
  EVENT_INITDIALOG(XMsgBox::OnInitDialog)
  EVENT_CLOSE(XMsgBox::OnClose)
  EVENT_HELP(XMsgBox::OnHelp)
  EVENT_BN_CLICKED(IDOK,XMsgBox::OnOption)
  EVENT_BN_CLICKED(IDCANCEL,XMsgBox::OnOption)
  EVENT_BN_CLICKED(IDABORT,XMsgBox::OnOption)
  EVENT_BN_CLICKED(IDIGNORE,XMsgBox::OnOption)
  EVENT_BN_CLICKED(IDNO,XMsgBox::OnOption)
  EVENT_BN_CLICKED(IDYES,XMsgBox::OnOption)
  EVENT_BN_CLICKED(IDRETRY,XMsgBox::OnOption)
  EVENT_BN_CLICKED(IDTRYAGAIN,XMsgBox::OnOption)
  EVENT_BN_CLICKED(IDCONTINUE,XMsgBox::OnOption)
  EVENT_BN_CLICKED(IDHELP,XMsgBox::OnHelp)
  EVENT_TIMER_ID(WM_XMSGBOX_TIMEOUT,XMsgBox::OnTimeOut)
END_EVENT_TABLE()

#define IDS_ERROR 2

XMsgBox::XMsgBox(){
  m_hIcon=NULL;
  m_hUser32=NULL;
  // Get dialog font
	NONCLIENTMETRICS ncm;
	ncm.cbSize=sizeof(NONCLIENTMETRICS);
	::SystemParametersInfo(SPI_GETNONCLIENTMETRICS,sizeof(NONCLIENTMETRICS),&ncm,0);
	m_hFont=::CreateFontIndirect(&ncm.lfMessageFont);
	m_bTimeOutSetted=FALSE;
}

XMsgBox::~XMsgBox(){
	if(m_hIcon!=NULL){
		::DestroyIcon(m_hIcon);
		m_hIcon=NULL;
	}
	if(m_hFont){
	  ::DeleteObject(m_hFont);
	  m_hFont=NULL;
	}
}

INT XMsgBox::MessageBox(HWND hWnd,LPCTSTR lpText,LPCTSTR lpCaption,UINT uType,UINT uTimeout){

  // For resources
  m_hUser32=::GetModuleHandle(_T("user32.dll"));
  if(!m_hUser32)return 0;

  // Get Caption
  TCHAR sBuffer[MAX_RES_STRING];
  if(!lpCaption){
    if(!LoadString(m_hUser32,IDS_ERROR,sBuffer,sizeof(sBuffer)/sizeof(TCHAR)))
      return 0;
    lpCaption=sBuffer;
  }

  // Make Styles
  DWORD dwStyle=WS_POPUPWINDOW|WS_CAPTION|WS_VISIBLE|DS_ABSALIGN|DS_NOIDLEMSG|DS_3DLOOK;
  if(uType&MB_SYSTEMMODAL)
    dwStyle|=DS_SYSMODAL|DS_SETFOREGROUND;
  else
    dwStyle|=DS_MODALFRAME|WS_SYSMENU;
  if(uType&MB_SETFOREGROUND)
    dwStyle|=DS_SETFOREGROUND;

  DWORD dwStyleEx=WS_EX_DLGMODALFRAME|WS_EX_WINDOWEDGE|WS_EX_CONTROLPARENT;
  if(uType&MB_TOPMOST)
    dwStyleEx|=WS_EX_TOPMOST;
  if(uType&MB_RTLREADING)
    dwStyleEx|=WS_EX_RTLREADING;
  if(uType&MB_RIGHT)
    dwStyleEx|=WS_EX_RIGHT;

  if(uType&MB_RTLREADING){
    dwStyleEx|=WS_EX_LAYOUTRTL;
    dwStyleEx&=~WS_EX_RIGHT;
    uType&=~MB_RTLREADING;
    uType^=MB_RIGHT;
  }

  // Save data to InitDialog
  m_lpCaption=lpCaption;
  m_lpText=lpText;
  m_uType=uType;
  m_uTimeout=uTimeout;

  // Do Modal
  if(::IsWindow(hWnd))
    ::EnableWindow(hWnd,FALSE);

  // Create Dialog
  INT nRet=CreateDlgBoxEx(dwStyleEx,XMSGBOX_CLASSNAME,lpCaption,dwStyle,XDEFAULTRECT,hWnd);

  // Reset Modal
  if(::IsWindow(hWnd))
    ::EnableWindow(hWnd,TRUE);

  return nRet;
}

BOOL XMsgBox::OnInitDialog(WPARAM /*wParam*/,LPARAM /*lParam*/){
  TCHAR sBuffer[XMSGBOX_MAXBTNSTR];INT len=0;
  SIZE sCaption,sIcon,sText,sButtons;UINT uDefId=0;
  BOOL bCancelButton=FALSE;
  // Fail if not loaded...
  if(!m_hUser32){
    EndDialog(0);
    return TRUE;
  }

  // Select the new font and store the old one.
  XClientDC dc(*this);
  HFONT hOldFont=(HFONT)dc.SelectObject(m_hFont);

  // Get Dialog Units Based on Font
  DWORD dwDUnits=GetDialogBaseUnitsEx(m_hFont);
  // Calculate Basic Margins
  UINT cxOuterMargin=XDUToXPix(dwDUnits,XMSGBOX_DU_OUTERMARGIN);
  UINT cxInnerMargin=XDUToXPix(dwDUnits,XMSGBOX_DU_INNERMARGIN);
  UINT cxButtonGap=XDUToXPix(dwDUnits,XMSGBOX_DU_BUTTONGAP);
  UINT cxMargin=2*cxOuterMargin;

  // Init Sizes
  ZeroMemory(&sCaption,sizeof(sCaption));
  ZeroMemory(&sIcon,sizeof(sIcon));
  ZeroMemory(&sText,sizeof(sText));
  // Buttons has min size
  sButtons.cx=XDUToXPix(dwDUnits,XMSGBOX_DU_BTNWIDTH);
  sButtons.cy=XDUToXPix(dwDUnits,XMSGBOX_DU_BTNHEIGHT);

  // Load icon...
	switch(m_uType&MB_ICONMASK){
    case MB_ICONEXCLAMATION:
      m_hIcon=::LoadIcon(NULL,IDI_EXCLAMATION);
      //MessageBeep(MB_ICONEXCLAMATION);
    break;
    case MB_ICONHAND:
      m_hIcon=::LoadIcon(NULL,IDI_HAND);
      //MessageBeep(MB_ICONHAND);
    break;
    case MB_ICONQUESTION:
      m_hIcon=::LoadIcon(NULL,IDI_QUESTION);
      //MessageBeep(MB_ICONQUESTION);
    break;
    case MB_ICONASTERISK:
      m_hIcon=::LoadIcon(NULL,IDI_ASTERISK);
      //MessageBeep(MB_ICONASTERISK);
    break;
    default:
      m_hIcon=NULL;
      //MessageBeep(MB_OK);
    break;
  }

  // Create Icon Window
  if(m_hIcon!=NULL){
		ICONINFO iiIconInfo;
		if(::GetIconInfo(m_hIcon,&iiIconInfo)){
		  // Create a handle to access the bitmap information of the icon.
      BITMAP bmIcon;
      // Retrieve the bitmap information of the icon.
      ::GetObject((HGDIOBJ)iiIconInfo.hbmColor,sizeof(bmIcon),&bmIcon);
      // Create the control for the icon.
      GetStaticIcon()->Create(*this,NULL,IDC_STATIC,XRECT(0,0,bmIcon.bmWidth,bmIcon.bmHeight),
        WS_CHILD|WS_VISIBLE|WS_GROUP|SS_ICON);
      // Set the icon of the control.
      GetStaticIcon()->SetIcon(m_hIcon);
      // Save Icon Size (Plus border)
      sIcon.cx=bmIcon.bmWidth+cxInnerMargin;
      sIcon.cy=bmIcon.bmHeight;
		}
	}

  // Create Text Window
  if(m_lpText){
    //get text from rsrc?
    LPCTSTR lpText=_T("");//If can't find Resource!
    LPTSTR szBuffer=NULL;
    if(IS_INTRESOURCE(m_lpText)){
      UINT uLen=::GetStringLength(XGetInstanceHandle(),(UINT_PTR)m_lpText);
      if(uLen&&(szBuffer=(LPTSTR)LocalAlloc(LMEM_ZEROINIT,++uLen*sizeof(TCHAR)))){
        if(::LoadString(XGetInstanceHandle(),(UINT_PTR)m_lpText,szBuffer,uLen))
          lpText=szBuffer;
      }
    }else{
      // Copy to an internal buffer to trim if needed!
      szBuffer=(LPTSTR)LocalAlloc(LMEM_ZEROINIT,(lstrlen(m_lpText)+1)*sizeof(TCHAR));
      lstrcpy(szBuffer,m_lpText);
      lpText=szBuffer;
    }
    // Create a rect with the maximum width.
    RECT rc={0,0,(INT)(GetSystemMetrics(SM_CXSCREEN)/8*5-sIcon.cx-cxMargin),
      GetSystemMetrics(SM_CYSCREEN)};
    // Trim message
    if(lstrlen(lpText)!=0){
      LPCTSTR str=lpText;
      while(*str==' ')str++;
      if(*str!=0){
        // Trim trailing space
        LPTSTR end=(LPTSTR)str+lstrlen(str)-1;
        while(end>str&&(*end==' '||*end=='\n'||*end=='\r'))end--;
        *(end+1)=0;
      }
      lpText=str;
    }
    // Realy has text?
    if(lstrlen(lpText)!=0)
      // Draw the text and retrieve the size of the text.
      dc.DrawText(lpText,&rc,DT_CALCRECT|DT_WORDBREAK|DT_EXPANDTABS|
        DT_NOPREFIX|DT_EXTERNALLEADING|DT_EDITCONTROL);
    else{
      rc.right=rc.left+1;
      rc.bottom=rc.top+1;
    }
    // Save Size
    sText.cx=rc.right;
    sText.cy=rc.bottom;
  	// Create a variable with the style of the control.
    DWORD dwStyle=WS_CHILD|WS_GROUP|WS_VISIBLE|SS_NOPREFIX|SS_EDITCONTROL,dwStyleEx=0;
    // Check whether the text should be right aligned.
    if(m_uType&MB_RIGHT)
      dwStyle|=SS_RIGHT;// Align the text to the right.
    else
      dwStyle|=SS_LEFT;// Align the text to the left.
    // Check whether the text will be read from right to left.
    if(m_uType&MB_RTLREADING)
      dwStyleEx|=WS_EX_RTLREADING;
    // Create the static control for the message.
    GetStaticText()->CreateEx(dwStyleEx,*this,lpText,IDC_STATIC,XRECT(0,0,rc.right,rc.bottom),dwStyle);
    // Set the font of the dialog.
    GetStaticText()->SetFont(m_hFont);
    //free buffer (if used)
    if(szBuffer)
      LocalFree(szBuffer);
	}

  // Get buttons
  UINT nButtons=(m_uType&MB_TYPEMASK);
  BOOL bHelpButton=(m_uType&MB_HELP)==MB_HELP;

  // Get num of buttons
	switch(nButtons){
    case MB_ABORTRETRYIGNORE:case MB_CANCELTRYCONTINUE:case MB_YESNOCANCEL:
      m_uNumButtons=3;
    break;
    case MB_OKCANCEL:case MB_RETRYCANCEL:case MB_YESNO:
      m_uNumButtons=2;
    break;
    case MB_OK:default:
      m_uNumButtons=1;
    break;
	}
	if(bHelpButton)
    m_uNumButtons++;

  // Get the default button.
  UINT nDefButton=(m_uType&MB_DEFMASK)>>8;
  if(nDefButton>=m_uNumButtons)
    nDefButton=0;

  // Run through all buttons
  for(UINT i=0;i<m_uNumButtons;i++){
    UINT uButtonId=IDOK;
    switch(nButtons){
      case MB_ABORTRETRYIGNORE:
        switch(i){
          case 0:uButtonId=IDABORT;break;
          case 1:uButtonId=IDRETRY;break;
          case 2:uButtonId=IDIGNORE;break;
          default:
            if(bHelpButton)
              uButtonId=IDHELP;
          break;
        }
      break;
      case MB_CANCELTRYCONTINUE:
        switch(i){
          case 0:uButtonId=IDCANCEL;bCancelButton=TRUE;break;
          case 1:uButtonId=IDTRYAGAIN;break;
          case 2:uButtonId=IDCONTINUE;break;
          default:
            if(bHelpButton)
              uButtonId=IDHELP;
          break;
        }
      break;
      case MB_YESNOCANCEL:
        switch(i){
          case 0:uButtonId=IDYES;break;
          case 1:uButtonId=IDNO;break;
          case 2:uButtonId=IDCANCEL;bCancelButton=TRUE;break;
          default:
            if(bHelpButton)
              uButtonId=IDHELP;
          break;
        }
      break;
      case MB_YESNO:
        switch(i){
          case 0:uButtonId=IDYES;break;
          case 1:uButtonId=IDNO;break;
          default:
            if(bHelpButton)
              uButtonId=IDHELP;
          break;
        }
      break;
      case MB_OKCANCEL:
        switch(i){
          case 0:uButtonId=IDOK;break;
          case 1:uButtonId=IDCANCEL;bCancelButton=TRUE;break;
          default:
            if(bHelpButton)
              uButtonId=IDHELP;
          break;
        }
      break;
      case MB_RETRYCANCEL:
        switch(i){
          case 0:uButtonId=IDRETRY;break;
          case 1:uButtonId=IDCANCEL;bCancelButton=TRUE;break;
          default:
            if(bHelpButton)
              uButtonId=IDHELP;
          break;
        }
      break;
      default:
        if(i>0&&bHelpButton)
          uButtonId=IDHELP;
        bCancelButton=TRUE;
      break;
    }
    if((len=::LoadString(m_hUser32,uButtonId+XMSGBOX_BTNSTRRSRC,sBuffer,sizeof(sBuffer)))!=0){
      // Retrieve the size of the text.
      SIZE sTextSize=dc.GetTextExtent(sBuffer,len);
      // Add Borders
      sTextSize.cx+=2*cxOuterMargin;
      sTextSize.cy+=2*cxOuterMargin/2;
      // Save Size (only if bigger)
      if(sButtons.cx<sTextSize.cx)
        sButtons.cx=sTextSize.cx;
      sButtons.cy=sTextSize.cy;
      // Mark Default button
      DWORD dwStyle=WS_CHILD|WS_VISIBLE|WS_TABSTOP;
      dwStyle|=(i==0?WS_GROUP:0);
      dwStyle|=(i==nDefButton?BS_DEFPUSHBUTTON:BS_PUSHBUTTON);
      // Save Default Button ID
      if(i==nDefButton)
        uDefId=uButtonId;
      // Create Button, maybe size will change!
      GetButton(i)->Create(*this,sBuffer,uButtonId,XRECT(0,0,sTextSize.cx,sTextSize.cy),dwStyle);
      // Set the font of the dialog.
      GetButton(i)->SetFont(m_hFont);
    }else{
      EndDialog(0);
      return TRUE;
    }
  }

  // Calculate Caption Size
  if(m_lpCaption){
    SIZE sCaptionSize={0,0};
    //what if lpCaption is a resource!
    LPCTSTR lpText=_T("");//If can't find Resource!
    LPTSTR szBuffer=NULL;
    if(IS_INTRESOURCE(m_lpCaption)){
      UINT uLen=::GetStringLength(XGetInstanceHandle(),(UINT_PTR)m_lpCaption);
      if(uLen&&(szBuffer=(LPTSTR)LocalAlloc(LMEM_ZEROINIT,++uLen*sizeof(TCHAR)))){
        if(::LoadString(XGetInstanceHandle(),(UINT_PTR)m_lpCaption,szBuffer,uLen))
          lpText=szBuffer;
      }
      sCaptionSize=dc.GetTextExtent(lpText,lstrlen(lpText));
    }else{
      sCaptionSize=dc.GetTextExtent(m_lpCaption,lstrlen(m_lpCaption));
    }
    sCaption.cx=sCaptionSize.cx+GetSystemMetrics(SM_CXSIZE)/3;
    INT xMaxSize=GetSystemMetrics(SM_CXSCREEN)/8*5;
    if(sCaption.cx>xMaxSize)
      sCaption.cx=xMaxSize;
    //free buffer (if used)
    if(szBuffer)
      LocalFree(szBuffer);
  }

  // Restore Font
	dc.SelectObject(hOldFont);

	//group sizes
  SIZE sInfoSize,sButtsSize,sContentSize;
  // Group buttons size
  sButtsSize.cx=m_uNumButtons*sButtons.cx+(m_uNumButtons-1)*cxButtonGap;
  sButtsSize.cy=sButtons.cy;
  // Group Icon/Text size
  sInfoSize.cx=__max(sCaption.cx,sText.cx)+sIcon.cx;
  sInfoSize.cy=__max(sIcon.cy,sText.cy);
  // Total Content Size (Based on ClientWindow!)
  sContentSize.cx=__max(sInfoSize.cx,sButtsSize.cx)+cxMargin;
  sContentSize.cy=sInfoSize.cy+cxInnerMargin+sButtsSize.cy+cxMargin;

  // Set margins
  UINT uXOffset=cxOuterMargin,uYOffset=cxOuterMargin;
	// Move Windows...
	if(m_hIcon!=NULL){
		// Move the icon control.
		GetStaticIcon()->SetWindowPos(NULL,uXOffset,uYOffset,0,0,SWP_NOSIZE|SWP_NOZORDER|SWP_NOACTIVATE);
		// Add the size of the icon
		uXOffset+=sIcon.cx;
	}

	if(m_lpText){
	  // Aligment?
	  if(m_uType&MB_RIGHT)
      uXOffset=sContentSize.cx-(sText.cx+cxOuterMargin);
	  // Center text vertical
	  if(sIcon.cy>sText.cy)
      uYOffset+=(sIcon.cy-sText.cy)/2;
	  // Set the position of the message text.
    GetStaticText()->SetWindowPos(NULL,uXOffset,uYOffset,0,0,SWP_NOSIZE|SWP_NOZORDER|SWP_NOACTIVATE);
	}

  // Center the buttons
  uXOffset=(sContentSize.cx-sButtsSize.cx)/2;
  uYOffset=sContentSize.cy-cxOuterMargin-sButtsSize.cy;
  // Run through all buttons.
	for(UINT i=0;i<m_uNumButtons;i++){
		// Move the button.
		GetButton(i)->SetWindowPos(NULL,uXOffset,uYOffset,sButtons.cx,sButtons.cy,SWP_NOZORDER|SWP_NOACTIVATE);
		// Set the new x position of the next button.
		uXOffset+=sButtons.cx+cxButtonGap;
	}

	// Resize Window (Based on Client)
	ClientResize(sContentSize.cx,sContentSize.cy);
	CenterWindow();

  // Set TopMost Window Z-Level
	if(m_uType&MB_TOPMOST)
    SetWindowPos(HWND_TOPMOST,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);

  // If this dialogbox does not contain a IDCANCEL button, then
  // remove the CLOSE command from the system menu.
  if(!bCancelButton){
    HMENU hMenu;
    if((hMenu=GetSystemMenu(FALSE)))
      DeleteMenu(hMenu,SC_CLOSE,(UINT)MF_BYCOMMAND);
  }

  if((m_uType&MB_TYPEMASK)==MB_OK){
    // Make the ID of OK button to be CANCEL, because we want
    // the ESC to terminate the dialogbox; GetDlgItem32() will
    // not fail, because this is MB_OK messagebox!
    HWND hwnd=::GetDlgItem(m_hWnd,IDOK);
    if(hwnd!=NULL){
      ::SetWindowLongPtr(hwnd,GWLP_ID,IDCANCEL);
      // Update Default Button Id
      uDefId=IDCANCEL;
    }
  }

	// Set Focus on Default Button
	GetButton(nDefButton)->SetFocus();
	SetDefId(uDefId);

  if(m_uTimeout!=INFINITE){
    SetTimer(WM_XMSGBOX_TIMEOUT,m_uTimeout);
    m_bTimeOutSetted=TRUE;
  }

  // Return FALSE to set the focus correctly.
	return FALSE;
}

INT XMsgBox::OnClose(){
  if(m_bTimeOutSetted){
    KillTimer(WM_XMSGBOX_TIMEOUT);
    m_bTimeOutSetted=FALSE;
  }
  EndDialog(IDCANCEL);
  return TRUE;
}

INT XMsgBox::OnHelp(WPARAM /*wParam*/,LPARAM /*lParam*/){
  // See if we need to inform the parent
  HWND hwndOwner=::GetWindow(m_hWnd,GW_OWNER);
  if(hwndOwner&&hwndOwner!=::GetDesktopWindow()){
    HELPINFO HelpInfo;
    HelpInfo.cbSize=sizeof(HELPINFO);
    HelpInfo.iContextType=HELPINFO_WINDOW;
    HelpInfo.iCtrlId=IDHELP;
    HelpInfo.hItemHandle=m_hWnd;
    HelpInfo.dwContextId=::GetWindowContextHelpId(m_hWnd);
    DWORD dwPos=::GetMessagePos();
    HelpInfo.MousePos.x=GET_X_LPARAM(dwPos);
    HelpInfo.MousePos.y=GET_Y_LPARAM(dwPos);
    ::SendMessage(hwndOwner,WM_HELP,0,(LPARAM)&HelpInfo);
  }
  return TRUE;
}

INT XMsgBox::OnOption(DWORD dwParam,HWND /*hWnd*/){
  if(!GetDlgItem(LOWORD(dwParam)))
    return FALSE;
  if(m_bTimeOutSetted){
    KillTimer(WM_XMSGBOX_TIMEOUT);
    m_bTimeOutSetted=FALSE;
  }
  EndDialog(LOWORD(dwParam));
  return TRUE;
}


VOID XMsgBox::OnTimeOut(TIMERPROC /*lpTimerFunc*/){
  EndDialog(IDCANCEL);
  m_bTimeOutSetted=FALSE;
}
