#ifndef _XOLECTRL_H_
#define _XOLECTRL_H_

#include "xwnd.h"
#include "xcoords.h"
#include "xgdi.h"

#ifdef _MSC_VER
#pragma comment(lib, "ole32.lib")
#pragma comment(lib, "oleaut32.lib")
#pragma comment(lib, "uuid.lib")
#endif

class XOleControl:public XWnd{
  DECLARE_DYNCREATE(XOleControl)
  public:
    XOleControl(HWND hwnd=NULL);
    virtual ~XOleControl();

    virtual HWND Create(HWND hWndParent,LPCTSTR lpWindowName,LPCTSTR lpszClsidName,UINT CtrlId,
      const RECT& rect=XDEFAULTRECT,DWORD dwStyle=XDEFAULTWNDSTYLE,BSTR bstrLicKey=NULL,LPVOID lpParam=NULL);

    virtual HWND CreateEx(DWORD dwExStyle,HWND hWndParent,LPCTSTR lpClsidName,LPCTSTR lpszClassName,UINT CtrlId,
      const RECT& rect=XDEFAULTRECT,DWORD dwStyle=XDEFAULTWNDSTYLE,BSTR bstrLicKey=NULL,LPVOID lpParam=NULL);

    virtual HWND Create(HWND hWndParent,LPCTSTR lpWindowName,REFCLSID clsid,UINT CtrlId,
      const RECT& rect=XDEFAULTRECT,DWORD dwStyle=XDEFAULTWNDSTYLE,BSTR bstrLicKey=NULL,LPVOID lpParam=NULL);

    virtual HWND CreateEx(DWORD dwExStyle,HWND hWndParent,LPCTSTR lpWindowName,REFCLSID clsid,UINT CtrlId,
      const RECT& rect=XDEFAULTRECT,DWORD dwStyle=XDEFAULTWNDSTYLE,BSTR bstrLicKey=NULL,LPVOID lpParam=NULL);

    virtual BOOL DestroyWindow();

    virtual BOOL IsValid(){
      return (this==NULL?FALSE:(m_hWnd!=NULL&&::IsWindow(m_hWnd))||m_bIsWindowless);
    }

    virtual DWORD GetStyle()const;
    virtual DWORD GetExStyle()const;
    virtual DWORD SetStyle(DWORD dwStyle);
    virtual DWORD SetExStyle(DWORD dwExStyle);
    virtual BOOL TestStyle(BOOL bEx,DWORD dwStyle);
    virtual BOOL ModifyStyle(BOOL bEx=FALSE,DWORD dwRemove=0,DWORD dwAdd=0,UINT nFlags=0);
    virtual BOOL SetWindowText(LPCTSTR lpszString);
    virtual INT GetWindowText(LPTSTR lpString,INT nMaxCount)const;
    virtual INT GetDlgCtrlID()const;
    virtual INT SetDlgCtrlID(INT nID);
    virtual BOOL MoveWindow(INT X,INT Y,INT nWidth,INT nHeight,BOOL);
    virtual BOOL SetWindowPos(HWND after,INT x,INT y,INT cx,INT cy,UINT uFlags);
    virtual BOOL ShowWindow(UINT mode=SW_SHOWNORMAL);
    virtual BOOL IsEnabled()const;
    virtual BOOL EnableWindow(BOOL bEnable=TRUE);
    virtual HWND SetFocus();
    virtual HWND SetFocus(LPMSG lpmsg);

    virtual BOOL InvokeHelper(DISPID dwDispID,WORD wFlags,VARTYPE vtRet,PVOID pvRet,const WCHAR* pbParamInfo,...);
    virtual BOOL GetProperty(DISPID dwDispID,VARTYPE vtProp,PVOID pvProp)const;
    virtual BOOL SetProperty(DISPID dwDispID,VARTYPE vtProp,...);

    virtual BOOL PreTranslateMessage(LPMSG lpMsg);

    BOOL GetEventIID(IID* piid);
    BOOL IsMatchingMnemonic(LPMSG lpMsg);
    VOID SendMnemonic(LPMSG lpMsg);
    VOID FreezeEvents(BOOL bFreeze);

    virtual BOOL QuickActivate();
    virtual HRESULT DoVerb(LONG nVerb, LPMSG lpMsg=NULL);
    virtual BOOL GetAmbientProperty(DISPID dispid,VARIANT* pvar);

  protected:
    BOOL SetExtent();
    VOID GetControlInfo();

    virtual DWORD ConnectSink(REFIID iid, LPUNKNOWN punkSink);
    virtual VOID DisconnectSink(REFIID iid, DWORD dwCookie);

    virtual BOOL OnEvent(XSINKEVENT* pEvent);
    virtual BOOL OnAmbientProperty(DISPID dispid,VARIANT* pvar);

    virtual BOOL Attach(HWND hWnd);
    virtual HWND Detach();

  private:
    BOOL InitDispDriver()const;

  private:
    IID m_iidEvents;
    CONTROLINFO m_ctlInfo;
    DWORD m_dwMiscStatus;
    DWORD m_dwEventSink;
    DWORD m_dwPropNotifySink;
    DWORD m_dwStyle,m_dwStyleMask;
    DWORD m_dwExStyle,m_dwExStyleMask;
    XOleDispatchDriver m_dispDriver;
    HDC m_hDC;BOOL m_bCreated;

