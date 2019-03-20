#ifndef _XWND_H_
#define _XWND_H_

#include "xapp.h"
#include "xevent.h"
#include "xcoords.h"
#include "xtl.h"

#define XDEFAULTWNDCLASSNAME  _T("XWndClass")
#define XDEFAULTWNDCLASSSTYLE CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS

#define XDEFAULTICON          LoadIcon(NULL,IDI_APPLICATION)
#define XDEFAULTCURSOR        LoadCursor(NULL,IDC_ARROW)
#define XDEFAULTBACKGROUND    (HBRUSH)COLOR_BACKGROUND

#define XDEFAULTDIALOGICON    NULL
#define XDEFAULTDIALOGCURSOR  NULL
#define XDEFAULTDIALOGBG      NULL

#define XDEFAULTWNDSTYLE      WS_OVERLAPPEDWINDOW
#define XDEFAULTWNDSTYLEEX    0

#define XDEFAULTRECT  XRECT(CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT)

#define XIDM_MDI_FIRSTCHILD   0xFF00
//#define XIDM_MDI_FIRSTCHILD 0xFE00

// XWindow Flags
//#define XWF_TOOLTIPS         0x0001  // window is enabled for tooltips
//#define XWF_TEMPHIDE         0x0002  // window is temporarily hidden
//#define XWF_STAYDISABLED     0x0004  // window should stay disabled
//#define XWF_DOMODAL          0x0010  // modal loop should continue running
#define XWF_MODALLOOP          0x0020  // currently in modal loop
#define XWF_OLECTLCONTAINER  0x0100  // some descendant is an OLE control
//#define XWF_TRACKINGTOOLTIPS 0x0400  // window is enabled for tracking tooltips

//#define XWF_STAYACTIVE       0x0020  // look active even though not active
//#define XWF_NOPOPMSG         0x0040  // ignore WM_POPMESSAGESTRING calls
//#define XWF_MODALDISABLE     0x0080  // window is disabled
//#define XWF_KEEPMINIACTIVE   0x0200  // stay activate even though you are deactivated

//#define WF_NOWIN32ISDIALOGMSG   0x0800
//#define WF_ISWINFORMSVIEWWND    0x1000

class XWnd:public XEvent{
  DECLARE_DYNCREATE(XWnd)
  public:
    XWnd(HWND hWnd=NULL);
    virtual ~XWnd();

    virtual BOOL IsValid(){
      return (this==NULL?FALSE:(m_hWnd!=NULL&&::IsWindow(m_hWnd)));
    }

    HWND GetSafeHandle() const{
      return (this==NULL?NULL:m_hWnd);
    }

    virtual DWORD GetStyle() const{
      XASSERT(::IsWindow(m_hWnd))
      return (DWORD)::GetWindowLong(m_hWnd,GWL_STYLE);
    }

    virtual DWORD GetExStyle() const{
      XASSERT(::IsWindow(m_hWnd))
      return (DWORD)::GetWindowLong(m_hWnd,GWL_EXSTYLE);
    }

    virtual DWORD SetStyle(DWORD dwStyle){
      XASSERT(::IsWindow(m_hWnd))
      return (DWORD)::SetWindowLong(m_hWnd,GWL_STYLE,dwStyle);
    }

    virtual DWORD SetExStyle(DWORD dwExStyle){
      XASSERT(::IsWindow(m_hWnd))
      return (DWORD)::SetWindowLong(m_hWnd,GWL_EXSTYLE,dwExStyle);
    }

    virtual BOOL TestStyle(BOOL bEx,DWORD dwStyle){
      XASSERT(::IsWindow(m_hWnd))
      return (::GetWindowLong(m_hWnd,(bEx)?GWL_EXSTYLE:GWL_STYLE)&(dwStyle));
    }

    virtual BOOL ModifyStyle(BOOL bEx=FALSE,DWORD dwRemove=0,DWORD dwAdd=0,UINT nFlags=0);

    HWND GetOwner()const{
      return (!m_hWndOwner?::GetWindow(m_hWnd,GW_OWNER):m_hWndOwner);
    }

    VOID SetOwner(HWND pOwnerWnd){
      m_hWndOwner=pOwnerWnd;
    }

    static XWnd* FromHandle(HWND hWnd);
    static XWnd* FromHandlePermanent(HWND hWnd);

    virtual BOOL Attach(HWND hWnd);
    virtual HWND Detach();

    virtual HWND Create(LPCTSTR lpClassName,LPCTSTR lpWindowName,DWORD dwStyle=XDEFAULTWNDSTYLE,
      INT x=CW_USEDEFAULT,INT y=CW_USEDEFAULT,INT nWidth=CW_USEDEFAULT,INT nHeight=CW_USEDEFAULT,
      HWND hWndParent=NULL,HMENU hMenu=NULL,LPVOID lpParam=NULL){
      return CreateEx(0,lpClassName,lpWindowName,dwStyle,x,y,nWidth,nHeight,hWndParent,hMenu,lpParam);
    }

    virtual HWND Create(LPCTSTR lpClassName,LPCTSTR lpWindowName,DWORD dwStyle=XDEFAULTWNDSTYLE,
      const RECT& rect=XDEFAULTRECT,HWND hWndParent=NULL,HMENU hMenu=NULL,LPVOID lpParam=NULL){
      return CreateEx(0,lpClassName,lpWindowName,dwStyle,rect,hWndParent,hMenu,lpParam);
    }

    virtual HWND CreateEx(DWORD dwExStyle,LPCTSTR lpClassName,LPCTSTR lpWindowName,DWORD dwStyle=XDEFAULTWNDSTYLE,
      const RECT& rect=XDEFAULTRECT,HWND hWndParent=NULL,HMENU hMenu=NULL,LPVOID lpParam=NULL){
      return CreateEx(dwExStyle,lpClassName,lpWindowName,dwStyle,rect.left,rect.top,rect.right,rect.bottom,hWndParent,hMenu,lpParam);
    }

    virtual HWND CreateEx(DWORD dwExStyle,LPCTSTR lpClassName,LPCTSTR lpWindowName,DWORD dwStyle=XDEFAULTWNDSTYLE,
      INT x=CW_USEDEFAULT,INT y=CW_USEDEFAULT,INT nWidth=CW_USEDEFAULT,INT nHeight=CW_USEDEFAULT,
      HWND hWndParent=NULL,HMENU hMenu=NULL,LPVOID lpParam=NULL);

    virtual BOOL DestroyWindow();

    static XWnd* GetActiveWindow(){
      return new XWnd(::GetActiveWindow());
    }

    static XWnd* GetCapture(){
      return new XWnd(::GetCapture());
    }

    static XWnd* GetFocus(){
      return new XWnd(::GetFocus());
    }

    static XWnd* GetDesktopWindow(){
      return new XWnd(::GetDesktopWindow());
    }

    static XWnd* GetForegroundWindow(){
      return new XWnd(::GetForegroundWindow());
    }

    static XWnd* FindWindow(LPCTSTR lpszClassName,LPCTSTR lpszWindowName=NULL){
      return new XWnd(::FindWindow(lpszClassName,lpszWindowName));
    }

