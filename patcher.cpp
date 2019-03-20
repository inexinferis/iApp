#include "patcher.h"
#include "utils.h"

Patcher::Patcher(PPatchTable _patchTable,INT _tableEntries){
  pBuffer=NULL;pData=NULL;dwSize=0;dwLastError=0;
  ZeroMemory(szPath,sizeof(szPath));
  ZeroMemory(szNewPath,sizeof(szNewPath));
  allowoverwrite=FALSE;
  patchTable=_patchTable;
  tableEntries=_tableEntries;
}

Patcher::~Patcher(){
  if(pBuffer){
    GlobalFree(pBuffer);
  }
}

BOOL Patcher::Init(INT vr,LPCSTR file,LPCSTR newName,BOOL aow){
  BOOL bRet=FALSE;
  if(file&&newName&&vr){
    RtlWow64EnableFsRedirection(FALSE);
    if(FileExist(file)){
      strcpy(szPath,file);
      strcpy(szNewPath,newName);
      allowoverwrite=aow;
      //load patterns
      for(int i=0;i<tableEntries;i++){
        if(patchTable[i].vr==vr){
          pData=&patchTable[i];
          break;
        }
      }
      bRet=(pData!=NULL);
      if(!bRet){
        dwLastError=2;
      }
    }else{
      dwLastError=1;
    }
    RtlWow64EnableFsRedirection(TRUE);
  }else{
    dwLastError=401;
  }
  return bRet;
}

BOOL Patcher::Patch(){
  INT i;PBYTE data;DWORD HeaderSum,CheckSum;
  if(pData&&Open()){
    for(i=0;i<pData->len&&i<MAXPATHRECORDS;i++){
      if(!(data=FindPattern(pBuffer,dwSize,pData->offsets[i].pattern,pData->offsets[i].patlent,pData->offsets[i].offset))){
        dwLastError=10+i;
        break;//pattern not found???
      }
      memmove(data,pData->offsets[i].data,pData->offsets[i].datalen);
      if(pData->offsets[i].offset2){
        if(pData->offsets[i].datalen2){
          memmove(data+pData->offsets[i].offset2,pData->offsets[i].data2,pData->offsets[i].datalen2);
        }else{
          memmove(data+pData->offsets[i].offset2,pData->offsets[i].data,pData->offsets[i].datalen);
        }
      }
    }
    if(i==pData->len){
      //Update checksum
      CheckSumMappedFile(pBuffer,dwSize,&HeaderSum,&CheckSum);
      PIMAGE_DOS_HEADER phdrDos=(PIMAGE_DOS_HEADER)pBuffer;
      PIMAGE_NT_HEADERS64 phdrNt64=(PIMAGE_NT_HEADERS64)(pBuffer+phdrDos->e_lfanew);
      phdrNt64->OptionalHeader.CheckSum=CheckSum;
      return Save();
    }
  }
  return FALSE;
}

BOOL Patcher::Open(){
  DWORD dwRead;BOOL bRet=FALSE;
  RtlWow64EnableFsRedirection(FALSE);
  if(INVALID_HANDLE_VALUE!=file.Create(szPath,OPEN_EXISTING|GENERIC_READ,FILE_SHARE_READ)){
    if(!pBuffer){
      dwSize=file.GetLength();
      if(dwSize!=(DWORD)-1){
        pBuffer=(PBYTE)GlobalAlloc(GPTR,dwSize);
      }else{
        dwLastError=4;
      }
    }
    if(pBuffer!=NULL){
      if(file.Read(pBuffer,dwSize,&dwRead,NULL)){
        //CheckSumMappedFile(pBuffer,dwSize,&HeaderSum,&CheckSum);
        bRet=TRUE;
      }else{
        dwLastError=6;
        GlobalFree(pBuffer);
        pBuffer=NULL;
      }
    }else{
      dwLastError=5;
    }
    file.Close();
  }else{
    dwLastError=3;
  }
  RtlWow64EnableFsRedirection(TRUE);
  return bRet;
}

BOOL Patcher::Save(){
  DWORD dwWrite;BOOL bRet=FALSE;
  if(pBuffer!=NULL){
    RtlWow64EnableFsRedirection(FALSE);
    if(INVALID_HANDLE_VALUE!=file.Create(szNewPath,(allowoverwrite?CREATE_ALWAYS:CREATE_NEW)|
      GENERIC_WRITE,FILE_SHARE_READ)){
      bRet=file.Write(pBuffer,dwSize,&dwWrite,NULL);
      if(!bRet){
        dwLastError=9;
      }
      file.Close();
    }else{
      dwLastError=8;
    }
    RtlWow64EnableFsRedirection(TRUE);
  }else{
    dwLastError=7;
  }
  return bRet;
}
