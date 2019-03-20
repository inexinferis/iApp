#include "xwnd.h"
#include "xmenu.h"

/*****************************************************************/
// Funciones Base...
/*****************************************************************/

IMPLEMENT_DYNCREATE(XWnd,XEvent)

//Definición de Eventos a Interceptar...
BEGIN_EVENT_TABLE(XWnd,XEvent)
END_EVENT_TABLE()

BEGIN_INTERFACE_EVENTS(XWnd,XEvent)
  INTERFACE_EVENT(XWnd,IID_IOleInPlaceFrame,OleIPFrame)
	INTERFACE_EVENT(XWnd,IID_IOleContainer,OleContainer)
END_INTERFACE_EVENTS()

HHOOK XWnd::m_hCbtFilter=NULL;
XWnd *XWnd::m_pWndInit=NULL;

XWnd::XWnd(HWND hWnd):m_hWnd(hWnd),m_hWndOwner(NULL),m_WndProc(NULL),m_bAttached(FALSE),m_nFlags(0){
  //Init Structs
  ZeroMemory(&m_cMsg,sizeof(m_cMsg));
  ZeroMemory(&m_wcs,sizeof(m_wcs));
  ZeroMemory(&m_wcx,sizeof(m_wcx));
  //Ole Support...
  m_pWndParent=NULL;
  m_pObject=NULL;
  m_pActiveObject=NULL;
  m_pInPlaceObject=NULL;
  m_pWindowlessObject=NULL;
  m_pSiteUIActive=m_pSiteCapture=m_pSiteFocus=NULL;
  m_nWindowlessControls=0;
  m_bIsWindowless=FALSE;
  ZeroMemory(&m_rect,sizeof(m_rect));
  ZeroMemory(&CtrlAppearance,sizeof(CtrlAppearance));
  CtrlAppearance.colorBack=(COLORREF)-1;
  CtrlAppearance.colorFore=(COLORREF)-1;
  m_nCtrlId=0;
}

XWnd::~XWnd(){
  if(m_hWnd!=NULL&&m_bAttached)
    DestroyWindow();
}

BOOL XWnd::Attach(HWND hWnd){
  XASSERT(m_hWnd==NULL)
  if(hWnd!=NULL&&::IsWindow(hWnd)){
    //Set Handle...
    m_hWnd=hWnd;
    //add to Wnd's maps
    m_bAttached=XWinApp::m_XWndMap.Add(hWnd,this);
    //Save Object Pointer
    if(!::GetProp(hWnd,XEVENTOBJECT)){
      ::SetProp(hWnd,XEVENTOBJECT,(PVOID)this);
      if(::GetProp(hWnd,XEVENTOBJECT)==(PVOID)this){
        // Allow other changes
        PreSubclassWindow();
        // Change window proc...
        m_WndProc=(WNDPROC)::SetWindowLongPtr(hWnd,GWLP_WNDPROC,(DWORD_PTR)XWndProc);
        XASSERT(m_WndProc!=XWndProc)
        return (m_WndProc!=NULL);
      }
    }
  }
  return FALSE;
}

HWND XWnd::Detach(){
  //release window object
  HWND hWnd=m_hWnd;
  if(hWnd!=NULL){
    //kill client sites...
    if(m_XWndSiteMap.Size()){
      std::map<XWnd*,HWND>::const_iterator it;
      XWnd* pCtrlSite=NULL;
      for(it=m_XWndSiteMap.Begin();it!=m_XWndSiteMap.End();it++){
        //destroy client windows (they can't exist without me!)
        //(no matter if the Window is not destroyed)
        if(pCtrlSite)
          pCtrlSite->DestroyWindow();
        //make sure to increse pointer before destroy
        //(client will try to remove itself)
        pCtrlSite=(*it).first;
        XASSERT(pCtrlSite!=NULL)
      }
      //destroy last (if any)
      if(pCtrlSite)
        pCtrlSite->DestroyWindow();
      m_XWndSiteMap.RemoveAll();
    }
    // Remove Object Pointer
    if(::IsWindow(hWnd)&&::GetProp(hWnd,XEVENTOBJECT)){
      ::RemoveProp(hWnd,XEVENTOBJECT);
      if(m_WndProc!=NULL){
        WNDPROC WndProc=(WNDPROC)::SetWindowLongPtr(hWnd,GWLP_WNDPROC,(DWORD_PTR)m_WndProc);
        XASSERT(WndProc==XWndProc)
        XUNUSED(WndProc)
        //restore window proc
        m_WndProc=NULL;
      }
    }
    if(m_bAttached){
      //remove from Wnd's maps
      XWinApp::m_XWndMap.Remove(hWnd);
      m_bAttached=FALSE;
    }
    // Unset Handle...
    m_hWnd=NULL;
  }
  return hWnd;
}

XWnd *XWnd::FromHandle(HWND hwnd){
  XWnd *pWnd=NULL;
  if(::IsWindow(hwnd)){
    pWnd=FromHandlePermanent(hwnd);
    if(!pWnd){
      pWnd=new XWnd;
      if(pWnd&&!pWnd->Attach(hwnd)){
        delete pWnd;
        pWnd=NULL;
      }
    }
  }
  return pWnd;
}

XWnd *XWnd::FromHandlePermanent(HWND hwnd){
  if(::IsWindow(hwnd))
    return (XWnd *)::GetProp(hwnd,XEVENTOBJECT);
  return NULL;
}

BOOL XWnd::PreTranslateMessage(LPMSG /*lpMsg*/){
  return FALSE;
}

BOOL XWnd::PreCreateWindow(LPCREATESTRUCT wcs){
  if(wcs->lpszClass==NULL)
		wcs->lpszClass=XDEFAULTWNDCLASSNAME;
  return TRUE;
}

BOOL XWnd::PreRegisterClass(LPWNDCLASSEX wcx){
  //use default class?
  if(!wcx->lpszClassName)
    wcx->lpszClassName=XDEFAULTWNDCLASSNAME;
  return TRUE;
}

HWND XWnd::CreateEx(DWORD dwExStyle,LPCTSTR lpszClassName,LPCTSTR lpszWindowName,DWORD dwStyle,
INT x,INT y,INT nWidth,INT nHeight,HWND hWndParent,HMENU hMenu,LPVOID lpParam){
  XASSERT(m_hWnd==NULL)
  HWND hWnd=NULL;
  //is already registered...
  m_wcx.cbSize=sizeof(WNDCLASSEX);
  if(!lpszClassName||!::GetClassInfoEx(XGetInstanceHandle(),lpszClassName,&m_wcx)){
    //set default data:
    m_wcx.cbSize=sizeof(WNDCLASSEX);
    m_wcx.hbrBackground=XDEFAULTBACKGROUND;
    m_wcx.style=XDEFAULTWNDCLASSSTYLE;
    m_wcx.hCursor=XDEFAULTCURSOR;
    m_wcx.hIconSm=XDEFAULTICON;
    m_wcx.hIcon=XDEFAULTICON;
    m_wcx.cbClsExtra=0;
    m_wcx.cbWndExtra=0;
    m_wcx.lpszMenuName=NULL;
    m_wcx.lpszClassName=lpszClassName;
    m_wcx.lpfnWndProc=::DefWindowProc;
    m_wcx.hInstance=XGetInstanceHandle();
    //user need change this?
    if(!PreRegisterClass(&m_wcx))
      return hWnd;
    //try to register if not already registered...
    WNDCLASSEX wcx;wcx.cbSize=sizeof(WNDCLASSEX);
    if(!::GetClassInfoEx(XGetInstanceHandle(),m_wcx.lpszClassName,&wcx)&&
       !::RegisterClassEx(&m_wcx))
      return hWnd;
  }

  //Fill Create Window Struct
  m_wcs.hInstance=XGetInstanceHandle();
  m_wcs.lpszClass=m_wcx.lpszClassName;
  m_wcs.lpCreateParams=lpParam;
  m_wcs.hwndParent=hWndParent;
  m_wcs.lpszName=lpszWindowName;
  m_wcs.dwExStyle=dwExStyle;
  m_wcs.style=dwStyle;
  m_wcs.hMenu=hMenu;
  m_wcs.x=x;m_wcs.y=y;
  m_wcs.cx=nWidth;
  m_wcs.cy=nHeight;
  //user need change this?
  if(!PreCreateWindow(&m_wcs))
    return hWnd;
  // this must not change!
  XASSERT(m_wcs.lpszClass==m_wcx.lpszClassName)
  XASSERT(m_wcs.hwndParent==hWndParent)
  //get window title from rsrc?
  LPTSTR szBuffer=NULL;
  LPCTSTR lptsWindowName=m_wcs.lpszName;
  if(lptsWindowName&&IS_INTRESOURCE(lptsWindowName)){
    UINT uLen=::GetStringLength(XGetInstanceHandle(),(UINT_PTR)lptsWindowName);
    if(uLen&&(szBuffer=(LPTSTR)LocalAlloc(LMEM_ZEROINIT,++uLen*sizeof(TCHAR)))){
      if(::LoadString(XGetInstanceHandle(),(UINT_PTR)lptsWindowName,szBuffer,uLen))
        lptsWindowName=szBuffer;
      else//Can't find Resource!
        lptsWindowName=NULL;
    }
  }
  //create window
  HookCreation();
  hWnd=CreateWindowEx(m_wcs.dwExStyle,m_wcs.lpszClass,lptsWindowName,m_wcs.style,
    m_wcs.x,m_wcs.y,m_wcs.cx,m_wcs.cy,m_wcs.hwndParent,m_wcs.hMenu,m_wcs.hInstance,m_wcs.lpCreateParams);
  UnhookCreation();
  //free buffer (if used)
  if(szBuffer)
    LocalFree(szBuffer);
  //check if was hooked ok
  XASSERT(hWnd==m_hWnd)
  return hWnd;
}

