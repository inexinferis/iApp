#ifndef MAIN_H
#define MAIN_H

#include "x/x.h"
/*#include "x/xgdi.h"
#include "x/xcoords.h"
#include "x/xwnd.h"
#include "x/xapp.h"
#include "x/xframewnd.h"
#include "x/xobject.h"*/
#include "x/xskinnededit.h"
#include "x/xskinnedstatic.h"
#include "x/xskinnedbutton.h"
#include "x/xskinnedframe.h"
#include "x/xskinneddlg.h"
#include "x/xskinnedmenu.h"
#include "x/xskinnedlistview.h"
#include "x/xskinnedtreeview.h"
#include "x/xskinnedprogress.h"
#include "x/xskinnedcombobox.h"
#include "x/xskinnedstatusbar.h"
#include "x/xshellnotifyicon.h"
#include "connection.h"
#include "register.h"
#include "iapprsrc.h"
#include "utils.h"
#include "patchguarddisabler.h"

#define IDM_IAPPLOGOUT        200
#define IDM_IAPPUPDATE        201
#define IDM_IAPPSETTINGS      202
#define IDM_IAPPEXIT          203

#define IDM_TOOLPATCHGUARD    205

#define IDM_HELPCONTENTS      210
#define IDM_HELPLICENCES      211
#define IDM_HELPABOUT         212

#define IDB_START       0x200
#define IDB_EXTRA       0x201
#define IDP_PROGRESS    0x202
#define IDP_UPDPROGRESS 0x203

#define IDS_LINK 0x500

#define ID_USERNAME  (UINT)0x1000
#define ID_PASSWORD  (UINT)0x1001

#define IDT_CHECKSTATUS 0x666
#define IDT_HACKERSTATUS 0x777

#define MINWINSIZEX     800
#define MINWINSIZEY     600
#define LISTWINWIDTH    240
#define REDITWINHEIGHT  140
#define STATUSBARHEIGHT 24

#define WM_UPDATESTATE    WM_USER +0xDDDD //internal update...
#define EVENT_UPDATESTATE(pfun)\
	EVENT_MESSAGE_EX(WM_UPDATESTATE,TVV,(VOID(XEvent::*)(VOID))(&pfun))

#define SYS_ERROR_BUFFER_SIZE   512

#define ENTRY_TAG       "[iApp]"

#define MAX_PROGRAMS 32


typedef BOOL (XWnd::*LPXWND_INTERNET_STATUS_CALLBACK)(HINTERNET hInternet, DWORD_PTR dwContext,DWORD dwInternetStatus, LPVOID lpvStatusInformation,DWORD dwStatusInformationLength);
typedef DWORD(XWnd::*LPXWND_THREAD_ROUTINE)(LPVOID);

extern const OLECHAR sBrowserStyle[];
extern const TCHAR *g_StrMap[];

enum{
  IAppMainStr=0, //0
  IAppMainFrame, //1
  IAppInstaller, //2
  IAppHelp,      //3
  IAppReg,       //4
  IAppHide,      //5
};

class IApp;

class ConnectingDlg: public XSkinnedDlg {
  public:
    ConnectingDlg(IApp *app):pApp(app){}
    ~ConnectingDlg(){}
    //eventos...
    INT OnCreate(LPCREATESTRUCT pcs);
    VOID OnCancel(WPARAM wParam,LPARAM lParam);
    VOID OnDestroy();

    virtual BOOL OnStatusCallback(HINTERNET hInternet,DWORD_PTR dwContext,DWORD dwInternetStatus,
      LPVOID lpvStatusInformation,DWORD dwStatusInformationLength);

  private:
    IApp *pApp;
    XSkinnedStatic sStatus;
    XSkinnedButton bCancel;

  DECLARE_EVENT_TABLE()
};

class UpdateDlg: public XSkinnedDlg {
  public:
    UpdateDlg(IApp *app):pApp(app){}
    ~UpdateDlg(){}
    //eventos...
    INT OnCreate(LPCREATESTRUCT pcs);
    VOID OnCancel(WPARAM wParam,LPARAM lParam);
    VOID OnDestroy();

    virtual BOOL OnStatusCallback(HINTERNET hInternet,DWORD_PTR dwContext,DWORD dwInternetStatus,
      LPVOID lpvStatusInformation,DWORD dwStatusInformationLength);