    static XWnd* FindWindowEx(HWND hwndParent,HWND hwndChildAfter,LPCTSTR lpszClass,LPCTSTR lpszWindow){
      return new XWnd(::FindWindowEx(hwndParent,hwndChildAfter,lpszClass,lpszWindow));
    }

    static XWnd* WindowFromPoint(POINT Point){
      return new XWnd(::WindowFromPoint(Point));
    }

    static XWnd* GetClipboardViewer(){
      return new XWnd(::GetClipboardViewer());
    }

    static XWnd* GetClipboardOwner(){
      return new XWnd(::GetClipboardOwner());
    }

    static XWnd* GetOpenClipboardWindow(){
      return new XWnd(::GetOpenClipboardWindow());
    }

    virtual HWND CreateDlg(HWND hWndParent,LPCTSTR lpTemplate,LPARAM lParamInit=0,HINSTANCE hInstance=NULL);
    virtual HWND CreateDlgIndirect(HWND hWndParent,LPCDLGTEMPLATE lpTemplate,LPARAM lParamInit=0,HINSTANCE hInstance=NULL);
    virtual HWND CreateDlgEx(DWORD dwExStyle,LPCTSTR lpClassName,LPCTSTR lpWindowName,DWORD dwStyle=XDEFAULTWNDSTYLE,CONST RECT& rect=XDEFAULTRECT,
      HWND hWndParent=NULL,LPCTSTR lpMenuName=NULL,LPVOID lParam=NULL);

    virtual INT_PTR CreateDlgBox(HWND hWndParent,LPCTSTR lpTemplate,LPARAM dwInitParam=0,HINSTANCE hInstance=NULL);
    virtual INT_PTR CreateDlgBoxIndirect(HWND hWndParent,LPCDLGTEMPLATE lpTemplate,LPARAM dwInitParam=0,HINSTANCE hInstance=NULL);
    virtual INT_PTR CreateDlgBoxEx(DWORD dwExStyle,LPCTSTR lpClassName,LPCTSTR lpWindowName,DWORD dwStyle=XDEFAULTWNDSTYLE,CONST RECT& rect=XDEFAULTRECT,
      HWND hWndParent=NULL,LPCTSTR lpMenuName=NULL,LPVOID lParam=NULL);

    DWORD GetDialogBaseUnitsEx(HFONT hFont);
    BOOL MapDialogRectEx(HFONT hFont,LPRECT lpRect);

    BOOL MapDialogRect(LPRECT lpRect){
      XASSERT(::IsWindow(m_hWnd))
      return ::MapDialogRect(m_hWnd,lpRect);
    }

    BOOL BeginModalState(){
      XASSERT(::IsWindow(m_hWnd))
      return ::EnableWindow(m_hWnd,FALSE);
    }

    BOOL EndModalState(){
      XASSERT(::IsWindow(m_hWnd))
      return ::EnableWindow(m_hWnd,TRUE);
    }

    virtual BOOL EnableWindow(BOOL bEnable=TRUE){
      XASSERT(::IsWindow(m_hWnd))
      return ::EnableWindow(m_hWnd,bEnable);
    }

    virtual BOOL IsEnabled() const{
      XASSERT(::IsWindow(m_hWnd))
      return ::IsWindowEnabled(m_hWnd);
    }

    virtual HWND SetActive(){
      XASSERT(::IsWindow(m_hWnd))
      return ::SetActiveWindow(m_hWnd);
	  }

    virtual HWND SetCapture(){
      XASSERT(::IsWindow(m_hWnd))
      return ::SetCapture(m_hWnd);
    }

    BOOL DragDetect(POINT pt)const{
      XASSERT(::IsWindow(m_hWnd))
      return ::DragDetect(m_hWnd,pt);
    }

    virtual HWND SetFocus(){
      XASSERT(::IsWindow(m_hWnd))
      return ::SetFocus(m_hWnd);
    }

    BOOL SetForegroundWindow(){
      XASSERT(::IsWindow(m_hWnd))
      return ::SetForegroundWindow(m_hWnd);
    }

    UINT ArrangeIconicWindows(){
      XASSERT(::IsWindow(m_hWnd))
      return ::ArrangeIconicWindows(m_hWnd);
    }

    BOOL BringWindowToTop(){
      XASSERT(::IsWindow(m_hWnd))
      return ::BringWindowToTop(m_hWnd);
    }

    HWND GetTopWindow(){
      XASSERT(::IsWindow(m_hWnd))
      return ::GetTopWindow(m_hWnd);
    }

    BOOL CloseWindow(){
      XASSERT(::IsWindow(m_hWnd))
      return ::CloseWindow(m_hWnd);
    }

  #if (_WIN32_WINDOWS >= 0x0500)

    ULONG_PTR GetClassLong(int nIndex){
      //GetClassLong is deprecated
      XASSERT(::IsWindow(m_hWnd))
      return ::GetClassLongPtr(m_hWnd,nIndex);
    }

    ULONG_PTR SetClassLong(int nIndex,LONG_PTR dwNewLong){
      //SetClassLong is deprecated
      XASSERT(::IsWindow(m_hWnd))
      return ::SetClassLongPtr(m_hWnd,nIndex,dwNewLong);
    }

  #else

    DWORD GetClassLong(int nIndex){
      XASSERT(::IsWindow(m_hWnd))
      return ::GetClassLong(m_hWnd,nIndex);
    }

    DWORD SetClassLong(int nIndex,LONG dwNewLong){
      XASSERT(::IsWindow(m_hWnd))
      return ::SetClassLong(m_hWnd,nIndex,dwNewLong);
    }

    WORD GetClassWord(int nIndex){
      XASSERT(::IsWindow(m_hWnd))
      return ::GetClassWord(m_hWnd,nIndex);
    }

    WORD SetClassWord(int nIndex,WORD wNewWord){
      XASSERT(::IsWindow(m_hWnd))
      return ::SetClassWord(m_hWnd,nIndex,wNewWord);
    }

  #endif

    int GetClassName(LPTSTR lpClassName,int nMaxCount){
      XASSERT(::IsWindow(m_hWnd))
      return ::GetClassName(m_hWnd,lpClassName,nMaxCount);
    }

    DWORD GetContextHelpId() const{
	    XASSERT(::IsWindow(m_hWnd))
      return ::GetWindowContextHelpId(m_hWnd);
	  }

    INT MessageBox(LPCTSTR lpText,LPCTSTR lpCaption=NULL,UINT uType=0){
      XASSERT(::IsWindow(m_hWnd))
      return ::MessageBox(m_hWnd,lpText,lpCaption,uType);
    }

    INT MessageBoxEx(LPCTSTR lpText,LPCTSTR lpCaption,UINT uType,WORD wLanguageId){
      XASSERT(::IsWindow(m_hWnd))
      return ::MessageBoxEx(m_hWnd,lpText,lpCaption,uType,wLanguageId);
    }

    virtual BOOL ShowWindow(UINT mode=SW_SHOWNORMAL){
      XASSERT(::IsWindow(m_hWnd))
      return ::ShowWindow(m_hWnd,mode);
    }

    virtual BOOL HideWindow(){
      XASSERT(::IsWindow(m_hWnd))
      return ::ShowWindow(m_hWnd,SW_HIDE);
    }

    BOOL SetProp(LPCTSTR lpString,HANDLE hData){
      XASSERT(::IsWindow(m_hWnd))
      return ::SetProp(m_hWnd,lpString,hData);
    }

