#include "xfile.h"

/*******************************************************************/
// XFile
/*******************************************************************/

IMPLEMENT_DYNAMIC(XFile,XObject)
IMPLEMENT_DYNAMIC(XFileFind,XObject)

XFile::XFile(LPCTSTR lpszFileName,DWORD dwCreateAccessFlag,DWORD dwShareMode,DWORD dwFlags,LPSECURITY_ATTRIBUTES lpSecurityAttributes){
  XASSERT(lpszFileName!=NULL)
  if(lpszFileName!=NULL)
    Create(lpszFileName,dwCreateAccessFlag,dwShareMode,dwFlags,lpSecurityAttributes);
}

XFile::~XFile(){
  if(m_hFile&&m_hFile!=INVALID_HANDLE_VALUE&&m_bAttached)
    Close();
}

HANDLE XFile::Create(LPCTSTR lpszFileName,DWORD dwCreateAccessFlag,DWORD dwShareMode,DWORD dwFlags,LPSECURITY_ATTRIBUTES lpSecurityAttributes){
  XASSERT(m_hFile==NULL&&lpszFileName!=NULL)
  if(lpszFileName!=NULL)
    return Attach(::CreateFile(lpszFileName,dwCreateAccessFlag&0xF0000000,dwShareMode,lpSecurityAttributes,dwCreateAccessFlag&0xF,dwFlags,NULL));
  return NULL;
}

BOOL XFile::Read(PVOID lpBuf,UINT nCount,LPDWORD lpBytesRead,LPOVERLAPPED lpOverlapped){
  DWORD dwRead=0;
	if(!m_hFile||m_hFile==INVALID_HANDLE_VALUE||!lpBuf||!nCount)
    return FALSE;
	BOOL bRet=::ReadFile(m_hFile,lpBuf,nCount,&dwRead,lpOverlapped);
	if(lpBytesRead)
    *lpBytesRead=dwRead;
	return bRet;
}

BOOL XFile::Write(LPCVOID lpBuf,UINT nCount,LPDWORD lpBytesWrite,LPOVERLAPPED lpOverlapped){
  DWORD nWritten=0;
	if(!m_hFile||m_hFile==INVALID_HANDLE_VALUE||!lpBuf||!nCount)
    return FALSE;
	BOOL bRet=::WriteFile(m_hFile,lpBuf,nCount,&nWritten,lpOverlapped);
	if(lpBytesWrite)
    *lpBytesWrite=nWritten;
  return bRet;
}

BOOL XFile::ReadEx(PVOID lpBuf,UINT nCount,LPOVERLAPPED lpOverlapped,LPOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine){
	if(!m_hFile||m_hFile==INVALID_HANDLE_VALUE||!lpBuf||!nCount)
    return FALSE;
	return ::ReadFileEx(m_hFile,lpBuf,nCount,lpOverlapped,lpCompletionRoutine);
}

BOOL XFile::WriteEx(LPCVOID lpBuf,UINT nCount,LPOVERLAPPED lpOverlapped,LPOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine){
	if(!m_hFile||m_hFile==INVALID_HANDLE_VALUE||!lpBuf||!nCount)
    return FALSE;
	return ::WriteFileEx(m_hFile,lpBuf,nCount,lpOverlapped,lpCompletionRoutine);
}

ULONGLONG XFile::Seek(LONGLONG lOff,UINT nFrom){
  LARGE_INTEGER liOff;
	if(!m_hFile||m_hFile==INVALID_HANDLE_VALUE)
    return 0;
  liOff.QuadPart=lOff;
  liOff.LowPart=::SetFilePointer(m_hFile,liOff.LowPart,&liOff.HighPart,(DWORD)nFrom);
	if(liOff.LowPart!=INVALID_SET_FILE_POINTER)
    return liOff.QuadPart;
  return 0;
}

ULONGLONG XFile::GetPosition() const{
	LARGE_INTEGER liPos;
	if(!m_hFile||m_hFile==INVALID_HANDLE_VALUE)
    return 0;
  liPos.QuadPart=0;
	liPos.LowPart=::SetFilePointer(m_hFile, liPos.LowPart, &liPos.HighPart , FILE_CURRENT);
	if(liPos.LowPart!=INVALID_SET_FILE_POINTER)
	  return liPos.QuadPart;
  return 0;
}

BOOL XFile::Flush(){
	if(m_hFile&&m_hFile!=INVALID_HANDLE_VALUE)
		return FlushFileBuffers(m_hFile);
  return FALSE;
}

BOOL XFile::Close(){
  if(m_hFile&&m_hFile!=INVALID_HANDLE_VALUE)
    return CloseHandle(Detach());
  return FALSE;
}

BOOL XFile::Lock(ULONGLONG dwPos,ULONGLONG dwCount){
	if(m_hFile&&m_hFile!=INVALID_HANDLE_VALUE){
    ULARGE_INTEGER liPos,liCount;
    liPos.QuadPart=dwPos;
    liCount.QuadPart=dwCount;
    return ::LockFile(m_hFile,liPos.LowPart,liPos.HighPart,liCount.LowPart,liCount.HighPart);
	}
	return FALSE;
}

BOOL XFile::Unlock(ULONGLONG dwPos,ULONGLONG dwCount){
	if(m_hFile&&m_hFile!=INVALID_HANDLE_VALUE){
    ULARGE_INTEGER liPos,liCount;
    liPos.QuadPart=dwPos;
    liCount.QuadPart=dwCount;
    return ::UnlockFile(m_hFile, liPos.LowPart, liPos.HighPart, liCount.LowPart,liCount.HighPart);
  }
	return FALSE;
}

BOOL XFile::SetLength(ULONGLONG dwNewLen){
	if(m_hFile&&m_hFile!=INVALID_HANDLE_VALUE){
    Seek(dwNewLen,FILE_BEGIN);
    return ::SetEndOfFile(m_hFile);
	}
	return FALSE;
}

ULONGLONG XFile::GetLength()const{
  ULARGE_INTEGER liSize;
	if(m_hFile&&m_hFile!=INVALID_HANDLE_VALUE){
    liSize.LowPart=::GetFileSize(m_hFile,&liSize.HighPart);
    if(liSize.LowPart!=INVALID_FILE_SIZE)
      return liSize.QuadPart;
	}
	return 0;
}

BOOL XFile::GetLengthEx(PLARGE_INTEGER lpFileSize){
  if(m_hFile&&m_hFile!=INVALID_HANDLE_VALUE)
    return ::GetFileSizeEx(m_hFile,lpFileSize);
  return FALSE;
}

BOOL XFile::GetFileTime(LPFILETIME lpCreationTime,LPFILETIME lpLastAccessTime,LPFILETIME lpLastWriteTime){
  if(m_hFile&&m_hFile!=INVALID_HANDLE_VALUE)
    return ::GetFileTime(m_hFile,lpCreationTime,lpLastAccessTime,lpLastWriteTime);
  return FALSE;
}

BOOL XFile::SetFileTime(LPFILETIME lpCreationTime,LPFILETIME lpLastAccessTime,LPFILETIME lpLastWriteTime){
  if(m_hFile&&m_hFile!=INVALID_HANDLE_VALUE)
    return ::SetFileTime(m_hFile,lpCreationTime,lpLastAccessTime,lpLastWriteTime);
  return FALSE;
}
