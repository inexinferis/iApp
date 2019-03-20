#ifndef _XUNDEF_H_
#define _XUNDEF_H_

#include "xobject.h"
#include "docobj.h"

typedef struct tagSIZES { // pts
  SHORT cx;
  SHORT cy;
}SIZES;

#ifdef GetWindowTask
#undef GetWindowTask
inline HTASK WINAPI GetWindowTask(HWND hWnd){
  return (HTASK)(DWORD_PTR)::GetWindowThreadProcessId(hWnd,NULL);
}
#endif


#ifdef GetNextWindow
#undef GetNextWindow
inline HWND WINAPI GetNextWindow(HWND hWnd,UINT nDirection){
  return ::GetWindow(hWnd,nDirection);
}
#endif

#ifdef DrawState
#undef DrawState
inline BOOL WINAPI DrawState(HDC hdc,HBRUSH hbr,DRAWSTATEPROC lpOutputFunc,LPARAM lData,WPARAM wData,int x,int y,int cx,int cy,UINT fuFlags){
 #ifdef UNICODE
  return ::DrawStateW(hdc,hbr,lpOutputFunc,lData,wData,x,y,cx,cy,fuFlags);
 #else
  return ::DrawStateA(hdc,hbr,lpOutputFunc,lData,wData,x,y,cx,cy,fuFlags);
 #endif
}
#endif

//STL undef max & min...
#ifndef __max
  #define __max(a,b)  (((a) > (b)) ? (a) : (b))
#endif
#ifndef __min
  #define __min(a,b)  (((a) < (b)) ? (a) : (b))
#endif

#ifndef XDUToXPix
  #define XDUToXPix(baseunits,x) ::MulDiv(x,LOWORD(baseunits),4)
#endif
#ifndef YDUToXPix
  #define YDUToYPix(baseunits,y) ::MulDiv(y,HIWORD(baseunits),8)
#endif

#ifndef XPixToXDU
  #define XPixToXDU(baseunits,x) ::MulDiv(x,4,LOWORD(baseunits))
#endif
#ifndef YPixToYDU
  #define YPixToYDU(baseunits,y) ::MulDiv(y,8,HIWORD(baseunits))
#endif

#ifndef NextWordBoundary
  #define NextWordBoundary(p)     ((PBYTE)(p) + ((ULONG_PTR)(p) & 1))
#endif

#ifndef NextDWordBoundary
  #define NextDWordBoundary(p)    ((PBYTE)(p) + ((ULONG_PTR)(-(LONG_PTR)(p)) & 3))
#endif

#ifndef DIALOG_CLASSA
  #define DIALOG_CLASSA	"#32770"
#endif

#ifndef DIALOG_CLASSW
  #define DIALOG_CLASSW	L"#32770"
#endif

#ifndef DIALOG_CLASS
  #ifdef UNICODE
    #define DIALOG_CLASS DIALOG_CLASSW
  #else
    #define DIALOG_CLASS DIALOG_CLASSA
  #endif
#endif

#ifndef IS_ATOM
  #define IS_ATOM(x) (((ULONG_PTR)(x) > 0x0) && ((ULONG_PTR)(x) < 0x10000))
#endif

#ifndef LVM_SETEXTENDEDLISTVIEWSTYLE
  #define LVM_SETEXTENDEDLISTVIEWSTYLE (LVM_FIRST + 54)
#endif

#ifndef LVM_GETEXTENDEDLISTVIEWSTYLE
  #define LVM_GETEXTENDEDLISTVIEWSTYLE (LVM_FIRST + 55)
#endif

#ifndef EN_ALIGN_LTR_EC
  #define EN_ALIGN_LTR_EC 0x0700
#endif

#ifndef EN_ALIGN_RTL_EC
  #define EN_ALIGN_RTL_EC 0x0701
#endif

#ifndef HDSIL_NORMAL
  #define HDSIL_NORMAL 0
#endif

#ifndef HDSIL_STATE
  #define HDSIL_STATE 1
#endif

#ifndef SS_EDITCONTROL
  #define SS_EDITCONTROL 0x2000L
