#ifndef _XOBJECT_H_
#define _XOBJECT_H_

/* Windows Versions */
#define _WIN32_WIN10    0x0A00
#define _WIN32_WINBLUE  0x0603
#define _WIN32_WIN8     0x0602
#define _WIN32_W7       0x0601
#define _WIN32_WVISTA   0x0600
#define _WIN32_W2K3     0x0502
#define _WIN32_WXP      0x0501
#define _WIN32_W2K      0x0500
#define _WIN32_WNT      0x0400

//Old Windows Versions (only for compatibility)
#define _WIN32_WME     0x0490
#define _WIN32_W98     0x0410

/* Internet Explorer Versions */
#define _WIN32_IE90    0x0900
#define _WIN32_IE80    0x0800
#define _WIN32_IE70    0x0700
#define _WIN32_IE60SP2 0x0603
#define _WIN32_IE60SP1 0x0601
#define _WIN32_IE60    0x0600
#define _WIN32_IE55    0x0550
#define _WIN32_IE501   0x0501
#define _WIN32_IE50    0x0500

//Old IE Versions (only for compatibility)
#define _WIN32_IE41    0x0401
#define _WIN32_IE4     0x0400
#define _WIN32_IE3     0x0300

//Set Default OS WinXP, all others are obsolete...
#ifndef OSVERSION
  #define OSVERSION _WIN32_WXP
#endif

#if (OSVERSION >= _WIN32_W2K)
  #define WINVER          OSVERSION
  #define _WIN32_WINNT    OSVERSION
  #define _WIN32_WINDOWS  OSVERSION
#else
  #define _WIN32_WINDOWS  OSVERSION
#endif

#ifndef IEVERSION
  #if (OSVERSION >= _WIN32_W2K)
    #if (OSVERSION >= _WIN32_WXP)
      #if (OSVERSION >= _WIN32_WVISTA)
        #if (OSVERSION >= _WIN32_W7)
          #define IEVERSION _WIN32_IE80
        #else
          #define IEVERSION _WIN32_IE70
        #endif
      #else
        #define IEVERSION _WIN32_IE60
      #endif
    #else
      #define IEVERSION _WIN32_IE501
    #endif
  #else
    #define IEVERSION _WIN32_IE41
  #endif
#endif

#define _WIN32_IE IEVERSION

#if (_WIN32_WINNT >= 0x501)
  #define ICC_NTCLASSES ICC_LINK_CLASS|ICC_STANDARD_CLASSES
#else
  #define ICC_NTCLASSES 0
#endif /* _WIN32_WINNT */

#if (_WIN32_IE >= 0x0400)
  #define IE4CLASSES \
    ICC_INTERNET_CLASSES|ICC_PAGESCROLLER_CLASS|ICC_NATIVEFNTCTL_CLASS
#else
  #define IE4CLASSES 0
#endif /* _WIN32_IE */

#define ICC_ALLCLASSES \
  ICC_ANIMATE_CLASS|ICC_BAR_CLASSES|ICC_COOL_CLASSES|\
  ICC_DATE_CLASSES|ICC_HOTKEY_CLASS|ICC_LISTVIEW_CLASSES|\
  ICC_PROGRESS_CLASS|ICC_TAB_CLASSES|ICC_TREEVIEW_CLASSES|\
  ICC_UPDOWN_CLASS|ICC_USEREX_CLASSES|ICC_WIN95_CLASSES|\
  ICC_NTCLASSES|IE4CLASSES

#include <windows.h>
#include <commctrl.h>
#include <uxtheme.h>
#include <wininet.h>
#include <tchar.h>
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <math.h>
#include <malloc.h>

#include <map>
#include <list>

#include "xundef.h"

/*******************************************************************/
// Dbg Macros
/*******************************************************************/

BOOL DbgPrint(LPCTSTR form, ...);
BOOL DbgMsg(LPCTSTR form, ...);
VOID MessageBoxF(HWND hwnd, LPCTSTR caption, LPCTSTR form, ...);
VOID DbgAssert(LPCTSTR pCondition,LPCTSTR pFileName,INT iLine,LPCTSTR pMsg=NULL);
VOID ShowLastError(UINT errcode=0);

