#include "xskinnedwnd.h"

XSkinnedWnd::XSkinnedWnd(XWnd *Wnd):wthis(Wnd){
  m_sysmenu=m_maxable=m_minable=m_sizable=m_hasicon=m_active=m_hasmenu=FALSE;
  m_backBrush.CreateSolid(XCOLOR_BACKGROUND);
  m_borderBrush.CreateSolid(XCOLOR_BACKGROUND_BORDER);

  // Get menu font
	NONCLIENTMETRICS ncm;
	ncm.cbSize=sizeof(NONCLIENTMETRICS);
	::SystemParametersInfo(SPI_GETNONCLIENTMETRICS,sizeof(NONCLIENTMETRICS),&ncm,0);
	m_hFont=::CreateFontIndirect(&ncm.lfCaptionFont);
	m_hMenuFont=::CreateFontIndirect(&ncm.lfMenuFont);
  m_hMenu=NULL;
}

XSkinnedWnd::~XSkinnedWnd(){
  if(m_hFont)
    ::DeleteObject(m_hFont);
  if(m_hMenuFont)
    ::DeleteObject(m_hMenuFont);
}

VOID XSkinnedWnd::SkinPreRegisterClass(LPWNDCLASSEX wcx){
  wcx->hbrBackground=::CreateSolidBrush(XCOLOR_BACKGROUND);
}

VOID XSkinnedWnd::SkinPreCreateWindow(LPCREATESTRUCT /*wcs*/){
  //???
}

VOID XSkinnedWnd::SkinPreTranslateMessage(LPMSG lpMsg){
  static BOOL bUnderline=FALSE;
  UINT nMsg=lpMsg->message;
  if(m_hasmenu&&(nMsg==WM_SYSKEYDOWN||nMsg==WM_SYSKEYUP||nMsg==WM_KEYUP)){// Alt key presed?
    if(((TCHAR)lpMsg->wParam==VK_MENU||(GetKeyState(VK_MENU)&0x80000000))&&!(GetKeyState(VK_CONTROL)&0x80000000)){
      if(bUnderline!=(BOOL)((GetKeyState(VK_MENU)&0x80000000)!=0)){
        RECT wr,mRect;
        wthis->GetWindowRect(&wr);
        CopyRect(&mRect,&m_rMenu);
        OffsetRect(&mRect,wr.left,wr.top);
        HRGN hReg=::CreateRectRgnIndirect(&mRect);
        DrawCaptionBar(*wthis,hReg,m_active,TRUE,!bUnderline);
        ::DeleteObject(hReg);
        bUnderline=(GetKeyState(VK_MENU)&0x80000000);
      }
      /*if((TCHAR)lpMsg->wParam==VK_MENU||((TCHAR)lpMsg->wParam==VK_F10&&
      !((GetKeyState(VK_SHIFT)&0x80000000)||(GetKeyState(VK_CONTROL)&0x80000000)||HIWORD(lpMsg->lParam)&KF_ALTDOWN)))*/
    }
  }
}

/*****************************************************************************\
  EVENTS
\*****************************************************************************/

VOID XSkinnedWnd::SkinSetText(LPSTR /*swText*/){
  if(wthis->GetStyle()&WS_CAPTION)
    DrawCaptionBar(*wthis,(HRGN)1,m_active);
}

VOID XSkinnedWnd::SkinSetIcon(BOOL /*bBig*/,HICON /*hIcon*/){
  if(wthis->GetStyle()&WS_CAPTION)
    DrawCaptionBar(*wthis,(HRGN)1,m_active);
}

BOOL XSkinnedWnd::SkinPrint(HDC hDC,DWORD dwFlag){
  if((dwFlag&PRF_CHECKVISIBLE)&&!wthis->IsVisible())
    return FALSE;
  if(dwFlag&PRF_NONCLIENT)
    DrawFrame(*wthis,(HRGN)1,m_active);
  if(dwFlag&PRF_CLIENT){
    RECT wr,cr;
    XDC pDC(hDC);
    INT iDC=pDC.Save();
    POINT pt=pDC.GetWindowOrg();
    wthis->GetClientRect(&cr);
    wthis->GetWindowRect(&wr);
    if(dwFlag & PRF_NONCLIENT)
      pDC.SetWindowOrg(pt.x-cr.left-wr.left,pt.y-cr.top-wr.top);
    pDC.IntersectClipRect(0,0,cr.right-cr.left,cr.bottom-cr.top);
    if(dwFlag&PRF_ERASEBKGND)
      wthis->SendMessage(WM_ERASEBKGND,(WPARAM)hDC,0L);
    wthis->SendMessage(WM_PRINTCLIENT,(WPARAM)hDC,dwFlag);
    pDC.Restore(iDC);
  }
  return TRUE;
}

/*****************************************************************************\
  NC EVENTS
\*****************************************************************************/

VOID XSkinnedWnd::SkinNCCreate(LPCREATESTRUCT /*pcs*/){
  DisableThemes();
}

BOOL XSkinnedWnd::SkinNCActivate(BOOL bActivate,HRGN hRgn){
  if(m_active==bActivate)
		return TRUE;;// nothing to do
  m_active=bActivate;
  DrawCaptionBar(*wthis,hRgn,bActivate);
  //DrawMenuBar(???)
  return TRUE;
}

VOID XSkinnedWnd::SkinNCPaint(HRGN hRgn){
  if(!wthis->IsVisible())
    return;
  if(wthis->GetStyle()&WS_CAPTION){
    // Skin System Menu (if Any...)
    if(wthis->GetSystemMenu()&&!xSysMenu.IsValid())
      xSysMenu.Attach(wthis->GetSystemMenu());
    // DrawFrame
    DrawFrame(*wthis,hRgn,m_active);
  }
}