BOOL XWnd::DestroyWindow(){
  if(m_hWnd==NULL||!::IsWindow(m_hWnd))
    return FALSE;
  // Destroy Window
  if(::DestroyWindow(m_hWnd)){
    // Should have been detached by OnNcDestroy
    XASSERT(m_bAttached==FALSE)
    // make sure of detach!
    Detach();
    return TRUE;
  }
  return FALSE;
}

HWND XWnd::CreateDlg(HWND hWndParent,LPCTSTR szTemplate,LPARAM dwInitParam,HINSTANCE hInstance){
  XASSERT(m_hWnd==NULL)
  HWND hWnd=NULL;
  //find template
	HRSRC hResource=::FindResource(hInstance,szTemplate,RT_DIALOG);
	HGLOBAL hDialogTemplate=::LoadResource(hInstance,hResource);
  if(hDialogTemplate){
    //lock dialog..
    LPCDLGTEMPLATE lpTemplate=(LPCDLGTEMPLATE)::LockResource(hDialogTemplate);
    XASSERT(lpTemplate!=NULL)
    //create dialog
    hWnd=CreateDlgIndirect(hWndParent,lpTemplate,dwInitParam,hInstance);
    ::FreeResource(hDialogTemplate);
  }
  return hWnd;
}

HWND XWnd::CreateDlgIndirect(HWND hWndParent,LPCDLGTEMPLATE lpTemplate,LPARAM dwInitParam,HINSTANCE hInstance){
  XASSERT(m_hWnd==NULL&&lpTemplate!=NULL)
  HWND hWnd=NULL;
  if(!hInstance)
    hInstance=XGetInstanceHandle();
  //set flags
  m_nModRes=-1;
  //create dialog
  HookCreation();
	hWnd=::CreateDialogIndirectParam(hInstance,lpTemplate,hWndParent,XDlgProc,dwInitParam);
	UnhookCreation();
  //check if was hooked ok
  XASSERT(hWnd==m_hWnd)
  return hWnd;
}

HWND XWnd::CreateDlgEx(DWORD dwExStyle,LPCTSTR lpszClassName,LPCTSTR lpszWindowName,DWORD dwStyle,CONST RECT& rect,HWND hWndParent,LPCTSTR lpszMenuName,LPVOID lParam){
  XASSERT(m_hWnd==NULL)
  HWND hWnd=NULL;
  LPWSTR lpWMenu=NULL,lpWClassName=NULL,lpWWindowName=NULL;
  //is already registered...
  m_wcx.cbSize=sizeof(WNDCLASSEX);
  if(lpszClassName&&!::GetClassInfoEx(XGetInstanceHandle(),lpszClassName,&m_wcx)){
    //get default class data (we need original dialog data)
    if(!::GetClassInfoEx(XGetInstanceHandle(),DIALOG_CLASS,&m_wcx))
      return hWnd;
    //set data: DefDlgProc
    m_wcx.lpszClassName=lpszClassName;
    m_wcx.hInstance=XGetInstanceHandle();
    //user need change this?
    if(!PreRegisterClass(&m_wcx))
      return hWnd;
    //try to register if not already registered...
    WNDCLASSEX wcx;wcx.cbSize=sizeof(WNDCLASSEX);
    if(!::GetClassInfoEx(XGetInstanceHandle(),m_wcx.lpszClassName,&wcx)&&
       !::RegisterClassEx(&m_wcx))
      return hWnd;
  }else//try get default data
    if(!lpszClassName&&!::GetClassInfoEx(XGetInstanceHandle(),DIALOG_CLASS,&m_wcx))
      return hWnd;

  //Fill Create Window Struct
  m_wcs.hInstance=XGetInstanceHandle();
  m_wcs.lpszClass=m_wcx.lpszClassName;
  m_wcs.lpCreateParams=lParam;
  m_wcs.hwndParent=hWndParent;
  m_wcs.lpszName=lpszWindowName;
  m_wcs.dwExStyle=dwExStyle;
  m_wcs.style=dwStyle;
  m_wcs.hMenu=NULL;
  m_wcs.x=rect.left;
  m_wcs.y=rect.top;
  m_wcs.cx=rect.right;
  m_wcs.cy=rect.bottom;
  //user need change this?
  if(!PreCreateWindow(&m_wcs))
    return hWnd;

  //This must not change!
  XASSERT(m_wcs.hwndParent==hWndParent)

  //get window title from rsrc?
  LPTSTR szWindowNameBuffer=NULL;
  LPCTSTR lptsWindowName=m_wcs.lpszName;
  if(lptsWindowName&&IS_INTRESOURCE(lptsWindowName)){
    UINT uLen=::GetStringLength(XGetInstanceHandle(),(UINT_PTR)lptsWindowName);
    if(uLen&&(szWindowNameBuffer=(LPTSTR)LocalAlloc(LMEM_ZEROINIT,++uLen*sizeof(TCHAR)))){
      if(::LoadString(XGetInstanceHandle(),(UINT_PTR)lptsWindowName,szWindowNameBuffer,uLen))
        lptsWindowName=szWindowNameBuffer;
      else//Can't find Resource!
        lptsWindowName=NULL;
    }
  }

#ifndef UNICODE

  //Convert Strings
  LPWSTR lpWMenuBuffer=NULL,lpWClassNameBuffer=NULL,lpWWindowNameBuffer=NULL;UINT m_length;
  if(lpszMenuName){
    if(IS_INTRESOURCE(lpszMenuName))
      lpWMenu=(LPWSTR)lpszMenuName;
    else if((m_length=::MultiByteToWideChar(CP_ACP,0,lpszMenuName,-1,NULL,0))){
      lpWMenuBuffer=(LPWSTR)LocalAlloc(LMEM_ZEROINIT,m_length*sizeof(WCHAR));
      ::MultiByteToWideChar(CP_ACP,0,lpszMenuName,-1,lpWMenuBuffer,m_length);
      lpWMenu=lpWMenuBuffer;
    }
  }
  if(lpszClassName){
    if(IS_ATOM(lpszClassName))
      lpWClassName=(LPWSTR)lpszClassName;
    else if((m_length=::MultiByteToWideChar(CP_ACP,0,m_wcx.lpszClassName,-1,NULL,0))){
      lpWClassNameBuffer=(LPWSTR)LocalAlloc(LMEM_ZEROINIT,m_length*sizeof(WCHAR));
      ::MultiByteToWideChar(CP_ACP,0,m_wcx.lpszClassName,-1,lpWClassNameBuffer,m_length);
      lpWClassName=lpWClassNameBuffer;
    }
  }
  if(lpszWindowName&&(m_length=::MultiByteToWideChar(CP_ACP,0,lptsWindowName,-1,NULL,0))){
    lpWWindowNameBuffer=(LPWSTR)LocalAlloc(LMEM_ZEROINIT,m_length*sizeof(WCHAR));
    ::MultiByteToWideChar(CP_ACP,0,lptsWindowName,-1,lpWWindowNameBuffer,m_length);
    lpWWindowName=lpWWindowNameBuffer;
  }

#else

  //Just Assign
  lpWMenu=lpszMenuName;
  lpWClassName=m_wcx.lpszClassName;
  lpWWindowName=lptsWindowName;

#endif

  //Calculate Template Size
  ULONG_PTR cbLen=sizeof(DLGTEMPLATE);
  if(lpWMenu){
    if(IS_INTRESOURCE(lpWMenu))
      cbLen+=2*sizeof(WCHAR);
    else
      cbLen+=wcslen(lpWMenu)*sizeof(WCHAR);
  }
  cbLen+=sizeof(WCHAR);
  if(lpWClassName){
    if(IS_ATOM(lpWClassName))
      cbLen+=2*sizeof(WCHAR);
    else
      cbLen+=wcslen(lpWClassName)*sizeof(WCHAR);
  }
  cbLen+=sizeof(WCHAR);
  if(lpWWindowName)
    cbLen+=wcslen(lpWWindowName)*sizeof(WCHAR);
  cbLen+=sizeof(WCHAR);

  //Fill Template
  PWCHAR hTemplate=(PWCHAR)LocalAlloc(LMEM_ZEROINIT,cbLen);
  if(hTemplate){
    //Map Window Rect to Dialog Rect
    DWORD dwDlgBaseUnit=GetDialogBaseUnits();
    m_wcs.x=MulDiv(m_wcs.x,4,LOWORD(dwDlgBaseUnit));
    m_wcs.cx=MulDiv(m_wcs.cx,4,LOWORD(dwDlgBaseUnit));
    m_wcs.y=MulDiv(m_wcs.y,8,HIWORD(dwDlgBaseUnit));
    m_wcs.cy=MulDiv(m_wcs.cy,8,HIWORD(dwDlgBaseUnit));
    //Create template...
    LPDLGTEMPLATE lpDlgTmp=(LPDLGTEMPLATE)hTemplate;
    lpDlgTmp->style=m_wcs.style;
    lpDlgTmp->dwExtendedStyle=m_wcs.dwExStyle;
    lpDlgTmp->cdit=0;
    lpDlgTmp->x=m_wcs.x;
    lpDlgTmp->y=m_wcs.y;
    lpDlgTmp->cx=m_wcs.cx;
    lpDlgTmp->cy=m_wcs.cy;
    //Fill Arrays
    PWCHAR p=hTemplate+sizeof(DLGTEMPLATE)/sizeof(WCHAR);
    if(lpWMenu){
      if(IS_INTRESOURCE(lpWMenu)){
        *p++=0xFFFF;
        *p++=(WORD)(DWORD_PTR)lpWMenu;
      }else{
        wcscpy(p,lpWMenu);
        p+=wcslen(lpWMenu);
      }
    }
    p++;
    if(lpWClassName){
      if(IS_ATOM(lpWClassName)){
        *p++=0xFFFF;
        *p++=(WORD)(DWORD_PTR)lpWClassName;
      }else{
        wcscpy(p,lpWClassName);
        p+=wcslen(lpWClassName);
      }
    }
    p++;
    if(lpWWindowName){
      wcscpy(p,lpWWindowName);
      p+=wcslen(lpWWindowName);
    }
    //Create Dialog...
    hWnd=CreateDlgIndirect(hWndParent,(LPDLGTEMPLATE)hTemplate,(LPARAM)m_wcs.lpCreateParams);
    LocalFree(hTemplate);
  }

#ifndef UNICODE
  //Free Buffers (if we use it)
  if(lpWMenuBuffer)
    LocalFree(lpWMenuBuffer);
  if(lpWClassNameBuffer)
    LocalFree(lpWClassNameBuffer);
  if(lpWWindowNameBuffer)
    LocalFree(lpWWindowNameBuffer);
#endif
  //free buffer (if used)
  if(szWindowNameBuffer)
    LocalFree(szWindowNameBuffer);
  return hWnd;
}

