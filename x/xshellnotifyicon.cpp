#include "xshellnotifyicon.h"
#include "xutils.h"

IMPLEMENT_DYNAMIC(XShellNotifyIcon,XObject)

XShellNotifyIcon::XShellNotifyIcon():m_bCreated(FALSE){
  ZeroMemory(&m_NotifyIconData,sizeof(m_NotifyIconData));
  m_NotifyIconData.cbSize=sizeof(m_NotifyIconData);
}

XShellNotifyIcon::~XShellNotifyIcon(){
  if(m_bCreated)
    Delete();
}

BOOL XShellNotifyIcon::Add(HWND hWnd,UINT uID,HICON hIcon,LPCTSTR szTip,UINT uCBMsg,BOOL bShowTip){
  XASSERT(!m_bCreated)
  XASSERT(hIcon!=NULL&&(!szTip||(szTip&&lstrlen(szTip)<=MAX_NIF_TIP_TEXT)))
  if(m_bCreated)
    return FALSE;
  if(hIcon==NULL||(szTip&&lstrlen(szTip)>MAX_NIF_TIP_TEXT))
    return FALSE;
  m_NotifyIconData.hWnd=hWnd;
  m_NotifyIconData.uID=uID;
  m_NotifyIconData.uFlags=NIF_MESSAGE|NIF_ICON;
  m_NotifyIconData.uCallbackMessage=(uCBMsg?uCBMsg:WM_XNOTIFYICON);
  m_NotifyIconData.hIcon=hIcon;
  if(szTip){
    m_NotifyIconData.uFlags|=NIF_TIP;
    lstrcpy(m_NotifyIconData.szTip,szTip);
  }
  #if (_WIN32_WINNT >= 0x0600)
    if(bShowTip)
      m_NotifyIconData.uFlags|=NIF_SHOWTIP;
  #else
    XUNUSED_ALWAYS(bShowTip)
  #endif
  return (m_bCreated=Shell_NotifyIcon(NIM_ADD,&m_NotifyIconData));
}

#if (_WIN32_WINNT >= 0x0601)
BOOL XShellNotifyIcon::Add(HWND hWnd,GUID guidItem,HICON hIcon,LPCTSTR szTip,UINT uCBMsg,BOOL bShowTip){
  XASSERT(!m_bCreated)
  XASSERT(hIcon!=NULL&&(!szTip||(szTip&&lstrlen(szTip)<=MAX_NIF_TIP_TEXT)))
  if(m_bCreated)
    return FALSE;
  if(hIcon==NULL||(szTip&&lstrlen(szTip)>MAX_NIF_TIP_TEXT))
    return FALSE;
  m_NotifyIconData.hWnd=hWnd;
  m_NotifyIconData.guidItem=guidItem;
  m_NotifyIconData.uFlags=NIF_GUID|NIF_MESSAGE|NIF_ICON;
  m_NotifyIconData.uCallbackMessage=(uCBMsg?uCBMsg:WM_XNOTIFYICON);
  m_NotifyIconData.hIcon=hIcon;
  if(szTip){
    m_NotifyIconData.uFlags|=NIF_TIP;
    lstrcpy(m_NotifyIconData.szTip,szTip);
  }
  m_bCreated=Shell_NotifyIcon(NIM_ADD,&m_NotifyIconData);
  #if (_WIN32_WINNT >= 0x0600)
    if(bShowTip)
      m_NotifyIconData.uFlags|=NIF_SHOWTIP;
  #endif
  return m_bCreated;
}
#endif

#if (_WIN32_WINNT >= 0x0500)
BOOL XShellNotifyIcon::Hide(){
  XASSERT(m_bCreated)
  if(!m_bCreated)
    return FALSE;
  m_NotifyIconData.uFlags=NIF_STATE;
  m_NotifyIconData.dwState=NIS_HIDDEN;
  m_NotifyIconData.dwStateMask=NIS_HIDDEN;
  return Shell_NotifyIcon(NIM_MODIFY,&m_NotifyIconData);
}

BOOL XShellNotifyIcon::Show(){
  XASSERT(m_bCreated)
  if(!m_bCreated)
    return FALSE;
  m_NotifyIconData.uFlags=NIF_STATE;
  m_NotifyIconData.dwState=0;
  m_NotifyIconData.dwStateMask=NIS_HIDDEN;
  return Shell_NotifyIcon(NIM_MODIFY,&m_NotifyIconData);
}

