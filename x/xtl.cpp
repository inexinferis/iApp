#include "xtl.h"

VOID XOleVariant::SetByteArray(BYTE* pData,INT_PTR nSize){
  XASSERT(nSize<LONG_MAX)
  vt=VT_EMPTY;
  CreateOneDimArray((DWORD)nSize);
  CopyBinaryData(pData,(DWORD)nSize);
}

VOID XOleVariant::SetLongBinary(const HGLOBAL &hData,ULONG dwDataLength){
  XASSERT(dwDataLength<LONG_MAX)
  vt=VT_EMPTY;
  CreateOneDimArray((ULONG)dwDataLength);
  BYTE* pData=(BYTE*)::GlobalLock(hData);
  CopyBinaryData(pData,(ULONG)dwDataLength);
  ::GlobalUnlock(hData);
}

BOOL XOleVariant::GetByteArray(BYTE* pDest,ULONG *pSize){
  XASSERT(V_ISARRAY(this))
  XASSERT(pSize!=NULL)
  LPVOID pSrc;BOOL bRet=FALSE;
  ::SafeArrayAccessData(V_ARRAY(this),&pSrc);
  ULONG nDim=::SafeArrayGetDim(V_ARRAY(this));
  XASSERT((nDim==0)||(nDim==1))
  if(nDim==1){
    LONG iLowerBound,iUpperBound;
    ::SafeArrayGetLBound(V_ARRAY(this),1,&iLowerBound);
    ::SafeArrayGetUBound(V_ARRAY(this),1,&iUpperBound);
    LONG nElements=(iUpperBound-iLowerBound)+1;
    ULONG nBytes = nElements*::SafeArrayGetElemsize(V_ARRAY(this));
    if(*pSize>nBytes)
      *pSize=nBytes;
    if(pDest!=NULL)
      memcpy(pDest,pSrc,*pSize);
    bRet=TRUE;
  }else
    *pSize=0;
  ::SafeArrayUnaccessData(V_ARRAY(this));
  return bRet;
}

VOID XOleVariant::SetString(LPCTSTR lpszSrc,VARTYPE vtSrc){
  #ifdef _UNICODE
  XASSERT(vtSrc==VT_BSTR)
  #else
  XASSERT(vtSrc==VT_BSTR||vtSrc==VT_BSTRT)
  #endif
  Clear();
  vt=VT_BSTR;
  bstrVal=NULL;
  if(lpszSrc!=NULL){
    #ifndef _UNICODE
    if(vtSrc==VT_BSTRT){
      int nLen=lstrlen(lpszSrc);
      bstrVal=::SysAllocStringByteLen(lpszSrc,nLen);
      XASSERT(bstrVal!=NULL)
    }else
    #endif
      bstrVal=XComBSTR(lpszSrc);
  }
}

VOID XOleVariant::CreateArray(VARTYPE vtSrc,DWORD dwDims,DWORD* rgElements){
  XASSERT(rgElements!=NULL)
  if(rgElements==NULL)
    return;
  SAFEARRAYBOUND* rgsaBounds=new SAFEARRAYBOUND[dwDims];
  for(DWORD dwIndex = 0; dwIndex < dwDims; dwIndex++){
    rgsaBounds[dwIndex].lLbound=0;
    rgsaBounds[dwIndex].cElements=rgElements[dwIndex];
  }
  CreateArray(vtSrc,dwDims,rgsaBounds);
  delete[] rgsaBounds;
}

VOID XOleVariant::CreateArray(VARTYPE vtSrc,DWORD dwDims,SAFEARRAYBOUND* rgsabound){
  XASSERT(dwDims>0)
  XASSERT(rgsabound!=NULL)
  XASSERT(!(vtSrc&VT_ARRAY))
  XASSERT(!(vtSrc&VT_BYREF))
  XASSERT(!(vtSrc&VT_VECTOR))
  XASSERT(vtSrc!=VT_EMPTY)
  XASSERT(vtSrc!=VT_NULL)
  if(dwDims==0||rgsabound==NULL||(vtSrc&VT_ARRAY)||(vtSrc&VT_BYREF)||(vtSrc&VT_VECTOR)||vtSrc==VT_EMPTY||vtSrc==VT_NULL)
    return;
  Clear();
  parray=::SafeArrayCreate(vtSrc,dwDims,rgsabound);
  if(parray==NULL)
    return;
  vt=(unsigned short)(vtSrc|VT_ARRAY);
}

