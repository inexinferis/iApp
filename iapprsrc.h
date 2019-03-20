#include <windows.h>

#define IDI_IAPPICON        0x50
#define IDI_DWICON          0x51

#define IDI_HLICON          0x60
#define IDI_CSICON          0x61
#define IDI_HL2ICON         0x62
#define IDI_CSSICON         0x63
#define IDI_CSGOICON        0x64

#define IDI_IRICON          0x70
#define IDI_SEICON          0x71
#define IDI_IRSICON         0x72
#define IDI_IRGOICON        0x73
#define IDI_HDICON          0x74

#define IDB_CSBMP           0x80
#define IDB_IRBMP           0x81
#define IDB_SEBMP           0x82

#define IDR_CERTIFICATE     0x99

#define SID_ADMINREQUIRED   0x100
#define SID_INIT_SESSION    0x101
#define SID_WELCOME         0x102
#define SID_USERNAME        0x103
#define SID_PASSWORD        0x104
#define SID_REMEMBER        0x105
#define SID_LOGIN           0x106
#define SID_CANCEL          0x107
#define SID_OK              0x108
#define SID_CLOSE           0x109
#define SID_PLAY            0x110
#define SID_INSTALL         0x111
#define SID_UNINSTALL       0x112
#define SID_ACTIVATE        0x113
#define SID_DOWNLOAD        0x114
#define SID_UPDATE          0x115
#define SID_BUY             0x116
#define SID_NOTAVAILABLE    0x117
#define SID_INFO            0x118
#define SID_UNLOAD          0x119

#define SID_STARTINGSESSION 0x200
#define SID_CONNECTING      0x201
#define SID_RESOLVING       0x202
#define SID_SENDINGREQUEST  0x203
#define SID_RECEIVINGRESPONSE 0x204
#define SID_CLOSINGCONNECTION 0x205
#define SID_COULDNOTCONNECT 0x206
#define SID_CLOSINGSESSION  0x207
#define SID_CONNECTIONOK    0x208

#define SID_INVALIDPROTOCOL 0x220
#define SID_USERNOTACTIVE   0x221
#define SID_IVALIDUSERDATA  0x222
#define SID_BANNEDACCOUNT   0x223
#define SID_FILENOTFOUND    0x224
#define SID_MD5CHECKFAIL    0x225
#define SID_UNINSTALLWARN   0x226
#define SID_ACTIVATEMSG     0x227
#define SID_ACTIVATEWARN    0x228
#define SID_ALREADYRUNNING  0x229
#define SID_LOADERROR       0x230
#define SID_CHEATLOADED     0x231
#define SID_CHEATUNLOADED   0x232
#define SID_CHEATNOTFOUND   0x233
#define SID_CHEATINVALID    0x234
#define SID_CHEATERROR      0x235
#define SID_INVALIDHWID     0x236
#define SID_CHEATINITFAIL   0x237

#define SID_IAPP_MENU       0x300
#define SID_LOGOUT_MENU     0x301
#define SID_UPDATE_MENU     0x302
#define SID_SETTINGS_MENU   0x303
#define SID_EXIT_MENU       0x304

#define SID_TOOLS_MENU      0X320
#define SID_PATCHGUARD_MENU 0X321

#define SID_HELP_MENU       0x350
#define SID_CONTENTS_MENU   0x351
#define SID_LICENCES_MENU   0x352
#define SID_ABOUT_MENU      0x353

#define SID_PATCHGUARD      0x360
#define SID_PATCH           0x361
#define SID_RESTORE         0x362
#define SID_PATCHED         0x363
#define SID_NOTPATCHED      0x364
#define SID_ADVANCEDOPTIONS 0x365
#define SID_DEFAULTENTRYE   0x366
#define SID_LOADERNAME      0x367
#define SID_KERNELNAME      0x368
#define SID_INCOMPATIBLEWVR 0x369
#define SID_INITFAIL        0x370
#define SID_INVALIDNAMES    0x371
#define SID_FILESEXISTS     0x372
#define SID_PATCHFAIL       0x373
#define SID_RESTOREFAIL     0x374
#define SID_ADV_WARNING     0x375

#define SID_ABOUT_IAPP      0x400
#define SID_ABOUT_DETAIL    0x401
#define SID_ABOUT_HWID      0x402

#define SID_IAPP_SETTINGS   0x420
#define SID_IAPP_LANG       0x421
#define SID_IAPP_AUTOLANG   0x422
#define SID_IAPP_ENGLANG    0x423
#define SID_IAPP_ESPLANG    0x424
#define SID_IAPP_AUTOSTART  0x425
#define SID_IAPP_MINSTART   0x426
#define SID_IAPP_FLOGOUT    0x427
#define SID_IAPP_NEEDRESTART 0x428