INT XSkinnedWnd::SkinNCCalcSize(BOOL /*bValidCA*/,NCCALCSIZE_PARAMS *pParams){
  RECT* lprc=(RECT*)pParams;
  if(wthis->IsIconic()){
    lprc->right=lprc->left;
    lprc->bottom=lprc->top;
  }else if(wthis->IsZoomed()){
    if(wthis->GetStyle()&WS_CAPTION){
      InflateRect(lprc,-3*XBORDER_SIZE,-3*XBORDER_SIZE);
      lprc->top+=XCAPTION_SIZE;
      if(lprc->left>lprc->right)
        lprc->right=lprc->left;
      if(lprc->top>lprc->bottom)
        lprc->bottom=lprc->top;
    }
  }else{
    if(wthis->GetStyle()&WS_CAPTION){
      InflateRect(lprc,-2*XBORDER_SIZE,-2*XBORDER_SIZE);
      lprc->top+=XCAPTION_SIZE;
      if(lprc->left>lprc->right)
        lprc->right=lprc->left;
      if(lprc->top>lprc->bottom)
        lprc->bottom=lprc->top;
    }
    //MenuBarCompute...
    //ScrollBarsCompute...
  }
  return 0;
}

INT XSkinnedWnd::SkinNCHitTest(POINTS pt){
  POINT point;
  POINTSTOPOINT(point,pt);
  return HitTest(*wthis,point);
}

BOOL XSkinnedWnd::SkinNCLButtonDown(INT iHittest,POINTS pt){
  switch(iHittest){
    case HTCLOSE:case HTHELP:case HTMINBUTTON:case HTMAXBUTTON:
      wthis->SetCapture();
      for(;;){
        MSG msg;
        if(PeekMessage(&msg,NULL,WM_MOUSEFIRST,WM_MOUSELAST,PM_REMOVE)){
          if(msg.message==WM_LBUTTONUP){
            UINT cmd=0;
            wthis->ReleaseCapture();
            if(iHittest==HitTest(*wthis,msg.pt)){
              switch(iHittest){
                case HTCLOSE:
                  cmd=SC_CLOSE;
                break;
                case HTHELP:
                  cmd=SC_CONTEXTHELP;
                break;
                case HTMINBUTTON:
                  if(wthis->IsIconic())
                    cmd=SC_RESTORE;
                  else
                    cmd=SC_MINIMIZE;
                break;
                case HTMAXBUTTON:
                  if(wthis->IsZoomed())
                    cmd=SC_RESTORE;
                  else
                    cmd=SC_MAXIMIZE;
                break;
                default:break;
              }
            }
            if(cmd!=0){
              if(cmd!=SC_CONTEXTHELP){
                /*HMENU hMenu=wthis->GetSystemMenu(FALSE);//Not Work!!! Bugged Menu!!!
                if(hMenu&&(GetMenuState(hMenu,cmd,MF_BYCOMMAND)&MFS_GRAYED))
                  break;*/
              }
              wthis->SendMessage(WM_SYSCOMMAND,cmd,MAKELPARAM(pt.x,pt.y));
            }
            break;
          }
          //if(msg.message==WM_MOUSEMOVE)//draw pushed...
        }
        if(!WaitMessage()){
          wthis->ReleaseCapture();
          break;
        }
      }
    break;
    case HTSYSMENU:case HTMENU:
      wthis->SetActive();
      wthis->SendMessage(WM_SYSCOMMAND,SC_MOUSEMENU|iHittest,MAKELPARAM(pt.x,pt.y));
    break;
    case HTCAPTION:
      wthis->SetActive();
      if(!wthis->IsZoomed())
        wthis->SendMessage(WM_SYSCOMMAND,SC_MOVE|iHittest,MAKELPARAM(pt.x,pt.y));
    break;
    case HTHSCROLL:
      wthis->SendMessage(WM_SYSCOMMAND,SC_HSCROLL|iHittest,MAKELPARAM(pt.x,pt.y));
    break;
    case HTVSCROLL:
      wthis->SendMessage(WM_SYSCOMMAND,SC_VSCROLL|iHittest,MAKELPARAM(pt.x,pt.y));
    break;
    default:
      if(iHittest>=HTSIZEFIRST&&iHittest<=HTSIZELAST)
        wthis->SendMessage(WM_SYSCOMMAND,SC_SIZE+(iHittest-HTSIZEFIRST)+1,MAKELPARAM(pt.x,pt.y));
    break;
  }
  return TRUE;
}

BOOL XSkinnedWnd::SkinNCLButtonUp(INT /*iHittest*/,POINTS /*pt*/){
  return TRUE;
}

BOOL XSkinnedWnd::SkinNCLButtonDbClk(INT iHittest,POINTS pt){
  UINT cmd=0;
  if(iHittest==HTSYSMENU)
    cmd=SC_CLOSE;
  if(iHittest==HTCAPTION&&m_sizable){
    if(wthis->IsZoomed())
      cmd=SC_RESTORE;
    else
      cmd=SC_MAXIMIZE;
  }
  if(cmd)
    wthis->SendMessage(WM_SYSCOMMAND,cmd,MAKELONG(pt.x,pt.y));
  return 0;
}

BOOL XSkinnedWnd::SkinNCRButtonDown(INT iHittest,POINTS /*pt*/){
  if(iHittest==HTCAPTION||iHittest==HTSYSMENU){
    wthis->SetCapture();
    for(;;){
      MSG msg;
      if(PeekMessage(&msg,NULL,WM_MOUSEFIRST,WM_MOUSELAST,PM_REMOVE)){
        if(msg.message==WM_RBUTTONUP){
          wthis->ReleaseCapture();
          INT nHit=HitTest(*wthis,msg.pt);
          if((nHit==HTCAPTION)||(nHit==HTSYSMENU)||
            (nHit==HTVSCROLL)||(nHit==HTHSCROLL)){
            wthis->SendMessage(WM_CONTEXTMENU,(WPARAM)(HWND)*wthis,MAKELONG(msg.pt.x,msg.pt.y));
          }
          break;
        }
      }
      if(!WaitMessage()){
        wthis->ReleaseCapture();
        break;
      }
    }
  }
  return TRUE;
}

