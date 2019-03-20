#ifndef _XSHELLNOTIFYICON_H_
#define _XSHELLNOTIFYICON_H_

#include "xobject.h"
#include "xevent.h"

#if (_WIN32_WINNT >= 0x0500)
  #define MAX_NIF_TIP_TEXT      127 // 1 null character...
#else
  #define MAX_NIF_TIP_TEXT      63 // 1 null character...
#endif

#define MAX_NIF_INFO_TITLE      63 // 1 null character...
#define MAX_NIF_INFO_TEXT       255 // 1 null character...

#define WM_XNOTIFYICON          WM_USER+0x0100 //A Default Notify Icon MSG
#define EVENT_XNOTIFYICON(pfun) \
	EVENT_MESSAGE_EX(WM_XNOTIFYICON,TVWL,(VOID(XEvent::*)(UINT,UINT))(&pfun))

class XShellNotifyIcon:public XObject{
  DECLARE_DYNAMIC(XShellNotifyIcon)
  public:

    XShellNotifyIcon();
    ~XShellNotifyIcon();

    BOOL Add(HWND hWnd,UINT uID,HICON hIcon,LPCTSTR szTip=NULL,UINT uCBMsg=0,BOOL bShowTip=FALSE);

  #if (_WIN32_WINNT >= 0x0601)
    BOOL Add(HWND hWnd,GUID guidItem,HICON hIcon,LPCTSTR szTip=NULL,UINT uCBMsg=0,BOOL bShowTip=FALSE);
  #endif

  #if (_WIN32_WINNT >= 0x0500)
    BOOL Hide();

    BOOL Show();

    BOOL ShowBalloon(LPCTSTR szInfo,LPCTSTR szInfoTitle,DWORD dwInfoFlags,UINT uTimeout,HICON hBalloonIcon=NULL,BOOL bRealtime=FALSE);
  #endif

    BOOL SetTooltipText(LPCTSTR szTip);

    BOOL SetTooltipText(HINSTANCE hInstance,LPCTSTR lpszName);

    BOOL SetIcon(HICON hIcon);

    BOOL SetIcon(HINSTANCE hInstance,LPCTSTR lpIconName);

  #if (_WIN32_WINNT >= 0x0500)
    BOOL SetVersion(UINT uVersion);
  #endif

    BOOL SetFocus();

  #if (_WIN32_WINNT >= 0x0601)
    BOOL GetRect(LPRECT ir);
  #endif

    BOOL Delete();

    BOOL IsValid(){return m_bCreated;}

  private:
    NOTIFYICONDATA m_NotifyIconData;
    BOOL m_bCreated;
};

#endif //_XSHELLNOTIFYICON_H_