  public:
    // Interfaces
    BEGIN_INTERNAL_INTERFACE(XOleControl,OleClientSite,IOleClientSite)
      STDMETHOD(SaveObject)();
      STDMETHOD(GetMoniker)(DWORD, DWORD, LPMONIKER*);
      STDMETHOD(GetContainer)(LPOLECONTAINER*);
      STDMETHOD(ShowObject)();
      STDMETHOD(OnShowWindow)(BOOL);
      STDMETHOD(RequestNewObjectLayout)();
    END_INTERNAL_INTERFACE(XOleControl,OleClientSite)

    BEGIN_INTERNAL_INTERFACE(XOleControl,OleIPSite,IOleInPlaceSiteWindowless)
      STDMETHOD(GetWindow)(HWND*);
      STDMETHOD(ContextSensitiveHelp)(BOOL);
      STDMETHOD(CanInPlaceActivate)();
      STDMETHOD(OnInPlaceActivate)();
      STDMETHOD(OnUIActivate)();
      STDMETHOD(GetWindowContext)(LPOLEINPLACEFRAME*,LPOLEINPLACEUIWINDOW*, LPRECT, LPRECT, LPOLEINPLACEFRAMEINFO);
      STDMETHOD(Scroll)(SIZE);
      STDMETHOD(OnUIDeactivate)(BOOL);
      STDMETHOD(OnInPlaceDeactivate)();
      STDMETHOD(DiscardUndoState)();
      STDMETHOD(DeactivateAndUndo)();
      STDMETHOD(OnPosRectChange)(LPCRECT);
      STDMETHOD(OnInPlaceActivateEx)( BOOL* ptNoRedraw, DWORD dwFlags );
      STDMETHOD(OnInPlaceDeactivateEx)( BOOL tNoRedraw );
      STDMETHOD(RequestUIActivate)();
      STDMETHOD(CanWindowlessActivate)();
      STDMETHOD(GetCapture)();
      STDMETHOD(SetCapture)( BOOL tCapture );
      STDMETHOD(GetFocus)();
      STDMETHOD(SetFocus)( BOOL tFocus );
      STDMETHOD(GetDC)( LPCRECT prect, DWORD dwFlags, HDC* phDC );
      STDMETHOD(ReleaseDC)( HDC hDC );
      STDMETHOD(InvalidateRect)( LPCRECT prect, BOOL tErase );
      STDMETHOD(InvalidateRgn)( HRGN hRgn, BOOL tErase );
      STDMETHOD(ScrollRect)( int dx, int dy, LPCRECT prectScroll,LPCRECT prectClip );
      STDMETHOD(AdjustRect)( LPRECT prect );//Fix ocidl.h  (line 737 aprox) on error! (Is not Constant!)
      STDMETHOD(OnDefWindowMessage)( UINT nMessage, WPARAM wParam,LPARAM lParam, LRESULT* plResult );
    END_INTERNAL_INTERFACE(XOleControl,OleIPSite)

    BEGIN_INTERNAL_INTERFACE(XOleControl,OleControlSite,IOleControlSite)
      STDMETHOD(OnControlInfoChanged)();
      STDMETHOD(LockInPlaceActive)(BOOL fLock);
      STDMETHOD(GetExtendedControl)(LPDISPATCH* ppDisp);
      STDMETHOD(TransformCoords)(POINTL* lpptlHimetric,POINTF* lpptfContainer, DWORD flags);
      STDMETHOD(TranslateAccelerator)(LPMSG lpMsg, DWORD grfModifiers);
      STDMETHOD(OnFocus)(BOOL fGotFocus);
      STDMETHOD(ShowPropertyFrame)();
    END_INTERNAL_INTERFACE(XOleControl,OleControlSite)

    BEGIN_INTERNAL_INTERFACE(XOleControl,AmbientProps,IDispatch)
      STDMETHOD(GetTypeInfoCount)(unsigned int*);
      STDMETHOD(GetTypeInfo)(unsigned int, LCID, ITypeInfo**);
      STDMETHOD(GetIDsOfNames)(REFIID, LPOLESTR*, unsigned int cNames, LCID, DISPID*);
      STDMETHOD(Invoke)(DISPID, REFIID, LCID, unsigned short, DISPPARAMS*,VARIANT*, EXCEPINFO*, unsigned int*);
    END_INTERNAL_INTERFACE(XOleControl,AmbientProps)

    BEGIN_INTERNAL_INTERFACE(XOleControl,PropertyNotifySink,IPropertyNotifySink)
      STDMETHOD(OnChanged)(DISPID dispid);
      STDMETHOD(OnRequestEdit)(DISPID dispid);
    END_INTERNAL_INTERFACE(XOleControl,PropertyNotifySink)

    BEGIN_INTERNAL_INTERFACE(XOleControl,EventSink,IDispatch)
      STDMETHOD(GetTypeInfoCount)(unsigned int*);
      STDMETHOD(GetTypeInfo)(unsigned int, LCID, ITypeInfo**);
      STDMETHOD(GetIDsOfNames)(REFIID, LPOLESTR*, unsigned int cNames, LCID, DISPID*);
      STDMETHOD(Invoke)(DISPID, REFIID, LCID, unsigned short, DISPPARAMS*,VARIANT*, EXCEPINFO*, unsigned int*);
    END_INTERNAL_INTERFACE(XOleControl,EventSink)

    DECLARE_INTERFACE_EVENT_TABLE()
    DECLARE_EVENT_TABLE()
};

#endif //_XOLECTRL_H_
