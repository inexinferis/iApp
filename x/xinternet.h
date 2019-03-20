#ifndef _XINTERNET_H_
#define _XINTERNET_H_

#include "xapp.h"
#include "xfile.h"
#include "xutils.h"

#define HTTP_VERB_POST    _T("POST")
#define HTTP_VERB_GET     _T("GET"),
#define HTTP_VERB_HEAD	  _T("HEAD"),
#define HTTP_VERB_PUT 	  _T("PUT"),
#define HTTP_VERB_LINK	  _T("LINK"),
#define HTTP_VERB_DELETE	_T("DELETE"),
#define HTTP_VERB_UNLINK	_T("UNLINK"),

#ifdef _MSC_VER
#pragma comment(lib, "wininet.lib")
#endif

class XInternet;
typedef BOOL (XInternet::*LPXINTERNET_STATUS_CALLBACK)(HINTERNET hInternet, DWORD_PTR dwContext,
  DWORD dwInternetStatus, LPVOID lpvStatusInformation,DWORD dwStatusInformationLength);

class XInternet:public XObject{
  DECLARE_DYNAMIC(XInternet)
  public:
    XInternet(HINTERNET hInternet=NULL):m_hInternet(hInternet),
      m_bAttached(FALSE),m_bCallbackEnabled(FALSE),m_pOldCallback(NULL){
      m_StatusCBObj=this;m_StatusCBProc=&XInternet::OnStatusCallback;
    }

    virtual ~XInternet(){
      if(m_hInternet!=NULL&&m_bAttached)
        Close();
    };

    static XInternet* FromHandlePermanent(HINTERNET hInternet){
      XASSERT(hInternet!=NULL)
      XInternet* pINT=NULL;
      if(hInternet)
        pINT=XWinApp::m_XIntMap.Find(hInternet);
      return pINT;
    }

    static XInternet* FromHandle(HINTERNET hInternet){
      XASSERT(hInternet!=NULL)
      XInternet* pINT=NULL;
      if(hInternet){
        pINT=FromHandlePermanent(hInternet);
        if(!pINT){
          pINT=new XInternet();
          if(pINT)pINT->Attach(hInternet);
        }
      }
      return pINT;
    }

    HINTERNET Attach(HINTERNET hInternet){
      XASSERT(m_hInternet==NULL)
      if(!hInternet)return NULL;
      m_bAttached=XWinApp::m_XIntMap.Add(hInternet,this);
      m_hInternet=hInternet;
      return hInternet;
    }

    HINTERNET Detach(){
      XASSERT(m_hInternet!=NULL)
      HINTERNET hInternet=m_hInternet;
      if(m_bAttached){
        XWinApp::m_XIntMap.Remove(hInternet);
        m_bAttached=FALSE;
      }
      m_hInternet=NULL;
      return hInternet;
    }

    BOOL IsValid(){
      return (m_hInternet!=NULL);
    }

    HINTERNET GetSafeHandle()const{
      return (this != NULL ? m_hInternet: NULL);
    }

    DWORD AttemptConnect(){
      return ::InternetAttemptConnect(0);
    }

    HINTERNET Open(LPCTSTR lpszAgent,DWORD dwAccessType,LPCTSTR lpszProxyName,LPCTSTR lpszProxyBypass,DWORD dwFlags){
      return Attach(::InternetOpen(lpszAgent,dwAccessType,lpszProxyName,lpszProxyBypass,dwFlags));
    }

    BOOL QueryOption(DWORD dwOption,LPVOID lpBuffer,LPDWORD lpdwBufferLength)const{
      XASSERT(m_hInternet!=NULL)
      XASSERT(*lpdwBufferLength!=0)
      return ::InternetQueryOption(m_hInternet,dwOption,lpBuffer,lpdwBufferLength);
    }

    BOOL QueryOption(DWORD dwOption,PDWORD dwValue)const{
      XASSERT(m_hInternet!=NULL)
      DWORD dwLen=sizeof(DWORD);
      return ::InternetQueryOption(m_hInternet,dwOption,dwValue,&dwLen);
    }

    BOOL SetOption(DWORD dwOption,LPVOID lpBuffer,DWORD dwBufferLength,DWORD dwFlags=0){
      XASSERT(m_hInternet!=NULL)
      XASSERT(lpBuffer!=NULL&&dwBufferLength!=0)
      return ::InternetSetOptionEx(m_hInternet,dwOption,lpBuffer,dwBufferLength,dwFlags);
    }

    BOOL SetOption(DWORD dwOption,DWORD dwValue,DWORD dwFlags=0){
      XASSERT(m_hInternet!=NULL)
      return ::InternetSetOptionEx(m_hInternet,dwOption,&dwValue,sizeof(dwValue),dwFlags);
    }

    BOOL QueryDataAvailable(LPDWORD lpdwNumberOfBytesAvailable,DWORD dwFlags=0,DWORD_PTR dwContext=1){
      XASSERT(m_hInternet!=NULL)
      return ::InternetQueryDataAvailable(m_hInternet,lpdwNumberOfBytesAvailable,dwFlags,dwContext);
    }

