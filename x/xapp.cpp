#include "xapp.h"
#include "xevent.h"
#include "xwnd.h"

#define XINITCOMMONCTRLS

/********************************************************\
  CORE MAIN FUNCTIONS
\********************************************************/

IMPLEMENT_DYNAMIC(XWinApp,XEvent)

//Definición de Eventos a Interceptar...
BEGIN_EVENT_TABLE(XWinApp,XEvent)
END_EVENT_TABLE()

//App Global Var...
XWinApp *XWinApp::XApp=NULL;
BOOL XWinApp::bOleInitialized=FALSE;

//Object's Map's
XObjectMap<HWND,XWnd*> XWinApp::m_XWndMap;
XObjectMap<HDC,XDC*> XWinApp::m_XDCMap;
XObjectMap<HGDIOBJ,XGDIObject*> XWinApp::m_XGDIMap;
XObjectMap<HMENU,XMenu*> XWinApp::m_XMenuMap;
XObjectMap<HIMAGELIST,XImageList*> XWinApp::m_XILMap;
XObjectMap<HINTERNET,XInternet*> XWinApp::m_XIntMap;

//WinApp Methods...
INT XWinApp::Run(HINSTANCE hInst,LPTSTR pcArgs,INT nShow){
  INT userret=0,loopret=0;
  //init app things...
  m_hInstance=hInst;m_args=pcArgs;m_showMode=nShow;
#ifdef XINITCOMMONCTRLS
  #if(_WIN32_WINDOWS >= 0x0500)
    INITCOMMONCONTROLSEX icex={sizeof(INITCOMMONCONTROLSEX),ICC_ALLCLASSES};
    InitCommonControlsEx(&icex);
  #else
    InitCommonControls();
  #endif
#endif
  //reset errors (if any...)
  SetLastError(0);
  //call user methods...
  if(((userret=OnInit(hInst,pcArgs,nShow))==TRUE)&&m_XWndMap.Size()){
    loopret=MsgLoop();
    userret=OnExit(loopret);
  }
  return userret;
}

VOID XWinApp::OnIdle(){
  //Process Idle Message...
  std::map < HWND , PXWND >::const_iterator it;
  //Obtenemos lista de ventanas clientes...
  for(it=m_XWndMap.Begin();it!=m_XWndMap.End();it++){
    PXWND pWnd=(*it).second;
    XASSERT(pWnd!=NULL)
    if(pWnd&&pWnd->IsValid()&&pWnd->IsVisible())
      XWnd::CallWndProc(pWnd,WM_IDLEUPDATECMDUI,TRUE);
  }
  //send msg to app...
  AppProc(WM_IDLEUPDATECMDUI,TRUE);
}

BOOL XWinApp::IsIdleMessage(PMSG pMsg){
  // Retorna Falso si el mensaje despachado no provoca un cambio
  // en la interfaz o un cambio interno de la tabla de eventos, esto
  // es más que nada para no sobrecargar el proceso OnIdle
	// redundantes WM_MOUSEMOVE and WM_NCMOUSEMOVE
	if(pMsg->message==WM_MOUSEMOVE||pMsg->message==WM_NCMOUSEMOVE){
    if(pMsg->message==m_lMsg.message)
		  if(pMsg->pt.x==m_lMsg.pt.x&&pMsg->pt.y==m_lMsg.pt.y)
        return TRUE;
    // recordar para la próxima vez...
    m_lMsg.message=pMsg->message;
    m_lMsg.pt.x=pMsg->pt.x;
    m_lMsg.pt.y=pMsg->pt.y;
		return FALSE;
	}
	// WM_PAINT & WM_SYSTIMER debe ser procesado
	return (pMsg->message!=WM_PAINT&&pMsg->message!=0x0118);
}

INT XWinApp::ExitInstance(){
  // GetMessage return ERROR_INVALID_WINDOW_HANDLE
  // when application exit... why? i don't know...
  if(GetLastError()==ERROR_INVALID_WINDOW_HANDLE)
    SetLastError(0);
	return m_Msg.wParam;
}

