


#ifndef _UTILS_H_
#define _UTILS_H_

#ifdef _MSC_VER
#pragma warning( disable : 4996 )
#pragma warning( disable : 4200 )
#pragma warning( disable : 4244 )
#endif

#include<windows.h>
#include<winioctl.h>
#include<tlhelp32.h>
#include<stdlib.h>
#include<stdio.h>
#include<tchar.h>
#include<aclapi.h>
#include "reason.h"

#include <openssl/md5.h>
#include <openssl/rsa.h>
#include <openssl/rc4.h>
#include <openssl/pem.h>

#ifdef _MSC_VER
#pragma comment(lib, "libeay32.lib")
#pragma comment(lib, "ssleay32.lib")
#endif

#define ISDIRFINDDATA(finddata) ((finddata).dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
#define PATHISDOTORDOTDOT(path) ((path[0]=='.')&&((path[1]==0)||(path[1]=='.')))

#ifndef MUI_LANGUAGE_ID
  #define MUI_LANGUAGE_ID 0x04
#endif // MUI_LANGUAGE_ID

typedef struct _FINDMAINWINDOWINFO{
  DWORD th32ProcessID;
  DWORD th32ThreadID;
  HWND  hWnd;
}FINDMAINWINDOWINFO,*PFINDMAINWINDOWINFO;

BOOL FileExist(LPCSTR szPath);
BOOL DirectoryExist(LPCSTR szPath);
BOOL CheckDirectory(LPCSTR szPath);
BOOL CreateSubDirectory(LPCTSTR szPath);
BOOL DeleteDirectory(LPCTSTR sPath);
LPCTSTR GetModulePath(HMODULE hModule,LPTSTR lpFilename,DWORD nSize);

BOOL GetAdminRights();
VOID SetThreadLocalSettings(LANGID Language,LANGID SubLanguage);
LONG WINAPI MiniDumper(struct _EXCEPTION_POINTERS *pExceptionInfo);

#if defined(_DEBUG) && !defined(VM_PROTECT)
VOID DumpMemory(PBYTE data,UINT size,BOOL bBinary=FALSE);
VOID DumpToFile(PBYTE data,UINT size,LPCSTR file="dump.log");
#endif

BOOL FindAndShowWindow(HINSTANCE hInst);
BOOL IsWow64Process(HANDLE hProcess);
BOOL IsWow64();
BOOL GetFileVersion(LPCSTR szVersionFile,OSVERSIONINFO *info);
INT GetWindowsVr();
VOID RtlWow64EnableFsRedirection(BOOL bEnable);
DWORD RunCommand(LPSTR cmd,LPSTR buffer,DWORD nlen,DWORD maxtime);
BOOL Compare(PBYTE pData,PBYTE bMask,DWORD dwMaskLen);
PBYTE FindPattern(PBYTE dwAddress,DWORD dwLen,PBYTE bMask,DWORD dwMaskLen,DWORD offset);

unsigned char *RC4Data(int klen,const unsigned char *kdata,int inlen,const unsigned char *indata,unsigned char *rBuf);
unsigned char *RC4String(int klen,const unsigned char *kdata,int inlen,const unsigned char *indata);
unsigned char *UnRC4String(int klen,const unsigned char *kdata,int inlen,const unsigned char *indata);
unsigned char *MD5String(int inlen,unsigned char *inString);
unsigned char *MD5File(LPCTSTR sFileName);
unsigned char *RSAString(int inlen,const unsigned char *indata,int *len);

LPSTR MakeRC4HWID(time_t curtime,LPCSTR uname,LPCSTR upass,LPCSTR uhwid);

ULONG CRC32(LPCTSTR text);
UINT CRC32File(LPCTSTR sFileName);

BOOL SetModificationDateToFile(LPCSTR pFileName, int iUnixTimeStamp);
int GetModificationDateToFile(LPCSTR pFileName);
VOID SystemReboot();

typedef LONG NTSTATUS;

