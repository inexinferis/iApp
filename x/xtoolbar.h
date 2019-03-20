#ifndef _XTOOLBAR_H_
#define _XTOOLBAR_H_

#include "xwnd.h"

#define XDEFTOOLBARSTYLE WS_CHILD|WS_CLIPCHILDREN|WS_VISIBLE

class XToolBar:public XWnd{
  DECLARE_DYNAMIC(XToolBar)
  public:
    XToolBar(HWND hwnd=NULL):XWnd(hwnd){}
    virtual ~XToolBar(){}

    virtual HWND Create(HWND pParentWnd,LPCTSTR lpszWindowName,UINT CtrlId,const RECT& rect=XDEFAULTRECT,DWORD dwStyle=XDEFTOOLBARSTYLE,LPVOID lpParam=NULL){
      return XWnd::Create(WC_EDIT,lpszWindowName,dwStyle,rect.left,rect.top,rect.right,rect.bottom,pParentWnd,(HMENU)(UINT_PTR)CtrlId,lpParam);
    }

    virtual HWND CreateEx(DWORD dwStyleEx,HWND pParentWnd,LPCTSTR lpszWindowName,UINT CtrlId,const RECT& rect=XDEFAULTRECT,DWORD dwStyle=XDEFTOOLBARSTYLE,LPVOID lpParam=NULL){
      return XWnd::CreateEx(dwStyleEx,WC_EDIT,lpszWindowName,dwStyle,rect.left,rect.top,rect.right,rect.bottom,pParentWnd,(HMENU)(UINT_PTR)CtrlId,lpParam);
    }

