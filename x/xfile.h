#ifndef _XFILE_H_
#define _XFILE_H_

#include "xobject.h"

class XFile:public XObject{
	DECLARE_DYNAMIC(XFile)
  public:
    XFile(HANDLE hFile=NULL):m_hFile(hFile),m_bAttached(FALSE){}
    XFile(LPCTSTR lpszFileName,DWORD dwCreateFlag=OPEN_EXISTING|GENERIC_ALL,
      DWORD dwShareMode=FILE_SHARE_READ,DWORD dwFlags=FILE_ATTRIBUTE_NORMAL,LPSECURITY_ATTRIBUTES lpSecurityAttributes=NULL);
    virtual ~XFile();

    BOOL IsValid(){
      return (this==NULL?FALSE:(m_hFile!=NULL&&m_hFile!=INVALID_HANDLE_VALUE));
    }

    XFile* Duplicate()const{
      XASSERT(m_hFile!=NULL&&m_hFile!=INVALID_HANDLE_VALUE);
      HANDLE hFile;
      if(::DuplicateHandle(::GetCurrentProcess(),m_hFile,::GetCurrentProcess(),&hFile,0,FALSE,DUPLICATE_SAME_ACCESS)){
        XFile* pFile=new XFile(hFile);
        pFile->m_bAttached=m_bAttached;
        return pFile;
      }
      return NULL;
    }

    HANDLE Attach(HANDLE hFile){
      XASSERT(m_hFile==NULL)
      if(!hFile||hFile==INVALID_HANDLE_VALUE)return NULL;
      m_bAttached=TRUE;
      m_hFile=hFile;
      return hFile;
    }

    HANDLE Detach(){
      HANDLE hFile=m_hFile;
      m_bAttached=FALSE;
      m_hFile=NULL;
      return hFile;
    }

    HANDLE GetSafeHandle() const {
      return (this==NULL?NULL:m_hFile);
    }

    virtual HANDLE Create(LPCTSTR lpszFileName,DWORD dwCreateAccessFlag=OPEN_EXISTING|GENERIC_ALL,
      DWORD dwShareMode=FILE_SHARE_READ,DWORD dwFlags=FILE_ATTRIBUTE_NORMAL,LPSECURITY_ATTRIBUTES lpSecurityAttributes=NULL);
    virtual BOOL Read(PVOID lpBuf,UINT nCount,LPDWORD lpBytesRead=NULL,LPOVERLAPPED lpOverlapped=NULL);
    virtual BOOL Write(LPCVOID lpBuf,UINT nCount,LPDWORD lpBytesWrite=NULL,LPOVERLAPPED lpOverlapped=NULL);
    virtual BOOL ReadEx(PVOID lpBuf,UINT nCount,LPOVERLAPPED lpOverlapped=NULL,LPOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine=NULL);
    virtual BOOL WriteEx(LPCVOID lpBuf,UINT nCount,LPOVERLAPPED lpOverlapped=NULL,LPOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine=NULL);

    virtual ULONGLONG Seek(LONGLONG lOff, UINT nFrom=FILE_BEGIN);
    virtual ULONGLONG GetPosition() const;
    virtual ULONGLONG GetLength() const;
    virtual BOOL SetLength(ULONGLONG dwNewLen);
    virtual BOOL GetLengthEx(PLARGE_INTEGER lpFileSize);
    virtual BOOL GetFileTime(LPFILETIME lpCreationTime,LPFILETIME lpLastAccessTime,LPFILETIME lpLastWriteTime);
    virtual BOOL SetFileTime(LPFILETIME lpCreationTime,LPFILETIME lpLastAccessTime,LPFILETIME lpLastWriteTime);

    virtual VOID SeekToBegin(){
      Seek(0,FILE_BEGIN);
    }

    virtual ULONGLONG SeekToEnd(){
      return Seek(0,FILE_END);
    }

    virtual BOOL Lock(ULONGLONG dwPos, ULONGLONG dwCount);
    virtual BOOL Unlock(ULONGLONG dwPos, ULONGLONG dwCount);

    virtual BOOL Flush();
    virtual BOOL Close();

    XFile &operator=(HANDLE hFile){
      XASSERT(m_hFile==NULL)
      m_hFile=hFile;
      return *this;
    }

    operator HANDLE() const{
      return (this==NULL?NULL:m_hFile);
    }

  private:
    HANDLE m_hFile;
  protected:
    BOOL m_bAttached;
};

class XFileFind:public XObject{
  DECLARE_DYNAMIC(XFileFind)
  public:
    XFileFind():m_hContext(NULL){
      ZeroMemory(&m_FindData,sizeof(m_FindData));
      ZeroMemory(m_szFullPath,sizeof(m_szFullPath));
    }

    virtual ~XFileFind(){
      Close();
    }

    BOOL IsValid(){
      return (this==NULL?FALSE:(m_hContext!=NULL&&m_hContext!=INVALID_HANDLE_VALUE));
    }

    virtual HANDLE FindFirst(LPCTSTR pstrName=NULL,DWORD dwUnused=0){
      XUNUSED_ALWAYS(dwUnused)
      if(IsValid())
        Close();
      if(pstrName==NULL)
        pstrName=_T("*.*");
      m_hContext=::FindFirstFile(pstrName,&m_FindData);
      if(m_hContext==INVALID_HANDLE_VALUE)
        m_hContext=NULL;
      else if(!_tfullpath(m_szFullPath,pstrName,MAX_PATH)){
        ::SetLastError(ERROR_INVALID_NAME);
        Close();
      }
      return m_hContext;
    }

