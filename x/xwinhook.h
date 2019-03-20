#ifndef _XWINHOOK_H_
#define _XWINHOOK_H_

#include "xobject.h"

class XWindowHook;
typedef LRESULT (XWindowHook::*XHOOKPROC)(INT nCode,WPARAM wParam,LPARAM lParam,PBOOL bEatIt);

class XWindowHook:public XObject{
  DECLARE_DYNAMIC(XWindowHook)
  public:
    XWindowHook():m_idHook(-1){
      m_HookProc=&XWindowHook::HookProc;
      m_HookObj=this;
    }

    virtual ~XWindowHook(){
      if(m_idHook!=-1)
        Unhook();
    }

    BOOL SetHook(INT idHook){
      return SetHook(idHook,this,&XWindowHook::HookProc);
    }

    BOOL SetHook(INT idHook,XObject *HookObj,XHOOKPROC lpfn);
    BOOL Unhook();

  protected:

    virtual LRESULT HookProc(INT nCode,WPARAM wParam,LPARAM lParam,PBOOL bEatIt);

  private:
    INT m_idHook;
    XHOOKPROC m_HookProc;
    XWindowHook *m_HookObj;

    static struct XHOOKDATA{
      HHOOK hHook;
      HOOKPROC hProc;
      std::list<XWindowHook*>lObjs;
      //XHOOKDATA(HHOOK nhHook,HOOKPROC nhProc):hHook(nhHook),hProc(nhProc){}
    }m_XHookMap[];

    static LRESULT CALLBACK XHookProc(INT idHook,INT nCode,WPARAM wParam,LPARAM lParam);

    //Internal message redirection...
    static LRESULT CALLBACK XJournalRecordProc(INT nCode,WPARAM wParam,LPARAM lParam){
      return XHookProc(WH_JOURNALRECORD,nCode,wParam,lParam);
    }

    static LRESULT CALLBACK XJournalPlaybackProc(INT nCode, WPARAM wParam, LPARAM lParam){
      return XHookProc(WH_JOURNALPLAYBACK,nCode,wParam,lParam);
    }

    static LRESULT CALLBACK XKeyboardProc(INT nCode, WPARAM wParam, LPARAM lParam){
      return XHookProc(WH_KEYBOARD,nCode,wParam,lParam);
    }

    static LRESULT CALLBACK XGetMsgProc(INT nCode, WPARAM wParam, LPARAM lParam){
      return XHookProc(WH_GETMESSAGE,nCode,wParam,lParam);
    }

    static LRESULT CALLBACK XCallWndProc(INT nCode, WPARAM wParam, LPARAM lParam){
      return XHookProc(WH_CALLWNDPROC,nCode,wParam,lParam);
    }

    static LRESULT CALLBACK XCBTProc(INT nCode, WPARAM wParam, LPARAM lParam){
      return XHookProc(WH_CBT,nCode,wParam,lParam);
    }

    static LRESULT CALLBACK XSysMsgProc(INT nCode, WPARAM wParam, LPARAM lParam){
      return XHookProc(WH_SYSMSGFILTER,nCode,wParam,lParam);
    }

    static LRESULT CALLBACK XMouseProc(INT nCode, WPARAM wParam, LPARAM lParam){
      return XHookProc(WH_MOUSE,nCode,wParam,lParam);
    }

    static LRESULT CALLBACK XHardwareProc(INT nCode, WPARAM wParam, LPARAM lParam){
      return XHookProc(WH_HARDWARE,nCode,wParam,lParam);
    }

    static LRESULT CALLBACK XDebugProc(INT nCode, WPARAM wParam, LPARAM lParam){
      return XHookProc(WH_DEBUG,nCode,wParam,lParam);
    }

    static LRESULT CALLBACK XShellProc(INT nCode, WPARAM wParam, LPARAM lParam){
      return XHookProc(WH_SHELL,nCode,wParam,lParam);
    }

    static LRESULT CALLBACK XForegroundIdleProc(INT nCode, WPARAM wParam, LPARAM lParam){
      return XHookProc(WH_FOREGROUNDIDLE,nCode,wParam,lParam);
    }

    static LRESULT CALLBACK XCallWndRetProc(INT nCode,WPARAM wParam,LPARAM lParam){
      return XHookProc(WH_CALLWNDPROCRET,nCode,wParam,lParam);
    }

    static LRESULT CALLBACK XLowLevelKeyboardProc(INT nCode, WPARAM wParam, LPARAM lParam){
      return XHookProc(WH_KEYBOARD_LL,nCode,wParam,lParam);
    }

    static LRESULT CALLBACK XLowLevelMouseProc(INT nCode, WPARAM wParam, LPARAM lParam){
      return XHookProc(WH_MOUSE_LL,nCode,wParam,lParam);
    }

    static LRESULT CALLBACK XMessageProc(INT nCode, WPARAM wParam, LPARAM lParam){
      return XHookProc(WH_MSGFILTER,nCode,wParam,lParam);
    }
};


#endif //_XWINHOOK_H_
