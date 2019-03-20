#include "xolectrl.h"

IMPLEMENT_DYNCREATE(XOleControl,XWnd)

//Definición de Eventos a Interceptar...
BEGIN_EVENT_TABLE(XOleControl,XWnd)
END_EVENT_TABLE()

#ifdef __GNUC__
//not defined in mingw 3.17
XDEFINE_GUID(IID_IOleInPlaceSiteWindowless,0x922eada0,0x3424,0x11cf,0xb6,0x70,0,0xaa,0,0x4c,0xd6,0xd8);
XDEFINE_GUID(IID_IOleInPlaceObjectWindowless,0x1c2056cc,0x5ef4,0x101b,0x8b,0xc8,0,0xaa,0,0x3e,0x3b,0x29);
#endif

BEGIN_INTERFACE_EVENTS(XOleControl,XWnd)
  INTERFACE_EVENT(XOleControl,IID_IOleClientSite,OleClientSite)
	INTERFACE_EVENT(XOleControl,IID_IOleInPlaceSite,OleIPSite)
  INTERFACE_EVENT(XOleControl,IID_IOleInPlaceSiteEx,OleIPSite)
  INTERFACE_EVENT(XOleControl,IID_IOleInPlaceSiteWindowless,OleIPSite)
  INTERFACE_EVENT(XOleControl,IID_IOleInPlaceFrame,OleIPFrame)
	INTERFACE_EVENT(XOleControl,IID_IOleContainer,OleContainer)
	INTERFACE_EVENT(XOleControl,IID_IOleControlSite,OleControlSite)
	INTERFACE_EVENT(XOleControl,IID_IDispatch,AmbientProps)
END_INTERFACE_EVENTS()

XOleControl::XOleControl(HWND hwnd):XWnd(hwnd){
  m_bCreated=FALSE;
  m_dwMiscStatus=0;
  m_dwEventSink=0;
  m_dwPropNotifySink=0;
  m_dwStyle=0;
  m_dwStyleMask=WS_GROUP|WS_TABSTOP;
  m_dwExStyle=0;
  m_dwExStyleMask=WS_EX_CLIENTEDGE;
  m_hDC=NULL;
}

XOleControl::~XOleControl(){
  if(m_bCreated)
    DestroyWindow();
}

BOOL XOleControl::PreTranslateMessage(LPMSG lpMsg){
  if(m_pWndParent!=NULL&&m_pWndParent->PreTranslateMessage(lpMsg))
    return TRUE;
	return XWnd::PreTranslateMessage(lpMsg);
}

HWND XOleControl::Create(HWND hWndParent,LPCTSTR lpszWindowName,LPCTSTR lpszClsidName,UINT CtrlId,
  const RECT& rect,DWORD dwStyle,BSTR bstrLicKey,LPVOID lpParam){
  return CreateEx(0,hWndParent,lpszWindowName,lpszClsidName,CtrlId,rect,dwStyle,bstrLicKey,lpParam);
}

HWND XOleControl::Create(HWND hWndParent,LPCTSTR lpszWindowName,REFCLSID clsid,UINT CtrlId,const RECT& rect,DWORD dwStyle,BSTR bstrLicKey,LPVOID lpParam){
  return CreateEx(0,hWndParent,lpszWindowName,clsid,CtrlId,rect,dwStyle,bstrLicKey,lpParam);
}

HWND XOleControl::CreateEx(DWORD dwExStyle,HWND hWndParent,LPCTSTR lpszWindowName,LPCTSTR lpszClsidName,UINT CtrlId,
  const RECT& rect,DWORD dwStyle,BSTR bstrLicKey,LPVOID lpParam){
  CLSID clsid;
  if(FAILED(CLSIDFromProgID(TSTR2WSTR(lpszClsidName),&clsid)))
    return NULL;
  return CreateEx(dwExStyle,hWndParent,lpszWindowName,clsid,CtrlId,rect,dwStyle,bstrLicKey,lpParam);
}