VOID XSkinnedWnd::SkinNCMouseMove(INT iHittest,POINTS pt){
  if(iHittest==HTHELP||iHittest==HTMINBUTTON||iHittest==HTMAXBUTTON||iHittest==HTCLOSE||iHittest==HTMENU){
    RECT wr,uRect,mRect;
    wthis->GetWindowRect(&wr);
    if(iHittest==HTHELP)
      CopyRect(&uRect,&m_rHelp);
    if(iHittest==HTMINBUTTON)
      CopyRect(&uRect,&m_rMin);
    if(iHittest==HTMAXBUTTON)
      CopyRect(&uRect,&m_rMax);
    if(iHittest==HTCLOSE)
      CopyRect(&uRect,&m_rClose);
    if(iHittest==HTMENU)
      CopyRect(&uRect,&m_rMenu);
    OffsetRect(&uRect,wr.left,wr.top);
    HRGN hReg=::CreateRectRgnIndirect(&uRect);
    if(iHittest==HTMENU){
      INT hilited=-1;
      HMENU hMenu=wthis->GetMenu();
      if(hMenu){
        INT numItems=::GetMenuItemCount(hMenu);
        for(INT i=0;i<numItems;i++){
          // Get menu rect...
          RECT mr;POINT cpt;
          POINTSTOPOINT(cpt,pt);
          wthis->GetMenuItemRect(hMenu,i,&mr);
          if(PtInRect(&mr,cpt)){
            hilited=i;
            wthis->HiliteMenuItem(hMenu,i,MF_BYPOSITION|MF_HILITE);
            CopyRect(&mRect,&mr);
          }
        }
        if(hilited!=-1){
          DrawCaptionBar(*wthis,hReg,m_active,TRUE);
          wthis->SetCapture();
          for(;;){
            MSG msg;
            if(PeekMessage(&msg,NULL,WM_MOUSEFIRST,WM_MOUSELAST,PM_REMOVE)){
              if(msg.message==WM_LBUTTONDOWN){
                wthis->ReleaseCapture();
                ::DeleteObject(hReg);
                SkinNCLButtonDown(iHittest,pt);
                return;
              }else{//WM_MOUSEMOVE
                if(!PtInRect(&mRect,msg.pt)){
                  wthis->ReleaseCapture();
                  break;
                }
              }
            }
            if(!WaitMessage()){
              wthis->ReleaseCapture();
              break;
            }
          }
          wthis->HiliteMenuItem(hMenu,hilited,MF_BYPOSITION|MF_UNHILITE);
          DrawCaptionBar(*wthis,hReg,m_active,TRUE);
        }
      }
    }else{
      m_moveHitTest=(UINT)iHittest;
      DrawCaptionBar(*wthis,hReg,m_active,TRUE);
      wthis->SetCapture();
      for(;;){
        MSG msg;
        if(PeekMessage(&msg,NULL,WM_MOUSEFIRST,WM_MOUSELAST,PM_REMOVE)){
          if(msg.message==WM_LBUTTONDOWN){
            wthis->ReleaseCapture();
            ::DeleteObject(hReg);
            m_moveHitTest=0;
            SkinNCLButtonDown(iHittest,pt);
            return;
          }else{//WM_MOUSEMOVE
            if(!PtInRect(&uRect,msg.pt)){
              wthis->ReleaseCapture();
              break;
            }
          }
        }
        if(!WaitMessage()){
          wthis->ReleaseCapture();
          break;
        }
      }
      m_moveHitTest=0;
      DrawCaptionBar(*wthis,hReg,m_active,TRUE);
    }
    ::DeleteObject(hReg);
  }
}

VOID XSkinnedWnd::SkinNCDestroy(){
  if(xSysMenu.IsValid())
    xSysMenu.Detach();
}

/*****************************************************************************\
  NC EVENTS
\*****************************************************************************/

HHOOK XSkinnedWnd::m_hCbtFilter=NULL;
XSkinnedWnd *XSkinnedWnd::m_pWndInit=NULL;
const TCHAR XSKINNEDMENU[]=_T("XSkinnedMenu");

VOID XSkinnedWnd::SkinInitMenuPopup(HMENU /*hMenu*/,DWORD /*info*/){
  if(m_hCbtFilter||m_pWndInit){
    ::UnhookWindowsHookEx(m_hCbtFilter);
    m_hCbtFilter=NULL;m_pWndInit=NULL;
  }
  m_pWndInit=this;
  m_hCbtFilter=::SetWindowsHookEx(WH_CALLWNDPROC,(HOOKPROC)&XSkinnedWnd::NewMenuHook,NULL,::GetCurrentThreadId());
}

