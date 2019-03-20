#include "bcdedit.h"
#include "utils.h"

BCDEdit::BCDEdit(){
  if(IsWow64()){
    Load();
  }
}

BOOL BCDEdit::Load(){
  ZeroMemory(buffer,sizeof(buffer));
  ZeroMemory(lines,sizeof(lines));
  ZeroMemory(Entries,sizeof(Entries));
  bootmgr=NULL;current=NULL;_default=NULL;
  numlines=0;numentries=0;//
  if(!RunCommand((LPSTR)"bcdedit /enum",buffer,sizeof(buffer),5000)){
    return ParseData();
  }
  return FALSE;
}

BOOL BCDEdit::Export(LPCSTR name){
  CHAR sBuffer[MAX_PATH];
  if(name){
    sprintf(sBuffer,"bcdedit /export \"%s\"",name);
    return !RunCommand(sBuffer,NULL,0,5000);
  }
  return FALSE;
}

BOOL BCDEdit::Import(LPCSTR name,BOOL clean){
  CHAR sBuffer[MAX_PATH];
  if(name){
    sprintf(sBuffer,"bcdedit /import \"%s\"%s",name,clean?" /clean":NULL);
    return !RunCommand(sBuffer,NULL,0,5000);
  }
  return FALSE;
}

BOOL BCDEdit::Set(LPCSTR var,LPCSTR val){
  CHAR sBuffer[MAX_PATH];
  if(var&&val){
    sprintf(sBuffer,"bcdedit /set %s %s",var,val);
    return !RunCommand(sBuffer,NULL,0,5000);
  }
  return FALSE;
}

BOOL BCDEdit::Set(LPCSTR id,LPCSTR var,LPCSTR val){
  CHAR sBuffer[MAX_PATH];
  if(id&&var&&val){
    sprintf(sBuffer,"bcdedit /set %s %s %s",id,var,val);
    return !RunCommand(sBuffer,NULL,0,5000);
  }
  return FALSE;
}

BOOL BCDEdit::DeleteValue(LPCSTR val){
  CHAR sBuffer[MAX_PATH];
  if(val){
    sprintf(sBuffer,"bcdedit /deletevalue %s",val);
    return !RunCommand(sBuffer,NULL,0,5000);
  }
  return FALSE;
}

BOOL BCDEdit::DeleteValue(LPCSTR id,LPCSTR val){
  CHAR sBuffer[MAX_PATH];
  if(id&&val){
    sprintf(sBuffer,"bcdedit /deletevalue %s %s",id,val);
    return !RunCommand(sBuffer,NULL,0,5000);
  }
  return FALSE;
}

BOOL BCDEdit::Copy(LPCSTR name,LPCSTR newname,LPSTR newid){
  CHAR sBuffer[MAX_PATH];CHAR rBuffer[1024];
  if(name&&newname){
    sprintf(sBuffer,"bcdedit /copy %s /d \"%s\"",name,newname);
    if(!RunCommand(sBuffer,rBuffer,sizeof(rBuffer),5000)){
      if(newid){
        PCHAR p=strchr(rBuffer,'{');
        if(p){
          PCHAR q=strchr(p,'}');
          if(q){
            *(++q)=0;
            strcpy(newid,p);
          }
        }
      }
      return TRUE;
    }
  }
  return FALSE;
}

BOOL BCDEdit::Create(LPCSTR id,LPCSTR description,LPCSTR application,LPCSTR inherit,LPCSTR device){
  CHAR sBuffer[MAX_PATH];
  if(id){
    sprintf(sBuffer,"bcdedit /create %s",id);
    if(description)
      sprintf(sBuffer,"%s /description %s",sBuffer,description);
    if(application)
      sprintf(sBuffer,"%s /application %s",sBuffer,application);
    if(inherit)
      sprintf(sBuffer,"%s /inherit %s",sBuffer,inherit);
    if(device)
      sprintf(sBuffer,"%s /device %s",sBuffer,device);
    return !RunCommand(sBuffer,NULL,0,5000);
  }
  return FALSE;
}

