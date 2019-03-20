#include "connection.h"

LPTSTR Connection::GetLastErrorMsg(){
  static TCHAR szMsgBuf[INET_ERROR_BUFFER_SIZE];
  if(::FormatMessage(FORMAT_MESSAGE_FROM_HMODULE|FORMAT_MESSAGE_FROM_SYSTEM,GetModuleHandle(_T("wininet.dll")),
    ::GetLastError(),0,szMsgBuf,INET_ERROR_BUFFER_SIZE,NULL))
    return szMsgBuf;
  return NULL;
}

DWORD Connection::Login(){
  return 0;
}

DWORD Connection::Activate(){
  return 0;
}

DWORD Connection::GetStatus(){
  return 0;
}

DWORD Connection::Logout(){
  return 0;
}

DWORD Connection::CheckUpdate(){
  return 0;
}

DWORD Connection::GetFile(){
  return 0;
}

DWORD Connection::Update(){
  return 0;
}

DWORD Connection::Connect(){
  return 0;
}

BOOL Connection::IsBusy(){
  return (iRequest.IsValid()||iConnection.IsValid()||iSession.IsValid());
}

VOID Connection::Disconnect(){
  //save connect() last error
  DWORD lastError=GetLastError();
  iRequest.Close();
  iConnection.Close();
  iSession.Close();
  //there was an error in connect() ? return it!
  if(lastError!=0){
    SetLastError(lastError);
  }
}

BOOL InternetStatus::OnStatusCallback(HINTERNET /*hInternet*/,DWORD /*dwContext*/,DWORD /*dwInternetStatus*/,LPVOID /*lpvStatusInformation*/,DWORD /*dwStatusInformationLength*/){
  return TRUE;
}