LRESULT CALLBACK XSkinnedWnd::NewMenuHook(INT nCode,WPARAM wParam,LPARAM lParam){
  if(m_hCbtFilter&&m_pWndInit){
    HHOOK hCbtFilter=m_hCbtFilter;
    if(nCode<0)return CallNextHookEx(hCbtFilter,nCode,wParam,lParam);
    CWPSTRUCT* pTemp=(CWPSTRUCT*)lParam;
    if(pTemp->message==MN_SIZEWINDOW&&pTemp->hwnd!=NULL){
      TCHAR szClassName[20];
      INT Count=GetClassName(pTemp->hwnd,szClassName,sizeof(szClassName));
      // Check for the Menu-class
      if(Count==6&&!_tcscmp(szClassName,_T("#32768"))){
        if(!GetProp(pTemp->hwnd,XSKINNEDMENU)){
          //Set Data to Get Back
          SetProp(pTemp->hwnd,XSKINNEDMENU,(LPVOID)m_pWndInit);
          if(GetProp(pTemp->hwnd,XSKINNEDMENU)==(LPVOID)m_pWndInit){
            // Change Window Proc
            m_pWndInit->m_MenuProc=(WNDPROC)::SetWindowLongPtr(pTemp->hwnd,GWLP_WNDPROC,(DWORD_PTR)NewMenuProc);
            XASSERT(m_pWndInit->m_MenuProc!=NewMenuProc)
            //::SetWindowLongPtr(pTemp->hwnd,GWL_STYLE,GetWindowLongPtr(pTemp->hwnd,GWL_STYLE)&~(WS_BORDER));
            ::SetWindowLongPtr(pTemp->hwnd,GWL_EXSTYLE,GetWindowLongPtr(pTemp->hwnd,GWL_EXSTYLE)&~(WS_EX_WINDOWEDGE));//WS_EX_DLGMODALFRAME|
            ::UnhookWindowsHookEx(m_hCbtFilter);
            m_hCbtFilter=NULL;
            m_pWndInit=NULL;
          }
        }
      }
    }
    return CallNextHookEx(hCbtFilter,nCode,wParam,lParam);
  }
  return 0;
}

LRESULT CALLBACK XSkinnedWnd::NewMenuProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam){
  XSkinnedWnd *pthis=(XSkinnedWnd *)::GetProp(hWnd,XSKINNEDMENU);
  if(!pthis)return 0;
  WNDPROC oldWndProc=pthis->m_MenuProc;
  if(!oldWndProc)return 0;
  switch(uMsg){
    case WM_NCPAINT:{
      XWindowDC WDC(hWnd);XRECT wr;
      ::GetWindowRect(hWnd,&wr);
      ::OffsetRect(&wr,-wr.left,-wr.top);
      WDC.FrameSolidRect(&wr,XCOLOR_BACKGROUND_BORDER);
      ::InflateRect(&wr,-2,-2);
      WDC.FrameSolidRect(&wr,XCOLOR_BACKGROUND,3);
      return 0;
    }
    break;
    case WM_PRINT:{//Fix Bug "Attenuate or slide menus"
      LRESULT ret=CallWindowProc(oldWndProc,hWnd,uMsg,wParam,lParam);
      XDC WDC((HDC)wParam);XRECT wr;
      ::GetWindowRect(hWnd,&wr);
      ::OffsetRect(&wr,-wr.left,-wr.top);
      WDC.FrameSolidRect(&wr,XCOLOR_BACKGROUND_BORDER);
      ::InflateRect(&wr,-2,-2);
      WDC.FrameSolidRect(&wr,XCOLOR_BACKGROUND,3);
      return ret;
    }break;
    case WM_ERASEBKGND:
      return 0;
    break;
    case WM_NCDESTROY://Restore data
      if(::SetWindowLongPtr(hWnd,GWLP_WNDPROC,(INT_PTR)oldWndProc))
        ::RemoveProp(hWnd,XSKINNEDMENU);
    break;
  }
  return CallWindowProc(oldWndProc,hWnd,uMsg,wParam,lParam);
}

/*****************************************************************************\
  Private Methods
\*****************************************************************************/

VOID XSkinnedWnd::DisableThemes(){
  HMODULE uxtheme=GetModuleHandle(_T("uxtheme.dll"));
  if(uxtheme){
    typedef HRESULT (WINAPI *tSetWindowTheme)(HWND,LPCWSTR,LPCWSTR);
    tSetWindowTheme pSetWindowTheme=(tSetWindowTheme)GetProcAddress(uxtheme,"SetWindowTheme");
    if(pSetWindowTheme)
      pSetWindowTheme(*wthis,L"",L"");
  }
  HMODULE user32=GetModuleHandle(_T("user32.dll"));
  if(user32){
    typedef VOID (WINAPI *tDisableProcessWindowsGhosting)(VOID);
    tDisableProcessWindowsGhosting pDisableProcessWindowsGhosting=(tDisableProcessWindowsGhosting)GetProcAddress(user32,"DisableProcessWindowsGhosting");
    if(pDisableProcessWindowsGhosting)
      pDisableProcessWindowsGhosting();
  }
}

VOID XSkinnedWnd::DrawFrame(XWND &pWnd,HRGN hRgn,BOOL /*bActive*/){
  RECT rcClip;
  // Not visible? Do nothing!
  if(!pWnd.IsVisible())
    return;
  XDCEx WDC(pWnd,hRgn,DCX_WINDOW|DCX_USESTYLE|DCX_INTERSECTRGN|DCX_NODELETERGN|DCX_LOCKWINDOWUPDATE);
  if(hRgn>NULL||WDC.GetClipBox(&rcClip)!=NULLREGION){
    DrawCaptionBar(pWnd,hRgn,m_active);
    //DrawMenuBar(?)
    //DrawScrollBar(?)
  }
}