  private:
    IApp *pApp;
    XSkinnedStatic sStatus;
    XSkinnedButton bCancel;
    XSkinnedProgress pProgress;

  DECLARE_EVENT_TABLE()
};

class LoginDlg: public XSkinnedDlg {
  public:
    LoginDlg(IApp *app):pApp(app),bLoggedIn(FALSE){}
    ~LoginDlg(){}
    //eventos...
    INT OnCreate(LPCREATESTRUCT pcs);
    VOID OnClose();
    VOID OnDestroy();

    VOID OnEditChange(WPARAM dwParam,LPARAM lParam);
    VOID OnLogin(WPARAM wParam,LPARAM lParam);
		VOID OnCancel(WPARAM wParam,LPARAM lParam);

    DWORD LoginThread(LPVOID lpParameter);

  private:
    IApp *pApp;
    XSkinnedEdit uName,uPass;
    XSkinnedStatic sWelcome,sName,sPass;
    XSkinnedButton bLogin,bCancel,bRemember;
    XThread logThread;
    BOOL bLoggedIn;

    friend class ConnectingDlg;

  DECLARE_EVENT_TABLE()
};

class MyHtmlCtrl: public XHtmlCtrl{
  protected:
    virtual HRESULT OnShowContextMenu(DWORD, LPPOINT, LPUNKNOWN, LPDISPATCH){
      return S_OK;
    }

    #ifndef _DEBUG
    virtual VOID OnBeforeNavigate2(BSTR URL,DWORD /*nFlags*/,BSTR /*TargetFrameName*/,VARIANT* /*PostData*/,BSTR /*Headers*/,BOOL* pbCancel){
      if(wcscmp(URL,L"about:blank")!=0){
        ShellExecuteW(NULL,L"open",URL,0,0,SW_SHOWNORMAL);
        *pbCancel=TRUE;
      }
    }
    #else
    virtual VOID OnBeforeNavigate2(BSTR /*URL*/,DWORD /*nFlags*/,BSTR /*TargetFrameName*/,VARIANT* /*PostData*/,BSTR /*Headers*/,BOOL* /*pbCancel*/){
    }
    #endif

    virtual HRESULT OnGetHostInfo(DOCHOSTUIINFO* pInfo){
      OLECHAR* pCSSBuffer=(OLECHAR*)CoTaskMemAlloc((wcslen(sBrowserStyle)+1)*sizeof(OLECHAR));
      wcscpy(pCSSBuffer,sBrowserStyle);
      pInfo->cbSize=sizeof(DOCHOSTUIINFO);
      pInfo->dwFlags=DOCHOSTUIFLAG_NO3DBORDER|DOCHOSTUIFLAG_DISABLE_HELP_MENU;
      pInfo->dwDoubleClick=DOCHOSTUIDBLCLK_DEFAULT;
      pInfo->pchHostCss=pCSSBuffer;
      pInfo->pchHostNS=NULL;
      return S_OK;
    }
};

class MainFrame;
class TabPage: public XSkinnedDlg{
  public:
    TabPage():pid(0),item(0),timeleft(0),tottime(0),hItem(NULL),pMainFrame(NULL),bNeedWarn(TRUE),bDownloading(FALSE),hCheatModule(NULL){
    }
    virtual ~TabPage(){}

    INT OnCreate(LPCREATESTRUCT pcs);
    VOID OnSize(UINT nType,POINTS dwSize);
    VOID OnStart(WPARAM wParam,LPARAM lParam);
    VOID OnExtra(WPARAM wParam,LPARAM lParam);

    DWORD DownloadThread(LPVOID lpParameter);
    DWORD ActivateThread(LPVOID lpParameter);

    DWORD GetState();
    VOID UpdateState();

    static BOOL IsDownloading();
    static BOOL IsACheatLoaded();
    static BOOL UnloadCheats();

    BOOL OnStatusCallback(HINTERNET hInternet,DWORD_PTR dwContext,DWORD dwInternetStatus,
      LPVOID lpvStatusInformation,DWORD dwStatusInformationLength);

  private:

    BOOL Uninstall();
    BOOL CheckFiles();
    BOOL CheckMD5(LPCSTR sMD5,LPCSTR sFileName);

