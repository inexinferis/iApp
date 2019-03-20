#ifndef _XTHREAD_H_
#define _XTHREAD_H_

#include "xobject.h"
#include "xutils.h"

/**************************************************************/
// XTls...
/**************************************************************/

class XTls:public XObject{
  DECLARE_DYNAMIC(XTls)
  public:
    XTls():m_dwTlsIndex(0){}

    XTls(DWORD dwTlsIndex):m_dwTlsIndex(dwTlsIndex){};
    virtual ~XTls(){}

    static XTls *FromIndex(DWORD dwTlsIndex){
      return new XTls(dwTlsIndex);
    }

    DWORD Attach(DWORD dwTlsIndex){
      XASSERT(m_dwTlsIndex==0)
      m_dwTlsIndex=dwTlsIndex;
      return dwTlsIndex;
    }

    DWORD Detach(){
      XASSERT(m_dwTlsIndex!=0)
      DWORD dwTlsIndex=m_dwTlsIndex;
      m_dwTlsIndex=0;
      return dwTlsIndex;
    }

    DWORD GetIndex(){
      return (this != NULL ? m_dwTlsIndex : 0);
    }

    BOOL IsValid(){
      return (m_dwTlsIndex!=0);
    }

    DWORD Alloc(){
      XASSERT(m_dwTlsIndex==0)
      return Attach(TlsAlloc());
    }

    BOOL Free(){
      XASSERT(m_dwTlsIndex!=0)
      BOOL bRet=TlsFree(m_dwTlsIndex);
      m_dwTlsIndex=0;
      return bRet;
    }

    LPVOID GetValue(){
      XASSERT(m_dwTlsIndex!=0)
      return TlsGetValue(m_dwTlsIndex);
    }

    BOOL SetValue(LPVOID lpTlsValue){
      XASSERT(m_dwTlsIndex!=0)
      return TlsSetValue(m_dwTlsIndex,lpTlsValue);
    }

    XTls &operator=(const DWORD dwTlsIndex){
      XASSERT(m_dwTlsIndex==0)
      m_dwTlsIndex=dwTlsIndex;
      return *this;
    }

    operator DWORD(){
      return m_dwTlsIndex;
    }

  private:
    DWORD m_dwTlsIndex;
};

/**************************************************************/
// XThread...
/**************************************************************/

class XThread;

typedef DWORD(XThread::*LPXTHREAD_START_ROUTINE)(LPVOID);

class XThread:public XObject{
  DECLARE_DYNAMIC(XThread)
  public:
    XThread(HANDLE hThread=NULL):m_hThread(hThread),m_ThreadProc(NULL){
      m_ThreadObj=this;
    }

    virtual ~XThread(){
      if(m_hThread&&m_ThreadProc==&XThread::ThreadProc)
        Terminate();
    }

    static XThread *FromHandle(HANDLE hThread){
      return new XThread(hThread);
    }

    HANDLE Attach(HANDLE hThread){
      XASSERT(m_hThread==NULL)
      if(!hThread)return NULL;
      m_hThread=hThread;
      return hThread;
    }

    HANDLE Detach(){
      HANDLE hThread=m_hThread;
      if(!m_hThread)return NULL;
      m_hThread=NULL;
      return hThread;
    }

    HANDLE GetSafeHandle()const{
      return (this != NULL ? m_hThread : NULL);
    }

    BOOL IsValid(){
      return (m_hThread!=NULL);
    }

    HANDLE FromCurrentThread(){
      return Attach(::GetCurrentThread());
    }

    HANDLE Create(LPVOID lpParameter=NULL,LPDWORD lpThreadId=NULL,BOOL bSuspended=FALSE);

    HANDLE Create(LPSECURITY_ATTRIBUTES lpThreadAttributes,SIZE_T dwStackSize=0,
      LPVOID lpParameter=NULL,DWORD dwCreationFlags=0,LPDWORD lpThreadId=NULL);