VOID XOleVariant::CreateOneDimArray(VARTYPE vtSrc,DWORD dwElements,const void* pvSrcData,LONG nLBound){
  XASSERT(dwElements>0)
  if(!(dwElements > 0))
    return;
  SAFEARRAYBOUND rgsabound;
  rgsabound.cElements=dwElements;
  rgsabound.lLbound=nLBound;
  CreateArray(vtSrc,1,&rgsabound);
  if(pvSrcData!=NULL){
    void* pvDestData;
    ArrayAccessData(&pvDestData);
    unsigned __int64 tmp_64=static_cast<unsigned __int64>(ArrayGetElemSize()) * static_cast<unsigned __int64>(dwElements);
    XASSERT(tmp_64<=INT_MAX)
    memcpy(pvDestData,pvSrcData,static_cast<size_t>(tmp_64));
    ArrayUnaccessData();
  }
}

VOID XOleVariant::CreateOneDimArray(DWORD dwSize){
  UINT nDim;
  if(vt!=(VT_UI1|VT_ARRAY)||(nDim=::SafeArrayGetDim(parray))!=1){
    HRESULT hr=::VariantClear(this);
    XASSERT(hr==NOERROR)
    XUNUSED(hr)
    vt=VT_UI1|VT_ARRAY;
    SAFEARRAYBOUND bound;
    bound.cElements=dwSize;
    bound.lLbound=0;
    parray=::SafeArrayCreate(VT_UI1,1,&bound);
    XASSERT(parray!=NULL)
  }else{
    LONG lLower,lUpper;
    ::SafeArrayGetLBound(parray,1,&lLower);
    ::SafeArrayGetUBound(parray,1,&lUpper);
    LONG lSize=lUpper-lLower;
    XASSERT(lSize>=0)
    if(lSize<0)
      lSize=0;
    if((DWORD)lSize!=dwSize){
      SAFEARRAYBOUND bound;
      bound.cElements=dwSize;
      bound.lLbound=lLower;
      ::SafeArrayRedim(parray,&bound);
    }
  }
}

DWORD XOleVariant::ArrayGetOneDimSize(){
  XASSERT(V_ISARRAY(this))
  if(V_ISARRAY(this)){
    XASSERT(ArrayGetDim()==1)
    if(ArrayGetDim()!=1)
      return 0;
    LONG nUBound,nLBound;
    ArrayGetUBound(1,&nUBound);
    ArrayGetLBound(1,&nLBound);
    return nUBound+1-nLBound;
  }
  return 0;
}

VOID XOleVariant::ArrayResizeOneDim(DWORD dwElements){
  XASSERT(V_ISARRAY(this))
  if(V_ISARRAY(this)){
    XASSERT(ArrayGetDim()==1)
    if(ArrayGetDim()!=1)
      return;
    SAFEARRAYBOUND rgsabound;
    rgsabound.cElements=dwElements;
    rgsabound.lLbound=0;
    ArrayRedim(&rgsabound);
  }
}

VOID XOleVariant::CopyBinaryData(const PVOID pvSrc,DWORD dwSize){
  PVOID pDest;
  ::SafeArrayAccessData(parray,&pDest);
  memcpy(pDest,pvSrc,dwSize);
  ::SafeArrayUnaccessData(parray);
}

