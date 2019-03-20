#include "xhtmlctrl.h"

//not defined in mingw 3.17
XDEFINE_GUID(IID_IDocHostUIHandler,0xbd3f23c0,0xd43e,0x11CF,0x89,0x3b, 0x00, 0xaa, 0x00, 0xbd, 0xce, 0x1a);

BEGIN_INTERFACE_EVENTS(XHtmlCtrl,XOleControl)
  INTERFACE_EVENT(XHtmlCtrl,IID_IDocHostUIHandler,DocHostUIHandler)
END_INTERFACE_EVENTS()

//Definición de Eventos a Interceptar...
BEGIN_EVENT_TABLE(XHtmlCtrl,XOleControl)
END_EVENT_TABLE()

BEGIN_SINK_EVENTS(XHtmlCtrl,XOleControl)
  ON_EVENT(XTHISCTRLID,DISPID_BEFORENAVIGATE2,XHtmlCtrl::BeforeNavigate2,VTS_DISPATCH VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PBOOL)
  ON_EVENT(XTHISCTRLID,DISPID_NAVIGATECOMPLETE2,XHtmlCtrl::NavigateComplete2,VTS_DISPATCH VTS_PVARIANT)
  ON_EVENT(XTHISCTRLID,DISPID_DOCUMENTCOMPLETE,XHtmlCtrl::DocumentComplete,VTS_DISPATCH VTS_PVARIANT)
END_SINK_EVENTS()

IMPLEMENT_DYNCREATE(XHtmlCtrl,XOleControl)

XHtmlCtrl::~XHtmlCtrl(){
  if(m_pBrowserApp.IsValid())
    m_pBrowserApp.Release();
}

BOOL XHtmlCtrl::PreTranslateMessage(MSG* pMsg){
	XASSERT(pMsg!=NULL)
	if(XOleControl::PreTranslateMessage(pMsg))
		return TRUE;
	if(m_pBrowserApp!=NULL){
		XComQIPtr<IOleInPlaceActiveObject,IID_IOleInPlaceActiveObject> spInPlace(m_pBrowserApp);
		if(spInPlace)
			return (spInPlace->TranslateAccelerator(pMsg)==S_OK)?TRUE:FALSE;
	}
	return FALSE;
}

HWND XHtmlCtrl::Create(HWND hWndParent,LPCTSTR lpWindowName,UINT CtrlId,const RECT& rect,DWORD dwStyle){
  return CreateEx(0,hWndParent,lpWindowName,CtrlId,rect,dwStyle);
}

HWND XHtmlCtrl::CreateEx(DWORD dwExStyle,HWND hWndParent,LPCTSTR lpWindowName,UINT CtrlId,const RECT& rect,DWORD dwStyle){
  if(!XOleControl::CreateEx(dwExStyle,hWndParent,lpWindowName,CLSID_WebBrowser,CtrlId,rect,dwStyle))
    return NULL;
  LPUNKNOWN lpUnk=GetControlUnknown();
  if(!SUCCEEDED(lpUnk->QueryInterface(IID_IWebBrowser2,(LPVOID*)&m_pBrowserApp))){
		DestroyWindow();
		return NULL;
	}
  return m_hWnd;
}

BOOL XHtmlCtrl::DestroyWindow(){
  if(m_pBrowserApp.IsValid())
    m_pBrowserApp.Release();
  return XOleControl::DestroyWindow();
}

VOID XHtmlCtrl::SetCharset(LPTSTR buffer){
	IHTMLDocument2* lpTarget=NULL;
	LPDISPATCH lpDisp=GetHtmlDocument();
	if(lpDisp!=NULL){
		if(SUCCEEDED(lpDisp->QueryInterface(IID_IHTMLDocument2,(LPVOID*)&lpTarget))){
      BSTR bstr=XComBSTR(buffer);
      lpTarget->put_defaultCharset(bstr);
			lpTarget->put_charset(bstr);
			lpTarget->Release();
			::SysFreeString(bstr);
		}
		lpDisp->Release();
	}
}