    BOOL EnableStatusCallback(BOOL bEnable=TRUE){
      XASSERT(m_hInternet!=NULL)
      if(m_hInternet==NULL)
        return FALSE;
      if(bEnable){
        XASSERT(!m_bCallbackEnabled)
        if(!m_bCallbackEnabled){
          m_pOldCallback=::InternetSetStatusCallback(m_hInternet,XInternetStatusCallback);
          if(m_pOldCallback!=INTERNET_INVALID_STATUS_CALLBACK){
            m_bCallbackEnabled=TRUE;
            return TRUE;
          }
        }
      }else{
        XASSERT(m_bCallbackEnabled)
        if(m_bCallbackEnabled){
          if(::InternetSetStatusCallback(m_hInternet,NULL)!=INTERNET_INVALID_STATUS_CALLBACK){
            m_bCallbackEnabled=FALSE;
            m_pOldCallback=NULL;
            //reset internal pointers...
            m_StatusCBObj=this;m_StatusCBProc=&XInternet::OnStatusCallback;
            return TRUE;
          }
        }
      }
      return FALSE;
    }

    INTERNET_STATUS_CALLBACK SetStatusCallback(INTERNET_STATUS_CALLBACK lpfnInternetCallback){
      XASSERT(m_hInternet!=NULL)
      return ::InternetSetStatusCallback(m_hInternet,lpfnInternetCallback);
    }

    VOID SetStatusCallbackEx(XObject *StatusCBObj,LPXINTERNET_STATUS_CALLBACK StatusCBProc){
      XASSERT(StatusCBObj!=NULL&&StatusCBProc!=NULL)
      m_StatusCBObj=(XInternet*)StatusCBObj;
      m_StatusCBProc=StatusCBProc;
    }

    VOID RemoveStatusCallbackEx(){
      m_StatusCBProc=NULL;m_StatusCBObj=NULL;
    }

    BOOL SetCookie(LPCTSTR pstrUrl,LPCTSTR pstrCookieName,LPCTSTR pstrCookieData){
      XASSERT(pstrUrl!=NULL&&pstrCookieName!=NULL)
      return ::InternetSetCookie(pstrUrl,pstrCookieName,pstrCookieData);
    }

    DWORD GetCookieLength(LPCTSTR pstrUrl,LPCTSTR pstrCookieName){
      XASSERT(pstrUrl!=NULL&&pstrCookieName!=NULL)
      DWORD dwRet=0;
      ::InternetGetCookie(pstrUrl,pstrCookieName,NULL,&dwRet);
      return dwRet;
    }

    BOOL GetCookie(LPCTSTR pstrUrl,LPCTSTR pstrCookieName,LPTSTR pstrCookieData,DWORD dwBufLen){
      XASSERT(pstrUrl!=NULL&&pstrCookieName!=NULL&&pstrCookieData!=NULL)
      return ::InternetGetCookie(pstrUrl,pstrCookieName,pstrCookieData,&dwBufLen);
    }

    virtual BOOL Close(){
      if(m_hInternet!=NULL){
        if(m_bCallbackEnabled)
          EnableStatusCallback(FALSE);
        return ::InternetCloseHandle(Detach());
      }
      return FALSE;
    }

    operator HINTERNET(){
      return (this==NULL?NULL:m_hInternet);
    }

    XInternet &operator=(HINTERNET hInternet){
      XASSERT(m_hInternet==NULL)
      m_hInternet=hInternet;
      return *this;
    }

  protected:

    virtual BOOL OnStatusCallback(HINTERNET /*hInternet*/,DWORD_PTR /*dwContext*/,DWORD /*dwInternetStatus*/,
      LPVOID /*lpvStatusInformation*/,DWORD /*dwStatusInformationLength*/){
        return TRUE;
    }

  private:
    HINTERNET m_hInternet;
    BOOL m_bAttached,m_bCallbackEnabled;
    INTERNET_STATUS_CALLBACK m_pOldCallback;
    XInternet *m_StatusCBObj;
    LPXINTERNET_STATUS_CALLBACK m_StatusCBProc;

  private:
    static VOID WINAPI XInternetStatusCallback(HINTERNET hInternet, DWORD_PTR dwContext,
      DWORD dwInternetStatus, LPVOID lpvStatusInformation,DWORD dwStatusInformationLength);
};

class XConnection:public XObject{
  DECLARE_DYNAMIC(XConnection)
  protected:
    ///////////////////////////////////////////////
    // No public constructor! Should be derived! //
    ///////////////////////////////////////////////
    XConnection(HINTERNET hConnection=NULL):m_hConnection(hConnection),
      m_bAttached(FALSE),pSession(NULL){}
  public:
    virtual ~XConnection(){
      if(m_hConnection!=NULL&&m_bAttached)
        Close();
    }

    HINTERNET Attach(HINTERNET hConnection){
      XASSERT(m_hConnection==NULL)
      if(!hConnection)return NULL;
      if(pSession)
        m_bAttached=XWinApp::m_XIntMap.Add(hConnection,pSession);
      else
        m_bAttached=TRUE;
      m_hConnection=hConnection;
      return hConnection;
    }

    HINTERNET Detach(){
      XASSERT(m_hConnection!=NULL)
      HINTERNET hConnection=m_hConnection;
      if(m_bAttached){
        if(pSession)
          XWinApp::m_XIntMap.Remove(hConnection);
        m_bAttached=FALSE;
      }
      m_bAttached=FALSE;
      m_hConnection=NULL;
      return hConnection;
    }