    virtual BOOL FindNext(){
      XASSERT(m_hContext!=NULL)
      if(m_hContext==NULL)
        return FALSE;
      return ::FindNextFile(m_hContext,&m_FindData);
    }

    virtual BOOL Close(){
      if(m_hContext==NULL)
        return FALSE;
      BOOL bRet=::FindClose(m_hContext);
      m_hContext=NULL;
      return bRet;
    }

    ULONGLONG GetLength()const{
      XASSERT(m_hContext!=NULL)
      ULARGE_INTEGER nFileSize;
      nFileSize.LowPart=m_FindData.nFileSizeLow;
      nFileSize.HighPart=m_FindData.nFileSizeHigh;
      return nFileSize.QuadPart;
    }

    virtual LPCTSTR GetFileName()const{
      XASSERT(m_hContext!=NULL)
      return m_FindData.cFileName;
    }

    virtual LPCTSTR GetFilePath(LPTSTR szBuffer,INT iLen)const{
      XASSERT(m_hContext!=NULL)
      XASSERT(szBuffer!=NULL&&iLen!=0)
      LPCTSTR pszFileName=GetFileName();
      if((lstrlen(m_szFullPath)+lstrlen(pszFileName)+2)<=iLen){
        lstrcpy(szBuffer,m_szFullPath);
        LPTSTR pchLast=_tcsdec(szBuffer,szBuffer+lstrlen(szBuffer));
        XASSERT(pchLast!=NULL)
        if((*pchLast!=_T('\\'))&&(*pchLast!=_T('/')))
          *(++pchLast)='\\';
        lstrcpy(++pchLast,pszFileName);
        return szBuffer;
      }
      return NULL;
    }

    virtual LPCTSTR GetFileTitle(LPTSTR szBuffer,INT iLen)const{
      XASSERT(m_hContext!=NULL)
      XASSERT(szBuffer!=NULL&&iLen!=0)
      LPCTSTR pszFileName=GetFileName();
      if(lstrlen(pszFileName)<iLen){
        _tsplitpath(pszFileName,NULL,NULL,szBuffer,NULL);
        return szBuffer;
      }
      return NULL;
    }

    virtual LPCTSTR GetFileURL(LPTSTR szBuffer,INT iLen)const{
      XASSERT(m_hContext!=NULL)
      XASSERT(szBuffer!=NULL&&iLen!=0)
      if((lstrlen(m_szFullPath)+lstrlen(GetFileName())+9)<=iLen){
        lstrcpy(szBuffer,_T("file://"));
        GetFilePath(szBuffer+7,iLen-7);
        return szBuffer;
      }
      return NULL;
    }

    virtual LPCTSTR GetRoot()const{
      XASSERT(m_hContext!=NULL)
      return m_szFullPath;
    }

    virtual BOOL GetLastAccessTime(FILETIME* pTimeStamp)const{
      XASSERT(m_hContext!=NULL)
      XASSERT(pTimeStamp!=NULL)
      if(pTimeStamp!=NULL){
        *pTimeStamp=m_FindData.ftLastAccessTime;
        return TRUE;
      }
      return FALSE;
    }

    virtual BOOL GetLastWriteTime(FILETIME* pTimeStamp)const{
      XASSERT(m_hContext!=NULL)
      XASSERT(pTimeStamp!=NULL)
      if(pTimeStamp!=NULL){
        *pTimeStamp=m_FindData.ftLastWriteTime;
        return TRUE;
      }
      return FALSE;
    }

    virtual BOOL GetCreationTime(FILETIME* pTimeStamp)const{
      XASSERT(m_hContext!=NULL)
      XASSERT(pTimeStamp!=NULL)
      if(pTimeStamp!=NULL){
        *pTimeStamp=m_FindData.ftCreationTime;
        return TRUE;
      }
      return FALSE;
    }

    virtual BOOL MatchesMask(DWORD dwMask)const{
      XASSERT(m_hContext!=NULL)
      return (m_FindData.dwFileAttributes&dwMask);
    }

    virtual BOOL IsDots()const{
      XASSERT(m_hContext!=NULL)
      if(IsDirectory()&&m_FindData.cFileName[0]=='.'){
        if(m_FindData.cFileName[1]=='\0'||
          (m_FindData.cFileName[1]=='.'&&
          m_FindData.cFileName[2]=='\0'))
        return TRUE;
      }
      return FALSE;
    }

    BOOL IsReadOnly()const{
      return MatchesMask(FILE_ATTRIBUTE_READONLY);
    }

    BOOL IsDirectory()const{
      return MatchesMask(FILE_ATTRIBUTE_DIRECTORY);
    }

    BOOL IsCompressed()const{
      return MatchesMask(FILE_ATTRIBUTE_COMPRESSED);
    }

    BOOL IsSystem()const{
      return MatchesMask(FILE_ATTRIBUTE_SYSTEM);
    }

    BOOL IsHidden()const{
      return MatchesMask(FILE_ATTRIBUTE_HIDDEN);
    }

    BOOL IsTemporary() const{
      return MatchesMask(FILE_ATTRIBUTE_TEMPORARY);
    }

    BOOL IsNormal()const{
      return MatchesMask(FILE_ATTRIBUTE_NORMAL);
    }

    BOOL IsArchived() const{
      return MatchesMask(FILE_ATTRIBUTE_ARCHIVE);
    }

  protected:
    HANDLE m_hContext;
    WIN32_FIND_DATA m_FindData;
    TCHAR m_szFullPath[MAX_PATH];
};

/*******************************************************************/
//
/*******************************************************************/

#endif //_XFILE_H_