    HANDLE Create(XObject *ThreadObj,LPXTHREAD_START_ROUTINE lpStartAddress,LPVOID lpParameter=NULL,LPDWORD lpThreadId=NULL,BOOL bSuspended=FALSE);

    HANDLE Create(XObject *ThreadObj,LPXTHREAD_START_ROUTINE lpStartAddress,LPSECURITY_ATTRIBUTES lpThreadAttributes,
      SIZE_T dwStackSize=0,LPVOID lpParameter=NULL,DWORD dwCreationFlags=0,LPDWORD lpThreadId=NULL);

    HANDLE Create(LPTHREAD_START_ROUTINE lpStartAddress,LPVOID lpParameter=NULL,LPDWORD lpThreadId=NULL,BOOL bSuspended=FALSE);

    HANDLE Create(LPSECURITY_ATTRIBUTES lpThreadAttributes,SIZE_T dwStackSize,
      LPTHREAD_START_ROUTINE lpStartAddress,LPVOID lpParameter=NULL,DWORD dwCreationFlags=0,LPDWORD lpThreadId=NULL);

    HANDLE CreateRemote(HANDLE hProcess,LPTHREAD_START_ROUTINE lpStartAddress,LPVOID lpParameter=NULL,LPDWORD lpThreadId=NULL,BOOL bSuspended=FALSE);

    HANDLE CreateRemote(HANDLE hProcess,LPSECURITY_ATTRIBUTES lpThreadAttributes,SIZE_T dwStackSize,
      LPTHREAD_START_ROUTINE lpStartAddress,LPVOID lpParameter=NULL,DWORD dwCreationFlags=0,LPDWORD lpThreadId=NULL);

  #if (_WIN32_WINDOWS >= 0x0600)
    //Not Defined in Mingw WinAPI 3.17
    /*HANDLE CreateRemoteEx(HANDLE hProcess,LPTHREAD_START_ROUTINE lpStartAddress,LPVOID lpParameter,
      LPPROC_THREAD_ATTRIBUTE_LIST lpAttributeList,LPDWORD lpThreadId=NULL,BOOL bSuspended=FALSE){

    HANDLE CreateRemoteEx(HANDLE hProcess,LPSECURITY_ATTRIBUTES lpThreadAttributes,SIZE_T dwStackSize,LPTHREAD_START_ROUTINE lpStartAddress,
      LPVOID lpParameter,DWORD dwCreationFlags,LPPROC_THREAD_ATTRIBUTE_LIST lpAttributeList,LPDWORD lpThreadId);
    */
  #endif

  #if (_WIN32_WINDOWS >= 0x0500)
    HANDLE Open(DWORD dwDesiredAccess,BOOL bInheritHandle,DWORD dwThreadId){
      XASSERT(m_hThread==NULL)
      return Attach(::OpenThread(dwDesiredAccess,bInheritHandle,dwThreadId));
    }
  #endif

    INT GetPriority(){
      XASSERT(m_hThread!=NULL)
      return ::GetThreadPriority(m_hThread);
    }

    BOOL GetPriorityBoost(PBOOL pDisablePriorityBoost){
      XASSERT(m_hThread!=NULL)
      return ::GetThreadPriorityBoost(m_hThread,pDisablePriorityBoost);
    }

    BOOL SetPriority(INT nPriority){
      XASSERT(m_hThread!=NULL)
      return ::SetThreadPriority(m_hThread,nPriority);
    }

    BOOL SetPriorityBoost(BOOL DisablePriorityBoost){
      XASSERT(m_hThread!=NULL)
      return ::SetThreadPriorityBoost(m_hThread,DisablePriorityBoost);
    }

    DWORD_PTR SetAffinityMask(DWORD_PTR dwThreadAffinityMask){
      XASSERT(m_hThread!=NULL)
      return ::SetThreadAffinityMask(m_hThread,dwThreadAffinityMask);
    }