    BOOL CheatIsLoaded();
    BOOL InstallCheat();
    VOID UninstallCheat();
    BOOL LoadCheat();
    BOOL UnloadCheat();

    static VOID CloseConnections();
    static TabPage *pPages;

  private:
    IApp *pApp;
    XSkinnedStatic m_CheatIcon,m_CheatBmp,m_CheatStatus,m_CheatDesc;
    XSkinnedButton m_GroupTopLeft,m_GroupTopRight;
    XSkinnedButton m_Start,m_Extra;
    XSkinnedProgress m_Progress;
    MyHtmlCtrl m_Html;
    XImageList *pIconLst,*pBmpLst;
    UINT pid,item,timeleft,tottime;
    HTREEITEM hItem;
    MainFrame *pMainFrame;
    BOOL bNeedWarn,bDownloading;
    XThread downThread,actThread;
    Connection con;
    UINT clTime;

    HMODULE hCheatModule;

    friend class MainFrame;

  DECLARE_EVENT_TABLE()
};

class PatchguardDlg: public XSkinnedDlg {
  public:
    PatchguardDlg(IApp *app);
    ~PatchguardDlg(){}
    //eventos...
    INT OnCreate(LPCREATESTRUCT pcs);
    VOID OnAdvancedOptionsClicked(WPARAM dwParams,LPARAM lParam);
    VOID OnPatchClicked(WPARAM wParam,LPARAM lParam);
    VOID OnRestoreClicked(WPARAM wParam,LPARAM lParam);
    VOID OnOk(WPARAM wParam,LPARAM lParam);
    VOID OnClose();

  private:
    IApp *pApp;
    MainFrame *pMainFrame;
    XSkinnedButton bClose,bPatch,bRestore,bGroup;
    XSkinnedButton bAdvancedOptions,bDefaultEntryPoint;
    XSkinnedEdit uLoaderName,uKernelName;
    XSkinnedStatic sStatus,sLoaderName,sKernelName;

  DECLARE_EVENT_TABLE()
};

class AboutDlg: public XSkinnedDlg {
  public:
    AboutDlg(IApp *app):pApp(app),sLink(NULL,XCOLOR_TEXT_ACTIVE,XCOLOR_TEXT_INACTIVE){}
    ~AboutDlg(){}
    //eventos...
    INT OnCreate(LPCREATESTRUCT pcs);
    VOID OnLinkClicked(WPARAM dwParams,LPARAM lParam);
    VOID OnOk(WPARAM wParam,LPARAM lParam);
    VOID OnClose();

  private:
    IApp *pApp;
    XSkinnedButton bClose;
    XSkinnedStatic sAbout,sHwid;
    XStaticLink sLink;

  DECLARE_EVENT_TABLE()
};

class SettingsDlg: public XSkinnedDlg {
  public:
    SettingsDlg(IApp *app);
    ~SettingsDlg(){}
    //eventos...
    INT OnCreate(LPCREATESTRUCT pcs);
    VOID OnSave(WPARAM wParam,LPARAM lParam);
    VOID OnCancel(WPARAM wParam,LPARAM lParam);
    VOID OnClose();

  private:
    IApp *pApp;
    MainFrame *pMainFrame;
    XSkinnedStatic sLang;
    XSkinnedButton bGroup;
    XSkinnedButton bOk,bCancel,bAutoStart,bMinStart,bForceLogout;
    XSkinnedComboBox cLang;

  DECLARE_EVENT_TABLE()
};

class MyRichEdit:public XRichEdit{
  public:
    MyRichEdit():bFirst(TRUE){}

    VOID PutString(LPCTSTR form,...){
      XASSERT(::IsWindow(m_hWnd))
      static TCHAR tmp[2048];
      va_list p;
      va_start(p,form);
      wvsprintf(tmp,form,p);
      SetSel(-1,-1);
      if(bFirst)
        bFirst=FALSE;
      else
        ReplaceSel(_T("\r\n"));
      ReplaceSel(tmp);
      ScrollCaret();
      SendMessage(WM_VSCROLL,SB_BOTTOM);
      va_end(p);
    }

    VOID ShowError(){
      LPVOID lpMsgBuf;
      FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM,NULL,GetLastError(),0,(LPTSTR)&lpMsgBuf,0,NULL);
      PutString(LoadStringStatic(NULL,STR_CHEATERROR),lpMsgBuf);
      LocalFree(lpMsgBuf);
      SetLastError(0);
    }

  private:
    BOOL bFirst;
};

