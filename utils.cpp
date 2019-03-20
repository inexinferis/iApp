#include "x/x.h"
#include "utils.h"

ULONG crc32_table[256];

LPSTR CreateBuffer(){
  static DWORD dwTlsIndex=TLS_OUT_OF_INDEXES;
  static CHAR  sBuffer[2048];
  if(dwTlsIndex==TLS_OUT_OF_INDEXES)
    dwTlsIndex=TlsAlloc();
  if(dwTlsIndex!=TLS_OUT_OF_INDEXES){
    LPSTR lpvData=(LPSTR)TlsGetValue(dwTlsIndex);
    if(!lpvData){
      lpvData=(LPSTR)LocalAlloc(LPTR,2048);
      if(!lpvData||!TlsSetValue(dwTlsIndex,lpvData)){
        if(lpvData)
          LocalFree((HLOCAL)lpvData);
        lpvData=sBuffer;
      }
    }
    return lpvData;
  }
  return sBuffer;
}

BOOL FileExist(LPCTSTR szPath){
	DWORD dwAttrib=GetFileAttributes(szPath);
	return (dwAttrib!=INVALID_FILE_ATTRIBUTES&&!(dwAttrib&FILE_ATTRIBUTE_DIRECTORY));
}

BOOL DirectoryExist(LPCTSTR szPath){
	DWORD dwAttrib=GetFileAttributes(szPath);
	return (dwAttrib!=INVALID_FILE_ATTRIBUTES&&(dwAttrib&FILE_ATTRIBUTE_DIRECTORY));
}

BOOL CheckDirectory(LPCTSTR szPath){
  LPTSTR p=(LPTSTR)szPath;
  while(*p){
    if(*p=='\\'||*p=='/'){
      *p=0;
      if(!DirectoryExist(szPath)&&!CreateSubDirectory(szPath))
        return FALSE;
      *p='\\';
    }
    p++;
  }
  return TRUE;
}

BOOL CreateSubDirectory(LPCTSTR szPath){
  PSID pUsersSid=NULL;PACL acl=NULL,pOldDACL=NULL;EXPLICIT_ACCESS ea;
  SID_IDENTIFIER_AUTHORITY SIDAuthNt=SECURITY_NT_AUTHORITY;
  HANDLE hDir;SECURITY_DESCRIPTOR* pSD=NULL;
  if(CreateDirectory(szPath,NULL)){
    if((hDir=CreateFile(szPath,READ_CONTROL|WRITE_DAC,0,NULL,OPEN_EXISTING,FILE_FLAG_BACKUP_SEMANTICS,NULL))!=INVALID_HANDLE_VALUE){
      if(AllocateAndInitializeSid(&SIDAuthNt,2,SECURITY_BUILTIN_DOMAIN_RID,DOMAIN_ALIAS_RID_USERS,0,0,0,0,0,0,&pUsersSid)){
        ZeroMemory(&ea, sizeof(ea));
        // Set read/write access for all...
        ea.grfAccessPermissions = GENERIC_ALL;
        ea.grfAccessMode = GRANT_ACCESS;
        ea.grfInheritance = CONTAINER_INHERIT_ACE|OBJECT_INHERIT_ACE;
        ea.Trustee.TrusteeForm = TRUSTEE_IS_SID;
        ea.Trustee.TrusteeType = TRUSTEE_IS_GROUP;
        ea.Trustee.ptstrName = (LPTSTR)pUsersSid;
        //update access
        GetSecurityInfo(hDir,SE_FILE_OBJECT,DACL_SECURITY_INFORMATION,NULL,NULL,&pOldDACL,NULL,&pSD);
        SetEntriesInAcl(1,&ea,pOldDACL,&acl);
        SetSecurityInfo(hDir,SE_FILE_OBJECT,DACL_SECURITY_INFORMATION,NULL,NULL,acl,NULL);
        FreeSid(pUsersSid);
        LocalFree(pSD);
        LocalFree(pOldDACL);
        LocalFree(acl);
      }
      CloseHandle(hDir);
    }
    return TRUE;
  }
  return FALSE;
}

BOOL DirectoryIsEmpty(LPCTSTR sPath){
  WIN32_FIND_DATA FindData;HANDLE hFind;
  TCHAR szTemp[MAX_PATH];
  wsprintf(szTemp,_T("%s\\*"),(PCHAR)sPath);
  if((hFind=FindFirstFile(szTemp,&FindData))!=INVALID_HANDLE_VALUE){
    do{
      if(PATHISDOTORDOTDOT(FindData.cFileName))
        continue;
      FindClose(hFind);
      return FALSE;
    }while(FindNextFile(hFind,&FindData));
    FindClose(hFind);
  }
  return TRUE;
}