    BOOL IsValid(){
      return (this==NULL?FALSE:m_hConnection!=NULL);
    }

    HINTERNET GetSafeHandle()const{
      return (this==NULL?NULL:m_hConnection);
    }

    HINTERNET Connect(HINTERNET hInternet,LPCTSTR pstrServer,LPCTSTR pstrUserName=NULL,LPCTSTR pstrPassword=NULL,
      DWORD dwService=INTERNET_SERVICE_HTTP,INTERNET_PORT nPort=INTERNET_INVALID_PORT_NUMBER,DWORD_PTR dwContext=1,BOOL bPassive=FALSE){
      XASSERT(hInternet!=NULL)
      XASSERT(pstrServer!=NULL)
      pSession=XWinApp::m_XIntMap.Find(hInternet);
      return Attach(::InternetConnect(hInternet,pstrServer,nPort,pstrUserName,pstrPassword,
        dwService,(bPassive?INTERNET_FLAG_PASSIVE:0),dwContext));
    }

    BOOL GetLastResponseInfo(LPDWORD lpdwError,LPTSTR lpszBuffer,LPDWORD lpdwBufferLength){
      XASSERT(lpdwError!=NULL&&lpszBuffer!=NULL&&lpdwBufferLength!=NULL)
      return ::InternetGetLastResponseInfo(lpdwError,lpszBuffer,lpdwBufferLength);
    }

    BOOL QueryOption(DWORD dwOption,LPVOID lpBuffer,LPDWORD lpdwBufferLength)const{
      XASSERT(m_hConnection!=NULL)
      XASSERT(*lpdwBufferLength!=0)
      return InternetQueryOption(m_hConnection,dwOption,lpBuffer,lpdwBufferLength);
    }

    BOOL QueryOption(DWORD dwOption,PDWORD dwValue)const{
      XASSERT(m_hConnection!=NULL)
      DWORD dwLen=sizeof(DWORD);
      return InternetQueryOption(m_hConnection,dwOption,dwValue,&dwLen);
    }

    BOOL SetOption(DWORD dwOption,LPVOID lpBuffer,DWORD dwBufferLength,DWORD dwFlags=0){
      XASSERT(m_hConnection!=NULL)
      XASSERT(lpBuffer!=NULL&&dwBufferLength!=0)
      return ::InternetSetOptionEx(m_hConnection,dwOption,lpBuffer,dwBufferLength,dwFlags);
    }

    BOOL SetOption(DWORD dwOption,DWORD dwValue,DWORD dwFlags=0){
      XASSERT(m_hConnection!=NULL)
      return ::InternetSetOptionEx(m_hConnection,dwOption,&dwValue,sizeof(dwValue),dwFlags);
    }

    virtual BOOL Close(){
      if(m_hConnection!=NULL)
        return ::InternetCloseHandle(Detach());
      return FALSE;
    }

    operator HINTERNET(){
      return (this==NULL?NULL:m_hConnection);
    }

    XConnection &operator=(HINTERNET hConnection){
      XASSERT(m_hConnection==NULL)
      m_hConnection=hConnection;
      return *this;
    }

  protected:
    HINTERNET m_hConnection;
    BOOL m_bAttached;
    XInternet *pSession;
};

class XHTTPConnection:public XConnection{
  DECLARE_DYNAMIC(XHTTPConnection)
  public:
    XHTTPConnection(HINTERNET hConnection=NULL):XConnection(hConnection){}
    virtual ~XHTTPConnection(){}
};

class XFTPConnection:public XConnection{
  DECLARE_DYNAMIC(XFTPConnection)
  public:
    XFTPConnection(HINTERNET hConnection=NULL):XConnection(hConnection){}
    virtual ~XFTPConnection(){}

    BOOL SetCurrentDirectory(LPCTSTR lpszDirectory){
      XASSERT(m_hConnection!=NULL)
      XASSERT(lpszDirectory!=NULL)
      return ::FtpSetCurrentDirectory(m_hConnection,lpszDirectory);
    }

    BOOL GetCurrentDirectory(LPTSTR lpszDirectory,LPDWORD lpdwLen){
      XASSERT(m_hConnection!=NULL)
      XASSERT(lpszDirectory!=NULL&&lpdwLen!=0)
      return ::FtpGetCurrentDirectory(m_hConnection,lpszDirectory,lpdwLen);
    }

    BOOL CreateDirectory(LPCTSTR lpszDirectory){
      XASSERT(m_hConnection!=NULL)
      XASSERT(lpszDirectory!=NULL)
      return ::FtpCreateDirectory(m_hConnection,lpszDirectory);
    }

    BOOL RemoveDirectory(LPCTSTR lpszDirectory){
      XASSERT(m_hConnection!=NULL)
      XASSERT(lpszDirectory!=NULL)
      return ::FtpRemoveDirectory(m_hConnection,lpszDirectory);
    }

    BOOL RenameFile(LPCTSTR lpszExisting,LPCTSTR lpszNew){
      XASSERT(m_hConnection!=NULL)
      XASSERT(lpszExisting!=NULL&&lpszNew!=NULL)
      return ::FtpRenameFile(m_hConnection,lpszExisting,lpszNew);
    }