    HANDLE GetProp(LPCTSTR lpString){
      XASSERT(::IsWindow(m_hWnd))
      return ::GetProp(m_hWnd,lpString);
    }

    HANDLE RemoveProp(LPCTSTR lpString){
      XASSERT(::IsWindow(m_hWnd))
      return ::RemoveProp(m_hWnd,lpString);
    }

    BOOL ShowOwnedPopups(BOOL bShow){
      XASSERT(::IsWindow(m_hWnd))
      return ::ShowOwnedPopups(m_hWnd,bShow);
    }

    virtual HICON SetIcon(HICON hIcon,BOOL bBigIcon=FALSE){
      XASSERT(::IsWindow(m_hWnd))
      return (HICON)::SendMessage(m_hWnd,WM_SETICON,bBigIcon,(LPARAM)hIcon);
    }

    virtual HICON GetIcon(BOOL bBigIcon=FALSE)const{
      XASSERT(::IsWindow(m_hWnd))
      return (HICON)::SendMessage(m_hWnd,WM_GETICON,bBigIcon,0);
    }

    virtual INT SetDlgCtrlID(INT nID){
      XASSERT(::IsWindow(m_hWnd))
      return (INT)::SetWindowLong(m_hWnd,GWL_ID,nID);
    }

    virtual INT GetDlgCtrlID()const{
      XASSERT(::IsWindow(m_hWnd))
      return ::GetDlgCtrlID(m_hWnd);
    }

    HWND GetDlgItem(INT nIDDlgItem)const{
      XASSERT(::IsWindow(m_hWnd))
      return ::GetDlgItem(m_hWnd,nIDDlgItem);
    }

    VOID Print(HDC hdc,DWORD dwFlags)const{
      XASSERT(::IsWindow(m_hWnd))
      ::SendMessage(m_hWnd,WM_PRINT,(WPARAM)hdc,(LPARAM)dwFlags);
    }

    VOID PrintClient(HDC hdc,DWORD dwFlags)const{
      XASSERT(::IsWindow(m_hWnd))
      ::SendMessage(m_hWnd,WM_PRINTCLIENT,(WPARAM)hdc, dwFlags);
    }

    VOID SetRedraw(BOOL bRedraw){
      XASSERT(::IsWindow(m_hWnd))
      ::SendMessage(m_hWnd,WM_SETREDRAW,bRedraw,0);
    }

    HFONT GetFont()const{
      XASSERT(::IsWindow(m_hWnd))
      return (HFONT)::SendMessage(m_hWnd,WM_GETFONT,0,0);
    }

    VOID SetFont(HFONT hFont,BOOL bRedraw=TRUE)const{
      XASSERT(::IsWindow(m_hWnd))
      ::SendMessage(m_hWnd,WM_SETFONT,(WPARAM)hFont,MAKELPARAM(bRedraw,0));
    }

  #if (_WIN32_WINDOWS >= 0x0501)

    BOOL PrintWindow(HDC hdcBlt,UINT nFlags){
      XASSERT(::IsWindow(m_hWnd))
      return ::PrintWindow(m_hWnd,hdcBlt,nFlags);
    }

  #endif

    LONG_PTR GetWindowLong(INT nIndex)const{
      XASSERT(::IsWindow(m_hWnd))
      return ::GetWindowLong(m_hWnd,nIndex);
    }

    LONG_PTR SetWindowLong(INT nIndex,LONG_PTR dwNewLong)const{
      XASSERT(::IsWindow(m_hWnd))
      return ::SetWindowLong(m_hWnd,nIndex,dwNewLong);
    }

  #ifdef _WIN64

    LONG GetWindowLongPtr(INT nIndex)const{
      XASSERT(::IsWindow(m_hWnd))
      return ::GetWindowLongPtr(m_hWnd,nIndex);
    }

    LONG SetWindowLongPtr(INT nIndex,LONG dwNewLong)const{
      XASSERT(::IsWindow(m_hWnd))
      return ::SetWindowLongPtr(m_hWnd,nIndex,dwNewLong);
    }

  #endif

    BOOL IsWindow()const{
      return ::IsWindow(m_hWnd);
    }

    BOOL IsVisible(){
      XASSERT(::IsWindow(m_hWnd))
      return IsWindowVisible(m_hWnd);
    }

    BOOL IsActive(){
      XASSERT(::IsWindow(m_hWnd))
      return (::GetActiveWindow()==m_hWnd);
    }

    BOOL IsFocused(){
      XASSERT(::IsWindow(m_hWnd))
      return (::GetFocus()==m_hWnd);
    }

    BOOL IsChild(HWND hWnd){
      XASSERT(::IsWindow(m_hWnd))
      return ::IsChild(m_hWnd,hWnd);
    }

    BOOL IsDialogMessage(LPMSG lpMsg){
      XASSERT(::IsWindow(m_hWnd))
      return ::IsDialogMessage(m_hWnd,lpMsg);
    }

  #if (_WIN32_WINDOWS >= 0x0500)

    BOOL AnimateWindow(DWORD dwTime,DWORD dwFlags){
      XASSERT(::IsWindow(m_hWnd))
      return ::AnimateWindow(m_hWnd,dwTime,dwFlags);
    }

  #endif

    BOOL DrawAnimatedRects(INT idAni,const RECT *lprcFrom,const RECT *lprcTo){
      XASSERT(::IsWindow(m_hWnd))
      return ::DrawAnimatedRects(m_hWnd,idAni,lprcFrom,lprcTo);
    }

    BOOL DrawCaption(HDC hdc,LPCRECT lprc,UINT uFlags){
      XASSERT(::IsWindow(m_hWnd))
      return ::DrawCaption(m_hWnd,hdc,lprc,uFlags);
    }

    BOOL EnableScrollBar(UINT wSBflags,UINT wArrows){
      XASSERT(::IsWindow(m_hWnd))
      return ::EnableScrollBar(m_hWnd,wSBflags,wArrows);
    }

    BOOL IsTopParentActive()const{
      XASSERT(::IsWindow(m_hWnd))
      HWND pParent=GetTopLevelParent();
      if(::IsWindow(pParent))
        return ::GetForegroundWindow()==pParent;
      return FALSE;
    }

    BOOL ActivateTopParent(){
      XASSERT(::IsWindow(m_hWnd))
      HWND pActiveWnd=::GetForegroundWindow();
      if(pActiveWnd==NULL||!(pActiveWnd==m_hWnd||::IsChild(pActiveWnd,m_hWnd))){
        HWND pParent=GetTopLevelParent();
        if(::IsWindow(pParent))
          return ::SetForegroundWindow(pParent);
      }
      return FALSE;
    }

    virtual BOOL GetWindowRect(LPRECT rc)const{
      XASSERT(::IsWindow(m_hWnd))
      return ::GetWindowRect(m_hWnd,rc);
    }

    virtual BOOL GetClientRect(LPRECT rc)const{
      XASSERT(::IsWindow(m_hWnd))
      return ::GetClientRect(m_hWnd,rc);
    }

    HWND GetWindow(UINT uCmd)const{
      XASSERT(::IsWindow(m_hWnd))
      return ::GetWindow(m_hWnd,uCmd);
    }