HWND XOleControl::CreateEx(DWORD dwExStyle,HWND hWndParent,LPCTSTR lpszWindowName,REFCLSID clsid,UINT CtrlId,const RECT& rect,DWORD dwStyle,BSTR bstrLicKey,LPVOID /*lpParam*/){
  //parent window need to be valid and XWnd Object...
  XASSERT(m_hWnd==NULL&&hWndParent!=NULL)
  m_pWndParent=FromHandlePermanent(hWndParent);
  XASSERT(m_pWndParent!=NULL)

  //mark parent as ole container
  if(!(m_pWndParent->m_nFlags&XWF_OLECTLCONTAINER))
    m_pWndParent->m_nFlags|=XWF_OLECTLCONTAINER;

  //init ole...
  if(!XWinApp::OleInit())
    return NULL;

  //create control interface...
  HRESULT hr=E_FAIL;
  if(!::SysStringLen(bstrLicKey)){
		LPCLASSFACTORY pClassFactory=NULL;
		if(SUCCEEDED(hr=CoGetClassObject(clsid,CLSCTX_INPROC_SERVER|CLSCTX_INPROC_HANDLER,NULL,IID_IClassFactory,(void**)&pClassFactory))){
			XASSERT(pClassFactory!=NULL)
			hr=pClassFactory->CreateInstance(NULL,IID_IOleObject,(void**)&m_pObject);
			pClassFactory->Release();
		}
	}else{
		LPCLASSFACTORY2 pClassFactory=NULL;
		if(SUCCEEDED(hr=CoGetClassObject(clsid,CLSCTX_INPROC_SERVER|CLSCTX_INPROC_HANDLER,NULL,IID_IClassFactory2,(void**)&pClassFactory))){
			XASSERT(pClassFactory!=NULL)
			hr=pClassFactory->CreateInstanceLic(NULL,NULL,IID_IOleObject,bstrLicKey,(void**)&m_pObject);
			pClassFactory->Release();
		}
	}

	XASSERT(m_pObject!=NULL)
	if(FAILED(hr)||m_pObject==NULL)
    return NULL;

  //try quick activate...
  GetEventIID(&m_iidEvents);
  BOOL bQuickActivated=QuickActivate();

  if(!bQuickActivated){
		m_pObject->GetMiscStatus(DVASPECT_CONTENT,&m_dwMiscStatus);
		if(m_dwMiscStatus&OLEMISC_SETCLIENTSITEFIRST&&FAILED(hr=m_pObject->SetClientSite(&m_OleClientSite))){
      m_pObject->Close(OLECLOSE_NOSAVE);
      m_pObject->Release();
      m_pObject=NULL;
      return NULL;
		}
	}

	LPPERSISTSTREAMINIT pPersStm=NULL;
	LPPERSISTSTORAGE pPersStg=NULL;
	if(SUCCEEDED(m_pObject->QueryInterface(IID_IPersistStreamInit,(void**)&pPersStm))){// initialize via IPersistStreamInit
		XASSERT(pPersStm!=NULL)
		hr=pPersStm->InitNew();
		pPersStm->Release();
	}else if (SUCCEEDED(m_pObject->QueryInterface(IID_IPersistStorage,(void**)&pPersStg))){// initialize via IPersistStorage
		XASSERT(pPersStg!=NULL)
    LPLOCKBYTES pLockBytes=NULL;
    if(SUCCEEDED(hr=CreateILockBytesOnHGlobal(NULL,TRUE,&pLockBytes))){
      XASSERT(pLockBytes!=NULL)
      LPSTORAGE pStorage=NULL;
      if(SUCCEEDED(hr=StgCreateDocfileOnILockBytes(pLockBytes,STGM_CREATE|STGM_READWRITE|STGM_SHARE_EXCLUSIVE,0,&pStorage))){
        XASSERT(pStorage!=NULL)
        hr=pPersStg->InitNew(pStorage);
        pStorage->Release();
      }
      pLockBytes->Release();
    }
		pPersStg->Release();
	}else
    hr=E_FAIL;

  if(FAILED(hr)){
    m_pObject->Close(OLECLOSE_NOSAVE);
    m_pObject->Release();
    m_pObject=NULL;
    return NULL;
  }

  if(!bQuickActivated){
		if(!(m_dwMiscStatus&OLEMISC_SETCLIENTSITEFIRST)&&FAILED(hr=m_pObject->SetClientSite(&m_OleClientSite))){
      m_pObject->Close(OLECLOSE_NOSAVE);
      m_pObject->Release();
      m_pObject=NULL;
      return NULL;
    }
	}

  m_nCtrlId=CtrlId;
  CopyRect(&m_rect,&rect);
  SetExtent();

  if(!bQuickActivated){
    m_dwEventSink=ConnectSink(m_iidEvents,&m_EventSink);
    m_dwPropNotifySink=ConnectSink(IID_IPropertyNotifySink,&m_PropertyNotifySink);
  }

  if(SUCCEEDED(hr=m_pObject->QueryInterface(IID_IOleInPlaceObject,(LPVOID*)&m_pInPlaceObject))){
    if(dwStyle&WS_VISIBLE)
      hr=DoVerb(OLEIVERB_INPLACEACTIVATE);
    else{
      OffsetRect(&m_rect,-32000,-32000);
      if(SUCCEEDED(hr=DoVerb(OLEIVERB_INPLACEACTIVATE))&&SUCCEEDED(hr=DoVerb(OLEIVERB_HIDE))){
        OffsetRect(&m_rect,32000, 32000);
        hr=m_pInPlaceObject->SetObjectRects(&m_rect,&m_rect);
      }
    }
  }

  HWND hWnd=NULL;
  if(FAILED(hr)){
    if(m_pInPlaceObject!=NULL){
      m_pInPlaceObject->Release();
      m_pInPlaceObject=NULL;
    }
    DisconnectSink(m_iidEvents,m_dwEventSink);
    DisconnectSink(IID_IPropertyNotifySink,m_dwPropNotifySink);
    m_dwEventSink=0;
    m_dwPropNotifySink=0;
    m_pObject->Release();
    m_pObject=NULL;
    return NULL;
  }

  if(SUCCEEDED(m_pInPlaceObject->GetWindow(&hWnd)))
    Attach(hWnd);

  GetControlInfo();
  if(lpszWindowName!=NULL)
    SetWindowText(lpszWindowName);

  if(m_dwMiscStatus&OLEMISC_INVISIBLEATRUNTIME)
    dwStyle&=~WS_VISIBLE;
  m_dwStyle=dwStyle&m_dwStyleMask;
  ModifyStyle(FALSE,0,m_dwStyle|(dwStyle&(WS_DISABLED|WS_BORDER)),0);
  m_dwExStyle=dwExStyle&m_dwExStyleMask;

  XASSERT(m_hWnd!=NULL||m_bIsWindowless)
  m_pWndParent->m_XWndSiteMap.Add(this,m_hWnd);
  m_bCreated=TRUE;

  return m_hWnd;
}

BOOL XOleControl::Attach(HWND hWnd){
	XASSERT(hWnd!=NULL)
	if(m_hWnd!=hWnd){
    XASSERT(m_hWnd==NULL)
    return XWnd::Attach(hWnd);
  }
  return FALSE;
}

HWND XOleControl::Detach(){
  if(m_pWndParent!=NULL&&m_pWndParent->m_pSiteCapture==this)
    m_pWndParent->m_pSiteCapture=NULL;

  if(m_dwEventSink){
    DisconnectSink(m_iidEvents,m_dwEventSink);
    m_dwEventSink=0;
  }

  if(m_dwPropNotifySink){
    DisconnectSink(IID_IPropertyNotifySink,m_dwPropNotifySink);
    m_dwPropNotifySink=0;
  }

  if(m_pWindowlessObject!=NULL){
	  m_pWindowlessObject->Release();
	  m_pWindowlessObject=NULL;
  }

  if(m_pInPlaceObject!=NULL){
		m_pInPlaceObject->InPlaceDeactivate();
		m_pInPlaceObject->Release();
		m_pInPlaceObject=NULL;
	}

  if(m_pObject!=NULL){
		m_pObject->SetClientSite(NULL);
		m_pObject->Close(OLECLOSE_NOSAVE);
		m_pObject->Release();
		m_pObject=NULL;
	}

	if(m_pWndParent&&m_bIsWindowless){
	  m_pWndParent->m_nWindowlessControls--;
	  XASSERT(m_pWndParent->m_nWindowlessControls>=0)
	}

	m_bCreated=FALSE;
  return XWnd::Detach();
}

BOOL XOleControl::DestroyWindow(){
  if(m_bCreated){
    if(m_pWndParent!=NULL)
      m_pWndParent->m_XWndSiteMap.Remove(this);
    ShowWindow(SW_HIDE);
    Detach();
    return TRUE;
	}
	return FALSE;
}

HRESULT XOleControl::DoVerb(LONG nVerb,LPMSG lpMsg){
	XASSERT(m_pObject!=NULL&&m_pWndParent!=NULL&&m_pWndParent->m_hWnd!=NULL);
	return m_pObject->DoVerb(nVerb,lpMsg,&m_OleClientSite,0,m_pWndParent->m_hWnd,&m_rect);
}

BOOL XOleControl::SetExtent(){
  XASSERT(m_pObject!=NULL)
  HRESULT hr=E_FAIL;
  SIZE sz={m_rect.right-m_rect.left,m_rect.bottom-m_rect.top};
  XClientDC dc(NULL);
  dc.DPtoHIMETRIC(&sz);
	if(SUCCEEDED(hr=m_pObject->SetExtent(DVASPECT_CONTENT,&sz))){
		if(SUCCEEDED(m_pObject->GetExtent(DVASPECT_CONTENT,&sz))){
			dc.HIMETRICtoDP(&sz);
			m_rect.right=m_rect.left+sz.cx;
			m_rect.bottom=m_rect.top+sz.cy;
		}
	}
	return SUCCEEDED(hr);
}