#endif

#ifndef IDC_STATIC
  #define IDC_STATIC    (UINT)-1
#endif

#ifndef GET_X_LPARAM
  #define GET_X_LPARAM(lp)   ((int)(short)LOWORD(lp))
#endif

#ifndef GET_Y_LPARAM
  #define GET_Y_LPARAM(lp)   ((int)(short)HIWORD(lp))
#endif

#ifndef MASKENUM
  #define MASKENUM(a,b) ((a&0x1F)==b)
#endif

#if (_WIN32_WINNT >= 0x501)
  #define BUTTON_IMAGELIST_ALIGN_LEFT 0
  #define BUTTON_IMAGELIST_ALIGN_RIGHT 1
  #define BUTTON_IMAGELIST_ALIGN_TOP 2
  #define BUTTON_IMAGELIST_ALIGN_BOTTOM 3
  #define BUTTON_IMAGELIST_ALIGN_CENTER 4

  #ifndef TVGN_NEXTSELECTED
  #define TVGN_NEXTSELECTED 0xb
  #endif // TVGN_NEXTSELECTED

  #ifndef TVM_MAPACCIDTOHTREEITEM
  #define TVM_MAPACCIDTOHTREEITEM (TV_FIRST+42)
  #endif // TVM_MAPACCIDTOHTREEITEM

  #ifndef TVM_MAPHTREEITEMTOACCID
  #define TVM_MAPHTREEITEMTOACCID (TV_FIRST+43)
  #endif // TVM_MAPHTREEITEMTOACCID

  #ifndef EM_SETCUEBANNER

  typedef struct tagLVGROUP {
    UINT cbSize;
    UINT mask;
    LPWSTR pszHeader;
    int cchHeader;
    LPWSTR pszFooter;
    int cchFooter;
    int iGroupId;
    UINT stateMask;
    UINT state;
    UINT uAlign;
    #if NTDDI_VERSION >= 0x06000000
    #define LVGROUP_V5_SIZE CCSIZEOF_STRUCT (LVGROUP, uAlign)

    LPWSTR pszSubtitle;
    UINT cchSubtitle;
    LPWSTR pszTask;
    UINT cchTask;
    LPWSTR pszDescriptionTop;
    UINT cchDescriptionTop;
    LPWSTR pszDescriptionBottom;
    UINT cchDescriptionBottom;
    int iTitleImage;
    int iExtendedImage;
    int iFirstItem;
    UINT cItems;
    LPWSTR pszSubsetTitle;
    UINT cchSubsetTitle;
    #endif
  } LVGROUP,*PLVGROUP;

  typedef struct tagLVGROUPMETRICS {
    UINT cbSize;
    UINT mask;
    UINT Left;
    UINT Top;
    UINT Right;    UINT Bottom;
    COLORREF crLeft;
    COLORREF crTop;
    COLORREF crRight;
    COLORREF crBottom;
    COLORREF crHeader;
    COLORREF crFooter;
  } LVGROUPMETRICS,*PLVGROUPMETRICS;

  typedef int (CALLBACK *PFNLVGROUPCOMPARE)(int,int,void *);

  typedef struct tagLVINSERTGROUPSORTED {
    PFNLVGROUPCOMPARE pfnGroupCompare;
    void *pvData;
    LVGROUP lvGroup;
  } LVINSERTGROUPSORTED,*PLVINSERTGROUPSORTED;

  typedef struct {
    UINT cbSize;
    DWORD dwFlags;
    int iItem;
    DWORD dwReserved;
  } LVINSERTMARK,*LPLVINSERTMARK;

  typedef struct tagLVSETINFOTIP {
    UINT cbSize;
    DWORD dwFlags;
    LPWSTR pszText;
    int iItem;
    int iSubItem;
  } LVSETINFOTIP,*PLVSETINFOTIP;

  typedef struct tagLVTILEVIEWINFO {
    UINT cbSize;
    DWORD dwMask;
    DWORD dwFlags;
    SIZE sizeTile;
    int cLines;
    RECT rcLabelMargin;
  } LVTILEVIEWINFO,*PLVTILEVIEWINFO;

  typedef struct tagLVTILEINFO {
    UINT cbSize;
    int iItem;
    UINT cColumns;
    PUINT puColumns;
    #if NTDDI_VERSION >= 0x06000000
    int *piColFmt;
    #endif
  } LVTILEINFO,*PLVTILEINFO;

  typedef struct _tagEDITBALLOONTIP {
    DWORD cbStruct;
    LPCWSTR pszTitle;
    LPCWSTR pszText;
    INT ttiIcon;
  } EDITBALLOONTIP,*PEDITBALLOONTIP;

  typedef enum _undonameid {
    UID_UNKNOWN = 0,
    UID_TYPING = 1,
    UID_DELETE = 2,
    UID_DRAGDROP = 3,
    UID_CUT = 4,
    UID_PASTE = 5,
    UID_AUTOTABLE = 6
  } UNDONAMEID;

  typedef enum tagOLECONTF{
    OLECONTF_EMBEDDINGS = 1,
    OLECONTF_LINKS = 2,
    OLECONTF_OTHERS = 4,
    OLECONTF_OLNYUSER = 8,
    OLECONTF_ONLYIFRUNNING = 16
  }OLECONTF;

  typedef enum tagACTIVATEFLAGS {
    ACTIVATE_WINDOWLESS = 1
  }ACTIVATEFLAGS;

  #undef INTERFACE
  EXTERN_C const IID IID_IOleInPlaceObjectWindowless;
  #define INTERFACE IOleInPlaceObjectWindowless
  DECLARE_INTERFACE_(IOleInPlaceObjectWindowless,IOleInPlaceObject)
  {
    STDMETHOD(QueryInterface)(THIS_ REFIID,PVOID*) PURE;
    STDMETHOD_(ULONG,AddRef)(THIS) PURE;
    STDMETHOD_(ULONG,Release)(THIS) PURE;
    STDMETHOD(GetWindow)(THIS_ HWND*) PURE;
    STDMETHOD(ContextSensitiveHelp)(THIS_ BOOL) PURE;
    STDMETHOD(InPlaceDeactivate)(THIS) PURE;
    STDMETHOD(UIDeactivate)(THIS) PURE;
    STDMETHOD(SetObjectRects)(THIS_ LPCRECT,LPCRECT) PURE;
    STDMETHOD(ReactivateAndUndo)(THIS) PURE;
    STDMETHOD(OnWindowMessage)(THIS_ UINT msg,WPARAM wParam,LPARAM lParam,LRESULT *plResult) PURE;
    STDMETHOD(GetDropTarget)(THIS_ IDropTarget **ppDropTarget) PURE;
  };
  #undef INTERFACE

  #define ECM_FIRST 0x1500
  #define EM_SETCUEBANNER (ECM_FIRST+1)
  #define Edit_SetCueBannerText(hwnd,lpcwText) (WINBOOL)SNDMSG((hwnd),EM_SETCUEBANNER,0,(LPARAM)(lpcwText))
  #define EM_GETCUEBANNER (ECM_FIRST+2)
  #define Edit_GetCueBannerText(hwnd,lpwText,cchText) (WINBOOL)SNDMSG((hwnd),EM_GETCUEBANNER,(WPARAM)(lpwText),(LPARAM)(cchText))
  #define EM_SHOWBALLOONTIP (ECM_FIRST+3)
  #define Edit_ShowBalloonTip(hwnd,peditballoontip) (WINBOOL)SNDMSG((hwnd),EM_SHOWBALLOONTIP,0,(LPARAM)(peditballoontip))
  #define EM_HIDEBALLOONTIP (ECM_FIRST+4)
  #define Edit_HideBalloonTip(hwnd) (WINBOOL)SNDMSG((hwnd),EM_HIDEBALLOONTIP,0,0)
  #define PBS_MARQUEE 0x8
  #define PBM_SETMARQUEE (WM_USER+10)

  #define RB_GETBANDMARGINS (WM_USER+40)
  #define RB_SETWINDOWTHEME CCM_SETWINDOWTHEME
  #define SBM_GETSCROLLBARINFO 0x00EB

  #endif

  #ifndef BCM_FIRST

  typedef struct {
    HIMAGELIST himl;
    RECT       margin;
    UINT       uAlign;
  } BUTTON_IMAGELIST, *PBUTTON_IMAGELIST;

  #define BCM_FIRST   0x1600
  #define BCM_GETIDEALSIZE    (BCM_FIRST + 1)
  #define BCM_SETIMAGELIST    (BCM_FIRST + 2)
  #define BCM_GETIMAGELIST    (BCM_FIRST + 3)
  #define BCM_SETTEXTMARGIN   (BCM_FIRST + 4)
  #define BCM_GETTEXTMARGIN   (BCM_FIRST + 5)

  #endif // BCM_FIRST