INT_PTR XWnd::CreateDlgBox(HWND hWndParent,LPCTSTR szTemplate,LPARAM dwInitParam,HINSTANCE hInstance){
  XASSERT(m_hWnd==NULL)
  INT_PTR ret=-1;
  //find template
	HRSRC hResource=::FindResource(hInstance,szTemplate,RT_DIALOG);
	HGLOBAL hDialogTemplate=::LoadResource(hInstance,hResource);
  if(hDialogTemplate){
    //lock dialog..
    LPCDLGTEMPLATE lpTemplate=(LPCDLGTEMPLATE)::LockResource(hDialogTemplate);
    XASSERT(lpTemplate!=NULL)
    //create dialog
    ret=CreateDlgBoxIndirect(hWndParent,lpTemplate,dwInitParam,hInstance);
    ::FreeResource(hDialogTemplate);
  }
  return ret;
}

INT_PTR XWnd::CreateDlgBoxIndirect(HWND hWndParent,LPCDLGTEMPLATE lpTemplate,LPARAM dwInitParam,HINSTANCE hInstance){
  XASSERT(m_hWnd==NULL&&lpTemplate!=NULL)
  INT_PTR iModalRet=-1;
  if(CreateDlgIndirect(hWndParent,lpTemplate,dwInitParam,hInstance)){
    // need disable parent?
    BOOL bNeedEnableParent=FALSE;
    if(hWndParent&&hWndParent!=::GetDesktopWindow()&&::IsWindowEnabled(hWndParent)){
      ::EnableWindow(hWndParent,FALSE);
      bNeedEnableParent=TRUE;
    }
    // init modal loop
    iModalRet=DoModal();
    XASSERT(iModalRet==m_nModRes)
    // hide the window
    if(m_hWnd!=NULL)
      ::SetWindowPos(m_hWnd,NULL,0,0,0,0,SWP_HIDEWINDOW|SWP_NOSIZE|SWP_NOMOVE|SWP_NOACTIVATE|SWP_NOZORDER);
    // restore parent
    if(bNeedEnableParent)
      ::EnableWindow(hWndParent,TRUE);
    if(hWndParent!=NULL&&::GetActiveWindow()==m_hWnd)
      ::SetActiveWindow(hWndParent);
    // destroy modal window
    ::DestroyWindow(m_hWnd);
    // Should have been detached by OnNcDestroy
    XASSERT(m_bAttached==FALSE)
    // make sure of detach!
    Detach();
	}
	return iModalRet;
}

INT_PTR XWnd::CreateDlgBoxEx(DWORD dwExStyle,LPCTSTR lpszClassName,LPCTSTR lpszWindowName,DWORD dwStyle,CONST RECT& rect,HWND hWndParent,LPCTSTR lpszMenuName,LPVOID lParam){
  XASSERT(m_hWnd==NULL)
  INT_PTR iModalRet=-1;
  if(CreateDlgEx(dwExStyle,lpszClassName,lpszWindowName,dwStyle,rect,hWndParent,lpszMenuName,lParam)){
    // need disable parent?
    BOOL bNeedEnableParent=FALSE;
    if(hWndParent&&hWndParent!=::GetDesktopWindow()&&::IsWindowEnabled(hWndParent)){
      ::EnableWindow(hWndParent,FALSE);
      bNeedEnableParent=TRUE;
    }
    // init modal loop
    iModalRet=DoModal();
    XASSERT(iModalRet==m_nModRes)
    // hide the window
    if(m_hWnd!=NULL)
      ::SetWindowPos(m_hWnd,NULL,0,0,0,0,SWP_HIDEWINDOW|SWP_NOSIZE|SWP_NOMOVE|SWP_NOACTIVATE|SWP_NOZORDER);
    // restore parent
    if(bNeedEnableParent)
      ::EnableWindow(hWndParent,TRUE);
    if(hWndParent!=NULL&&::GetActiveWindow()==m_hWnd)
      ::SetActiveWindow(hWndParent);
    // destroy modal window
    ::DestroyWindow(m_hWnd);
    // Should have been detached by OnNcDestroy
    XASSERT(m_bAttached==FALSE)
    // make sure of detach!
    Detach();
	}
	return iModalRet;
}

INT_PTR XWnd::DoModal(){
  XASSERT(::IsWindow(m_hWnd)) // window must be created
	XASSERT(!(m_nFlags&XWF_MODALLOOP)) // window must not already be in modal state
  XWinApp *pApp=XGetApp();
  XASSERT(pApp!=NULL)
  // for tracking the idle time state
	BOOL bIdle=TRUE;
	m_nFlags|=XWF_MODALLOOP;
	PMSG pMsg=pApp->GetCurrentMessage();
	HWND hWndParent=::GetParent(m_hWnd);
	BOOL bFirstEmpty=TRUE;
  //start loop...
	for(;;){
    //procesamos tareas en background
    if(bIdle&&!::PeekMessage(pMsg,NULL,0,0,PM_NOREMOVE)){
      if(bFirstEmpty){
        // ShowWindow the first time the queue goes empty
        ::ShowWindow(m_hWnd,SW_SHOWNORMAL);
        bFirstEmpty=FALSE;
      }
      // call OnIdle while in bIdle state
      if(!(::GetWindowLongPtr(m_hWnd,GWL_STYLE)&DS_NOIDLEMSG)){
        if(hWndParent!=NULL&&::IsWindow(hWndParent))
          ::SendMessage(hWndParent,WM_ENTERIDLE,MSGF_DIALOGBOX,(LPARAM)m_hWnd);
      }
      //send udate idle
      pApp->OnIdle();
    }
    do{
      //get message... ( Updated from ReactOS... )
      if(::GetMessage(pMsg,NULL,0,0)<=0){
        //if msg was wm_quit resend...
        pApp->EndMsgLoop(pMsg->wParam);
        return -1;
      }
      //destroyed externaly?
      if(!::IsWindow(m_hWnd))
        return 0;
      //check exit message...
      if(!(m_nFlags&XWF_MODALLOOP))
        return m_nModRes;
      //despachamos mensajes
      if(!pApp->PreTranslateMessage(pMsg)){
        ::TranslateMessage(pMsg);
        ::DispatchMessage(pMsg);
      }
      /*
      //check end modal is sended internaly
      XASSERT(m_nFlags&XWF_MODALLOOP)
      //despachamos mensajes
      if(!pApp->DispatchMessages()){
        //if msg was wm_quit resend...
        pApp->EndMsgLoop(pMsg->wParam);
        return -1;
      }
      //check exit message...
      if(!(m_nFlags&XWF_MODALLOOP))
        return m_nModRes;*/
      //verificamos si el mensaje puede necesitar tareas en bg...
      bIdle=pApp->IsIdleMessage(pMsg);
		}while(::PeekMessage(pMsg,NULL,0,0,PM_NOREMOVE));
  }
	return -1;
}

