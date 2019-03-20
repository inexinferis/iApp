#include "xmdichildwnd.h"

BEGIN_EVENT_TABLE(XMDIChildWnd,XFrameWnd)
END_EVENT_TABLE()

IMPLEMENT_DYNCREATE(XMDIChildWnd,XFrameWnd)

BOOL XMDIChildWnd::PreCreateWindow(LPCREATESTRUCT wcs){
	XASSERT(wcs->style&WS_CHILD);
	if(wcs->lpszClass==NULL)
		wcs->lpszClass=XDEFMDICHILDWNDCLASSNAME;
	return XFrameWnd::PreCreateWindow(wcs);
}

BOOL XMDIChildWnd::PreRegisterClass(LPWNDCLASSEX wcx){
  if(wcx->lpszClassName==NULL)
		wcx->lpszClassName=XDEFMDICHILDWNDCLASSNAME;
  return XFrameWnd::PreRegisterClass(wcx);
}

BOOL XMDIChildWnd::PreTranslateMessage(LPMSG lpMsg){
  if(XFrameWnd::PreTranslateMessage(lpMsg))
    return TRUE;
  if(m_hAccelTable&&lpMsg->message>=WM_KEYFIRST&&lpMsg->message<=WM_KEYLAST)
    return ::TranslateAccelerator(MDIGetFrame(),m_hAccelTable,lpMsg);
	return FALSE;
}

BOOL XMDIChildWnd::CreateClient(LPCREATESTRUCT lpcs){
  return XFrameWnd::CreateClient(lpcs);
}

VOID XMDIChildWnd::RecalcLayout(){
  XFrameWnd::RecalcLayout();
}

HWND XMDIChildWnd::Create(HWND hParentWnd,LPCTSTR lpszWindowName,LPCTSTR lpszClassName,
  DWORD dwStyle,const RECT& rect,LPCTSTR lpszMenu,DWORD dwExStyle,LPVOID lpParam){
  HWND hWndMDIClient=NULL;
  XASSERT(hParentWnd!=NULL)
  if(hParentWnd==NULL)
    return NULL;//We Need to have a parent

  //find mdi client!
  HWND hWndChild;TCHAR szClassName[256];
  for(hWndChild=::GetTopWindow(hParentWnd);hWndChild!=NULL;hWndChild=::GetNextWindow(hWndChild,GW_HWNDNEXT)){
		if(::GetClassName(hWndChild,szClassName,sizeof(szClassName))&&!lstrcmpi(szClassName,_T("mdiclient"))){
		  hWndMDIClient=hWndChild;
		  break;
		}
	}
	if(!hWndMDIClient)
    return NULL;

	//now copy into a MDICREATESTRUCT for real create
	MDICREATESTRUCT mcs;
	mcs.szClass=lpszClassName;
	mcs.szTitle=lpszWindowName;
	mcs.hOwner=XGetInstanceHandle();
	mcs.x=rect.left;
	mcs.y=rect.top;
	mcs.cx=rect.right;
	mcs.cy=rect.bottom;
	mcs.style=dwStyle&~(WS_MAXIMIZE|WS_VISIBLE);
	mcs.lParam=(LPARAM)lpParam;

  //create MID window
	HWND hWnd=XFrameWnd::Create(lpszWindowName,lpszClassName,dwStyle,rect,hWndMDIClient,lpszMenu,dwExStyle|WS_EX_MDICHILD,(LPVOID)&mcs);

	if(hWnd&&m_wcs.style&WS_VISIBLE){
		::BringWindowToTop(hWnd);
		if(m_wcs.style&WS_MINIMIZE)
			::ShowWindow(hWnd,SW_SHOWMINIMIZED);
		else if(m_wcs.style&WS_MAXIMIZE)
			::ShowWindow(hWnd,SW_SHOWMAXIMIZED);
		else
			::ShowWindow(hWnd,SW_SHOWNORMAL);
    ::SendMessage(hWndMDIClient,WM_MDIACTIVATE,(WPARAM)hWnd,0);
		::SendMessage(hWndMDIClient,WM_MDIREFRESHMENU,0,0);
		m_hParentWnd=hParentWnd;
	}

	return hWnd;
}

HWND XMDIChildWnd::LoadFrame(HWND hParentWnd,UINT uResID,DWORD dwStyle,LPVOID lpParam){
  XASSERT(hParentWnd!=NULL)
  XASSERT(uResID!=0&&uResID<0x8000)
  XASSERT(!(dwStyle&WS_POPUP));
  if(hParentWnd==NULL)
    return NULL;//We Need to have a parent

  TCHAR szBuffer[1024],*lpszWindowName=NULL;
  if(::LoadString(XGetInstanceHandle(),uResID,szBuffer,sizeof(szBuffer)/sizeof(TCHAR)))
    lpszWindowName=szBuffer;

  return Create(hParentWnd,lpszWindowName,XDEFMDICHILDWNDCLASSNAME,dwStyle|WS_CHILD,XDEFAULTRECT,
	  MAKEINTRESOURCE(uResID),XDEFAULTFRAMEWNDSTYLEEX,lpParam);
}

LRESULT XMDIChildWnd::DefWindowProc(UINT nMsg,WPARAM wParam,LPARAM lParam){
  return ::DefMDIChildProc(m_hWnd,nMsg,wParam,lParam);
}

BOOL XMDIChildWnd::DestroyWindow(){
	if(m_hWnd==NULL)
		return FALSE;
	MDIDestroy();
	return TRUE;
}
