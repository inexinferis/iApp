#include "xthread.h"

/**************************************************************/
// XThread...
/**************************************************************/

IMPLEMENT_DYNAMIC(XTls,XObject)
IMPLEMENT_DYNAMIC(XThread,XObject)

HANDLE XThread::Create(LPSECURITY_ATTRIBUTES lpThreadAttributes,SIZE_T dwStackSize,
  LPTHREAD_START_ROUTINE lpStartAddress,LPVOID lpParameter,DWORD dwCreationFlags,LPDWORD lpThreadId){
  XASSERT(m_hThread==NULL)
  return Attach(::CreateThread(lpThreadAttributes,dwStackSize,lpStartAddress,lpParameter,dwCreationFlags,lpThreadId));
}

HANDLE XThread::CreateRemote(HANDLE hProcess,LPSECURITY_ATTRIBUTES lpThreadAttributes,SIZE_T dwStackSize,
  LPTHREAD_START_ROUTINE lpStartAddress,LPVOID lpParameter,DWORD dwCreationFlags,LPDWORD lpThreadId){
  XASSERT(m_hThread==NULL)
  return Attach(::CreateRemoteThread(hProcess,lpThreadAttributes,dwStackSize,lpStartAddress,lpParameter,dwCreationFlags,lpThreadId));
}

HANDLE XThread::Create(LPTHREAD_START_ROUTINE lpStartAddress,LPVOID lpParameter,LPDWORD lpThreadId,BOOL bSuspended){
  XASSERT(m_hThread==NULL)
  return Create(NULL,0,lpStartAddress,lpParameter,(bSuspended)?CREATE_SUSPENDED:0,lpThreadId);
}

HANDLE XThread::CreateRemote(HANDLE hProcess,LPTHREAD_START_ROUTINE lpStartAddress,LPVOID lpParameter,LPDWORD lpThreadId,BOOL bSuspended){
  XASSERT(m_hThread==NULL)
  return CreateRemote(hProcess,NULL,0,lpStartAddress,lpParameter,(bSuspended)?CREATE_SUSPENDED:0,lpThreadId);
}

#if (_WIN32_WINDOWS >= 0x0600)
//Not Defined in Mingw WinAPI 3.17
/*HANDLE XThread::CreateRemoteEx(HANDLE hProcess,LPTHREAD_START_ROUTINE lpStartAddress,LPVOID lpParameter,
  LPPROC_THREAD_ATTRIBUTE_LIST lpAttributeList,LPDWORD lpThreadId,BOOL bSuspended){
  XASSERT(m_hThread==NULL)
  return Attach(::CreateRemoteThreadEx(hProcess,NULL,0,lpStartAddress,lpParameter,(bSuspended)?CREATE_SUSPENDED:0,lpAttributeList,lpThreadId));
}

HANDLE XThread::CreateRemoteEx(HANDLE hProcess,LPSECURITY_ATTRIBUTES lpThreadAttributes,SIZE_T dwStackSize,
  LPTHREAD_START_ROUTINE lpStartAddress,LPVOID lpParameter,DWORD dwCreationFlags,
  LPPROC_THREAD_ATTRIBUTE_LIST lpAttributeList,LPDWORD lpThreadId){
  XASSERT(m_hThread==NULL)
  return Attach(::CreateRemoteThreadEx(hProcess,lpThreadAttributes,dwStackSize,lpStartAddress,lpParameter,dwCreationFlags,lpAttributeList,lpThreadId));
}*/
#endif

HANDLE XThread::Create(XObject *ThreadObj,LPXTHREAD_START_ROUTINE lpStartAddress,
LPSECURITY_ATTRIBUTES lpThreadAttributes,SIZE_T dwStackSize,LPVOID lpParameter,
DWORD dwCreationFlags,LPDWORD lpThreadId){
  XASSERT(m_hThread==NULL)
  m_lpParameter=lpParameter;
  m_ThreadObj=(XThread*)ThreadObj;
  m_ThreadProc=lpStartAddress;
  return Create(lpThreadAttributes,dwStackSize,XThreadProc,this,dwCreationFlags,lpThreadId);
}

HANDLE XThread::Create(XObject *ThreadObj,LPXTHREAD_START_ROUTINE lpStartAddress,
LPVOID lpParameter,LPDWORD lpThreadId,BOOL bSuspended){
  XASSERT(m_hThread==NULL)
  return Create(ThreadObj,lpStartAddress,NULL,0,lpParameter,(bSuspended)?CREATE_SUSPENDED:0,lpThreadId);
}

HANDLE XThread::Create(LPSECURITY_ATTRIBUTES lpThreadAttributes,SIZE_T dwStackSize,
LPVOID lpParameter,DWORD dwCreationFlags,LPDWORD lpThreadId){
  XASSERT(m_hThread==NULL)
  return Create(this,&XThread::ThreadProc,lpThreadAttributes,dwStackSize,lpParameter,dwCreationFlags,lpThreadId);
}

HANDLE XThread::Create(LPVOID lpParameter,LPDWORD lpThreadId,BOOL bSuspended){
  XASSERT(m_hThread==NULL)
  return Create(this,&XThread::ThreadProc,NULL,0,lpParameter,(bSuspended)?CREATE_SUSPENDED:0,lpThreadId);
}

DWORD WINAPI XThread::XThreadProc(LPVOID lpParameter){
  XThread *pThread=(XThread*)lpParameter;
  if(pThread&&pThread->m_ThreadObj&&pThread->m_ThreadProc){
    pThread->ExitThread(((pThread->m_ThreadObj)->*(pThread->m_ThreadProc))(pThread->m_lpParameter));
    return 0;
  }
  ::ExitThread(0);
  return 0;
}

DWORD XThread::ThreadProc(LPVOID /*lpParameter*/){
  return 0;
}
