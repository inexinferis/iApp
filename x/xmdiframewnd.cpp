#include "xmdiframewnd.h"
#include "xmdiclient.h"

//Definición de Eventos a Interceptar...
BEGIN_EVENT_TABLE(XMDIFrameWnd,XFrameWnd)
END_EVENT_TABLE()

IMPLEMENT_DYNCREATE(XMDIFrameWnd,XFrameWnd)

BOOL XMDIFrameWnd::PreCreateWindow(LPCREATESTRUCT wcs){
	return XFrameWnd::PreCreateWindow(wcs);
}

BOOL XMDIFrameWnd::PreRegisterClass(LPWNDCLASSEX wcx){
  return XFrameWnd::PreRegisterClass(wcx);
}

BOOL XMDIFrameWnd::PreTranslateMessage(LPMSG lpMsg){
  if(XFrameWnd::PreTranslateMessage(lpMsg))
    return TRUE;
  if(lpMsg->message>=WM_KEYFIRST&&lpMsg->message<=WM_KEYLAST&&
    (lpMsg->message==WM_KEYDOWN||lpMsg->message==WM_SYSKEYDOWN))
		return ::TranslateMDISysAccel(m_hWndMDIClient,lpMsg);
  return FALSE;
}

HWND XMDIFrameWnd::LoadFrame(UINT uResID,DWORD dwStyle,HWND hParentWnd,LPVOID lpParam){
	if(!XFrameWnd::LoadFrame(uResID,dwStyle,hParentWnd,lpParam))
		return NULL;
	m_hDefaultMenu=::GetMenu(m_hWnd);
	return m_hWnd;
}

BOOL XMDIFrameWnd::CreateClient(LPCREATESTRUCT lpcs){
  HMENU hWindowMenu=NULL;
	if(m_hDefaultMenu==NULL){
		hWindowMenu=::GetMenu(m_hWnd);
		//XASSERT(hWindowMenu!=NULL)
		if(hWindowMenu){
      int iMenu=::GetMenuItemCount(hWindowMenu)-2;
      XASSERT(iMenu>=0)
      hWindowMenu=::GetSubMenu(hWindowMenu,iMenu);
      XASSERT(hWindowMenu!=NULL)
		}
	}

	XASSERT(::IsWindow(m_hWnd))
  XASSERT(m_hWndMDIClient==NULL)

  DWORD dwStyle=XDEFMDICLIENTSTYLE;
  DWORD dwExStyle=XDEFMDICLIENTSTYLEEX;

  CLIENTCREATESTRUCT ccs;
  ccs.hWindowMenu=hWindowMenu;
	ccs.idFirstChild=XIDM_MDI_FIRSTCHILD+1;

	if(lpcs->style&(WS_HSCROLL|WS_VSCROLL)){
		dwStyle|=(lpcs->style&(WS_HSCROLL|WS_VSCROLL));
		ModifyStyle(WS_HSCROLL|WS_VSCROLL,0,SWP_NOREDRAW|SWP_FRAMECHANGED);
	}

  m_hWndMDIClient=::CreateWindowEx(dwExStyle,_T("mdiclient"),NULL,dwStyle,0,0,0,0,m_hWnd,
    (HMENU)XIDM_MDI_FIRSTCHILD,XGetInstanceHandle(),(LPVOID)&ccs);

	if(m_hWndMDIClient){
	  ::BringWindowToTop(m_hWndMDIClient);
	  RecalcLayout();
    return TRUE;
	}
	return FALSE;
}

VOID XMDIFrameWnd::RecalcLayout(){
  XFrameWnd::RecalcLayout();
}

HMENU XMDIFrameWnd::GetWindowMenuPopup(HMENU hMenuBar){
	if(hMenuBar==NULL)
		return NULL;
	XASSERT(::IsMenu(hMenuBar))
	INT iMenu=::GetMenuItemCount(hMenuBar)-2;
	if(iMenu>=0)
    return ::GetSubMenu(hMenuBar,iMenu);
	return NULL;
}

LRESULT XMDIFrameWnd::OnCommand(WPARAM wParam,LPARAM lParam){
  XWnd *pChild=FromHandlePermanent(MDIGetActive());
  if(pChild)//send as an owner msg
    return XWnd::CallWndProc(pChild,WM_COMMAND,wParam,lParam);
	if(!XFrameWnd::OnCommand(wParam,lParam))
		return 0;
	if(!lParam&&(LOWORD(wParam)&0xF000)==0xF000)
		return DefWindowProc(WM_COMMAND,wParam,lParam);
	return (LRESULT)-1;
}

LRESULT XMDIFrameWnd::DefWindowProc(UINT nMsg,WPARAM wParam,LPARAM lParam){
  if(m_hWndMDIClient)
    return ::DefFrameProc(m_hWnd,m_hWndMDIClient,nMsg,wParam,lParam);
  return XFrameWnd::DefWindowProc(nMsg,wParam,lParam);
}

/*XMDIChildWnd* CMDIFrameWnd::CreateNewChild(XRuntimeClass* pClass,UINT nResources,HMENU hMenu,HACCEL hAccel)
{
	XASSERT(pClass != NULL)
	XMDIChildWnd* pFrame = (CMDIChildWnd*) pClass->CreateObject();
	//XASSERT_KINDOF(CMDIChildWnd, pFrame);

	// load the frame
	XCreateContext context;
	context.m_pCurrentFrame = this;

	pFrame->SetHandles(hMenu, hAccel);
	if (!pFrame->LoadFrame(nResources,
			WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, NULL, &context))
	{
		XTRACE(traceAppMsg, 0, "Couldn't load frame window.\n");
		return NULL;
	}

	string strFullString, strTitle;
	if (strFullString.LoadString(nResources))
		ExtractSubString(strTitle, strFullString, CDocTemplate::docName);

	// redraw the frame and parent
	pFrame->SetTitle(strTitle);
	pFrame->InitialUpdateFrame(NULL, TRUE);

	return pFrame;
}*/