    INT GetWindowRgn(HRGN hRgn)const{
      XASSERT(::IsWindow(m_hWnd))
      return ::GetWindowRgn(m_hWnd,hRgn);
    }

    INT SetWindowRgn(HRGN hRgn,BOOL bRedraw){
      XASSERT(::IsWindow(m_hWnd))
      return ::SetWindowRgn(m_hWnd,hRgn,bRedraw);
    }

  #if (_WIN32_WINDOWS >= 0x0500)

    VOID GetEffectiveClientRect(LPRECT rc,INT *lpInfo){
      XASSERT(::IsWindow(m_hWnd))
      ::GetEffectiveClientRect(m_hWnd,rc,lpInfo);
    }

  #endif

    LRESULT SendMessage(UINT message,WPARAM wParam=0,LPARAM lParam=0)const{
      XASSERT(::IsWindow(m_hWnd))
      return ::SendMessage(m_hWnd, message, wParam, lParam);
    }

    BOOL PostMessage(UINT message,WPARAM wParam=0,LPARAM lParam=0)const{
      XASSERT(::IsWindow(m_hWnd))
      return ::PostMessage(m_hWnd, message, wParam, lParam);
    }

    LONG SendDlgItemMessage(INT nIDDlgItem,UINT Msg,WPARAM wParam,LPARAM lParam){
      XASSERT(::IsWindow(m_hWnd))
      return ::SendDlgItemMessage(m_hWnd,nIDDlgItem,Msg,wParam,lParam);
    }

    BOOL SendNotifyMessage(UINT Msg,WPARAM wParam,LPARAM lParam){
      XASSERT(::IsWindow(m_hWnd))
      return ::SendNotifyMessage(m_hWnd,Msg,wParam,lParam);
    }

    BOOL UpdateWindow(){
      XASSERT(::IsWindow(m_hWnd))
      return ::UpdateWindow(m_hWnd);
    }

    BOOL LockWindowUpdate(){
      XASSERT(::IsWindow(m_hWnd))
      return ::LockWindowUpdate(m_hWnd);
    }

    BOOL UnLockWindowUpdate()const{
      XASSERT(::IsWindow(m_hWnd))
      return ::LockWindowUpdate(0);
    }

    BOOL GetUpdateRect(LPRECT lpRect,BOOL bErase=FALSE)const{
      XASSERT(::IsWindow(m_hWnd))
      return ::GetUpdateRect(m_hWnd,lpRect,bErase);
    }

    INT GetUpdateRgn(HRGN hRgn,BOOL bErase=FALSE)const{
      XASSERT(::IsWindow(m_hWnd))
      return ::GetUpdateRgn(m_hWnd,hRgn,bErase);
    }

    BOOL Invalidate(BOOL bErase=TRUE)const{
      XASSERT(::IsWindow(m_hWnd))
      return ::InvalidateRect(m_hWnd,NULL,bErase);
    }

    BOOL InvalidateRect(LPRECT rect,BOOL erase)const{
      XASSERT(::IsWindow(m_hWnd))
      return ::InvalidateRect(m_hWnd,rect,erase);
    }

    BOOL InvalidateRgn(HRGN hRgn,BOOL bErase)const{
      XASSERT(::IsWindow(m_hWnd))
      return ::InvalidateRgn(m_hWnd,hRgn,bErase);
    }

    BOOL ValidateRect(const LPRECT lpRect){
      XASSERT(::IsWindow(m_hWnd))
      return ::ValidateRect(m_hWnd,lpRect);
    }

    BOOL ValidateRgn(HRGN hRgn){
      XASSERT(::IsWindow(m_hWnd))
      return ::ValidateRgn(m_hWnd,hRgn);
    }

    BOOL RedrawWindow(const LPRECT lprcUpdate=NULL,HRGN hrgnUpdate=NULL,UINT flags=RDW_INVALIDATE|RDW_UPDATENOW|RDW_ERASE){
      XASSERT(::IsWindow(m_hWnd))
      return ::RedrawWindow(m_hWnd,lprcUpdate,hrgnUpdate,flags);
    }

    BOOL ResizeWindow(INT cx,INT cy){
      XASSERT(::IsWindow(m_hWnd))
      XRect rect;
      ::GetWindowRect(m_hWnd,&rect);
      return ::SetWindowPos(m_hWnd,NULL,0,0,rect.Width()+cx,rect.Height()+cy,SWP_NOMOVE|SWP_NOACTIVATE|SWP_NOZORDER);
    }

    virtual BOOL SetWindowPos(HWND after,INT X,INT Y,INT cx,INT cy,UINT uFlags){
      XASSERT(::IsWindow(m_hWnd))
      return ::SetWindowPos(m_hWnd,after,X,Y,cx,cy,uFlags);
    }

    INT MapWindowPoints(HWND hWndTo,LPPOINT pPoint,INT cPoints)const{
      XASSERT(::IsWindow(m_hWnd))
      return ::MapWindowPoints(m_hWnd,hWndTo,pPoint,cPoints);
    }

    INT MapWindowPoints(HWND hWndTo,LPRECT prc)const{
      XASSERT(::IsWindow(m_hWnd))
      return ::MapWindowPoints(m_hWnd,hWndTo,(LPPOINT)prc,2);
    }

    INT MapPointsWindow(HWND hWndFrom,LPPOINT pPoint,INT cPoints=2)const{
      XASSERT(::IsWindow(m_hWnd))
      return ::MapWindowPoints(hWndFrom,m_hWnd,pPoint,cPoints);
    }

    INT MapPointsWindow(HWND hWndFrom,LPRECT prc)const{
      XASSERT(::IsWindow(m_hWnd))
      return ::MapWindowPoints(hWndFrom,m_hWnd,(LPPOINT)prc,2);
    }

    BOOL ClientToScreen(LPPOINT pPoint)const{
      XASSERT(::IsWindow(m_hWnd))
      return ::ClientToScreen(m_hWnd,pPoint);
    }

    BOOL ClientToScreen(LPRECT prc)const{
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::MapWindowPoints(m_hWnd,NULL,(LPPOINT)prc,2);
    }

    BOOL ScreenToClient(LPPOINT pPoint)const{
      XASSERT(::IsWindow(m_hWnd))
      return ::ScreenToClient(m_hWnd,pPoint);
    }

    BOOL ScreenToClient(LPRECT prc)const{
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::MapWindowPoints(NULL,m_hWnd,(LPPOINT)prc,2);
    }

    HWND SetParent(HWND newParent){
      XASSERT(::IsWindow(m_hWnd))
      return ::SetParent(m_hWnd,newParent);
    }

    HWND GetParent(){
      XASSERT(::IsWindow(m_hWnd))
      return ::GetParent(m_hWnd);
    }

    HWND GetTopLevelParent()const{
      XASSERT(::IsWindow(m_hWnd))
      HWND hWndTmp,hWndParent=m_hWnd;
      while((hWndTmp=::GetParent(hWndParent))!=NULL)
        hWndParent=hWndTmp;
      return hWndParent;
    }

    HWND GetTopLevelOwner()const{
      XASSERT(::IsWindow(m_hWnd))
      HWND hWndT,hWndOwner=m_hWnd;
      while((hWndT=::GetWindow(hWndOwner,GW_OWNER))!=NULL)
        hWndOwner=hWndT;
      return hWndOwner;
    }