#endif /* _WIN32_WINNT */

/*#define DCX_WINDOW           0x00000001L
#define DCX_CACHE            0x00000002L
#define DCX_NORESETATTRS     0x00000004L
#define DCX_CLIPCHILDREN     0x00000008L
#define DCX_CLIPSIBLINGS     0x00000010L
#define DCX_PARENTCLIP       0x00000020L
#define DCX_EXCLUDERGN       0x00000040L
#define DCX_INTERSECTRGN     0x00000080L
#define DCX_EXCLUDEUPDATE    0x00000100L
#define DCX_INTERSECTUPDATE  0x00000200L
#define DCX_LOCKWINDOWUPDATE 0x00000400L*/
#ifndef DCX_USESTYLE
  #define DCX_USESTYLE       0x00010000
#endif

#ifndef DCX_NODELETERGN
  #define DCX_NODELETERGN    0x00040000
#endif

#ifndef DCX_NORECOMPUTE
  #define DCX_NORECOMPUTE    0x00100000
#endif
/*#define DCX_VALIDATE         0x00200000L
#define DCX_DEFAULTCLIP      0x80000000L*/

#ifndef DT_HIDEPREFIX
  #define DT_HIDEPREFIX      0x00100000
#endif

/*typedef struct _FULLDLGTEMPLATE{
  DLGTEMPLATE mHeader;
  WORD  mMenu;//[];... Values -> [ 0x0000 (No Menu) | 0xFFFF 0xXXXX (Menu Ordinal) | L"Menu Name" ]
  WORD mClass;//[];... Values -> [ 0x0000 (Default) | 0xFFFF 0xXXXX (Class ATOM) | L"Class Name" ]
  WORD mTitle;//[];... Values -> [ 0x0000 (No Title) | L"Title" ]
}FULLDLGTEMPLATE,*LPFULLDLGTEMPLATE;

typedef struct _FULLDLGITEMTEMPLATE{
  DLGITEMTEMPLATE mHeader;
  WORD mClass;//[];... Values -> [ 0xFFFF 0xXXXX (Class ATOM) | L"Class Name" ]
  WORD mTitle;//[];... Values -> [ 0xFFFF 0xXXXX (Icon/Img Ordinal) | L"Title" ]
  WORD  mData;//[];... Values -> [ 0x0000 (no data) | 0xXXXX (Data Size) ]
}FULLDLGITEMTEMPLATE,*LPFULLDLGITEMTEMPLATE;*/

