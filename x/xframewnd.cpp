#include "xframewnd.h"

//Definición de Eventos a Interceptar...
BEGIN_EVENT_TABLE(XFrameWnd,XWnd)
  EVENT_CREATE(XFrameWnd::OnCreate)
  EVENT_SIZE(XFrameWnd::OnSize)
END_EVENT_TABLE()

IMPLEMENT_DYNCREATE(XFrameWnd,XWnd)

HWND XFrameWnd::Create(LPCTSTR lpszWindowName,LPCTSTR lpszClassName,DWORD dwStyle,
  const RECT& rect,HWND hParentWnd,LPCTSTR lpszMenu,DWORD dwExStyle,LPVOID lpParam){
  HMENU hMenu=NULL;
	if(lpszMenu!=NULL&&(hMenu=::LoadMenu(XGetInstanceHandle(),lpszMenu))==NULL)
    return NULL;
  return XWnd::CreateEx(dwExStyle,lpszClassName,lpszWindowName,dwStyle,rect.left,rect.top,rect.right,rect.bottom,hParentWnd,hMenu,lpParam);
}

HWND XFrameWnd::LoadFrame(UINT uResID,DWORD dwStyle,HWND hParentWnd,LPVOID lpParam){
  XASSERT(uResID!=0&&uResID<0x8000)
	if(Create(MAKEINTRESOURCE(uResID),XDEFAULTFRAMEWNDCLASSNAME,dwStyle,XDEFAULTRECT,
	  hParentWnd,MAKEINTRESOURCE(uResID),XDEFAULTFRAMEWNDSTYLEEX,lpParam))
	  LoadAccelTable(XGetInstanceHandle(),MAKEINTRESOURCE(uResID));
  return m_hWnd;
}

BOOL XFrameWnd::PreCreateWindow(LPCREATESTRUCT wcs){
  wcs->dwExStyle|=WS_EX_CLIENTEDGE;
  return XWnd::PreCreateWindow(wcs);
}

BOOL XFrameWnd::PreRegisterClass(LPWNDCLASSEX wcx){
  if(wcx->lpszClassName==NULL)
		wcx->lpszClassName=XDEFAULTFRAMEWNDCLASSNAME;
  return XWnd::PreRegisterClass(wcx);
}

BOOL XFrameWnd::PreTranslateMessage(LPMSG lpMsg){
  XASSERT(::IsWindow(m_hWnd))
  //if(PreTranslateInput(lpMsg))//fail with menues!
    //return TRUE;
  if(m_hAccelTable!=NULL&&lpMsg->message>=WM_KEYFIRST&&lpMsg->message<=WM_KEYLAST)
		return TranslateAccelerator(m_hAccelTable,lpMsg);
	return FALSE;
}

LRESULT XFrameWnd::OnCommand(WPARAM wParam,LPARAM lParam){
	return XWnd::OnCommand(wParam,lParam);
}

LRESULT XFrameWnd::DefWindowProc(UINT nMsg, WPARAM wParam, LPARAM lParam){
  return XWnd::DefWindowProc(nMsg,wParam,lParam);
}

BOOL XFrameWnd::CreateClient(LPCREATESTRUCT /*lpcs*/){
  return TRUE;
}

VOID XFrameWnd::RecalcLayout(){
  RepositionControls(0,0xffff,XIDM_MDI_FIRSTCHILD);
  return;
}

BOOL XFrameWnd::LoadAccelTable(HINSTANCE hInstance,LPCTSTR lpszResourceName){
  XASSERT(m_hAccelTable==NULL&&lpszResourceName!=NULL)
  m_hAccelTable=::LoadAccelerators(hInstance,lpszResourceName);
  return (m_hAccelTable!=NULL);
}

VOID XFrameWnd::ActivateFrame(INT nCmdShow){
	if(nCmdShow==-1){
		if(!::IsWindowVisible(m_hWnd))
			nCmdShow=SW_SHOWNORMAL;
		else if(::IsIconic(m_hWnd))
			nCmdShow=SW_RESTORE;
	}
	if(nCmdShow!=-1)
		::ShowWindow(m_hWnd,nCmdShow);
  if(nCmdShow!=SW_HIDE&&nCmdShow!=SW_MINIMIZE&&nCmdShow!=SW_SHOWMINNOACTIVE&&
		nCmdShow!=SW_SHOWNA&&nCmdShow!=SW_SHOWNOACTIVATE)
		::BringWindowToTop(::GetLastActivePopup(m_hWnd));
}

VOID XFrameWnd::OnSize(UINT nType,SIZES /*sSize*/){
	if(nType!=SIZE_MINIMIZED)
		RecalcLayout();
}

INT XFrameWnd::OnCreate(LPCREATESTRUCT lpcs){
	if(!CreateClient(lpcs))
		return -1;
	return 0;
}

/*xgStatusBar* xgFrame::xgCreateStatusBar(xgUInt ctrlid,long style){
  #ifdef XG_DEBUG
    xgObject::xgTrace("xgFrame","CreateStatusBar");
  #endif
  if(!StatusBar){
    StatusBar=new xgStatusBar();
    if(StatusBar&&StatusBar->xgCreate(this,ctrlid,style|xgDefSBStyle))
      return StatusBar;
    else
      if(StatusBar)
        delete StatusBar;
  }
  return xgNull;
}

xgToolBar* xgFrame::xgCreateToolBar(xgUInt ctrlid,long style){
  #ifdef XG_DEBUG
    xgObject::xgTrace("xgFrame","CreateToolBar");
  #endif
  if(!ToolBar){
    ToolBar = new xgToolBar();
    if(ToolBar&&ToolBar->xgCreate(this,ctrlid,style|xgDefTBStyle))
      return ToolBar;
    else
      if(ToolBar)
        delete ToolBar;
  }
  return xgNull;
}

xgMenu* xgFrame::xgSetMenu(xgMenu *nmenu){
  #ifdef XG_DEBUG
    xgObject::xgTrace("xgFrame","xgSetMenu");
  #endif
  if(nmenu->xgSetMenu(this))
    Menu=nmenu;
  return Menu;
}

xgCoolBar* xgFrame::xgCreateCoolBar(xgUInt ctrlid,long style){
  #ifdef XG_DEBUG
    xgObject::xgTrace("xgFrame","CreateCoolBar");
  #endif
  if(!CoolBar){
    CoolBar = new xgCoolBar();
    if(CoolBar&&CoolBar->xgCreate(this,ctrlid,style|xgDefTBStyle))
      return CoolBar;
    else
      if(CoolBar)
        delete CoolBar;
  }
  return xgNull;
}

xgMenuBar* xgFrame::xgCreateMenuBar(xgUInt ctrlid,long style){
  #ifdef XG_DEBUG
    xgObject::xgTrace("xgFrame","CreateMenuBar");
  #endif
  if(!MenuBar){
    MenuBar = new xgMenuBar();
    if(MenuBar&&MenuBar->xgCreate(this,ctrlid,style|xgDefTBStyle))
      return MenuBar;
    else
      if(MenuBar)
        delete MenuBar;
  }
  return xgNull;
}*/