    HWND GetParentOwner()const{
      XASSERT(::IsWindow(m_hWnd))
      HWND hWndT,hWndParent=m_hWnd;
      while((::GetWindowLong(hWndParent,GWL_STYLE)&WS_CHILD)&&(hWndT=::GetParent(hWndParent))!=NULL)
        hWndParent=hWndT;
      return hWndParent;
    }

    HWND GetAncestor(UINT gaFlags=GA_PARENT)const{
      XASSERT(::IsWindow(m_hWnd))
      return ::GetAncestor(m_hWnd,gaFlags);
    }

    HWND GetLastActivePopup()const{
      XASSERT(::IsWindow(m_hWnd))
      return ::GetLastActivePopup(m_hWnd);
    }

    HWND GetNextWindow(UINT wCmd=GW_HWNDNEXT)const{
      XASSERT(::IsWindow(m_hWnd))
      return ::GetWindow(m_hWnd,wCmd);
    }

    HWND ChildWindowFromPoint(POINT point)const{
      XASSERT(::IsWindow(m_hWnd))
      return ::ChildWindowFromPoint(m_hWnd, point);
    }

    HWND ChildWindowFromPoint(POINT point, UINT nFlags)const{
      XASSERT(::IsWindow(m_hWnd))
      return ::ChildWindowFromPointEx(m_hWnd, point, nFlags);
    }

    LONG GetWindowedChildCount(){
      XASSERT(::IsWindow(m_hWnd))
      LONG lCount=0;
      for(HWND pChild=::GetWindow(m_hWnd,GW_CHILD);pChild!=NULL; pChild=::GetWindow(pChild,GW_HWNDNEXT),lCount++);
      return lCount;
    }

    UINT GetDlgItemInt(INT nIDDlgItem,BOOL *lpTranslated=NULL,BOOL bSigned=TRUE){
      XASSERT(::IsWindow(m_hWnd))
      return ::GetDlgItemInt(m_hWnd,nIDDlgItem,lpTranslated,bSigned);
    }

    UINT GetDlgItemText(INT nIDDlgItem,LPTSTR lpString,INT nMaxCount){
      XASSERT(::IsWindow(m_hWnd))
      return ::GetDlgItemText(m_hWnd,nIDDlgItem,lpString,nMaxCount);
    }

    BOOL WINAPI SetDlgItemInt(INT nIDDlgItem,UINT uValue,BOOL bSigned=TRUE){
      XASSERT(::IsWindow(m_hWnd))
      return ::SetDlgItemInt(m_hWnd,nIDDlgItem,uValue,bSigned);
    }

    BOOL WINAPI SetDlgItemText(INT nIDDlgItem,LPCTSTR lpString){
      XASSERT(::IsWindow(m_hWnd))
      return ::SetDlgItemText(m_hWnd,nIDDlgItem,lpString);
    }

    HWND GetNextDlgGroupItem(HWND hCtl,BOOL bPrevious=FALSE){
      XASSERT(::IsWindow(m_hWnd))
      return ::GetNextDlgGroupItem(m_hWnd,hCtl,bPrevious);
    }

    HWND GetNextDlgTabItem(HWND hCtl,BOOL bPrevious=FALSE){
      XASSERT(::IsWindow(m_hWnd))
      return ::GetNextDlgTabItem(m_hWnd,hCtl,bPrevious);
    }

    BOOL CheckDlgButton(INT nIDButton,UINT uCheck){
      XASSERT(::IsWindow(m_hWnd))
      return ::CheckDlgButton(m_hWnd,nIDButton,uCheck);
    }

    BOOL CheckRadioButton(INT nIDFirstButton,INT nIDLastButton,INT nIDCheckButton){
      XASSERT(::IsWindow(m_hWnd))
      return ::CheckRadioButton(m_hWnd,nIDFirstButton,nIDLastButton,nIDCheckButton);
    }

    INT DlgDirList(LPTSTR lpPathSpec,INT nIDListBox,INT nIDStaticPath,UINT uFileType){
      XASSERT(::IsWindow(m_hWnd))
      return ::DlgDirList(m_hWnd,lpPathSpec,nIDListBox,nIDStaticPath,uFileType);
    }

    INT DlgDirListComboBox(LPTSTR lpPathSpec,INT nIDComboBox,INT nIDStaticPath,UINT uFiletype){
      XASSERT(::IsWindow(m_hWnd))
      return ::DlgDirListComboBox(m_hWnd, lpPathSpec, nIDComboBox, nIDStaticPath, uFiletype);
    }

    BOOL DlgDirSelect(LPTSTR lpString,INT nIDListBox){
      XASSERT(::IsWindow(m_hWnd))
      return ::DlgDirSelectEx(m_hWnd,lpString,MAX_PATH,nIDListBox);
    }

    BOOL DlgDirSelect(LPTSTR lpString,INT nCount,INT nIDListBox){
      XASSERT(::IsWindow(m_hWnd))
      return ::DlgDirSelectEx(m_hWnd,lpString,nCount,nIDListBox);
    }

    BOOL DlgDirSelectComboBox(LPTSTR lpString,INT nIDComboBox){
      XASSERT(::IsWindow(m_hWnd))
      return ::DlgDirSelectComboBoxEx(m_hWnd,lpString,MAX_PATH,nIDComboBox);
    }

    BOOL DlgDirSelectComboBox(LPTSTR lpString,INT nCount,INT nIDComboBox){
      XASSERT(::IsWindow(m_hWnd))
      return ::DlgDirSelectComboBoxEx(m_hWnd,lpString,nCount,nIDComboBox);
    }

    UINT IsDlgButtonChecked(INT nIDButton){
      XASSERT(::IsWindow(m_hWnd))
      return ::IsDlgButtonChecked(m_hWnd,nIDButton);
    }

    INT GetCheckedRadioButton(INT nIDFirstButton,INT nIDLastButton)const{
      for(INT nID=nIDFirstButton;nID<=nIDLastButton;nID++){
        if(::IsDlgButtonChecked(m_hWnd,nID))
          return nID;
      }
      return 0;
    }

    BOOL GetScrollBarInfo(LONG idObject,PSCROLLBARINFO psbi){
      XASSERT(::IsWindow(m_hWnd)&&psbi!=NULL)
      return ::GetScrollBarInfo(m_hWnd,idObject,psbi);
    }

  #if (_WIN32_WINDOWS >= 0x0500)

    BOOL GetWindowInfo(PWINDOWINFO pwi){
      XASSERT(::IsWindow(m_hWnd))
      return ::GetWindowInfo(m_hWnd,pwi);
    }

    BOOL GetTitleBarInfo(PTITLEBARINFO pti){
      XASSERT(::IsWindow(m_hWnd)&&pti!=NULL)
      return ::GetTitleBarInfo(m_hWnd,pti);
    }

  #endif

    BOOL GetMenuBarInfo(LONG idObject,LONG idItem,PMENUBARINFO pmbi){
      XASSERT(::IsWindow(m_hWnd)&&pmbi!=NULL)
      return ::GetMenuBarInfo(m_hWnd,idObject,idItem,pmbi);
    }

    BOOL DrawMenuBar(){
      XASSERT(::IsWindow(m_hWnd))
      return ::DrawMenuBar(m_hWnd);
    }

