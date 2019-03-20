#ifndef _XREGKEY_H_
#define _XREGKEY_H_

#include "xobject.h"

#ifndef KEY_WOW64_64KEY
  #define KEY_WOW64_64KEY 0x100
#endif

#ifndef KEY_WOW64_32KEY
  #define KEY_WOW64_32KEY 0x200
#endif

#ifndef KEY_WOW64_RES
  #define KEY_WOW64_RES   0x300
#endif

class XRegKey:public XObject{
  DECLARE_DYNAMIC(XRegKey)
  public:
    XRegKey(HKEY hKey=NULL):m_hKey(hKey){}
    ~XRegKey(){
      if(m_hKey)
        Close();
    }

    static XRegKey *FromHandle(HKEY hKey){
      return new XRegKey(hKey);
    }

    HKEY Attach(HKEY hKey){
      XASSERT(m_hKey==NULL)
      m_hKey=hKey;
      return hKey;
    }

    HKEY Detach(){
      HKEY hKey=m_hKey;
      m_hKey=NULL;
      return hKey;
    }

    HKEY GetSafeHandle()const{
      return ( this != NULL ? m_hKey : NULL );
    }

    BOOL IsValid(){
      return (m_hKey!=NULL);
    }

    LONG Open(HKEY hKey,LPCTSTR lpSubKey,REGSAM samDesired=KEY_READ|KEY_WRITE){
      XASSERT(m_hKey==NULL&&hKey!=NULL)
      return ::RegOpenKeyEx(hKey,lpSubKey,0,samDesired,&m_hKey);
    }

    LONG Create(HKEY hKey,LPCTSTR lpSubKey,LPTSTR lpClass=REG_NONE,DWORD dwOptions=REG_OPTION_NON_VOLATILE,REGSAM samDesired=KEY_READ|KEY_WRITE,LPSECURITY_ATTRIBUTES lpSecAttr=NULL,LPDWORD lpdwDisposition=NULL){
      XASSERT(m_hKey==NULL&&hKey!=NULL)
      return ::RegCreateKeyEx(hKey,lpSubKey,0,lpClass,dwOptions,samDesired,lpSecAttr,&m_hKey,lpdwDisposition);
    }

    LONG Connect(LPCTSTR lpMachineName,HKEY hKey){
      XASSERT(m_hKey==NULL&&hKey!=NULL)
      return ::RegConnectRegistry(lpMachineName,hKey,&m_hKey);
    }

    LONG DeleteKey(LPCTSTR lpSubKey,REGSAM samDesired=KEY_WOW64_32KEY|KEY_WOW64_64KEY){
      XASSERT(m_hKey!=NULL)
      #if WINVER >= 0x0502
        return ::RegDeleteKeyEx(m_hKey,lpSubKey,samDesired,0);
      #else
        XUNUSED_ALWAYS(samDesired)
      #endif	// WINVER
      return ::RegDeleteKey(m_hKey,lpSubKey);
    }

    LONG RecurseDeleteKey(LPCTSTR lpszKey){
      XASSERT(m_hKey!=NULL)
      XRegKey key;TCHAR szBuffer[MAX_PATH];DWORD dwSize=MAX_PATH;
      LONG lRes=key.Open(m_hKey,lpszKey,KEY_READ|KEY_WRITE);
      if(lRes!=ERROR_SUCCESS)
        return lRes;
      while(RegEnumKeyEx(key.m_hKey,0,szBuffer,&dwSize,NULL,NULL,NULL,NULL)==ERROR_SUCCESS){
        lRes=key.RecurseDeleteKey(szBuffer);
        if(lRes!=ERROR_SUCCESS)
          return lRes;
        dwSize=MAX_PATH;
      }
      key.Close();
      return DeleteKey(lpszKey);
    }

    LONG Close(){
      LONG lRes=ERROR_SUCCESS;
      if(m_hKey!=NULL){
        lRes=::RegCloseKey(m_hKey);
        m_hKey=NULL;
      }
      return lRes;
    }