VOID XOleControl::GetControlInfo(){
	memset(&m_ctlInfo,0,sizeof(CONTROLINFO));
	m_ctlInfo.cb=sizeof(CONTROLINFO);
	LPOLECONTROL pOleCtl=NULL;
	XASSERT(m_pObject!=NULL)
	if(SUCCEEDED(m_pObject->QueryInterface(IID_IOleControl,(LPVOID*)&pOleCtl))){
		XASSERT(pOleCtl!=NULL)
		pOleCtl->GetControlInfo(&m_ctlInfo);
		pOleCtl->Release();
	}
}

BOOL XOleControl::GetEventIID(IID* piid){
	*piid=GUID_NULL;
	XASSERT(m_pObject!=NULL)
	LPPROVIDECLASSINFO2 pPCI2=NULL;
	if(SUCCEEDED(m_pObject->QueryInterface(IID_IProvideClassInfo2,(LPVOID*)&pPCI2))){
		XASSERT(pPCI2!=NULL)
		if(SUCCEEDED(pPCI2->GetGUID(GUIDKIND_DEFAULT_SOURCE_DISP_IID,piid))){
			XASSERT(!IsEqualIID(*piid,GUID_NULL))
		}else{
			XASSERT(IsEqualIID(*piid,GUID_NULL))
    }
		pPCI2->Release();
	}
	LPPROVIDECLASSINFO pPCI=NULL;
	if(IsEqualIID(*piid,GUID_NULL)&&SUCCEEDED(m_pObject->QueryInterface(IID_IProvideClassInfo,(LPVOID*)&pPCI))){
		XASSERT(pPCI!=NULL)
		LPTYPEINFO pClassInfo=NULL;
		if(SUCCEEDED(pPCI->GetClassInfo(&pClassInfo))){
			XASSERT(pClassInfo!=NULL)
			LPTYPEATTR pClassAttr;
			if(SUCCEEDED(pClassInfo->GetTypeAttr(&pClassAttr))){
				XASSERT(pClassAttr!=NULL)
				XASSERT(pClassAttr->typekind==TKIND_COCLASS);
				for(UINT i=0;i<pClassAttr->cImplTypes;i++){
          INT nFlags=0;
					if(SUCCEEDED(pClassInfo->GetImplTypeFlags(i,&nFlags))&&((nFlags&IMPLTYPEFLAG_MASK)==IMPLTYPEFLAG_DEFAULTSOURCE)){
						HREFTYPE hRefType;LPTYPEINFO pEventInfo = NULL;
						if(SUCCEEDED(pClassInfo->GetRefTypeOfImplType(i,&hRefType))&&SUCCEEDED(pClassInfo->GetRefTypeInfo(hRefType,&pEventInfo))){
							XASSERT(pEventInfo!=NULL)
							LPTYPEATTR pEventAttr;
							if(SUCCEEDED(pEventInfo->GetTypeAttr(&pEventAttr))){
								XASSERT(pEventAttr!=NULL)
								*piid=pEventAttr->guid;
								pEventInfo->ReleaseTypeAttr(pEventAttr);
							}
							pEventInfo->Release();
						}
						break;
					}
				}
				pClassInfo->ReleaseTypeAttr(pClassAttr);
			}
			pClassInfo->Release();
		}
		pPCI->Release();
	}
	return (!IsEqualIID(*piid,GUID_NULL));
}

BOOL XOleControl::QuickActivate(){
	XASSERT(m_pObject!=NULL)
	BOOL bQuickActivated=FALSE;
	IQuickActivate* pQuick=NULL;
	if(SUCCEEDED(m_pObject->QueryInterface(IID_IQuickActivate,(void**)&pQuick))){
		XASSERT(pQuick!=NULL);
		QACONTAINER qaContainer;
		qaContainer.cbSize=sizeof(QACONTAINER);

		qaContainer.pClientSite=&m_OleClientSite;
		qaContainer.pPropertyNotifySink=&m_PropertyNotifySink;
		qaContainer.pUnkEventSink=&m_EventSink;

		qaContainer.pAdviseSink=NULL;
		qaContainer.pBindHost=NULL;
		qaContainer.pUndoMgr=NULL;
		qaContainer.pOleControlSite=NULL;
		qaContainer.pServiceProvider=NULL;

    VARIANT var;
    qaContainer.dwAmbientFlags=0;
    for(UINT i=0;i<(sizeof(AmbientsFlags)/sizeof(AmbientsFlags[0]));i++){
			if(OnAmbientProperty(AmbientsFlags[i].dwDispID,&var)&&(V_BOOL(&var)))
				qaContainer.dwAmbientFlags|=AmbientsFlags[i].dwFlag;
		}
    OnAmbientProperty(DISPID_AMBIENT_FORECOLOR,&var);
    qaContainer.colorFore=V_I4(&var);
    OnAmbientProperty(DISPID_AMBIENT_BACKCOLOR,&var);
		qaContainer.colorBack=V_I4(&var);
    OnAmbientProperty(DISPID_AMBIENT_APPEARANCE,&var);
		qaContainer.dwAppearance=V_I2(&var);
    OnAmbientProperty(DISPID_AMBIENT_LOCALEID,&var);
		qaContainer.lcid=V_I4(&var);
    OnAmbientProperty(DISPID_AMBIENT_PALETTE,&var);
    qaContainer.hpal=(HPALETTE)V_I4(&var);
    OnAmbientProperty(DISPID_AMBIENT_FONT,&var);
    if(!V_DISPATCH(&var)||FAILED(V_DISPATCH(&var)->QueryInterface(IID_IFont,(void**)&qaContainer.pFont)))
			qaContainer.pFont=NULL;

		QACONTROL qaControl;
		qaControl.cbSize=sizeof(QACONTROL);
		if(SUCCEEDED(pQuick->QuickActivate(&qaContainer,&qaControl))){
			m_dwMiscStatus=qaControl.dwMiscStatus;
			m_dwEventSink=qaControl.dwEventCookie;
			m_dwPropNotifySink=qaControl.dwPropNotifyCookie;
			bQuickActivated=TRUE;
		}
		pQuick->Release();
		if(qaContainer.pFont!=NULL)
			qaContainer.pFont->Release();
	}
	return bQuickActivated;
}

DWORD XOleControl::ConnectSink(REFIID iid, LPUNKNOWN punkSink){
	XASSERT(m_pObject!=NULL)
	LPCONNECTIONPOINTCONTAINER pConnPtCont;
	if((m_pObject!=NULL)&&SUCCEEDED(m_pObject->QueryInterface(IID_IConnectionPointContainer,(LPVOID*)&pConnPtCont))){
		XASSERT(pConnPtCont!=NULL)
		LPCONNECTIONPOINT pConnPt=NULL;
		DWORD dwCookie=0;
		if(SUCCEEDED(pConnPtCont->FindConnectionPoint(iid,&pConnPt))){
			XASSERT(pConnPt!=NULL)
			pConnPt->Advise(punkSink,&dwCookie);
			pConnPt->Release();
		}
		pConnPtCont->Release();
		return dwCookie;
	}
	return 0;
}