BOOL XWinApp::PreTranslateMessage(PMSG pMsg){
  //it's a valid window?
	if(pMsg->hwnd==NULL){
    if(AppProc(pMsg->message,pMsg->wParam,pMsg->lParam))
      return TRUE;
    //event not catched...
    return FALSE;
  }
  //save handle and go for parents
  for(HWND hwnd=pMsg->hwnd;hwnd;hwnd=GetParent(hwnd)){
    XWnd *pWnd=XWnd::FromHandlePermanent(hwnd);
    if(pWnd&&pWnd->PreTranslateMessage(pMsg))
      return TRUE;
  }
  //no pre translate messages...
  return FALSE;
}

PMSG XWinApp::GetCurrentMessage(){
  //only return actual msg...
  return &m_Msg;
}

BOOL XWinApp::DispatchMessages(){
  if(::GetMessage(&m_Msg,NULL,0,0)<=0)
		return FALSE;
  // process this message
	if(!PreTranslateMessage(&m_Msg)){
    ::TranslateMessage(&m_Msg);
    ::DispatchMessage(&m_Msg);
	}
  return TRUE;
}

INT XWinApp::MsgLoop(){
  //tareas para realizar en background...
  BOOL bIdle=TRUE;
	for(;;){
    //procesamos tareas en background
    if(bIdle&&!::PeekMessage(&m_Msg,NULL,0,0,PM_NOREMOVE))
      OnIdle();
    do{
      //despachamos mensajes
      if(!DispatchMessages())
        return ExitInstance();
      //verificamos si el mensaje puede necesitar tareas en bg...
      bIdle=IsIdleMessage(&m_Msg);
		}while(::PeekMessage(&m_Msg,NULL,0,0,PM_NOREMOVE));
	}
}

VOID XWinApp::EndMsgLoop(DWORD dwResult){
  //clean ole
  OleTerm();
  //extra process cleanup
  ::PostQuitMessage(dwResult);
}

BOOL XWinApp::AppProc(UINT uMsg,WPARAM wParam,LPARAM lParam){
  std::list< PXEventTableEntry >::iterator it;
  //create cache...
  if(MakeEventsCache(TRUE)){
    //Process Messages...
    for(it=m_eventscache.begin();it!=m_eventscache.end();it++){
      //EventList
      PXEventTableEntry pEntry=*it;
      //process entrys
      if(pEntry&&pEntry->uMsg==uMsg&&pEntry->pEventFunc){
        if((!pEntry->uCode||pEntry->uCode==wParam)&&//all codes or just the same
        ((pEntry->uMinId==(UINT)-1&&pEntry->uMaxId==(UINT)-1)||((UINT)lParam>=pEntry->uMinId&&(UINT)lParam<=pEntry->uMaxId))){//a range
          CallEventProc(pEntry,wParam,lParam);
          return TRUE;//only the first top event function...
        }
      }
    }
  }
  //we don't process the message
  return FALSE;
}

BOOL XWinApp::OleInit(){
  if(bOleInitialized)
    return TRUE;
  bOleInitialized=SUCCEEDED(::OleInitialize(NULL));
  return bOleInitialized;
}

BOOL XWinApp::OleTerm(){
  if(!bOleInitialized)
    return FALSE;
  ::CoFreeUnusedLibraries();
  ::OleUninitialize();
  bOleInitialized=FALSE;
  return TRUE;
}

/********************************************************\
  CORE MAIN FUNCTIONS
\********************************************************/

IMPLEMENT_DYNAMIC(XConsoleApp,XObject)

XConsoleApp *XConsoleApp::XApp=NULL;

INT XConsoleApp::Run(INT argc, PCHAR argv[]){
  m_argc=argc;m_argv=argv;
  m_hInstance=GetModuleHandle(NULL);
  return Main(argc,argv);
}
