#ifndef _XHTMLCTRL_H_
#define _XHTMLCTRL_H_

#include "xtl.h"
#include "xolectrl.h"
#include <exdisp.h>
#include <mshtml.h>

class XHtmlCtrl:public XOleControl{
  DECLARE_DYNCREATE(XHtmlCtrl)
  public:
    XHtmlCtrl(HWND hwnd=NULL):XOleControl(hwnd){}
    virtual ~XHtmlCtrl();

    virtual HWND Create(HWND hWndParent,LPCTSTR lpWindowName,UINT CtrlId,const RECT& rect=XDEFAULTRECT,DWORD dwStyle=XDEFAULTWNDSTYLE);

    virtual HWND CreateEx(DWORD dwExStyle,HWND hWndParent,LPCTSTR lpWindowName,UINT CtrlId,const RECT& rect=XDEFAULTRECT,DWORD dwStyle=XDEFAULTWNDSTYLE);

    virtual BOOL DestroyWindow();

    LPWSTR GetType(LPWSTR buffer,UINT sz) const {
      XASSERT(m_pBrowserApp!=NULL)
      XASSERT(buffer!=NULL&&sz>0)
      BSTR bstr;
      HRESULT hr=m_pBrowserApp->get_Type(&bstr);
      XASSERT(SUCCEEDED(hr))
      if(SUCCEEDED(hr)){
        wcsncpy(buffer,bstr,sz);
        buffer[sz]=0;
        SysFreeString(bstr);
        return buffer;
      }
      return NULL;
    }

    LPWSTR GetFullName(LPWSTR buffer,UINT sz) const {
      XASSERT(m_pBrowserApp!=NULL)
      XASSERT(buffer!=NULL&&sz>0)
      BSTR bstr;
      HRESULT hr=m_pBrowserApp->get_FullName(&bstr);
      XASSERT(SUCCEEDED(hr))
      if(SUCCEEDED(hr)){
        wcsncpy(buffer,bstr,sz);
        buffer[sz]=0;
        SysFreeString(bstr);
        return buffer;
      }
      return NULL;
    }

    LONG GetLeft() const {
      XASSERT(m_pBrowserApp!=NULL)
      LONG result;
      HRESULT hr=m_pBrowserApp->get_Left(&result);
      XASSERT(SUCCEEDED(hr))
      XUNUSED(hr)
      return result;
    }

    VOID SetLeft(LONG nNewValue){
      XASSERT(m_pBrowserApp!=NULL)
      m_pBrowserApp->put_Left(nNewValue);
    }

    LONG GetTop() const {
      XASSERT(m_pBrowserApp!=NULL)
      LONG result;
      HRESULT hr=m_pBrowserApp->get_Top(&result);
      XASSERT(SUCCEEDED(hr))
      XUNUSED(hr)
      return result;
    }

    VOID SetTop(LONG nNewValue){
      XASSERT(m_pBrowserApp!=NULL)
      m_pBrowserApp->put_Top(nNewValue);
    }

    INT GetToolBar() const {
      XASSERT(m_pBrowserApp!=NULL)
      INT result;
      HRESULT hr=m_pBrowserApp->get_ToolBar(&result);
      XASSERT(SUCCEEDED(hr))
      XUNUSED(hr)
      return result;
    }

    VOID SetToolBar(INT nNewValue){
      XASSERT(m_pBrowserApp!=NULL)
      m_pBrowserApp->put_ToolBar(nNewValue);
    }

    LONG GetHeight() const {
      XASSERT(m_pBrowserApp!=NULL)
      LONG result;
      HRESULT hr=m_pBrowserApp->get_Height(&result);
      XASSERT(SUCCEEDED(hr))
      XUNUSED(hr)
      return result;
    }

    VOID SetHeight(LONG nNewValue){
      XASSERT(m_pBrowserApp!=NULL)
      m_pBrowserApp->put_Height(nNewValue);
    }

    LONG GetWidth() const {
      XASSERT(m_pBrowserApp!=NULL)
      LONG result;
      HRESULT hr=m_pBrowserApp->get_Width(&result);
      XASSERT(SUCCEEDED(hr))
      XUNUSED(hr)
      return result;
    }