VOID XWnd::EndModal(INT_PTR nResult){
  XASSERT(::IsWindow(m_hWnd))
	if(m_nFlags&XWF_MODALLOOP){
    //save result
    m_nModRes=nResult;
    //remove modal flag...
		m_nFlags&=~XWF_MODALLOOP;
    //force check flag...
		PostMessage(WM_NULL);
	}
}

BOOL XWnd::ModifyStyle(BOOL bEx,DWORD dwRemove,DWORD dwAdd,UINT nFlags){
  XASSERT(::IsWindow(m_hWnd))
  DWORD dwStyle=::GetWindowLong(m_hWnd,(bEx)?GWL_EXSTYLE:GWL_STYLE);
  DWORD dwNewStyle=(dwStyle&~dwRemove)|dwAdd;
  if(dwStyle==dwNewStyle)
    return FALSE;
  ::SetWindowLong(m_hWnd,(bEx)?GWL_EXSTYLE:GWL_STYLE,dwNewStyle);
  if(nFlags!=0)
    ::SetWindowPos(m_hWnd,NULL,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE|SWP_NOZORDER|SWP_NOACTIVATE|nFlags);
  return TRUE;
}

BOOL XWnd::CalcWindowRect(LPRECT lpClientRect,BOOL nAdjust,BOOL bMenu){
  XASSERT(::IsWindow(m_hWnd))
  DWORD dwStyle=::GetWindowLong(m_hWnd,GWL_STYLE);
  DWORD dwExStyle=::GetWindowLong(m_hWnd,GWL_EXSTYLE);
  if(!nAdjust)
    dwExStyle&=~WS_EX_CLIENTEDGE;
  return ::AdjustWindowRectEx(lpClientRect,dwStyle,bMenu,dwExStyle);
}

BOOL XWnd::ClientResize(INT nWidth,INT nHeight){
  XASSERT(::IsWindow(m_hWnd))
  RECT cr,wr;
  ::GetClientRect(m_hWnd,&cr);
  ::GetWindowRect(m_hWnd,&wr);
  return ::SetWindowPos(m_hWnd,NULL,0,0,nWidth+(wr.right-wr.left)-cr.right,nHeight+(wr.bottom-wr.top)-cr.bottom,
    SWP_NOMOVE|SWP_NOZORDER|SWP_NOACTIVATE);
}

BOOL XWnd::CenterWindow(HWND pOwner){
  XASSERT(::IsWindow(m_hWnd))
  HWND hWndCenter=NULL;
	DWORD dwStyle=::GetWindowLong(m_hWnd,GWL_STYLE);
	if(pOwner==NULL){
		if(dwStyle&WS_CHILD)
			hWndCenter=::GetParent(m_hWnd);
		else
			hWndCenter=::GetWindow(m_hWnd,GW_OWNER);
	}else
    hWndCenter=pOwner;
	XRect wr,rcArea,rcCenter;
	GetWindowRect(&wr);
	if(!(dwStyle&WS_CHILD)){
		if(hWndCenter!=NULL){
			DWORD dwParentStyle=::GetWindowLong(hWndCenter,GWL_STYLE);
			if(!(dwParentStyle&WS_VISIBLE)||(dwParentStyle&WS_MINIMIZE))
				hWndCenter=NULL;
		}
		#if (_WIN32_WINNT >= 0x0500 || _WIN32_WINDOWS >= 0x0410)
 		MONITORINFO mi;
		mi.cbSize=sizeof(mi);
		if(hWndCenter==NULL){
			GetMonitorInfo(MonitorFromWindow(GetTopLevelParent(),MONITOR_DEFAULTTOPRIMARY),&mi);
			rcCenter=mi.rcWork;
			rcArea=mi.rcWork;
		}else{
			::GetWindowRect(hWndCenter,&rcCenter);
			GetMonitorInfo(MonitorFromWindow(hWndCenter,MONITOR_DEFAULTTONEAREST),&mi);
			rcArea=mi.rcWork;
		}
		#else
		if(hWndCenter==NULL){
      ::SystemParametersInfo(SPI_GETWORKAREA,0,&rcArea,0);
      rcCenter=rcArea;
		}else{
		  ::GetWindowRect(hWndCenter,&rcCenter);
      ::SystemParametersInfo(SPI_GETWORKAREA,0,&rcArea,0);
		}
		#endif
	}else{
	  HWND hWndParent=::GetParent(m_hWnd);
		XASSERT(::IsWindow(hWndParent))
		::GetClientRect(hWndParent,&rcArea);
		XASSERT(::IsWindow(hWndCenter))
		::GetClientRect(hWndCenter,&rcCenter);
		::MapWindowPoints(hWndCenter,hWndParent,(POINT*)&rcCenter,2);
	}
	int xLeft=(rcCenter.left+rcCenter.right)/2-wr.Width()/2;
	int yTop=(rcCenter.top+rcCenter.bottom)/2-wr.Height()/2;
	if(xLeft+wr.Width()>rcArea.right)
		xLeft=rcArea.right-wr.Width();
	if(xLeft<rcArea.left)
		xLeft=rcArea.left;
	if(yTop+wr.Height()>rcArea.bottom)
		yTop=rcArea.bottom-wr.Height();
	if(yTop<rcArea.top)
		yTop=rcArea.top;
	return SetWindowPos(NULL,xLeft,yTop,-1,-1,SWP_NOSIZE|SWP_NOZORDER|SWP_NOACTIVATE);
}

BOOL XWnd::RepositionControls(UINT nIDFirst,UINT nIDLast,UINT nIDOver,
	LPRECT lpRectParam,LPCRECT lpRectClient,BOOL bOnlyQuery){

	HDWP hDWP=NULL;RECT rect;
	HWND hWndOver=NULL;
	UINT numChildren=0;

  for(HWND hWndChild=::GetTopWindow(m_hWnd);hWndChild!=NULL;hWndChild=::GetNextWindow(hWndChild,GW_HWNDNEXT))
    numChildren++;

  if(numChildren&&!bOnlyQuery)
    hDWP=::BeginDeferWindowPos(numChildren);

	if(lpRectClient==NULL)
		GetClientRect(&rect);
	else
		rect=*lpRectClient;

	for(HWND hWndChild=::GetTopWindow(m_hWnd);hWndChild!=NULL;hWndChild=::GetNextWindow(hWndChild,GW_HWNDNEXT)){
		UINT_PTR nIDC=(UINT)::GetDlgCtrlID(hWndChild);
		if(nIDC==nIDOver)
			hWndOver=hWndChild;
		else if(nIDC>=nIDFirst&&nIDC<=nIDLast)
			::SendMessage(hWndChild,WM_SIZEPARENT,(WPARAM)&rect,(LPARAM)hDWP);
	}

	if(bOnlyQuery){
		XASSERT(lpRectParam!=NULL);
		::CopyRect(lpRectParam,&rect);
		return TRUE;
	}

	if(nIDOver!=0&&hWndOver!=NULL){
		XWnd* pOver=XWnd::FromHandle(hWndOver);
		if(lpRectParam!=NULL){
			rect.left+=lpRectParam->left;
			rect.top+=lpRectParam->top;
			rect.right-=lpRectParam->right;
			rect.bottom-=lpRectParam->bottom;
		}
		pOver->CalcWindowRect(&rect);
		RepositionWindow(hDWP,hWndOver,&rect);
	}

	if(hDWP!=NULL&&::EndDeferWindowPos(hDWP))
    return TRUE;
  return FALSE;
}

VOID XWnd::RepositionWindow(HDWP hDWP,HWND hWnd,LPCRECT lpRect){
	XASSERT(::IsWindow(m_hWnd))
	XASSERT(lpRect!=NULL)
	HWND hWndParent=::GetParent(hWnd);
	XASSERT(hWndParent!=NULL)

	RECT rOld;
	::GetWindowRect(hWnd,&rOld);
	::MapWindowPoints(NULL,hWndParent,(LPPOINT)&rOld,2);
	if(::EqualRect(&rOld,lpRect))
		return;

	if(hDWP){
		hDWP=::DeferWindowPos(hDWP,hWnd,NULL,
			lpRect->left,lpRect->top,lpRect->right-lpRect->left,
			lpRect->bottom-lpRect->top,SWP_NOACTIVATE|SWP_NOZORDER);
	}else{
		::SetWindowPos(hWnd,NULL,lpRect->left,lpRect->top,
			lpRect->right-lpRect->left,lpRect->bottom-lpRect->top,
			SWP_NOACTIVATE|SWP_NOZORDER);
	}
}

HWND XWnd::GetDescendantWindow(INT nID,HWND hWnd){
  HWND hWndChild,hWndTmp;
  XASSERT(::IsWindow(m_hWnd))
  if(!hWnd)hWnd=m_hWnd;
	if((hWndChild=::GetDlgItem(hWnd,nID))!=NULL){
		if(::GetTopWindow(hWndChild)!=NULL){
			hWndTmp=GetDescendantWindow(nID,hWndChild);
			if(hWndTmp!=NULL)
				return hWndTmp;
		}
		return hWndChild;
	}
	for(hWndChild=::GetTopWindow(hWnd);hWndChild!=NULL;hWndChild=::GetNextWindow(hWndChild,GW_HWNDNEXT)){
		hWndChild=GetDescendantWindow(nID,hWndChild);
		if(hWndChild!=NULL)
			return hWndChild;
	}
	return NULL;
}