/*VOID XSkinnedWnd::DrawControls(XWND &pWnd,XDC &WDC,BOOL bActive){
  RECT wr;
  // Get Window Rect (Origin Based)
  pWnd.GetWindowRect(&wr);
  OffsetRect(&wr,-wr.left,-wr.top);
  // Get Caption Rect
  RECT tr={0,0,wr.right,2*XBORDER_SIZE+XCAPTION_SIZE};// Icon / Title / Buttons Rect
  InflateRect(&tr,-XBORDER_SIZE,-XBORDER_SIZE);

  // Get Buttons
  DWORD dwStyle=pWnd.GetStyle();
  m_sysmenu=dwStyle&WS_SYSMENU;
  m_maxable=dwStyle&WS_MAXIMIZEBOX;
  m_minable=dwStyle&WS_MINIMIZEBOX;
  m_sizable=dwStyle&WS_SIZEBOX;

  // Check if any option was disabled...
  HMENU hMenu=wthis->GetSystemMenu(FALSE);// BUGBUG!!! Bugged Menu!!!
  BOOL bSysmenu=(!hMenu||(GetMenuState(hMenu,SC_CLOSE,MF_BYCOMMAND)!=(UINT)-1));
  BOOL bMaxable=(!hMenu||(GetMenuState(hMenu,SC_MAXIMIZE,MF_BYCOMMAND)!=(UINT)-1));
  BOOL bMinable=(!hMenu||(GetMenuState(hMenu,SC_MINIMIZE,MF_BYCOMMAND)!=(UINT)-1));
  BOOL bSizable=(!hMenu||(GetMenuState(hMenu,SC_SIZE,MF_BYCOMMAND)!=(UINT)-1));

  // Draw Buttons
  if(m_sysmenu)
    DrawButton(WDC,tr,HTCLOSE,bSysmenu);
  if(m_maxable)
    DrawButton(WDC,tr,HTMAXBUTTON,bMaxable);
  if(m_minable)
    DrawButton(WDC,tr,HTMINBUTTON,bMinable);
  if(m_sizable&&bSizable)
    DrawResizeTriangle(WDC,wr);

  // Draw Title Bar
  DrawTitle(pWnd,WDC,tr,bActive);
}*/

VOID XSkinnedWnd::DrawCaptionBar(XWND &pWnd,HRGN hRgn,BOOL bActive,BOOL bUpdateCtrls,BOOL bAccel){
  RECT wr;XDC WDC;
  // Not visible? Do nothing!
  if(!pWnd.IsVisible())
    return;

  // Get DC (Windows 7 needs rgn != 1)
  if(hRgn!=(HRGN)1){
    WDC=pWnd.GetDCEx(hRgn,DCX_WINDOW|DCX_USESTYLE|(hRgn?DCX_INTERSECTRGN|DCX_NODELETERGN:0));
    if(!WDC.IsValid())
      WDC=pWnd.GetWindowDC();
  }else
    WDC=pWnd.GetWindowDC();
  XASSERT(WDC.IsValid())

  // Get Window Rect (Origin Based)
  pWnd.GetWindowRect(&wr);
  OffsetRect(&wr,-wr.left,-wr.top);

  if(!bUpdateCtrls){
    // Draw Borders
    WDC.FramePatBlt(&wr,2*XBORDER_SIZE,m_backBrush);
    WDC.FrameRect(&wr,m_borderBrush);

    // Draw Caption Background
    RECT cr={1,1,wr.right-2,2*XBORDER_SIZE+XCAPTION_SIZE};// Caption rect
    WDC.DrawGradient(&cr,bActive?XCOLOR_BACKGROUND_ACTIVE:XCOLOR_BACKGROUND_INACTIVE,XCOLOR_BACKGROUND,1);
  }

  UINT maxoffset = pWnd.IsZoomed() ? XBORDER_SIZE : 0;
  // Get Caption Rect
  RECT tr={(INT)maxoffset,(INT)maxoffset,(INT)(wr.right-maxoffset),(INT)(2*XBORDER_SIZE+XCAPTION_SIZE+maxoffset)};// Icon / Title / Buttons Rect
  //WDC.FrameRect(&tr,m_borderBrush);
  InflateRect(&tr,-XBORDER_SIZE,-XBORDER_SIZE);
  //WDC.FrameRect(&tr,m_borderBrush);

  // Get Buttons
  DWORD dwStyle=pWnd.GetStyle();
  m_sysmenu=dwStyle&WS_SYSMENU;
  m_maxable=dwStyle&WS_MAXIMIZEBOX;
  m_minable=dwStyle&WS_MINIMIZEBOX;
  m_sizable=dwStyle&WS_SIZEBOX;

  // Check if any option was disabled...
  HMENU hMenu=wthis->GetSystemMenu(FALSE);// BUGBUG!!! Bugged Menu!!!
  BOOL bSysmenu=(!hMenu||(GetMenuState(hMenu,SC_CLOSE,MF_BYCOMMAND)!=(UINT)-1));
  BOOL bMaxable=(!hMenu||(GetMenuState(hMenu,SC_MAXIMIZE,MF_BYCOMMAND)!=(UINT)-1));
  BOOL bMinable=(!hMenu||(GetMenuState(hMenu,SC_MINIMIZE,MF_BYCOMMAND)!=(UINT)-1));
  BOOL bSizable=(!hMenu||(GetMenuState(hMenu,SC_SIZE,MF_BYCOMMAND)!=(UINT)-1));

  // Draw Buttons
  if(m_sysmenu)
    DrawButton(WDC,tr,HTCLOSE,bSysmenu);
  if(m_maxable)
    DrawButton(WDC,tr,HTMAXBUTTON,bMaxable);
  if(m_minable)
    DrawButton(WDC,tr,HTMINBUTTON,bMinable);
  if(m_sizable&&bSizable)
    DrawResizeTriangle(WDC,wr);

  // Draw Title Bar
  DrawTitle(pWnd,WDC,tr,bActive,bAccel);
  pWnd.ReleaseDC(WDC);
}