    VOID SetWidth(LONG nNewValue){
      XASSERT(m_pBrowserApp!=NULL)
      m_pBrowserApp->put_Width(nNewValue);
    }

    BOOL GetVisible() const {
      XASSERT(m_pBrowserApp!=NULL)
      VARIANT_BOOL result;
      HRESULT hr=m_pBrowserApp->get_Visible(&result);
      XASSERT(SUCCEEDED(hr))
      if(FAILED(hr))return FALSE;
      return result;
    }

    VOID SetVisible(BOOL bNewValue){
      XASSERT(m_pBrowserApp!=NULL)
      m_pBrowserApp->put_Visible((bNewValue)?VARIANT_TRUE:VARIANT_FALSE);
    }

    LPWSTR GetLocationName(LPWSTR buffer,UINT sz) const {
      XASSERT(m_pBrowserApp!=NULL)
      XASSERT(buffer!=NULL&&sz>0)
      BSTR bstr;
      HRESULT hr=m_pBrowserApp->get_LocationName(&bstr);
      XASSERT(SUCCEEDED(hr))
      if(SUCCEEDED(hr)){
        wcsncpy(buffer,bstr,sz);
        buffer[sz]=0;
        SysFreeString(bstr);
        return buffer;
      }
      return NULL;
    }

    LPWSTR GetLocationURL(LPWSTR buffer,UINT sz) const {
      XASSERT(m_pBrowserApp!=NULL)
      XASSERT(buffer!=NULL&&sz>0)
      BSTR bstr;
      HRESULT hr=m_pBrowserApp->get_LocationURL(&bstr);
      XASSERT(SUCCEEDED(hr))
      if(SUCCEEDED(hr)){
        wcsncpy(buffer,bstr,sz);
        buffer[sz]=0;
        SysFreeString(bstr);
        return buffer;
      }
      return NULL;
    }

    BOOL GetSource(LPSTR buffer,UINT sz){
      XASSERT(m_pBrowserApp!=NULL)
      XASSERT(buffer!=NULL&&sz>0)
      BOOL bRetVal=FALSE;
      XComPtr<IDispatch,IID_IDispatch> spDisp;
      m_pBrowserApp->get_Document(&spDisp);
      if(spDisp!=NULL){
        XComQIPtr<IPersistStreamInit,IID_IPersistStreamInit> spPersistStream(spDisp);
        if(spPersistStream!=NULL){
          HGLOBAL hMemory;
          hMemory=GlobalAlloc(GMEM_MOVEABLE, 0);
          if(hMemory!=NULL){
            XComPtr<IStream,IID_IStream> spStream;
            if(SUCCEEDED(CreateStreamOnHGlobal(hMemory, TRUE, &spStream))){
              STATSTG statStg;
              spPersistStream->Save(spStream,FALSE);
              spStream->Stat(&statStg,STATFLAG_NONAME);
              LPCSTR pstr=static_cast<LPCSTR>(GlobalLock(hMemory));
              if(pstr!=NULL){
                bRetVal=TRUE;
                int len=__min(sz,statStg.cbSize.LowPart);
                strncpy(buffer,pstr,len);
                buffer[len]=0;
                GlobalUnlock(hMemory);
              }else
                GlobalFree(hMemory);
            }else
              GlobalFree(hMemory);
          }
        }
      }
      return bRetVal;
    }

    BOOL GetBusy() const{
      XASSERT(m_pBrowserApp!=NULL)
      VARIANT_BOOL result;
      HRESULT hr=m_pBrowserApp->get_Busy(&result);
      XASSERT(SUCCEEDED(hr))
      if(FAILED(hr))
        return FALSE;
      return result;
    }

    READYSTATE GetReadyState() const {
      XASSERT(m_pBrowserApp!=NULL)
      READYSTATE result;
      HRESULT hr=m_pBrowserApp->get_ReadyState(&result);
      XASSERT(SUCCEEDED(hr))
      XUNUSED(hr)
      return result;
    }

    BOOL GetOffline() const{
      XASSERT(m_pBrowserApp!=NULL)
      VARIANT_BOOL result;
      HRESULT hr=m_pBrowserApp->get_Offline(&result);
      XASSERT(SUCCEEDED(hr))
      if(FAILED(hr))return FALSE;
      return result;
    }