    virtual BOOL SetWindowText(LPCTSTR lpString){
      XASSERT(::IsWindow(m_hWnd))
      return ::SetWindowText(m_hWnd,lpString);
    }

    BOOL SetWindowTextEx(HINSTANCE hInstance,LPCTSTR lpRsrcString){
      XASSERT(::IsWindow(m_hWnd))
      //set window text from rsrc...
      BOOL bRet=FALSE;LPTSTR szBuffer=NULL;
      if(lpRsrcString&&IS_INTRESOURCE(lpRsrcString)){
        UINT uLen=::GetStringLength(hInstance,(UINT_PTR)lpRsrcString);
        if(uLen&&(szBuffer=(LPTSTR)LocalAlloc(LMEM_ZEROINIT,++uLen*sizeof(TCHAR)))){
          if(::LoadString(hInstance,(UINT_PTR)lpRsrcString,szBuffer,uLen))
            bRet=SetWindowText(szBuffer);
          LocalFree(szBuffer);
        }
      }
      return bRet;
    }

    virtual INT GetWindowText(LPTSTR lpString,INT nMaxCount)const{
      XASSERT(::IsWindow(m_hWnd)&&lpString!=NULL)
      return ::GetWindowText(m_hWnd,lpString,nMaxCount);
    }

    INT GetWindowTextLength()const{
      XASSERT(::IsWindow(m_hWnd))
      return ::GetWindowTextLength(m_hWnd);
    }

    BOOL SetWindowPlacement(const WINDOWPLACEMENT*lpwndpl){
      XASSERT(::IsWindow(m_hWnd))
      return ::SetWindowPlacement(m_hWnd,lpwndpl);
    }

    BOOL GetWindowPlacement(WINDOWPLACEMENT *lpwndpl){
      XASSERT(::IsWindow(m_hWnd))
      return ::GetWindowPlacement(m_hWnd,lpwndpl);
    }

    UINT GetWindowShowState(){
      XASSERT(::IsWindow(m_hWnd))
      WINDOWPLACEMENT wd;
      wd.length=sizeof(WINDOWPLACEMENT);
      if(::GetWindowPlacement(m_hWnd,&wd))
        return wd.showCmd;
      return 0;
    }

    BOOL IsIconic()const{
      XASSERT(::IsWindow(m_hWnd))
      return ::IsIconic(m_hWnd);
    }

    BOOL IsZoomed()const{
      XASSERT(::IsWindow(m_hWnd))
      return ::IsZoomed(m_hWnd);
    }

    BOOL ReleaseCapture(){
      XASSERT(::IsWindow(m_hWnd))
      return ::ReleaseCapture();
    }

    virtual BOOL MoveWindow(INT X,INT Y,INT nWidth,INT nHeight,BOOL bRepaint){
      XASSERT(::IsWindow(m_hWnd))
      return ::MoveWindow(m_hWnd,X,Y,nWidth,nHeight,bRepaint);
    }

    BOOL MoveWindow(LPPOINT pos,LPSIZE size,BOOL bRepaint){
      XASSERT(::IsWindow(m_hWnd))
      return ::MoveWindow(m_hWnd,pos->x,pos->y,size->cx,size->cy,bRepaint);
    }

    BOOL MoveWindow(LPRECT rect,BOOL bRepaint){
      XASSERT(::IsWindow(m_hWnd))
      return ::MoveWindow(m_hWnd,rect->left,rect->top,rect->right,rect->bottom,bRepaint);
    }

    HDC BeginPaint(LPPAINTSTRUCT lpPaint){
      XASSERT(::IsWindow(m_hWnd))
      return ::BeginPaint(m_hWnd,lpPaint);
    }

    BOOL EndPaint(CONST PAINTSTRUCT *lpPaint){
      XASSERT(::IsWindow(m_hWnd))
      return ::EndPaint(m_hWnd,lpPaint);
    }

    HDC GetDC(){
      XASSERT(::IsWindow(m_hWnd))
      return ::GetDC(m_hWnd);
    }

    HDC GetDCEx(HRGN hrgnClip,DWORD flags){
      XASSERT(::IsWindow(m_hWnd))
      return ::GetDCEx(m_hWnd,hrgnClip,flags);
    }

    HDC GetWindowDC(){
      XASSERT(::IsWindow(m_hWnd))
      return ::GetWindowDC(m_hWnd);
    }

    INT ReleaseDC(HDC hdc){
      XASSERT(::IsWindow(m_hWnd))
      return ::ReleaseDC(m_hWnd,hdc);
    }

    BOOL SetMenu(HMENU hMenu){
      XASSERT(::IsWindow(m_hWnd))
      return ::SetMenu(m_hWnd,hMenu);
    }

    HMENU GetMenu(){
      XASSERT(::IsWindow(m_hWnd))
      return ::GetMenu(m_hWnd);
    }

    BOOL GetMenuItemRect(HMENU hMenu,UINT uItem,LPRECT rcItem){
      XASSERT(::IsWindow(m_hWnd))
      return ::GetMenuItemRect(m_hWnd,hMenu,uItem,rcItem);
    }

    HMENU GetSystemMenu(BOOL bRevert=FALSE){
      XASSERT(::IsWindow(m_hWnd))
      return ::GetSystemMenu(m_hWnd,bRevert);
    }

    BOOL HiliteMenuItem(HMENU hMenu,UINT uItem,UINT uHilite){
      XASSERT(::IsWindow(m_hWnd))
      return ::HiliteMenuItem(m_hWnd,hMenu,uItem,uHilite);
    }

    BOOL WinHelp(LPTSTR lpszHelp,UINT uCommand,UINT dwData){
      XASSERT(::IsWindow(m_hWnd))
      return ::WinHelp(m_hWnd,lpszHelp,uCommand,dwData);
    }

    BOOL TrackMouseEvent(DWORD dwFlags=TME_HOVER|TME_LEAVE,DWORD dwHoverTime=HOVER_DEFAULT){
      XASSERT(::IsWindow(m_hWnd))
      TRACKMOUSEEVENT EventTrack={sizeof(TRACKMOUSEEVENT),dwFlags,m_hWnd,dwHoverTime};
      return ::TrackMouseEvent(&EventTrack);
    }

    BOOL GetScrollInfo(INT fnBar,LPSCROLLINFO lpsi,UINT nMask=SIF_ALL){
      XASSERT(::IsWindow(m_hWnd)&&lpsi!=NULL)
      lpsi->cbSize=sizeof(*lpsi);
      lpsi->fMask=nMask;
      return ::GetScrollInfo(m_hWnd,fnBar,lpsi);
    }

    INT SetScrollInfo(INT fnBar,LPCSCROLLINFO lpsi,BOOL fRedraw=TRUE){
      XASSERT(::IsWindow(m_hWnd))
      return ::SetScrollInfo(m_hWnd,fnBar,lpsi,fRedraw);
    }

    BOOL SetScrollRange(INT nBar, INT nMinPos, INT nMaxPos, BOOL bRedraw){
      XASSERT(::IsWindow(m_hWnd))
      return ::SetScrollRange(m_hWnd, nBar, nMinPos, nMaxPos, bRedraw);
    }