BOOL XShellNotifyIcon::ShowBalloon(LPCTSTR szInfo,LPCTSTR szInfoTitle,DWORD dwInfoFlags,UINT uTimeout,HICON hBalloonIcon,BOOL bRealtime){
  XASSERT(m_bCreated)
  XASSERT(szInfo&&szInfoTitle&&dwInfoFlags&&uTimeout)
  XASSERT(lstrlen(szInfoTitle)<=MAX_NIF_INFO_TITLE)
  XASSERT(lstrlen(szInfo)<=MAX_NIF_INFO_TEXT)
  if(!m_bCreated)
    return FALSE;
  if(!szInfoTitle||lstrlen(szInfoTitle)>MAX_NIF_INFO_TITLE)
    return FALSE;
  if(!szInfo||lstrlen(szInfo)>MAX_NIF_INFO_TEXT)
    return FALSE;
  m_NotifyIconData.uFlags=NIF_INFO;
  m_NotifyIconData.dwInfoFlags=dwInfoFlags;
  m_NotifyIconData.uTimeout=uTimeout;
  lstrcpy(m_NotifyIconData.szInfoTitle,szInfoTitle);
  lstrcpy(m_NotifyIconData.szInfo,szInfo);
  #if (_WIN32_WINNT >= 0x0600)
    if(hBalloonIcon)
      m_NotifyIconData.hBalloonIcon=hBalloonIcon;
    if(bRealtime)
      m_NotifyIconData.uFlags|=NIF_REALTIME;
  #else
    XUNUSED_ALWAYS(hBalloonIcon)
    XUNUSED_ALWAYS(bRealtime)
  #endif
  return Shell_NotifyIcon(NIM_MODIFY,&m_NotifyIconData);
}
#endif

BOOL XShellNotifyIcon::SetTooltipText(LPCTSTR szTip){
  XASSERT(m_bCreated)
  XASSERT(szTip&&lstrlen(szTip)<=MAX_NIF_TIP_TEXT)
  if(!m_bCreated)
    return FALSE;
  if(!szTip||lstrlen(szTip)>MAX_NIF_TIP_TEXT)
    return FALSE;
  m_NotifyIconData.uFlags=NIF_TIP;
  lstrcpy(m_NotifyIconData.szTip,szTip);
  return Shell_NotifyIcon(NIM_MODIFY,&m_NotifyIconData);
}

BOOL XShellNotifyIcon::SetTooltipText(HINSTANCE hInstance,LPCTSTR lpszName){
  XASSERT(m_bCreated&&lpszName)
  if(!m_bCreated||!lpszName)
    return FALSE;
  LPCTSTR text=LoadStringStatic(hInstance,lpszName);
  return (text&&SetTooltipText(text));
}

BOOL XShellNotifyIcon::SetIcon(HICON hIcon){
  XASSERT(m_bCreated&&hIcon)
  if(!m_bCreated||!hIcon)
    return FALSE;
  m_NotifyIconData.uFlags=NIF_ICON;
  m_NotifyIconData.hIcon=hIcon;
  return Shell_NotifyIcon(NIM_MODIFY,&m_NotifyIconData);
}

BOOL XShellNotifyIcon::SetIcon(HINSTANCE hInstance,LPCTSTR lpIconName){
  XASSERT(m_bCreated&&lpIconName)
  if(!m_bCreated||!lpIconName)
    return FALSE;
  HICON hIcon=::LoadIcon(hInstance,lpIconName);
  return (hIcon&&SetIcon(hIcon));
}

#if (_WIN32_WINNT >= 0x0500)
BOOL XShellNotifyIcon::SetVersion(UINT uVersion){
  XASSERT(m_bCreated)
  if(!m_bCreated)
    return FALSE;
  m_NotifyIconData.uVersion=uVersion;
  return Shell_NotifyIcon(NIM_SETVERSION,&m_NotifyIconData);
}
#endif

BOOL XShellNotifyIcon::SetFocus(){
  XASSERT(m_bCreated)
  if(!m_bCreated)
    return FALSE;
  return Shell_NotifyIcon(NIM_SETFOCUS,&m_NotifyIconData);
}

#if (_WIN32_WINNT >= 0x0601)
BOOL XShellNotifyIcon::GetRect(LPRECT ir){
  XASSERT(m_bCreated)
  if(!m_bCreated)
    return FALSE;
  NOTIFYICONIDENTIFIER notid;
  notid.cbSize=sizeof(notid);
  notid.hWnd=m_NotifyIconData.hWnd;
  notid.uID=m_NotifyIconData.uID;
  notid.guidItem=m_NotifyIconData.guidItem;
  return (Shell_NotifyIconGetRect(&notid,ir)==S_OK);
}
#endif

BOOL XShellNotifyIcon::Delete(){
  XASSERT(m_bCreated)
  if(!m_bCreated)
    return FALSE;
  m_bCreated=FALSE;
  m_NotifyIconData.uFlags=0;
  return Shell_NotifyIcon(NIM_DELETE,&m_NotifyIconData);
}