VOID XWnd::SendMessageToDescendants(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL bDeep,HWND hWnd){
	for(HWND hWndChild=::GetTopWindow(hWnd?hWnd:m_hWnd);hWndChild!=NULL;hWndChild=::GetNextWindow(hWndChild,GW_HWNDNEXT)){
		::SendMessage(hWndChild,uMsg,wParam,lParam);
		if(bDeep&&::GetTopWindow(hWndChild)!=NULL)
			SendMessageToDescendants(uMsg,wParam,lParam,bDeep,hWndChild);
	}
}

DWORD XWnd::GetDialogBaseUnitsEx(HFONT hFont){
  XASSERT(::IsWindow(m_hWnd))
  TEXTMETRIC tm;SIZE size;
  if(hFont){
    HDC hDC=::GetDC(m_hWnd);
    HFONT hOldFont=(HFONT)::SelectObject(hDC,hFont);
    if(!hOldFont){
      ::ReleaseDC(m_hWnd,hDC);
      return 0;
    }
    ::GetTextMetrics(hDC,&tm);
    LONG baseUnitY=tm.tmHeight;
    ::GetTextExtentPoint32(hDC,_T("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"),52,&size);
    LONG baseUnitX=(size.cx+26)/52;
    ::SelectObject(hDC,hOldFont);
    ::ReleaseDC(m_hWnd,hDC);
    return MAKEWPARAM(baseUnitX,baseUnitY);
  }
  return 0;
}

BOOL XWnd::MapDialogRectEx(HFONT hFont,LPRECT lpRect){
  DWORD dwDlgBaseUnit=GetDialogBaseUnitsEx(hFont);
  if(dwDlgBaseUnit){
    lpRect->left=MulDiv(lpRect->left,LOWORD(dwDlgBaseUnit),4);
    lpRect->right=MulDiv(lpRect->right,LOWORD(dwDlgBaseUnit),4);
    lpRect->top=MulDiv(lpRect->top,HIWORD(dwDlgBaseUnit),8);
    lpRect->bottom=MulDiv(lpRect->bottom,HIWORD(dwDlgBaseUnit),8);
    return TRUE;
  }
  return FALSE;
}

BOOL XWnd::DisableWindowTheme(){
  HMODULE uxtheme=GetModuleHandle(_T("uxtheme.dll"));
  if(uxtheme){
    typedef HRESULT (WINAPI *tSetWindowTheme)(HWND,LPCWSTR,LPCWSTR);
    tSetWindowTheme pSetWindowTheme=(tSetWindowTheme)GetProcAddress(uxtheme,"SetWindowTheme");
    if(pSetWindowTheme)
      return (pSetWindowTheme(m_hWnd,L"",L"")==S_OK);
  }
  //returns TRUE if can't find dll or function... (assumes that the window is not themed)
  return TRUE;
}

BOOL XWnd::PreTranslateInput(LPMSG lpMsg){
	XASSERT(::IsWindow(m_hWnd))
	if((lpMsg->message<WM_KEYFIRST||lpMsg->message>WM_KEYLAST)&&
  (lpMsg->message<WM_MOUSEFIRST||lpMsg->message>WM_MOUSELAST))
		return FALSE;
	return IsDialogMessage(lpMsg);
}

inline const MSG* XWnd::GetCurrentMessage(){
  return &m_cMsg;
}

inline LRESULT XWnd::Default(){
  return DefWindowProc(m_cMsg.message,m_cMsg.wParam,m_cMsg.lParam);
}

inline LRESULT XWnd::DefWindowProc(UINT uMsg,WPARAM wParam,LPARAM lParam){
  //not processed WM_INITDIALOG... just return TRUE
  if(uMsg==WM_INITDIALOG)
    return TRUE;
  //try to call original window proc
  if(m_WndProc)
    return ::CallWindowProc(m_WndProc,m_hWnd,uMsg,wParam,lParam);
  //ret default
  return ::DefWindowProc(m_hWnd,uMsg,wParam,lParam);
}

inline LRESULT XWnd::DefDlgProc(UINT uMsg,WPARAM wParam,LPARAM lParam){
  //ret default
  return ::DefDlgProc(m_hWnd,uMsg,wParam,lParam);
}

BOOL XWnd::OnOleEvent(UINT idCtrl,UINT nCode,XSINKEVENT* pEvent){
  HRESULT hResult=S_OK;BOOL bRet=FALSE;
  UINT uArgError=(UINT)-1;
  if(nCode==WM_OLEEVENT){
    const XEventSinkTableEntry *pEntry=GetEventSinkEntry(idCtrl,pEvent);
    if(pEntry==NULL)
      return FALSE;
    BOOL bRange=(pEntry->uMaxId!=(UINT)-1);
    switch(pEvent->m_eventKind){
      case XSINKEVENT::XS_EVENT:{
        VARIANT var;
        memset(&var,0,sizeof(var));
        if(bRange){
          DISPPARAMS dispparams;
          memcpy(&dispparams,pEvent->m_pDispParams,sizeof(DISPPARAMS));
          dispparams.rgvarg=new VARIANT[++dispparams.cArgs];
          memcpy(dispparams.rgvarg,pEvent->m_pDispParams->rgvarg,sizeof(VARIANT)*(dispparams.cArgs-1));
          VARIANT* pvarID=&dispparams.rgvarg[dispparams.cArgs-1];
          V_VT(pvarID)=VT_I4;
          V_I4(pvarID)=idCtrl;
          hResult=CallMemberFunc(pEntry,DISPATCH_METHOD,&var,&dispparams,&uArgError);
          delete []dispparams.rgvarg;
        }else
          hResult=CallMemberFunc(pEntry,DISPATCH_METHOD,&var,pEvent->m_pDispParams,&uArgError);
        XASSERT(FAILED(hResult)||(V_VT(&var)==VT_BOOL))
        bRet=V_BOOL(&var);
			}break;
			case XSINKEVENT::XS_PROPCHANGED:{
				if(bRange){
					tBUEvtFunc pfn=(tBUEvtFunc)pEntry->pfnSet;
					bRet=(this->*pfn)(idCtrl);
				}else{
					tBVEvtFunc pfn=(tBVEvtFunc)pEntry->pfnSet;
					bRet=(this->*pfn)();
				}
			}
			break;
      case XSINKEVENT::XS_PROPREQUEST:{
				BOOL bAllow=TRUE;
				if(bRange){
					tBUBEvtFunc pfn=(tBUBEvtFunc)pEntry->pfn;
					bRet=(this->*pfn)(idCtrl,&bAllow);
				}else{
					tBBEvtFunc pfn=(tBBEvtFunc)pEntry->pfn;
					bRet=(this->*pfn)(&bAllow);
				}
				hResult=bAllow?S_OK:S_FALSE;
			}
			break;
      default:
        XASSERT(FALSE)
      break;
		}
	}
	if(FAILED(hResult)&&pEvent->m_puArgError!=NULL&&uArgError!=(UINT)-1)
		*pEvent->m_puArgError=uArgError;
	pEvent->m_hResult=hResult;
	return bRet;
}

LRESULT XWnd::OnWindowlessMsg(UINT uMsg,WPARAM wParam,LPARAM lParam,PBOOL bProcessed){
  LRESULT hResult=0;
  switch(uMsg){
    case WM_MOUSEMOVE:case WM_LBUTTONDOWN:case WM_LBUTTONUP:case WM_LBUTTONDBLCLK:
    case WM_RBUTTONDOWN:case WM_RBUTTONUP:case WM_RBUTTONDBLCLK:case WM_MBUTTONDOWN:
    case WM_MBUTTONUP:case WM_MBUTTONDBLCLK:{
      XWnd *pMouseMessageSite=NULL;
      if(m_pSiteCapture!=NULL){
        XASSERT(m_pSiteCapture->m_pWindowlessObject!=NULL)
        pMouseMessageSite=m_pSiteCapture;
      }else{
        std::map<XWnd*,HWND>::const_iterator it;
        for(it=m_XWndSiteMap.Begin();it!=m_XWndSiteMap.End()&&pMouseMessageSite==NULL;it++){
          XWnd* pCtrlSite=(*it).first;
          XASSERT(pCtrlSite!=NULL&&pCtrlSite->m_pObject!=NULL)
          if(pCtrlSite&&pCtrlSite->m_bIsWindowless){
            XASSERT(pCtrlSite->m_pWindowlessObject!=NULL)
            POINT point;
            POINTSTOPOINT(point,*(POINTS*)&lParam);
            if(PtInRect(&pCtrlSite->m_rect,point)){
              pMouseMessageSite=pCtrlSite;
              break;
            }
          }
        }
      }
      if(pMouseMessageSite!=NULL){
        BOOL bRet=(pMouseMessageSite->m_pWindowlessObject->OnWindowMessage(uMsg,wParam,lParam,&hResult)==S_OK);
        if(bProcessed)*bProcessed=bRet;
      }
    }break;
    case WM_KEYDOWN:case WM_KEYUP:case WM_CHAR:case WM_DEADCHAR:case WM_SYSKEYDOWN:
    case WM_SYSKEYUP:case WM_SYSDEADCHAR:case WM_HELP:case WM_CANCELMODE:case WM_IME_STARTCOMPOSITION:
    case WM_IME_ENDCOMPOSITION:case WM_IME_COMPOSITION:case WM_IME_SETCONTEXT:case WM_IME_NOTIFY:
    case WM_IME_CONTROL:case WM_IME_COMPOSITIONFULL:case WM_IME_SELECT:case WM_IME_CHAR:
    case WM_IME_KEYDOWN:case WM_IME_KEYUP:
      if(m_pSiteFocus!=NULL&&m_pSiteFocus->m_pWindowlessObject!=NULL){
        BOOL bRet=(m_pSiteFocus->m_pWindowlessObject->OnWindowMessage(uMsg,wParam,lParam,&hResult)==S_OK);
        if(bProcessed)*bProcessed=bRet;
      }
    break;
  }
  return hResult;
}