    VOID SetOffline(BOOL bNewValue){
      XASSERT(m_pBrowserApp!=NULL)
      m_pBrowserApp->put_Offline((bNewValue)?VARIANT_TRUE:VARIANT_FALSE);
    }

    BOOL GetSilent() const {
      XASSERT(m_pBrowserApp!=NULL)
      VARIANT_BOOL result;
      HRESULT hr=m_pBrowserApp->get_Silent(&result);
      XASSERT(SUCCEEDED(hr))
      if(FAILED(hr))return FALSE;
      return result;
    }

    VOID SetSilent(BOOL bNewValue){
      XASSERT(m_pBrowserApp!=NULL)
      m_pBrowserApp->put_Silent((bNewValue)?VARIANT_TRUE:VARIANT_FALSE);
    }

    LPDISPATCH GetApplication() const {
      XASSERT(m_pBrowserApp!=NULL)
      LPDISPATCH result;
      HRESULT hr=m_pBrowserApp->get_Application(&result);
      XASSERT(SUCCEEDED(hr))
      if(FAILED(hr))return NULL;
      return result;
    }

    LPDISPATCH GetParentBrowser() const {
      XASSERT(m_pBrowserApp!=NULL)
      LPDISPATCH result;
      HRESULT hr=m_pBrowserApp->get_Parent(&result);
      XASSERT(SUCCEEDED(hr))
      if(FAILED(hr))return NULL;
      return result;
    }

    LPDISPATCH GetContainer() const {
      XASSERT(m_pBrowserApp!=NULL)
      LPDISPATCH result;
      HRESULT hr=m_pBrowserApp->get_Container(&result);
      XASSERT(SUCCEEDED(hr))
      if(FAILED(hr))return NULL;
      return result;
    }

    LPDISPATCH GetHtmlDocument() const {
      XASSERT(m_pBrowserApp!=NULL)
      LPDISPATCH result;
      HRESULT hr=m_pBrowserApp->get_Document(&result);
      XASSERT(SUCCEEDED(hr))
      if(FAILED(hr))return NULL;
      return result;
    }

    BOOL GetTopLevelContainer() const{
      XASSERT(m_pBrowserApp!=NULL)
      VARIANT_BOOL result;
      HRESULT hr=m_pBrowserApp->get_TopLevelContainer(&result);
      XASSERT(SUCCEEDED(hr))
      if(FAILED(hr))return FALSE;
      return result;
    }

    BOOL GetMenuBar() const {
      XASSERT(m_pBrowserApp!=NULL)
      VARIANT_BOOL result;
      HRESULT hr=m_pBrowserApp->get_MenuBar(&result);
      XASSERT(SUCCEEDED(hr))
      if(FAILED(hr))return FALSE;
      return result;
    }

    VOID SetMenuBar(BOOL bNewValue){
      XASSERT(m_pBrowserApp!=NULL)
      m_pBrowserApp->put_MenuBar((bNewValue)?VARIANT_TRUE:VARIANT_FALSE);
    }

    BOOL GetFullScreen() const {
      XASSERT(m_pBrowserApp!=NULL)
      VARIANT_BOOL result;
      HRESULT hr=m_pBrowserApp->get_FullScreen(&result);
      XASSERT(SUCCEEDED(hr))
      if(FAILED(hr))return FALSE;
      return result;
    }

    VOID SetFullScreen(BOOL bNewValue){
      XASSERT(m_pBrowserApp!=NULL)
      m_pBrowserApp->put_FullScreen((bNewValue)?VARIANT_TRUE:VARIANT_FALSE);
    }

    BOOL GetStatusBar() const {
      XASSERT(m_pBrowserApp!=NULL)
      VARIANT_BOOL result;
      HRESULT hr=m_pBrowserApp->get_StatusBar(&result);
      XASSERT(SUCCEEDED(hr))
      if(FAILED(hr))return FALSE;
      return result;
    }

    VOID SetStatusBar(BOOL bNewValue){
      XASSERT(m_pBrowserApp!=NULL)
      m_pBrowserApp->put_StatusBar((bNewValue)?VARIANT_TRUE:VARIANT_FALSE);
    }