#define SID_IAPP_STATUS     0x450
#define SID_READYTOPLAY     0x451
#define SID_READYTOACTIVATE 0x452
#define SID_READYTODOWNLOAD 0x453
#define SID_READYTOINSTALL  0x454
#define SID_OUTDATEDVERSION 0x455
#define SID_FILESMISSING    0x456
#define SID_ACTIVATING      0x457
#define SID_DOWNLOADING     0x458
#define SID_RUNNING         0x459
#define SID_NOLICENCES      0x460

#define SID_APP_NAME        0x500
#define SID_APP_NAME_FULL   0x501
#define SID_WEB_URL         0x502
#define SID_APP_CREDITS     0x503
#define SID_INTERNAL_ERROR  0x600
#define SID_NOINFORMATION   0x601
#define SID_CHECKINGUPDATES 0x602
#define SID_UPDATEAVAILABLE 0x603
#define SID_MANUALUPDATE    0x604
#define SID_NOUPDATES       0x605
#define SID_UPDATING        0x606
#define SID_CONNECTIONERROR 0x607
#define SID_CONNECTIONLOST  0x608
#define SID_SYSTEMRESTART   0x609
#define SID_RESTARTREQUIRED 0x610
#define SID_APPCRACKED      0x611
#define SID_INVALIDTOKEN    0x612

#define SID_LICTIME         0x620
#define SID_PLAYDESC        0x621
#define SID_ACTDESC         0x622
#define SID_DOWNDESC        0x623
#define SID_UPDDESC         0x624
#define SID_MISSDESC        0x625
#define SID_NOLICDESC       0x626
#define SID_NOAVADESC       0x627

#define SID_SEMIPUBLICCHEATS  0x630
#define SID_PRIVATECHEATS   0x631
#define SID_PUBLICCHEATS    0x632

#define STR_ADMINREQUIRED   MAKEINTRESOURCE(SID_ADMINREQUIRED)
#define STR_INIT_SESSION    MAKEINTRESOURCE(SID_INIT_SESSION)
#define STR_WELCOME         MAKEINTRESOURCE(SID_WELCOME)
#define STR_USERNAME        MAKEINTRESOURCE(SID_USERNAME)
#define STR_PASSWORD        MAKEINTRESOURCE(SID_PASSWORD)
#define STR_REMEMBER        MAKEINTRESOURCE(SID_REMEMBER)
#define STR_LOGIN           MAKEINTRESOURCE(SID_LOGIN)
#define STR_CANCEL          MAKEINTRESOURCE(SID_CANCEL)
#define STR_OK              MAKEINTRESOURCE(SID_OK)
#define STR_CLOSE           MAKEINTRESOURCE(SID_CLOSE)
#define STR_PLAY            MAKEINTRESOURCE(SID_PLAY)
#define STR_INSTALL         MAKEINTRESOURCE(SID_INSTALL)
#define STR_UNINSTALL       MAKEINTRESOURCE(SID_UNINSTALL)
#define STR_ACTIVATE        MAKEINTRESOURCE(SID_ACTIVATE)
#define STR_DOWNLOAD        MAKEINTRESOURCE(SID_DOWNLOAD)
#define STR_UPDATE          MAKEINTRESOURCE(SID_UPDATE)
#define STR_BUY             MAKEINTRESOURCE(SID_BUY)
#define STR_NOTAVAILABLE    MAKEINTRESOURCE(SID_NOTAVAILABLE)
#define STR_INFO            MAKEINTRESOURCE(SID_INFO)
#define STR_UNLOAD          MAKEINTRESOURCE(SID_UNLOAD)


#define STR_STARTINGSESSION MAKEINTRESOURCE(SID_STARTINGSESSION)
#define STR_CONNECTING      MAKEINTRESOURCE(SID_CONNECTING)
#define STR_RESOLVING       MAKEINTRESOURCE(SID_RESOLVING)
#define STR_SENDINGREQUEST  MAKEINTRESOURCE(SID_SENDINGREQUEST)
#define STR_RECEIVINGRESPONSE MAKEINTRESOURCE(SID_RECEIVINGRESPONSE)
#define STR_CLOSINGCONNECTION MAKEINTRESOURCE(SID_CLOSINGCONNECTION)
#define STR_COULDNOTCONNECT MAKEINTRESOURCE(SID_COULDNOTCONNECT)
#define STR_CLOSINGSESSION  MAKEINTRESOURCE(SID_CLOSINGSESSION)
#define STR_CONNECTIONOK    MAKEINTRESOURCE(SID_CONNECTIONOK)