BOOL XOleVariant::CompareSafeArrays(SAFEARRAY* parray1, SAFEARRAY* parray2){
  BOOL bCompare=FALSE;
  if(parray1==NULL||parray2==NULL)
    return parray1==parray2;
  DWORD dwDim1=::SafeArrayGetDim(parray1);
  DWORD dwDim2=::SafeArrayGetDim(parray2);
  if(dwDim1!=dwDim2)
    return FALSE;
  else if(dwDim1==0)
    return TRUE;
  DWORD dwSize1=::SafeArrayGetElemsize(parray1);
  DWORD dwSize2=::SafeArrayGetElemsize(parray2);
  if(dwSize1!=dwSize2)
    return FALSE;
  long* pLBound1 = NULL;
  long* pLBound2 = NULL;
  long* pUBound1 = NULL;
  long* pUBound2 = NULL;
  void* pData1 = NULL;
  void* pData2 = NULL;
  pLBound1 = new long[dwDim1];
  pLBound2 = new long[dwDim2];
  pUBound1 = new long[dwDim1];
  pUBound2 = new long[dwDim2];
  size_t nTotalElements = 1;
  for(DWORD dwIndex=0;dwIndex<dwDim1;dwIndex++){
    ::SafeArrayGetLBound(parray1,dwIndex+1,&pLBound1[dwIndex]);
    ::SafeArrayGetLBound(parray2,dwIndex+1,&pLBound2[dwIndex]);
    ::SafeArrayGetUBound(parray1,dwIndex+1,&pUBound1[dwIndex]);
    ::SafeArrayGetUBound(parray2,dwIndex+1,&pUBound2[dwIndex]);
    if(pUBound1[dwIndex]-pLBound1[dwIndex]!=pUBound2[dwIndex]-pLBound2[dwIndex]){
      delete[] pLBound1;
      delete[] pLBound2;
      delete[] pUBound1;
      delete[] pUBound2;
      return FALSE;
    }
    nTotalElements*=pUBound1[dwIndex]-pLBound1[dwIndex]+1;
  }
  ::SafeArrayAccessData(parray1,&pData1);
  ::SafeArrayAccessData(parray2,&pData2);
  size_t nSize=nTotalElements*dwSize1;
  int nOffset=memcmp(pData1,pData2,nSize);
  bCompare=nOffset==0;
  ::SafeArrayUnaccessData(parray1);
  ::SafeArrayUnaccessData(parray2);
  delete[] pLBound1;
  delete[] pLBound2;
  delete[] pUBound1;
  delete[] pUBound2;
  return bCompare;
}


// XOleDispatchDriver

XOleDispatchDriver::XOleDispatchDriver(const XOleDispatchDriver& dispatchSrc){
  XASSERT(this!=&dispatchSrc)
  if(this!=&dispatchSrc){
    m_lpDispatch=dispatchSrc.m_lpDispatch;
    if(m_lpDispatch!=NULL)
      m_lpDispatch->AddRef();
    m_bAutoRelease=TRUE;
  }
}

BOOL XOleDispatchDriver::Create(REFCLSID clsid,HRESULT *pHr){
	XASSERT(m_lpDispatch==NULL)
	LPUNKNOWN lpUnknown=NULL;
	HRESULT hr=CoCreateInstance(clsid,NULL,CLSCTX_ALL|CLSCTX_REMOTE_SERVER,IID_IUnknown,(PVOID*)&lpUnknown);
	if(hr==E_INVALIDARG)
		hr=CoCreateInstance(clsid,NULL,CLSCTX_ALL&~CLSCTX_REMOTE_SERVER,IID_IUnknown,(PVOID*)&lpUnknown);
	if(FAILED(hr)||!lpUnknown){
    if(pHr!=NULL)
      *pHr=hr;
    return FALSE;
	}
	if(FAILED(hr=OleRun(lpUnknown))||FAILED(hr=lpUnknown->QueryInterface(IID_IDispatch,(PVOID*)&m_lpDispatch))||m_lpDispatch==NULL){
    if(pHr!=NULL)
      *pHr=hr;
    lpUnknown->Release();
    return FALSE;
	}
  lpUnknown->Release();
	m_bAutoRelease=TRUE;
	return TRUE;
}

BOOL XOleDispatchDriver::Create(LPCTSTR lpszProgID,HRESULT *pHr){
	XASSERT(m_lpDispatch==NULL)
	CLSID clsid;
	HRESULT hr=CLSIDFromProgID(TSTR2WSTR(lpszProgID),&clsid);
	if(FAILED(hr)){
		if(pHr!=NULL)
			*pHr=hr;
		return FALSE;
	}
	return Create(clsid,pHr);
}