LRESULT XWnd::OnCommand(WPARAM wParam,LPARAM lParam,PBOOL pProcessed){
  std::list< PXEventTableEntry >::iterator it;
  PXEventTableEntry pCallEntry=NULL;
  HWND hWndCtrl=(HWND)lParam;
  UINT uCode=HIWORD(wParam);
  UINT uId=LOWORD(wParam);
  BOOL bProcessed=FALSE;
  LRESULT lRet=0;
  //find current handler...
  for(it=m_commandscache.begin();it!=m_commandscache.end();it++){
    //EventList
    PXEventTableEntry pEntry=*it;
    //process entrys
    if(pEntry&&pEntry->pEventFunc){
      if((!pEntry->uCode||pEntry->uCode==uCode)&&//all command events or just the same
      ((pEntry->uMinId==(UINT)-1&&pEntry->uMaxId==(UINT)-1)||(uId>=pEntry->uMinId&&uId<=pEntry->uMaxId)))//a control range
        pCallEntry=pEntry;
    }
  }
  //we need reflect? (not a menu or accelerator)
  if(hWndCtrl!=NULL&&(!pCallEntry||pCallEntry->bForce!=TRUE)){
    //need to reflect?
    if(::IsWindow(hWndCtrl)&&hWndCtrl!=m_hWnd){
      //send to child window (if is an object)
      XWnd *pWnd=FromHandlePermanent(hWndCtrl);
      if(pWnd)//try reflect...
        lRet=pWnd->OnCommand(wParam,lParam,&bProcessed);
    }
  }
  //finaly, check if we need send to parent...
  if(pCallEntry&&(!bProcessed||pCallEntry->bForce!=FALSE)){
    lRet=CallEventProc(pCallEntry,wParam,lParam);
    bProcessed=TRUE;
  }
  //update state...
  if(pProcessed)*pProcessed=bProcessed;
  return lRet;
}

LRESULT XWnd::OnNotify(UINT nCtrlId,LPNMHDR pnmh,PBOOL pProcessed){
  std::list< PXEventTableEntry >::iterator it;
  PXEventTableEntry pCallEntry=NULL;
  HWND hWndCtrl=pnmh->hwndFrom;
  UINT uCode=pnmh->code;
  UINT uId=pnmh->idFrom;
  BOOL bProcessed=FALSE;
  LRESULT lRet=0;
  //find current handler...
  for(it=m_notifyscache.begin();it!=m_notifyscache.end();it++){
    //EventList
    PXEventTableEntry pEntry=*it;
    //process entrys
    if(pEntry&&pEntry->pEventFunc){
      if((!pEntry->uCode||pEntry->uCode==uCode)&&//all command events or just the same
      ((pEntry->uMinId==(UINT)-1&&pEntry->uMaxId==(UINT)-1)||(uId>=pEntry->uMinId&&uId<=pEntry->uMaxId)))//a control range
        pCallEntry=pEntry;
    }
  }
  //we need reflect? (not a menu or accelerator)
  if(hWndCtrl!=NULL&&(!pCallEntry||pCallEntry->bForce!=TRUE)){
    //need to reflect?
    if(::IsWindow(hWndCtrl)&&hWndCtrl!=m_hWnd){
      //send to child window (if is an object)
      XWnd *pWnd=FromHandlePermanent(hWndCtrl);
      if(pWnd)//try reflect...
        lRet=pWnd->OnNotify(nCtrlId,pnmh,&bProcessed);
    }
  }
  //finaly, check if we need send to parent...
  if(pCallEntry&&(!bProcessed||pCallEntry->bForce!=FALSE)){
    lRet=CallEventProc(pCallEntry,(WPARAM)nCtrlId,(LPARAM)pnmh);
    bProcessed=TRUE;
  }
  //update state...
  if(pProcessed)*pProcessed=bProcessed;
  return lRet;
}

LRESULT XWnd::OnWndMsg(UINT uMsg,WPARAM wParam,LPARAM lParam,PBOOL pProcessed){
  std::list< PXEventTableEntry >::iterator it;
  PXEventTableEntry pCallEntry=NULL;
  BOOL bProcessed=FALSE;
  LRESULT lRet=0;
  //Process Messages...
  switch(uMsg){
    case WM_COMMAND:
      lRet=OnCommand(wParam,lParam,&bProcessed);
    break;
    case WM_NOTIFY:
      lRet=OnNotify((UINT)wParam,(LPNMHDR)lParam,&bProcessed);
    break;
    default:
      //find current handler...
      for(it=m_eventscache.begin();it!=m_eventscache.end();it++){
        //EventList
        PXEventTableEntry pEntry=*it;
        //process entrys
        if(pEntry&&pEntry->uMsg==uMsg&&pEntry->pEventFunc){
          if((!pEntry->uCode||pEntry->uCode==wParam)&&//all codes or just the same
          ((pEntry->uMinId==(UINT)-1&&pEntry->uMaxId==(UINT)-1)||((UINT)lParam>=pEntry->uMinId&&(UINT)lParam<=pEntry->uMaxId))){//a range
            pCallEntry=pEntry;
            break;//only the first top event function...
          }
        }
      }
      //we need reflect?
      if(!pCallEntry||pCallEntry->bForce!=TRUE){
        switch(uMsg){
          case WM_CTLCOLORLISTBOX:case WM_CTLCOLORSCROLLBAR:
          case WM_CTLCOLOREDIT:case WM_CTLCOLORSTATIC:
          case WM_CTLCOLORBTN:case WM_CTLCOLORDLG:
          case WM_CHARTOITEM:case WM_VKEYTOITEM:
          case WM_HSCROLL:case WM_VSCROLL:
          case WM_CTLCOLORMSGBOX:{
            HWND hCtrl=(HWND)lParam;
            if(::IsWindow(hCtrl)&&hCtrl!=m_hWnd){
              XWnd *pWnd=(XWnd *)FromHandlePermanent(hCtrl);
              if(pWnd)//send as an owner msg
                lRet=pWnd->OnWndMsg(uMsg,wParam,lParam,&bProcessed);
            }
          }
          break;
          case WM_PARENTNOTIFY:
            if((LOWORD(wParam)==WM_CREATE||LOWORD(wParam)==WM_DESTROY)){
              HWND hCtrl=(HWND)lParam;
              if(::IsWindow(hCtrl)&&hCtrl!=m_hWnd){
                XWnd *pWnd=(XWnd *)FromHandlePermanent(hCtrl);
                if(pWnd)//send as an owner msg
                  lRet=pWnd->OnWndMsg(uMsg,wParam,lParam,&bProcessed);
              }
            }
          break;
          case WM_MEASUREITEM:{
            LPMEASUREITEMSTRUCT lpMIS=(LPMEASUREITEMSTRUCT)lParam;
            if(lpMIS){
              if(lpMIS->CtlType==ODT_MENU){
                  XMenu *pMenu=XMenu::FindPopupMenuFromID(lpMIS->itemID,lpMIS->itemData);
                  if(pMenu){
                    //send as an owner msg
                    lRet=pMenu->OnMeasureItem(m_hWnd,lpMIS);
                    bProcessed=TRUE;
                  }
              }else{
                HWND hWnd=GetDescendantWindow(lpMIS->CtlID);
                if(::IsWindow(hWnd)&&hWnd!=m_hWnd){
                  XWnd *pWnd=(XWnd *)FromHandlePermanent(hWnd);
                  if(pWnd)//send as an owner msg
                    lRet=pWnd->OnWndMsg(uMsg,wParam,lParam,&bProcessed);
                }
              }
            }
          }
          break;
          case WM_DRAWITEM:{
            LPDRAWITEMSTRUCT lpDIS=(LPDRAWITEMSTRUCT)lParam;
            if(lpDIS){
              if(lpDIS->CtlType==ODT_MENU){
                if(::IsMenu((HMENU)lpDIS->hwndItem)){
                  XMenu *pMenu=(XMenu *)XMenu::FromHandlePermanent((HMENU)lpDIS->hwndItem);
                  if(pMenu){
                    //send as an owner msg
                    lRet=pMenu->OnDrawItem(m_hWnd,lpDIS);
                    bProcessed=TRUE;
                  }
                }
              }else
              if(::IsWindow(lpDIS->hwndItem)&&lpDIS->hwndItem!=m_hWnd){
                XWnd *pWnd=(XWnd *)FromHandlePermanent(lpDIS->hwndItem);
                if(pWnd)//send as an owner msg
                  lRet=pWnd->OnWndMsg(uMsg,wParam,lParam,&bProcessed);
              }
            }
          }
          break;
          case WM_COMPAREITEM:{
            LPCOMPAREITEMSTRUCT lpCIS=(LPCOMPAREITEMSTRUCT)lParam;
            if(lpCIS&&::IsWindow(lpCIS->hwndItem)&&lpCIS->hwndItem!=m_hWnd){
              XWnd *pWnd=(XWnd *)FromHandlePermanent(lpCIS->hwndItem);
              if(pWnd)//send as an owner msg
                lRet=pWnd->OnWndMsg(uMsg,wParam,lParam,&bProcessed);
            }
          }
          break;
          case WM_DELETEITEM:{
            PDELETEITEMSTRUCT pDIS=(PDELETEITEMSTRUCT)lParam;
            if(pDIS&&::IsWindow(pDIS->hwndItem)&&pDIS->hwndItem!=m_hWnd){
              XWnd *pWnd=(XWnd *)FromHandlePermanent(pDIS->hwndItem);
              if(pWnd)//send as an owner msg
                lRet=pWnd->OnWndMsg(uMsg,wParam,lParam,&bProcessed);
            }
          }
          break;
          case WM_MENUCHAR:{
            XMenu *pMenu=(XMenu *)XMenu::FromHandlePermanent((HMENU)lParam);
            if(pMenu){
              //send as an owner msg
              lRet=pMenu->OnMenuChar(m_hWnd,LOWORD(wParam),HIWORD(wParam));
              bProcessed=TRUE;
            }
          }
          case WM_SYSCOLORCHANGE:case WM_PALETTECHANGED:
          case WM_DEVMODECHANGE:case WM_SETTINGCHANGE:
          case WM_DISPLAYCHANGE:
            // forward this message to all other child windows
            if(!(GetStyle()&WS_CHILD))
              SendMessageToDescendants(uMsg,wParam,lParam,TRUE);
          break;
        }
        //is a windowless msg?
        if(m_nWindowlessControls>0){
          if(((uMsg>=WM_MOUSEFIRST)&&(uMsg<=WM_MOUSELAST))||
          ((uMsg>=WM_KEYFIRST)&&(uMsg<=WM_IME_KEYLAST))||
          ((uMsg>=WM_IME_SETCONTEXT)&&(uMsg<=WM_IME_KEYUP)))
            lRet=OnWindowlessMsg(uMsg,wParam,lParam,&bProcessed);
        }
      }
      //finaly, check if we need send to parent...
      if(pCallEntry&&(!bProcessed||pCallEntry->bForce!=FALSE)){
        lRet=CallEventProc(pCallEntry,wParam,lParam);
        bProcessed=TRUE;
      }
      //update state...
      if(pProcessed)*pProcessed=bProcessed;
    break;
  }
  //update state...
  if(pProcessed)*pProcessed=bProcessed;
  return lRet;
}