#define STR_INVALIDPROTOCOL MAKEINTRESOURCE(SID_INVALIDPROTOCOL)
#define STR_USERNOTACTIVE   MAKEINTRESOURCE(SID_USERNOTACTIVE)
#define STR_IVALIDUSERDATA  MAKEINTRESOURCE(SID_IVALIDUSERDATA)
#define STR_BANNEDACCOUNT   MAKEINTRESOURCE(SID_BANNEDACCOUNT)
#define STR_FILENOTFOUND    MAKEINTRESOURCE(SID_FILENOTFOUND)
#define STR_MD5CHECKFAIL    MAKEINTRESOURCE(SID_MD5CHECKFAIL)
#define STR_UNINSTALLWARN   MAKEINTRESOURCE(SID_UNINSTALLWARN)
#define STR_ALREADYRUNNING  MAKEINTRESOURCE(SID_ALREADYRUNNING)
#define STR_LOADERROR       MAKEINTRESOURCE(SID_LOADERROR)
#define STR_CHEATLOADED     MAKEINTRESOURCE(SID_CHEATLOADED)
#define STR_CHEATUNLOADED   MAKEINTRESOURCE(SID_CHEATUNLOADED)
#define STR_CHEATNOTFOUND   MAKEINTRESOURCE(SID_CHEATNOTFOUND)
#define STR_CHEATINVALID    MAKEINTRESOURCE(SID_CHEATINVALID)
#define STR_CHEATERROR      MAKEINTRESOURCE(SID_CHEATERROR)


#define STR_IAPP_MENU       MAKEINTRESOURCE(SID_IAPP_MENU)
#define STR_LOGOUT_MENU     MAKEINTRESOURCE(SID_LOGOUT_MENU)
#define STR_UPDATE_MENU     MAKEINTRESOURCE(SID_UPDATE_MENU)
#define STR_SETTINGS_MENU   MAKEINTRESOURCE(SID_SETTINGS_MENU)
#define STR_EXIT_MENU       MAKEINTRESOURCE(SID_EXIT_MENU)


#define STR_TOOLS_MENU      MAKEINTRESOURCE(SID_TOOLS_MENU)
#define STR_PATCHGUARD_MENU MAKEINTRESOURCE(SID_PATCHGUARD_MENU)


#define STR_HELP_MENU       MAKEINTRESOURCE(SID_HELP_MENU)
#define STR_CONTENTS_MENU   MAKEINTRESOURCE(SID_CONTENTS_MENU)
#define STR_LICENCES_MENU   MAKEINTRESOURCE(SID_LICENCES_MENU)
#define STR_ABOUT_MENU      MAKEINTRESOURCE(SID_ABOUT_MENU)


#define STR_PATCHGUARD      MAKEINTRESOURCE(SID_PATCHGUARD)
#define STR_PATCH           MAKEINTRESOURCE(SID_PATCH)
#define STR_RESTORE         MAKEINTRESOURCE(SID_RESTORE)
#define STR_PATCHED         MAKEINTRESOURCE(SID_PATCHED)
#define STR_NOTPATCHED      MAKEINTRESOURCE(SID_NOTPATCHED)
#define STR_ADVANCEDOPTIONS MAKEINTRESOURCE(SID_ADVANCEDOPTIONS)
#define STR_DEFAULTENTRYE   MAKEINTRESOURCE(SID_DEFAULTENTRYE)
#define STR_LOADERNAME      MAKEINTRESOURCE(SID_LOADERNAME)
#define STR_KERNELNAME      MAKEINTRESOURCE(SID_KERNELNAME)
#define STR_INCOMPATIBLEWVR MAKEINTRESOURCE(SID_INCOMPATIBLEWVR)
#define STR_INITFAIL        MAKEINTRESOURCE(SID_INITFAIL)
#define STR_INVALIDNAMES    MAKEINTRESOURCE(SID_INVALIDNAMES)
#define STR_FILESEXISTS     MAKEINTRESOURCE(SID_FILESEXISTS)
#define STR_PATCHFAIL       MAKEINTRESOURCE(SID_PATCHFAIL)
#define STR_RESTOREFAIL     MAKEINTRESOURCE(SID_RESTOREFAIL)
#define STR_ADV_WARNING     MAKEINTRESOURCE(SID_ADV_WARNING)


#define STR_ABOUT_IAPP      MAKEINTRESOURCE(SID_ABOUT_IAPP)
#define STR_ABOUT_DETAIL    MAKEINTRESOURCE(SID_ABOUT_DETAIL)


