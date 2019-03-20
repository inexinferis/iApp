#ifndef _PATCHGUARDDISABLER_H_
#define _PATCHGUARDDISABLER_H_

#include "x\x.h"
#include "utils.h"
#include "patcher.h"
#include "bcdedit.h"

#define NTOSKRNL    "ntoskrnl"
#define WINLOAD     "winload"
#define NEWNTOSKRNL "ntkernel"
#define NEWWINLOAD  "osloader"

#define WVERSIONS 9

class IApp;

class PatchguardDisabler{
  public:
    PatchguardDisabler(IApp *pApp,LPCSTR _szPathName);
    virtual ~PatchguardDisabler(){}

    BOOL Init(INT vr,LPCSTR nntoskrnl=NEWNTOSKRNL,LPCSTR nwinload=NEWWINLOAD);
    BOOL IsPatched();
    BOOL IsCurrentPatched();
    BOOL CanBePatched();
    BOOL Patch(BOOL setDefault=FALSE);
    BOOL Restore();

    CHAR szWinLoadPath[MAX_PATH],szNewWinLoadPath[MAX_PATH];
    CHAR szNtOSKrnlPath[MAX_PATH],szNewNtOSKrnlPath[MAX_PATH];
    DWORD dwLastError;

  protected:

  private:
    CHAR sysDir[MAX_PATH];
    CHAR szPathName[8];
    BOOL checked,patched,deffiles;
    Patcher ntoskrnl,winload;
    BCDEdit *bcdedit;
    PBCDEditStruct bcdPatched,bcdOriginal;

    static LPCSTR origntoskrnl;
    static LPCSTR origwinload;
};

#endif // _PATCHGUARDDISABLER_H_