const XOleDispatchDriver& XOleDispatchDriver::operator=(const XOleDispatchDriver& dispatchSrc){
  XASSERT(this!=&dispatchSrc)
  if(this!=&dispatchSrc){
    if(m_lpDispatch!=NULL&&m_bAutoRelease)
      m_lpDispatch->Release();
    m_lpDispatch=dispatchSrc.m_lpDispatch;
    if(m_lpDispatch!=NULL)
      m_lpDispatch->AddRef();
    m_bAutoRelease=TRUE;
  }
  return *this;
}

LPDISPATCH XOleDispatchDriver::FromObject(LPUNKNOWN lpIUnknown,BOOL bAutoRelease){
  XASSERT(lpIUnknown!=NULL)
  if(lpIUnknown){
    LPDISPATCH pDispatch;
		if(SUCCEEDED(lpIUnknown->QueryInterface(IID_IDispatch,(LPVOID*)&pDispatch))){
			XASSERT(pDispatch!=NULL)
			return Attach(pDispatch,bAutoRelease);
		}
  }
  return NULL;
}

LPDISPATCH XOleDispatchDriver::Attach(LPDISPATCH lpDispatch,BOOL bAutoRelease){
	XASSERT(lpDispatch!=NULL)
	LPDISPATCH lpOldDispatch=Release();
	m_lpDispatch=lpDispatch;
	m_bAutoRelease=bAutoRelease;
	return lpOldDispatch;
}

LPDISPATCH XOleDispatchDriver::Detach(){
	LPDISPATCH lpDispatch=m_lpDispatch;
	m_lpDispatch=NULL;
	return lpDispatch;
}

LPDISPATCH XOleDispatchDriver::Release(){
	if(m_lpDispatch!=NULL&&m_bAutoRelease)
		m_lpDispatch->Release();
	return Detach();
}