#define STR_IAPP_SETTINGS   MAKEINTRESOURCE(SID_IAPP_SETTINGS)
#define STR_IAPP_LANG       MAKEINTRESOURCE(SID_IAPP_LANG)
#define STR_IAPP_AUTOLANG   MAKEINTRESOURCE(SID_IAPP_AUTOLANG)
#define STR_IAPP_ENGLANG    MAKEINTRESOURCE(SID_IAPP_ENGLANG)
#define STR_IAPP_ESPLANG    MAKEINTRESOURCE(SID_IAPP_ESPLANG)
#define STR_IAPP_AUTOSTART  MAKEINTRESOURCE(SID_IAPP_AUTOSTART)
#define STR_IAPP_MINSTART   MAKEINTRESOURCE(SID_IAPP_MINSTART)
#define STR_IAPP_FLOGOUT    MAKEINTRESOURCE(SID_IAPP_FLOGOUT)
#define STR_IAPP_NEEDRESTART MAKEINTRESOURCE(SID_IAPP_NEEDRESTART)


#define STR_IAPP_STATUS     MAKEINTRESOURCE(SID_IAPP_STATUS)
#define STR_READYTOPLAY     MAKEINTRESOURCE(SID_READYTOPLAY)
#define STR_READYTOACTIVATE MAKEINTRESOURCE(SID_READYTOACTIVATE)
#define STR_READYTODOWNLOAD MAKEINTRESOURCE(SID_READYTODOWNLOAD)
#define STR_READYTOINSTALL  MAKEINTRESOURCE(SID_READYTOINSTALL)
#define STR_OUTDATEDVERSION MAKEINTRESOURCE(SID_OUTDATEDVERSION)
#define STR_FILESMISSING    MAKEINTRESOURCE(SID_FILESMISSING)
#define STR_ACTIVATING      MAKEINTRESOURCE(SID_ACTIVATING)
#define STR_DOWNLOADING     MAKEINTRESOURCE(SID_DOWNLOADING)
#define STR_RUNNING         MAKEINTRESOURCE(SID_RUNNING)
#define STR_NOLICENCES      MAKEINTRESOURCE(SID_NOLICENCES)


#define STR_APP_NAME        MAKEINTRESOURCE(SID_APP_NAME)
#define STR_APP_NAME_FULL   MAKEINTRESOURCE(SID_APP_NAME_FULL)
#define STR_WEB_URL         MAKEINTRESOURCE(SID_WEB_URL)
#define STR_APP_CREDITS     MAKEINTRESOURCE(SID_APP_CREDITS)


#define STR_INTERNAL_ERROR  MAKEINTRESOURCE(SID_INTERNAL_ERROR)
#define STR_NOINFORMATION   MAKEINTRESOURCE(SID_NOINFORMATION)
#define STR_CHECKINGUPDATES MAKEINTRESOURCE(SID_CHECKINGUPDATES)
#define STR_UPDATEAVAILABLE MAKEINTRESOURCE(SID_UPDATEAVAILABLE)
#define STR_MANUALUPDATE    MAKEINTRESOURCE(SID_MANUALUPDATE)
#define STR_NOUPDATES       MAKEINTRESOURCE(SID_NOUPDATES)
#define STR_UPDATING        MAKEINTRESOURCE(SID_UPDATING)
#define STR_CONNECTIONERROR MAKEINTRESOURCE(SID_CONNECTIONERROR)
#define STR_CONNECTIONLOST  MAKEINTRESOURCE(SID_CONNECTIONLOST)
#define STR_SYSTEMRESTART   MAKEINTRESOURCE(SID_SYSTEMRESTART)
#define STR_RESTARTREQUIRED MAKEINTRESOURCE(SID_RESTARTREQUIRED)


#define STR_LICTIME         MAKEINTRESOURCE(SID_LICTIME)
#define STR_PLAYDESC        MAKEINTRESOURCE(SID_PLAYDESC)
#define STR_ACTDESC         MAKEINTRESOURCE(SID_ACTDESC)
#define STR_DOWNDESC        MAKEINTRESOURCE(SID_DOWNDESC)
#define STR_UPDDESC         MAKEINTRESOURCE(SID_UPDDESC)
#define STR_MISSDESC        MAKEINTRESOURCE(SID_MISSDESC)
#define STR_NOLICDESC       MAKEINTRESOURCE(SID_NOLICDESC)
#define STR_NOAVADESC       MAKEINTRESOURCE(SID_NOAVADESC)


#define STR_SEMIPUBLICCHEATS  MAKEINTRESOURCE(SID_SEMIPUBLICCHEATS)
#define STR_PRIVATECHEATS   MAKEINTRESOURCE(SID_PRIVATECHEATS)
#define STR_PUBLICCHEATS    MAKEINTRESOURCE(SID_PUBLICCHEATS)
