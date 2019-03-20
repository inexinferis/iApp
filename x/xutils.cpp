#include "xutils.h"

INT GetStringLength(HINSTANCE hInstance,UINT uID){
  LPWSTR pBuf=NULL;
  return LoadStringW(hInstance,uID,(LPWSTR)&pBuf,0);
}

UINT LoadStringExW(HINSTANCE hInstance,UINT uId,LPWSTR buffer,UINT buflen,WORD wLanguage){
  HRSRC hResource=FindResourceExW(hInstance,(LPWSTR)RT_STRING,MAKEINTRESOURCEW((LOWORD(uId)>>4)+1),wLanguage);
  if(!hResource)return 0;
  HGLOBAL hmem=LoadResource(hInstance,hResource);
  if(!hmem)return 0;
  WCHAR *p=(WCHAR*)LockResource(hmem);
  if(!p)return 0;
  WCHAR *end=(WCHAR *)((PBYTE)p+SizeofResource(hInstance,hResource));
  INT index=uId&0x000F;//[WORD size][WORD string[XX]]
  for(;(index>0)&&(p<end);p+=*p+1,index--);
  if(!buflen){
    if(buffer)
      *((LPWSTR*)buffer)=p+1;
    return *p;
  }
  UINT i=__min(buflen-1,*p);
  if(i>0){
    memcpy(buffer,p+1,i*sizeof(WCHAR));
    buffer[i]=0;
  }else if(buflen>1){
    buffer[0]=0;
    return 0;
  }
  return i;
}

UINT LoadStringExA(HINSTANCE hInstance,UINT uId,LPSTR buffer,UINT buflen,WORD wLanguage){
  LPWSTR pBuf=NULL;
  DWORD retval=LoadStringExW(hInstance,uId,(LPWSTR)&pBuf,0,wLanguage);
  if(retval&&buffer){
    retval=WideCharToMultiByte(CP_ACP,0,pBuf,retval,buffer,buflen-1,NULL,NULL);
    buffer[retval]=0;
  }
  return retval;
}

LPTSTR LoadStringBuffered(HINSTANCE hInstance,LPCTSTR lpszName){
  LPTSTR szBuffer=NULL;
  if(lpszName&&IS_INTRESOURCE(lpszName)){
    UINT uLen=::GetStringLength(hInstance,(UINT_PTR)lpszName);
    if(uLen&&(szBuffer=(LPTSTR)LocalAlloc(LMEM_ZEROINIT,++uLen*sizeof(TCHAR)))){
      if(!::LoadString(hInstance,(UINT_PTR)lpszName,szBuffer,uLen)){
        LocalFree(szBuffer);
        szBuffer=NULL;
      }
    }
  }
  return szBuffer;
}

LPTSTR LoadStringStatic(HINSTANCE hInstance,LPCTSTR lpszName){
  static TCHAR szBuffer[2048];
  if(lpszName&&IS_INTRESOURCE(lpszName))
    ::LoadString(hInstance,(UINT_PTR)lpszName,szBuffer,sizeof(szBuffer));
  return szBuffer;
}

LPTSTR FormatMsg(LPCTSTR lpszFormat,...){
	LPTSTR lpszTemp=NULL;
	va_list argList;
	va_start(argList,lpszFormat);
	FormatMessage(FORMAT_MESSAGE_FROM_STRING|FORMAT_MESSAGE_ALLOCATE_BUFFER,
		lpszFormat,0,0,(LPTSTR)&lpszTemp,0,&argList);
	va_end(argList);
	return lpszTemp;
}

LPTSTR FormatMsgEx(HINSTANCE hInstance,LPCTSTR nRsrcID,...){
  LPTSTR lpszTemp=NULL;
  va_list argList;
  va_start(argList,nRsrcID);
  LPTSTR lpszFormat=LoadStringBuffered(hInstance,nRsrcID);
  if(lpszFormat){
    FormatMessage(FORMAT_MESSAGE_FROM_STRING|FORMAT_MESSAGE_ALLOCATE_BUFFER,
      lpszFormat,0,0,(LPTSTR)&lpszTemp,0,&argList);
    LocalFree(lpszFormat);
  }
  va_end(argList);
  return lpszTemp;
}

ATOM XWndClass::RegisterClass(LPCTSTR lpszClassName,UINT style,HICON hIcon,HICON hIconSm,HCURSOR hCursor,
  HBRUSH hbrBackground,LPCTSTR lpszMenuName,INT cbClsExtra,INT cbWndExtra,WNDPROC lpfnWndProc,HINSTANCE hInstance){
  WNDCLASSEX m_wincl;
  //we need a class name
  if(!lpszClassName)
    return FALSE;
  //check if is already registered!
  if((m_atom=::GetClassInfoEx(hInstance,lpszClassName,&m_wincl)))
    return m_atom;
	m_wincl.cbSize=sizeof(WNDCLASSEX);
  m_wincl.style=style;
  m_wincl.lpfnWndProc=lpfnWndProc;
  m_wincl.cbClsExtra=cbClsExtra;
  m_wincl.cbWndExtra=cbWndExtra;
  m_wincl.hInstance=hInstance;
  m_wincl.hIcon=hIcon;
  m_wincl.hIconSm=hIconSm;
  m_wincl.hCursor=hCursor;
  m_wincl.hbrBackground=hbrBackground;
  m_wincl.lpszMenuName=lpszMenuName;
  m_wincl.lpszClassName=lpszClassName;
	return (m_atom=RegisterClassEx(&m_wincl));
}