    BOOL GetRegisterAsBrowser() const {
      XASSERT(m_pBrowserApp!=NULL)
      VARIANT_BOOL result;
      HRESULT hr=m_pBrowserApp->get_RegisterAsBrowser(&result);
      XASSERT(SUCCEEDED(hr))
      if(FAILED(hr))return FALSE;
      return result;
    }

    VOID SetRegisterAsBrowser(BOOL bNewValue){
      XASSERT(m_pBrowserApp!=NULL)
      m_pBrowserApp->put_RegisterAsBrowser((bNewValue)?VARIANT_TRUE:VARIANT_FALSE);
    }

    BOOL GetRegisterAsDropTarget() const {
      XASSERT(m_pBrowserApp!=NULL)
      VARIANT_BOOL result;
      HRESULT hr=m_pBrowserApp->get_RegisterAsDropTarget(&result);//see exdisp.h (line 197 aprox) in case of error...
      XASSERT(SUCCEEDED(hr))
      if(FAILED(hr))return FALSE;
      return result;
    }

    VOID SetRegisterAsDropTarget(BOOL bNewValue){
      XASSERT(m_pBrowserApp!=NULL)
      m_pBrowserApp->put_RegisterAsDropTarget((bNewValue)?VARIANT_TRUE:VARIANT_FALSE);
    }

    BOOL GetTheaterMode() const {
      XASSERT(m_pBrowserApp!=NULL)
      VARIANT_BOOL result;
      HRESULT hr=m_pBrowserApp->get_TheaterMode(&result);
      XASSERT(SUCCEEDED(hr))
      if(FAILED(hr))return FALSE;
      return result;
    }

    VOID SetTheaterMode(BOOL bNewValue){
      XASSERT(m_pBrowserApp!=NULL)
      m_pBrowserApp->put_TheaterMode((bNewValue)?VARIANT_TRUE:VARIANT_FALSE);
    }

    BOOL GetAddressBar() const {
      XASSERT(m_pBrowserApp!=NULL)
      VARIANT_BOOL result;
      HRESULT hr=m_pBrowserApp->get_AddressBar(&result);
      XASSERT(SUCCEEDED(hr))
      if(FAILED(hr))return FALSE;
      return result;
    }

    VOID SetAddressBar(BOOL bNewValue){
      XASSERT(m_pBrowserApp!=NULL)
      m_pBrowserApp->put_AddressBar((bNewValue)?VARIANT_TRUE:VARIANT_FALSE);
    }

    BOOL GetProperty(LPCTSTR lpszProperty,LPWSTR buffer,UINT sz){
      XASSERT(m_pBrowserApp!=NULL)
      BSTR bstrProperty=XComBSTR(lpszProperty);
      BOOL bResult=FALSE;
      VARIANT vReturn;
      vReturn.vt=VT_BSTR;
      vReturn.bstrVal=NULL;
      HRESULT hr=m_pBrowserApp->GetProperty(bstrProperty,&vReturn);
      if(SUCCEEDED(hr)){
        wcsncpy(buffer,vReturn.bstrVal,sz);
        buffer[sz]=0;
        bResult=TRUE;
      }
      ::SysFreeString(bstrProperty);
      return bResult;
    }

    VARIANT GetProperty(LPCTSTR lpszProperty){
      VARIANT result;
      static WCHAR parms[]=VTS_BSTR;
      InvokeHelper(0x12f,DISPATCH_METHOD,VT_VARIANT,(void*)&result,parms,lpszProperty);
      return result;
    }

    VOID PutProperty(LPCTSTR lpszProperty,const VARIANT& vtValue){
      XASSERT(m_pBrowserApp!=NULL)
      BSTR bstrProp=XComBSTR(lpszProperty);
      m_pBrowserApp->PutProperty(bstrProp, vtValue);
      ::SysFreeString(bstrProp);
    }

    VOID PutProperty(LPCTSTR lpszPropertyName,LONG lValue){
      XASSERT(m_pBrowserApp!=NULL)
      PutProperty(lpszPropertyName,XOleVariant(lValue,VT_UI4));
    }

    VOID PutProperty(LPCTSTR lpszPropertyName,SHORT iValue){
      XASSERT(m_pBrowserApp!=NULL)
      PutProperty(lpszPropertyName,XOleVariant(iValue,VT_UI2));
    }