#ifndef WM_CTLCOLOR
  #define WM_CTLCOLOR 25
#endif

#ifndef CMDSETID_Forms3
  EXTERN_C const GUID CGID_MSHTML;
  #define CMDSETID_Forms3 CGID_MSHTML
#endif

#define VTS_NONE            NULL           // used for members with 0 params
#define VTS_I2              L"\x0002"      // a 'short'
#define VTS_I4              L"\x0003"      // a 'long'
#define VTS_R4              L"\x0004"      // a 'float'
#define VTS_R8              L"\x0005"      // a 'double'
#define VTS_CY              L"\x0006"      // a 'CY' or 'CY*'
#define VTS_DATE            L"\x0007"      // a 'DATE'
#define VTS_WBSTR           L"\x0008"      // an 'LPCOLESTR'
#define VTS_DISPATCH        L"\x0009"      // an 'IDispatch*'
#define VTS_SCODE           L"\x000A"      // an 'SCODE'
#define VTS_BOOL            L"\x000B"      // a 'BOOL'
#define VTS_VARIANT         L"\x000C"      // a 'const VARIANT&' or 'VARIANT*'
#define VTS_UNKNOWN         L"\x000D"      // an 'IUnknown*'
#ifdef _UNICODE
    #define VTS_BSTR            VTS_WBSTR  // an 'LPCOLESTR'
    #define VT_BSTRT            VT_BSTR