    LONG Flush(){
      XASSERT(m_hKey!=NULL)
      return ::RegFlushKey(m_hKey);
    }

    LONG SetValue(LPCTSTR lpValueName,DWORD dwType,LPCVOID pValue,DWORD cbData){
      XASSERT(m_hKey!=NULL)
      return ::RegSetValueEx(m_hKey,lpValueName,0,dwType,(PBYTE)pValue,cbData);
    }

    LONG SetBinaryValue(LPCTSTR pszValueName,LPCVOID pValue,ULONG nBytes){
      XASSERT(m_hKey!=NULL)
      return ::RegSetValueEx(m_hKey,pszValueName,0,REG_BINARY,(PBYTE)pValue,nBytes);
    }

    LONG SetDWORDValue(LPCTSTR pszValueName,DWORD dwValue){
      XASSERT(m_hKey!=NULL)
      return ::RegSetValueEx(m_hKey,pszValueName,0,REG_DWORD,(PBYTE)&dwValue,sizeof(DWORD));
    }

    LONG SetQWORDValue(LPCTSTR pszValueName,ULONGLONG qwValue){
      XASSERT(m_hKey!=NULL)
      return ::RegSetValueEx(m_hKey,pszValueName,0,REG_QWORD,(PBYTE)&qwValue,sizeof(ULONGLONG));
    }

    LONG SetStringValue(LPCTSTR pszValueName,LPCTSTR pszValue,DWORD dwType=REG_SZ){
      XASSERT(m_hKey!=NULL&&pszValue!=NULL&&((dwType==REG_SZ)||(dwType==REG_EXPAND_SZ)))
      return ::RegSetValueEx(m_hKey,pszValueName,0,dwType,(PBYTE)pszValue,(lstrlen(pszValue)+1)*sizeof(TCHAR));
    }

    LONG SetMultiStringValue(LPCTSTR pszValueName,LPCTSTR pszValue){
      XASSERT(m_hKey!=NULL&&pszValue!=NULL)
      LPCTSTR pszTemp=pszValue;
      ULONG nBytes=0;ULONG nLength;
      do{
        nLength=lstrlen(pszTemp)+1;
        pszTemp+=nLength;
        nBytes+=nLength*sizeof(TCHAR);
      }while(nLength!=1);
      return ::RegSetValueEx(m_hKey,pszValueName,0,REG_MULTI_SZ,(PBYTE)pszValue,nBytes);
    }

    LONG QueryValue(LPCTSTR lpValueName,LPDWORD lpType,LPBYTE lpData,LPDWORD lpcbData){
      XASSERT(m_hKey!=NULL)
      return ::RegQueryValueEx(m_hKey,lpValueName,NULL,lpType,lpData,lpcbData);
    }

    LONG QueryBinaryValue(LPCTSTR pszValueName,PVOID pValue,PULONG pnBytes){
      XASSERT(m_hKey!=NULL&&pnBytes!=NULL)
      DWORD dwType;
      LONG lRes=::RegQueryValueEx(m_hKey,pszValueName,NULL,&dwType,(LPBYTE)pValue,pnBytes);
      if(lRes!=ERROR_SUCCESS)
        return lRes;
      if(dwType!=REG_BINARY)
        return ERROR_INVALID_DATA;
      return ERROR_SUCCESS;
    }

    LONG QueryDWORDValue(LPCTSTR pszValueName,PDWORD dwValue){
      XASSERT(m_hKey!=NULL)
      DWORD dwType;ULONG nBytes=sizeof(DWORD);
      LONG lRes=::RegQueryValueEx(m_hKey,pszValueName,NULL,&dwType,(LPBYTE)dwValue,&nBytes);
      if(lRes!=ERROR_SUCCESS)
        return lRes;
      if(dwType!=REG_DWORD)
        return ERROR_INVALID_DATA;
      return ERROR_SUCCESS;
    }