BOOL DeleteDirectory(LPCTSTR sPath){
  WIN32_FIND_DATA FindData;HANDLE hFind;
  TCHAR szTemp[MAX_PATH];
  wsprintf(szTemp,_T("%s\\*"),(PCHAR)sPath);
  if((hFind=FindFirstFile(szTemp,&FindData))!=INVALID_HANDLE_VALUE){
    do{
      if(!PATHISDOTORDOTDOT(FindData.cFileName)){
        if(ISDIRFINDDATA(FindData)){
          wsprintf(szTemp,_T("%s\\%s"),(PCHAR)sPath,FindData.cFileName);
          DeleteDirectory(szTemp);
        }
      }
    }while(FindNextFile(hFind,&FindData));
    FindClose(hFind);
  }
  if(DirectoryIsEmpty(sPath))
    return RemoveDirectory(sPath);
  return FALSE;
}

LPCTSTR GetModulePath(HMODULE hModule,LPTSTR lpFilename,DWORD nSize){
  PTCHAR pName=NULL;
  if(GetModuleFileName(hModule,lpFilename,nSize)){
    for(PTCHAR p=lpFilename;*p;p++)
      if(*p=='\\')pName=p;
    if(pName)
      *(pName++)=0;
  }
  return pName;
}

DWORD FindProcess(LPCTSTR sProcName){
  HANDLE hSnapshot;PROCESSENTRY32 pEntry32;
  pEntry32.dwSize=sizeof(PROCESSENTRY32);
  hSnapshot=CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
  if(hSnapshot==INVALID_HANDLE_VALUE)return 0;
  if(!Process32First(hSnapshot,&pEntry32)){
    CloseHandle(hSnapshot);
    return 0;
  }
  do{
    if(!_tcsicmp(pEntry32.szExeFile,sProcName)){
      CloseHandle(hSnapshot);
      return pEntry32.th32ProcessID;
    }
  }while(Process32Next(hSnapshot,&pEntry32));
  CloseHandle(hSnapshot);
  return 0;
}

BOOL CALLBACK FindMainWindowEnum(HWND hwnd,LPARAM data){
  DWORD th32ProcessID,th32ThreadID;
  PFINDMAINWINDOWINFO ucppi=(PFINDMAINWINDOWINFO)data;
  if(GetWindowLong(hwnd,GWL_HWNDPARENT)==0){
    th32ThreadID=GetWindowThreadProcessId(hwnd,&th32ProcessID);
    if(ucppi->th32ProcessID==th32ProcessID){
      ucppi->th32ThreadID=th32ThreadID;
      ucppi->hWnd=hwnd;
      return FALSE;
    }
	}
	return TRUE;
}

HWND WINAPI FindMainWindow(DWORD dwProcId,PDWORD pdwThreadId){
  FINDMAINWINDOWINFO data={dwProcId,0,NULL};
  EnumWindows(&FindMainWindowEnum,(LPARAM)&data);
  if(pdwThreadId)*pdwThreadId=data.th32ThreadID;
  return data.hWnd;
}

BOOL GetAdminRights(){
  HANDLE Token;BOOL UserIsAdmin=FALSE;
  if(OpenProcessToken(GetCurrentProcess(),TOKEN_QUERY,&Token)){
    DWORD InfoLength,GroupIndex;PSID AdminSid;
    PTOKEN_GROUPS TokenGroupList;
    SID_IDENTIFIER_AUTHORITY SystemSidAuthority={SECURITY_NT_AUTHORITY};
    GetTokenInformation(Token,TokenGroups,NULL,0,&InfoLength);
    if(GetLastError()==ERROR_INSUFFICIENT_BUFFER){
      TokenGroupList=(PTOKEN_GROUPS)LocalAlloc(LPTR,InfoLength);
      if(TokenGroupList){
        if(GetTokenInformation(Token,TokenGroups,TokenGroupList,InfoLength,&InfoLength)){
          if(AllocateAndInitializeSid(&SystemSidAuthority,2,SECURITY_BUILTIN_DOMAIN_RID,
            DOMAIN_ALIAS_RID_ADMINS,0, 0, 0, 0, 0, 0,&AdminSid)){
            for(GroupIndex=0; GroupIndex < TokenGroupList->GroupCount; GroupIndex++ ) {
              if (EqualSid(TokenGroupList->Groups[GroupIndex].Sid, AdminSid)) {
                UserIsAdmin=TRUE;
                break;
              }
            }
            FreeSid(AdminSid);
            LocalFree(TokenGroupList);
          }
        }
      }
    }
    CloseHandle(Token);
  }
  return(UserIsAdmin);
}