HRESULT XOleDispatchDriver::InvokeHelper(DISPID dwDispID,WORD wFlags,VARTYPE vtRet,PVOID pvRet,const WCHAR* pbParamInfo,...)const{
	HRESULT hr=E_INVALIDARG;
  if(!m_lpDispatch)
		return hr;
  DISPPARAMS params;va_list argList;
  memset(&params,0,sizeof(params));
  va_start(argList,pbParamInfo);
	if(pbParamInfo!=NULL)
		params.cArgs=lstrlenW(pbParamInfo);
  DISPID dispidNamed=DISPID_PROPERTYPUT;
	if(wFlags&(DISPATCH_PROPERTYPUT|DISPATCH_PROPERTYPUTREF)){
		XASSERT(params.cArgs>0)
		params.cNamedArgs=1;
		params.rgdispidNamedArgs=&dispidNamed;
	}
	if(params.cArgs!=0){
    VARIANT* pArg=new VARIANT[params.cArgs];
    XASSERT(pArg!=NULL)
    params.rgvarg=pArg;
    memset(pArg,0,sizeof(VARIANT)*params.cArgs);
    const WCHAR* pb=pbParamInfo;
    pArg+=params.cArgs-1;
		while(*pb!=0){
			XASSERT(pArg>=params.rgvarg)
			pArg->vt=*pb;
			switch(pArg->vt){
        case VT_UI1:
          pArg->bVal=va_arg(argList,int);//BYTE);
        break;
        case VT_UI2:
          pArg->uiVal=va_arg(argList,int);//USHORT);
        break;
        case VT_UI4:
          pArg->ulVal=va_arg(argList,ULONG);
        break;
        case VT_UI8:
          pArg->ullVal=va_arg(argList,ULONGLONG);
        break;
        case VT_I1:
          pArg->cVal=va_arg(argList,int);//char);
        break;
        case VT_I2:
          pArg->iVal=va_arg(argList,int);//short);
        break;
        case VT_I4:
          pArg->lVal=va_arg(argList,long);
        break;
        case VT_I8:
          pArg->llVal=va_arg(argList,LONGLONG);
        break;
        case VT_R4:
          pArg->fltVal=(float)va_arg(argList,double);
        break;
        case VT_R8:
          pArg->dblVal=va_arg(argList,double);
        break;
        case VT_DATE:
          pArg->date=va_arg(argList,DATE);
        break;
        case VT_CY:
          pArg->cyVal=*va_arg(argList,CY*);
        break;
        case VT_BSTR:{
          LPCOLESTR lpsz=va_arg(argList,LPOLESTR);
          pArg->bstrVal=::SysAllocString(lpsz);
          if(lpsz!=NULL&&pArg->bstrVal==NULL){
            va_end(argList);
            return hr;
          }
        }break;
        #ifndef _UNICODE
        case VT_BSTRA:{
					LPCSTR lpsz=va_arg(argList,LPSTR);
					pArg->bstrVal=::SysAllocString(CA2W(lpsz));
					if(lpsz!=NULL&&pArg->bstrVal==NULL){
						va_end(argList);
            return hr;
          }
					pArg->vt=VT_BSTR;
				}break;
        #endif
        case VT_DISPATCH:
          pArg->pdispVal=va_arg(argList,LPDISPATCH);
				break;
        case VT_ERROR:
          pArg->scode=va_arg(argList,SCODE);
				break;
        case VT_BOOL:
          V_BOOL(pArg)=(VARIANT_BOOL)(va_arg(argList,BOOL)?-1:0);
				break;
        case VT_VARIANT:
          *pArg=*va_arg(argList,VARIANT*);
				break;
        case VT_UNKNOWN:
          pArg->punkVal=va_arg(argList,LPUNKNOWN);
				break;
        case VT_UI1|VT_BYREF:
          pArg->pbVal=va_arg(argList,BYTE*);
				break;
        case VT_UI2|VT_BYREF:
          pArg->puiVal=va_arg(argList,USHORT*);
				break;
        case VT_UI4|VT_BYREF:
          pArg->pulVal=va_arg(argList,ULONG*);
				break;
        case VT_UI8|VT_BYREF:
          pArg->pullVal = va_arg(argList, ULONGLONG*);
        break;
        case VT_I1|VT_BYREF:
          pArg->pcVal = va_arg(argList, char*);
        break;
        case VT_I2|VT_BYREF:
          pArg->piVal = va_arg(argList, short*);
        break;
        case VT_I4|VT_BYREF:
          pArg->plVal = va_arg(argList, long*);
        break;
        case VT_I8|VT_BYREF:
          pArg->pllVal = va_arg(argList, LONGLONG*);
        break;
        case VT_R4|VT_BYREF:
          pArg->pfltVal = va_arg(argList, float*);
        break;
        case VT_R8|VT_BYREF:
          pArg->pdblVal = va_arg(argList, double*);
        break;
        case VT_DATE|VT_BYREF:
          pArg->pdate = va_arg(argList, DATE*);
        break;
        case VT_CY|VT_BYREF:
          pArg->pcyVal = va_arg(argList, CY*);
        break;
        case VT_BSTR|VT_BYREF:
          pArg->pbstrVal = va_arg(argList, BSTR*);
        break;
        case VT_DISPATCH|VT_BYREF:
          pArg->ppdispVal = va_arg(argList, LPDISPATCH*);
        break;
        case VT_ERROR|VT_BYREF:
          pArg->pscode = va_arg(argList, SCODE*);
        break;
        case VT_BOOL|VT_BYREF:{
          BOOL* pboolVal=va_arg(argList,BOOL*);
          *pboolVal=*pboolVal?MAKELONG(0xffff,0):0;
          pArg->pboolVal=(VARIANT_BOOL*)pboolVal;
        }break;
        case VT_VARIANT|VT_BYREF:
          pArg->pvarVal=va_arg(argList,VARIANT*);
        break;
        case VT_UNKNOWN|VT_BYREF:
          pArg->ppunkVal=va_arg(argList,LPUNKNOWN*);
        break;
        default:
          va_end(argList);
          return hr;
				break;
			}
			--pArg;++pb;
		}
	}
	VARIANT vaResult,*pvarResult=NULL;
	if(vtRet!=VT_EMPTY){
    memset(&vaResult,0,sizeof(vaResult));
		pvarResult=&vaResult;
  }
	UINT nArgErr=(UINT)-1;
	hr=m_lpDispatch->Invoke(dwDispID,IID_NULL,0,wFlags,&params,pvarResult,NULL,&nArgErr);
	if(params.cArgs!=0){
		VARIANT* pArg=params.rgvarg+params.cArgs-1;
		const WCHAR* pb=pbParamInfo;
		while(*pb!=0){
			switch((VARTYPE)*pb){
      #ifndef _UNICODE
        case VT_BSTRA:
      #endif
        case VT_BSTR:
          VariantClear(pArg);
				break;
			}
			--pArg;++pb;
		}
	}
	delete[] params.rgvarg;
	params.rgvarg=NULL;
	if(FAILED(hr)){
    VariantClear(&vaResult);
    va_end(argList);
    return hr;
	}
	if(vtRet!=VT_EMPTY){
		if(vtRet!=VT_VARIANT){
			hr=VariantChangeType(&vaResult, &vaResult, 0, vtRet);
			if(FAILED(hr)){
				VariantClear(&vaResult);
				va_end(argList);
				return hr;
			}
			XASSERT(vtRet==vaResult.vt)
		}
		switch(vtRet){
      case VT_UI1:
        *(BYTE*)pvRet=vaResult.bVal;
      break;
      case VT_UI2:
        *(USHORT*)pvRet=vaResult.uiVal;
      break;
      case VT_UI4:
        *(ULONG*)pvRet=vaResult.ulVal;
      break;
      case VT_UI8:
        *(ULONGLONG*)pvRet=vaResult.ullVal;
      break;
      case VT_I1:
        *(char*)pvRet=vaResult.cVal;
      break;
      case VT_I2:
        *(short*)pvRet=vaResult.iVal;
      break;
      case VT_I4:
        *(long*)pvRet=vaResult.lVal;
      break;
      case VT_I8:
        *(LONGLONG*)pvRet=vaResult.llVal;
      break;
      case VT_R4:
        *(float*)pvRet=vaResult.fltVal;
      break;
      case VT_R8:
        *(double*)pvRet=vaResult.dblVal;
      break;
      case VT_DATE:
        *(double*)pvRet=*(double*)&vaResult.date;
      break;
      case VT_CY:
        *(CY*)pvRet=vaResult.cyVal;
      break;
      case VT_BSTR:
        *(BSTR*)pvRet=vaResult.bstrVal;
      break;
      case VT_DISPATCH:
        *(LPDISPATCH*)pvRet=vaResult.pdispVal;
      break;
      case VT_ERROR:
        *(SCODE*)pvRet=vaResult.scode;
      break;
      case VT_BOOL:
        *(BOOL*)pvRet=(V_BOOL(&vaResult)!=0);
      break;
      case VT_VARIANT:
        *(VARIANT*)pvRet=vaResult;
      break;
      case VT_UNKNOWN:
        *(LPUNKNOWN*)pvRet=vaResult.punkVal;
			break;
      default:
        hr=E_INVALIDARG;
      break;
		}
	}
	va_end(argList);
	return hr;
}

