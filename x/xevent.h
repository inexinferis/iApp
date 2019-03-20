#ifndef _XEVENT_H_
#define _XEVENT_H_

#include "xobject.h"

#define XEVENTOBJECT _T("XEventObject")
#define XALLCTRLID   (UINT)-1
#define XTHISCTRLID  (UINT)-1

//type of parameters
enum{TIWL=1,TIL,TIW,TIV,TVWL,TVL,TVW,TVV};

#define WM_IDLEUPDATECMDUI    0x0360      // wParam == bDisableIfNoHandler
#define WM_SIZEPARENT         0x0361      // lParam = LPSIZEPARENTPARAMS
#define WM_RECALCPARENT       0x0368      // force RecalcLayout on frame window
#define WM_SIZECHILD          0x0369      // wParam = ID of child window,lParam = lpRectNew (new position/size)

#define WM_OLEEVENT                ((UINT)(-2))    // OLE event
#define WM_OLECOMMAND              ((UINT)(-3))    // OLE document command
#define WM_OLEUNREGISTER           ((UINT)(-4))    // OLE unregister

/*****************************************************/
// Interface Methods macros (use 'this' to avoid null pointer warning... )
/*****************************************************/

#define BEGIN_INTERNAL_INTERFACE(theClass,localClass,baseClass)                \
	class X##localClass:public baseClass{                                        \
    public:                                                                    \
      X##localClass(){                                                         \
        nOffset=(size_t)((BYTE*)&((theClass*)1)->m_##localClass-(BYTE *)1);    \
      }                                                                        \
      STDMETHOD_(ULONG,AddRef)();                                              \
      STDMETHOD_(ULONG,Release)();                                             \
      STDMETHOD(QueryInterface)(REFIID iid,LPVOID* ppvObj);

#define END_INTERNAL_INTERFACE(theClass,localClass)                            \
      theClass *GetParentPtr(){                                                \
        return (theClass *)((BYTE*)this-nOffset);                              \
      }                                                                        \
    private:                                                                   \
      size_t nOffset;                                                          \
	} m_##localClass;                                                            \
	friend class X##localClass;