    BOOL DeleteFile(LPCTSTR lpszFileName){
      XASSERT(m_hConnection!=NULL)
      XASSERT(lpszFileName!=NULL)
      return ::FtpDeleteFile(m_hConnection,lpszFileName);
    }

  #if _WIN32_IE >= 0x0500
    BOOL Command(DWORD dwFlags,LPCTSTR lpszCommand,DWORD_PTR dwContext=1,BOOL fExpectResponse=FALSE,HINTERNET *phFtpCommand=NULL){
      XASSERT(m_hConnection!=NULL)
      XASSERT(lpszCommand!=NULL)
      return ::FtpCommand(m_hConnection,fExpectResponse,dwFlags,lpszCommand,dwContext,phFtpCommand);
    }
  #endif//_WIN32_IE >= 0x0500

    BOOL PutFile(LPCTSTR lpszLocalFile,LPCTSTR lpszNewRemoteFile,DWORD dwFlags,DWORD_PTR dwContext=1){
      XASSERT(m_hConnection!=NULL)
      XASSERT(lpszLocalFile!=NULL&&lpszNewRemoteFile!=NULL)
      return ::FtpPutFile(m_hConnection,lpszLocalFile,lpszNewRemoteFile,dwFlags,dwContext);
    }

    BOOL GetFile(LPCTSTR lpszRemoteFile,LPCTSTR lpszNewFile,BOOL fFailIfExists,DWORD dwFlagsAndAttributes,DWORD dwFlags,DWORD_PTR dwContext=1){
      XASSERT(m_hConnection!=NULL)
      XASSERT(lpszRemoteFile!=NULL&&lpszNewFile!=NULL)
      return ::FtpGetFile(m_hConnection,lpszRemoteFile,lpszNewFile,fFailIfExists,dwFlagsAndAttributes,dwFlags,dwContext);
    }

    DWORD GetFileSize(LPDWORD lpdwFileSizeHigh){
      XASSERT(m_hConnection!=NULL)
      return ::FtpGetFileSize(m_hConnection,lpdwFileSizeHigh);
    }
};

class XGopherConnection:public XConnection{
  DECLARE_DYNAMIC(XGopherConnection)
  public:
    XGopherConnection(HINTERNET hConnection=NULL):XConnection(hConnection){}
    virtual ~XGopherConnection(){}

    BOOL GetAttribute(LPCTSTR lpszLocator,LPCTSTR lpszAttributeName,LPBYTE lpBuffer,DWORD dwBufferLength,LPDWORD lpdwCharactersReturned,GOPHER_ATTRIBUTE_ENUMERATOR lpfnEnumerator,DWORD_PTR dwContext=1){
      XASSERT(m_hConnection!=NULL)
      return ::GopherGetAttribute(m_hConnection,lpszLocator,lpszAttributeName,lpBuffer,dwBufferLength,lpdwCharactersReturned,lpfnEnumerator,dwContext);
    }

    BOOL CreateLocator(LPCTSTR pstrServerName,LPCTSTR pstrDisplayString,LPCTSTR pstrSelectorString,DWORD dwGopherType,
      LPTSTR lpszLocator,LPDWORD lpdwBufferLength,INTERNET_PORT nPort=INTERNET_INVALID_PORT_NUMBER){
      XASSERT(pstrDisplayString!=NULL&&pstrServerName!=NULL&&pstrSelectorString!=NULL)
      return ::GopherCreateLocator(pstrServerName,nPort,pstrDisplayString,pstrSelectorString,dwGopherType,
        lpszLocator,lpdwBufferLength);
    }
};

class XInternetFile:public XFile{
  DECLARE_DYNAMIC(XInternetFile)
  protected:
    ///////////////////////////////////////////////
    // No public constructor! Should be derived! //
    ///////////////////////////////////////////////
    XInternetFile(HINTERNET hFile=NULL):m_hFile(hFile),pSession(NULL){}
  public:
    virtual ~XInternetFile(){
      if(m_hFile&&m_bAttached)
        Close();
    }

    BOOL IsValid(){
      return (this==NULL?FALSE:m_hFile!=NULL);
    }

    HINTERNET Attach(HINTERNET hFile){
      XASSERT(m_hFile==NULL)
      if(!hFile)return NULL;
      if(pSession)
        m_bAttached=XWinApp::m_XIntMap.Add(hFile,pSession);
      else
        m_bAttached=TRUE;
      m_hFile=hFile;
      return hFile;
    }

    HINTERNET Detach(){
      XASSERT(m_hFile!=NULL)
      HINTERNET hFile=m_hFile;
      if(m_bAttached){
        if(pSession)
          XWinApp::m_XIntMap.Remove(hFile);
        m_bAttached=FALSE;
      }
      m_hFile=NULL;
      return hFile;
    }

    HINTERNET GetSafeHandle()const{
      return (this==NULL?NULL:m_hFile);
    }

    HINTERNET OpenUrl(HINTERNET hInternet,LPCTSTR lpszUrl,LPCTSTR lpszHeaders,DWORD dwHeadersLength,DWORD dwFlags=0,DWORD_PTR dwContext=1){
      XASSERT(hInternet!=NULL&&lpszUrl!=NULL)
      pSession=XWinApp::m_XIntMap.Find(hInternet);
      return Attach(::InternetOpenUrl(hInternet,lpszUrl,lpszHeaders,dwHeadersLength,dwFlags,dwContext));
    }