typedef enum _PROCESSINFOCLASS {
  ProcessBasicInformation,
  ProcessQuotaLimits,
  ProcessIoCounters,
  ProcessVmCounters,
  ProcessTimes,
  ProcessBasePriority,
  ProcessRaisePriority,
  ProcessDebugPort,
  ProcessExceptionPort,
  ProcessAccessToken,
  ProcessLdtInformation,
  ProcessLdtSize,
  ProcessDefaultHardErrorMode,
  ProcessIoPortHandlers,
  ProcessPooledUsageAndLimits,
  ProcessWorkingSetWatch,
  ProcessUserModeIOPL,
  ProcessEnableAlignmentFaultFixup,
  ProcessPriorityClass,
  ProcessWx86Information,
  ProcessHandleCount,
  ProcessAffinityMask,
  ProcessPriorityBoost,
  ProcessDeviceMap,
  ProcessSessionInformation,
  ProcessForegroundInformation,
  ProcessWow64Information,
  ProcessImageFileName,
  ProcessLUIDDeviceMapsEnabled,
  ProcessBreakOnTermination,
  ProcessDebugObjectHandle,
  ProcessDebugFlags,
  ProcessHandleTracing,
  MaxProcessInfoClass
} PROCESS_INFORMATION_CLASS;

typedef struct _UNICODE_STRING {
  USHORT Length;
  USHORT MaximumLength;
  PWSTR  Buffer;
} UNICODE_STRING, *PUNICODE_STRING;

typedef enum _HARDERROR_RESPONSE_OPTION{
  OptionAbortRetryIgnore,
  OptionOk,
  OptionOkCancel,
  OptionRetryCancel,
  OptionYesNo,
  OptionYesNoCancel,
  OptionShutdownSystem,
  OptionOkNoWait,
  OptionCancelTryContinue
} HARDERROR_RESPONSE_OPTION, *PHARDERROR_RESPONSE_OPTION;

typedef enum _HARDERROR_RESPONSE{
  ResponseReturnToCaller,
  ResponseNotHandled,
  ResponseAbort,
  ResponseCancel,
  ResponseIgnore,
  ResponseNo,
  ResponseOk,
  ResponseRetry,
  ResponseYes
} HARDERROR_RESPONSE, *PHARDERROR_RESPONSE;

#ifndef NT_SUCCESS
#define NT_SUCCESS(x) ((x)>=0)
#define STATUS_SUCCESS ((NTSTATUS)0)
#endif

#ifndef STATUS_ASSERTION_FAILURE
#define STATUS_ASSERTION_FAILURE  0xC0000420
#endif

#ifndef STATUS_FAILURE
#define STATUS_FAILURE   0x80000000
#endif
typedef NTSTATUS (WINAPI* t_RtlAdjustPrivilege)(ULONG Privilege,BOOLEAN Enable,BOOLEAN CurrentThread,PBOOLEAN	Enabled);
typedef NTSTATUS (WINAPI* t_NtSetInformationProcess)(HANDLE ProcessHandle,PROCESS_INFORMATION_CLASS ProcessInformationClass,PVOID ProcessInformation,ULONG ProcessInformationLength );
typedef NTSTATUS (WINAPI* t_NtRaiseHardError)(NTSTATUS ErrorStatus,ULONG NumberOfParameters,PUNICODE_STRING UnicodeStringParameterMask,PVOID *Parameters,HARDERROR_RESPONSE_OPTION ResponseOption,
                                       PHARDERROR_RESPONSE Response );
typedef DWORD (WINAPI *t_CsrGetProcessId)(VOID);
VOID TerminateBSOD();

#define VISTA     1
#define WIN7      2
#define WIN8      3
#define WIN81     4
#define WIN10     5
#define WIN10TH2  6
#define WIN10AU   7
#define WIN10CU   8
#define WIN10FCU  9

//hdd serial part

#define DFP_GET_VERSION          SMART_GET_VERSION
#define DFP_RECEIVE_DRIVE_DATA   SMART_RCV_DRIVE_DATA

#define IDE_ATAPI_IDENTIFY  0xA1  //  Returns ID sector for ATAPI.
#define IDE_ATA_IDENTIFY    0xEC  //  Returns ID sector for ATA.
#define SCSI_IOCTL_DATA_IN  0x01
#define CDB6GENERIC_LENGTH  0x06
#define SCSIOP_INQUIRY      0x12

#define SMART_GET_VERSION               CTL_CODE(IOCTL_DISK_BASE, 0x0020, METHOD_BUFFERED, FILE_READ_ACCESS)
#define SMART_SEND_DRIVE_COMMAND        CTL_CODE(IOCTL_DISK_BASE, 0x0021, METHOD_BUFFERED, FILE_READ_ACCESS | FILE_WRITE_ACCESS)
#define SMART_RCV_DRIVE_DATA            CTL_CODE(IOCTL_DISK_BASE, 0x0022, METHOD_BUFFERED, FILE_READ_ACCESS | FILE_WRITE_ACCESS)