//This part is experimental!
BOOL XSkinnedWnd::DrawMenuBar(XWND &pWnd,XDC &WDC,RECT &cr,BOOL bActive,BOOL bAccel){
  HMENU hMenu=wthis->GetMenu();
  m_hasmenu=(hMenu!=NULL&&IsMenu(hMenu));
  if(!m_hasmenu)
    return FALSE;
  // Skin System Menu (if Any...)
  /*if(m_hasicon&&!xSysMenu.IsValid())
    xSysMenu.Attach(wthis->GetSystemMenu());*/
  /*MENUITEMINFO mii;
  mii.cbSize=sizeof(mii);
  mii.fMask=MIIM_BITMAP|MIIM_DATA|MIIM_SUBMENU;
  if(::GetMenuItemInfo(hMenu,0,TRUE,&mii)&&(mii.hbmpItem!=HBMMENU_SYSTEM)){
    XMenuEx* pMenu=XMenuEx::FromHandle(hMenu);
    if(pMenu){
      XMenuEx* sMenu=pMenu->Prepend(0,NULL,MFS_ENABLED,MFT_STRING,HBMMENU_SYSTEM);
      //sMenu->SetSubMenu(wthis->GetSystemMenu());
      pMenu->SetSubMenu(wthis->GetSystemMenu());
    }else{
      mii.dwItemData=(DWORD)wthis->GetSafeHandle();
      mii.hSubMenu=NULL;//wthis->GetSystemMenu();
      mii.hbmpItem=HBMMENU_SYSTEM;
      InsertMenuItem(hMenu,0,TRUE,&mii);
    }
  }*/
  // Draw Menues
  HFONT hFontOld=(HFONT)WDC.SelectObject(m_hMenuFont);
  INT iOldBk=WDC.SetBkMode(TRANSPARENT);
  LONG clrOldText=WDC.SetTextColor(bActive?XCOLOR_TEXT_ACTIVE:XCOLOR_TEXT_INACTIVE);
  INT numItems=::GetMenuItemCount(hMenu);
  m_rMenu=cr;
  for(INT i=0;i<numItems;i++){
    MENUITEMINFO mii;
    TCHAR szMenu[XMENUSTRLEN];
    mii.cbSize=sizeof(mii);
    mii.fMask=MIIM_FTYPE|MIIM_STATE|MIIM_BITMAP|MIIM_STRING|MIIM_DATA|MIIM_ID;
    mii.dwTypeData=szMenu;mii.cch=(sizeof(szMenu)/sizeof(TCHAR))-1;
    if(::GetMenuItemInfo(hMenu,i,TRUE,&mii)){
      // Get menu rect...
      RECT mr,wr;
      wthis->GetWindowRect(&wr);
      wthis->GetMenuItemRect(hMenu,i,&mr);
      OffsetRect(&mr,-wr.left,-wr.top);
      // Call ownerdraw!
      if(mii.fType&MFT_OWNERDRAW){
        DRAWITEMSTRUCT dis;
        dis.CtlType=ODT_MENU;
        dis.CtlID=0;
        dis.itemID=mii.wID;
        dis.itemData=mii.dwItemData;
        dis.itemAction=ODA_DRAWENTIRE;
        dis.itemState=
         ((mii.fState&MF_GRAYED)?ODS_GRAYED:0)|
         ((mii.fState&MFS_DEFAULT)?ODS_DEFAULT:0)|
         ((mii.fState&MFS_CHECKED)?ODS_CHECKED:0)|
         ((mii.fState&MFS_DISABLED)?ODS_DISABLED:0)|
         ((mii.fState&MFS_HILITE)?ODS_HOTLIGHT:0)|
         (!(bAccel)?ODS_NOACCEL:0);
         //(mnDrawHilite(pItem)?ODS_SELECTED:0)|
         //(TestMF(pmenu, MFINACTIVE)?ODS_INACTIVE:0)|

        dis.hwndItem=(HWND)hMenu;
        dis.hDC=WDC;
        dis.rcItem=mr;
        wthis->SendMessage(WM_DRAWITEM,0,(LPARAM)&dis);
      }else{
        //not complety implemented, should be done in ownerdraw!
        if(mii.hbmpItem&&mii.hbmpItem==HBMMENU_SYSTEM){
          HICON hi=(HICON)pWnd.GetClassLong(GCL_HICONSM);
          if(!hi)
            hi=(HICON)pWnd.GetClassLong(GCL_HICON);
          if(!hi)// Use Default Icon...
            hi=::LoadIcon(NULL,IDI_APPLICATION);
          m_hasicon=(hi!=NULL);
          if(m_hasicon){
            WDC.DrawIconEx(mr.left+GetSystemMetrics(SM_CXEDGE),mr.top+GetSystemMetrics(SM_CYEDGE),
              hi,GetSystemMetrics(SM_CXSMICON),GetSystemMetrics(SM_CYSMICON),0,0,DI_NORMAL);
          }
        }
        // Very Basic Draw... (it's suppose to be done in ownerdraw)
        if(mii.dwTypeData)
          WDC.DrawTextEx(mii.dwTypeData,mii.cch,&mr,DT_HIDEPREFIX|DT_END_ELLIPSIS|DT_SINGLELINE|DT_CENTER|DT_VCENTER,NULL);
      }
      cr.left=mr.right;
    }
  }
  m_rMenu.right=cr.left;
  WDC.SetTextColor(clrOldText);
  WDC.SetBkMode(iOldBk);
  WDC.SelectObject(hFontOld);
  return TRUE;
}

RECT XSkinnedWnd::GetButtonRect(RECT &wRect){
	RECT rect={wRect.right-XBUTTON_SIZE,wRect.top,wRect.right,wRect.top+XBUTTON_SIZE};
  wRect.right-=XBUTTON_SIZE+XBORDER_SIZE;
  return rect;
}

