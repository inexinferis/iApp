#ifndef _PATCHER_H_
#define _PATCHER_H_

#include "x\x.h"
#include <imagehlp.h>

#define MAXPATHRECORDS  5
#define MAXBUFFERLEN    50
#define MAXFILENAMELEN  12

typedef struct _PatchTable {
  INT vr;
  INT len;
  struct _offsets {
    INT patlent;
    INT datalen;
    INT datalen2;
    INT offset;
    INT offset2;
    PBYTE pattern;//[MAXBUFFERLEN];
    PBYTE data;//[MAXBUFFERLEN];
    PBYTE data2;//[MAXBUFFERLEN];
  } offsets[MAXPATHRECORDS];
} PatchTable,*PPatchTable;

class Patcher {
  public:
    Patcher(PPatchTable _patchTable,INT _tableEntries);
    ~Patcher();

    BOOL Init(INT vr,LPCSTR file,LPCSTR newName,BOOL aow);
    BOOL Patch();

    CHAR szNewPath[MAX_PATH];
    DWORD dwLastError;

  private:
    BOOL Open();
    BOOL Save();

  private:
    PPatchTable patchTable,pData;
    BOOL allowoverwrite;
    INT tableEntries;
    CHAR szPath[MAX_PATH];
    CHAR version[32];
    CHAR checksum[32];
    DWORD dwSize;
    PBYTE pBuffer;
    XFile file;
};

#endif //_PATCHER_H_