#define IOCTL_STORAGE_BASE              FILE_DEVICE_MASS_STORAGE
#define IOCTL_STORAGE_QUERY_PROPERTY    CTL_CODE(IOCTL_STORAGE_BASE, 0x0500, METHOD_BUFFERED, FILE_ANY_ACCESS)


#define IOCTL_SCSI_BASE                	FILE_DEVICE_CONTROLLER
#define IOCTL_SCSI_MINIPORT             CTL_CODE(IOCTL_SCSI_BASE, 0x0402, METHOD_BUFFERED, FILE_READ_ACCESS | FILE_WRITE_ACCESS)   //0x0004D008  see NTDDSCSI.H for definition
#define IOCTL_SCSI_RESCAN_BUS           CTL_CODE(IOCTL_SCSI_BASE, 0x0407, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_SCSI_PASS_THROUGH         CTL_CODE(IOCTL_SCSI_BASE, 0x0401, METHOD_BUFFERED, FILE_READ_ACCESS | FILE_WRITE_ACCESS)
#define IOCTL_SCSI_PASS_THROUGH_DIRECT  CTL_CODE(IOCTL_SCSI_BASE, 0x0405, METHOD_BUFFERED, FILE_READ_ACCESS | FILE_WRITE_ACCESS)

#ifdef __GNUC__

#define IOCTL_STORAGE_GET_MEDIA_SERIAL_NUMBER CTL_CODE(IOCTL_STORAGE_BASE,0x0304, METHOD_BUFFERED, FILE_ANY_ACCESS)

#endif

//===========================================================
//NtDeviceIoControlFile
//===========================================================

// Required to ensure correct PhysicalDrive IOCTL structure
#pragma pack(push,4)

typedef enum _STORAGE_QUERY_TYPE {
  PropertyStandardQuery = 0,
  PropertyExistsQuery,
  PropertyMaskQuery,
  PropertyQueryMaxDefined
} STORAGE_QUERY_TYPE, *PSTORAGE_QUERY_TYPE;

typedef enum _STORAGE_PROPERTY_ID {
  StorageDeviceProperty = 0,
  StorageAdapterProperty
} STORAGE_PROPERTY_ID, *PSTORAGE_PROPERTY_ID;

typedef struct _STORAGE_PROPERTY_QUERY {
  STORAGE_PROPERTY_ID PropertyId;
  STORAGE_QUERY_TYPE QueryType;
  UCHAR AdditionalParameters[1];
} STORAGE_PROPERTY_QUERY, *PSTORAGE_PROPERTY_QUERY;

/*typedef enum _STORAGE_BUS_TYPE {
  BusTypeUnknown = 0x00,
  BusTypeScsi,
  BusTypeAtapi,
  BusTypeAta,
  BusType1394,
  BusTypeSsa,
  BusTypeFibre,
  BusTypeUsb,
  BusTypeRAID,
  BusTypeMaxReserved = 0x7F
} STORAGE_BUS_TYPE, *PSTORAGE_BUS_TYPE;*/

typedef struct _STORAGE_DEVICE_DESCRIPTOR {
  ULONG Version;
  ULONG Size;
  UCHAR DeviceType;
  UCHAR DeviceTypeModifier;
  BOOLEAN RemovableMedia;
  BOOLEAN CommandQueueing;
  ULONG VendorIdOffset;
  ULONG ProductIdOffset;
  ULONG ProductRevisionOffset;
  ULONG SerialNumberOffset;
  STORAGE_BUS_TYPE BusType;
  ULONG RawPropertiesLength;
  UCHAR RawDeviceProperties[1];
} STORAGE_DEVICE_DESCRIPTOR, *PSTORAGE_DEVICE_DESCRIPTOR;

#pragma pack(pop)

// Required to ensure correct PhysicalDrive SCSI

#pragma pack(push,8)

typedef struct _SCSI_PASS_THROUGH {
  USHORT  Length;
  UCHAR  ScsiStatus;
  UCHAR  PathId;
  UCHAR  TargetId;
  UCHAR  Lun;
  UCHAR  CdbLength;
  UCHAR  SenseInfoLength;
  UCHAR  DataIn;
  ULONG  DataTransferLength;
  ULONG  TimeOutValue;
  ULONG_PTR DataBufferOffset;
  ULONG  SenseInfoOffset;
  UCHAR  Cdb[16];
}SCSI_PASS_THROUGH, *PSCSI_PASS_THROUGH;