VOID XSkinnedWnd::DrawTitle(XWND &pWnd,XDC &WDC,RECT &tr,BOOL bActive,BOOL bAccel){
  // Has icon?
  DWORD dwExStyle=pWnd.GetExStyle();//DLGFRAME??
  m_hasicon=!(dwExStyle&WS_EX_DLGMODALFRAME)&&!(dwExStyle&WS_EX_TOOLWINDOW);
  // Draw Menu
  BOOL bMenu=DrawMenuBar(pWnd,WDC,tr,bActive,bAccel);
  //WDC.FrameRect(&tr,m_borderBrush);
  if(!bMenu&&m_hasicon){
    HICON hi=pWnd.GetIcon(FALSE);
    if(!hi)
      hi=pWnd.GetIcon(TRUE);
    if(!hi)
      hi=(HICON)pWnd.GetClassLong(GCL_HICONSM);
    if(!hi)
      hi=(HICON)pWnd.GetClassLong(GCL_HICON);
    if(!hi)// Use Default Icon...
      hi=::LoadIcon(NULL,IDI_APPLICATION);
    m_hasicon=(hi!=NULL);
    if(m_hasicon){
      // Center Icon
      m_rIcon.right=GetSystemMetrics(SM_CXSMICON);
      m_rIcon.bottom=GetSystemMetrics(SM_CYSMICON);
      m_rIcon.left=tr.left+((tr.bottom-tr.top)-m_rIcon.right)/2;
      m_rIcon.top=tr.top+((tr.bottom-tr.top)-m_rIcon.bottom)/2;
      WDC.DrawIconEx(m_rIcon.left,m_rIcon.top,hi,m_rIcon.right,m_rIcon.bottom,0,0,DI_NORMAL);
      m_rIcon.right+=m_rIcon.left;
      m_rIcon.bottom+=m_rIcon.top;
      tr.left+=tr.bottom-tr.top;
    }
  }
  //WDC.FrameRect(&tr,m_borderBrush);
  // Draw Title
  TCHAR wtitle[XTITLEMAX];
  UINT len=pWnd.GetWindowText(wtitle,sizeof(wtitle)-1);
  if(len){
    HFONT hOldFont=(HFONT)WDC.SelectObject((HFONT)m_hFont);
    tr.left+=XBORDER_SIZE;
    LONG clrOldBk=WDC.SetBkMode(TRANSPARENT);
    LONG clrOldText=WDC.SetTextColor(bActive?XCOLOR_TEXT_ACTIVE:XCOLOR_TEXT_INACTIVE);
    WDC.DrawTextEx(wtitle,len,&tr,(bMenu?DT_CENTER:0)|DT_NOPREFIX|DT_END_ELLIPSIS|DT_SINGLELINE|DT_VCENTER,NULL);
    WDC.SetTextColor(clrOldText);
    WDC.SetBkColor(clrOldBk);
    WDC.SelectObject(hOldFont);
  }
}

VOID XSkinnedWnd::DrawButton(XDC &WDC,RECT &wr,UINT index,BOOL bEnabled){
	COLORREF colorfirst,colormiddle,colorshadow;

  if(bEnabled){
    if(m_moveHitTest==index){
      colorfirst=XCTRL_COLOR_SELECTED_FIRST;
      colormiddle=XCTRL_COLOR_SELECTED_MIDDLE;
      colorshadow=XCTRL_COLOR_SELECTED_SHADOW;
    }else{
      colorfirst=XCTRL_COLOR_NORMAL_FIRST;
      colormiddle=XCTRL_COLOR_NORMAL_MIDDLE;
      colorshadow=XCTRL_COLOR_NORMAL_SHADOW;
    }
  }else{
    colorfirst=XCTRL_COLOR_DISABLED_FIRST;
    colormiddle=XCTRL_COLOR_DISABLED_MIDDLE;
    colorshadow=XCTRL_COLOR_DISABLED_SHADOW;
  }

  switch(index){
    case HTHELP:
      m_rHelp=GetButtonRect(wr);
    break;
	  case HTMINBUTTON:
      m_rMin=GetButtonRect(wr);
      //WDC.FrameRect(&m_rMin,m_borderBrush);
      WDC.DrawSolidLine(m_rMin.left+6,m_rMin.bottom-7,m_rMin.right-6,m_rMin.bottom-7,colorshadow);
      WDC.DrawSolidLine(m_rMin.left+6,m_rMin.bottom-6,m_rMin.right-6,m_rMin.bottom-6,colorfirst);
      WDC.DrawSolidLine(m_rMin.left+6,m_rMin.bottom-5,m_rMin.right-6,m_rMin.bottom-5,colormiddle);
	  break;
	  case HTMAXBUTTON:
      m_rMax=GetButtonRect(wr);
      //WDC.FrameRect(&m_rMax,m_borderBrush);
      WDC.DrawSolidRectangle(m_rMax.left+5,m_rMax.top+6,m_rMax.right-5,m_rMax.bottom-5,colorfirst,XCOLOR_BACKGROUND_INACTIVE);
      WDC.DrawSolidLine(m_rMax.left+6,m_rMax.top+7,m_rMax.right-6,m_rMax.top+7,colorfirst);
      WDC.DrawSolidLine(m_rMax.left+6,m_rMax.top+8,m_rMax.right-6,m_rMax.top+8,colorshadow);
      WDC.DrawSolidLine(m_rMax.left+5,m_rMax.bottom-5,m_rMax.right-5,m_rMax.bottom-5,colorshadow);
	  break;
	  default://HTCLOSE
      m_rClose=GetButtonRect(wr);
      //WDC.FrameRect(&m_rClose,m_borderBrush);
      WDC.DrawSolidLine(m_rClose.left+6,m_rClose.top+6,m_rClose.right-5,m_rClose.bottom-5,colorshadow);
      WDC.DrawSolidLine(m_rClose.right-6,m_rClose.top+6,m_rClose.left+5,m_rClose.bottom-5,colorshadow);
      WDC.DrawSolidLine(m_rClose.left+4,m_rClose.top+6,m_rClose.right-7,m_rClose.bottom-5,colormiddle);
      WDC.DrawSolidLine(m_rClose.right-8,m_rClose.top+6,m_rClose.left+3,m_rClose.bottom-5,colormiddle);
      WDC.DrawSolidLine(m_rClose.left+5,m_rClose.top+6,m_rClose.right-6,m_rClose.bottom-5,colorfirst);
      WDC.DrawSolidLine(m_rClose.right-7,m_rClose.top+6,m_rClose.left+4,m_rClose.bottom-5,colorfirst);
	  break;
  }
}