    BOOL QueryOption(DWORD dwOption,LPVOID lpBuffer,LPDWORD lpdwBufferLength)const{
      XASSERT(m_hFile!=NULL)
      XASSERT(*lpdwBufferLength!=0)
      return ::InternetQueryOption(m_hFile,dwOption,lpBuffer,lpdwBufferLength);
    }

    BOOL QueryOption(DWORD dwOption, DWORD& dwValue)const{
      XASSERT(m_hFile!=NULL)
      DWORD dwLen=sizeof(DWORD);
      return ::InternetQueryOption(m_hFile,dwOption,&dwValue,&dwLen);
    }

    BOOL SetOption(DWORD dwOption,LPVOID lpBuffer,DWORD dwBufferLength,DWORD dwFlags=0){
      XASSERT(m_hFile!=NULL)
      XASSERT(lpBuffer!=NULL&&dwBufferLength!=0);
      return ::InternetSetOptionEx(m_hFile,dwOption,lpBuffer,dwBufferLength,dwFlags);
    }

    BOOL SetOption(DWORD dwOption,DWORD dwValue,DWORD dwFlags=0){
      XASSERT(m_hFile!=NULL)
      return ::InternetSetOptionEx(m_hFile,dwOption,&dwValue,sizeof(dwValue),dwFlags);
    }

    virtual BOOL Read(LPVOID lpBuf,UINT nCount,LPDWORD lpBytesRead=NULL){
      XASSERT(m_hFile!=NULL&&lpBuf!=NULL&&nCount!=0)
      if(m_hFile==NULL)
        return FALSE;
      DWORD dwBytes=0;
      BOOL bRet=::InternetReadFile(m_hFile,(LPVOID)lpBuf,nCount,&dwBytes);
      if(lpBytesRead)
        *lpBytesRead=dwBytes;
      return bRet;
    }

    virtual BOOL ReadEx(LPINTERNET_BUFFERS lpBuffersOut,DWORD dwFlags=WININET_API_FLAG_SYNC,DWORD_PTR dwContext=1){
      XASSERT(m_hFile!=NULL)
      return ::InternetReadFileEx(m_hFile,lpBuffersOut,dwFlags,dwContext);
    }

    virtual BOOL Write(LPCVOID lpBuf,UINT nCount,LPDWORD lpBytesWrite=NULL){
      XASSERT(m_hFile!=NULL&&lpBuf!=NULL&&nCount!=0)
      if(m_hFile==NULL)
        return FALSE;
      DWORD dwBytes=0;
      BOOL bRet=::InternetWriteFile(m_hFile,lpBuf,nCount,&dwBytes);
      if(lpBytesWrite)
        *lpBytesWrite=dwBytes;
      return bRet;
    }

    virtual BOOL WriteEx(LPINTERNET_BUFFERS /*lpBuffersOut*/,DWORD /*dwFlags=WININET_API_FLAG_SYNC*/,DWORD_PTR /*dwContext=1*/){
      XASSERT(m_hFile!=NULL)
      XASSERT(FALSE)//Not Supported
      return FALSE;
    }

    virtual ULONGLONG Seek(LONGLONG lOffset,UINT /*nFrom=FILE_BEGIN*/){
      XASSERT(m_hFile!=NULL)
      LARGE_INTEGER liOff;
      liOff.QuadPart=lOffset;
      //Not declared in Mingw (???)
      //liOff.LowPart=::InternetSetFilePointer(m_hFile,liOff.LowPart,NULL,&liOff.HighPart,nFrom,0);
      if(liOff.LowPart!=INVALID_SET_FILE_POINTER)
        return liOff.QuadPart;
      return 0;
    }

    virtual ULONGLONG GetLength()const{
      XASSERT(m_hFile!=NULL)
      DWORD dwRet=0;
      if(m_hFile!=NULL)
        ::InternetQueryDataAvailable(m_hFile,&dwRet,0,0);
      return dwRet;
    }

    virtual BOOL Close(){
      if(m_hFile!=NULL)
        return ::InternetCloseHandle(Detach());
      return FALSE;
    }

    // Not supported methods...
    XFile* Duplicate()const{
      XASSERT(m_hFile!=NULL)
      XASSERT(FALSE)//Not Supported
      return NULL;
    }

    BOOL Flush(){
      XASSERT(m_hFile!=NULL)
      XASSERT(FALSE)//Not Supported
      return FALSE;
    }

    BOOL Lock(ULONGLONG,ULONGLONG){
      XASSERT(m_hFile!=NULL)
      XASSERT(FALSE)//Not Supported
      return FALSE;
      //return ::InternetLockRequestFile(m_hFile,lphLockReqHandle);
    }

    BOOL Unlock(ULONGLONG,ULONGLONG){
      XASSERT(m_hFile!=NULL)
      XASSERT(FALSE)//Not Supported
      return FALSE;
      //return ::InternetUnlockRequestFile(hLockRequestInfo);
    }

    BOOL SetLength(ULONGLONG){
      XASSERT(m_hFile!=NULL)
      XASSERT(FALSE)//Not Supported
      return FALSE;
    }