#else
    #define VTS_BSTR            L"\x000E"  // an 'LPCSTR'
    #define VT_BSTRA            14
    #define VT_BSTRT            VT_BSTRA
#endif
#define VTS_UI1             L"\x000F"      // a 'BYTE'

#define VTS_PI2             L"\x4002"      // a 'short*'
#define VTS_PI4             L"\x4003"      // a 'long*'
#define VTS_PR4             L"\x4004"      // a 'float*'
#define VTS_PR8             L"\x4005"      // a 'double*'
#define VTS_PCY             L"\x4006"      // a 'CY*'
#define VTS_PDATE           L"\x4007"      // a 'DATE*'
#define VTS_PBSTR           L"\x4008"      // a 'BSTR*'
#define VTS_PDISPATCH       L"\x4009"      // an 'IDispatch**'
#define VTS_PSCODE          L"\x400A"      // an 'SCODE*'
#define VTS_PBOOL           L"\x400B"      // a 'VARIANT_BOOL*'
#define VTS_PVARIANT        L"\x400C"      // a 'VARIANT*'
#define VTS_PUNKNOWN        L"\x400D"      // an 'IUnknown**'
#define VTS_PUI1            L"\x400F"      // a 'BYTE*'

//#define VT_BYREF         0x4000
#define VT_MARKER          0xFFFF

#ifndef V_INT	// missing in MinGW (as of 07.02.2004)
#define	V_INT(x) V_UNION(x, intVal)
#endif

#ifndef V_UINT	// missing in MinGW (as of 07.02.2004)
#define V_UINT(X) V_UNION(X, uintVal)
#endif

#ifndef IMPLTYPEFLAG_MASK
#define IMPLTYPEFLAG_MASK (IMPLTYPEFLAG_FDEFAULT|IMPLTYPEFLAG_FSOURCE|IMPLTYPEFLAG_FRESTRICTED)
#endif

#ifndef IMPLTYPEFLAG_DEFAULTSOURCE
#define IMPLTYPEFLAG_DEFAULTSOURCE (IMPLTYPEFLAG_FDEFAULT|IMPLTYPEFLAG_FSOURCE)
#endif

#ifndef XDEFINE_GUID
#define XDEFINE_GUID(n,a,b,c,d,e,f,g,h,i,j,k) const GUID n = {a,b,c,{d,e,f,g,h,i,j,k}}
#endif

#ifdef __GNUC__

#ifndef GCL_HICONSM
#define GCL_HICONSM (-34)
#endif // GCL_HICONSM

#ifndef GCL_HICON
#define GCL_HICON   (-14)
#endif // GCL_HICON

#endif

#ifdef _MSC_VER
  //#pragma pointers_to_members( full_generality )
  #pragma warning( disable : 4996 )
  #pragma warning( disable : 4521 )
  #pragma warning( disable : 4522 )
	#pragma warning( disable : 4355 )
	#pragma warning( disable : 4158 )
/*
#pragma comment(lib, "oledlg.lib")
#pragma comment(lib, "urlmon.lib")
*/
#endif

typedef struct _DOCHOSTUIINFO{
  ULONG cbSize;
  DWORD dwFlags;
  DWORD dwDoubleClick;
  OLECHAR *pchHostCss;
  OLECHAR *pchHostNS;
}DOCHOSTUIINFO;