BOOL BCDEdit::Delete(LPCSTR name,BOOL f,BOOL nocleanup){
  CHAR sBuffer[MAX_PATH];
  if(name){
    sprintf(sBuffer,"bcdedit /delete %s%s%s",name,(f?" /f":""),(nocleanup?" /nocleanup":""));
    return !RunCommand(sBuffer,NULL,0,5000);
  }
  return FALSE;
}

BOOL BCDEdit::SetDefault(LPCSTR id){
  CHAR sBuffer[MAX_PATH];
  if(id){
    sprintf(sBuffer,"bcdedit /default %s",id);
    return !RunCommand(sBuffer,NULL,0,5000);
  }
  return FALSE;
}

BOOL BCDEdit::ParseData(){
  INT curentry=0,startline=0;
  PCHAR p=buffer;
  while(p&&*p){
    lines[numlines++]=p;
    PCHAR q=strstr(p,"\r\n");
    if(q){
      *q=0;
      p=q+2;
    }else{
      p=NULL;
    }
  }
  for(int i=0;i<numlines;i++){
    if(strstr(lines[i],"--------")){
      curentry=numentries++;
      startline=i;
    }
    if(numentries){
      if(startline&&startline+1==i){
        p=lines[i];
        while(*(++p)!=' ');
        while(*(++p)==' ');
        strncpy(Entries[curentry].id,p,MAX_PATH);
      }
      if(!strnicmp(lines[i],"device",6)){
        p=lines[i]+6;
        while(*(++p)==' ');
        strncpy(Entries[curentry].device,p,MAX_PATH);
      }
      if(!strnicmp(lines[i],"description",11)){
        p=lines[i]+11;
        while(*(++p)==' ');
        strncpy(Entries[curentry].description,p,MAX_PATH);
      }
      if(!strnicmp(lines[i],"default",7)){
        p=lines[i]+7;
        while(*(++p)==' ');
        strncpy(Entries[curentry]._default,p,MAX_PATH);
      }
      if(!strnicmp(lines[i],"path",4)){
        p=lines[i]+4;
        while(*(++p)==' ');
        strncpy(Entries[curentry].path,p,MAX_PATH);
      }
      if(!strnicmp(lines[i],"kernel",6)){
        p=lines[i]+6;
        while(*(++p)==' ');
        strncpy(Entries[curentry].kernel,p,MAX_PATH);
      }
      if(!strnicmp(lines[i],"timeout",7)){
        p=lines[i]+7;
        while(*(++p)==' ');
        strncpy(Entries[curentry].timeout,p,10);
      }
      if(!strnicmp(lines[i],"testsigning",11)){
        p=lines[i]+11;
        while(*(++p)==' ');
        strncpy(Entries[curentry].testsigning,p,4);
      }
      if(!strnicmp(lines[i],"nointegritychecks",17)){
        p=lines[i]+17;
        while(*(++p)==' ');
        strncpy(Entries[curentry].nointegritychecks,p,4);
      }
      if(!strnicmp(lines[i],"debug",5)){
        p=lines[i]+5;
        while(*(++p)==' ');
        strncpy(Entries[curentry].debug,p,4);
      }
      if(!strnicmp(Entries[curentry].id,"{bootmgr}",9)) {
        bootmgr=&Entries[curentry];
      }
      if(!strnicmp(Entries[curentry].id,"{current}",9)) {
        current=&Entries[curentry];
      }
      if(!strnicmp(Entries[curentry].id,"{default}",9)) {
        _default=&Entries[curentry];
      }
    }
  }

  //current deleted but default is present
  if(!current&&_default){
    current=_default;
  }

  if(!_default){
    _default=current;
  }

  return (bootmgr&&current&&_default);
}