    VOID PutProperty(LPCTSTR lpszPropertyName,LPCTSTR lpszValue){
      XASSERT(m_pBrowserApp!=NULL)
      PutProperty(lpszPropertyName,XOleVariant(lpszValue));
    }

    VOID PutProperty(LPCTSTR lpszPropertyName,double dValue){
      XASSERT(m_pBrowserApp!=NULL)
      PutProperty(lpszPropertyName,XOleVariant(dValue));
    }

    BOOL LoadFromResource(HINSTANCE hInstance,LPCTSTR lpszResource){
      BOOL bRetVal=FALSE;
      TCHAR szModule[MAX_PATH];
      if(GetModuleFileName(hInstance,szModule,MAX_PATH)){
        TCHAR szUrl[2048];
        _stprintf(szUrl,_T("res://%s/%s"),szModule,lpszResource);
        Navigate(szUrl,0,0,0);
        bRetVal=TRUE;
      }
      return bRetVal;
    }

    VOID GoBack(){
      XASSERT(m_pBrowserApp!=NULL)
      m_pBrowserApp->GoBack();
    }

    VOID GoForward(){
      XASSERT(m_pBrowserApp!=NULL)
      m_pBrowserApp->GoForward();
    }

    VOID GoHome(){
      XASSERT(m_pBrowserApp!=NULL)
      m_pBrowserApp->GoHome();
    }

    VOID GoSearch(){
      XASSERT(m_pBrowserApp!=NULL)
      m_pBrowserApp->GoSearch();
    }

    VOID Refresh(){
      XASSERT(m_pBrowserApp!=NULL)
      m_pBrowserApp->Refresh();
    }

    VOID Refresh2(LONG nLevel){
      XASSERT(m_pBrowserApp!=NULL)
      XOleVariant vReturn(nLevel,VT_UI4);
      m_pBrowserApp->Refresh2(&vReturn);
    }

    VOID Stop(){
      XASSERT(m_pBrowserApp!=NULL)
      m_pBrowserApp->Stop();
    }

    OLECMDF QueryStatusWB(OLECMDID cmdID) const {
      XASSERT(m_pBrowserApp!=NULL)
      OLECMDF result;
      HRESULT hr=m_pBrowserApp->QueryStatusWB(cmdID,&result);
      XASSERT(SUCCEEDED(hr))
      XUNUSED(hr)
      return result;
    }

    VOID ExecWB(OLECMDID cmdID, OLECMDEXECOPT cmdexecopt,VARIANT* pvaIn, VARIANT* pvaOut){
      XASSERT(m_pBrowserApp!=NULL)
      m_pBrowserApp->ExecWB(cmdID, cmdexecopt, pvaIn, pvaOut);
    }

    HRESULT QueryFormsCommand(DWORD dwCommandID,BOOL* pbSupported, BOOL* pbEnabled, BOOL* pbChecked){
      XASSERT(m_pBrowserApp!=NULL)
      HRESULT hr=E_FAIL;
      XComPtr<IDispatch,IID_IDispatch> spDispDocument;
      m_pBrowserApp->get_Document(&spDispDocument);
      XComQIPtr<IHTMLDocument2,IID_IHTMLDocument2> spDoc(spDispDocument);
      if(spDoc!=NULL){
        XComQIPtr<IOleCommandTarget,IID_IOleCommandTarget> spCmdTarget(spDoc);
        if(spCmdTarget!=NULL){
          OLECMD cmdInfo;
          cmdInfo.cmdID=dwCommandID;
          cmdInfo.cmdf=0;
          hr=spCmdTarget->QueryStatus(&CMDSETID_Forms3,1,&cmdInfo,NULL);
          if(SUCCEEDED(hr)){
            if(pbSupported!=NULL)
              *pbSupported=(cmdInfo.cmdf&OLECMDF_SUPPORTED)?TRUE:FALSE;
            if(pbEnabled!=NULL)
              *pbEnabled=(cmdInfo.cmdf&OLECMDF_ENABLED)?TRUE:FALSE;
            if(pbChecked!=NULL)
              *pbChecked=(cmdInfo.cmdf&OLECMDF_LATCHED)?TRUE:FALSE;
          }
        }
      }
      return hr;
    }