    operator HINTERNET(){
      return (this==NULL?NULL:m_hFile);
    }

    XInternetFile &operator=(HINTERNET hFile){
      XASSERT(m_hFile==NULL)
      m_hFile=hFile;
      return *this;
    }

	protected:
    HINTERNET m_hFile;
    XInternet *pSession;
};

class XFTPFile:public XInternetFile{
  DECLARE_DYNAMIC(XFTPFile)
  public:
    XFTPFile(HINTERNET hFile=NULL):XInternetFile(hFile){}
    virtual ~XFTPFile(){}

    HINTERNET OpenFile(HINTERNET hConnection,LPCTSTR lpszFileName,DWORD dwAccess,DWORD dwFlags=0,DWORD_PTR dwContext=1){
      XASSERT(hConnection!=NULL)
      XASSERT(lpszFileName!=NULL)
      pSession=XWinApp::m_XIntMap.Find(hConnection);
      return Attach(::FtpOpenFile(hConnection,lpszFileName,dwAccess,dwFlags,dwContext));
    }

  #if _WIN32_IE >= 0x0500
    BOOL Command(HINTERNET hConnection,DWORD dwFlags,LPCTSTR lpszCommand,DWORD_PTR dwContext=1){
      XASSERT(hConnection!=NULL)
      XASSERT(lpszCommand!=NULL)
      HINTERNET hFtpCommand;
      if(::FtpCommand(hConnection,TRUE,dwFlags,lpszCommand,dwContext,&hFtpCommand)){
        pSession=XWinApp::m_XIntMap.Find(hConnection);
        Attach(hFtpCommand);
        return TRUE;
      }
      return FALSE;
    }
  #endif//_WIN32_IE >= 0x0500
};

class XHTTPFile:public XInternetFile{
  DECLARE_DYNAMIC(XHTTPFile)
  public:
    XHTTPFile(HINTERNET hFile=NULL):XInternetFile(hFile){}
    virtual ~XHTTPFile(){}

    HINTERNET OpenRequest(HINTERNET hConnection,LPCTSTR lpszVerb,LPCTSTR lpszObjectName,LPCTSTR lpszVersion,LPCTSTR lpszReferer,LPCTSTR *lplpszAcceptTypes,DWORD dwFlags,DWORD_PTR dwContext=1){
      XASSERT(hConnection!=NULL)
      pSession=XWinApp::m_XIntMap.Find(hConnection);
      return Attach(::HttpOpenRequest(hConnection,lpszVerb,lpszObjectName,lpszVersion,lpszReferer,lplpszAcceptTypes,dwFlags,dwContext));
    }

    BOOL AddRequestHeaders(LPCTSTR pstrHeaders,DWORD dwModifiers=HTTP_ADDREQ_FLAG_ADD,INT dwHeadersLen=-1){
      XASSERT(m_hFile!=NULL&&pstrHeaders!=NULL&&dwHeadersLen!=0)
      return ::HttpAddRequestHeaders(m_hFile,pstrHeaders,dwHeadersLen,dwModifiers);
    }

    BOOL SendRequest(LPCTSTR pstrHeaders=NULL,DWORD dwHeadersLen=0,LPVOID lpOptional=NULL,DWORD dwOptionalLen=0){
      XASSERT(m_hFile!=NULL)
      XASSERT(dwOptionalLen==0||lpOptional!=NULL)
      XASSERT(dwHeadersLen==0||pstrHeaders!=NULL)
      return ::HttpSendRequest(m_hFile,pstrHeaders,dwHeadersLen,lpOptional,dwOptionalLen);
    }

    BOOL SendRequestEx(DWORD dwTotalLen,DWORD dwFlags=HSR_INITIATE,DWORD_PTR dwContext=1){
      XASSERT(m_hFile!=NULL)
      INTERNET_BUFFERS buffer;
      memset(&buffer,0,sizeof(buffer));
      buffer.dwStructSize=sizeof(buffer);
      buffer.dwBufferTotal=dwTotalLen;
      return SendRequestEx(&buffer,NULL,dwFlags,dwContext);
    }

    BOOL SendRequestEx(LPINTERNET_BUFFERS lpBuffIn,LPINTERNET_BUFFERS lpBuffOut=NULL,DWORD dwFlags=HSR_INITIATE,DWORD_PTR dwContext=1){
      XASSERT(m_hFile!=NULL)
      return ::HttpSendRequestEx(m_hFile,lpBuffIn,lpBuffOut,dwFlags,dwContext);
    }

    BOOL EndRequest(DWORD dwFlags=0,LPINTERNET_BUFFERS lpBuffIn=NULL,DWORD_PTR dwContext=1){
      XASSERT(m_hFile!=NULL)
      return ::HttpEndRequest(m_hFile,lpBuffIn,dwFlags,dwContext);
    }

    BOOL QueryInfo(DWORD dwInfoLevel,DWORD& dwResult,LPDWORD lpdwIndex=NULL)const{
      dwInfoLevel|=HTTP_QUERY_FLAG_NUMBER;
      DWORD dwDWSize=sizeof(DWORD);
      return QueryInfo(dwInfoLevel,&dwResult,&dwDWSize,lpdwIndex);
    }