    BOOL GetScrollRange(INT nBar, LPINT lpMinPos, LPINT lpMaxPos)const{
      XASSERT(::IsWindow(m_hWnd))
      return ::GetScrollRange(m_hWnd, nBar, lpMinPos, lpMaxPos);
    }

    INT GetScrollPos(INT nBar){
      XASSERT(::IsWindow(m_hWnd))
      return ::GetScrollPos(m_hWnd,nBar);
    }

    INT SetScrollPos(INT nBar,INT nPos,BOOL bRedraw=TRUE){
      XASSERT(::IsWindow(m_hWnd))
      return ::SetScrollPos(m_hWnd,nBar,nPos,bRedraw);
    }

    BOOL ScrollWindow(INT xAmount,INT yAmount,LPCRECT lpRect = NULL,LPCRECT lpClipRect = NULL){
      XASSERT(::IsWindow(m_hWnd))
      return ::ScrollWindow(m_hWnd,xAmount,yAmount,lpRect,lpClipRect);
    }

    INT ScrollWindowEx(INT dx,INT dy,const LPRECT prcScroll,const LPRECT prcClip,HRGN hrgnUpdate,LPRECT prcUpdate,UINT flags){
      XASSERT(::IsWindow(m_hWnd))
      return ::ScrollWindowEx(m_hWnd,dx,dy,prcScroll,prcClip,hrgnUpdate,prcUpdate,flags);
    }

    INT GetScrollLimit(INT nBar){
      XASSERT(::IsWindow(m_hWnd))
      INT nMin,nMax;SCROLLINFO info;
      ::GetScrollRange(m_hWnd,nBar,&nMin,&nMax);
      info.cbSize=sizeof(info);
      info.fMask=SIF_PAGE;
      if(::GetScrollInfo(m_hWnd,nBar,&info))
        nMax-=__max(info.nPage-1,0);
      return nMax;
    }

    BOOL ShowScrollBar(INT wBar,BOOL bShow=TRUE){
      XASSERT(::IsWindow(m_hWnd))
      return ::ShowScrollBar(m_hWnd,wBar,bShow);
    }

    VOID DragAcceptFiles(BOOL fAccept=TRUE){
      XASSERT(::IsWindow(m_hWnd))
      ::DragAcceptFiles(m_hWnd,fAccept);
    }

    BOOL KillTimer(UINT_PTR uIDEvent){
      XASSERT(::IsWindow(m_hWnd))
      return ::KillTimer(m_hWnd,uIDEvent);
    }

    UINT_PTR SetTimer(UINT_PTR nIDEvent,UINT uElapse,TIMERPROC lpTimerFunc=NULL){
      XASSERT(::IsWindow(m_hWnd))
      return ::SetTimer(m_hWnd,nIDEvent,uElapse,lpTimerFunc);
    }

    BOOL FlashWindow(BOOL bInvert){
      XASSERT(::IsWindow(m_hWnd))
      return ::FlashWindow(m_hWnd,bInvert);
    }

    INT TranslateAccelerator(HACCEL hAccTable,LPMSG lpMsg){
      XASSERT(::IsWindow(m_hWnd))
      return ::TranslateAccelerator(m_hWnd,hAccTable,lpMsg);
    }

  #if (_WIN32_WINNT >= 0x500)

    BOOL FlashWindowEx(DWORD dwFlags,UINT uCount,DWORD dwTimeout){
      XASSERT(::IsWindow(m_hWnd))
      FLASHWINFO fwi;
      fwi.cbSize=sizeof(fwi);
      fwi.hwnd=m_hWnd;
      fwi.dwFlags=dwFlags;
      fwi.uCount=uCount;
      fwi.dwTimeout=dwTimeout;
      return ::FlashWindowEx(&fwi);
    }

  #endif

    BOOL OpenClipboard(){
      XASSERT(::IsWindow(m_hWnd))
      return ::OpenClipboard(m_hWnd);
    }

    HWND SetClipboardViewer(){
      XASSERT(::IsWindow(m_hWnd))
      return ::SetClipboardViewer(m_hWnd);
    }

    BOOL ChangeClipboardChain(HWND hWndNewNext){
      XASSERT(::IsWindow(m_hWnd))
      return ::ChangeClipboardChain(m_hWnd,hWndNewNext);
    }

    BOOL WinHelp(LPCTSTR lpszHelp,UINT uCommand,ULONG_PTR dwData){
      XASSERT(::IsWindow(m_hWnd))
      return ::WinHelp(m_hWnd,lpszHelp,uCommand,dwData);
    }

    BOOL CreateCaret(HBITMAP hBitmap,INT nWidth,INT nHeight){
      XASSERT(::IsWindow(m_hWnd))
      return ::CreateCaret(m_hWnd,hBitmap,nWidth,nHeight);
    }

    BOOL CreateGrayCaret(INT nWidth,INT nHeight){
      XASSERT(::IsWindow(m_hWnd))
      return ::CreateCaret(m_hWnd,(HBITMAP)1,nWidth,nHeight);
    }

    BOOL CreateSolidCaret(INT nWidth,INT nHeight){
      XASSERT(::IsWindow(m_hWnd))
      return ::CreateCaret(m_hWnd,(HBITMAP)0,nWidth,nHeight);
    }

    BOOL HideCaret(){
      XASSERT(::IsWindow(m_hWnd))
      return ::HideCaret(m_hWnd);
    }

    BOOL ShowCaret(){
      XASSERT(::IsWindow(m_hWnd))
      return ::ShowCaret(m_hWnd);
    }

    static BOOL GetCaretPos(LPPOINT lpPoint){
      return ::GetCaretPos(lpPoint);
    }

    static BOOL SetCaretPos(POINT point){
      return ::SetCaretPos(point.x,point.y);
    }

  #if (_WIN32_WINNT >= 0x501)

    BOOL GetLayeredWindowAttributes(COLORREF *pcrKey,BYTE *pbAlpha,DWORD *pdwFlags){
      XASSERT(::IsWindow(m_hWnd))
      return ::GetLayeredWindowAttributes(m_hWnd,pcrKey,pbAlpha,pdwFlags);
    }

    BOOL SetLayeredWindowAttributes(COLORREF crKey,BYTE bAlpha,DWORD dwFlags){
      XASSERT(::IsWindow(m_hWnd))
      return ::SetLayeredWindowAttributes(m_hWnd,crKey,bAlpha,dwFlags);
    }

    BOOL UpdateLayeredWindow(HDC hdcDst,POINT *pptDst,SIZE *psize,HDC hdcSrc,POINT *pptSrc,COLORREF crKey,BLENDFUNCTION *pblend,DWORD dwFlags){
      XASSERT(::IsWindow(m_hWnd))
      return ::UpdateLayeredWindow(m_hWnd,hdcDst,pptDst,psize,hdcSrc,pptSrc,crKey,pblend,dwFlags);
    }

    HRESULT SetWindowTheme(LPCWSTR pszSubAppName,LPCWSTR pszSubIdList){
      XASSERT(::IsWindow(m_hWnd))
      return ::SetWindowTheme(m_hWnd,pszSubAppName,pszSubIdList);
    }

  #endif //_WIN32_WINNT >= 0x501

    BOOL DisableWindowTheme();

    operator HWND(){
      return (this==NULL?NULL:m_hWnd);
    }