    LONG QueryQWORDValue(LPCTSTR pszValueName,PULONGLONG qwValue){
      XASSERT(m_hKey!=NULL)
      DWORD dwType;ULONG nBytes=sizeof(ULONGLONG);
      LONG lRes=::RegQueryValueEx(m_hKey,pszValueName,NULL,&dwType,(LPBYTE)qwValue,&nBytes);
      if(lRes!=ERROR_SUCCESS)
        return lRes;
      if(dwType!=REG_QWORD)
        return ERROR_INVALID_DATA;
      return ERROR_SUCCESS;
    }

    LONG QueryStringValue(LPCTSTR pszValueName,LPTSTR pszValue,ULONG nChars,PULONG pnChars=NULL){
      XASSERT(m_hKey!=NULL&&nChars!=0)
      DWORD dwType;ULONG nBytes=(nChars)*sizeof(TCHAR);
      LONG lRes=::RegQueryValueEx(m_hKey,pszValueName,NULL,&dwType,(LPBYTE)pszValue,&nBytes);
      if(lRes!=ERROR_SUCCESS)
        return lRes;
      if(dwType!=REG_SZ&&dwType!=REG_EXPAND_SZ)
        return ERROR_INVALID_DATA;
      if(pszValue!=NULL){
        if(nBytes!=0){
          if((nBytes%sizeof(TCHAR)!=0)||(pszValue[nBytes/sizeof(TCHAR)-1]!=0))
            return ERROR_INVALID_DATA;
        }else
          pszValue[0]=_T('\0');
      }
      if(pnChars)
        *pnChars=nBytes/sizeof(TCHAR);
      return ERROR_SUCCESS;
    }

    LONG QueryMultiStringValue(LPCTSTR pszValueName,LPTSTR pszValue,ULONG nChars,PULONG pnChars=NULL){
      XASSERT(m_hKey!=NULL&&nChars!=0)
      DWORD dwType;ULONG nBytes=(nChars)*sizeof(TCHAR);
      if(pszValue!=NULL&&nChars<2)
        return ERROR_INSUFFICIENT_BUFFER;
      LONG lRes=::RegQueryValueEx(m_hKey,pszValueName,NULL,&dwType,(LPBYTE)pszValue,&nBytes);
      if(lRes!=ERROR_SUCCESS)
        return lRes;
      if(dwType!=REG_MULTI_SZ)
        return ERROR_INVALID_DATA;
      if(pszValue!=NULL&&(nBytes%sizeof(TCHAR)!=0||nBytes/sizeof(TCHAR)<1||
         pszValue[nBytes/sizeof(TCHAR)-1]!=0||((nBytes/sizeof(TCHAR))>1&&
         pszValue[nBytes/sizeof(TCHAR)-2]!=0)))
        return ERROR_INVALID_DATA;
      if(pnChars)
        *pnChars=nBytes/sizeof(TCHAR);
      return ERROR_SUCCESS;
    }

    LONG DeleteValue(LPCTSTR lpValueName){
      XASSERT(m_hKey!=NULL)
      return ::RegDeleteValue(m_hKey,lpValueName);
    }

    LONG GetKeySecurity(SECURITY_INFORMATION SecurityInformation,PSECURITY_DESCRIPTOR pSecurityDescriptor,LPDWORD lpcbSecurityDescriptor){
      XASSERT(m_hKey!=NULL&&lpcbSecurityDescriptor!=NULL)
      return ::RegGetKeySecurity(m_hKey,SecurityInformation,pSecurityDescriptor,lpcbSecurityDescriptor);
    }

    LONG SetKeySecurity(SECURITY_INFORMATION SecurityInformation,PSECURITY_DESCRIPTOR pSecurityDescriptor){
      XASSERT(m_hKey!=NULL&&pSecurityDescriptor!=NULL)
      return ::RegSetKeySecurity(m_hKey,SecurityInformation,pSecurityDescriptor);
    }