    BOOL EnableButton(INT nID,BOOL bEnable=TRUE){
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,TB_ENABLEBUTTON,nID,MAKELPARAM(bEnable,0));
    }

    BOOL CheckButton(INT nID,BOOL bCheck=TRUE){
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,TB_CHECKBUTTON,nID,MAKELPARAM(bCheck,0));
    }

    BOOL PressButton(INT nID,BOOL bPress=TRUE){
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,TB_PRESSBUTTON,nID,MAKELPARAM(bPress,0));
    }

    BOOL HideButton(INT nID,BOOL bHide=TRUE){
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,TB_HIDEBUTTON,nID,MAKELPARAM(bHide,0));
    }

    BOOL Indeterminate(INT nID,BOOL bIndeterminate=TRUE){
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,TB_INDETERMINATE,nID,MAKELPARAM(bIndeterminate,0));
    }

    BOOL IsButtonEnabled(INT nID)const{
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,TB_ISBUTTONENABLED,nID,0);
    }

    BOOL IsButtonChecked(INT nID)const{
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,TB_ISBUTTONCHECKED,nID,0);
    }

    BOOL IsButtonPressed(INT nID)const{
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,TB_ISBUTTONPRESSED,nID,0);
    }

    BOOL IsButtonHidden(INT nID)const{
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,TB_ISBUTTONHIDDEN,nID,0);
    }

    BOOL IsButtonIndeterminate(INT nID)const{
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,TB_ISBUTTONINDETERMINATE,nID,0);
    }

    BOOL SetState(INT nID,UINT nState){
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,TB_SETSTATE,nID,MAKELPARAM(nState,0));
    }

    INT GetState(INT nID)const{
      XASSERT(::IsWindow(m_hWnd))
      return (INT)::SendMessage(m_hWnd,TB_GETSTATE,nID,0);
    }

    BOOL AddButtons(INT nNumButtons,LPTBBUTTON lpButtons){
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,TB_ADDBUTTONS,nNumButtons,(LPARAM)lpButtons);
    }

    BOOL InsertButton(INT nIndex,LPTBBUTTON lpButton){
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,TB_INSERTBUTTON,nIndex,(LPARAM)lpButton);
    }

    BOOL DeleteButton(INT nIndex){
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,TB_DELETEBUTTON,nIndex,0);
    }

    BOOL GetButton(INT nIndex,LPTBBUTTON lpButton)const{
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,TB_GETBUTTON,nIndex,(LPARAM)lpButton);
    }

    INT GetButtonCount()const{
      XASSERT(::IsWindow(m_hWnd))
      return (INT)::SendMessage(m_hWnd,TB_BUTTONCOUNT,0,0);
    }

    UINT CommandToIndex(UINT nID)const{
      XASSERT(::IsWindow(m_hWnd))
      return (UINT)::SendMessage(m_hWnd,TB_COMMANDTOINDEX,nID,0);
    }

    VOID Customize(){
      XASSERT(::IsWindow(m_hWnd))
      ::SendMessage(m_hWnd,TB_CUSTOMIZE,0,0);
    }

    INT AddStrings(LPCTSTR lpszStrings){
      XASSERT(::IsWindow(m_hWnd))
      return (INT)::SendMessage(m_hWnd,TB_ADDSTRING,0,(LPARAM)lpszStrings);
    }

    BOOL GetItemRect(INT nIndex,LPRECT lpRect)const{
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,TB_GETITEMRECT,nIndex,(LPARAM)lpRect);
    }

    VOID SetButtonStructSize(INT nSize){
      XASSERT(::IsWindow(m_hWnd))
      ::SendMessage(m_hWnd,TB_BUTTONSTRUCTSIZE,nSize,0);
    }

    BOOL SetButtonSize(SIZE size){
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,TB_SETBUTTONSIZE,0,MAKELPARAM(size.cx,size.cy));
    }

    BOOL SetBitmapSize(SIZE size){
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,TB_SETBITMAPSIZE,0,MAKELPARAM(size.cx,size.cy));
    }

    VOID AutoSize(){
      XASSERT(::IsWindow(m_hWnd))
      ::SendMessage(m_hWnd,TB_AUTOSIZE,0,0);
    }

    HWND GetToolTips()const{
      XASSERT(::IsWindow(m_hWnd))
      return (HWND)::SendMessage(m_hWnd,TB_GETTOOLTIPS,0,0);
    }

    VOID SetToolTips(HWND hTipWnd){
      XASSERT(::IsWindow(m_hWnd))
      ::SendMessage(m_hWnd,TB_SETTOOLTIPS,(WPARAM)hTipWnd,0);
    }

    VOID SetOwner(HWND hWnd){
      XASSERT(::IsWindow(m_hWnd))
      ::SendMessage(m_hWnd,TB_SETPARENT,(WPARAM)hWnd,0);
    }

    VOID SetRows(INT nRows,BOOL bLarger,LPRECT lpRect){
      XASSERT(::IsWindow(m_hWnd))
      ::SendMessage(m_hWnd,TB_SETROWS,MAKEWPARAM(nRows,bLarger),(LPARAM)lpRect);
    }

    INT GetRows()const{
      XASSERT(::IsWindow(m_hWnd))
      return (INT)::SendMessage(m_hWnd,TB_GETROWS,0,0);
    }

    BOOL SetCmdID(INT nIndex,UINT nID){
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,TB_SETCMDID,nIndex,nID);
    }

    UINT GetBitmapFlags()const{
      XASSERT(::IsWindow(m_hWnd))
      return (UINT)::SendMessage(m_hWnd,TB_GETBITMAPFLAGS,0,0);
    }

    INT GetBitmap(INT nID)const{
      XASSERT(::IsWindow(m_hWnd))
      return (INT)::SendMessage(m_hWnd,TB_GETBITMAP,nID,0);
    }

    BOOL ReplaceBitmap(LPTBREPLACEBITMAP pReplaceBitmap){
      XASSERT(::IsWindow(m_hWnd))
      return (INT)::SendMessage(m_hWnd,TB_REPLACEBITMAP,0,(LPARAM)pReplaceBitmap);
    }

    BOOL ChangeBitmap(INT idButton,INT iBitmap){
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,TB_CHANGEBITMAP,idButton,(LPARAM)MAKELPARAM(iBitmap,0));
    }

    INT HitTest(LPPOINT ppt)const{
      XASSERT(::IsWindow(m_hWnd))
      return (INT)::SendMessage(m_hWnd,TB_HITTEST,0,(LPARAM)ppt);
    }

    DWORD GetExtendedStyle()const{
      XASSERT(::IsWindow(m_hWnd))
      return (DWORD)::SendMessage(m_hWnd,TB_GETEXTENDEDSTYLE,0,0);
    }

    DWORD SetExtendedStyle(DWORD dwExStyle){
      XASSERT(::IsWindow(m_hWnd))
      return (DWORD)::SendMessage(m_hWnd,TB_SETEXTENDEDSTYLE,0,dwExStyle);
    }

    VOID GetPadding(PINT pHorzPadding,PINT pVertPadding)const{
      XASSERT(::IsWindow(m_hWnd))
      DWORD dwPadding=(DWORD)::SendMessage(m_hWnd,TB_GETPADDING,0,0);
      if(pHorzPadding)
        *pHorzPadding=(INT)LOWORD(dwPadding);
      if(pVertPadding)
        *pVertPadding=(INT)HIWORD(dwPadding);
    }

    DWORD SetPadding(INT nHorzPadding,INT nVertPadding){
      XASSERT(::IsWindow(m_hWnd))
      return (DWORD)::SendMessage(m_hWnd,TB_SETPADDING,0,(LPARAM)MAKELPARAM(nHorzPadding,nVertPadding));
    }

    COLORREF GetInsertMarkColor()const{
      XASSERT(::IsWindow(m_hWnd))
      return (COLORREF)::SendMessage(m_hWnd,TB_GETINSERTMARKCOLOR,0,0);
    }

    COLORREF SetInsertMarkColor(COLORREF clrNew){
      XASSERT(::IsWindow(m_hWnd))
      return (COLORREF)::SendMessage(m_hWnd,TB_SETINSERTMARKCOLOR,0,(LPARAM) clrNew);
    }

    BOOL GetColorScheme(COLORSCHEME* lpcs)const{
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)(::SendMessage(m_hWnd,TB_GETCOLORSCHEME,0,(LPARAM)lpcs) != 0);
    }

    VOID SetColorScheme(const COLORSCHEME* lpcs){
      XASSERT(::IsWindow(m_hWnd))
      ::SendMessage(m_hWnd,TB_SETCOLORSCHEME,0,(LPARAM)lpcs);
    }

    DWORD GetButtonSize()const{
      XASSERT(::IsWindow(m_hWnd))
      return (DWORD)::SendMessage(m_hWnd,TB_GETBUTTONSIZE,0,0);
    }

    HIMAGELIST GetDisabledImageList()const{
      XASSERT(::IsWindow(m_hWnd))
      return (HIMAGELIST)::SendMessage(m_hWnd,TB_GETDISABLEDIMAGELIST,0,0);
    }

    HIMAGELIST GetHotImageList()const{
      XASSERT(::IsWindow(m_hWnd))
      return (HIMAGELIST)::SendMessage(m_hWnd,TB_GETHOTIMAGELIST,0,0);
    }

    HIMAGELIST GetImageList()const{
      XASSERT(::IsWindow(m_hWnd))
      return (HIMAGELIST)::SendMessage(m_hWnd,TB_GETIMAGELIST,0,0);
    }

    virtual DWORD GetStyle()const{
      XASSERT(::IsWindow(m_hWnd))
      return (DWORD)::SendMessage(m_hWnd,TB_GETSTYLE,0,0);
    }

    INT GetMaxTextRows()const{
      XASSERT(::IsWindow(m_hWnd))
      return (INT)::SendMessage(m_hWnd,TB_GETTEXTROWS,0,0);
    }

    BOOL GetRect(INT nID,LPRECT lpRect)const{
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,TB_GETRECT,nID,(LPARAM)lpRect);
    }

    BOOL IsButtonHighlighted(INT nID)const{
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,TB_ISBUTTONHIGHLIGHTED,nID,0);
    }

    VOID LoadImages(INT iBitmapID,HINSTANCE hinst){
      XASSERT(::IsWindow(m_hWnd))
      ::SendMessage(m_hWnd,TB_LOADIMAGES,iBitmapID,(LPARAM)hinst);
    }

    BOOL SetButtonWidth(INT cxMin,INT cxMax){
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,TB_SETBUTTONWIDTH,0,MAKELPARAM(cxMin,cxMax));
    }

    HIMAGELIST SetDisabledImageList(HIMAGELIST hImageList){
      XASSERT(::IsWindow(m_hWnd))
      return (HIMAGELIST)::SendMessage(m_hWnd,TB_SETDISABLEDIMAGELIST,0,(LPARAM)hImageList);
    }

    HIMAGELIST SetHotImageList(HIMAGELIST hImageList){
      XASSERT(::IsWindow(m_hWnd))
      return (HIMAGELIST)::SendMessage(m_hWnd,TB_SETHOTIMAGELIST,0,(LPARAM)hImageList);
    }

    HIMAGELIST SetImageList(HIMAGELIST hImageList){
      XASSERT(::IsWindow(m_hWnd))
      return (HIMAGELIST)::SendMessage(m_hWnd,TB_SETIMAGELIST,0,(LPARAM)hImageList);
    }

    BOOL SetIndent(INT iIndent){
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,TB_SETINDENT,iIndent,0);
    }

    BOOL SetMaxTextRows(INT iMaxRows){
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,TB_SETMAXTEXTROWS,iMaxRows,0);
    }

    virtual DWORD SetStyle(DWORD dwStyle){
      XASSERT(::IsWindow(m_hWnd))
      DWORD dwOldStyle=(DWORD)::SendMessage(m_hWnd,TB_GETSTYLE,0,0);
      ::SendMessage(m_hWnd,TB_SETSTYLE,0,dwStyle);
      return dwOldStyle;
    }

    INT GetButtonInfo(INT nID,TBBUTTONINFO* ptbbi)const{
      XASSERT(::IsWindow(m_hWnd))
      return (INT)::SendMessage(m_hWnd,TB_GETBUTTONINFO,nID,(LPARAM)ptbbi);
    }

    BOOL SetButtonInfo(INT nID,TBBUTTONINFO* ptbbi){
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,TB_SETBUTTONINFO,nID,(LPARAM)ptbbi);
    }

    DWORD SetDrawTextFlags(DWORD dwMask,DWORD dwDTFlags){
      XASSERT(::IsWindow(m_hWnd))
      return (DWORD)::SendMessage(m_hWnd,TB_SETDRAWTEXTFLAGS,dwMask,dwDTFlags);
    }

    BOOL GetAnchorHighlight()const{
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,TB_GETANCHORHIGHLIGHT,0,0);
    }

    BOOL SetAnchorHighlight(BOOL fAnchor){
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,TB_SETANCHORHIGHLIGHT,fAnchor,0);
    }

    INT GetHotItem()const{
      XASSERT(::IsWindow(m_hWnd))
      return (INT)::SendMessage(m_hWnd,TB_GETHOTITEM,0,0);
    }

    INT SetHotItem(INT nHot){
      XASSERT(::IsWindow(m_hWnd))
      return (INT)::SendMessage(m_hWnd,TB_SETHOTITEM,nHot,0);
    }

    VOID GetInsertMark(TBINSERTMARK* ptbim)const{
      XASSERT(::IsWindow(m_hWnd))
      ::SendMessage(m_hWnd,TB_GETINSERTMARK,0,(LPARAM)ptbim);
    }

    VOID SetInsertMark(TBINSERTMARK* ptbim){
      XASSERT(::IsWindow(m_hWnd))
      ::SendMessage(m_hWnd,TB_SETINSERTMARK,0,(LPARAM)ptbim);
    }

    BOOL GetMaxSize(LPSIZE pSize)const{
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,TB_GETMAXSIZE,0,(LPARAM)pSize);
    }

    BOOL InsertMarkHitTest(LPPOINT ppt,LPTBINSERTMARK ptbim)const{
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,TB_INSERTMARKHITTEST,(WPARAM)ppt,(LPARAM)ptbim);
    }

    BOOL MapAccelerator(TCHAR chAccel,PUINT pIDBtn){
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,TB_MAPACCELERATOR,(WPARAM)chAccel,(LPARAM)pIDBtn);
    }

    BOOL MarkButton(INT nID,BOOL bHighlight=TRUE){
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,TB_MARKBUTTON,nID,MAKELPARAM(bHighlight,0));
    }

    BOOL MoveButton(UINT nOldPos,UINT nNewPos){
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,TB_MOVEBUTTON,nOldPos,nNewPos);
    }

    INT AddBitmap(INT nNumButtons,HBITMAP hBitmap){
      XASSERT(::IsWindow(m_hWnd))
      TBADDBITMAP tbab;tbab.hInst=NULL;tbab.nID=(UINT_PTR)hBitmap;
      return (INT)::SendMessage(m_hWnd,TB_ADDBITMAP,nNumButtons,(LPARAM)&tbab);
    }

    INT AddBitmap(INT nNumButtons,HINSTANCE hInstance,UINT nBitmapID){
      XASSERT(::IsWindow(m_hWnd))
      TBADDBITMAP tbab;tbab.hInst=hInstance;tbab.nID=nBitmapID;
      return (INT)::SendMessage(m_hWnd,TB_ADDBITMAP,nNumButtons,(LPARAM)&tbab);
    }

    VOID SaveState(HKEY hKeyRoot,LPCTSTR lpszSubKey,LPCTSTR lpszValueName){
      XASSERT(::IsWindow(m_hWnd))
      TBSAVEPARAMS tbs;tbs.hkr=hKeyRoot;tbs.pszSubKey=lpszSubKey;tbs.pszValueName=lpszValueName;
      ::SendMessage(m_hWnd,TB_SAVERESTORE,TRUE,(LPARAM)&tbs);
    }

    VOID RestoreState(HKEY hKeyRoot,LPCTSTR lpszSubKey,LPCTSTR lpszValueName){
      XASSERT(::IsWindow(m_hWnd))
      TBSAVEPARAMS tbs;tbs.hkr=hKeyRoot;tbs.pszSubKey=lpszSubKey;tbs.pszValueName=lpszValueName;
      ::SendMessage(m_hWnd,TB_SAVERESTORE,FALSE,(LPARAM)&tbs);
    }

  #if _WIN32_IE >= 0x0500
    INT GetString(INT nString,LPTSTR lpstrString,UINT cchMaxLen)const{
      XASSERT(::IsWindow(m_hWnd))
      return (INT)::SendMessage(m_hWnd,TB_GETSTRING,MAKEWPARAM(cchMaxLen,nString),(LPARAM)lpstrString);
    }
  #endif

  #if _WIN32_IE >= 0x0501
    #ifdef TB_GETMETRICS
    VOID GetMetrics(LPTBMETRICS ptbm)const{
      XASSERT(::IsWindow(m_hWnd)&&ptbm!=NULL)
      ::SendMessage(m_hWnd,TB_GETMETRICS,0,(LPARAM)ptbm);
    }
    #endif

    #ifdef TB_GETMETRICS
    VOID SetMetrics(LPTBMETRICS ptbm){
      XASSERT(::IsWindow(m_hWnd)&&ptbm!=NULL)
      ::SendMessage(m_hWnd,TB_SETMETRICS,0,(LPARAM)ptbm);
    }
    #endif

    #ifdef TB_SETWINDOWTHEME
    HRESULT SetWindowTheme(LPCWSTR pszSubAppName){
      XASSERT(::IsWindow(m_hWnd))
      return (HRESULT)::SendMessage(m_hWnd,TB_SETWINDOWTHEME,0,(LPARAM)pszSubAppName);
    }
    #endif
  #endif

  #if _WIN32_WINNT >= 0x600
    HIMAGELIST SetPressedImageList(INT iImageID,HIMAGELIST hImageList){
      XASSERT(::IsWindow(m_hWnd))
      return (HIMAGELIST)::SendMessage(m_hWnd,TB_SETPRESSEDIMAGELIST,iImageID,(LPARAM)hImageList);
    }

    HIMAGELIST GetPressedImageList()const{
      XASSERT(::IsWindow(m_hWnd))
      return (HIMAGELIST)::SendMessage(m_hWnd,TB_GETPRESSEDIMAGELIST,0,0);
    }
  #endif

    HRESULT GetDropTarget(IDropTarget** ppDropTarget)const{
      XASSERT(::IsWindow(m_hWnd))
      return (HRESULT)::SendMessage(m_hWnd,TB_GETOBJECT,(WPARAM)&IID_IDropTarget,(LPARAM)ppDropTarget);
    }

  public:
    DECLARE_EVENT_TABLE()
};

#endif // _XTOOLBAR_H_