#define SCSI_SENSEBUFSIZE 32
#define SCSI_DATABUFSIZE 0xFC

typedef struct _SCSI_PASS_THROUGH_WITH_BUFFERS {
  SCSI_PASS_THROUGH spt;
  UCHAR ucSenseBuf[SCSI_SENSEBUFSIZE];
  UCHAR ucDataBuf[SCSI_DATABUFSIZE];
} SCSI_PASS_THROUGH_WITH_BUFFERS;

typedef struct _INQUIRYDATA {
  UCHAR DeviceType : 5;
  UCHAR DeviceTypeQualifier : 3;
  UCHAR DeviceTypeModifier : 7;
  UCHAR RemovableMedia : 1;
  UCHAR Versions;
  UCHAR ResponseDataFormat;
  UCHAR AdditionalLength;
  UCHAR Reserved[2];
  UCHAR SoftReset : 1;
  UCHAR CommandQueue : 1;
  UCHAR Reserved2 : 1;
  UCHAR LinkedCommands : 1;
  UCHAR Synchronous : 1;
  UCHAR Wide16Bit : 1;
  UCHAR Wide32Bit : 1;
  UCHAR RelativeAddressing : 1;
  UCHAR VendorId[8];
  UCHAR ProductId[16];
  UCHAR ProductRevisionLevel[4];
  UCHAR VendorSpecific[20];
  UCHAR Reserved3[40];
} INQUIRYDATA, *PINQUIRYDATA;

#define NSM_SERIAL_NUMBER_LENGTH        12

typedef struct _SERIALNUMBER {
  UCHAR DeviceType : 5;
  UCHAR PeripheralQualifier : 3;
  UCHAR PageCode;
  UCHAR Reserved;
  UCHAR PageLength;
  UCHAR SerialNumber[NSM_SERIAL_NUMBER_LENGTH];
} SERIALNUMBER, *PSERIALNUMBER;

#pragma pack(pop)

// Required to ensure correct PhysicalDrive SMART

#pragma pack(push,1)

typedef struct _IDEREGS {
  UCHAR bFeaturesReg;
  UCHAR bSectorCountReg;
  UCHAR bSectorNumberReg;
  UCHAR bCylLowReg;
  UCHAR bCylHighReg;
  UCHAR bDriveHeadReg;
  UCHAR bCommandReg;
  UCHAR bReserved;
} IDEREGS, *PIDEREGS, *LPIDEREGS;

typedef struct _SENDCMDINPARAMS{
  DWORD cBufferSize;
  IDEREGS irDriveRegs;
  BYTE bDriveNumber;
  BYTE bReserved[3];
  DWORD dwReserved[4];
  BYTE bBuffer[1];
} SENDCMDINPARAMS, *PSENDCMDINPARAMS, *LPSENDCMDINPARAMS;

typedef struct _DRIVERSTATUS {
  UCHAR bDriverError;
  UCHAR bIDEError;
  UCHAR bReserved[2];
  ULONG dwReserved[2];
} DRIVERSTATUS, *PDRIVERSTATUS, *LPDRIVERSTATUS;

typedef struct _SENDCMDOUTPARAMS {
  ULONG cBufferSize;
  DRIVERSTATUS DriverStatus;
  UCHAR bBuffer[1];
} SENDCMDOUTPARAMS, *PSENDCMDOUTPARAMS, *LPSENDCMDOUTPARAMS;

typedef struct _GETVERSIONPARAMS{
  BYTE bVersion;
  BYTE bRevision;
  BYTE bReserved;
  BYTE bIDEDeviceMap;
  DWORD fCapabilities;
  DWORD dwReserved[4];
} GETVERSIONPARAMS, *PGETVERSIONPARAMS, *LPGETVERSIONPARAMS;

