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
        #ifdef _DEBUG
          DbgPrint("--- Patcher::Init -> Init fail");
        #endif
      }
    }else{
      dwLastError=1;
      #ifdef _DEBUG
        DbgPrint("--- Patcher::Init -> File %s not exists\n",file);
      #endif
    }
    RtlWow64EnableFsRedirection(TRUE);
  }else{
    dwLastError=401;
    #ifdef _DEBUG
      DbgPrint("--- Patcher::Init -> Invalid arguments\n");
    #endif
  }
  return bRet;
}

BOOL Patcher::Patch(){
  INT i;PBYTE data;DWORD HeaderSum,CheckSum;
  if(pData&&Open()){
    for(i=0;i<pData->len&&i<MAXPATHRECORDS;i++){
      if(!(data=FindPattern(pBuffer,dwSize,pData->offsets[i].pattern,pData->offsets[i].patlent,pData->offsets[i].offset))){
        dwLastError=10+i;
        #ifdef _DEBUG
          DbgPrint("--- Patcher::Patch -> Pattern %d [%02X] not found for %s\n",i,pData->offsets[i].pattern[0],szPath);
          INT j=pData->offsets[i].patlent;
          for(;j>0;j--){
            if((data=FindPattern(pBuffer,dwSize,pData->offsets[i].pattern,j,pData->offsets[i].offset))){
              DbgPrint("--- Patcher::Patch -> Pattern %02X found for at %d [%p] - %s\n",
                pData->offsets[i].pattern[0],j,data-pBuffer,szPath);
              break;
            }
          }
        #endif
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
    #ifdef _DEBUG
      DbgPrint("--- Patcher::Patch -> Patch Pattern Status %d %d\n",i,pData->len);
    #endif
    if(i==pData->len){
      //Update checksum
      CheckSumMappedFile(pBuffer,dwSize,&HeaderSum,&CheckSum);
      PIMAGE_DOS_HEADER phdrDos=(PIMAGE_DOS_HEADER)pBuffer;
      PIMAGE_NT_HEADERS64 phdrNt64=(PIMAGE_NT_HEADERS64)(pBuffer+phdrDos->e_lfanew);
      phdrNt64->OptionalHeader.CheckSum=CheckSum;
      return Save();
    }
  }
  #ifdef _DEBUG
  else
    DbgPrint("--- Patcher::Patch -> Can't open file %s or pdata is empty -> 0x%p\n",szPath,pData);
  #endif
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
        #ifdef _DEBUG
          DbgPrint("--- Patcher::Open -> Get file size fail for %s\n",szPath);
        #endif
      }
    }
    if(pBuffer!=NULL){
      if(file.Read(pBuffer,dwSize,&dwRead,NULL)){
        //CheckSumMappedFile(pBuffer,dwSize,&HeaderSum,&CheckSum);
        bRet=TRUE;
      }else{
        dwLastError=6;
        #ifdef _DEBUG
          DbgPrint("--- Patcher::Open -> read file fail for %s\n",szPath);
        #endif
        GlobalFree(pBuffer);
        pBuffer=NULL;
      }
    }else{
      dwLastError=5;
      #ifdef _DEBUG
        DbgPrint("--- Patcher::Open -> Can't allocate memory for %s\n",szPath);
      #endif
    }
    file.Close();
  }else{
    dwLastError=3;
    #ifdef _DEBUG
      DbgPrint("--- Patcher::Open -> Open file %s fail\n",szPath);
    #endif
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
        #ifdef _DEBUG
          DbgPrint("--- Patcher::Save -> Write file fail for %s\n",szNewPath);
        #endif
      }
      file.Close();
    }else{
      dwLastError=8;
      #ifdef _DEBUG
        DbgPrint("--- Patcher::Save -> File open fail for %s\n",szNewPath);
      #endif
    }
    RtlWow64EnableFsRedirection(TRUE);
  }else{
    dwLastError=7;
    #ifdef _DEBUG
      DbgPrint("--- Patcher::Save -> No memory allocated for %s\n",szNewPath);
    #endif
  }
  return bRet;
}