HRESULT XOleDispatchDriver::GetProperty(DISPID dwDispID,VARTYPE vtProp,PVOID pvProp)const{
	return InvokeHelper(dwDispID,DISPATCH_PROPERTYGET,vtProp,pvProp,NULL);
}

HRESULT XOleDispatchDriver::SetProperty(DISPID dwDispID,VARTYPE vtProp,...){
	va_list argList;
	va_start(argList,vtProp);
	WCHAR rgbParams[2];
  #ifndef _UNICODE
  if(vtProp==VT_BSTR)
    vtProp=VT_BSTRA;
  #endif
	rgbParams[0]=(WCHAR)vtProp;
	rgbParams[1]=0;
	WORD wFlags=(WORD)(vtProp==VT_DISPATCH?DISPATCH_PROPERTYPUTREF:DISPATCH_PROPERTYPUT);
	HRESULT hr=InvokeHelper(dwDispID,wFlags,VT_EMPTY,NULL,rgbParams,argList);
	va_end(argList);
	return hr;
}


// XEnumUnknown

XEnumUnknown::XEnumUnknown(LPUNKNOWN *pvEnum, UINT nSize,BOOL bNeedFree){
	m_pvEnum=pvEnum;
	m_bNeedFree=bNeedFree;
	m_pClonedFrom=NULL;
	m_nSize=nSize;
	m_nCurPos=0;
}