BOOL XHtmlCtrl::GetCurrentCharset(LPTSTR buffer,UINT sz){
	BOOL bRet=FALSE;
	IHTMLDocument2* pHTMLDoc2=NULL;
	LPDISPATCH lpDisp=GetHtmlDocument();
	XASSERT(buffer!=NULL&&sz>0)
	if(lpDisp!=NULL){
		if(SUCCEEDED(lpDisp->QueryInterface(IID_IHTMLDocument2,(LPVOID*)&pHTMLDoc2))){
      BSTR cs=XComBSTR(sz);
			pHTMLDoc2->get_charset(&cs);
			pHTMLDoc2->Release();
			lstrcpyn(buffer,CBSTR2TSTR(cs),sz);
      ::SysFreeString(cs);
			bRet=TRUE;
		}
		lpDisp->Release();
	}
	return bRet;
}

BOOL XHtmlCtrl::GetTitle(LPTSTR buffer,UINT sz){
  BOOL bRet=FALSE;
	IHTMLDocument2* pHTMLDoc2=NULL;
	LPDISPATCH lpDisp=GetHtmlDocument();
  XASSERT(buffer!=NULL&&sz>0)
	if(lpDisp!=NULL){
		if(SUCCEEDED(lpDisp->QueryInterface(IID_IHTMLDocument2,(LPVOID*)&pHTMLDoc2))){
      BSTR cs=XComBSTR(sz);
      pHTMLDoc2->get_title(&cs);
      pHTMLDoc2->Release();
      lstrcpyn(buffer,CBSTR2TSTR(cs),sz);
      ::SysFreeString(cs);
      bRet=TRUE;
		}
		lpDisp->Release();
	}
	return bRet;
}

BOOL XHtmlCtrl::EnableScrollBar(BOOL bEnable){
  BOOL bRet=FALSE;
  IHTMLDocument2* pHTMLDoc2=NULL;
	LPDISPATCH lpDisp=GetHtmlDocument();
	if(lpDisp!=NULL){
    if(SUCCEEDED(lpDisp->QueryInterface(IID_IHTMLDocument2,(LPVOID*)&pHTMLDoc2))){
      IHTMLElement *pElement=NULL;
      if(SUCCEEDED(pHTMLDoc2->get_body(&pElement))&&pElement){
        IHTMLBodyElement *pBody=NULL;
        if(SUCCEEDED(pElement->QueryInterface(IID_IHTMLBodyElement,(LPVOID*)&pBody))){
          BSTR cs=XComBSTR(bEnable?L"YES":L"NO");
          bRet=SUCCEEDED(pBody->put_scroll(cs));
          pBody->Release();
          ::SysFreeString(cs);;
        }
        pElement->Release();
      }
      pHTMLDoc2->Release();
    }
    lpDisp->Release();
	}
	return bRet;
}

BOOL XHtmlCtrl::SetHtmlContent(LPCTSTR string){
  BOOL bRet=FALSE;
  if(SUCCEEDED(Navigate(_T("about:blank")))){
    LPDISPATCH lpDisp=GetHtmlDocument();
    if(lpDisp!=NULL){
      IHTMLDocument2* pHTMLDoc2=NULL;
      if(SUCCEEDED(lpDisp->QueryInterface(IID_IHTMLDocument2,(LPVOID*)&pHTMLDoc2))){
        SAFEARRAY* pSafeArray=::SafeArrayCreateVector(VT_VARIANT,0,1);
        if(pSafeArray){
          VARIANT* pElement=NULL;
          if(SUCCEEDED(::SafeArrayAccessData(pSafeArray,(LPVOID*)&pElement))){
            pElement->vt=VT_BSTR;
            pElement->bstrVal=XComBSTR(string);
            if(SUCCEEDED(::SafeArrayUnaccessData(pSafeArray))){
              pHTMLDoc2->clear();
              pHTMLDoc2->close();
              if(SUCCEEDED(pHTMLDoc2->write(pSafeArray)))
                bRet=TRUE;
            }
            ::SysFreeString(pElement->bstrVal);
          }
          ::SafeArrayDestroy(pSafeArray);
        }
        pHTMLDoc2->Release();
      }
      lpDisp->Release();
    }
	}
	return bRet;
}


// IDocHostUIHandler Events

HRESULT XHtmlCtrl::OnGetExternal(LPDISPATCH*){
	return S_FALSE;
}

HRESULT XHtmlCtrl::OnShowContextMenu(DWORD, LPPOINT, LPUNKNOWN, LPDISPATCH){
	return S_FALSE;
}