VOID XOleControl::DisconnectSink(REFIID iid, DWORD dwCookie){
	if(dwCookie==0||m_pObject==NULL)
		return;
	LPCONNECTIONPOINTCONTAINER pConnPtCont;
	if(SUCCEEDED(m_pObject->QueryInterface(IID_IConnectionPointContainer,(LPVOID*)&pConnPtCont))){
		XASSERT(pConnPtCont!=NULL)
		LPCONNECTIONPOINT pConnPt=NULL;
		if(SUCCEEDED(pConnPtCont->FindConnectionPoint(iid,&pConnPt))){
			XASSERT(pConnPt!=NULL)
			pConnPt->Unadvise(dwCookie);
			pConnPt->Release();
		}
		pConnPtCont->Release();
	}
}

BOOL XOleControl::IsMatchingMnemonic(LPMSG lpMsg){
  XASSERT(lpMsg!=NULL)
  BOOL bMatch=FALSE;
	if((m_ctlInfo.cAccel==0)||(m_ctlInfo.hAccel==NULL))
		return FALSE;
	ACCEL* pAccel=new ACCEL[m_ctlInfo.cAccel];
	INT cAccel=CopyAcceleratorTable(m_ctlInfo.hAccel,pAccel,m_ctlInfo.cAccel);
	XASSERT(cAccel==m_ctlInfo.cAccel)
	for(INT i=0;i<cAccel;i++){
		BOOL fVirt=(lpMsg->message==WM_SYSCHAR?FALT:0);
		WORD key=LOWORD(lpMsg->wParam);
		if(((pAccel[i].fVirt&~FNOINVERT)==fVirt)&&(pAccel[i].key==key)){
			bMatch=TRUE;
			break;
		}
	}
	delete []pAccel;
	return bMatch;
}

VOID XOleControl::SendMnemonic(LPMSG lpMsg){
	XASSERT(m_pObject!=NULL)
	if(!(m_dwMiscStatus&OLEMISC_NOUIACTIVATE))
		SetFocus();
	LPOLECONTROL pOleCtl=NULL;
	if(SUCCEEDED(m_pObject->QueryInterface(IID_IOleControl,(LPVOID*)&pOleCtl))){
		XASSERT(pOleCtl!=NULL)
		pOleCtl->OnMnemonic(lpMsg);
		pOleCtl->Release();
	}
}

VOID XOleControl::FreezeEvents(BOOL bFreeze){
	XASSERT(m_pObject!=NULL)
	LPOLECONTROL pOleCtl=NULL;
	if(SUCCEEDED(m_pObject->QueryInterface(IID_IOleControl,(LPVOID*)&pOleCtl))){
		XASSERT(pOleCtl!=NULL)
		pOleCtl->FreezeEvents(bFreeze);
		pOleCtl->Release();
	}
}

BOOL XOleControl::InitDispDriver()const{
  if(m_dispDriver.IsValid())
    return TRUE;
  const_cast<XOleControl*>(this)->m_dispDriver.FromObject(m_pObject);
  return m_dispDriver.IsValid();
}

BOOL XOleControl::InvokeHelper(DISPID dwDispID,WORD wFlags,VARTYPE vtRet,PVOID pvRet,const WCHAR* pbParamInfo,...){
  BOOL bRet=FALSE;
	va_list argList;
	va_start(argList,pbParamInfo);
  if(InitDispDriver())
    bRet=SUCCEEDED(m_dispDriver.InvokeHelper(dwDispID,wFlags,vtRet,pvRet,pbParamInfo,argList));
	va_end(argList);
	return bRet;
}

BOOL XOleControl::GetProperty(DISPID dwDispID,VARTYPE vtProp,PVOID pvProp)const{
  BOOL bRet=FALSE;
	if(InitDispDriver())
    bRet=SUCCEEDED(m_dispDriver.GetProperty(dwDispID,vtProp,pvProp));
  return bRet;
}

BOOL XOleControl::SetProperty(DISPID dwDispID, VARTYPE vtProp, ...){
  BOOL bRet=FALSE;
	va_list argList;
	va_start(argList, vtProp);
	if(InitDispDriver())
    bRet=SUCCEEDED(m_dispDriver.SetProperty(dwDispID,vtProp,argList));
	va_end(argList);
	return bRet;
}

DWORD XOleControl::GetStyle()const{
  DWORD dwStyle=m_dwStyle;
	if(!m_bIsWindowless)
		dwStyle|=(::GetWindowLong(m_hWnd,GWL_STYLE)&(WS_VISIBLE|WS_DISABLED));
  BOOL bEnabled=TRUE;
	GetProperty(DISPID_ENABLED,VT_BOOL,&bEnabled);
  dwStyle=bEnabled?dwStyle&~WS_DISABLED:dwStyle|WS_DISABLED;
	SHORT sBorderStyle=0;
  GetProperty(DISPID_BORDERSTYLE,VT_I2,&sBorderStyle);
  if(sBorderStyle==1)
    dwStyle|=WS_BORDER;
	return dwStyle;
}

DWORD XOleControl::GetExStyle()const{
  DWORD dwExStyle=0;
  if(!m_bIsWindowless)
    dwExStyle=::GetWindowLong(m_hWnd,GWL_EXSTYLE);
  SHORT sAppearance=0;
  GetProperty(DISPID_APPEARANCE,VT_I2,&sAppearance);
  if(sAppearance==1)
    dwExStyle|=WS_EX_CLIENTEDGE;
	return dwExStyle;
}

BOOL XOleControl::ModifyStyle(BOOL bEx,DWORD dwRemove,DWORD dwAdd,UINT nFlags){
  if(bEx){
    m_dwExStyle=((m_dwStyle&~dwRemove)|dwAdd)&m_dwExStyleMask;
    if((dwRemove&WS_EX_CLIENTEDGE)||(dwAdd&WS_EX_CLIENTEDGE)){
      if(SetProperty(DISPID_APPEARANCE,VT_I2,(dwAdd&WS_EX_CLIENTEDGE))){
        dwRemove&=~WS_EX_CLIENTEDGE;
        dwAdd&=~WS_EX_CLIENTEDGE;
      }
    }
	}else{
	  m_dwStyle=((m_dwStyle&~dwRemove)|dwAdd)&m_dwStyleMask;
    if((dwRemove&WS_DISABLED)||(dwAdd&WS_DISABLED)){
      if(SetProperty(DISPID_ENABLED,VT_BOOL,(~dwAdd&WS_DISABLED))){
        dwRemove&=~WS_DISABLED;
        dwAdd&=~WS_DISABLED;
      }
    }
    if((dwRemove&WS_BORDER)||(dwAdd&WS_BORDER)){
      if(SetProperty(DISPID_BORDERSTYLE,VT_I2,(dwAdd&WS_BORDER))){
        dwRemove&=~WS_BORDER;
        dwAdd&=~WS_BORDER;
      }
    }
	}
  if(!m_bIsWindowless)
	  return XWnd::ModifyStyle(bEx,dwRemove,dwAdd,nFlags);
  return TRUE;
}