    BOOL QueryInfo(DWORD dwInfoLevel,SYSTEMTIME* pSystemTime,LPDWORD lpdwIndex=NULL)const{
      dwInfoLevel|=HTTP_QUERY_FLAG_SYSTEMTIME;
      DWORD dwTimeSize=sizeof(SYSTEMTIME);
      return QueryInfo(dwInfoLevel,pSystemTime,&dwTimeSize,lpdwIndex);
    }

    BOOL QueryInfo(DWORD dwInfoLevel,LPVOID lpvBuffer,LPDWORD lpdwBufferLength,LPDWORD lpdwIndex)const{
      XASSERT(m_hFile!=NULL)
      XASSERT(lpvBuffer!=NULL&&lpdwBufferLength&&*lpdwBufferLength>0)
      return ::HttpQueryInfo(m_hFile,dwInfoLevel,lpvBuffer,lpdwBufferLength,lpdwIndex);
    }

    BOOL QueryInfoStatusCode(DWORD& dwStatusCode)const{
      XASSERT(m_hFile!=NULL)
      TCHAR szBuffer[100];DWORD dwLen=sizeof(szBuffer)/sizeof(TCHAR);
      if(::HttpQueryInfo(m_hFile,HTTP_QUERY_STATUS_CODE,szBuffer,&dwLen,NULL)){
        dwStatusCode=(DWORD)_ttol(szBuffer);
        return TRUE;
      }
      return FALSE;
    }

    DWORD ErrorDlg(HWND hParent=NULL,DWORD dwError=ERROR_INTERNET_INCORRECT_PASSWORD,
      DWORD dwFlags=FLAGS_ERROR_UI_FLAGS_GENERATE_DATA|FLAGS_ERROR_UI_FLAGS_CHANGE_OPTIONS,LPVOID* lppvData=NULL){
      LPVOID lpEmpty=NULL;
      if(lppvData==NULL)
        lppvData=&lpEmpty;
      if(hParent==NULL)
        hParent=::GetDesktopWindow();
      return ::InternetErrorDlg(hParent,m_hFile,dwError,dwFlags,lppvData);
    }
};

class XGopherFile:public XInternetFile{
  DECLARE_DYNAMIC(XGopherFile)
  public:
    XGopherFile(HINTERNET hFile=NULL):XInternetFile(hFile){}
    virtual ~XGopherFile(){}

    HINTERNET OpenFile(HINTERNET hConnection,LPCTSTR lpszLocator,LPCTSTR lpszView,DWORD dwFlags=0,DWORD_PTR dwContext=1){
      XASSERT(hConnection!=NULL)
      pSession=XWinApp::m_XIntMap.Find(hConnection);
      return Attach(::GopherOpenFile(hConnection,lpszLocator,lpszView,dwFlags,dwContext));
    }

    virtual BOOL Write(LPCVOID /*lpBuf*/,UINT /*nCount*/,LPDWORD /*lpBytesWrite=NULL*/){
      XASSERT(m_hFile!=NULL)
      XASSERT(FALSE)//Not Supported
      return FALSE;
    }
};

class XFTPFileFind:public XFileFind{
  DECLARE_DYNAMIC(XFTPFileFind)
  public:
    XFTPFileFind(HINTERNET hConnection):m_hConnection(hConnection){}

    virtual ~XFTPFileFind(){
      Close();
    }

    virtual HANDLE FindFirst(LPCTSTR pstrName=NULL,DWORD dwFlags=INTERNET_FLAG_RELOAD,DWORD dwContext=1){
      XASSERT((dwFlags&INTERNET_FLAG_ASYNC)==0)
      XASSERT(m_hConnection!=NULL)
      if(m_hConnection==NULL)
        return FALSE;
      if(IsValid())
        Close();
      if(pstrName==NULL)
        pstrName=_T("*");
      m_hContext=::FtpFindFirstFile(m_hConnection,pstrName,&m_FindData,dwFlags,dwContext);
      if(m_hContext==NULL||m_hContext==INVALID_HANDLE_VALUE)
        return NULL;
      TCHAR szFullPath[MAX_PATH];DWORD dwBuflen=sizeof(szFullPath);
      ::FtpGetCurrentDirectory(m_hConnection,szFullPath,&dwBuflen);
      if(_tcspbrk(pstrName,_T("\\/"))==NULL){
        if(::FtpSetCurrentDirectory(m_hConnection,pstrName)){
          DWORD dwBuflen=sizeof(szFullPath);
          ::FtpGetCurrentDirectory(m_hConnection,m_szFullPath,&dwBuflen);
          ::FtpSetCurrentDirectory(m_hConnection,szFullPath);
        }else
          lstrcpy(m_szFullPath,szFullPath);
      }else{
        LPCTSTR pstrOther=_tcsrchr(pstrName,_T('\\'));
        LPCTSTR pstrRoot=_tcsrchr(pstrName,_T('/'));
        if(pstrRoot==NULL)
          pstrRoot=pstrName;
        if (pstrOther==NULL)
          pstrOther=pstrName;
        INT nLast=0;
        if(pstrRoot>=pstrOther)
          nLast=ULONG(pstrRoot-pstrName);
        else
          nLast=ULONG(pstrOther-pstrName);
        if(nLast==0)
          nLast++;
        lstrcpy(m_szFullPath,pstrName);
        m_szFullPath[nLast]=0;
      }
      return m_hContext;
    }