#ifdef _DEBUG

  #ifndef XASSERT
    #define XASSERT(_x_)                                                                 \
    if(!(_x_))                                                                           \
      DbgAssert(_T(#_x_),_T(__FILE__),__LINE__);
  #endif

  #ifndef XASSERTEX
    #define XASSERTEX(_x_,msg)                                                           \
    if(!(_x_))                                                                           \
      DbgAssert(_T(#_x_),_T(__FILE__),__LINE__,msg);
  #endif

  #ifndef XTRACE
    #define XTRACE(exp) DbgPrint(_T("TRACE: %s\n"),#exp);
  #endif

  #ifndef XTRACEEX
    #define XTRACEEX(msg) DbgPrint(_T("TRACEEX: %s\n"),msg);
  #endif

  #ifndef XVERIFY
    #define XVERIFY(exp)                                                                 \
    (!(exp)) ? DbgPrint(_T("WARNING!! Verify %s Failed!\n"),#exp) : TRUE
  #endif

#else

  #ifdef _DEBUGEX

    #ifndef XASSERT
      #define XASSERT(_x_)                                                                 \
      if(!(_x_))                                                                           \
        DbgPrint(_T("ERROR!! Assert %s Failed At line %d of %s\n"),#_x_,__LINE__,TEXT(__FILE__));
    #endif

    #ifndef XASSERTEX
      #define XASSERTEX(_x_,msg)                                                           \
      if(!(_x_))                                                                           \
        DbgPrint(_T("ERROR!! Assert %s Failed At line %d of %s: %s!\n"),#_x_,__LINE__,TEXT(__FILE__),msg);
    #endif

    #ifndef XTRACE
      #define XTRACE(exp) DbgPrint(_T("TRACE: %s\n"),#exp);
    #endif

    #ifndef XTRACEEX
      #define XTRACEEX(msg) DbgPrint(_T("TRACEEX: %s\n"),msg);
    #endif

    #ifndef XVERIFY
      #define XVERIFY(exp)                                                                 \
      (!(exp)) ? DbgPrint(_T("WARNING!! Verify %s Failed!\n"),#exp) : TRUE
    #endif

  #else

    #ifndef XASSERT
      #define XASSERT(_x_) ((void)0);
    #endif

    #ifndef XASSERTEX
      #define XASSERTEX(_x_,msg) ((void)0);
    #endif

    #ifndef XTRACE
      #define XTRACE(exp) ((void)0);
    #endif

    #ifndef XTRACEEX
      #define XTRACEEX(msg) ((void)0);
    #endif

    #ifndef XVERIFY
      #define XVERIFY(exp) ((void)0);
    #endif

  #endif

#endif

#ifndef XVER
  #define XVER 0x0001
#endif

#ifdef _DEBUG
  #define XUNUSED(x)
#else
  #define XUNUSED(x) UNREFERENCED_PARAMETER(x);
#endif

#define XUNUSED_ALWAYS(x) UNREFERENCED_PARAMETER(x);

#ifndef XINLINE
  #define XINLINE inline
#endif

/*#ifndef XPACKED
	#define XPACKED    4
#endif*/

/*******************************************************************/
// Class Macros
/*******************************************************************/

#define DECLARE_NO_COPY(classname)                                            \
  private:                                                                    \
    classname(const classname&);                                              \
	  void operator=(const classname&);

#define CLASS_INFO(classname) (classname::m_##classname##Info)

#define DECLARE_DYNAMIC(classname)                                            \
  public:                                                                     \
    static XClassInfo m_##classname##Info;                                    \
    virtual XClassInfo *GetClassInfo() const;

#define DECLARE_DYNCREATE(classname)                                          \
	DECLARE_DYNAMIC(classname)                                                  \
	static XObject* CreateObject();

#define DECLARE_CLASS(classname) DECLARE_DYNAMIC(classname)

#define IMPLEMENT_CLASS(classname,baseclass1,baseclass2,pfNew)                \
  XClassInfo CLASS_INFO(classname)(#classname,sizeof(classname),              \
    baseclass1,baseclass2,pfNew);                                             \
  XClassInfo *classname::GetClassInfo() const                                 \
    { return &CLASS_INFO(classname); }

#define IMPLEMENT_DYNAMIC_BASE(classname)                                     \
    IMPLEMENT_CLASS(classname,NULL,NULL,NULL)

#define IMPLEMENT_DYNAMIC(classname,basename1)                                \
    IMPLEMENT_CLASS(classname,&CLASS_INFO(basename1),NULL,NULL)

#define IMPLEMENT_DYNAMIC2(classname,basename1,basename2)                     \
    IMPLEMENT_CLASS(classname,&CLASS_INFO(basename1),&CLASS_INFO(basename2), NULL)

#define IMPLEMENT_DYNCREATE(classname,basename1)                              \
	XObject* classname::CreateObject()                                          \
		{ return new classname; }                                                 \
	IMPLEMENT_CLASS(classname,&CLASS_INFO(basename1),NULL,classname::CreateObject)

#define IMPLEMENT_DYNCREATE2(classname,basename1,basename2)                   \
	XObject* classname::CreateObject()                                          \
		{ return new classname; }                                                 \
	IMPLEMENT_CLASS(classname,&CLASS_INFO(basename1),&CLASS_INFO(basename2),classname::CreateObject)

#define RUNTIME_CLASS(classname) ((XClassInfo*)&CLASS_INFO(classname))

#define XASSERT_KINDOF(classname, object) \
	XASSERT((object)->IsKindOf(RUNTIME_CLASS(classname)))

/*******************************************************************/
// XClassInfo
/*******************************************************************/

class XObject;

struct XClassInfo{

  XClassInfo(LPCSTR className,UINT size,XClassInfo *baseClass1,
    XClassInfo *baseClass2,XObject* (*pCreateObject)()):
    m_className(className),m_objectSize(size),m_baseClass1(baseClass1),
    m_baseClass2(baseClass2),m_pCreateObject(pCreateObject),m_next(sm_first){
      sm_first=this;
  }

  ~XClassInfo();

  LPCSTR GetClassName() const { return m_className; }
  UINT GetSize() const { return m_objectSize; }

  LPCSTR GetBaseClassName1() const{
    return m_baseClass1 ? m_baseClass1->GetClassName() : NULL;
  }
  LPCSTR GetBaseClassName2() const{
    return m_baseClass2 ? m_baseClass2->GetClassName() : NULL;
  }

  const XClassInfo *GetBaseClass1() const { return m_baseClass1; }
  const XClassInfo *GetBaseClass2() const { return m_baseClass2; }

  static const XClassInfo *FindClass(LPCSTR className);
  static const XClassInfo *GetFirst() { return sm_first; }
  const XClassInfo *GetNext() const { return m_next; }

  BOOL IsKindOf(const XClassInfo *info) const;

  XObject *CreateObject();

  LPCSTR m_className;
  UINT m_objectSize;

  const XClassInfo *m_baseClass1,*m_baseClass2;
  XObject* (*m_pCreateObject)();

  static XClassInfo *sm_first;
  XClassInfo *m_next;

  DECLARE_NO_COPY(XClassInfo)
};

/*******************************************************************/
// XObject
/*******************************************************************/

class XObject{
  DECLARE_NO_COPY(XObject)
  protected:
	  XObject(){};
  public:
    //pure virtual destructor; should always be empty
    virtual ~XObject(){}
/*
    //this 4 are always static!
    void* operator new(size_t sz);//always is static...
    void  operator delete(void* m);//always is static...
    void* operator new[](size_t sz);//always is static...
    void  operator delete[](void* m);//always is static...
*/

    BOOL IsKindOf(const XClassInfo *info){
      XClassInfo *thisInfo=GetClassInfo();
      return thisInfo->IsKindOf(info);
    }

  #ifdef _DEBUG

    BOOL AssertValid()const{
      XASSERT(this!=NULL)
      return (this!=NULL);
    }

    VOID Dump(){
      DbgPrint(_T("Class %s at %X\n"),GetClassInfo()->m_className,(void*)this);
    }

  #endif //DEBUG

  public:
    static XClassInfo m_XObjectInfo;
    virtual XClassInfo *GetClassInfo() const;
};

/*******************************************************************/
// XException
/*******************************************************************/

class XException:public XObject{
	DECLARE_DYNAMIC(XException)
  public:
    //XException(BOOL bAutoDelete=TRUE):m_bAutoDelete(bAutoDelete){};
    XException():m_dwError(0),m_szErrorString(NULL){};
    //pure virtual destructor; should always be empty
    virtual ~XException(){}

    virtual DWORD GetError(){return m_dwError;}
    virtual LPCTSTR GetErrorMessage(){return m_szErrorString;}

    /*VOID Delete(){
      if(m_bAutoDelete)
        delete this;
    }*/

  private:
    DWORD  m_dwError;
		LPCTSTR m_szErrorString;
    //BOOL m_bAutoDelete;
};

/*******************************************************************/
// XGC (Deprecated)
/*******************************************************************/
/*
class XGCNodo{
  friend class XGC;
  private:
    XGCNodo(PVOID value){
      val = value;
      next = NULL;
      bDel = FALSE;
    }
    XGCNodo * next;
    BOOL bDel;
    PVOID val;
};

class XGC{
  private:
    XGC();
    ~XGC();
    //Pure Memory and List operations...
    PVOID Append(size_t sz)throw (bad_alloc);
    VOID Remove(PVOID val)throw();

  public:

    //Memory operations...
    static XGC  *StartCollect()throw();
    static VOID *Alloc(size_t sz)throw (bad_alloc);
    static VOID Free(PVOID val)throw();
    static INT  ForceCollect()throw();
    static VOID FreeCollect()throw();

    HANDLE GetExitEvent(){return hExitEvent;}

  private:
    //Collect timing Thread
    static DWORD WINAPI CollectThread(LPVOID params);

    //Extern operations
    static XGC *GetpGC();
    INT  Collect()throw();
    VOID FreeObjects()throw();

    XGCNodo *first,*last;
    UINT nelem;
    DWORD cThid;HANDLE hExitEvent,hThread;
    static class XGC* pgc;
};*/

/*******************************************************************/
//
/*******************************************************************/

#endif //_XOBJECT_H_