DWORD XOleControl::SetStyle(DWORD dwStyle){
  DWORD dwOldStyle=m_dwStyle;
  m_dwStyle=dwStyle&m_dwStyleMask;
  if(SetProperty(DISPID_ENABLED,VT_BOOL,(~dwStyle&WS_DISABLED)))
    dwStyle&=~WS_DISABLED;
  if(SetProperty(DISPID_BORDERSTYLE,VT_I2,(dwStyle&WS_BORDER)))
    dwStyle&=~WS_BORDER;
  if(!m_bIsWindowless)
    return XWnd::SetStyle(dwStyle);
  return dwOldStyle;
}

DWORD XOleControl::SetExStyle(DWORD dwExStyle){
  DWORD dwOldExStyle=m_dwExStyle;
  m_dwExStyle=dwExStyle&m_dwExStyleMask;
  if(SetProperty(DISPID_APPEARANCE,VT_I2,(dwExStyle&WS_EX_CLIENTEDGE)))
    dwExStyle&=~WS_EX_CLIENTEDGE;
  if(!m_bIsWindowless)
    return XWnd::SetExStyle(dwExStyle);
  return dwOldExStyle;
}

BOOL XOleControl::TestStyle(BOOL bEx,DWORD dwStyle){
  return ((bEx)?m_dwExStyle&dwStyle:m_dwStyle&dwStyle);
}

BOOL XOleControl::SetWindowText(LPCTSTR lpszString){
	XASSERT(lpszString);
	return (SetProperty(DISPID_CAPTION,VT_BSTR,lpszString)||SetProperty(DISPID_TEXT,VT_BSTR,lpszString));
}

INT XOleControl::GetWindowText(LPTSTR lpString,INT nMaxCount)const{
  BSTR str=NULL;
  if(!GetProperty(DISPID_CAPTION,VT_BSTR,&str))
    GetProperty(DISPID_TEXT,VT_BSTR,&str);
  if(str){
    INT len=::SysStringLen(str);
    lstrcpyn(lpString,CBSTR2TSTR(str),nMaxCount-1);
    lpString[__min(nMaxCount,len)-1]=0;
    ::SysFreeString(str);
    return len;
  }
  return 0;
}

INT XOleControl::GetDlgCtrlID()const{
	return (INT)m_nCtrlId;
}

INT XOleControl::SetDlgCtrlID(INT nID){
	INT nPrevID=(INT)m_nCtrlId;
	m_nCtrlId=(UINT)nID;
	return nPrevID;
}

BOOL XOleControl::MoveWindow(INT x,INT y,INT nWidth,INT nHeight,BOOL){
	XASSERT(m_pInPlaceObject!=NULL)
	XASSERT(m_pObject!=NULL)
	RECT rectOld=m_rect;
	SetRect(&m_rect,x,y,x+nWidth,y+nHeight);
	if(SetExtent()){
		SetRect(&m_rect,x,y,x+m_rect.right-m_rect.left,y+m_rect.bottom-m_rect.top);
		m_pInPlaceObject->SetObjectRects(&m_rect,&m_rect);
		return TRUE;
	}else
		m_rect=rectOld;
  return FALSE;
}

BOOL XOleControl::SetWindowPos(HWND after,INT x,INT y,INT cx,INT cy,UINT nFlags){
	if((nFlags&(SWP_NOMOVE|SWP_NOSIZE))!=(SWP_NOMOVE|SWP_NOSIZE)){
		INT xNew,yNew,cxNew,cyNew;
		if(nFlags&SWP_NOMOVE){
			xNew=m_rect.left;
			yNew=m_rect.top;
		}else{
			xNew=x;
			yNew=y;
		}
		if(nFlags&SWP_NOSIZE){
			cxNew=m_rect.right-m_rect.left;
			cyNew=m_rect.bottom-m_rect.top;
		}else{
			cxNew=cx;
			cyNew=cy;
		}
		MoveWindow(xNew,yNew,cxNew,cyNew,FALSE);
	}
  if(nFlags&SWP_HIDEWINDOW)
		ShowWindow(SW_HIDE);
	if(nFlags&SWP_SHOWWINDOW)
		ShowWindow(SW_SHOW);
	nFlags&=~(SWP_HIDEWINDOW|SWP_SHOWWINDOW);
	nFlags|=(SWP_NOMOVE|SWP_NOSIZE);
	return ::SetWindowPos(m_hWnd,after,x,y,cx,cy,nFlags);
}

BOOL XOleControl::ShowWindow(UINT nCmdShow){
	BOOL bReturn=::IsWindowVisible(m_hWnd);
	switch(nCmdShow){
    case SW_SHOW:case SW_SHOWNORMAL:case SW_SHOWNOACTIVATE:
      DoVerb(OLEIVERB_SHOW);
    break;
    case SW_HIDE:
      DoVerb(OLEIVERB_HIDE);
		break;
	}
	return bReturn;
}

BOOL XOleControl::IsEnabled()const{
	BOOL bEnabled=TRUE;
	if(!m_bIsWindowless)
		bEnabled=::IsWindowEnabled(m_hWnd);
	GetProperty(DISPID_ENABLED,VT_BOOL,&bEnabled);
	return bEnabled;
}

BOOL XOleControl::EnableWindow(BOOL bEnable){
	BOOL bResult=TRUE;
	GetProperty(DISPID_ENABLED,VT_BOOL,&bResult);
  bResult=!bResult;
	SetProperty(DISPID_ENABLED,VT_BOOL,bEnable);
	if(!m_bIsWindowless)
    bResult=::EnableWindow(m_hWnd,bEnable);
	return bResult;
}

HWND XOleControl::SetFocus(){
	if(m_dwMiscStatus&OLEMISC_NOUIACTIVATE&&!m_bIsWindowless)
    return ::SetFocus(m_hWnd);
	HWND pWndPrev=::GetFocus();
	if((m_dwMiscStatus&OLEMISC_INVISIBLEATRUNTIME)==0){
		if(SUCCEEDED(DoVerb(OLEIVERB_UIACTIVATE)))
			m_pWndParent->m_pSiteFocus=this;
	}
	return pWndPrev;
}

HWND XOleControl::SetFocus(LPMSG lpmsg){
	if(m_dwMiscStatus&OLEMISC_NOUIACTIVATE&&!m_bIsWindowless)
    return ::SetFocus(m_hWnd);
  XASSERT(m_pWndParent!=NULL)
	HWND pWndPrev=::GetFocus();
	if((m_dwMiscStatus&OLEMISC_INVISIBLEATRUNTIME)==0){
		if(SUCCEEDED(DoVerb(OLEIVERB_UIACTIVATE,lpmsg)))
			m_pWndParent->m_pSiteFocus=this;
	}
	return pWndPrev;
}

BOOL XOleControl::GetAmbientProperty(DISPID /*dispid*/,VARIANT* /*pvar*/){
	return FALSE;//should be overload...
}

BOOL XOleControl::OnEvent(XSINKEVENT* pEvent){
	if(OnOleEvent(m_nCtrlId,WM_OLEEVENT,pEvent))
		return TRUE;
	XASSERT(m_pWndParent!=NULL)
	return m_pWndParent->OnOleEvent(m_nCtrlId,WM_OLEEVENT,pEvent);
}

