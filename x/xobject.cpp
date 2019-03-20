#include "xobject.h"

/*******************************************************************/
// Dbg Function
/*******************************************************************/

//#define OUTPUTCONSOLE

#ifdef _DEBUG

BOOL DbgPrint(LPCTSTR form, ...){
  static TCHAR tmp[4096];va_list p;
  va_start(p,form);
  wvsprintf(tmp,form,p);
#ifdef OUTPUTCONSOLE
  ::_putts(tmp);
#else
  ::OutputDebugString(tmp);
#endif
  va_end(p);
  return FALSE;
}

BOOL DbgMsg(LPCTSTR form, ...){
  static TCHAR tmp[4096];va_list p;
  va_start(p,form);
  wvsprintf(tmp,form,p);
  ::MessageBox(0,tmp,_T("DEBUG"),MB_ICONINFORMATION);
  va_end(p);
  return FALSE;
}

VOID MessageBoxF(HWND hwnd, LPCTSTR caption, LPCTSTR form, ...){
  static TCHAR tmp[4096];va_list p;
  va_start(p,form);
  wvsprintf(tmp,form,p);
  ::MessageBox(hwnd,tmp,caption,MB_ICONINFORMATION);
  va_end(p);
}

VOID DbgAssert(LPCTSTR pCondition,LPCTSTR pFileName,INT iLine,LPCTSTR pMsg){
  TCHAR tmp[4096];
  if(pMsg)
    wsprintf(tmp,_T("Condition %s Failed\nAt line %d of %s\nMsg: %s\nContinue? (Cancel to debug)"),pCondition,iLine,pFileName,pMsg);
  else
    wsprintf(tmp,_T("Condition %s Failed\nAt line %d of %s\nContinue? (Cancel to debug)"),pCondition, iLine, pFileName);
  switch(::MessageBox(NULL,tmp,_T("ERROR!! ASSERT Failed!"),MB_SYSTEMMODAL|MB_ICONHAND|MB_YESNOCANCEL|MB_SETFOREGROUND)){
    case IDNO:
      ::FatalAppExit(FALSE,_T("Application terminated"));
    break;
    case IDCANCEL:
      ::DebugBreak();
    break;
    case IDYES:break;
  }
}

VOID ShowLastError(UINT errcode){
  LPVOID lpMsgBuf;TCHAR swCaption[24];
  if(!errcode)errcode=GetLastError();
  _stprintf(swCaption,_T("Error: 0x%X"),errcode);
  FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM,NULL,errcode,0,(LPTSTR) &lpMsgBuf,0,NULL);
  MessageBox(NULL,(TCHAR*)lpMsgBuf,swCaption,MB_ICONEXCLAMATION|MB_OK|MB_TASKMODAL);
  LocalFree(lpMsgBuf);SetLastError(0);
}

#endif// _DEBUG

/*******************************************************************/
// XException
/*******************************************************************/

IMPLEMENT_DYNAMIC(XException, XObject)

/*******************************************************************/
// XObject
/*******************************************************************/

XClassInfo XObject::m_XObjectInfo("XObject",sizeof(XObject),NULL,NULL,NULL);

XClassInfo *XObject::GetClassInfo() const{
  return &m_XObjectInfo;
}

/*******************************************************************/
// XClassInfo
/*******************************************************************/

struct XClassInfo *XClassInfo::sm_first=NULL;

XClassInfo::~XClassInfo(){
  if(sm_first==this){
    sm_first=m_next;
  }else{
    XClassInfo *info=sm_first;
    while(info){
      if(info->m_next==this){
        info->m_next=m_next;
        break;
      }
      info=info->m_next;
    }
  }
}

BOOL XClassInfo::IsKindOf(const XClassInfo *info) const{
  return (info&&(info==this||
    (m_baseClass1&&m_baseClass1->IsKindOf(info))||
    (m_baseClass2&&m_baseClass2->IsKindOf(info))));
}

const XClassInfo *XClassInfo::FindClass(LPCSTR className){
  for(XClassInfo *info=sm_first;info;info=info->m_next){
    if(strcmp(info->GetClassName(),className)==0)
      return info;
  }
  return NULL;
}

XObject *XClassInfo::CreateObject() {
  return (m_pCreateObject?(*m_pCreateObject)():NULL);
}

/*
void* XObject::operator new(size_t sz)throw (bad_alloc){
  return XGC::Alloc(sz);
}

void XObject::operator delete(void* m)throw(){
  XGC::Free(m);
}

void* XObject::operator new[](size_t sz)throw (bad_alloc){
  return operator new(sz);
}

void XObject::operator delete[](void* m)throw(){
  operator delete(m);
}*/

