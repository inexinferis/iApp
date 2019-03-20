#include "xinternet.h"

IMPLEMENT_DYNAMIC(XInternet,XObject)

VOID WINAPI XInternet::XInternetStatusCallback(HINTERNET hInternet,DWORD_PTR dwContext,
  DWORD dwInternetStatus,LPVOID lpvStatusInformation,DWORD dwStatusInformationLength){
  XInternet* pSession=FromHandlePermanent(hInternet);
  //do callback
  if(pSession&&pSession->m_bCallbackEnabled){
    BOOL bCallChain=TRUE;
    //call client...
    if(pSession->m_StatusCBObj&&pSession->m_StatusCBProc)
      bCallChain=((pSession->m_StatusCBObj)->*(pSession->m_StatusCBProc))(hInternet,dwContext,dwInternetStatus,lpvStatusInformation,dwStatusInformationLength);
    //call original...
    if(bCallChain&&pSession->m_pOldCallback!=NULL)
      (*pSession->m_pOldCallback)(hInternet,dwContext,dwInternetStatus,lpvStatusInformation,dwStatusInformationLength);
  }
}

IMPLEMENT_DYNAMIC(XConnection,XObject)

IMPLEMENT_DYNAMIC(XFTPConnection,XConnection)
IMPLEMENT_DYNAMIC(XHTTPConnection,XConnection)
IMPLEMENT_DYNAMIC(XGopherConnection,XConnection)

IMPLEMENT_DYNAMIC(XInternetFile,XFile)

IMPLEMENT_DYNAMIC(XFTPFile,XInternetFile)
IMPLEMENT_DYNAMIC(XHTTPFile,XInternetFile)
IMPLEMENT_DYNAMIC(XGopherFile,XInternetFile)

IMPLEMENT_DYNAMIC(XFTPFileFind,XFileFind)
IMPLEMENT_DYNAMIC(XGopherFileFind,XFileFind)