#define METHOD_PROLOGUE(theClass,localClass)                                   \
	theClass* pThis=((theClass*)((BYTE*)this-((BYTE*)&((theClass*)1)->m_##localClass-(BYTE *)1)));\
	pThis;

/*****************************************************/
// Interface Map
/*****************************************************/

#define DECLARE_INTERFACE_EVENT_TABLE()                                        \
  private:                                                                     \
    static const XInterfaceEventTableEntry _ientries[];                        \
  protected:                                                                   \
    static const XInterfaceEventTable* GetThisInterfaceEventTable();           \
    virtual const XInterfaceEventTable* GetInterfaceEventTable()const{         \
      return GetThisInterfaceEventTable();                                     \
    };


#define BEGIN_INTERFACE_EVENTS(evntclass,baseclass)                            \
	const XInterfaceEventTable* evntclass::GetThisInterfaceEventTable(){         \
		static const XInterfaceEventTable _ietable={                               \
      baseclass::GetThisInterfaceEventTable,_ientries                          \
    };                                                                         \
    return &_ietable;                                                          \
  }                                                                            \
  const XInterfaceEventTableEntry evntclass::_ientries[]={


#define END_INTERFACE_EVENTS()                                                 \
		{NULL,(size_t)-1}                                                          \
	};

/*****************************************************/
// Interface Event table macro
/*****************************************************/

#define INTERFACE_EVENT(theClass,iid,localClass)                               \
		{(PVOID)&iid,(size_t)((BYTE*)&((theClass*)1)->m_##localClass-(BYTE *)1)},

#define GetInterfacePtr(pTarget,pEntry) \
	((LPUNKNOWN)((BYTE*)pTarget+pEntry->nOffset))

/*****************************************************/
// Event Sink Map
/*****************************************************/

#define DECLARE_EVENT_SINK_TABLE()                                             \
  private:                                                                     \
    static const XEventSinkTableEntry _sentries[];                             \
  protected:                                                                   \
    static const XEventSinkTable* GetThisEventSinkTable();                     \
    virtual const XEventSinkTable* GetEventSinkTable()const{                   \
      return GetThisEventSinkTable();                                          \
    };


#define BEGIN_SINK_EVENTS(evntclass,baseclass)                                 \
	const XEventSinkTable* evntclass::GetThisEventSinkTable(){                   \
		static const XEventSinkTable _setable={                                    \
      baseclass::GetThisEventSinkTable,_sentries                               \
    };                                                                         \
    return &_setable;                                                          \
  }                                                                            \
  const XEventSinkTableEntry evntclass::_sentries[]={


#define END_SINK_EVENTS()                                                      \
		{DISPID_UNKNOWN,VTS_NONE,VT_VOID,NULL,NULL,(size_t)-1,(UINT)-1,0}          \
	};

/*****************************************************/
// Event Sink table macros
/*****************************************************/

#define ON_EVENT(id,dispid,pfun,vtsParams)                                     \
	{dispid,vtsParams,VT_BOOL,tXCallFunc(&pfun),NULL,0,id,id},

#define ON_EVENT_RANGE(idFirst,idLast,dispid,pfun,vtsParams)                   \
	{dispid,vtsParams,VT_BOOL,tXCallFunc(&pfun),NULL,0,idFirst,idLast},

#define ON_PROPNOTIFY(id,dispid,pfun1,pfun2)                                   \
	{dispid,VTS_NONE,VT_VOID,tXCallFunc(&pfun1),tXCallFunc(&pfun2),1,id,id},

#define ON_PROPNOTIFY_RANGE(idFirst,idLast,dispid,pfun1,pfun2)                 \
	{dispid,VTS_NONE,VT_VOID,tXCallFunc(&pfun1),tXCallFunc(&pfun2),1,idFirst,idLast},

#define EVENT_PARAM(vtsParams) (BYTE*)(vtsParams)

/*****************************************************/
// Event table macros
/*****************************************************/

#define DECLARE_EVENT_TABLE()                                                  \
  private:                                                                     \
    static const XEventTableEntry _entries[];                                  \
  protected:                                                                   \
    static const XEventTable* GetThisEventTable();                             \
    virtual const XEventTable* GetEventTable()const{                           \
      return GetThisEventTable();                                              \
    }


#define BEGIN_EVENT_TABLE(evntclass,baseclass)                                 \
  const XEventTable* evntclass::GetThisEventTable(){                           \
    static const XEventTable _etable={                                         \
      baseclass::GetThisEventTable,_entries                                    \
    };                                                                         \
    return &_etable;                                                           \
  }                                                                            \
  const XEventTableEntry evntclass::_entries[]={


#define END_EVENT_TABLE()                                                      \
    {0,0,0,0,0,NULL,-1}                                                        \
  };

/*****************************************************/
// Event table macros
/*****************************************************/

#define FULL_EVENT_TABLE_ENTRY(evntcode,ctrlid,minctrlid,maxctrlid,funtype,pfun)\
  {(UINT)evntcode,(UINT)ctrlid,(UINT)minctrlid,(UINT)maxctrlid,funtype,tXCallFunc(pfun),-1},

#define RANGE_EVENT_TABLE_ENTRY(evntcode,minctrlid,maxctrlid,funtype,pfun)     \
  {(UINT)evntcode,0,(UINT)minctrlid,(UINT)maxctrlid,funtype,tXCallFunc(pfun),-1},

#define EVENT_TABLE_ENTRY(evntcode,ctrlid,funtype,pfun)                        \
  {(UINT)evntcode,0,(UINT)ctrlid,(UINT)ctrlid,funtype,tXCallFunc(pfun),-1},

#define SMALL_EVENT_TABLE_ENTRY(evntcode,funtype,pfun)                         \
  {(UINT)evntcode,0,(UINT)-1,(UINT)-1,funtype,tXCallFunc(pfun),-1},

/******************************************************/
// EVENT_MESSAGE
/******************************************************/

#define EVENT_MESSAGE(msg,pfun)                                                \
  SMALL_EVENT_TABLE_ENTRY(msg,TIWL,(INT(XEvent::*)(WPARAM,LPARAM))(&pfun))

#define EVENT_THREAD_MESSAGE(msg,pfun)                                         \
  SMALL_EVENT_TABLE_ENTRY(msg,TVWL,(VOID(XEvent::*)(WPARAM,LPARAM))(&pfun))

#define EVENT_MESSAGE_EX(msg,funtype,pfun)                                     \
	{msg,0,(UINT)-1,(UINT)-1,funtype,tXCallFunc(pfun),-1},

#define EVENT_THREAD_MESSAGE_EX(msg,funtype,pfun)                              \
	{msg,0,(UINT)-1,(UINT)-1,funtype,tXCallFunc(pfun),-1},

/******************************************************/
// EVENT_COMMAND
/******************************************************/

#define EVENT_COMMAND(pfun)                                                    \
  SMALL_EVENT_TABLE_ENTRY(WM_COMMAND,TVWL,(VOID(XEvent::*)(WPARAM,LPARAM))(&pfun))

#define EVENT_CODE_COMMAND(cmdcode,pfun)                                       \
  FULL_EVENT_TABLE_ENTRY(WM_COMMAND,cmdcode,(UINT)-1,(UINT)-1,TVWL,(VOID(XEvent::*)(WPARAM,LPARAM))(&pfun))

#define EVENT_CTRL_COMMAND(ctrlid,pfun)                                        \
  FULL_EVENT_TABLE_ENTRY(WM_COMMAND,0,ctrlid,ctrlid,TVWL,(VOID(XEvent::*)(WPARAM,LPARAM))(&pfun))

#define EVENT_RANGE_COMMAND(minctrlid,maxctrlid,pfun)                          \
  FULL_EVENT_TABLE_ENTRY(WM_COMMAND,0,minctrlid,maxctrlid,TVWL,(VOID(XEvent::*)(WPARAM,LPARAM))(&pfun))

/******************************************************/
// FOR CONTROLS
/******************************************************/

#define EVENT_CONTROL(cmdcode,ctrlid,pfun)                                     \
  FULL_EVENT_TABLE_ENTRY(WM_COMMAND,cmdcode,ctrlid,ctrlid,TVWL,(VOID(XEvent::*)(WPARAM,LPARAM))(&pfun))

#define EVENT_RANGE_CONTROL(cmdcode,minctrlid,maxctrlid,pfun)                  \
  FULL_EVENT_TABLE_ENTRY(WM_COMMAND,cmdcode,minctrlid,maxctrlid,TVWL,(VOID(XEvent::*)(WPARAM,LPARAM))(&pfun))

/******************************************************/
// EVENT_NOTIFY
/******************************************************/

#define EVENT_NOTIFY(pfun)                                                     \
  SMALL_EVENT_TABLE_ENTRY(WM_NOTIFY,TIL,(INT(XEvent::*)(NMHDR*))(&pfun))

#define EVENT_CODE_NOTIFY(evtcode,pfun)                                        \
  FULL_EVENT_TABLE_ENTRY(WM_NOTIFY,evtcode,(UINT)-1,(UINT)-1,TIL,(INT(XEvent::*)(NMHDR*))(&pfun))

#define EVENT_CTRL_NOTIFY(evtcode,ctrlid,pfun)                                 \
  FULL_EVENT_TABLE_ENTRY(WM_NOTIFY,evtcode,ctrlid,ctrlid,TIL,(INT(XEvent::*)(NMHDR*))(&pfun))

#define EVENT_RANGE_NOTIFY(evtcode,minctrlid,maxctrlid,pfun)                   \
  FULL_EVENT_TABLE_ENTRY(WM_NOTIFY,evtcode,minctrlid,maxctrlid,TIL,(INT(XEvent::*)(NMHDR*))(&pfun))

/*****************************************************/
// Event table EX macros (ignore reflect)
/*****************************************************/

#define FULL_EVENT_TABLE_ENTRY_EX(evntcode,ctrlid,minctrlid,maxctrlid,funtype,pfun,bforce)\
  {evntcode,ctrlid,minctrlid,maxctrlid,funtype,tXCallFunc(pfun),bforce},

#define RANGE_EVENT_TABLE_ENTRY_EX(evntcode,minctrlid,maxctrlid,funtype,pfun,bforce)\
  {evntcode,0,minctrlid,maxctrlid,funtype,tXCallFunc(pfun),bforce},

#define EVENT_TABLE_ENTRY_EX(evntcode,ctrlid,funtype,pfun,bforce)              \
  {evntcode,0,ctrlid,ctrlid,funtype,tXCallFunc(pfun),bforce},

#define SMALL_EVENT_TABLE_ENTRY_EX(evntcode,funtype,pfun,bforce)               \
  {evntcode,0,(UINT)-1,(UINT)-1,funtype,tXCallFunc(pfun),bforce},

/******************************************************/
// EVENT_COMMAND_EX
/******************************************************/

#define EVENT_COMMAND_EX(pfun,bforce)                                          \
  SMALL_EVENT_TABLE_ENTRY_EX(WM_COMMAND,TVWL,(VOID(XEvent::*)(WPARAM,LPARAM))(&pfun),bforce)

#define EVENT_CODE_COMMAND_EX(cmdcode,pfun,bforce)                             \
  FULL_EVENT_TABLE_ENTRY(WM_COMMAND,cmdcode,(UINT)-1,(UINT)-1,TVWL,(VOID(XEvent::*)(WPARAM,LPARAM))(&pfun),bforce)

#define EVENT_CTRL_COMMAND_EX(ctrlid,pfun,bforce)                              \
  FULL_EVENT_TABLE_ENTRY(WM_COMMAND,0,ctrlid,ctrlid,TVWL,(VOID(XEvent::*)(WPARAM,LPARAM))(&pfun),bforce)

#define EVENT_RANGE_COMMAND_EX(minctrlid,maxctrlid,pfun,bforce)                \
  FULL_EVENT_TABLE_ENTRY_EX(WM_COMMAND,0,minctrlid,maxctrlid,TVWL,(VOID(XEvent::*)(WPARAM,LPARAM))(&pfun),bforce)

/******************************************************/
// FOR CONTROLS_EX
/******************************************************/

#define EVENT_CONTROL_EX(cmdcode,ctrlid,pfun,bforce)                           \
  FULL_EVENT_TABLE_ENTRY_EX(WM_COMMAND,cmdcode,ctrlid,ctrlid,TVWL,(VOID(XEvent::*)(WPARAM,LPARAM))(&pfun),bforce)

#define EVENT_RANGE_CONTROL_EX(cmdcode,minctrlid,maxctrlid,pfun,bforce)        \
  FULL_EVENT_TABLE_ENTRY_EX(WM_COMMAND,cmdcode,minctrlid,maxctrlid,TVWL,(VOID(XEvent::*)(WPARAM,LPARAM))(&pfun),bforce)

/******************************************************/
// EVENT_NOTIFY_EX
/******************************************************/

#define EVENT_NOTIFY_EX(pfun,bforce)                                           \
  SMALL_EVENT_TABLE_ENTRY_EX(WM_NOTIFY,TIL,(INT(XEvent::*)(NMHDR*))(&pfun),bforce)

#define EVENT_CODE_NOTIFY_EX(evtcode,pfun,bforce)                              \
  FULL_EVENT_TABLE_ENTRY_EX(WM_NOTIFY,evtcode,(UINT)-1,(UINT)-1,TIL,(INT(XEvent::*)(NMHDR*))(&pfun),bforce)

#define EVENT_CTRL_NOTIFY_EX(evtcode,ctrlid,pfun,bforce)                       \
  FULL_EVENT_TABLE_ENTRY_EX(WM_NOTIFY,evtcode,ctrlid,ctrlid,TIL,(INT(XEvent::*)(NMHDR*))(&pfun),bforce)

#define EVENT_RANGE_NOTIFY_EX(evtcode,minctrlid,maxctrlid,pfun,bforce)         \
  FULL_EVENT_TABLE_ENTRY_EX(WM_NOTIFY,evtcode,minctrlid,maxctrlid,TIL,(INT(XEvent::*)(NMHDR*))(&pfun),bforce)

/******************************************************/
// Miscellaneous
/******************************************************/

#define EVENT_CREATE(pfun)\
  {WM_CREATE,0,(UINT)-1,(UINT)-1,TIL,tXCallFunc((BOOL(XEvent::*)(LPCREATESTRUCT))(&pfun)),-1},

#define EVENT_DESTROY(pfun)\
  {WM_DESTROY,0,(UINT)-1,(UINT)-1,TVV,tXCallFunc((VOID(XEvent::*)(VOID))(&pfun)),-1},

#define EVENT_MOVE(pfun)\
  {WM_MOVE,0,(UINT)-1,(UINT)-1,TVL,tXCallFunc((VOID(XEvent::*)(POINTS))(&pfun)),-1},

#define EVENT_SIZE(pfun)\
  {WM_SIZE,0,(UINT)-1,(UINT)-1,TVWL,tXCallFunc((VOID(XEvent::*)(UINT,POINTS))(&pfun)),-1},

#define EVENT_ACTIVATE(pfun)\
  {WM_ACTIVATE,0,(UINT)-1,(UINT)-1,TVWL,tXCallFunc((VOID(XEvent::*)(UINT,HWND))(&pfun)),-1},

#define EVENT_SETFOCUS(pfun)\
  {WM_SETFOCUS,0,(UINT)-1,(UINT)-1,TVW,tXCallFunc((VOID(XEvent::*)(HWND))(&pfun)),-1},

#define EVENT_KILLFOCUS(pfun)\
  {WM_KILLFOCUS,0,(UINT)-1,(UINT)-1,TVW,tXCallFunc((VOID(XEvent::*)(HWND))(&pfun)),-1},

#define EVENT_ENABLE(pfun)\
  {WM_ENABLE,0,(UINT)-1,(UINT)-1,TVW,tXCallFunc((VOID(XEvent::*)(BOOL))(&pfun)),-1},

#define EVENT_SETREDRAW(pfun)\
  {WM_SETREDRAW,0,(UINT)-1,(UINT)-1,TVW,tXCallFunc((VOID(XEvent::*)(BOOL))(&pfun)),-1},

#define EVENT_SETTEXT(pfun)\
  {WM_SETTEXT,0,(UINT)-1,(UINT)-1,TIL,tXCallFunc((BOOL(XEvent::*)(LPSTR))(&pfun)),-1},

#define EVENT_GETTEXT(pfun)\
  {WM_GETTEXT,0,(UINT)-1,(UINT)-1,TIWL,tXCallFunc((INT(XEvent::*)(INT,LPSTR))(&pfun)),-1},

#define EVENT_GETTEXTLENGTH(pfun)\
  {WM_GETTEXTLENGTH,0,(UINT)-1,(UINT)-1,TIV,tXCallFunc((INT(XEvent::*)(VOID))(&pfun)),-1},

#define EVENT_PAINT(pfun)\
  {WM_PAINT,0,(UINT)-1,(UINT)-1,TVV,tXCallFunc((VOID(XEvent::*)(VOID))(&pfun)),-1},

#define EVENT_CLOSE(pfun)\
  {WM_CLOSE,0,(UINT)-1,(UINT)-1,TVV,tXCallFunc((VOID(XEvent::*)(VOID))(&pfun)),-1},

#define EVENT_QUERYENDSESSION(pfun)\
  {WM_QUERYENDSESSION,0,(UINT)-1,(UINT)-1,TIL,tXCallFunc((BOOL(XEvent::*)(UINT))(&pfun)),-1},

#define EVENT_QUERYOPEN(pfun)\
  {WM_QUERYOPEN,0,(UINT)-1,(UINT)-1,TIV,tXCallFunc((BOOL(XEvent::*)(VOID))(&pfun)),-1},

#define EVENT_ERASEBKGND(pfun)\
  {WM_ERASEBKGND,0,(UINT)-1,(UINT)-1,TIW,tXCallFunc((BOOL(XEvent::*)(HDC))(&pfun)),-1},

#define EVENT_SYSCOLORCHANGE(pfun)\
  {WM_SYSCOLORCHANGE,0,(UINT)-1,(UINT)-1,TVV,tXCallFunc((VOID(XEvent::*)(VOID))(&pfun)),-1},

#define EVENT_ENDSESSION(pfun)\
  {WM_ENDSESSION,0,(UINT)-1,(UINT)-1,TVWL,tXCallFunc((VOID(XEvent::*)(BOOL,UINT))(&pfun)),-1},

#define EVENT_SHOWWINDOW(pfun)\
  {WM_SHOWWINDOW,0,(UINT)-1,(UINT)-1,TVWL,tXCallFunc((VOID(XEvent::*)(BOOL,UINT))(&pfun)),-1},

#define EVENT_CTLCOLOR(pfun)\
  {WM_CTLCOLOR,0,(UINT)-1,(UINT)-1,TIWL,tXCallFunc((HBRUSH(XEvent::*)(HDC,HWND))(&pfun)),-1},

#define EVENT_WININICHANGE(pfun)\
  {WM_WININICHANGE,0,(UINT)-1,(UINT)-1,TVL,tXCallFunc((VOID(XEvent::*)(LPCTSTR))(&pfun)),-1},

#define EVENT_SETTINGCHANGE(pfun)\
  {WM_SETTINGCHANGE,0,(UINT)-1,(UINT)-1,TVWL,tXCallFunc((VOID(XEvent::*)(UINT,LPCTSTR))(&pfun)),-1},

#define EVENT_DEVMODECHANGE(pfun)\
  {WM_DEVMODECHANGE,0,(UINT)-1,(UINT)-1,TVL,tXCallFunc((VOID(XEvent::*)(LPTSTR))(&pfun)),-1},

#define EVENT_ACTIVATEAPP(pfun)\
  {WM_ACTIVATEAPP,0,(UINT)-1,(UINT)-1,TVWL,tXCallFunc((VOID(XEvent::*)(BOOL,DWORD))(&pfun)),-1},

#define EVENT_FONTCHANGE(pfun)\
  {WM_FONTCHANGE,0,(UINT)-1,(UINT)-1,TVV,tXCallFunc((VOID(XEvent::*)(VOID))(&pfun)),-1},

#define EVENT_TIMECHANGE(pfun)\
  {WM_TIMECHANGE,0,(UINT)-1,(UINT)-1,TVV,tXCallFunc((VOID(XEvent::*)(VOID))(&pfun)),-1},

#define EVENT_CANCELMODE(pfun)\
  {WM_CANCELMODE,0,(UINT)-1,(UINT)-1,TVV,tXCallFunc((VOID(XEvent::*)(VOID))(&pfun)),-1},

#define EVENT_SETCURSOR(pfun)\
  {WM_SETCURSOR,0,(UINT)-1,(UINT)-1,TIWL,tXCallFunc((BOOL(XEvent::*)(HWND,DWORD))(&pfun)),-1},

#define EVENT_MOUSEACTIVATE(func)\
  {WM_MOUSEACTIVATE,0,(UINT)-1,(UINT)-1,TIWL,tXCallFunc((INT(XEvent::*)(HWND,DWORD))(&pfun)),-1},

#define EVENT_CHILDACTIVATE(func)\
  {WM_CHILDACTIVATE,0,(UINT)-1,(UINT)-1,TVV,tXCallFunc((VOID(XEvent::*)(VOID))(&pfun)),-1},

#define EVENT_QUEUESYNC(func)\
  {WM_QUEUESYNC,0,(UINT)-1,(UINT)-1,TVV,tXCallFunc((VOID(XEvent::*)(VOID))(&pfun)),-1},

#define EVENT_GETMINMAXINFO(pfun)\
  {WM_GETMINMAXINFO,0,(UINT)-1,(UINT)-1,TVL,tXCallFunc((VOID(XEvent::*)(PMINMAXINFO))(&pfun)),-1},

#define EVENT_PAINTICON(func)\
  {WM_PAINTICON,0,(UINT)-1,(UINT)-1,TVV,tXCallFunc((VOID(XEvent::*)(VOID))(&pfun)),-1},

#define EVENT_ICONERASEBKGND(pfun)\
  {WM_ICONERASEBKGND,0,(UINT)-1,(UINT)-1,TVW,tXCallFunc((VOID(XEvent::*)(HDC))(&pfun)),-1},

#define EVENT_NEXTDLGCTL(func)\
  {WM_NEXTDLGCTL,0,(UINT)-1,(UINT)-1,TVV,tXCallFunc((VOID(XEvent::*)(VOID))(&pfun)),-1},

#define EVENT_SPOOLERSTATUS(pfun)\
  {WM_SPOOLERSTATUS,0,(UINT)-1,(UINT)-1,TVWL,tXCallFunc((VOID(XEvent::*)(UINT,UINT))(&pfun)),-1},

#define EVENT_DRAWITEM(pfun)\
  {WM_DRAWITEM,0,(UINT)-1,(UINT)-1,TIWL,tXCallFunc((BOOL(XEvent::*)(INT,LPDRAWITEMSTRUCT))(&pfun)),-1},

#define EVENT_MEASUREITEM(pfun)\
  {WM_MEASUREITEM,0,(UINT)-1,(UINT)-1,TIWL,tXCallFunc((BOOL(XEvent::*)(INT,LPMEASUREITEMSTRUCT))(&pfun)),-1},

#define EVENT_DELETEITEM(pfun)\
  {WM_DELETEITEM,0,(UINT)-1,(UINT)-1,TIWL,tXCallFunc((BOOL(XEvent::*)(INT,LPDELETEITEMSTRUCT))(&pfun)),-1},

#define EVENT_VKEYTOITEM(pfun)\
  {WM_VKEYTOITEM,0,(UINT)-1,(UINT)-1,TIWL,tXCallFunc((INT(XEvent::*)(DWORD,HWND))(&pfun)),-1},

#define EVENT_CHARTOITEM(pfun)\
  {WM_CHARTOITEM,0,(UINT)-1,(UINT)-1,TIWL,tXCallFunc((INT(XEvent::*)(DWORD,HWND))(&pfun)),-1},

#define EVENT_SETFONT(pfun)\
  {WM_SETFONT,0,(UINT)-1,(UINT)-1,TVWL,tXCallFunc((VOID(XEvent::*)(HFONT,DWORD))(&pfun)),-1},

#define EVENT_GETFONT(func)\
  {WM_GETFONT,0,(UINT)-1,(UINT)-1,TIV,tXCallFunc((HFONT(XEvent::*)(VOID))(&pfun)),-1},

#define EVENT_SETHOTKEY(func)\
  {WM_SETHOTKEY,0,(UINT)-1,(UINT)-1,TIW,tXCallFunc((INT(XEvent::*)(DWORD))(&pfun)),-1},

#define EVENT_GETHOTKEY(func)\
  {WM_GETHOTKEY,0,(UINT)-1,(UINT)-1,TIV,tXCallFunc((UINT(XEvent::*)(VOID))(&pfun)),-1},

#define EVENT_QUERYDRAGICON(pfun)\
  {WM_QUERYDRAGICON,0,(UINT)-1,(UINT)-1,TIV,tXCallFunc((HCURSOR(XEvent::*)(VOID))(&pfun)),-1},

#define EVENT_COMPAREITEM(pfun)\
  {WM_COMPAREITEM,0,(UINT)-1,(UINT)-1,TIWL,tXCallFunc((INT(XEvent::*)(INT,LPCOMPAREITEMSTRUCT))(&pfun)),-1},

#define EVENT_GETOBJECT(pfun)\
  {WM_GETOBJECT,0,(UINT)-1,(UINT)-1,TIWL,tXCallFunc((LRESULT(XEvent::*)(DWORD,DWORD))(&pfun)),-1},

#define EVENT_COMPACTING(pfun)\
  {WM_COMPACTING,0,(UINT)-1,(UINT)-1,TVW,tXCallFunc((VOID(XEvent::*)(UINT))(&pfun)),-1},

#define EVENT_WINDOWPOSCHANGING(pfun)\
  {WM_WINDOWPOSCHANGING,0,(UINT)-1,(UINT)-1,TVL,tXCallFunc((VOID(XEvent::*)(LPWINDOWPOS))(&pfun)),-1},

#define EVENT_WINDOWPOSCHANGED(pfun)\
  {WM_WINDOWPOSCHANGED,0,(UINT)-1,(UINT)-1,TVL,tXCallFunc((VOID(XEvent::*)(LPWINDOWPOS))(&pfun)),-1},

#define EVENT_POWER(pfun)\
  {WM_POWER,0,(UINT)-1,(UINT)-1,TIW,tXCallFunc((INT(XEvent::*)(DWORD))(&pfun)),-1},

#define EVENT_COPYDATA(pfun)\
  {WM_COPYDATA,0,(UINT)-1,(UINT)-1,TIWL,tXCallFunc((BOOL(XEvent::*)(HWND,PCOPYDATASTRUCT))(&pfun)),-1},

#define EVENT_CANCELJOURNAL(pfun)\
  {WM_CANCELJOURNAL,0,(UINT)-1,(UINT)-1,TVV,tXCallFunc((VOID(XEvent::*)(VOID))(&pfun)),-1},

#define EVENT_INPUTLANGCHANGEREQUEST(pfun)\
  {WM_INPUTLANGCHANGEREQUEST,0,(UINT)-1,(UINT)-1,TVWL,tXCallFunc((VOID(XEvent::*)(UINT,UINT))(&pfun)),-1},

#define EVENT_INPUTLANGCHANGE(pfun)\
  {WM_INPUTLANGCHANGE,0,(UINT)-1,(UINT)-1,TVWL,tXCallFunc((VOID(XEvent::*)(UINT,UINT))(&pfun)),-1},

#define EVENT_TCARD(pfun)\
  {WM_TCARD,0,(UINT)-1,(UINT)-1,TVWL,tXCallFunc((VOID(XEvent::*)(UINT,DWORD))(&pfun)),-1},

#define EVENT_HELP(pfun)\
  {WM_HELP,0,(UINT)-1,(UINT)-1,TIL,tXCallFunc((BOOL(XEvent::*)(LPHELPINFO))(&pfun)),-1},

#define EVENT_USERCHANGED(pfun)\
  {WM_USERCHANGED,0,(UINT)-1,(UINT)-1,TVV,tXCallFunc((VOID(XEvent::*)(VOID))(&pfun)),-1},

#define EVENT_NOTIFYFORMAT(pfun)\
  {WM_NOTIFYFORMAT,0,(UINT)-1,(UINT)-1,TIWL,tXCallFunc((UINT(XEvent::*)(HWND,UINT))(&pfun)),-1},

#define EVENT_CONTEXTMENU(pfun)\
  {WM_CONTEXTMENU,0,(UINT)-1,(UINT)-1,TVWL,tXCallFunc((VOID(XEvent::*)(HWND,POINTS))(&pfun)),-1},

#define EVENT_STYLECHANGING(pfun)\
  {WM_STYLECHANGING,0,(UINT)-1,(UINT)-1,TVWL,tXCallFunc((VOID(XEvent::*)(INT,LPSTYLESTRUCT))(&pfun)),-1},

#define EVENT_STYLECHANGED(pfun)\
  {WM_STYLECHANGED,0,(UINT)-1,(UINT)-1,TVWL,tXCallFunc((VOID(XEvent::*)(INT,LPSTYLESTRUCT))(&pfun)),-1},

#define EVENT_DISPLAYCHANGE(pfun)\
  {WM_DISPLAYCHANGE,0,(UINT)-1,(UINT)-1,TVWL,tXCallFunc((VOID(XEvent::*)(UINT,POINTS))(&pfun)),-1},

#define EVENT_GETICON(pfun)\
  {WM_GETICON,0,(UINT)-1,(UINT)-1,TIW,tXCallFunc((HICON(XEvent::*)(INT))(&pfun)),-1},

#define EVENT_SETICON(pfun)\
  {WM_SETICON,0,(UINT)-1,(UINT)-1,TIWL,tXCallFunc((HICON(XEvent::*)(BOOL,HICON))(&pfun)),-1},

#define EVENT_NCCREATE(pfun)\
  {WM_NCCREATE,0,(UINT)-1,(UINT)-1,TIL,tXCallFunc((BOOL(XEvent::*)(LPCREATESTRUCT))(&pfun)),-1},

#define EVENT_NCDESTROY(pfun)\
  {WM_NCDESTROY,0,(UINT)-1,(UINT)-1,TVV,tXCallFunc((VOID(XEvent::*)(VOID))(&pfun)),-1},

#define EVENT_NCCALCSIZE(pfun)\
  {WM_NCCALCSIZE,0,(UINT)-1,(UINT)-1,TIWL,tXCallFunc((INT(XEvent::*)(BOOL,NCCALCSIZE_PARAMS*))(&pfun)),-1},

#define EVENT_NCHITTEST(pfun)\
  {WM_NCHITTEST,0,(UINT)-1,(UINT)-1,TIL,tXCallFunc((LRESULT(XEvent::*)(POINTS))(&pfun)),-1},

#define EVENT_NCPAINT(pfun)\
  {WM_NCPAINT,0,(UINT)-1,(UINT)-1,TVW,tXCallFunc((VOID(XEvent::*)(HRGN))(&pfun)),-1},

#define EVENT_NCACTIVATE(pfun)\
  {WM_NCACTIVATE,0,(UINT)-1,(UINT)-1,TIWL,tXCallFunc((BOOL(XEvent::*)(BOOL,HRGN))(&pfun)),-1},

#define EVENT_GETDLGCODE(pfun)\
  {WM_GETDLGCODE,0,(UINT)-1,(UINT)-1,TIWL,tXCallFunc((UINT(XEvent::*)(DWORD,LPMSG))(&pfun)),-1},

#define EVENT_SYNCPAINT(pfun)\
  {WM_SYNCPAINT,0,(UINT)-1,(UINT)-1,TVV,tXCallFunc((VOID(XEvent::*)(VOID))(&pfun)),-1},

//NC Mouse Event's

#define EVENT_NCMOUSEMOVE(pfun)\
  {WM_NCMOUSEMOVE,0,(UINT)-1,(UINT)-1,TVWL,tXCallFunc((VOID(XEvent::*)(UINT,POINTS))(&pfun)),-1},

#define EVENT_NCLBUTTONDOWN(pfun)\
  {WM_NCLBUTTONDOWN,0,(UINT)-1,(UINT)-1,TVWL,tXCallFunc((VOID(XEvent::*)(UINT,POINTS))(&pfun)),-1},

#define EVENT_NCLBUTTONUP(pfun)\
  {WM_NCLBUTTONUP,0,(UINT)-1,(UINT)-1,TVWL,tXCallFunc((VOID(XEvent::*)(UINT,POINTS))(&pfun)),-1},

#define EVENT_NCLBUTTONDBLCLK(pfun)\
  {WM_NCLBUTTONDBLCLK,0,(UINT)-1,(UINT)-1,TVWL,tXCallFunc((VOID(XEvent::*)(UINT,POINTS))(&pfun)),-1},

#define EVENT_NCRBUTTONDOWN(pfun)\
  {WM_NCRBUTTONDOWN,0,(UINT)-1,(UINT)-1,TVWL,tXCallFunc((VOID(XEvent::*)(UINT,POINTS))(&pfun)),-1},

#define EVENT_NCRBUTTONUP(pfun)\
  {WM_NCRBUTTONUP,0,(UINT)-1,(UINT)-1,TVWL,tXCallFunc((VOID(XEvent::*)(UINT,POINTS))(&pfun)),-1},

#define EVENT_NCRBUTTONDBLCLK(pfun)\
  {WM_NCRBUTTONDBLCLK,0,(UINT)-1,(UINT)-1,TVWL,tXCallFunc((VOID(XEvent::*)(UINT,POINTS))(&pfun)),-1},

#define EVENT_NCMBUTTONDOWN(pfun)\
  {WM_NCMBUTTONDOWN,0,(UINT)-1,(UINT)-1,TVWL,tXCallFunc((VOID(XEvent::*)(UINT,POINTS))(&pfun)),-1},

#define EVENT_NCMBUTTONUP(pfun)\
  {WM_NCMBUTTONUP,0,(UINT)-1,(UINT)-1,TVWL,tXCallFunc((VOID(XEvent::*)(UINT,POINTS))(&pfun)),-1},

#define EVENT_NCMBUTTONDBLCLK(pfun)\
  {WM_NCMBUTTONDBLCLK,0,(UINT)-1,(UINT)-1,TVWL,tXCallFunc((VOID(XEvent::*)(UINT,POINTS))(&pfun)),-1},

#define EVENT_NCXBUTTONDOWN(pfun)\
  {WM_NCXBUTTONDOWN,0,(UINT)-1,(UINT)-1,TVWL,tXCallFunc((VOID(XEvent::*)(UINT,POINTS))(&pfun)),-1},

#define EVENT_NCXBUTTONUP(pfun)\
  {WM_NCXBUTTONUP,0,(UINT)-1,(UINT)-1,TVWL,tXCallFunc((VOID(XEvent::*)(UINT,POINTS))(&pfun)),-1},

#define EVENT_NCXBUTTONDBLCLK(pfun)\
  {WM_NCXBUTTONDBLCLK,0,(UINT)-1,(UINT)-1,TVWL,tXCallFunc((VOID(XEvent::*)(UINT,POINTS))(&pfun)),-1},

#define EVENT_INPUT(pfun)\
  {WM_INPUT,0,(UINT)-1,(UINT)-1,TVWL,tXCallFunc((VOID(XEvent::*)(BOOL,LPRAWINPUT))(&pfun)),-1},

//Keyboard Event's

#define EVENT_KEYDOWN(pfun)\
  {WM_KEYDOWN,0,(UINT)-1,(UINT)-1,TVWL,tXCallFunc((VOID(XEvent::*)(UINT,UINT))(&pfun)),-1},

#define EVENT_KEYUP(pfun)\
  {WM_KEYUP,0,(UINT)-1,(UINT)-1,TVWL,tXCallFunc((VOID(XEvent::*)(UINT,UINT))(&pfun)),-1},

#define EVENT_CHAR(pfun)\
  {WM_CHAR,0,(UINT)-1,(UINT)-1,TVWL,tXCallFunc((VOID(XEvent::*)(UINT,UINT))(&pfun)),-1},

#define EVENT_DEADCHAR(pfun)\
  {WM_DEADCHAR,0,(UINT)-1,(UINT)-1,TVWL,tXCallFunc((VOID(XEvent::*)(UINT,UINT))(&pfun)),-1},

#define EVENT_SYSKEYDOWN(pfun)\
  {WM_SYSKEYDOWN,0,(UINT)-1,(UINT)-1,TVWL,tXCallFunc((VOID(XEvent::*)(UINT,UINT))(&pfun)),-1},

#define EVENT_SYSKEYUP(pfun)\
  {WM_SYSKEYUP,0,(UINT)-1,(UINT)-1,TVWL,tXCallFunc((VOID(XEvent::*)(UINT,UINT))(&pfun)),-1},

#define EVENT_SYSCHAR(pfun)\
  {WM_SYSCHAR,0,(UINT)-1,(UINT)-1,TVWL,tXCallFunc((VOID(XEvent::*)(UINT,UINT))(&pfun)),-1},

#define EVENT_SYSDEADCHAR(pfun)\
  {WM_SYSDEADCHAR,0,(UINT)-1,(UINT)-1,TVWL,tXCallFunc((VOID(XEvent::*)(UINT,UINT))(&pfun)),-1},

//MUI Event's

#define EVENT_INITDIALOG(pfun)\
  {WM_INITDIALOG,0,(UINT)-1,(UINT)-1,TIWL,tXCallFunc((BOOL(XEvent::*)(HWND,LPARAM))(&pfun)),-1},

#define EVENT_SYSCOMMAND(pfun)\
  {WM_SYSCOMMAND,0,(UINT)-1,(UINT)-1,TVWL,tXCallFunc((VOID(XEvent::*)(UINT,DWORD))(&pfun)),-1},

#define EVENT_TIMER(pfun)\
  {WM_TIMER,0,(UINT)-1,(UINT)-1,TVWL,tXCallFunc((VOID(XEvent::*)(UINT_PTR,TIMERPROC))(&pfun)),-1},

#define EVENT_TIMER_ID(id,pfun)\
  {WM_TIMER,id,(UINT)-1,(UINT)-1,TIW,tXCallFunc((VOID(XEvent::*)(TIMERPROC))(&pfun)),-1},

#define EVENT_HSCROLL(pfun)\
  {WM_HSCROLL,0,(UINT)-1,(UINT)-1,TVWL,tXCallFunc((VOID(XEvent::*)(DWORD,HWND))(&pfun)),-1},

#define EVENT_VSCROLL(pfun)\
  {WM_VSCROLL,0,(UINT)-1,(UINT)-1,TVWL,tXCallFunc((VOID(XEvent::*)(DWORD,HWND))(&pfun)),-1},

#define EVENT_INITMENU(pfun)\
  {WM_INITMENU,0,(UINT)-1,(UINT)-1,TVW,tXCallFunc((VOID(XEvent::*)(HMENU))(&pfun)),-1},

#define EVENT_INITMENUPOPUP(pfun)\
  {WM_INITMENUPOPUP,0,(UINT)-1,(UINT)-1,TVWL,tXCallFunc((VOID(XEvent::*)(HMENU,DWORD))(&pfun)),-1},

//WM_SYSTIMER

#define EVENT_MENUSELECT(pfun)\
  {WM_MENUSELECT,0,(UINT)-1,(UINT)-1,TVWL,tXCallFunc((VOID(XEvent::*)(UINT,HMENU))(&pfun)),-1},

#define EVENT_MENUCHAR(pfun)\
  {WM_MENUCHAR,0,(UINT)-1,(UINT)-1,TIWL,tXCallFunc((INT(XEvent::*)(UINT,HMENU))(&pfun)),-1},

#define EVENT_ENTERIDLE(pfun)\
  {WM_ENTERIDLE,0,(UINT)-1,(UINT)-1,TVWL,tXCallFunc((VOID(XEvent::*)(INT,HWND))(&pfun)),-1},

#define EVENT_MENURBUTTONUP(pfun)\
  {WM_MENURBUTTONUP,0,(UINT)-1,(UINT)-1,TVWL,tXCallFunc((VOID(XEvent::*)(INT,HMENU))(&pfun)),-1},

#define EVENT_MENUDRAG(pfun)\
  {WM_MENUDRAG,0,(UINT)-1,(UINT)-1,TIWL,tXCallFunc((BOOL(XEvent::*)(POINTS,HMENU))(&pfun)),-1},

#define EVENT_MENUGETOBJECT(pfun)\
  {WM_MENUGETOBJECT,0,(UINT)-1,(UINT)-1,TIL,tXCallFunc((BOOL(XEvent::*)(PMENUGETOBJECTINFO))(&pfun)),-1},

#define EVENT_UNINITMENUPOPUP(pfun)\
  {WM_UNINITMENUPOPUP,0,(UINT)-1,(UINT)-1,TVWL,tXCallFunc((VOID(XEvent::*)(HMENU,DWORD))(&pfun)),-1},

#define EVENT_MENUCOMMAND(pfun)\
  {WM_MENUCOMMAND,0,(UINT)-1,(UINT)-1,TVWL,tXCallFunc((VOID(XEvent::*)(INT,HMENU))(&pfun)),-1},

#define EVENT_CHANGEUISTATE(pfun)\
  {WM_CHANGEUISTATE,0,(UINT)-1,(UINT)-1,TVW,tXCallFunc((VOID(XEvent::*)(DWORD))(&pfun)),-1},

#define EVENT_UPDATEUISTATE(pfun)\
  {WM_UPDATEUISTATE,0,(UINT)-1,(UINT)-1,TVW,tXCallFunc((VOID(XEvent::*)(DWORD))(&pfun)),-1},

#define EVENT_QUERYUISTATE(pfun)\
  {WM_QUERYUISTATE,0,(UINT)-1,(UINT)-1,TIV,tXCallFunc((UINT(XEvent::*)(VOID))(&pfun)),-1},

//CtlColor Event's

#define EVENT_CTLCOLORMSGBOX(pfun)\
  {WM_CTLCOLORMSGBOX,0,(UINT)-1,(UINT)-1,TIWL,tXCallFunc((HBRUSH(XEvent::*)(HDC,HWND))(&pfun)),-1},

#define EVENT_CTLCOLOREDIT(pfun)\
  {WM_CTLCOLOREDIT,0,(UINT)-1,(UINT)-1,TIWL,tXCallFunc((HBRUSH(XEvent::*)(HDC,HWND))(&pfun)),-1},

#define EVENT_CTLCOLORLISTBOX(pfun)\
  {WM_CTLCOLORLISTBOX,0,(UINT)-1,(UINT)-1,TIWL,tXCallFunc((HBRUSH(XEvent::*)(HDC,HWND))(&pfun)),-1},

#define EVENT_CTLCOLORBTN(pfun)\
  {WM_CTLCOLORBTN,0,(UINT)-1,(UINT)-1,TIWL,tXCallFunc((HBRUSH(XEvent::*)(HDC,HWND))(&pfun)),-1},

#define EVENT_CTLCOLORDLG(pfun)\
  {WM_CTLCOLORDLG,0,(UINT)-1,(UINT)-1,TIWL,tXCallFunc((HBRUSH(XEvent::*)(HDC,HWND))(&pfun)),-1},

#define EVENT_CTLCOLORSCROLLBAR(pfun)\
  {WM_CTLCOLORSCROLLBAR,0,(UINT)-1,(UINT)-1,TIWL,tXCallFunc((HBRUSH(XEvent::*)(HDC,HWND))(&pfun)),-1},

#define EVENT_CTLCOLORSTATIC(pfun)\
  {WM_CTLCOLORSTATIC,0,(UINT)-1,(UINT)-1,TIWL,tXCallFunc((HBRUSH(XEvent::*)(HDC,HWND))(&pfun)),-1},

//Mouse Event's

#define EVENT_MOUSEMOVE(pfun)\
  {WM_MOUSEMOVE,0,(UINT)-1,(UINT)-1,TVWL,tXCallFunc((VOID(XEvent::*)(UINT,POINTS))(&pfun)),-1},

#define EVENT_LBUTTONDOWN(pfun)\
  {WM_LBUTTONDOWN,0,(UINT)-1,(UINT)-1,TVWL,tXCallFunc((VOID(XEvent::*)(UINT,POINTS))(&pfun)),-1},

#define EVENT_LBUTTONUP(pfun)\
  {WM_LBUTTONUP,0,(UINT)-1,(UINT)-1,TVWL,tXCallFunc((VOID(XEvent::*)(UINT,POINTS))(&pfun)),-1},

#define EVENT_LBUTTONDBLCLK(pfun)\
  {WM_LBUTTONDBLCLK,0,(UINT)-1,(UINT)-1,TVWL,tXCallFunc((VOID(XEvent::*)(UINT,POINTS))(&pfun)),-1},

#define EVENT_RBUTTONDOWN(pfun)\
  {WM_RBUTTONDOWN,0,(UINT)-1,(UINT)-1,TVWL,tXCallFunc((VOID(XEvent::*)(UINT,POINTS))(&pfun)),-1},

#define EVENT_RBUTTONUP(pfun)\
  {WM_RBUTTONUP,0,(UINT)-1,(UINT)-1,TVWL,tXCallFunc((VOID(XEvent::*)(UINT,POINTS))(&pfun)),-1},

#define EVENT_RBUTTONDBLCLK(pfun)\
  {WM_RBUTTONDBLCLK,0,(UINT)-1,(UINT)-1,TVWL,tXCallFunc((VOID(XEvent::*)(UINT,POINTS))(&pfun)),-1},

#define EVENT_MBUTTONDOWN(pfun)\
  {WM_MBUTTONDOWN,0,(UINT)-1,(UINT)-1,TVWL,tXCallFunc((VOID(XEvent::*)(UINT,POINTS))(&pfun)),-1},

#define EVENT_MBUTTONUP(pfun)\
  {WM_MBUTTONUP,0,(UINT)-1,(UINT)-1,TVWL,tXCallFunc((VOID(XEvent::*)(UINT,POINTS))(&pfun)),-1},

#define EVENT_MBUTTONDBLCLK(pfun)\
  {WM_MBUTTONDBLCLK,0,(UINT)-1,(UINT)-1,TVWL,tXCallFunc((VOID(XEvent::*)(UINT,POINTS))(&pfun)),-1},

#define EVENT_MOUSEWHEEL(pfun)\
  {WM_MOUSEWHEEL,0,(UINT)-1,(UINT)-1,TVWL,tXCallFunc((VOID(XEvent::*)(UINT,POINTS))(&pfun)),-1},

#define EVENT_XBUTTONDOWN(pfun)\
  {WM_XBUTTONDOWN,0,(UINT)-1,(UINT)-1,TVWL,tXCallFunc((VOID(XEvent::*)(UINT,POINTS))(&pfun)),-1},

#define EVENT_XBUTTONUP(pfun)\
  {WM_XBUTTONUP,0,(UINT)-1,(UINT)-1,TVWL,tXCallFunc((VOID(XEvent::*)(UINT,POINTS))(&pfun)),-1},

#define EVENT_XBUTTONDBLCLK(pfun)\
  {WM_XBUTTONDBLCLK,0,(UINT)-1,(UINT)-1,TVWL,tXCallFunc((VOID(XEvent::*)(UINT,POINTS))(&pfun)),-1},

#define EVENT_PARENTNOTIFY(pfun)\
  {WM_PARENTNOTIFY,0,(UINT)-1,(UINT)-1,TVWL,tXCallFunc((VOID(XEvent::*)(UINT,POINTS))(&pfun)),-1},

#define EVENT_ENTERMENULOOP(pfun)\
  {WM_ENTERMENULOOP,0,(UINT)-1,(UINT)-1,TVW,tXCallFunc((VOID(XEvent::*)(BOOL))(&pfun)),-1},

#define EVENT_EXITMENULOOP(pfun)\
  {WM_EXITMENULOOP,0,(UINT)-1,(UINT)-1,TVW,tXCallFunc((VOID(XEvent::*)(BOOL))(&pfun)),-1},

#define EVENT_NEXTMENU(pfun)\
  {WM_NEXTMENU,0,(UINT)-1,(UINT)-1,TVWL,tXCallFunc((VOID(XEvent::*)(UINT,PMDINEXTMENU))(&pfun)),-1},

#define EVENT_SIZING(pfun)\
  {WM_SIZING,0,(UINT)-1,(UINT)-1,TIWL,tXCallFunc((BOOL(XEvent::*)(UINT,LPRECT))(&pfun)),-1},

#define EVENT_CAPTURECHANGED(pfun)\
  {WM_CAPTURECHANGED,0,(UINT)-1,(UINT)-1,TVL,tXCallFunc((VOID(XEvent::*)(HWND))(&pfun)),-1},

#define EVENT_MOVING(pfun)\
  {WM_MOVING,0,(UINT)-1,(UINT)-1,TIL,tXCallFunc((BOOL(XEvent::*)(LPRECT))(&pfun)),-1},

#define EVENT_POWERBROADCAST(pfun)\
  {WM_POWERBROADCAST,0,(UINT)-1,(UINT)-1,TIWL,tXCallFunc((UINT(XEvent::*)(UINT,UINT))(&pfun)),-1},

#define EVENT_DEVICECHANGE(pfun)\
  {WM_DEVICECHANGE,0,(UINT)-1,(UINT)-1,TIWL,tXCallFunc((BOOL(XEvent::*)(UINT,DWORD_PTR))(&pfun)),-1},

//WM_MDI...

#define EVENT_MDICREATE(pfun)\
  {WM_MDICREATE,0,(UINT)-1,(UINT)-1,TIL,tXCallFunc((HWND(XEvent::*)(LPMDICREATESTRUCT))(&pfun)),-1},

#define EVENT_MDIDESTROY(pfun)\
  {WM_MDIDESTROY,0,(UINT)-1,(UINT)-1,TVW,tXCallFunc((VOID(XEvent::*)(HWND))(&pfun)),-1},

#define EVENT_MDIACTIVATE(pfun)\
  {WM_MDIACTIVATE,0,(UINT)-1,(UINT)-1,TVW,tXCallFunc((VOID(XEvent::*)(HWND))(&pfun)),-1},

#define EVENT_MDIRESTORE(pfun)\
  {WM_MDIRESTORE,0,(UINT)-1,(UINT)-1,TVW,tXCallFunc((VOID(XEvent::*)(HWND))(&pfun)),-1},

#define EVENT_MDINEXT(pfun)\
  {WM_MDINEXT,0,(UINT)-1,(UINT)-1,TVWL,tXCallFunc((VOID(XEvent::*)(HWND,BOOL))(&pfun)),-1},

#define EVENT_MDIMAXIMIZE(pfun)\
  {WM_MDIMAXIMIZE,0,(UINT)-1,(UINT)-1,TVW,tXCallFunc((VOID(XEvent::*)(HWND))(&pfun)),-1},

#define EVENT_MDITILE(pfun)\
  {WM_MDITILE,0,(UINT)-1,(UINT)-1,TIW,tXCallFunc((BOOL(XEvent::*)(INT))(&pfun)),-1},

#define EVENT_MDICASCADE(pfun)\
  {WM_MDICASCADE,0,(UINT)-1,(UINT)-1,TIW,tXCallFunc((BOOL(XEvent::*)(INT))(&pfun)),-1},

#define EVENT_MDIICONARRANGE(pfun)\
  {WM_MDIICONARRANGE,0,(UINT)-1,(UINT)-1,TVV,tXCallFunc((VOID(XEvent::*)(VOID))(&pfun)),-1},

#define EVENT_MDIGETACTIVE(pfun)\
  {WM_MDIGETACTIVE,0,(UINT)-1,(UINT)-1,TIL,tXCallFunc((HWND(XEvent::*)(LPBOOL))(&pfun)),-1},

#define EVENT_MDISETMENU(pfun)\
  {WM_MDISETMENU,0,(UINT)-1,(UINT)-1,TIWL,tXCallFunc((HMENU(XEvent::*)(HWND,HWND))(&pfun)),-1},

#define EVENT_ENTERSIZEMOVE(pfun)\
  {WM_ENTERSIZEMOVE,0,(UINT)-1,(UINT)-1,TVV,tXCallFunc((VOID(XEvent::*)(VOID))(&pfun)),-1},

#define EVENT_EXITSIZEMOVE(pfun)\
  {WM_EXITSIZEMOVE,0,(UINT)-1,(UINT)-1,TVV,tXCallFunc((VOID(XEvent::*)(VOID))(&pfun)),-1},

#define EVENT_DROPFILES(pfun)\
  {WM_DROPFILES,0,(UINT)-1,(UINT)-1,TVW,tXCallFunc((VOID(XEvent::*)(HDROP))(&pfun)),-1},

#define EVENT_MDIREFRESHMENU(pfun)\
  {WM_MDIREFRESHMENU,0,(UINT)-1,(UINT)-1,TIV,tXCallFunc((HMENU(XEvent::*)(VOID))(&pfun)),-1},

//WM_IME...

#define EVENT_NCMOUSEHOVER(pfun)\
  {WM_NCMOUSEHOVER,0,(UINT)-1,(UINT)-1,TVWL,tXCallFunc((VOID(XEvent::*)(UINT,POINTS))(&pfun)),-1},

#define EVENT_MOUSEHOVER(pfun)\
  {WM_MOUSEHOVER,0,(UINT)-1,(UINT)-1,TVWL,tXCallFunc((VOID(XEvent::*)(UINT,POINTS))(&pfun)),-1},

#define EVENT_NCMOUSELEAVE(pfun)\
  {WM_NCMOUSELEAVE,0,(UINT)-1,(UINT)-1,TVV,tXCallFunc((VOID(XEvent::*)(VOID))(&pfun)),-1},

#define EVENT_MOUSELEAVE(pfun)\
  {WM_MOUSELEAVE,0,(UINT)-1,(UINT)-1,TVV,tXCallFunc((VOID(XEvent::*)(VOID))(&pfun)),-1},

#define EVENT_RENDERFORMAT(pfun)\
  {WM_RENDERFORMAT,0,(UINT)-1,(UINT)-1,TVW,tXCallFunc((VOID(XEvent::*)(UINT))(&pfun)),-1},

#define EVENT_RENDERALLFORMATS(pfun)\
  {WM_RENDERALLFORMATS,0,(UINT)-1,(UINT)-1,TVV,tXCallFunc((VOID(XEvent::*)(VOID))(&pfun)),-1},

#define EVENT_DESTROYCLIPBOARD(pfun)\
  {WM_DESTROYCLIPBOARD,0,(UINT)-1,(UINT)-1,TVV,tXCallFunc((VOID(XEvent::*)(VOID))(&pfun)),-1},

#define EVENT_DRAWCLIPBOARD(pfun)\
  {WM_DRAWCLIPBOARD,0,(UINT)-1,(UINT)-1,TVV,tXCallFunc((VOID(XEvent::*)(VOID))(&pfun)),-1},

#define EVENT_PAINTCLIPBOARD(pfun)\
  {WM_PAINTCLIPBOARD,0,(UINT)-1,(UINT)-1,TVWL,tXCallFunc((VOID(XEvent::*)(HWND,HGLOBAL))(&pfun)),-1},

#define EVENT_VSCROLLCLIPBOARD(pfun)\
  {WM_VSCROLLCLIPBOARD,0,(UINT)-1,(UINT)-1,TVWL,tXCallFunc((VOID(XEvent::*)(HWND,DWORD))(&pfun)),-1},

#define EVENT_SIZECLIPBOARD(pfun)\
  {WM_SIZECLIPBOARD,0,(UINT)-1,(UINT)-1,TVWL,tXCallFunc((VOID(XEvent::*)(HWND,HGLOBAL))(&pfun)),-1},

#define EVENT_ASKCBFORMATNAME(pfun)\
  {WM_ASKCBFORMATNAME,0,(UINT)-1,(UINT)-1,TVWL,tXCallFunc((VOID(XEvent::*)(UINT,LPTSTR))(&pfun)),-1},

#define EVENT_CHANGECBCHAIN(pfun)\
  {WM_CHANGECBCHAIN,0,(UINT)-1,(UINT)-1,TVWL,tXCallFunc((VOID(XEvent::*)(HWND,HWND))(&pfun)),-1},

#define EVENT_HSCROLLCLIPBOARD(pfun)\
  {WM_HSCROLLCLIPBOARD,0,(UINT)-1,(UINT)-1,TVWL,tXCallFunc((VOID(XEvent::*)(HWND,DWORD))(&pfun)),-1},

#define EVENT_QUERYNEWPALETTE(pfun)\
  {WM_QUERYNEWPALETTE,0,(UINT)-1,(UINT)-1,TIV,tXCallFunc((BOOL(XEvent::*)(VOID))(&pfun)),-1},

#define EVENT_PALETTEISCHANGING(pfun)\
  {WM_PALETTEISCHANGING,0,(UINT)-1,(UINT)-1,TVW,tXCallFunc((VOID(XEvent::*)(HWND))(&pfun)),-1},

#define EVENT_PALETTECHANGED(pfun)\
  {WM_PALETTECHANGED,0,(UINT)-1,(UINT)-1,TVW,tXCallFunc((VOID(XEvent::*)(HWND))(&pfun)),-1},

#define EVENT_HOTKEY(winid,pfun)\
  {WM_HOTKEY,0,(UINT)-1,(UINT)-1,TVWL,tXCallFunc((VOID(XEvent::*)(UINT,UINT))(&pfun)),-1},

#define EVENT_PRINT(pfun)\
  {WM_PRINT,0,(UINT)-1,(UINT)-1,TVWL,tXCallFunc((VOID(XEvent::*)(HDC,UINT))(&pfun)),-1},

#define EVENT_PRINTCLIENT(pfun)\
  {WM_PRINTCLIENT,0,(UINT)-1,(UINT)-1,TVWL,tXCallFunc((VOID(XEvent::*)(HDC,UINT))(&pfun)),-1},

#define EVENT_APPCOMMAND(pfun)\
  {WM_APPCOMMAND,0,(UINT)-1,(UINT)-1,TIWL,tXCallFunc((BOOL(XEvent::*)(HWND,DWORD))(&pfun)),-1},

#define EVENT_THEMECHANGED(pfun)\
  {WM_THEMECHANGED,0,(UINT)-1,(UINT)-1,TVV,tXCallFunc((VOID(XEvent::*)(VOID))(&pfun)),-1},

/******************************************************/
//Custom Control MSG's
/******************************************************/

#define EVENT_BM_SETCHECK(pfun)\
  {BM_SETCHECK,0,(UINT)-1,(UINT)-1,TVW,tXCallFunc((VOID(XEvent::*)(UINT))(&pfun)),-1},

#define EVENT_BM_GETCHECK(pfun)\
  {BM_GETCHECK,0,(UINT)-1,(UINT)-1,TIV,tXCallFunc((UINT(XEvent::*)(VOID))(&pfun)),-1},

/******************************************************/
// Miscellaneous (EX)
/******************************************************/

#define EVENT_CTLCOLORLISTBOX_EX(pfun,bforce)\
  {WM_CTLCOLORLISTBOX,0,(UINT)-1,(UINT)-1,TIWL,tXCallFunc((HBRUSH(XEvent::*)(HDC,HWND))(&pfun)),bforce},

#define EVENT_CTLCOLORMSGBOX_EX(pfun,bforce)\
  {WM_CTLCOLORMSGBOX,0,(UINT)-1,(UINT)-1,TIWL,tXCallFunc((HBRUSH(XEvent::*)(HDC,HWND))(&pfun)),bforce},

#define EVENT_CTLCOLORSCROLLBAR_EX(pfun,bforce)\
  {WM_CTLCOLORSCROLLBAR,0,(UINT)-1,(UINT)-1,TIWL,tXCallFunc((HBRUSH(XEvent::*)(HDC,HWND))(&pfun)),bforce},

#define EVENT_CTLCOLOREDIT_EX(pfun,bforce)\
  {WM_CTLCOLOREDIT,0,(UINT)-1,(UINT)-1,TIWL,tXCallFunc((HBRUSH(XEvent::*)(HDC,HWND))(&pfun)),bforce},

#define EVENT_CTLCOLORSTATIC_EX(pfun,bforce)\
  {WM_CTLCOLORSTATIC,0,(UINT)-1,(UINT)-1,TIWL,tXCallFunc((HBRUSH(XEvent::*)(HDC,HWND))(&pfun)),bforce},

#define EVENT_CTLCOLORBTN_EX(pfun,bforce)\
  {WM_CTLCOLORBTN,0,(UINT)-1,(UINT)-1,TIWL,tXCallFunc((HBRUSH(XEvent::*)(HDC,HWND))(&pfun)),bforce},

#define EVENT_CTLCOLORDLG_EX(pfun,bforce)\
  {WM_CTLCOLORDLG,0,(UINT)-1,(UINT)-1,TIWL,tXCallFunc((HBRUSH(XEvent::*)(HDC,HWND))(&pfun)),bforce},

#define EVENT_CHARTOITEM_EX(pfun,bforce)\
  {WM_CHARTOITEM,0,(UINT)-1,(UINT)-1,TIWL,tXCallFunc((INT(XEvent::*)(DWORD,HWND))(&pfun)),bforce},

#define EVENT_VKEYTOITEM_EX(pfun,bforce)\
  {WM_VKEYTOITEM,0,(UINT)-1,(UINT)-1,TIWL,tXCallFunc((INT(XEvent::*)(DWORD,HWND))(&pfun)),bforce},

#define EVENT_HSCROLL_EX(pfun,bforce)\
  {WM_HSCROLL,0,(UINT)-1,(UINT)-1,TVWL,tXCallFunc((VOID(XEvent::*)(DWORD,HWND))(&pfun)),bforce},

#define EVENT_VSCROLL_EX(pfun,bforce)\
  {WM_VSCROLL,0,(UINT)-1,(UINT)-1,TVWL,tXCallFunc((VOID(XEvent::*)(DWORD,HWND))(&pfun)),bforce},

#define EVENT_PARENTNOTIFY_EX(pfun,bforce)\
  {WM_PARENTNOTIFY,0,(UINT)-1,(UINT)-1,TVWL,tXCallFunc((VOID(XEvent::*)(UINT,POINTS))(&pfun)),bforce},

#define EVENT_MEASUREITEM_EX(pfun,bforce)\
  {WM_MEASUREITEM,0,(UINT)-1,(UINT)-1,TIWL,tXCallFunc((BOOL(XEvent::*)(INT,LPMEASUREITEMSTRUCT))(&pfun)),bforce},

#define EVENT_DRAWITEM_EX(pfun,bforce)\
  {WM_DRAWITEM,0,(UINT)-1,(UINT)-1,TIWL,tXCallFunc((BOOL(XEvent::*)(INT,LPDRAWITEMSTRUCT))(&pfun)),bforce},

#define EVENT_COMPAREITEM_EX(pfun,bforce)\
  {WM_COMPAREITEM,0,(UINT)-1,(UINT)-1,TIWL,tXCallFunc((INT(XEvent::*)(INT,LPCOMPAREITEMSTRUCT))(&pfun)),bforce},

#define EVENT_DELETEITEM_EX(pfun,bforce)\
  {WM_DELETEITEM,0,(UINT)-1,(UINT)-1,TIWL,tXCallFunc((BOOL(XEvent::*)(INT,LPDELETEITEMSTRUCT))(&pfun)),bforce},

#define EVENT_MENUCHAR_EX(pfun,bforce)\
  {WM_MENUCHAR,0,(UINT)-1,(UINT)-1,TIWL,tXCallFunc((INT(XEvent::*)(UINT,HMENU))(&pfun)),bforce},

//static control notification codes...
#define EVENT_STN_CLICKED(id,pfun)\
	EVENT_CONTROL(STN_CLICKED,id,pfun)
#define EVENT_STN_DBLCLK(id,pfun)\
	EVENT_CONTROL(STN_DBLCLK,id,pfun)
#define EVENT_STN_ENABLE(id,pfun)\
	EVENT_CONTROL(STN_ENABLE,id,pfun)
#define EVENT_STN_DISABLE(id,pfun)\
	EVENT_CONTROL(STN_DISABLE,id,pfun)

//button control notification codes...
#define EVENT_BN_CLICKED(id,pfun)\
  EVENT_CONTROL(BN_CLICKED,id,pfun)
#define EVENT_BN_DOUBLECLICKED(id,pfun)\
  EVENT_CONTROL(BN_DOUBLECLICKED,id,pfun)
#define EVENT_BN_SETFOCUS(id,pfun)\
  EVENT_CONTROL(BN_SETFOCUS,id,pfun)
#define EVENT_BN_KILLFOCUS(id,pfun)\
  EVENT_CONTROL(BN_KILLFOCUS,id,pfun)

//edit control notification codes...
#define EVENT_EN_SETFOCUS(id,pfun)\
	EVENT_CONTROL(EN_SETFOCUS,id,pfun)
#define EVENT_EN_KILLFOCUS(id,pfun)\
	EVENT_CONTROL(EN_KILLFOCUS,id,pfun)
#define EVENT_EN_CHANGE(id,pfun)\
	EVENT_CONTROL(EN_CHANGE,id,pfun)
#define EVENT_EN_UPDATE(id,pfun)\
	EVENT_CONTROL(EN_UPDATE,id,pfun)
#define EVENT_EN_ERRSPACE(id,pfun)\
	EVENT_CONTROL(EN_ERRSPACE,id,pfun)
#define EVENT_EN_MAXTEXT(id,pfun)\
	EVENT_CONTROL(EN_MAXTEXT,id,pfun)
#define EVENT_EN_HSCROLL(id,pfun)\
	EVENT_CONTROL(EN_HSCROLL,id,pfun)
#define EVENT_EN_VSCROLL(id,pfun)\
	EVENT_CONTROL(EN_VSCROLL,id,pfun)

//richedit control notification codes...
#define EVENT_EN_IMECHANGE(id,pfun)\
	EVENT_CONTROL(EN_IMECHANGE,id,pfun)
#define EVENT_EN_ALIGNLTR(id,pfun)\
	EVENT_CONTROL(EN_ALIGNLTR,id,pfun)
#define EVENT_EN_ALIGNRTL(id,pfun)\
	EVENT_CONTROL(EN_ALIGNRTL,id,pfun)

//animation control notification codes...
#define EVENT_ACN_START(id,pfun)\
	EVENT_CONTROL(ACN_START,id,pfun)
#define EVENT_ACN_STOP(id,pfun)\
	EVENT_CONTROL(ACN_STOP,id,pfun)

//listbox notification codes...
#define EVENT_LBN_ERRSPACE(id,pfun)\
	EVENT_CONTROL(LBN_ERRSPACE,id,pfun)
#define EVENT_LBN_SELCHANGE(id,pfun)\
	EVENT_CONTROL(LBN_SELCHANGE,id,pfun)
#define EVENT_LBN_DBLCLK(id,pfun)\
	EVENT_CONTROL(LBN_DBLCLK,id,pfun)
#define EVENT_LBN_SELCANCEL(id,pfun)\
	EVENT_CONTROL(LBN_SELCANCEL,id,pfun)
#define EVENT_LBN_SETFOCUS(id,pfun)\
	EVENT_CONTROL(LBN_SETFOCUS,id,pfun)
#define EVENT_LBN_KILLFOCUS(id,pfun)\
	EVENT_CONTROL(LBN_KILLFOCUS,id,pfun)

//combobox notification codes...
#define EVENT_CBN_ERRSPACE(id,pfun)\
	EVENT_CONTROL(CBN_ERRSPACE,id,pfun)
#define EVENT_CBN_SELCHANGE(id,pfun)\
	EVENT_CONTROL(CBN_SELCHANGE,id,pfun)
#define EVENT_CBN_DBLCLK(id,pfun)\
	EVENT_CONTROL(CBN_DBLCLK,id,pfun)
#define EVENT_CBN_SETFOCUS(id,pfun)\
	EVENT_CONTROL(CBN_SETFOCUS,id,pfun)
#define EVENT_CBN_KILLFOCUS(id,pfun)\
	EVENT_CONTROL(CBN_KILLFOCUS,id,pfun)
#define EVENT_CBN_EDITCHANGE(id,pfun)\
	EVENT_CONTROL(CBN_EDITCHANGE,id,pfun)
#define EVENT_CBN_EDITUPDATE(id,pfun)\
	EVENT_CONTROL(CBN_EDITUPDATE,id,pfun)
#define EVENT_CBN_DROPDOWN(id,pfun)\
	EVENT_CONTROL(CBN_DROPDOWN,id,pfun)
#define EVENT_CBN_CLOSEUP(id,pfun)\
	EVENT_CONTROL(CBN_CLOSEUP,id,pfun)
#define EVENT_CBN_SELENDOK(id,pfun)\
	EVENT_CONTROL(CBN_SELENDOK,id,pfun)
#define EVENT_CBN_SELENDCANCEL(id,pfun)\
	EVENT_CONTROL(CBN_SELENDCANCEL,id,pfun)

//menu notification codes...
#define EVENT_MENU_CLICKED(id,pfun)\
  EVENT_CTRL_COMMAND(id,pfun)

//Fowards Declarations...
struct _XEventTableEntry;
typedef struct _XEventTable{
	const struct _XEventTable* (*GetBaseEventTable)();
	const struct _XEventTableEntry *pEntry;
}XEventTable,*PXEventTable;

//Fowards Declarations...
struct _XInterfaceEventTableEntry;
typedef struct _XInterfaceEventTable{
	const struct _XInterfaceEventTable* (*GetBaseEventTable)(); // NULL is root class
	const struct _XInterfaceEventTableEntry* pEntry; // map for this class
}XInterfaceEventTable,*PXInterfaceEventTable;

//Fowards Declarations...
struct _XEventSinkTableEntry;
typedef struct _XEventSinkTable{
	const struct _XEventSinkTable* (*GetBaseEventTable)(); // NULL is root class
	const struct _XEventSinkTableEntry* pEntry; // map for this class
}XEventSinkTable,*PXEventSinkTable;

struct XSINKEVENT{
	XSINKEVENT(int eventKind){
    m_eventKind=eventKind;
    m_dispid=DISPID_UNKNOWN;
    m_pDispParams=NULL;
    m_pExcepInfo=NULL;
    m_puArgError=NULL;
    m_hResult=S_OK;
  }

	XSINKEVENT(int eventKind, DISPID dispid, DISPPARAMS* pDispParams=NULL,
		EXCEPINFO* pExcepInfo=NULL,UINT* puArgError=NULL){
    m_eventKind=eventKind;
    m_dispid=dispid;
    m_pDispParams=pDispParams;
    m_pExcepInfo=pExcepInfo;
    m_puArgError=puArgError;
    m_hResult=S_OK;
  }

  enum{XS_EVENT,XS_PROPREQUEST,XS_PROPCHANGED};

	int m_eventKind;
	DISPID m_dispid;
	DISPPARAMS* m_pDispParams;
	EXCEPINFO* m_pExcepInfo;
	UINT* m_puArgError;
	BOOL m_bPropChanged;
	HRESULT m_hResult;
};

/**************************************************************/
// XEvent REFERENCE...
/**************************************************************/

class XEvent:public XObject{
  DECLARE_DYNAMIC(XEvent)
  public:
    virtual ~XEvent(){}
    virtual VOID OnIdle(){};
  protected:
    //def constructor
    XEvent():m_bCached(FALSE),m_dwRef(0){}

    //internal event's methods
    BOOL MakeEventsCache(BOOL bAppCache=FALSE);
    LRESULT CallEventProc(const struct _XEventTableEntry *pEntry,WPARAM wParam=0,LPARAM lParam=0);
    HRESULT CallMemberFunc(const struct _XEventSinkTableEntry *pEntry,WORD wFlags,VARIANT* pvarResult=NULL,DISPPARAMS* pDispParams=NULL,UINT* puArgErr=NULL);

    //for ole objects
    DWORD InternalQueryInterface(const VOID *iid,LPVOID* ppvObj);
    DWORD InternalAddRef();
    DWORD InternalRelease();
    LPUNKNOWN GetInterface(const VOID *iid);
    LPUNKNOWN QueryAggregates(const VOID *iid);

    //sink events...
    const struct _XEventSinkTableEntry* GetEventSinkEntry(UINT idCtrl, XSINKEVENT* pEvent);

    //used for speed...
    std::list < struct _XEventTableEntry* > m_eventscache, m_commandscache , m_notifyscache;

  private:
    BOOL m_bCached;//first cach events
    LONG m_dwRef;

  private://manual definition of event table
    static const struct _XEventTableEntry _entries[];
    static const struct _XInterfaceEventTableEntry _ientries[];
    static const struct _XEventSinkTableEntry _sentries[];

  protected:
    virtual const XEventTable* GetEventTable()const{
      return GetThisEventTable();
    }
    static const XEventTable* GetThisEventTable();

    virtual const XInterfaceEventTable* GetInterfaceEventTable()const{
      return GetThisInterfaceEventTable();
    };
    static const XInterfaceEventTable* GetThisInterfaceEventTable();

    virtual const XEventSinkTable* GetEventSinkTable()const{
      return GetThisEventSinkTable();
    };
    static const XEventSinkTable* GetThisEventSinkTable();
};

typedef XEvent XEVENT,*PXEVENT;

//base type for event function
typedef VOID (XEvent::*tXCallFunc)(VOID);

//Functions Definition...
typedef INT (XEvent::*tIVEvtFunc)();
typedef INT (XEvent::*tIWEvtFunc)(WPARAM);
typedef INT (XEvent::*tILEvtFunc)(LPARAM);
typedef INT (XEvent::*tIWLpEvtFunc)(WPARAM,LPARAM);
typedef VOID (XEvent::*tVVEvtFunc)();
typedef VOID (XEvent::*tVWEvtFunc)(WPARAM);
typedef VOID (XEvent::*tVLEvtFunc)(LPARAM);
typedef VOID (XEvent::*tVWLpEvtFunc)(WPARAM,LPARAM);

//Functions Definition (OLE)...
typedef BOOL (XEvent::*tBVEvtFunc)();//plus variable length args...
typedef BOOL (XEvent::*tBBEvtFunc)(PBOOL);//plus variable length args...
typedef BOOL (XEvent::*tBUEvtFunc)(UINT);//plus variable length args...
typedef BOOL (XEvent::*tBUBEvtFunc)(UINT,PBOOL);//plus variable length args...

//Estructura que contendrá los eventos estáticos...
typedef struct _XEventTableEntry{
  UINT uMsg;     //identificador del evento
  UINT uCode;    //identificador del control o ventana
  UINT uMinId;   //mínimo identificador (si envio un rango o el mensaje es WM_NOTIFY)
  UINT uMaxId;   //máximo identificador (si envio un rango o el mensaje es WM_NOTIFY)
  UINT uType;    //tipo de funcion (para el cast de Function)
  tXCallFunc pEventFunc;//direccion de la funcion
  BOOL bForce;   //force send reflected msg (-1 all, 0 child, 1 parent)
}XEventTableEntry,*PXEventTableEntry;

typedef struct _XInterfaceEventTableEntry{
	const PVOID piid;       // the interface id (IID) (NULL for aggregate)
	size_t nOffset;         // offset of the interface vtable from m_unknown
}XInterfaceEventTableEntry,*PXInterfaceEventTableEntry;

typedef struct _XEventSinkTableEntry{
  DISPID lDispID;         // DISPID (may be DISPID_UNKNOWN)
  LPCWSTR lpszParams;     // parameters description
  WORD vt;                // return value type / or type of property
  tXCallFunc pfn;      // normal member On<membercall> or, OnGet<property>
	tXCallFunc pfnSet;   // special member for OnSet<property>
	size_t nPropOffset;     // offset of the interface vtable from m_unknown
	UINT uMinId;            // mínimo identificador (si envio un rango o el mensaje es WM_NOTIFY)
  UINT uMaxId;            // máximo identificador (si envio un rango o el mensaje es WM_NOTIFY)
}XEventSinkTableEntry,*PXEventSinkTableEntry;

#endif /* _XEVENT_H_ */