    virtual BOOL FindNext(){
      XASSERT(m_hContext!=NULL)
      if(m_hContext==NULL)
        return FALSE;
      return ::InternetFindNextFile(m_hContext,&m_FindData);
    }

    LPCTSTR GetFileURL(LPTSTR szServerName,LPTSTR szBuffer,INT iLen)const{
      XASSERT(m_hContext!=NULL)
      XASSERT(szBuffer!=NULL&&iLen!=0)
      INT srvlen=lstrlen(szServerName);
      if((srvlen+lstrlen(m_szFullPath)+lstrlen(GetFileName())+8)<=iLen){
        lstrcpy(szBuffer,_T("ftp://"));
        lstrcat(szBuffer,szServerName);
        GetFilePath(szBuffer+(srvlen+6),iLen-(srvlen+6));
        return szBuffer;
      }
      return NULL;
    }

    virtual BOOL Close(){
      if(m_hContext!=NULL&&m_hContext!=INVALID_HANDLE_VALUE){
        BOOL bRet=::InternetCloseHandle(m_hContext);
        m_hContext=NULL;
        return bRet;
      }
      return FALSE;
    }

  protected:
    HINTERNET m_hConnection;
};


class XGopherFileFind:public XFileFind{
  DECLARE_DYNAMIC(XGopherFileFind)
  public:
    XGopherFileFind(HINTERNET hConnection):m_hConnection(hConnection){
      ZeroMemory(&m_GopherFindData,sizeof(m_GopherFindData));
    }

    virtual ~XGopherFileFind(){
      Close();
    }

    virtual HANDLE FindFirst(LPCTSTR pstrString,LPCTSTR lpszLocator=NULL,DWORD dwFlags=INTERNET_FLAG_RELOAD,DWORD dwContext=1){
      XASSERT((dwFlags&INTERNET_FLAG_ASYNC)==0)
      XASSERT(m_hConnection!=NULL)
      if(m_hConnection==NULL)
        return FALSE;
      if(IsValid())
        Close();
      m_hContext=::GopherFindFirstFile(m_hConnection,lpszLocator,pstrString,&m_GopherFindData,dwFlags,dwContext);
      return m_hContext;
    }

    virtual BOOL FindNext(){
      XASSERT(m_hContext!=NULL)
      if(m_hContext==NULL)
        return FALSE;
      return ::InternetFindNextFile(m_hContext,&m_GopherFindData);
    }

    virtual BOOL IsDots() const{
      return FALSE;
    }

    virtual BOOL GetLastWriteTime(FILETIME* pTimeStamp)const{
      XASSERT(m_hContext!=NULL&&pTimeStamp!=NULL)
      if(pTimeStamp!=NULL){
        *pTimeStamp=m_GopherFindData.LastModificationTime;
        return TRUE;
      }
      return FALSE;
    }

    virtual BOOL GetLastAccessTime(FILETIME* pTimeStamp)const{
      XASSERT(m_hContext!=NULL&&pTimeStamp!=NULL)
      return GetLastWriteTime(pTimeStamp);
    }

    virtual BOOL GetCreationTime(FILETIME* pTimeStamp)const{
      XASSERT(m_hContext!=NULL&&pTimeStamp!=NULL)
      return GetLastWriteTime(pTimeStamp);
    }

    virtual ULONGLONG GetLength() const{
      XASSERT(m_hContext!=NULL)
      ULARGE_INTEGER nLength;
      nLength.LowPart=m_GopherFindData.SizeLow;
      nLength.HighPart=m_GopherFindData.SizeHigh;
      return nLength.QuadPart;
    }

    virtual BOOL Close(){
      if(m_hContext!=NULL&&m_hContext!=INVALID_HANDLE_VALUE){
        BOOL bRet=::InternetCloseHandle(m_hContext);
        m_hContext=NULL;
        return bRet;
      }
      return FALSE;
    }

    LPCTSTR GetScreenName()const{
      XASSERT(m_hContext!=NULL)
      return m_GopherFindData.DisplayString;
    }

    // Unsupported...
    virtual LPCTSTR GetFileName()const{
      XASSERT(m_hConnection!=NULL)
      XASSERT(FALSE)//Not Supported
      return NULL;
    }

    virtual LPCTSTR GetFilePath()const{
      XASSERT(m_hConnection!=NULL)
      XASSERT(FALSE)//Not Supported
      return NULL;
    }

    virtual LPCTSTR GetFileTitle()const{
      XASSERT(m_hConnection!=NULL)
      XASSERT(FALSE)//Not Supported
      return NULL;
    }

    virtual LPCTSTR GetFileURL()const{
      XASSERT(m_hConnection!=NULL)
      XASSERT(FALSE)//Not Supported
      return NULL;
    }

    virtual LPCTSTR GetRoot()const{
      XASSERT(m_hConnection!=NULL)
      XASSERT(FALSE)//Not Supported
      return NULL;
    }

  protected:
    HINTERNET m_hConnection;
    GOPHER_FIND_DATA m_GopherFindData;
};

#endif // _XINTERNET_H_