class MainFrame: public XSkinnedFrame {
  public:
    MainFrame(IApp *app):bForceExit(FALSE),pApp(app),Settings(app),Patchguard(app),About(app){
      TabPage::pPages=Pages;NumPages=ActivePage=0;
      pActiveTab=NULL;
    }
    virtual ~MainFrame(){}
    //eventos...
    INT OnCreate(LPCREATESTRUCT pcs);
    VOID OnClose();
    VOID OnDestroy();
    VOID OnSize(UINT nType,POINTS dwSize);
    VOID OnGetMaxMinInfo(LPMINMAXINFO pmmi);
    VOID OnExit(WPARAM wParam,LPARAM lParam);
    VOID OnCheckUpdate(WPARAM wParam,LPARAM lParam);
    VOID OnSettings(WPARAM wParam,LPARAM lParam);
    VOID OnLogout(WPARAM wParam,LPARAM lParam);
    VOID OnAbout(WPARAM wParam,LPARAM lParam);
    VOID OnPatchguard(WPARAM wParam,LPARAM lParam);
    VOID OnHelpContents(WPARAM wParam,LPARAM lParam);
    VOID OnUpdateStatusTimer(TIMERPROC lpTimerFunc);

    DWORD LogoutThread(LPVOID lpParameter);
    DWORD CUpdateThread(LPVOID lpParameter);
    DWORD UpdateStatusThread(LPVOID lpParameter);

		VOID OnUpdateState(VOID);
    VOID OnSystrayNotify(UINT uId,UINT uMouseMsg);
    VOID OnTVSelChanged(TabPage *pTab);

    VOID LogEvent(LPCTSTR frmt,...);
    VOID LogEventEx(HINSTANCE hInstance,LPCTSTR nRsrcID,...);
    VOID UpdateState(LPCTSTR state);

    BOOL bForceExit;
    TabPage *pActiveTab;

  private:
    IApp *pApp;
    XSkinnedMenu Menu;
    XSkinnedTreeView Tree;
    XSkinnedStatusBar StatusBar;
    XImageList IconLst,BmpLst;
    XThread logoutThread,cupdateThread,getstatusThread;
    XShellNotifyIcon SysTray;

    MyRichEdit REdit;
    TabPage Pages[MAX_PROGRAMS];
    UINT NumPages,ActivePage;
    SettingsDlg Settings;
    PatchguardDlg Patchguard;
    AboutDlg About;

  DECLARE_EVENT_TABLE()
};

class IApp: public XWinApp {
  public:
    IApp():bFirst(TRUE),bConnected(FALSE),bRestart(FALSE),
    bCheckUpdate(TRUE),bClosing(FALSE),pgd(this,ENTRY_TAG),mainFrame(this),
    logDlg(this),updDlg(this),conDlg(this),Settings(this){}

    BOOL OnInit(HINSTANCE hInst,LPTSTR pcArgs,INT nShow);
    INT OnExit(INT loopret);

    //VOID ThreadMsg(WPARAM wParam,LPARAM lParam);
    BOOL StartApp(INT nShow);
    DWORD LoginThread(LPVOID lpParameter);
    DWORD CheckUpdateThread(LPVOID lpParameter);
    DWORD UpdateThread(LPVOID lpParameter);
    LPTSTR GetLastErrorMsg();
    VOID SetThreadLocale();
    VOID DisableAppTheme();

  public:
    BOOL bFirst,bConnected,bRestart,bCheckUpdate,bClosing;
    Connection con;
    BCDEdit bcdedit;
    PatchguardDisabler pgd;
    TCHAR sAppPath[MAX_PATH];
    TCHAR sAppFile[MAX_PATH];

    MainFrame mainFrame;
    LoginDlg logDlg;
    UpdateDlg updDlg;
    ConnectingDlg conDlg;
    XThread logThread,chkupdThread,updThread;
    HANDLE hMutex;

    SettingsDlg Settings;
    CHAR szIAppMutex[0x100];
    CHAR szIAppSection[0x100];

  private:

  DECLARE_EVENT_TABLE()
};

#endif //MAIN_H