    HRESULT ExecFormsCommand(DWORD dwCommandID,VARIANT* pVarIn, VARIANT* pVarOut){
      XASSERT(m_pBrowserApp!=NULL)
      HRESULT hr=E_FAIL;
      XComPtr<IDispatch,IID_IDispatch> spDispDocument;
      m_pBrowserApp->get_Document(&spDispDocument);
      XComQIPtr<IHTMLDocument2,IID_IHTMLDocument2> spDoc(spDispDocument);
      if(spDoc!=NULL){
        XComQIPtr<IOleCommandTarget,IID_IOleCommandTarget> spCmdTarget(spDoc);
        if(spCmdTarget!=NULL)
          hr=spCmdTarget->Exec(&CMDSETID_Forms3,dwCommandID,OLECMDEXECOPT_DONTPROMPTUSER,pVarIn,pVarOut);
      }
      return hr;
    }

    HRESULT Navigate(LPCTSTR lpszUrl,DWORD dwFlags=0,LPCTSTR lpszTargetFrameName=NULL,LPCTSTR lpszHeaders=NULL,LPVOID lpvPostData=NULL,DWORD dwPostDataLen=0){
      XComBSTR bstrURL(lpszUrl);
      XOleVariant vPostData;
      if(lpvPostData!=NULL){
        if(dwPostDataLen==0)
          dwPostDataLen=lstrlen((LPCTSTR)lpvPostData);
        vPostData.CreateOneDimArray(VT_UI1,dwPostDataLen,lpvPostData);
      }
      HRESULT hr=m_pBrowserApp->Navigate(bstrURL,XOleVariant((long)dwFlags,VT_I4),XOleVariant(lpszTargetFrameName,VT_BSTR),vPostData,XOleVariant(lpszHeaders,VT_BSTR));
      bstrURL.Free();
      return hr;
    }

    HRESULT Navigate2(LPITEMIDLIST pIDL, DWORD dwFlags=0,LPCTSTR lpszTargetFrameName=NULL){
      XASSERT(m_pBrowserApp!=NULL)
      XOleVariant empty;
      return m_pBrowserApp->Navigate2(XOleVariant(pIDL),XOleVariant((long)dwFlags,VT_I4),XOleVariant(lpszTargetFrameName,VT_BSTR),empty,empty);
    }

    HRESULT Navigate2(LPCTSTR lpszURL, DWORD dwFlags=0,LPCTSTR lpszTargetFrameName=NULL,LPCTSTR lpszHeaders=NULL,LPVOID lpvPostData=NULL,DWORD dwPostDataLen=0){
      XASSERT(m_pBrowserApp!=NULL)
      XOleVariant vPostData;
      if(lpvPostData!=NULL){
        if(dwPostDataLen==0)
          dwPostDataLen=lstrlen((LPCTSTR)lpvPostData);
        vPostData.CreateOneDimArray(VT_UI1,dwPostDataLen,lpvPostData);
      }
      return m_pBrowserApp->Navigate2(XOleVariant(lpszURL,VT_BSTR),XOleVariant((long)dwFlags,VT_I4),XOleVariant(lpszTargetFrameName,VT_BSTR),vPostData,XOleVariant(lpszHeaders,VT_BSTR));
    }

    HRESULT Navigate2(LPCTSTR lpszURL,DWORD dwFlags,PBYTE bPostData,INT_PTR dwPostDataLen,LPCTSTR lpszTargetFrameName=NULL,LPCTSTR lpszHeaders=NULL){
      XASSERT(m_pBrowserApp!=NULL)
      return m_pBrowserApp->Navigate2(XOleVariant(lpszURL,VT_BSTR),XOleVariant((long)dwFlags,VT_I4),XOleVariant(lpszTargetFrameName,VT_BSTR),XOleVariant(bPostData,dwPostDataLen),XOleVariant(lpszHeaders,VT_BSTR));
    }

    VOID SetCharset(LPTSTR buffer);
    BOOL GetCurrentCharset(LPTSTR buffer,UINT sz);
    BOOL GetTitle(LPTSTR buffer,UINT sz);
    BOOL EnableScrollBar(BOOL bEnable=TRUE);
    BOOL SetHtmlContent(LPCTSTR string);

    virtual BOOL PreTranslateMessage(MSG* pMsg);

  protected:

