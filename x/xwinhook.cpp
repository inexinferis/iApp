#include "xwinhook.h"

/**************************************************************/
// XWinHook...
/**************************************************************/

IMPLEMENT_DYNAMIC(XWindowHook,XObject)

XWindowHook::XHOOKDATA XWindowHook::m_XHookMap[]={
  {NULL,&XWindowHook::XJournalRecordProc,std::list<XWindowHook*>()},    // 0 -> WH_JOURNALRECORD
  {NULL,&XWindowHook::XJournalPlaybackProc,std::list<XWindowHook*>()},  // 1 -> WH_JOURNALPLAYBACK
  {NULL,&XWindowHook::XKeyboardProc,std::list<XWindowHook*>()},         // 2 -> WH_KEYBOARD
  {NULL,&XWindowHook::XGetMsgProc,std::list<XWindowHook*>()},           // 3 -> WH_GETMESSAGE
  {NULL,&XWindowHook::XCallWndProc,std::list<XWindowHook*>()},          // 4 -> WH_CALLWNDPROC
  {NULL,&XWindowHook::XCBTProc,std::list<XWindowHook*>()},              // 5 -> WH_CBT
  {NULL,&XWindowHook::XSysMsgProc,std::list<XWindowHook*>()},           // 6 -> WH_SYSMSGFILTER
  {NULL,&XWindowHook::XMouseProc,std::list<XWindowHook*>()},            // 7 -> WH_MOUSE
  {NULL,&XWindowHook::XHardwareProc,std::list<XWindowHook*>()},         // 8 -> WH_HARDWARE
  {NULL,&XWindowHook::XDebugProc,std::list<XWindowHook*>()},            // 9 -> WH_DEBUG
  {NULL,&XWindowHook::XShellProc,std::list<XWindowHook*>()},            // 10 -> WH_SHELL
  {NULL,&XWindowHook::XForegroundIdleProc,std::list<XWindowHook*>()},   // 11 -> WH_FOREGROUNDIDLE
  {NULL,&XWindowHook::XCallWndRetProc,std::list<XWindowHook*>()},       // 12 -> WH_CALLWNDPROCRET
  {NULL,&XWindowHook::XLowLevelKeyboardProc,std::list<XWindowHook*>()}, // 13 -> WH_KEYBOARD_LL
  {NULL,&XWindowHook::XLowLevelMouseProc,std::list<XWindowHook*>()},    // 14 -> WH_MOUSE_LL
  {NULL,&XWindowHook::XMessageProc,std::list<XWindowHook*>()}           // 15 / (-1) -> WH_MSGFILTER
};

BOOL XWindowHook::SetHook(INT idHook,XObject *HookObj,XHOOKPROC lpfn){
  XASSERT(m_idHook==-1&&idHook>-2&&idHook<15)
  XASSERT(HookObj!=NULL&&lpfn!=NULL)
  if(m_idHook==-1&&idHook>-2&&idHook<15&&HookObj!=NULL&&lpfn!=NULL){
    m_idHook=idHook;
    if(m_idHook==-1)m_idHook=15;
    if(!m_XHookMap[m_idHook].hHook)
      m_XHookMap[m_idHook].hHook=::SetWindowsHookEx(idHook,m_XHookMap[m_idHook].hProc,NULL,GetCurrentThreadId());
    if(m_XHookMap[m_idHook].hHook){
      m_HookProc=lpfn;m_HookObj=(XWindowHook*)HookObj;
      m_XHookMap[m_idHook].lObjs.push_back(this);
      return TRUE;
    }
    m_idHook=-1;
  }
  return FALSE;
}

BOOL XWindowHook::Unhook(){
  BOOL bRet=FALSE;
  if(m_idHook>-1&&m_idHook<16){
    m_XHookMap[m_idHook].lObjs.remove(this);
    if(m_XHookMap[m_idHook].lObjs.empty()&&m_XHookMap[m_idHook].hHook){
      bRet=::UnhookWindowsHookEx(m_XHookMap[m_idHook].hHook);
      m_XHookMap[m_idHook].hHook=NULL;
    }
    m_idHook=-1;
  }
  return bRet;
}

LRESULT CALLBACK XWindowHook::XHookProc(INT idHook,INT nCode,WPARAM wParam,LPARAM lParam){
  if(idHook==-1)idHook=15;
  if(nCode<0)return CallNextHookEx(m_XHookMap[idHook].hHook,nCode,wParam,lParam);
  std::list<XWindowHook*>::iterator it;
  for(it=m_XHookMap[idHook].lObjs.begin();it!=m_XHookMap[idHook].lObjs.end();it++){
    XWindowHook* pEntry=*it;
    if(pEntry&&pEntry->m_HookObj&&pEntry->m_HookProc){
      BOOL bEat=FALSE;
      LRESULT lRet=((pEntry->m_HookObj)->*(pEntry->m_HookProc))(nCode,wParam,lParam,&bEat);
      if(bEat)return lRet;
    }
  }
  return CallNextHookEx(m_XHookMap[idHook].hHook,nCode,wParam,lParam);
}

LRESULT XWindowHook::HookProc(INT /*nCode*/,WPARAM /*wParam*/,LPARAM /*lParam*/,PBOOL /*bEatIt*/){
  return 0;
}
