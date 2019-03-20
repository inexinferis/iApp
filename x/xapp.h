#ifndef _XAPP_H_
#define _XAPP_H_

#include "xutils.h"
#include "xevent.h"

class XWnd;
class XMenu;
class XDC;
class XGDIObject;
class XImageList;
class XInternet;

#ifdef _MSC_VER
#pragma comment(lib, "comctl32.lib")
#endif

class XWinApp:public XEvent{
  DECLARE_DYNAMIC(XWinApp)
  DECLARE_NO_COPY(XWinApp)
  public:
    XWinApp():m_hInstance(NULL),m_args(NULL),m_showMode(0){
      //only one instance...
      XASSERT(XApp==NULL)
    }
    virtual ~XWinApp(){}

    virtual BOOL OnInit(HINSTANCE hInst,LPTSTR pcArgs,INT nShow)=0;
    virtual INT OnExit(INT loopret)=0;

    virtual INT Run(HINSTANCE hInst,LPTSTR pcArgs,INT nShow);

    HINSTANCE GetInstance(){
      return m_hInstance;
    }

    LPTSTR GetCmdArg(){
      return m_args;
    }

    INT GetShowMode(){
      return m_showMode;
    }

    static BOOL OleInit();
    static BOOL OleTerm();

  protected:

    virtual VOID OnIdle();
    virtual INT  MsgLoop();
    virtual INT  ExitInstance();
    virtual BOOL DispatchMessages();
    virtual PMSG GetCurrentMessage();
    virtual BOOL IsIdleMessage(LPMSG pMsg);
    virtual BOOL PreTranslateMessage(LPMSG pMsg);
    virtual BOOL AppProc(UINT uMsg,WPARAM wParam=0,LPARAM lParam=0);
    virtual VOID EndMsgLoop(DWORD dwResult=0);

  private:
    //Msg
    MSG m_Msg,m_lMsg;

    //to have access...
    friend class XWnd;
    friend class XMenu;
    friend class XDC;
    friend class XGDIObject;
    friend class XImageList;
    friend class XInternet;
    //for XInternet Map
    friend class XConnection;
    friend class XFTPConnection;
    friend class XHTTPConnection;
    friend class XGopherConnection;
    friend class XInternetFile;
    friend class XFTPFile;
    friend class XHTTPFile;
    friend class XGopherFile;

    //objects maps...
    static XObjectMap<HWND,XWnd*> m_XWndMap;
    static XObjectMap<HMENU,XMenu*> m_XMenuMap;
    static XObjectMap<HDC,XDC*> m_XDCMap;
    static XObjectMap<HGDIOBJ,XGDIObject*> m_XGDIMap;
    static XObjectMap<HIMAGELIST,XImageList*> m_XILMap;
    static XObjectMap<HINTERNET,XInternet*> m_XIntMap;

    static BOOL bOleInitialized;

  protected:
    HINSTANCE m_hInstance;
    LPTSTR m_args;
    INT m_showMode;

  public:
    static XWinApp *XApp;
  DECLARE_EVENT_TABLE()
};

class XConsoleApp:public XObject{
  DECLARE_DYNAMIC(XConsoleApp)
  DECLARE_NO_COPY(XConsoleApp)
  public:
    XConsoleApp():m_hInstance(NULL),m_argv(NULL),m_argc(0){
      //only one instance...
      XASSERT(XApp==NULL)
    }
    virtual ~XConsoleApp(){}

    virtual INT Run(INT argc, PCHAR argv[]);
    virtual INT Main(INT argc, PCHAR argv[])=0;

    HINSTANCE GetInstance(){
      return m_hInstance;
    }

    PCHAR *GetArgV(){
      return m_argv;
    }

    INT GetArgC(){
      return m_argc;
    }

  private:
    HINSTANCE m_hInstance;
    PCHAR *m_argv;INT m_argc;
  public:
    static XConsoleApp *XApp;
};

typedef class XConsoleApp XCONSOLEAPP,*PXCONSOLEAPP;

//Proc Principal...
#define IMPLEMENT_APP(AppName)                                                            \
extern "C" INT WINAPI WinMain (HINSTANCE hInst,HINSTANCE ,LPSTR lpszArg,INT nShow){       \
  XWinApp::XApp=new AppName();                                                            \
  INT ret=XWinApp::XApp->Run(hInst,lpszArg,nShow);                                        \
  delete XWinApp::XApp;                                                                   \
  return ret;                                                                             \
}

//Proc Principal...
#define IMPLEMENT_CONSOLEAPP(AppName)                                                     \
extern "C" INT main(INT argc,PCHAR argv[]){                                               \
  XConsoleApp::XApp=new AppName();                                                        \
  INT ret=XConsoleApp::XApp->Run(argc,argv);                                              \
  delete XConsoleApp::XApp;                                                               \
  return ret;                                                                             \
}

#define XGetApp() XWinApp::XApp
#define XGetInstanceHandle() XGetApp()->GetInstance()

//#define XGetInstanceHandle() GetModuleHandle(NULL)

#endif // _XAPP_H_