    XWnd &operator=(HWND hWnd){
      XASSERT(!m_bAttached)
      m_hWnd=hWnd;
      return *this;
    }

    BOOL operator==(const HWND hWnd)const{
      return (hWnd==m_hWnd);
    }

    BOOL operator!=(const HWND hWnd)const{
      return (hWnd!=m_hWnd);
    }

    BOOL CenterWindow(HWND pOwner=NULL);
    BOOL ClientResize(INT nWidth,INT nHeight);
    HWND GetDescendantWindow(INT nID,HWND hWnd=NULL);
    VOID SendMessageToDescendants(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL bDeep=FALSE,HWND hWnd=NULL);
    BOOL PreTranslateInput(LPMSG lpMsg);
    virtual BOOL CalcWindowRect(LPRECT lpClientRect,BOOL nAdjust=FALSE,BOOL bMenu=FALSE);
    BOOL RepositionControls(UINT nIDFirst,UINT nIDLast,UINT nIDOver,LPRECT lpRectParam=NULL,LPCRECT lpRectClient=NULL,BOOL bOnlyQuery=FALSE);
    VOID RepositionWindow(HDWP hDWP,HWND hWnd,LPCRECT lpRect);

  protected:
    //
    const MSG* GetCurrentMessage();
    INT_PTR DoModal();
    VOID EndModal(INT_PTR nResult);

    //Special methods...
    virtual BOOL PreTranslateMessage(LPMSG lpMsg);
    virtual BOOL PreCreateWindow(LPCREATESTRUCT wcs);
    virtual BOOL PreRegisterClass(LPWNDCLASSEX wcx);
    virtual VOID PreSubclassWindow(){}

    // For methods default returns...
    virtual LRESULT Default();
    virtual BOOL OnOleEvent(UINT nID,UINT nCode,XSINKEVENT* pEvent);
    virtual LRESULT OnNotify(UINT nCtrlId,LPNMHDR pnmh,PBOOL bProcessed=NULL);
    virtual LRESULT OnCommand(WPARAM wParam,LPARAM lParam,PBOOL bProcessed=NULL);
    virtual LRESULT OnWndMsg(UINT uMsg,WPARAM wParam,LPARAM lParam,PBOOL bProcessed=NULL);
    virtual LRESULT OnWindowlessMsg(UINT uMsg,WPARAM wParam,LPARAM lParam,PBOOL bProcessed=NULL);
    virtual LRESULT WindowProc(UINT uMsg,WPARAM wParam,LPARAM lParam);
    virtual LRESULT DefWindowProc(UINT uMsg,WPARAM wParam,LPARAM lParam);
    virtual LRESULT DefDlgProc(UINT uMsg,WPARAM wParam,LPARAM lParam);

    // static methods
    static LRESULT CALLBACK XWndProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
    static BOOL CALLBACK XDlgProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam);

    //to have access...
    friend class XWinApp;
  public:
    static LRESULT WINAPI CallWndProc(XWnd* pWnd, UINT nMsg,WPARAM wParam=0,LPARAM lParam=0);

  protected:
    //some initialization methods
    BOOL HookCreation();
    BOOL UnhookCreation();

    //Hook
    static LRESULT CALLBACK CbtFilterHook(INT nCode,WPARAM wParam,LPARAM lParam);

  protected:
    //attached window...
    HWND m_hWnd,m_hWndOwner;
    MSG m_cMsg;WNDPROC m_WndProc;
    BOOL m_bAttached;UINT m_nFlags;
    INT_PTR m_nModRes;

    //used for owner draw dialogs...
    CREATESTRUCT m_wcs;
    WNDCLASSEX m_wcx;

  private:
    //attaching static vars...
    static HHOOK m_hCbtFilter;
    static XWnd *m_pWndInit;

  protected:
    // For Ole Controls Support ( Container )
    // XWnd as generic control container... ( XOleControl as control site )
    XWnd *m_pWndParent;//(container frame)
    LPOLEOBJECT m_pObject;
    LPOLEINPLACEACTIVEOBJECT m_pActiveObject;
    IOleInPlaceObject *m_pInPlaceObject;
    IOleInPlaceObjectWindowless* m_pWindowlessObject;
    XObjectMap<XWnd*,HWND> m_XWndSiteMap;
    XWnd *m_pSiteUIActive,*m_pSiteCapture,*m_pSiteFocus;
    INT m_nWindowlessControls;BOOL m_bIsWindowless;
    UINT m_nCtrlId;RECT m_rect;

    virtual VOID OnUIActivate(XWnd* pSite);
    virtual VOID OnUIDeactivate(XWnd* pSite);
    virtual BOOL GetAmbientProperty(DISPID dispid,VARIANT* pvar);

    BOOL SetOleFont(HFONT hFont);
    LPUNKNOWN GetControlUnknown(){return m_pObject;}
    XWnd *GetContainer()const{return m_pWndParent;}

    struct _CtrlAppearance{
      OLE_COLOR colorFore;
      OLE_COLOR colorBack;
      LPFONTDISP pFont;
      DWORD     dwAppearance;
      LONG      lcid;
      HPALETTE  hpal;
    }CtrlAppearance;

    typedef struct {DISPID dwDispID;DWORD dwFlag;}AmbientsFlagsStruct;
    static const AmbientsFlagsStruct AmbientsFlags[8];

  public:
    //for ole objects
    virtual DWORD QueryInterface(const VOID *iid,LPVOID* ppvObj);

  protected:
    //Interfaces...
    BEGIN_INTERNAL_INTERFACE(XWnd,OleIPFrame,IOleInPlaceFrame)
      STDMETHOD(GetWindow)(HWND*);
      STDMETHOD(ContextSensitiveHelp)(BOOL);
      STDMETHOD(GetBorder)(LPRECT);
      STDMETHOD(RequestBorderSpace)(LPCBORDERWIDTHS);
      STDMETHOD(SetBorderSpace)(LPCBORDERWIDTHS);
      STDMETHOD(SetActiveObject)(LPOLEINPLACEACTIVEOBJECT, LPCOLESTR);
      STDMETHOD(InsertMenus)(HMENU, LPOLEMENUGROUPWIDTHS);
      STDMETHOD(SetMenu)(HMENU, HOLEMENU, HWND);
      STDMETHOD(RemoveMenus)(HMENU);
      STDMETHOD(SetStatusText)(LPCOLESTR);
      STDMETHOD(EnableModeless)(BOOL);
      STDMETHOD(TranslateAccelerator)(LPMSG, WORD);
    END_INTERNAL_INTERFACE(XWnd,OleIPFrame)

    BEGIN_INTERNAL_INTERFACE(XWnd,OleContainer,IOleContainer)
      STDMETHOD(ParseDisplayName)(LPBINDCTX, LPOLESTR, ULONG*, LPMONIKER*);
      STDMETHOD(EnumObjects)(DWORD, LPENUMUNKNOWN*);
      STDMETHOD(LockContainer)(BOOL);
    END_INTERNAL_INTERFACE(XWnd,OleContainer)

    DECLARE_INTERFACE_EVENT_TABLE()

  friend class XSkinnedWnd;
  friend class XOleControl;

  DECLARE_EVENT_TABLE()
};

typedef class XWnd XWND,*PXWND;

#endif //_XWND_H_