HRESULT XHtmlCtrl::OnGetHostInfo(DOCHOSTUIINFO*){
	return S_OK;
}

HRESULT XHtmlCtrl::OnShowUI(DWORD,LPOLEINPLACEACTIVEOBJECT,LPOLECOMMANDTARGET, LPOLEINPLACEFRAME, LPOLEINPLACEUIWINDOW){
	return S_FALSE;
}

HRESULT XHtmlCtrl::OnHideUI(){
	return S_OK;
}

HRESULT XHtmlCtrl::OnUpdateUI(){
	return S_OK;
}

HRESULT XHtmlCtrl::OnEnableModeless(BOOL){
	return S_OK;
}

HRESULT XHtmlCtrl::OnDocWindowActivate(BOOL){
	return S_OK;
}

HRESULT XHtmlCtrl::OnFrameWindowActivate(BOOL){
	return S_OK;
}

HRESULT XHtmlCtrl::OnResizeBorder(LPCRECT, LPOLEINPLACEUIWINDOW, BOOL){
	return S_OK;
}

HRESULT XHtmlCtrl::OnTranslateAccelerator(LPMSG, const GUID*, DWORD){
	return S_FALSE;
}

HRESULT XHtmlCtrl::OnGetOptionKeyPath(LPOLESTR*, DWORD){
	return S_FALSE;
}

HRESULT XHtmlCtrl::OnGetDropTarget(LPDROPTARGET, LPDROPTARGET*){
	return S_FALSE;
}

HRESULT XHtmlCtrl::OnTranslateUrl(DWORD, OLECHAR*, OLECHAR**){
	return S_FALSE;
}

HRESULT XHtmlCtrl::OnFilterDataObject(LPDATAOBJECT /*pDataObject*/,LPDATAOBJECT* /*ppDataObject*/){
	return S_FALSE;
}


// XHtmlCtrl::XDocHostUIHandler

STDMETHODIMP_(ULONG) XHtmlCtrl::XDocHostUIHandler::AddRef(){
	return (ULONG)GetParentPtr()->InternalAddRef();
}

STDMETHODIMP_(ULONG) XHtmlCtrl::XDocHostUIHandler::Release(){
	return (ULONG)GetParentPtr()->InternalRelease();
}

STDMETHODIMP XHtmlCtrl::XDocHostUIHandler::QueryInterface(REFIID iid, LPVOID far* ppvObj){
	return GetParentPtr()->InternalQueryInterface(&iid,ppvObj);
}

STDMETHODIMP XHtmlCtrl::XDocHostUIHandler::GetExternal(LPDISPATCH *lppDispatch){
  return GetParentPtr()->OnGetExternal(lppDispatch);
}

STDMETHODIMP XHtmlCtrl::XDocHostUIHandler::ShowContextMenu(DWORD dwID, LPPOINT ppt, LPUNKNOWN pcmdtReserved, LPDISPATCH pdispReserved){
  return GetParentPtr()->OnShowContextMenu(dwID, ppt, pcmdtReserved, pdispReserved);
}

STDMETHODIMP XHtmlCtrl::XDocHostUIHandler::GetHostInfo(DOCHOSTUIINFO *pInfo){
	return GetParentPtr()->OnGetHostInfo(pInfo);
}

STDMETHODIMP XHtmlCtrl::XDocHostUIHandler::ShowUI(DWORD dwID, LPOLEINPLACEACTIVEOBJECT pActiveObject,LPOLECOMMANDTARGET pCommandTarget, LPOLEINPLACEFRAME pFrame,LPOLEINPLACEUIWINDOW pDoc){
	return GetParentPtr()->OnShowUI(dwID, pActiveObject, pCommandTarget, pFrame, pDoc);
}

STDMETHODIMP XHtmlCtrl::XDocHostUIHandler::HideUI(){
	return GetParentPtr()->OnHideUI();
}

STDMETHODIMP XHtmlCtrl::XDocHostUIHandler::UpdateUI(){
	return GetParentPtr()->OnUpdateUI();
}

STDMETHODIMP XHtmlCtrl::XDocHostUIHandler::EnableModeless(BOOL fEnable){
	return GetParentPtr()->OnEnableModeless(fEnable);
}