BOOL XOleControl::OnAmbientProperty(DISPID dispid,VARIANT* pvar){
  if(GetAmbientProperty(dispid,pvar))
    return TRUE;
  XASSERT(m_pWndParent!=NULL)
  return m_pWndParent->GetAmbientProperty(dispid,pvar);
}


// XOleControl::XOleClientSite

STDMETHODIMP_(ULONG) XOleControl::XOleClientSite::AddRef(){
	return (ULONG)GetParentPtr()->InternalAddRef();
}

STDMETHODIMP_(ULONG) XOleControl::XOleClientSite::Release(){
	return (ULONG)GetParentPtr()->InternalRelease();
}

STDMETHODIMP XOleControl::XOleClientSite::QueryInterface(REFIID iid,LPVOID* ppvObj){
  HRESULT hr=GetParentPtr()->InternalQueryInterface(&iid,ppvObj);
  return (HRESULT)hr;
}

STDMETHODIMP XOleControl::XOleClientSite::SaveObject(){
	return E_NOTIMPL;
}

STDMETHODIMP XOleControl::XOleClientSite::GetMoniker(DWORD, DWORD,LPMONIKER*){
	return E_NOTIMPL;
}

STDMETHODIMP XOleControl::XOleClientSite::GetContainer(LPOLECONTAINER* ppContainer){
  XOleControl *pThis=GetParentPtr();
  return (HRESULT)pThis->m_pWndParent->QueryInterface(&IID_IOleContainer,(LPVOID*)ppContainer);
}

STDMETHODIMP XOleControl::XOleClientSite::ShowObject(){
  XOleControl *pThis=GetParentPtr();
  HWND hWnd=NULL;
  XASSERT(pThis->m_pInPlaceObject!=NULL)
  if(SUCCEEDED(pThis->m_pInPlaceObject->GetWindow(&hWnd)))
    pThis->Attach(hWnd);
	return S_OK;
}

STDMETHODIMP XOleControl::XOleClientSite::OnShowWindow(BOOL){
	return S_OK;
}

STDMETHODIMP XOleControl::XOleClientSite::RequestNewObjectLayout(){
	return E_NOTIMPL;
}


// XOleControl::XOleIPSite

STDMETHODIMP_(ULONG) XOleControl::XOleIPSite::AddRef(){
	return (ULONG)GetParentPtr()->InternalAddRef();
}

STDMETHODIMP_(ULONG) XOleControl::XOleIPSite::Release(){
	return (ULONG)GetParentPtr()->InternalRelease();
}

STDMETHODIMP XOleControl::XOleIPSite::QueryInterface(REFIID iid, LPVOID* ppvObj){
	return (HRESULT)GetParentPtr()->InternalQueryInterface(&iid, ppvObj);
}

STDMETHODIMP XOleControl::XOleIPSite::GetWindow(HWND* phWnd){
	XOleControl *pThis=GetParentPtr();
	XASSERT(pThis->m_pWndParent!=NULL)
	*phWnd=pThis->m_pWndParent->m_hWnd;
	return *phWnd!=NULL?S_OK:E_FAIL;
}

STDMETHODIMP XOleControl::XOleIPSite::ContextSensitiveHelp(BOOL){
	return E_NOTIMPL;
}

STDMETHODIMP XOleControl::XOleIPSite::CanInPlaceActivate(){
	return S_OK;
}

STDMETHODIMP XOleControl::XOleIPSite::OnInPlaceActivate(){
	return S_OK;
}

STDMETHODIMP XOleControl::XOleIPSite::OnUIActivate(){
	XOleControl *pThis=GetParentPtr();
  XASSERT(pThis->m_pWndParent!=NULL)
	pThis->m_pWndParent->OnUIActivate(pThis);
	return S_OK;
}

STDMETHODIMP XOleControl::XOleIPSite::GetWindowContext(LPOLEINPLACEFRAME* ppFrame,LPOLEINPLACEUIWINDOW* ppDoc,LPRECT prectPos,LPRECT prectClip,LPOLEINPLACEFRAMEINFO pFrameInfo){
  XOleControl *pThis=GetParentPtr();
  XASSERT(ppFrame!=NULL)
  XASSERT(pFrameInfo!=NULL)
  XASSERT(prectPos!=NULL)
	XASSERT(prectClip!=NULL)
  if(ppDoc!=NULL)
		*ppDoc=NULL;
  XASSERT(pThis->m_pWndParent!=NULL)
  if(FAILED(pThis->m_pWndParent->QueryInterface(&IID_IOleInPlaceFrame,(LPVOID*)ppFrame)))
		return E_FAIL;
  ::CopyRect(prectPos,&pThis->m_rect);
  pThis->m_pWndParent->GetClientRect(prectClip);
	pFrameInfo->fMDIApp=FALSE;
	pFrameInfo->hwndFrame=pThis->m_pWndParent->m_hWnd;
	pFrameInfo->haccel=NULL;
	pFrameInfo->cAccelEntries=0;
	return S_OK;
}

STDMETHODIMP XOleControl::XOleIPSite::Scroll(SIZE){
	return S_FALSE;
}

STDMETHODIMP XOleControl::XOleIPSite::OnUIDeactivate(BOOL){
  XOleControl *pThis=GetParentPtr();
  XASSERT(pThis->m_pWndParent!=NULL)
	pThis->m_pWndParent->OnUIDeactivate(pThis);
	return S_OK;
}

STDMETHODIMP XOleControl::XOleIPSite::OnInPlaceDeactivate(){
  XOleControl *pThis=GetParentPtr();
  XASSERT(pThis->m_pWndParent!=NULL)
  pThis->XWnd::Detach();
  if(pThis->m_bIsWindowless){
	  if(pThis->m_pWindowlessObject!=NULL){
		  pThis->m_pWindowlessObject->Release();
		  pThis->m_pWindowlessObject=NULL;
	  }
	  pThis->m_pWndParent->m_nWindowlessControls--;
	  pThis->m_bIsWindowless=FALSE;
  }
	return S_OK;
}

STDMETHODIMP XOleControl::XOleIPSite::DiscardUndoState(){
	return S_OK;
}

STDMETHODIMP XOleControl::XOleIPSite::DeactivateAndUndo(){
  XOleControl *pThis=GetParentPtr();
  XASSERT(pThis->m_pInPlaceObject!=NULL)
  pThis->m_pInPlaceObject->UIDeactivate();
	return S_OK;
}

STDMETHODIMP XOleControl::XOleIPSite::OnPosRectChange(LPCRECT lprcPosRect){
  XOleControl *pThis=GetParentPtr();
  XASSERT(lprcPosRect!=NULL)
  XASSERT(pThis->m_hWnd!=NULL)
	XASSERT(pThis->m_pInPlaceObject!=NULL)
	XASSERT(pThis->m_pWndParent!=NULL)
	RECT rectClip;
	::CopyRect(&pThis->m_rect,lprcPosRect);
	::GetClientRect(pThis->m_pWndParent->m_hWnd,&rectClip);
	return pThis->m_pInPlaceObject->SetObjectRects(&pThis->m_rect,&rectClip);
}