    LONG SetKeyValue(LPCTSTR lpSubKey,LPCTSTR lpValueName,DWORD dwType,LPCVOID lpData,DWORD cbData){
      XASSERT(m_hKey!=NULL)
//Not Defined in Mingw WinAPI 3.17
/*#if (_WIN32_WINDOWS >= 0x0600)
      return ::RegSetKeyValue(m_hKey,lpSubKey,lpValueName,dwType,lpData,cbData);
#else*/
      XRegKey key;
      LONG lRes=key.Open(m_hKey,lpSubKey,KEY_READ|KEY_WRITE);
      if(lRes==ERROR_SUCCESS){
        lRes=key.SetValue(lpValueName,dwType,lpData,cbData);
        key.Close();
      }
      return lRes;
//#endif
    }

    LONG DeleteKeyValue(LPCTSTR lpSubKey,LPCTSTR lpValueName){
      XASSERT(m_hKey!=NULL)
//Not Defined in Mingw WinAPI 3.17
/*#if (_WIN32_WINDOWS >= 0x0600)
      return ::RegDeleteKeyValue(m_hKey,lpSubKey,lpValueName);
#else*/
      XRegKey key;
      LONG lRes=key.Open(m_hKey,lpSubKey,KEY_WRITE);
      if(lRes==ERROR_SUCCESS){
        lRes=key.DeleteValue(lpValueName);
        key.Close();
      }
      return lRes;
//#endif
    }

    LONG EnumKey(DWORD dwIndex,LPTSTR lpName,LPDWORD lpcName,PFILETIME lpftLastWriteTime=NULL){
      XASSERT(m_hKey!=NULL)
      return ::RegEnumKeyEx(m_hKey,dwIndex,lpName,lpcName,NULL,NULL,NULL,lpftLastWriteTime);
    }

    LONG EnumValue(DWORD dwIndex,LPTSTR lpValueName,LPDWORD lpcchValueName,LPDWORD lpType,LPBYTE lpData,LPDWORD lpcbData){
      XASSERT(m_hKey!=NULL)
      return ::RegEnumValue(m_hKey,dwIndex,lpValueName,lpcchValueName,NULL,lpType,lpData,lpcbData);
    }

    LONG QueryInfoKey(LPTSTR lpClass,LPDWORD lpcClass,LPDWORD lpcSubKeys,LPDWORD lpcMaxSubKeyLen,LPDWORD lpcMaxClassLen,
      LPDWORD lpcValues,LPDWORD lpcMaxValueNameLen,LPDWORD lpcMaxValueLen,LPDWORD lpcbSecurityDescriptor,PFILETIME lpftLastWriteTime){
      XASSERT(m_hKey!=NULL)
      return ::RegQueryInfoKey(m_hKey,lpClass,lpcClass,NULL,lpcSubKeys,lpcMaxSubKeyLen,lpcMaxClassLen,
        lpcValues,lpcMaxValueNameLen,lpcMaxValueLen,lpcbSecurityDescriptor,lpftLastWriteTime);
    }

    LONG QueryMultipleValues(PVALENT val_list,DWORD num_vals,LPTSTR lpValueBuf,LPDWORD ldwTotsize){
      XASSERT(m_hKey!=NULL)
      return ::RegQueryMultipleValues(m_hKey,val_list,num_vals,lpValueBuf,ldwTotsize);
    }