STDMETHODIMP XHtmlCtrl::XDocHostUIHandler::OnDocWindowActivate(BOOL fActivate){
	return GetParentPtr()->OnDocWindowActivate(fActivate);
}

STDMETHODIMP XHtmlCtrl::XDocHostUIHandler::OnFrameWindowActivate(BOOL fActivate){
	return GetParentPtr()->OnFrameWindowActivate(fActivate);
}

STDMETHODIMP XHtmlCtrl::XDocHostUIHandler::ResizeBorder(LPCRECT prcBorder, LPOLEINPLACEUIWINDOW pUIWindow, BOOL fFrameWindow){
	return GetParentPtr()->OnResizeBorder(prcBorder, pUIWindow, fFrameWindow);
}

STDMETHODIMP XHtmlCtrl::XDocHostUIHandler::TranslateAccelerator(LPMSG lpMsg, const GUID* pguidCmdGroup, DWORD nCmdID){
	return GetParentPtr()->OnTranslateAccelerator(lpMsg, pguidCmdGroup, nCmdID);
}

STDMETHODIMP XHtmlCtrl::XDocHostUIHandler::GetOptionKeyPath(LPOLESTR* pchKey, DWORD dwReserved){
	return GetParentPtr()->OnGetOptionKeyPath(pchKey, dwReserved);
}

STDMETHODIMP XHtmlCtrl::XDocHostUIHandler::GetDropTarget(LPDROPTARGET pDropTarget, LPDROPTARGET* ppDropTarget){
	return GetParentPtr()->OnGetDropTarget(pDropTarget, ppDropTarget);
}

STDMETHODIMP XHtmlCtrl::XDocHostUIHandler::TranslateUrl(DWORD dwTranslate, OLECHAR* pchURLIn, OLECHAR** ppchURLOut){
	return GetParentPtr()->OnTranslateUrl(dwTranslate, pchURLIn, ppchURLOut);
}

STDMETHODIMP XHtmlCtrl::XDocHostUIHandler::FilterDataObject(LPDATAOBJECT pDataObject, LPDATAOBJECT* ppDataObject){
	return GetParentPtr()->OnFilterDataObject(pDataObject, ppDataObject);
}


// XHtmlCtrl::Events (WebBrowser Events)

VOID XHtmlCtrl::OnBeforeNavigate2(BSTR /*URL*/,DWORD /*nFlags*/,BSTR /*TargetFrameName*/,VARIANT* /*PostData*/,BSTR /*Headers*/,BOOL* /*pbCancel*/){
  //should be catched by owner...
}

VOID XHtmlCtrl::OnNavigateComplete2(BSTR /*URL*/){
  //should be catched by owner...
}

VOID XHtmlCtrl::OnDocumentComplete(BSTR /*URL*/){
  //should be catched by owner...
}


// XHtmlCtrl::DispathEvents (WebBrowser Events)

VOID XHtmlCtrl::BeforeNavigate2(LPDISPATCH /*pDisp*/,VARIANT* URL,VARIANT* Flags,VARIANT* TargetFrameName,VARIANT* PostData,VARIANT* Headers,BOOL* Cancel){
  XASSERT(V_VT(URL)==VT_BSTR)
	XASSERT(V_VT(TargetFrameName)==VT_BSTR)
	XASSERT(V_VT(PostData)==(VT_VARIANT|VT_BYREF))
	XASSERT(V_VT(Headers)==VT_BSTR)
	XASSERT(Cancel!=NULL)
	BOOL bCancel=FALSE;
	OnBeforeNavigate2(V_BSTR(URL),V_I4(Flags),V_BSTR(TargetFrameName),V_VARIANTREF(PostData),V_BSTR(Headers),&bCancel);
  *Cancel=(bCancel?VARIANT_TRUE:VARIANT_FALSE);
}

VOID XHtmlCtrl::NavigateComplete2(LPDISPATCH /*pDisp*/,VARIANT* URL){
  XASSERT(V_VT(URL)==VT_BSTR)
  OnNavigateComplete2(V_BSTR(URL));
}

VOID XHtmlCtrl::DocumentComplete(LPDISPATCH /*pDisp*/, VARIANT* URL){
	XASSERT(V_VT(URL)==VT_BSTR)
	OnDocumentComplete(V_BSTR(URL));
}