STDMETHODIMP XOleControl::XOleIPSite::OnInPlaceActivateEx(BOOL* ptNoRedraw,DWORD dwFlags){
  XOleControl *pThis=GetParentPtr();
  XASSERT(pThis->m_pWndParent!=NULL)
  XASSERT(!pThis->m_bIsWindowless)
  if(dwFlags&ACTIVATE_WINDOWLESS){
    XASSERT(pThis->m_pWindowlessObject==NULL)
    pThis->m_pObject->QueryInterface(IID_IOleInPlaceObjectWindowless,(void**)&pThis->m_pWindowlessObject);
    XASSERT(pThis->m_pWindowlessObject!=NULL)
    pThis->m_pWndParent->m_nWindowlessControls++;
    pThis->m_bIsWindowless=TRUE;
  }
  if(ptNoRedraw!=NULL)
	  *ptNoRedraw = FALSE;
  return S_OK;
}

STDMETHODIMP XOleControl::XOleIPSite::OnInPlaceDeactivateEx( BOOL /* tNoRedraw */ ){
  return( OnInPlaceDeactivate() );
}

STDMETHODIMP XOleControl::XOleIPSite::RequestUIActivate(){
  return S_OK;
}

STDMETHODIMP XOleControl::XOleIPSite::AdjustRect(LPRECT /* prect */ ){
  return E_NOTIMPL;
}

STDMETHODIMP XOleControl::XOleIPSite::CanWindowlessActivate(){
	return S_OK;
}

STDMETHODIMP XOleControl::XOleIPSite::GetCapture(){
  XOleControl *pThis=GetParentPtr();
  XASSERT(pThis->m_pWndParent!=NULL)
  if(pThis->m_pWndParent->m_pSiteCapture==pThis)
	  return S_OK;
  return S_FALSE;
}

STDMETHODIMP XOleControl::XOleIPSite::GetDC(LPCRECT prect, DWORD /* dwFlags */, HDC* phDC ){
  XOleControl *pThis=GetParentPtr();
  XASSERT(pThis->m_pWndParent!=NULL)
  XASSERT(phDC!=NULL)
  if(!phDC)
    return E_FAIL;
  *phDC=NULL;
  if(pThis->m_hDC!=NULL)
	  return E_FAIL;
  pThis->m_hDC=pThis->m_pWndParent->GetDC();
  if(pThis->m_hDC==NULL)
	  return E_FAIL;
  RECT rect;
  if(prect==NULL)
	  CopyRect(&rect,&pThis->m_rect);
  else
	  IntersectRect(&rect,&pThis->m_rect,prect);
  HRGN rgn=CreateRectRgnIndirect(&rect);
  ExtSelectClipRgn(pThis->m_hDC,rgn,RGN_AND);
  *phDC=pThis->m_hDC;
  return S_OK;
}

STDMETHODIMP XOleControl::XOleIPSite::GetFocus(){
  return E_NOTIMPL;
}

STDMETHODIMP XOleControl::XOleIPSite::InvalidateRect(LPCRECT prect, BOOL bErase ){
  XOleControl *pThis=GetParentPtr();
  XASSERT(pThis->m_pWndParent!=NULL)
  RECT rect;
  if(prect==NULL)
	  CopyRect(&rect,&pThis->m_rect);
  else
	  IntersectRect(&rect,prect,&pThis->m_rect);
  if(!IsRectEmpty(&rect))
	  pThis->m_pWndParent->InvalidateRect(&rect,bErase);
  return S_OK;
}

STDMETHODIMP XOleControl::XOleIPSite::InvalidateRgn(HRGN hRgn,BOOL bErase){
  XOleControl *pThis=GetParentPtr();
  XASSERT(pThis->m_pWndParent!=NULL)
  if(hRgn==NULL)
	  return InvalidateRect(NULL,bErase);
  RECT rectRgn,rect;
  GetRgnBox(hRgn,&rectRgn);
  IntersectRect(&rect,&rectRgn,&pThis->m_rect);
  if(!EqualRect(&rect,&rectRgn))
    return InvalidateRect(NULL,bErase);
  pThis->m_pWndParent->InvalidateRgn(hRgn,bErase);
  return S_OK;
}

STDMETHODIMP XOleControl::XOleIPSite::OnDefWindowMessage( UINT /* nMessage */, WPARAM /* wParam */, LPARAM /* lParam */,LRESULT* /* plResult */ ){
  return E_NOTIMPL;
}

STDMETHODIMP XOleControl::XOleIPSite::ReleaseDC(HDC hDC){
  XOleControl *pThis=GetParentPtr();
  XASSERT(pThis->m_pWndParent!=NULL)
  XASSERT(hDC==pThis->m_hDC)
  pThis->m_pWndParent->ReleaseDC(hDC);
  pThis->m_hDC=NULL;
  return S_OK;
}

STDMETHODIMP XOleControl::XOleIPSite::ScrollRect(int /* dx */, int /* dy */, LPCRECT /* prectScroll */, LPCRECT /* prectClip */ ){
  return E_NOTIMPL;
}

STDMETHODIMP XOleControl::XOleIPSite::SetCapture(BOOL bCapture ){
  XOleControl *pThis=GetParentPtr();
  XASSERT(pThis->m_pWndParent!=NULL)
  if(bCapture){
    pThis->m_pWndParent->SetCapture();
    pThis->m_pWndParent->m_pSiteCapture=pThis;
  }else{
    pThis->m_pWndParent->m_pSiteCapture=NULL;
    pThis->m_pWndParent->ReleaseCapture();
  }
  return S_OK;
}

STDMETHODIMP XOleControl::XOleIPSite::SetFocus(BOOL bFocus){
  XOleControl *pThis=GetParentPtr();
  XASSERT(pThis->m_pWndParent!=NULL)
  if(bFocus){
	  pThis->m_pWndParent->m_pSiteFocus=pThis;
	  pThis->m_pWndParent->SetFocus();
  }else
	  pThis->m_pWndParent->m_pSiteFocus=NULL;
  return S_OK;
}


// XOleControl::XOleControlSite

STDMETHODIMP_(ULONG) XOleControl::XOleControlSite::AddRef(){
	return (ULONG)GetParentPtr()->InternalAddRef();
}

STDMETHODIMP_(ULONG) XOleControl::XOleControlSite::Release(){
	return (ULONG)GetParentPtr()->InternalRelease();
}

STDMETHODIMP XOleControl::XOleControlSite::QueryInterface(REFIID iid, LPVOID* ppvObj){
	return (HRESULT)GetParentPtr()->InternalQueryInterface(&iid, ppvObj);
}

STDMETHODIMP XOleControl::XOleControlSite::OnControlInfoChanged(){
	XOleControl *pThis=GetParentPtr();
	pThis->GetControlInfo();
	return NOERROR;
}

STDMETHODIMP XOleControl::XOleControlSite::LockInPlaceActive(BOOL){
	return E_NOTIMPL;
}