LRESULT XWnd::WindowProc(UINT uMsg,WPARAM wParam,LPARAM lParam){
  //only last called return value is used
  LRESULT lRet=0;BOOL bProcessed=FALSE;
  //save MSG
  MSG oldMsg=m_cMsg;
	m_cMsg.hwnd=m_hWnd;
	m_cMsg.message=uMsg;
	m_cMsg.wParam=wParam;
	m_cMsg.lParam=lParam;
  //create cache...
  if(MakeEventsCache())
    lRet=OnWndMsg(uMsg,wParam,lParam,&bProcessed);
  //restore msg
  m_cMsg=oldMsg;
  //check if we processed the message
  if(!bProcessed)//else return default...
    lRet=DefWindowProc(uMsg,wParam,lParam);
  // special case for WM_DESTROY
  if((uMsg==WM_DESTROY)&&(m_pSiteUIActive!=NULL))
		OnUIActivate(NULL);
  //make sure of detach us!
  if(uMsg==WM_NCDESTROY)
    Detach();//detach...
  //finally return...
  return lRet;
}

LRESULT WINAPI XWnd::CallWndProc(XWnd* pWnd, UINT uMsg,WPARAM wParam,LPARAM lParam){
  XASSERT(pWnd!=NULL)
  if(pWnd)
    return pWnd->WindowProc(uMsg,wParam,lParam);
  return 0;
}

LRESULT CALLBACK XWnd::XWndProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam){
  if(::IsWindow(hWnd)){
    XWnd *pWnd=FromHandlePermanent(hWnd);
    if(pWnd)
      return XWnd::CallWndProc(pWnd,uMsg,wParam,lParam);
  }
  return ::DefWindowProc(hWnd,uMsg,wParam,lParam);
}

BOOL CALLBACK XWnd::XDlgProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam){
	if(::IsWindow(hWnd)&&uMsg==WM_INITDIALOG){
	  XWnd *pWnd=FromHandlePermanent(hWnd);
    if(pWnd)
      return XWnd::CallWndProc(pWnd,uMsg,wParam,lParam);
		return TRUE;
	}
	return FALSE;
}

LRESULT CALLBACK XWnd::CbtFilterHook(INT nCode,WPARAM wParam,LPARAM lParam){
  if(nCode!=HCBT_CREATEWND)
		return CallNextHookEx(m_hCbtFilter,nCode,wParam,lParam);
  if(m_pWndInit!=NULL&&wParam!=0&&m_pWndInit->Attach((HWND)wParam)){
    //reset static var
    m_pWndInit=NULL;
    //release hook
    LRESULT lResult=CallNextHookEx(m_hCbtFilter,nCode,wParam,lParam);
    BOOL bUnhooked=::UnhookWindowsHookEx(m_hCbtFilter);
    XASSERT(bUnhooked)
    XUNUSED(bUnhooked)
    //reset static var
    m_hCbtFilter=NULL;
    return lResult;
  }
  return CallNextHookEx(m_hCbtFilter,nCode,wParam,lParam);
}

BOOL XWnd::HookCreation(){
  XASSERT(m_pWndInit==NULL&&m_hCbtFilter==NULL)
  if(m_pWndInit==NULL){
    if(m_hCbtFilter==NULL)
      m_hCbtFilter=::SetWindowsHookEx(WH_CBT,CbtFilterHook,NULL,::GetCurrentThreadId());
    XASSERT(m_hCbtFilter!=NULL)
    if(m_hCbtFilter!=NULL){
      m_pWndInit=this;
      return TRUE;
    }
  }
  return FALSE;
}

BOOL XWnd::UnhookCreation(){
  BOOL bRet=TRUE;
  if(m_hCbtFilter!=NULL){
    BOOL bUnhooked=::UnhookWindowsHookEx(m_hCbtFilter);
    XASSERT(bUnhooked)
    XUNUSED(bUnhooked)
    m_hCbtFilter=NULL;
    bRet=FALSE;
  }
  if(m_pWndInit!=NULL){
		m_pWndInit=NULL;
		bRet=FALSE;
	}
	return bRet;
}


// XWnd Ole Support

DWORD XWnd::QueryInterface(const VOID *iid,LPVOID* ppvObj){
  return InternalQueryInterface(iid,ppvObj);
}

const XWnd::AmbientsFlagsStruct XWnd::AmbientsFlags[8]={
  { DISPID_AMBIENT_USERMODE,          QACONTAINER_USERMODE },
  { DISPID_AMBIENT_UIDEAD,            QACONTAINER_UIDEAD },
  { DISPID_AMBIENT_SHOWGRABHANDLES,   QACONTAINER_SHOWGRABHANDLES },
  { DISPID_AMBIENT_SHOWHATCHING,      QACONTAINER_SHOWHATCHING },
  { DISPID_AMBIENT_DISPLAYASDEFAULT,  QACONTAINER_DISPLAYASDEFAULT },
  { DISPID_AMBIENT_AUTOCLIP,          QACONTAINER_AUTOCLIP },
  { DISPID_AMBIENT_MESSAGEREFLECT,    QACONTAINER_MESSAGEREFLECT },
  { DISPID_AMBIENT_SUPPORTSMNEMONICS, QACONTAINER_SUPPORTSMNEMONICS }
};

BOOL XWnd::SetOleFont(HFONT hFont){
	if(hFont==NULL){
    hFont=(HFONT)GetStockObject(DEFAULT_GUI_FONT);
    if(hFont==NULL)
      hFont=(HFONT)GetStockObject(SYSTEM_FONT);
    if(hFont==NULL)
      return FALSE;
	}

	LOGFONT logfont;
	GetObject(hFont,sizeof(LOGFONT),&logfont);

	FONTDESC fd;
	fd.cbSizeofstruct=sizeof(FONTDESC);
	fd.lpstrName=TSTR2WSTR(logfont.lfFaceName);
	fd.sWeight=(short)logfont.lfWeight;
	fd.sCharset=logfont.lfCharSet;
	fd.fItalic=logfont.lfItalic;
	fd.fUnderline=logfont.lfUnderline;
	fd.fStrikethrough=logfont.lfStrikeOut;

	long lfHeight=logfont.lfHeight;
	if(lfHeight<0)
		lfHeight=-lfHeight;

  HDC hDC=GetWindowDC();
  INT ppi=GetDeviceCaps(hDC,LOGPIXELSY);
  fd.cySize.Lo=lfHeight*720000/ppi;
	fd.cySize.Hi=0;
  ReleaseDC(hDC);
  if(CtrlAppearance.pFont)
    CtrlAppearance.pFont->Release();

  if(FAILED(::OleCreateFontIndirect(&fd,IID_IFontDisp,(void**)&CtrlAppearance.pFont))){
		CtrlAppearance.pFont=NULL;
		return FALSE;
  }
  return TRUE;
}