    LONG NotifyChangeKeyValue(BOOL bWatchSubtree,DWORD dwNotifyFilter,HANDLE hEvent,BOOL bAsync=TRUE){
      XASSERT(m_hKey!=NULL&&((hEvent!=NULL)||!bAsync))
      return ::RegNotifyChangeKeyValue(m_hKey,bWatchSubtree,dwNotifyFilter,hEvent,bAsync);
    }

#if (_WIN32_WINDOWS >= 0x0500)
    //Not Defined in Mingw WinAPI 3.17
    /*LONG OpenCurrentUser(REGSAM samDesired=KEY_ALL_ACCESS){
      XASSERT(m_hKey==NULL)
      return ::RegOpenCurrentUser(samDesired,&m_hKey);
    }

    LONG OpenUserClassesRoot(HANDLE hToken,REGSAM samDesired=KEY_ALL_ACCESS){
      XASSERT(m_hKey==NULL)
      return ::RegOpenUserClassesRoot(hToken,0,samDesired,&m_hKey);
    }*/
#endif

#if (_WIN32_WINDOWS >= 0x0600)
    //Not Defined in Mingw WinAPI 3.17
    /*LONG CopyTree(LPCTSTR lpSubKey,HKEY hKeyDest){
      XASSERT(m_hKey!=NULL)
      return ::RegCopyTree(m_hKey,lpSubKey,hKeyDest);
    }

    LONG CopyTree(HKEY hKeySrc,LPCTSTR lpSubKey){
      XASSERT(m_hKey!=NULL)
      return ::RegCopyTree(hKeySrc,lpSubKey,m_hKey);
    }

    LONG DeleteTree(LPCTSTR lpSubKey){
      XASSERT(m_hKey!=NULL)
      return ::RegDeleteTree(m_hKey,lpSubKey);
    }

    LONG CreateKeyTransacted(HKEY hKey,LPCTSTR lpSubKey,LPTSTR lpClass,DWORD dwOptions,REGSAM samDesired,
    const LPSECURITY_ATTRIBUTES lpSecurityAttributes,LPDWORD lpdwDisposition,HANDLE hTransaction,PVOID pExtendedParemeter){
      XASSERT(m_hKey==NULL)
      return ::RegCreateKeyTransacted(hKey,lpSubKey,0,lpClass,dwOptions,samDesired,lpSecurityAttributes,&m_hKey,lpdwDisposition,hTransaction,pExtendedParemeter);
    }

    LONG OpenKeyTransacted(HKEY hKey,LPCTSTR lpSubKey,DWORD ulOptions,REGSAM samDesired,HANDLE hTransaction,PVOID pExtendedParameter){
      XASSERT(m_hKey==NULL)
      return ::RegOpenKeyTransacted(hKey,lpSubKey,ulOptions,samDesired,&m_hKey,hTransaction,pExtendedParameter);
    }

    LONG DeleteKeyTransacted(LPCTSTR lpSubKey,REGSAM samDesired,HANDLE hTransaction){
      XASSERT(m_hKey!=NULL)
      return ::RegDeleteKeyTransacted(m_hKey,lpSubKey,samDesired,0,hTransaction,NULL);
    }

    LONG DisableReflectionKey(){
      XASSERT(m_hKey!=NULL)
      return ::RegDisableReflectionKey(m_hKey);
    }

    LONG EnableReflectionKey(){
      XASSERT(m_hKey!=NULL)
      return ::RegEnableReflectionKey(m_hKey);
    }

    LONG QueryReflectionKey(BOOL *bIsReflectionDisabled){
      XASSERT(m_hKey!=NULL)
      return ::RegQueryReflectionKey(m_hKey,bIsReflectionDisabled);
    }

    LONG GetValue(LPCTSTR lpSubKey,LPCTSTR lpValue,DWORD dwFlags,LPDWORD pdwType,PVOID pvData,LPDWORD pcbData){
      XASSERT(m_hKey!=NULL)
      return ::RegGetValue(m_hKey,lpSubKey,lpValue,dwFlags,pdwType,pvData,pcbData);
    }

    LONG LoadMUIString(LPCTSTR pszValue,LPTSTR pszOutBuf,DWORD cbOutBuf,LPDWORD pcbData,DWORD Flags,LPCTSTR pszDirectory){
      XASSERT(m_hKey!=NULL)
      return ::RegLoadMUIString(m_hKey,pszValue,pszOutBuf,cbOutBuf,pcbData,Flags,pszDirectory);
    }*/

#endif

    XRegKey &operator=(const HKEY hKey){
      m_hKey=hKey;
      return *this;
    }

    operator HKEY()const{
      return m_hKey;
    }

  private:
    HKEY m_hKey;
};

#endif //_XREGKEY_H_