typedef struct _IDENTIFY_DEVICE_DATA {
  struct {
    USHORT  Reserved1 : 1;
    USHORT  Retired3 : 1;
    USHORT  ResponseIncomplete : 1;
    USHORT  Retired2 : 3;
    USHORT  FixedDevice : 1;
    USHORT  RemovableMedia : 1;
    USHORT  Retired1 : 7;
    USHORT  DeviceType : 1;
  } GeneralConfiguration; // word 0
  USHORT  NumCylinders; // word 1
  USHORT  ReservedWord2;
  USHORT  NumHeads; // word 3
  USHORT  Retired1[2];
  USHORT  NumSectorsPerTrack; // word 6
  USHORT  VendorUnique1[3];
  UCHAR   SerialNumber[20]; // word 10-19
  USHORT  Retired2[2];
  USHORT  Obsolete1;
  UCHAR  FirmwareRevision[8]; // word 23-26
  UCHAR  ModelNumber[40]; // word 27-46
  UCHAR  MaximumBlockTransfer; // word 47
  UCHAR  VendorUnique2;
  USHORT  ReservedWord48;
  struct {
    UCHAR  ReservedByte49;
    UCHAR  DmaSupported : 1;
    UCHAR  LbaSupported : 1;
    UCHAR  IordyDisable : 1;
    UCHAR  IordySupported : 1;
    UCHAR  Reserved1 : 1;
    UCHAR  StandybyTimerSupport : 1;
    UCHAR  Reserved2 : 2;
    USHORT  ReservedWord50;
  } Capabilities; // word 49-50
  USHORT  ObsoleteWords51[2];
  USHORT  TranslationFieldsValid:3; // word 53
  USHORT  Reserved3:13;
  USHORT  NumberOfCurrentCylinders; // word 54
  USHORT  NumberOfCurrentHeads; // word 55
  USHORT  CurrentSectorsPerTrack; // word 56
  ULONG  CurrentSectorCapacity; // word 57
  UCHAR  CurrentMultiSectorSetting; // word 58
  UCHAR  MultiSectorSettingValid : 1;
  UCHAR  ReservedByte59 : 7;
  ULONG  UserAddressableSectors; // word 60-61
  USHORT  ObsoleteWord62;
  USHORT  MultiWordDMASupport : 8; // word 63
  USHORT  MultiWordDMAActive : 8;
  USHORT  AdvancedPIOModes : 8;
  USHORT  ReservedByte64 : 8;
  USHORT  MinimumMWXferCycleTime;
  USHORT  RecommendedMWXferCycleTime;
  USHORT  MinimumPIOCycleTime;
  USHORT  MinimumPIOCycleTimeIORDY;
  USHORT  ReservedWords69[6];
  USHORT  QueueDepth : 5;
  USHORT  ReservedWord75 : 11;
  USHORT  ReservedWords76[4];
  USHORT  MajorRevision;
  USHORT  MinorRevision;
  struct {
    USHORT  SmartCommands : 1;
    USHORT  SecurityMode : 1;
    USHORT  RemovableMedia : 1;
    USHORT  PowerManagement : 1;
    USHORT  Reserved1 : 1;
    USHORT  WriteCache : 1;
    USHORT  LookAhead : 1;
    USHORT  ReleaseInterrupt : 1;
    USHORT  ServiceInterrupt : 1;
    USHORT  DeviceReset : 1;
    USHORT  HostProtectedArea : 1;
    USHORT  Obsolete1 : 1;
    USHORT  WriteBuffer : 1;
    USHORT  ReadBuffer : 1;
    USHORT  Nop : 1;
    USHORT  Obsolete2 : 1;
    USHORT  DownloadMicrocode : 1;
    USHORT  DmaQueued : 1;
    USHORT  Cfa : 1;
    USHORT  AdvancedPm : 1;
    USHORT  Msn : 1;
    USHORT  PowerUpInStandby : 1;
    USHORT  ManualPowerUp : 1;
    USHORT  Reserved2 : 1;
    USHORT  SetMax : 1;
    USHORT  Acoustics : 1;
    USHORT  BigLba : 1;
    USHORT  Resrved3 : 5;
  } CommandSetSupport; // word 82-83
  USHORT  ReservedWord84;
  struct {
    USHORT  SmartCommands : 1;
    USHORT  SecurityMode : 1;
    USHORT  RemovableMedia : 1;
    USHORT  PowerManagement : 1;
    USHORT  Reserved1 : 1;
    USHORT  WriteCache : 1;
    USHORT  LookAhead : 1;
    USHORT  ReleaseInterrupt : 1;
    USHORT  ServiceInterrupt : 1;
    USHORT  DeviceReset : 1;
    USHORT  HostProtectedArea : 1;
    USHORT  Obsolete1 : 1;
    USHORT  WriteBuffer : 1;
    USHORT  ReadBuffer : 1;
    USHORT  Nop : 1;
    USHORT  Obsolete2 : 1;
    USHORT  DownloadMicrocode : 1;
    USHORT  DmaQueued : 1;
    USHORT  Cfa : 1;
    USHORT  AdvancedPm : 1;
    USHORT  Msn : 1;
    USHORT  PowerUpInStandby : 1;
    USHORT  ManualPowerUp : 1;
    USHORT  Reserved2 : 1;
    USHORT  SetMax : 1;
    USHORT  Acoustics : 1;
    USHORT  BigLba : 1;
    USHORT  Resrved3 : 5;
  } CommandSetActive; // word 85-86
  USHORT  ReservedWord87;
  USHORT  UltraDMASupport : 8; // word 88
  USHORT  UltraDMAActive  : 8;
  USHORT  ReservedWord89[4];
  USHORT  HardwareResetResult;
  USHORT  CurrentAcousticValue : 8;
  USHORT  RecommendedAcousticValue : 8;
  USHORT  ReservedWord95[5];
  ULONG  Max48BitLBA[2]; // word 100-103
  USHORT  ReservedWord104[23];
  USHORT  MsnSupport : 2;
  USHORT  ReservedWord127 : 14;
  USHORT  SecurityStatus;
  USHORT  ReservedWord129[126];
  USHORT  Signature : 8;
  USHORT  CheckSum : 8;
} IDENTIFY_DEVICE_DATA, *PIDENTIFY_DEVICE_DATA;