    // IDocHostUIHandler Events
    virtual HRESULT OnGetExternal(LPDISPATCH*);
    virtual HRESULT OnShowContextMenu(DWORD, LPPOINT, LPUNKNOWN, LPDISPATCH);
    virtual HRESULT OnGetHostInfo(DOCHOSTUIINFO*);
    virtual HRESULT OnShowUI(DWORD,LPOLEINPLACEACTIVEOBJECT,LPOLECOMMANDTARGET, LPOLEINPLACEFRAME, LPOLEINPLACEUIWINDOW);
    virtual HRESULT OnHideUI();
    virtual HRESULT OnUpdateUI();
    virtual HRESULT OnEnableModeless(BOOL);
    virtual HRESULT OnDocWindowActivate(BOOL);
    virtual HRESULT OnFrameWindowActivate(BOOL);
    virtual HRESULT OnResizeBorder(LPCRECT, LPOLEINPLACEUIWINDOW, BOOL);
    virtual HRESULT OnTranslateAccelerator(LPMSG, const GUID*, DWORD);
    virtual HRESULT OnGetOptionKeyPath(LPOLESTR*, DWORD);
    virtual HRESULT OnGetDropTarget(LPDROPTARGET, LPDROPTARGET*);
    virtual HRESULT OnTranslateUrl(DWORD, OLECHAR*, OLECHAR**);
    virtual HRESULT OnFilterDataObject(LPDATAOBJECT pDataObject,LPDATAOBJECT* ppDataObject);

    //Web Browser Events...
    virtual VOID OnBeforeNavigate2(BSTR URL,DWORD nFlags,BSTR TargetFrameName,VARIANT* PostData,BSTR Headers,BOOL* pbCancel);
    virtual VOID OnNavigateComplete2(BSTR URL);
    virtual VOID OnDocumentComplete(BSTR URL);

    //IDispatch Web Browser Events...
    VOID BeforeNavigate2(LPDISPATCH pDisp,VARIANT* URL,VARIANT* Flags, VARIANT* TargetFrameName,VARIANT* PostData,VARIANT* Headers,BOOL* Cancel);
    VOID NavigateComplete2(LPDISPATCH pDisp,VARIANT* URL);
    VOID DocumentComplete(LPDISPATCH pDisp, VARIANT* URL);

  public:
    // Interface maps
    BEGIN_INTERNAL_INTERFACE(XHtmlCtrl,DocHostUIHandler,IDocHostUIHandler)
      STDMETHOD(ShowContextMenu)(DWORD, LPPOINT, LPUNKNOWN, LPDISPATCH);
      STDMETHOD(GetHostInfo)(DOCHOSTUIINFO*);
      STDMETHOD(ShowUI)(DWORD, LPOLEINPLACEACTIVEOBJECT,LPOLECOMMANDTARGET, LPOLEINPLACEFRAME, LPOLEINPLACEUIWINDOW);
      STDMETHOD(HideUI)(void);
      STDMETHOD(UpdateUI)(void);
      STDMETHOD(EnableModeless)(BOOL);
      STDMETHOD(OnDocWindowActivate)(BOOL);
      STDMETHOD(OnFrameWindowActivate)(BOOL);
      STDMETHOD(ResizeBorder)(LPCRECT, LPOLEINPLACEUIWINDOW, BOOL);
      STDMETHOD(TranslateAccelerator)(LPMSG, const GUID*, DWORD);
      STDMETHOD(GetOptionKeyPath)(OLECHAR **, DWORD);
      STDMETHOD(GetDropTarget)(LPDROPTARGET, LPDROPTARGET*);
      STDMETHOD(GetExternal)(LPDISPATCH*);
      STDMETHOD(TranslateUrl)(DWORD, OLECHAR*, OLECHAR **);
      STDMETHOD(FilterDataObject)(LPDATAOBJECT , LPDATAOBJECT*);
    END_INTERNAL_INTERFACE(XHtmlCtrl,DocHostUIHandler)

  private:
    XComPtr<IWebBrowser2,IID_IWebBrowser2> m_pBrowserApp;

  DECLARE_INTERFACE_EVENT_TABLE()
  DECLARE_EVENT_TABLE()
  DECLARE_EVENT_SINK_TABLE()
};

#endif //_XHTMLCTRL_H_