BOOL XWnd::GetAmbientProperty(DISPID dispid,VARIANT* pvar){
  //Default Ambient Properties
	switch(dispid){
    case DISPID_AMBIENT_AUTOCLIP:
    case DISPID_AMBIENT_MESSAGEREFLECT:
    case DISPID_AMBIENT_SUPPORTSMNEMONICS:
    case DISPID_AMBIENT_USERMODE:
      V_VT(pvar)=VT_BOOL;
      V_BOOL(pvar)=VARIANT_TRUE;
    return TRUE;
    case DISPID_AMBIENT_SHOWGRABHANDLES:
    case DISPID_AMBIENT_SHOWHATCHING:
    case DISPID_AMBIENT_DISPLAYASDEFAULT:
    case DISPID_AMBIENT_UIDEAD:
      V_VT(pvar)=VT_BOOL;
      V_BOOL(pvar)=VARIANT_FALSE;
    return TRUE;
    case DISPID_AMBIENT_APPEARANCE:
      V_VT(pvar)=VT_I2;
      V_I2(pvar)=0;
    return TRUE;
    case DISPID_AMBIENT_PALETTE:
      V_VT(pvar)=VT_I4;
      V_I4(pvar)=(LONG_PTR)GetStockObject(DEFAULT_PALETTE);
    return TRUE;
    case DISPID_AMBIENT_BACKCOLOR:
    case DISPID_AMBIENT_FORECOLOR:
      if(CtrlAppearance.colorBack==(COLORREF)-1){
        HDC hDC=GetWindowDC();
        SendMessage(WM_CTLCOLORSTATIC,(WPARAM)hDC,(LPARAM)m_hWnd);
        CtrlAppearance.colorBack=GetBkColor(hDC);
        CtrlAppearance.colorFore=GetTextColor(hDC);
        ReleaseDC(hDC);
      }
      V_VT(pvar)=VT_COLOR;
      V_I4(pvar)=(dispid==DISPID_AMBIENT_BACKCOLOR)?CtrlAppearance.colorBack:CtrlAppearance.colorFore;
    return TRUE;
    case DISPID_AMBIENT_FONT:
      if(CtrlAppearance.pFont==NULL)
        SetOleFont(GetFont());
      XASSERT(CtrlAppearance.pFont!=NULL)
      if(CtrlAppearance.pFont==NULL)
        return FALSE;
      V_VT(pvar)=VT_FONT;
      CtrlAppearance.pFont->AddRef();
      V_DISPATCH(pvar)=CtrlAppearance.pFont;
    return TRUE;
    case DISPID_AMBIENT_LOCALEID:
      if(CtrlAppearance.lcid==0)
        CtrlAppearance.lcid=GetThreadLocale();
      V_VT(pvar)=VT_I4;
      V_I4(pvar)=CtrlAppearance.lcid;
    return TRUE;
    case DISPID_AMBIENT_DISPLAYNAME:
      V_VT(pvar)=VT_BSTR;
      V_BSTR(pvar)=SysAllocString(L"");
    return TRUE;
    case DISPID_AMBIENT_SCALEUNITS:
      V_VT(pvar)=VT_BSTR;
      V_BSTR(pvar)=SysAllocString(L"pixels");
    return TRUE;
	}
	return FALSE;
}

VOID XWnd::OnUIActivate(XWnd* pSite){
	if(m_pSiteUIActive!=NULL&&m_pSiteUIActive!=pSite)
		m_pSiteUIActive->m_pInPlaceObject->UIDeactivate();
	m_pSiteUIActive=pSite;
}

VOID XWnd::OnUIDeactivate(XWnd* pSite){
	if(m_pSiteUIActive==pSite)
		m_pSiteUIActive=NULL;
	if(m_pSiteFocus==pSite)
		m_pSiteFocus=NULL;
}


// XWnd::XOleIPFrame

STDMETHODIMP XWnd::XOleIPFrame::QueryInterface(REFIID iid, LPVOID* ppvObj){
	return (HRESULT)GetParentPtr()->InternalQueryInterface(&iid, ppvObj);
}

STDMETHODIMP_(ULONG) XWnd::XOleIPFrame::AddRef(){
	return (ULONG)GetParentPtr()->InternalAddRef();
}

STDMETHODIMP_(ULONG) XWnd::XOleIPFrame::Release(){
	return (ULONG)GetParentPtr()->InternalRelease();
}

STDMETHODIMP XWnd::XOleIPFrame::GetWindow(HWND* phWnd){
	XWnd *pThis=GetParentPtr();
	*phWnd=pThis->m_hWnd;
	return S_OK;
}

STDMETHODIMP XWnd::XOleIPFrame::ContextSensitiveHelp(BOOL){
	return E_NOTIMPL;
}

STDMETHODIMP XWnd::XOleIPFrame::GetBorder(LPRECT){
	return E_NOTIMPL;
}

STDMETHODIMP XWnd::XOleIPFrame::RequestBorderSpace(LPCBORDERWIDTHS){
	return E_NOTIMPL;
}

STDMETHODIMP XWnd::XOleIPFrame::SetBorderSpace(LPCBORDERWIDTHS){
	return E_NOTIMPL;
}

STDMETHODIMP XWnd::XOleIPFrame::SetActiveObject(LPOLEINPLACEACTIVEOBJECT pActiveObject, LPCOLESTR){
  XWnd *pThis=GetParentPtr();
	if(pThis->m_pSiteUIActive!=NULL){
		LPOLEINPLACEACTIVEOBJECT pOldActiveObject=pThis->m_pSiteUIActive->m_pActiveObject;
		if(pActiveObject!=NULL)
			pActiveObject->AddRef();
		pThis->m_pSiteUIActive->m_pActiveObject=pActiveObject;
		if(pOldActiveObject!=NULL)
			pOldActiveObject->Release();
	}
	return S_OK;
}

STDMETHODIMP XWnd::XOleIPFrame::InsertMenus(HMENU,LPOLEMENUGROUPWIDTHS){
	return E_NOTIMPL;
}

STDMETHODIMP XWnd::XOleIPFrame::SetMenu(HMENU, HOLEMENU, HWND){
	return E_NOTIMPL;
}

STDMETHODIMP XWnd::XOleIPFrame::RemoveMenus(HMENU){
	return E_NOTIMPL;
}

STDMETHODIMP XWnd::XOleIPFrame::SetStatusText(LPCOLESTR){
	return E_NOTIMPL;
}

STDMETHODIMP XWnd::XOleIPFrame::EnableModeless(BOOL){
	return S_OK;
}

STDMETHODIMP XWnd::XOleIPFrame::TranslateAccelerator(LPMSG,WORD){
	return E_NOTIMPL;
}


// XWnd::XOleContainer

STDMETHODIMP XWnd::XOleContainer::QueryInterface(REFIID iid, LPVOID* ppvObj){
	return (HRESULT)GetParentPtr()->InternalQueryInterface(&iid, ppvObj);
}

STDMETHODIMP_(ULONG) XWnd::XOleContainer::Release(){
	return (ULONG)GetParentPtr()->InternalRelease();
}

STDMETHODIMP_(ULONG) XWnd::XOleContainer::AddRef(){
	return (ULONG)GetParentPtr()->InternalAddRef();
}

STDMETHODIMP XWnd::XOleContainer::ParseDisplayName(LPBINDCTX,LPOLESTR, ULONG*, LPMONIKER*){
	return E_NOTIMPL;
}

STDMETHODIMP XWnd::XOleContainer::EnumObjects(DWORD dwFlags,LPENUMUNKNOWN* ppEnumUnknown){
  XWnd *pThis=GetParentPtr();
	HRESULT hr=E_OUTOFMEMORY;
	if(dwFlags&OLECONTF_EMBEDDINGS){
    LPUNKNOWN *ppUnk=new LPUNKNOWN[pThis->m_XWndSiteMap.Size()];
    if(ppUnk){
      UINT_PTR cObjects=0;
      std::map<XWnd*,HWND>::const_iterator it;
      for(it=pThis->m_XWndSiteMap.Begin();it!=pThis->m_XWndSiteMap.End();it++){
        XWnd* pCtrlSite=(*it).first;
        XASSERT(pCtrlSite!=NULL&&pCtrlSite->m_pObject!=NULL)
        pCtrlSite->m_pObject->AddRef();
        ppUnk[cObjects++]=pCtrlSite->m_pObject;
      }
      XEnumUnknown* pEnum=new XEnumUnknown(ppUnk,cObjects);
      XASSERT(pEnum!=NULL)
      *ppEnumUnknown=(IEnumUnknown*)&pEnum->m_Enum;
      hr=S_OK;
    }
  }
	return hr;
}

STDMETHODIMP XWnd::XOleContainer::LockContainer(BOOL){
	return E_NOTIMPL;
}