/*******************************************************************/
// XGC (Deprecated)
/*******************************************************************/
/*
class XGC* XGC::pgc=NULL;

XGC::XGC(){
  XTRACE("XGC -> XGC");
  nelem=0;first=last=NULL;hThread=NULL;
  //Creamos Evento...
  if((hExitEvent=CreateEvent(NULL,FALSE,FALSE,T("#XGGC#EXITEVENT#XGGC#")))!=NULL)
    hThread=CreateThread(NULL,0,CollectThread,(PVOID)this, 0, &cThid);
}

XGC::~XGC(){
  XTRACE("XGC -> ~XGC");
  if(hExitEvent){
    DWORD dwExitCode;
    SetEvent(hExitEvent);
    CloseHandle(hExitEvent);
    hExitEvent=NULL;
    if(hThread)
    while(GetExitCodeThread(hThread,&dwExitCode)==STILL_ACTIVE)
      Sleep(10);
    CloseHandle(hThread);
    hThread=NULL;
  }
  FreeObjects();
}

XGC* XGC::GetpGC(){
  XTRACE("XGC -> GetpGC");
  return pgc;
}

XGC *XGC::StartCollect()throw(){
  XTRACE("XGC -> StartCollect");
  if(!pgc)pgc=new XGC;
  return pgc;
}

VOID* XGC::Alloc(size_t sz)throw (bad_alloc){
  XTRACE("XGC -> Alloc");
  if(pgc)return pgc->Append(sz);
  else return operator new(sz);
}

VOID XGC::Free(PVOID val)throw(){
  XTRACE("XGC -> Free");
  if(pgc)pgc->Remove(val);
  else operator delete(val);
}

INT  XGC::ForceCollect()throw(){
  XTRACE("XGC -> ForceCollect");
  if(pgc)pgc->Collect();
}

VOID XGC::FreeCollect()throw(){
  XTRACE("XGC -> FreeCollect");
  if(pgc)delete pgc;
}

//Append new item...
PVOID XGC::Append(size_t sz)throw (bad_alloc){
  XTRACE("XGC -> Append");
  PVOID m=operator new(sz);
  if(m){
    XGCNodo *pt=new XGCNodo(m);
    if(first==NULL)first=pt;
    else last->next=pt;
    last=pt;++nelem;
  }
  return m;
}

//Mark for delete...
VOID XGC::Remove(PVOID val)throw(){
  XTRACE("XGC -> Remove");
  for(XGCNodo *pt=first;pt&&val;pt=pt->next){
    if(pt&&pt->val==val){
      pt->bDel=TRUE;
      break;
    }
  }
}

//Free marked memory...
INT XGC::Collect()throw(){
  XTRACE("XGC -> Collect");
  XGCNodo *pt=first;
  XGCNodo *nxt,*prv= NULL;
  while(pt){
    nxt = pt->next;
    if(pt->bDel==true){
      //Reorganice list..
      if(first==pt)first=nxt;
      else prv->next=nxt;
      //Free memory...
      operator delete(pt->val);
      delete pt;--nelem;
    }else
      prv = pt;
    pt = nxt;
  }
  return nelem;
}

VOID XGC::FreeObjects()throw(){
  XTRACE("XGC -> FreeObjects");
  XGCNodo *nxt=NULL,*pt=first;
  while(pt){
    first=pt->next;
    if(pt->val){
      if(!pt->bDel)//Call destructor...
        delete (Object *)(pt->val);
      //Free memory...
      operator delete(pt->val);
    }
    delete pt;pt=first;
  }
  first=NULL;
  pgc=NULL;
}

////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////

DWORD WINAPI XGC::CollectThread(LPVOID params){
  XTRACE("XGC -> CollectThread");
  CRITICAL_SECTION CollectThreadCS;
  if(InitializeCriticalSectionAndSpinCount(&CollectThreadCS,0x80000400)){
    XGC *pGC=(XGC*)params;
    if(pGC){
      HANDLE hExitEvent=pGC->GetExitEvent();
      for(;;){
        DWORD dwRes=WaitForSingleObject(hExitEvent,XCOLLECTTIME);
        switch(dwRes){
          case WAIT_TIMEOUT:
            EnterCriticalSection(&CollectThreadCS);
            pGC->Collect();
            LeaveCriticalSection(&CollectThreadCS);
          break;
          case WAIT_ABANDONED_0:case WAIT_OBJECT_0:
            ExitThread(0);
          break;
        }
      }
    }
    DeleteCriticalSection(&CollectThreadCS);
  }
  //Finalizamos thread...
  ExitThread(0);
}
*/