XEnumUnknown::~XEnumUnknown(){
  if(m_pClonedFrom!=NULL)
		m_pClonedFrom->InternalRelease();
	else{
    LPUNKNOWN* ppUnk=m_pvEnum;
		for(UINT i=0;i<m_nSize;i++){
			XASSERT(ppUnk[i]!=NULL)
        ppUnk[i]->Release();
		}
    if(m_bNeedFree)
      delete m_pvEnum;
  }
}


// XEnumUnknown::XEnum implementation

BEGIN_INTERFACE_EVENTS(XEnumUnknown,XEvent)
	INTERFACE_EVENT(XEnumUnknown,IID_IEnumUnknown,Enum)
END_INTERFACE_EVENTS()

STDMETHODIMP_(ULONG) XEnumUnknown::XEnum::AddRef(){
	XEnumUnknown *pThis=GetParentPtr();
	XASSERT(pThis!=NULL)
	return pThis->InternalAddRef();
}

STDMETHODIMP_(ULONG) XEnumUnknown::XEnum::Release(){
	XEnumUnknown *pThis=GetParentPtr();
	XASSERT(pThis!=NULL)
	return pThis->InternalRelease();
}

STDMETHODIMP XEnumUnknown::XEnum::QueryInterface(REFIID iid, LPVOID* ppvObj){
	XEnumUnknown *pThis=GetParentPtr();
	XASSERT(pThis!=NULL)
	return pThis->InternalQueryInterface(&iid, ppvObj);
}

STDMETHODIMP XEnumUnknown::XEnum::Next(ULONG celt, IUnknown** reelt, ULONG* pceltFetched){
	XEnumUnknown *pThis=GetParentPtr();
	XASSERT(pThis!=NULL)
	if(pceltFetched!=NULL)
		*pceltFetched=0;
	XASSERT(celt>0)
	XASSERT(celt==1||pceltFetched!=NULL)
  if(pThis->m_nCurPos<pThis->m_nSize){
    ULONG celtT=celt;
		while(celtT!=0&&pThis->m_nCurPos<pThis->m_nSize){
      LPUNKNOWN pUnk=pThis->m_pvEnum[pThis->m_nCurPos];
      XASSERT(pUnk!=NULL)
      memcpy(reelt,pUnk,sizeof(LPUNKNOWN));
      pUnk->AddRef();
			++pThis->m_nCurPos;
			++reelt;--celtT;
		}
		if(pceltFetched!=NULL)
			*pceltFetched=celt-celtT;
		return (celtT==0?S_OK:S_FALSE);
	}
	return E_UNEXPECTED;
}

STDMETHODIMP XEnumUnknown::XEnum::Skip(ULONG celt){
	XEnumUnknown *pThis=GetParentPtr();
	XASSERT(pThis!=NULL)
	XASSERT(celt>0)
	if((pThis->m_nCurPos+celt)<pThis->m_nSize){
    pThis->m_nCurPos+=celt;
    return S_OK;
  }
  pThis->m_nCurPos=(pThis->m_nSize>0)?pThis->m_nSize-1:0;
  return S_FALSE;
}

STDMETHODIMP XEnumUnknown::XEnum::Reset(){
	XEnumUnknown *pThis=GetParentPtr();
	XASSERT(pThis!=NULL)
	pThis->m_nCurPos=0;
	return S_OK;
}

STDMETHODIMP XEnumUnknown::XEnum::Clone(IEnumUnknown** ppenm){
  XEnumUnknown *pThis=GetParentPtr();
	XASSERT(pThis!=NULL)
	XEnumUnknown* pClone=new XEnumUnknown(pThis->m_pvEnum,pThis->m_nSize,FALSE);
	XASSERT(pClone!=NULL)
	pClone->m_nCurPos=pThis->m_nCurPos;
  if(pThis->m_pClonedFrom!=NULL)
		pClone->m_pClonedFrom=pThis->m_pClonedFrom;
	else
		pClone->m_pClonedFrom=pThis;
	pClone->m_pClonedFrom->InternalAddRef();
	*ppenm=&pClone->m_Enum;
  return S_OK;
}