typedef interface IOleInPlaceActiveObject *LPOLEINPLACEACTIVEOBJECT;
typedef interface IOleCommandTarget *LPOLECOMMANDTARGET;

#undef INTERFACE
EXTERN_C const IID IID_IDocHostUIHandler;
#define INTERFACE IDocHostUIHandler
DECLARE_INTERFACE_(IDocHostUIHandler,IUnknown)
{
	STDMETHOD(QueryInterface)(THIS_ REFIID,PVOID*) PURE;
	STDMETHOD_(ULONG,AddRef)(THIS) PURE;
	STDMETHOD_(ULONG,Release)(THIS) PURE;
	STDMETHOD(ShowContextMenu)(DWORD, LPPOINT, LPUNKNOWN, LPDISPATCH) PURE;
  STDMETHOD(GetHostInfo)(DOCHOSTUIINFO*) PURE;
  STDMETHOD(ShowUI)(DWORD,LPOLEINPLACEACTIVEOBJECT,LPOLECOMMANDTARGET, LPOLEINPLACEFRAME, LPOLEINPLACEUIWINDOW) PURE;
  STDMETHOD(HideUI)(void) PURE;
  STDMETHOD(UpdateUI)(void) PURE;
  STDMETHOD(EnableModeless)(BOOL) PURE;
  STDMETHOD(OnDocWindowActivate)(BOOL) PURE;
  STDMETHOD(OnFrameWindowActivate)(BOOL) PURE;
  STDMETHOD(ResizeBorder)(LPCRECT, LPOLEINPLACEUIWINDOW, BOOL) PURE;
  STDMETHOD(TranslateAccelerator)(LPMSG, const GUID*, DWORD) PURE;
  STDMETHOD(GetOptionKeyPath)(OLECHAR **, DWORD) PURE;
  STDMETHOD(GetDropTarget)(LPDROPTARGET, LPDROPTARGET*) PURE;
  STDMETHOD(GetExternal)(LPDISPATCH*) PURE;
  STDMETHOD(TranslateUrl)(DWORD, OLECHAR*, OLECHAR **) PURE;
  STDMETHOD(FilterDataObject)(LPDATAOBJECT , LPDATAOBJECT*) PURE;
};
#undef INTERFACE

#define DISPID_BEFORENAVIGATE2              250           // hyperlink clicked on
#define DISPID_NEWWINDOW2                   251
#define DISPID_NAVIGATECOMPLETE2            252           // UIActivate new document
#define DISPID_ONQUIT                       253
#define DISPID_ONVISIBLE                    254           // sent when the window goes visible/hidden
#define DISPID_ONTOOLBAR                    255           // sent when the toolbar should be shown/hidden
#define DISPID_ONMENUBAR                    256           // sent when the menubar should be shown/hidden
#define DISPID_ONSTATUSBAR                  257           // sent when the statusbar should be shown/hidden
#define DISPID_ONFULLSCREEN                 258           // sent when kiosk mode should be on/off
#define DISPID_DOCUMENTCOMPLETE             259           // new document goes ReadyState_Complete
#define DISPID_ONTHEATERMODE                260           // sent when theater mode should be on/off
#define DISPID_ONADDRESSBAR                 261           // sent when the address bar should be shown/hidden
#define DISPID_WINDOWSETRESIZABLE           262           // sent to set the style of the host window frame
#define DISPID_WINDOWCLOSING                263           // sent before script window.close closes the window
#define DISPID_WINDOWSETLEFT                264           // sent when the put_left method is called on the WebOC
#define DISPID_WINDOWSETTOP                 265           // sent when the put_top method is called on the WebOC
#define DISPID_WINDOWSETWIDTH               266           // sent when the put_width method is called on the WebOC
#define DISPID_WINDOWSETHEIGHT              267           // sent when the put_height method is called on the WebOC
#define DISPID_CLIENTTOHOSTWINDOW           268           // sent during window.open to request conversion of dimensions
#define DISPID_SETSECURELOCKICON            269           // sent to suggest the appropriate security icon to show
#define DISPID_FILEDOWNLOAD                 270           // Fired to indicate the File Download dialog is opening
#define DISPID_NAVIGATEERROR                271           // Fired to indicate the a binding error has occured
#define DISPID_PRIVACYIMPACTEDSTATECHANGE   272           // Fired when the user's browsing experience is impacted
#define DISPID_NEWWINDOW3                   273
#define DISPID_VIEWUPDATE                   281           // Fired when the contents of a shell browser window change
#define DISPID_SETPHISHINGFILTERSTATUS      282           // Fired by the Phishing Filter API to signal what state the analysis is in
#define DISPID_WINDOWSTATECHANGED           283           // Fired to indicate that the browser window's visibility or enabled state has changed