VOID SetThreadLocalSettings(LANGID Language,LANGID SubLanguage){
  typedef  BOOL  (WINAPI *tSetProcessPreferredUILanguages)(DWORD,PCWSTR,PULONG);
  typedef LANGID (WINAPI *tSetThreadUILanguage)(LANGID wReserved);
  static tSetProcessPreferredUILanguages pSetProcessPreferredUILanguages=NULL;
  static tSetThreadUILanguage pSetThreadUILanguage=NULL;
  static BOOL bProcess=FALSE;ULONG ulNumOfLangs=1;WCHAR wszLanguages[32];
  OSVERSIONINFOEX osver;
  ZeroMemory(&osver, sizeof(osver));
  osver.dwOSVersionInfoSize = sizeof(osver);
  GetVersionEx((OSVERSIONINFO *)&osver);
  if(osver.dwMajorVersion>5){
    if(!pSetProcessPreferredUILanguages||!pSetThreadUILanguage){
      HMODULE hKernel32=GetModuleHandle("kernel32.dll");
      if(hKernel32){
        pSetProcessPreferredUILanguages=(tSetProcessPreferredUILanguages)GetProcAddress(hKernel32,"SetProcessPreferredUILanguages");
        pSetThreadUILanguage=(tSetThreadUILanguage)GetProcAddress(hKernel32,"SetThreadUILanguage");
      }
    }
    if(!bProcess&&pSetProcessPreferredUILanguages){
      wsprintfW(wszLanguages,L"%04X%c",MAKELANGID(Language,SubLanguage),0);
      pSetProcessPreferredUILanguages(MUI_LANGUAGE_ID,wszLanguages,&ulNumOfLangs);
      bProcess=TRUE;
    }
    if(pSetThreadUILanguage)// vista or 2008
      pSetThreadUILanguage(Language);
  }
  ::SetThreadLocale(MAKELCID(MAKELANGID(Language,SubLanguage),SORT_DEFAULT));
}