VOID XSkinnedWnd::DrawResizeTriangle(XDC &WDC,RECT &wr){
  WDC.DrawSolidRectangle(wr.right-2,wr.bottom-2,wr.right-4,wr.bottom-4,XCTRL_COLOR_NORMAL_MIDDLE,XCTRL_COLOR_NORMAL_MIDDLE);
  WDC.DrawSolidRectangle(wr.right-6,wr.bottom-2,wr.right-8,wr.bottom-4,XCTRL_COLOR_NORMAL_MIDDLE,XCTRL_COLOR_NORMAL_MIDDLE);
  WDC.DrawSolidRectangle(wr.right-2,wr.bottom-6,wr.right-4,wr.bottom-8,XCTRL_COLOR_NORMAL_MIDDLE,XCTRL_COLOR_NORMAL_MIDDLE);

  WDC.DrawSolidRectangle(wr.right-6,wr.bottom-6,wr.right-8,wr.bottom-8,XCTRL_COLOR_NORMAL_MIDDLE,XCTRL_COLOR_NORMAL_MIDDLE);
  WDC.DrawSolidRectangle(wr.right-10,wr.bottom-2,wr.right-12,wr.bottom-4,XCTRL_COLOR_NORMAL_MIDDLE,XCTRL_COLOR_NORMAL_MIDDLE);
  WDC.DrawSolidRectangle(wr.right-2,wr.bottom-10,wr.right-4,wr.bottom-12,XCTRL_COLOR_NORMAL_FIRST,XCTRL_COLOR_NORMAL_FIRST);
}

INT XSkinnedWnd::HitTest(XWND &pWnd,POINT &point){
  RECT wr,rect;SIZE ws;
  //is on me?
  pWnd.GetWindowRect(&wr);
  if(!PtInRect(&wr,point))
		return HTNOWHERE;

  if(pWnd.GetStyle()&WS_CAPTION){
    //extra calc
    POINT pt;
    pt.x=point.x-wr.left;
    pt.y=point.y-wr.top;
    ws.cx=wr.right-wr.left;
    ws.cy=wr.bottom-wr.top;

    //we need check if any option was disabled...
    HMENU hMenu=pWnd.GetSystemMenu(FALSE);// BUGBUG!!! Bugged Menu!!!
    BOOL bSysmenu=(!hMenu||(GetMenuState(hMenu,SC_CLOSE,MF_BYCOMMAND)!=(UINT)-1));
    BOOL bMaxable=(!hMenu||(GetMenuState(hMenu,SC_MAXIMIZE,MF_BYCOMMAND)!=(UINT)-1));
    BOOL bMinable=(!hMenu||(GetMenuState(hMenu,SC_MINIMIZE,MF_BYCOMMAND)!=(UINT)-1));
    BOOL bSizable=(!hMenu||(GetMenuState(hMenu,SC_SIZE,MF_BYCOMMAND)!=(UINT)-1));

    /*if(m_???&&b???&&PtInRect(&m_rHelp,point))
      return HTHELP;*/

    if(m_sysmenu&&bSysmenu&&PtInRect(&m_rClose,pt))
      return HTCLOSE;

    if(m_maxable&&bMaxable&&PtInRect(&m_rMax,pt))
      return HTMAXBUTTON;

    if(m_minable&&bMinable&&PtInRect(&m_rMin,pt))
      return HTMINBUTTON;

    if(m_hasicon&&PtInRect(&m_rIcon,pt))
      return HTSYSMENU;

    if(m_hasmenu&&PtInRect(&m_rMenu,pt))
      return HTMENU;

    if(m_sizable&&bSizable&&!pWnd.IsZoomed()){
      SetRect(&rect,0,2*XBORDER_SIZE,XBORDER_SIZE,ws.cy-2*XBORDER_SIZE);
      if(PtInRect(&rect,pt))
        return HTLEFT;

      SetRect(&rect,2*XBORDER_SIZE,0,ws.cx-2*XBORDER_SIZE,XBORDER_SIZE);
      if(PtInRect(&rect,pt))
        return HTTOP;

      SetRect(&rect,ws.cx-XBORDER_SIZE,2*XBORDER_SIZE,ws.cx,ws.cy-2*XBORDER_SIZE);
      if(PtInRect(&rect,pt))
        return HTRIGHT;

      SetRect(&rect,2*XBORDER_SIZE,ws.cy-XBORDER_SIZE,ws.cx-2*XBORDER_SIZE,ws.cy);
      if(PtInRect(&rect,pt))
        return HTBOTTOM;

      SetRect(&rect,0,0,2*XBORDER_SIZE,2*XBORDER_SIZE);
      if(PtInRect(&rect,pt))
        return HTTOPLEFT;

      SetRect(&rect,ws.cx-2*XBORDER_SIZE,0,ws.cx,2*XBORDER_SIZE);
      if(PtInRect(&rect,pt))
        return HTTOPRIGHT;

      SetRect(&rect,0,ws.cy-2*XBORDER_SIZE,2*XBORDER_SIZE,ws.cy);
      if(PtInRect(&rect,pt))
        return HTBOTTOMLEFT;

      SetRect(&rect,ws.cx-2*XBORDER_SIZE,ws.cy-2*XBORDER_SIZE,ws.cx,ws.cy);
      if(PtInRect(&rect,pt))
        return HTBOTTOMRIGHT;
    }

    SetRect(&rect,0,0,ws.cx,2*XBORDER_SIZE+XCAPTION_SIZE);
    if(PtInRect(&rect,pt))
      return HTCAPTION;
  }
  return HTCLIENT;
}