typedef enum tagDOCHOSTUIFLAG {
    DOCHOSTUIFLAG_DIALOG = 0x00000001,
    DOCHOSTUIFLAG_DISABLE_HELP_MENU = 0x00000002,
    DOCHOSTUIFLAG_NO3DBORDER = 0x00000004,
    DOCHOSTUIFLAG_SCROLL_NO = 0x00000008,
    DOCHOSTUIFLAG_DISABLE_SCRIPT_INACTIVE = 0x00000010,
    DOCHOSTUIFLAG_OPENNEWWIN = 0x00000020,
    DOCHOSTUIFLAG_DISABLE_OFFSCREEN = 0x00000040,
    DOCHOSTUIFLAG_FLAT_SCROLLBAR = 0x00000080,
    DOCHOSTUIFLAG_DIV_BLOCKDEFAULT = 0x00000100,
    DOCHOSTUIFLAG_ACTIVATE_CLIENTHIT_ONLY = 0x00000200,
    DOCHOSTUIFLAG_OVERRIDEBEHAVIORFACTORY = 0x00000400,
    DOCHOSTUIFLAG_CODEPAGELINKEDFONTS = 0x00000800,
    DOCHOSTUIFLAG_URL_ENCODING_DISABLE_UTF8 = 0x00001000,
    DOCHOSTUIFLAG_URL_ENCODING_ENABLE_UTF8 = 0x00002000,
    DOCHOSTUIFLAG_ENABLE_FORMS_AUTOCOMPLETE = 0x00004000,
    DOCHOSTUIFLAG_ENABLE_INPLACE_NAVIGATION = 0x00010000,
    DOCHOSTUIFLAG_IME_ENABLE_RECONVERSION = 0x00020000,
    DOCHOSTUIFLAG_THEME = 0x00040000,
    DOCHOSTUIFLAG_NOTHEME = 0x00080000,
    DOCHOSTUIFLAG_NOPICS = 0x00100000,
    DOCHOSTUIFLAG_NO3DOUTERBORDER = 0x00200000,
    DOCHOSTUIFLAG_DISABLE_EDIT_NS_FIXUP = 0x00400000,
    DOCHOSTUIFLAG_LOCAL_MACHINE_ACCESS_CHECK = 0x00800000,
    DOCHOSTUIFLAG_DISABLE_UNTRUSTEDPROTOCOL = 0x01000000,
    DOCHOSTUIFLAG_HOST_NAVIGATES = 0x02000000,
    DOCHOSTUIFLAG_ENABLE_REDIRECT_NOTIFICATION = 0x04000000,
    DOCHOSTUIFLAG_USE_WINDOWLESS_SELECTCONTROL = 0x08000000,
    DOCHOSTUIFLAG_USE_WINDOWED_SELECTCONTROL = 0x10000000,
    DOCHOSTUIFLAG_ENABLE_ACTIVEX_INACTIVATE_MODE = 0x20000000,
    DOCHOSTUIFLAG_DPI_AWARE = 0x40000000
} DOCHOSTUIFLAG;

typedef enum tagDOCHOSTUIDBLCLK {
    DOCHOSTUIDBLCLK_DEFAULT = 0,
    DOCHOSTUIDBLCLK_SHOWPROPERTIES = 1,
    DOCHOSTUIDBLCLK_SHOWCODE = 2
} DOCHOSTUIDBLCLK;

#endif //_XUNDEF_H_