unsigned char *MD5File(LPCTSTR sFileName){
  static unsigned char rBuf[1024];int i;
  unsigned char md5digest[MD5_DIGEST_LENGTH];
  unsigned char bBuffer[4096];MD5_CTX md5Hash;
  HANDLE hFile=CreateFile(sFileName,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
  if(hFile!=INVALID_HANDLE_VALUE){
    memset(&md5Hash,0,sizeof(md5Hash));
    MD5_Init(&md5Hash);
    DWORD dwNumRead=0;
    while(ReadFile(hFile,bBuffer,sizeof(bBuffer),&dwNumRead,NULL)&&dwNumRead)
      MD5_Update(&md5Hash,bBuffer,dwNumRead);
    MD5_Final(md5digest,&md5Hash);
    CloseHandle(hFile);
    for(i=0;i<16;i++)
      sprintf((char*)&rBuf[i*2],"%02x",md5digest[i]&0xFF);
    return rBuf;
  }
  return NULL;
}

ULONG Reflect(ULONG ref,BYTE ch){
  ULONG value=0,i;
  for(i=1;i<(ULONG)(ch+1);i++){
    if(ref&1)value|=1<<(ch-i);
    ref>>=1;
  }
  return value;
}

VOID InitCRC32Table(){
  static BOOL bInitialized=FALSE;
  if(!bInitialized){
    ULONG ulPolynomial=0x04c11db7,i,j;
    for(i=0;i<=0xFF;i++){
      crc32_table[i]=Reflect(i,8)<<24;
      for(j=0;j<8;j++)
        crc32_table[i]=(crc32_table[i]<<1)^(crc32_table[i]&(1<<31)?ulPolynomial:0);
      crc32_table[i]=Reflect(crc32_table[i],32);
    }
    bInitialized=TRUE;
  }
}

ULONG CRC32(LPCTSTR text){
  ULONG ulCRC=0xffffffff;
  PBYTE buffer=(PBYTE)text;
  InitCRC32Table();
  for(;*buffer;buffer++)
    ulCRC=(ulCRC>>8)^crc32_table[(ulCRC&0xFF)^*buffer];
  return (ulCRC^0xffffffff);
}

UINT CRC32File(LPCTSTR sFileName){
  unsigned char bBuffer[4096];
  ULONG ulCRC=0xffffffff;
  HANDLE hFile=CreateFile(sFileName,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
  if(hFile!=INVALID_HANDLE_VALUE){
    DWORD dwNumRead=0;
    InitCRC32Table();
    while(ReadFile(hFile,bBuffer,sizeof(bBuffer),&dwNumRead,NULL)&&dwNumRead)
    for(DWORD dwLoop=0;dwLoop<dwNumRead;dwLoop++)
      ulCRC=(ulCRC>>8)^crc32_table[(ulCRC&0xFF)^bBuffer[dwLoop]];
    CloseHandle(hFile);
  }
  return (~ulCRC);
}

BOOL FindAndShowWindow(HINSTANCE hInst){
  TCHAR sFName[MAX_PATH];
  LPCTSTR pName=GetModulePath(hInst,sFName,MAX_PATH);
  if(!pName)return FALSE;
  DWORD dwProcId=FindProcess(pName);
  if(!dwProcId)return FALSE;
  HWND hWnd=FindMainWindow(dwProcId,NULL);
  if(!hWnd)return FALSE;
  ShowWindow(hWnd,SW_SHOW);
  SetForegroundWindow(hWnd);
  return TRUE;
}

BOOL IsWow64Process(HANDLE hProcess){
  typedef BOOL (WINAPI *LPFN_ISWOW64PROCESS)(HANDLE,PBOOL);
  static LPFN_ISWOW64PROCESS fnIsWow64Process=NULL;
  BOOL bIsWow64=FALSE;
  if(!fnIsWow64Process){
    HMODULE hKernel32=GetModuleHandle("kernel32.dll");
    fnIsWow64Process=(LPFN_ISWOW64PROCESS)GetProcAddress(hKernel32,"IsWow64Process");
  }
  if(fnIsWow64Process)
    fnIsWow64Process(hProcess,&bIsWow64);
  return bIsWow64;
}

BOOL IsWow64(){
  static BOOL bIsWow64=FALSE;
  if(!bIsWow64)
    bIsWow64=IsWow64Process(GetCurrentProcess());
  return bIsWow64;
}

BOOL GetFileVersion(LPCSTR szVersionFile,OSVERSIONINFO *info){
  DWORD verHandle=0;UINT size=0;LPBYTE lpBuffer=NULL;
  DWORD verSize=GetFileVersionInfoSize(szVersionFile,&verHandle);
  if(verSize!=0){
    LPSTR verData=(LPSTR)GlobalAlloc(GPTR,verSize);
    if(GetFileVersionInfo(szVersionFile,verHandle,verSize,verData)){
      if(VerQueryValue(verData,"\\",(VOID FAR* FAR*)&lpBuffer,&size)){
        if(size){
          VS_FIXEDFILEINFO *verInfo=(VS_FIXEDFILEINFO*)lpBuffer;
          if(verInfo->dwSignature==0xfeef04bd){
            info->dwMajorVersion=(verInfo->dwFileVersionMS >> 16)&0xffff;
            info->dwMinorVersion=(verInfo->dwFileVersionMS >>  0)&0xffff;
            info->dwPlatformId=(verInfo->dwFileVersionLS >> 16)&0xffff;
            info->dwBuildNumber=(verInfo->dwFileVersionLS >>  0)&0xffff;
            GlobalFree(verData);
            return TRUE;
          }
        }
      }
    }
    GlobalFree(verData);
  }
  return FALSE;
}

INT GetWindowsVr(){
  OSVERSIONINFO osVI;
  osVI.dwOSVersionInfoSize=sizeof(osVI);
  GetVersionEx(&osVI);
  INT wversion=0;
  switch(osVI.dwMajorVersion){
    case 6:
      switch(osVI.dwMinorVersion){
        case 0:
          wversion=VISTA;
        break;
        case 1:
          wversion=WIN7;
        break;
        case 2:
          wversion=WIN8;
        break;
        case 3:
          wversion=WIN81;
        break;
        default:break;
      }
    break;
    case 10:
      if(osVI.dwMinorVersion==0){
        switch(osVI.dwBuildNumber){
          case 10240:
            wversion=WIN10;
          break;
          case 10586:
            wversion=WIN10TH2;
          break;
          case 14393:
            wversion=WIN10AU;
          break;
          case 15063:
            wversion=WIN10CU;
          break;
          case 16299:
            wversion=WIN10FCU;
          break;
          default:break;
        }
      }
    break;
    default:break;
  }
  return wversion;
}

static BOOL bDoFsRedirection=TRUE;
VOID RtlWow64EnableFsRedirection(BOOL bEnable){
  typedef VOID (WINAPI *tRtlWow64EnableFsRedirection)(BOOL bEnable);
  static tRtlWow64EnableFsRedirection pRtlWow64EnableFsRedirection=NULL;
  if(!pRtlWow64EnableFsRedirection){
    HMODULE hNtDll=GetModuleHandle("ntdll.dll");
    pRtlWow64EnableFsRedirection=(tRtlWow64EnableFsRedirection)GetProcAddress(hNtDll,"RtlWow64EnableFsRedirection");
  }
  if(pRtlWow64EnableFsRedirection)
    pRtlWow64EnableFsRedirection(bEnable);
  bDoFsRedirection=bEnable;
}

DWORD RunCommand(LPSTR cmd,LPSTR buffer,DWORD nlen,DWORD maxtime){
  SECURITY_ATTRIBUTES sattr;HANDLE readfh,writefh;
  PROCESS_INFORMATION pinfo;STARTUPINFO sinfo;
  DWORD ExitCode=(DWORD)-1;
  if(cmd){
    ZeroMemory(&sinfo,sizeof(STARTUPINFO));
    sinfo.cb=sizeof(STARTUPINFO);
    sinfo.dwFlags=STARTF_USESHOWWINDOW;
    sinfo.wShowWindow=SW_HIDE;
    if(buffer&&nlen){
      sattr.nLength=sizeof(SECURITY_ATTRIBUTES);
      sattr.lpSecurityDescriptor=NULL;
      sattr.bInheritHandle=TRUE;
      if(CreatePipe(&readfh,&sinfo.hStdOutput,&sattr,0)){
        if(CreatePipe(&sinfo.hStdInput,&writefh,&sattr,0)){
          sinfo.hStdError=sinfo.hStdOutput;
          sinfo.dwFlags|=STARTF_USESTDHANDLES;
          RtlWow64EnableFsRedirection(FALSE);
          if(CreateProcess(NULL,cmd,NULL,NULL,TRUE,0,NULL,NULL,&sinfo,&pinfo)){
            if(WaitForSingleObjectEx(pinfo.hProcess,maxtime,FALSE)==WAIT_TIMEOUT)
              TerminateProcess(pinfo.hProcess,ExitCode);
            else
              GetExitCodeProcess(pinfo.hProcess,&ExitCode);
            if(ReadFile(readfh,buffer,nlen,&nlen,NULL)&&nlen)
              buffer[nlen]=0;
            CloseHandle(pinfo.hProcess);
            CloseHandle(pinfo.hThread);
          }
          RtlWow64EnableFsRedirection(TRUE);
          CloseHandle(sinfo.hStdInput);
          CloseHandle(writefh);
        }
        CloseHandle(sinfo.hStdOutput);
        CloseHandle(readfh);
      }
    }else{
      RtlWow64EnableFsRedirection(FALSE);
      if(CreateProcess(NULL,cmd,NULL,NULL,TRUE,0,NULL,NULL,&sinfo,&pinfo)){
        if(WaitForSingleObjectEx(pinfo.hProcess,maxtime,FALSE)==WAIT_TIMEOUT)
          TerminateProcess(pinfo.hProcess,ExitCode);
        else
          GetExitCodeProcess(pinfo.hProcess,&ExitCode);
        CloseHandle(pinfo.hProcess);
        CloseHandle(pinfo.hThread);
      }
      RtlWow64EnableFsRedirection(TRUE);
    }
  }
  return ExitCode;
}

BOOL Compare(PBYTE pData,PBYTE bMask,DWORD dwMaskLen){
	for(;dwMaskLen;++pData,++bMask,dwMaskLen--)
		if(*bMask!=0xFF && *pData!=*bMask)
		  return FALSE;
	return !dwMaskLen;
}

PBYTE FindPattern(PBYTE dwAddress,DWORD dwLen,PBYTE bMask,DWORD dwMaskLen,DWORD offset){
	for(DWORD i=0; i<dwLen; i++)
		if(Compare((PBYTE)(dwAddress+i),bMask,dwMaskLen))
		  return (PBYTE)(dwAddress+i+offset);
	return 0;
}
