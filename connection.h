


#ifndef _CONNECTION_H_
#define _CONNECTION_H_

#include "x/xinternet.h"
#include "utils.h"
#include <winsock2.h>

enum{//g_ConStrMap
  IConHeader=0, //0
  IConDomain,   //1
  IConUrl,      //2
  IConClient,   //3
  IConHttp,     //4
};

#define INET_ERROR_BUFFER_SIZE   512
#define MAX_PROT_BUFSIZE         2048000

class XURL{
  public:
    XURL(PTCHAR args=NULL){
      Clear();
      if(args)
        lstrcpy(g_buffer,args);
    }

    ~XURL(){
      Clear();
    }

    VOID Clear(){
      g_buffer[0]=0;
    }

    PTCHAR Get(){
      return g_buffer;
    }

    PTCHAR Append(LPCTSTR arg,LPCTSTR value){
      TCHAR tmp[2048];
      UINT len=lstrlen(g_buffer);
      if(!g_buffer[0]||(len&&g_buffer[len-1]=='?'))
        wsprintf(g_buffer,_T("%s=%s"),arg,value);
      else{
        wsprintf(tmp,_T("&%s=%s"),arg,value);
        lstrcat(g_buffer,tmp);
      }
      return g_buffer;
    }

    PTCHAR Append(LPCTSTR arg,UINT value){
      TCHAR tmp[2048];
      UINT len=lstrlen(g_buffer);
      if(!g_buffer[0]||(len&&g_buffer[len-1]=='?'))
        wsprintf(g_buffer,_T("%s=%d"),arg,value);
      else{
        wsprintf(tmp,_T("&%s=%d"),arg,value);
        lstrcat(g_buffer,tmp);
      }
      return g_buffer;
    }

    operator PTCHAR(){
      return g_buffer;
    }

    XURL &operator=(PTCHAR args){
      lstrcpy(g_buffer,args);
      return *this;
    }

  private:
    TCHAR g_buffer[2048];
};

class InternetStatus:public XInternet{
  public:
    virtual BOOL OnStatusCallback(HINTERNET hInternet,DWORD dwContext,DWORD dwInternetStatus,
      LPVOID lpvStatusInformation,DWORD dwStatusInformationLength);
};

class Connection{
  public:
    Connection():dwRequestSize(0),fTransferRate(0){
    }
    ~Connection(){}

    VOID SetStatusCallbackEx(XObject *StatusCBObj,LPXINTERNET_STATUS_CALLBACK StatusCBProc){
      XASSERT(StatusCBObj!=NULL&&StatusCBProc!=NULL)
      iSession.SetStatusCallbackEx(StatusCBObj,StatusCBProc);
    }

    VOID RemoveStatusCallbackEx(){
      iSession.RemoveStatusCallbackEx();
    }

    DWORD GetStatus();
    DWORD CheckUpdate();
    DWORD Update();
    DWORD Login();
    DWORD GetFile();
    DWORD Activate();
    DWORD Logout();
    LPTSTR GetLastErrorMsg();

    BOOL IsBusy();
    DWORD Connect();
    VOID Disconnect();

    VOID ConStatus(PDWORD total,PDWORD current,float *rate){
      *total=dwRequestSize;
      *current=dwReadedSize;
      *rate=fTransferRate;
    }

  protected:
    DWORD dwRequestSize,dwReadedSize;
    float fTransferRate;

  private:
    struct in_addr* ResolveDomain(LPCSTR host_name);

    struct in_addr* addr;
    XCriticalSection lock;
    InternetStatus iSession;
    XHTTPConnection iConnection;
    XHTTPFile iRequest;
};

extern const TCHAR *g_ConStrMap[];

#endif //_CONNECTION_H_