    DWORD SetIdealProcessor(DWORD dwIdealProcessor){
      XASSERT(m_hThread!=NULL)
      return ::SetThreadIdealProcessor(m_hThread,dwIdealProcessor);
    }

    BOOL GetTimes(LPFILETIME lpCreationTime,LPFILETIME lpExitTime,LPFILETIME lpKernelTime,LPFILETIME lpUserTime){
      XASSERT(m_hThread!=NULL)
      return ::GetThreadTimes(m_hThread,lpCreationTime,lpExitTime,lpKernelTime,lpUserTime);
    }

  #if (_WIN32_WINDOWS >= 0x0502)
    BOOL GetIOPendingFlag(PBOOL lpIOIsPending){
      XASSERT(m_hThread!=NULL)
      return ::GetThreadIOPendingFlag(m_hThread,lpIOIsPending);
    }
  #endif

  #if (_WIN32_WINDOWS >= 0x0600)
    //Not Defined in Mingw WinAPI 3.17
    /*BOOL GetGroupAffinity(PGROUP_AFFINITY GroupAffinity){
      XASSERT(m_hThread!=NULL)
      return ::GetThreadGroupAffinity(m_hThread,GroupAffinity);
    }

    BOOL GetIdealProcessorEx(PPROCESSOR_NUMBER lpIdealProcessor){
      XASSERT(m_hThread!=NULL)
      return ::GetThreadIdealProcessorEx(m_hThread,lpIdealProcessor);
    }

    BOOL SetGroupAffinity(const GROUP_AFFINITY *GroupAffinity,PGROUP_AFFINITY PreviousGroupAffinity){
      XASSERT(m_hThread!=NULL)
      return ::SetThreadGroupAffinity(m_hThread,GroupAffinity,PreviousGroupAffinity);
    }

    BOOL SetIdealProcessorEx(PPROCESSOR_NUMBER lpIdealProcessor,PPROCESSOR_NUMBER lpPreviousIdealProcessor){
      XASSERT(m_hThread!=NULL)
      return ::SetThreadIdealProcessorEx(m_hThread,lpIdealProcessor,lpPreviousIdealProcessor);
    }

    BOOL QueryCycleTime(PULONG64 CycleTime){
      XASSERT(m_hThread!=NULL)
      return ::QueryThreadCycleTime(m_hThread,CycleTime);
    }

    DWORD GetId(){
      XASSERT(m_hThread!=NULL)
      return ::GetThreadId(m_hThread);
    }*/

  #endif

    BOOL WaitForExit(DWORD dwMilliseconds=INFINITE){
      if(m_hThread)
        if(::WaitForSingleObject(m_hThread,dwMilliseconds)==WAIT_OBJECT_0)
          return TRUE;
      return FALSE;
    }

    BOOL GetExitCode(LPDWORD lpExitCode){
      if(m_hThread)
        return ::GetExitCodeThread(m_hThread,lpExitCode);
      return FALSE;
    }

    VOID ExitThread(DWORD dwExitCode=0){
      ::ExitThread(dwExitCode);
    }

    BOOL Terminate(DWORD dwExitCode=0){
      if(m_hThread)
        return ::TerminateThread(m_hThread,dwExitCode);
      return FALSE;
    }

    BOOL Close(){
      if(m_hThread)
        return ::CloseHandle(Detach());
      return FALSE;
    }

    XThread &operator=(const HANDLE hThread){
      XASSERT(m_hThread==NULL)
      m_hThread=hThread;
      return *this;
    }

    operator HANDLE(){
      return m_hThread;
    }

    virtual DWORD ThreadProc(LPVOID);

  private:
    HANDLE m_hThread;
    LPVOID m_lpParameter;
    XThread *m_ThreadObj;
    LPXTHREAD_START_ROUTINE m_ThreadProc;
  private:
    static DWORD WINAPI XThreadProc(LPVOID);
};

#endif //_XTHREAD_H_