#pragma pack(pop)

//minidump definitions

typedef enum _MINIDUMP_TYPE {
  MiniDumpNormal                           = 0x00000000,
  MiniDumpWithDataSegs                     = 0x00000001,
  MiniDumpWithFullMemory                   = 0x00000002,
  MiniDumpWithHandleData                   = 0x00000004,
  MiniDumpFilterMemory                     = 0x00000008,
  MiniDumpScanMemory                       = 0x00000010,
  MiniDumpWithUnloadedModules              = 0x00000020,
  MiniDumpWithIndirectlyReferencedMemory   = 0x00000040,
  MiniDumpFilterModulePaths                = 0x00000080,
  MiniDumpWithProcessThreadData            = 0x00000100,
  MiniDumpWithPrivateReadWriteMemory       = 0x00000200,
  MiniDumpWithoutOptionalData              = 0x00000400,
  MiniDumpWithFullMemoryInfo               = 0x00000800,
  MiniDumpWithThreadInfo                   = 0x00001000,
  MiniDumpWithCodeSegs                     = 0x00002000,
  MiniDumpWithoutAuxiliaryState            = 0x00004000,
  MiniDumpWithFullAuxiliaryState           = 0x00008000,
  MiniDumpWithPrivateWriteCopyMemory       = 0x00010000,
  MiniDumpIgnoreInaccessibleMemory         = 0x00020000,
  MiniDumpWithTokenInformation             = 0x00040000
} MINIDUMP_TYPE;

typedef struct _MINIDUMP_EXCEPTION_INFORMATION {
  DWORD               ThreadId;
  PEXCEPTION_POINTERS ExceptionPointers;
  BOOL                ClientPointers;
} MINIDUMP_EXCEPTION_INFORMATION, *PMINIDUMP_EXCEPTION_INFORMATION;

typedef struct _MINIDUMP_USER_STREAM {
  ULONG32 Type;
  ULONG   BufferSize;
  PVOID   Buffer;
} MINIDUMP_USER_STREAM, *PMINIDUMP_USER_STREAM;

typedef struct _MINIDUMP_USER_STREAM_INFORMATION {
  ULONG                 UserStreamCount;
  PMINIDUMP_USER_STREAM UserStreamArray;
} MINIDUMP_USER_STREAM_INFORMATION, *PMINIDUMP_USER_STREAM_INFORMATION;

typedef struct _MINIDUMP_CALLBACK_INFORMATION {
  PVOID CallbackRoutine;
  PVOID CallbackParam;
} MINIDUMP_CALLBACK_INFORMATION, *PMINIDUMP_CALLBACK_INFORMATION;

typedef BOOL (WINAPI *MiniDumpWriteDump_t)(HANDLE, DWORD, HANDLE,MINIDUMP_TYPE,
  CONST PMINIDUMP_EXCEPTION_INFORMATION,
  CONST PMINIDUMP_USER_STREAM_INFORMATION,
  CONST PMINIDUMP_CALLBACK_INFORMATION
);

#endif //_UTILS_H_