STDMETHODIMP XOleControl::XOleControlSite::GetExtendedControl(LPDISPATCH*){
	return E_NOTIMPL;
}

STDMETHODIMP XOleControl::XOleControlSite::TransformCoords(POINTL* pptHimetric, POINTF* pptContainer, DWORD dwFlags){
	XOleControl *pThis=GetParentPtr();
	HRESULT hr=E_INVALIDARG;SIZE sz;
	XClientDC dc(pThis->m_hWnd);
	if(dwFlags&XFORMCOORDS_HIMETRICTOCONTAINER){
		sz.cx=pptHimetric->x;
		sz.cy=pptHimetric->y;
		dc.HIMETRICtoDP(&sz);
		if(dwFlags&XFORMCOORDS_SIZE){
			pptContainer->x=(float)abs(sz.cx);
			pptContainer->y=(float)abs(sz.cy);
			hr=NOERROR;
		}else if(dwFlags&XFORMCOORDS_POSITION){
			pptContainer->x=(float)sz.cx;
			pptContainer->y=(float)sz.cy;
			hr=NOERROR;
		}
	}else if (dwFlags & XFORMCOORDS_CONTAINERTOHIMETRIC){
		sz.cx=(int)(pptContainer->x);
		sz.cy=(int)(pptContainer->y);
		dc.DPtoHIMETRIC(&sz);
		if(dwFlags&XFORMCOORDS_SIZE){
			pptHimetric->x=abs(sz.cx);
			pptHimetric->y=abs(sz.cy);
			hr=NOERROR;
		}else if(dwFlags&XFORMCOORDS_POSITION){
			pptHimetric->x=sz.cx;
			pptHimetric->y=sz.cy;
			hr=NOERROR;
		}
	}
	return hr;
}

STDMETHODIMP XOleControl::XOleControlSite::TranslateAccelerator(LPMSG, DWORD){
	return E_NOTIMPL;
}

STDMETHODIMP XOleControl::XOleControlSite::OnFocus(BOOL){
	return S_OK;
}

STDMETHODIMP XOleControl::XOleControlSite::ShowPropertyFrame(){
	return E_NOTIMPL;
}


// XOleControl::XAmbientProps

STDMETHODIMP_(ULONG) XOleControl::XAmbientProps::AddRef(){
	return (ULONG)GetParentPtr()->InternalAddRef();
}

STDMETHODIMP_(ULONG) XOleControl::XAmbientProps::Release(){
	return (ULONG)GetParentPtr()->InternalRelease();
}

STDMETHODIMP XOleControl::XAmbientProps::QueryInterface(REFIID iid, LPVOID* ppvObj){
	return (HRESULT)GetParentPtr()->InternalQueryInterface(&iid, ppvObj);
}

STDMETHODIMP XOleControl::XAmbientProps::GetTypeInfoCount(unsigned int*){
	return E_NOTIMPL;
}

STDMETHODIMP XOleControl::XAmbientProps::GetTypeInfo(unsigned int, LCID, ITypeInfo**){
	return E_NOTIMPL;
}

STDMETHODIMP XOleControl::XAmbientProps::GetIDsOfNames(REFIID, LPOLESTR*, unsigned int, LCID, DISPID*){
	return E_NOTIMPL;
}

STDMETHODIMP XOleControl::XAmbientProps::Invoke(DISPID dispid, REFIID, LCID, unsigned short wFlags,DISPPARAMS* pDispParams, VARIANT* pvarResult,EXCEPINFO*, unsigned int*){
  XOleControl *pThis=GetParentPtr();
	XASSERT(wFlags&DISPATCH_PROPERTYGET)
	XASSERT(pDispParams->cArgs==0)
	return (pThis->OnAmbientProperty(dispid,pvarResult)?S_OK:DISP_E_MEMBERNOTFOUND);
}


// XOleControl::XPropertyNotifySink

STDMETHODIMP XOleControl::XPropertyNotifySink::QueryInterface(REFIID iid, LPVOID* ppvObj){
	if(IsEqualIID(iid,IID_IUnknown)||IsEqualIID(iid,IID_IPropertyNotifySink)){
		*ppvObj=this;
		AddRef();
		return S_OK;
	}
  return E_NOINTERFACE;
}

STDMETHODIMP_(ULONG) XOleControl::XPropertyNotifySink::AddRef(){
	return 1;
}

STDMETHODIMP_(ULONG) XOleControl::XPropertyNotifySink::Release(){
	return 0;
}

STDMETHODIMP XOleControl::XPropertyNotifySink::OnChanged(DISPID dispid){
  XOleControl *pThis=GetParentPtr();
  XASSERT(pThis->m_pWndParent!=NULL)
	XSINKEVENT event(XSINKEVENT::XS_PROPCHANGED,dispid);
	pThis->OnEvent(&event);
	return event.m_hResult;
}

STDMETHODIMP XOleControl::XPropertyNotifySink::OnRequestEdit(DISPID dispid){
  XOleControl *pThis=GetParentPtr();
  XASSERT(pThis->m_pWndParent!=NULL)
	XSINKEVENT event(XSINKEVENT::XS_PROPREQUEST,dispid);
  pThis->OnEvent(&event);
	return event.m_hResult;
}


// XOleControl::XEventSink

STDMETHODIMP_(ULONG) XOleControl::XEventSink::AddRef(){
	return 1;
}

STDMETHODIMP_(ULONG) XOleControl::XEventSink::Release(){
	return 0;
}

STDMETHODIMP XOleControl::XEventSink::QueryInterface(REFIID iid, LPVOID* ppvObj){
  XOleControl *pThis=GetParentPtr();
	if(IsEqualIID(iid,IID_IUnknown)||IsEqualIID(iid,IID_IDispatch)||
		IsEqualIID(iid,pThis->m_iidEvents)){
		*ppvObj=this;
		AddRef();
		return S_OK;
	}
	return E_NOINTERFACE;
}

STDMETHODIMP XOleControl::XEventSink::GetTypeInfoCount(unsigned int*){
	return E_NOTIMPL;
}

STDMETHODIMP XOleControl::XEventSink::GetTypeInfo(unsigned int, LCID, ITypeInfo**){
	return E_NOTIMPL;
}

STDMETHODIMP XOleControl::XEventSink::GetIDsOfNames(REFIID, LPOLESTR*, unsigned int, LCID, DISPID*){
	return E_NOTIMPL;
}

STDMETHODIMP XOleControl::XEventSink::Invoke(DISPID dispid, REFIID, LCID, unsigned short wFlags,DISPPARAMS* pDispParams, VARIANT* pvarResult,EXCEPINFO* pExcepInfo, unsigned int* puArgError){
  XOleControl *pThis=GetParentPtr();
  XASSERT(pThis->m_pWndParent!=NULL)
  XASSERT(wFlags==DISPATCH_METHOD)
  XSINKEVENT event(XSINKEVENT::XS_EVENT,dispid,pDispParams,pExcepInfo,puArgError);
	pThis->OnEvent(&event);
	if(pvarResult!=NULL)
		::VariantClear(pvarResult);
	return event.m_hResult;
}
