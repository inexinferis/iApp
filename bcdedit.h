#ifndef _BCDEDIT_H_
#define _BCDEDIT_H_

#include "x\x.h"

#define MAX_BCDEDIT_BUFFER  4096
#define MAX_BCDEDIT_LINES   512
#define MAX_BCDEDIT_ENTRIES 10

typedef struct _BCDEditStruct{
  CHAR id[MAX_PATH];
  CHAR device[MAX_PATH];
  CHAR description[MAX_PATH];
  CHAR _default[MAX_PATH];
  CHAR path[MAX_PATH];
  CHAR kernel[MAX_PATH];
  CHAR timeout[10];
  CHAR testsigning[4];
  CHAR nointegritychecks[4];
  CHAR debug[4];
}BCDEditStruct,*PBCDEditStruct;

class BCDEdit{
  public:
    BCDEdit();
    virtual ~BCDEdit() {}

    BOOL Load();
    BOOL Export(LPCSTR name);
    BOOL Import(LPCSTR name,BOOL clean=FALSE);
    BOOL Set(LPCSTR var,LPCSTR val);
    BOOL Set(LPCSTR id,LPCSTR var,LPCSTR val);
    BOOL DeleteValue(LPCSTR val);
    BOOL DeleteValue(LPCSTR id,LPCSTR val);
    BOOL Copy(LPCSTR name,LPCSTR newname,LPSTR newid);
    BOOL Create(LPCSTR id,LPCSTR description=NULL,LPCSTR application=NULL,LPCSTR inherit=NULL,LPCSTR device=NULL);
    BOOL Delete(LPCSTR name,BOOL f=FALSE,BOOL cleanup=FALSE);
    BOOL SetDefault(LPCSTR id);
    BOOL Loaded(){return (bootmgr!=NULL);}

  private:
    BOOL ParseData();

    CHAR buffer[MAX_BCDEDIT_BUFFER];
    CHAR *lines[MAX_BCDEDIT_LINES];
    INT numlines;
    CHAR sCryptBuffer[MAX_PATH];

  public:
    BCDEditStruct Entries[MAX_BCDEDIT_ENTRIES];
    INT numentries;
    PBCDEditStruct bootmgr;
    PBCDEditStruct current;
    PBCDEditStruct _default;
};

#endif // _BCDEDIT_H_
