#include "xevent.h"
#include "xapp.h"
#include "xwnd.h"

/**************************************************************/
// XEvent REFERENCE...
/**************************************************************/

IMPLEMENT_DYNAMIC(XEvent,XObject)

static const UINT _StackMinSize[] ={
	0,                          // VTS_EMPTY
	0,                          // VTS_NULL
	sizeof(short),              // VTS_I2
	sizeof(long),               // VTS_I4
	sizeof(float),              // VTS_R4
	sizeof(double),             // VTS_R8
	sizeof(CY),                 // VTS_CY
	sizeof(DATE),               // VTS_DATE
	sizeof(LPCOLESTR),          // VTS_WBSTR (VT_BSTR)
	sizeof(LPDISPATCH),         // VTS_DISPATCH
	sizeof(SCODE),              // VTS_SCODE
	sizeof(BOOL),               // VTS_BOOL
	sizeof(LPVARIANT),          // VTS_VARIANT
	sizeof(LPUNKNOWN),          // VTS_UNKNOWN
	sizeof(LPCSTR),             // VTS_BSTR (VT_BSTRA -- MFC defined)
	0,
	sizeof(char),               // VTS_I1
	sizeof(BYTE),               // VTS_UI1
	sizeof(WORD),               // VTS_UI2
	sizeof(DWORD),              // VTS_UI4
	sizeof(LONGLONG),           // VTS_I8
	sizeof(ULONGLONG)           // VTS_UI8
};

BOOL XEvent::MakeEventsCache(BOOL bAppCache){
  //events cache
  if(!m_bCached){
    const XEventTable *pEventTable=GetEventTable();
    if(pEventTable){
      for(;pEventTable->GetBaseEventTable;pEventTable=(*pEventTable->GetBaseEventTable)()){
        //map redefined by it self!
        XASSERT(pEventTable!=(*pEventTable->GetBaseEventTable)())
        XEventTableEntry *pEntry=(XEventTableEntry *)pEventTable->pEntry;
        XASSERT(pEntry!=NULL)
        if(bAppCache){
          while(pEntry->uType){
            m_eventscache.push_back(pEntry);
            ++pEntry;
          }
        }else{
          while(pEntry->uType){
            switch(pEntry->uMsg){
              case WM_COMMAND://command's cache
                m_commandscache.push_back(pEntry);
              break;
              case WM_NOTIFY://notify's cache
                m_notifyscache.push_back(pEntry);
              break;
              default://other events cache
                m_eventscache.push_back(pEntry);
              break;
            }
            ++pEntry;
          }
        }
      }
      m_bCached=TRUE;
    }
  }
  return m_bCached;
}

LRESULT XEvent::CallEventProc(const XEventTableEntry *pEntry,WPARAM wParam,LPARAM lParam){
  XASSERT(pEntry!=NULL)
  //for default return 0
  LRESULT lRet=0;
  //process entry
  switch(pEntry->uType){
    case TVV:
      (this->*((tIVEvtFunc)pEntry->pEventFunc))();
    break;
    case TVW:
      (this->*((tIWEvtFunc)pEntry->pEventFunc))(wParam);
    break;
    case TVL:
      (this->*((tILEvtFunc)pEntry->pEventFunc))(lParam);
    break;
    case TVWL:
      (this->*((tVWLpEvtFunc)pEntry->pEventFunc))(wParam,lParam);
    break;
    case TIV:
      lRet=(this->*((tIVEvtFunc)pEntry->pEventFunc))();
    break;
    case TIW:
      lRet=(this->*((tIWEvtFunc)pEntry->pEventFunc))(wParam);
    break;
    case TIL:
      lRet=(this->*((tILEvtFunc)pEntry->pEventFunc))(lParam);
    break;
    default://TIWL
      lRet=(this->*((tIWLpEvtFunc)pEntry->pEventFunc))(wParam,lParam);
    break;
  }
  return lRet;
}

//we need to disable to use alloca call...
#ifdef _MSC_VER
	#pragma runtime_checks("s",off)
#endif

HRESULT XEvent::CallMemberFunc(const XEventSinkTableEntry *pEntry,WORD wFlags,VARIANT* pvarResult,DISPPARAMS* pDispParams,UINT* puArgErr){
  static const unsigned _ALIGN_STACK=sizeof(void*);
  XASSERT(pEntry!=NULL&&pEntry->pfn!=NULL)

	WCHAR bNoParams=0;
	const WCHAR* pbParams=pEntry->lpszParams;
	if(pbParams==NULL)
		pbParams=&bNoParams;
	UINT nParams=lstrlenW(pbParams);

	tXCallFunc pfn=pEntry->pfn;
	VARTYPE vtResult=pEntry->vt;

	if(wFlags&(DISPATCH_PROPERTYPUT|DISPATCH_PROPERTYPUTREF)){
		WCHAR* pbPropSetParams=(WCHAR*)alloca((nParams+3)*sizeof(WCHAR));
		XASSERT(pbPropSetParams!=NULL)
		if(!pbPropSetParams)
      return E_OUTOFMEMORY;
		XASSERT(!(pEntry->vt&VT_BYREF))
		memcpy(pbPropSetParams,pbParams,nParams*sizeof(WCHAR));
		pbParams=pbPropSetParams;
		VARTYPE vtProp=pEntry->vt;
    #ifndef _UNICODE
		if(vtProp==VT_BSTR)
			vtProp=VT_BSTRA;
    #endif
		pbPropSetParams[nParams++]=(WCHAR)VT_MARKER;
		pbPropSetParams[nParams++]=(WCHAR)vtProp;
		pbPropSetParams[nParams]=0;
		XASSERT(pEntry->pfnSet!=NULL)
		pfn=pEntry->pfnSet;
		vtResult=VT_EMPTY;
	}

	if(pDispParams->cArgs>(INT_MAX/sizeof(VARIANT)))
    return E_OUTOFMEMORY;

	VARIANT* rgTempVars=(VARIANT*)alloca(pDispParams->cArgs*sizeof(VARIANT));
	XASSERT(rgTempVars!=NULL)
	if(!rgTempVars)
    return E_OUTOFMEMORY;
  memset(rgTempVars,0,pDispParams->cArgs*sizeof(VARIANT));

	const WCHAR* pb=NULL;
	XASSERT(pbParams!=NULL)

	//Old version of compiller?
	#if defined(__GNUC__) && (__GNUC__ < 4 || (__GNUC__ == 4 && (__GNUC_MINOR__ < 7 )))
		//add space for "this" pointer... (NO THIS CALL!)
		UINT nSizeArgs=sizeof(void*);
	#else
		UINT nSizeArgs=0;
	#endif
	for(pb=pbParams;*pb!=0;++pb){
		if(*pb!=VT_MARKER){
			if(*pb&VT_BYREF){
        if((*pb&~VT_BYREF)!=VT_EMPTY&&(*pb&~VT_BYREF)!=VT_NULL)
          nSizeArgs+=sizeof(PVOID);
			}else{
				XASSERT(*pb<(sizeof(_StackMinSize)/sizeof(_StackMinSize[0])))
        nSizeArgs+=_StackMinSize[*pb];
      }
			nSizeArgs=(nSizeArgs+(_ALIGN_STACK-1))&~(_ALIGN_STACK-1);
		}
	}

  XASSERT(nSizeArgs!=0)
  BYTE* pStack=(BYTE*)alloca(nSizeArgs+2*sizeof(void*));
	XASSERT(pStack!=NULL)
  if(!pStack)
    return E_OUTOFMEMORY;
  BYTE* pStackStart=pStack;


	//Old version of compiller?
	#if defined(__GNUC__) && (__GNUC__ < 4 || (__GNUC__ == 4 && (__GNUC_MINOR__ < 7 )))
    //add "this" pointer... (NO THIS CALL!)
		*(PVOID*)pStack=this;
		pStack+=sizeof(PVOID);
	#endif

  INT iArg=pDispParams->cArgs;
  INT iArgMin=pDispParams->cNamedArgs;
  VARIANT* pArgs=pDispParams->rgvarg;
  BOOL bNamedArgs=FALSE;
  for(pb=pbParams;*pb!=0;++pb){
    VARTYPE vt=*pb;
    VARIANT* pArg;
    if(--iArg>=iArgMin){
      if(vt==VT_MARKER)
        break;
      pArg=&pArgs[iArg];
      if(vt!=VT_VARIANT&&vt!=pArg->vt){
        VARIANT* pArgTemp=&rgTempVars[iArg];
        XASSERT(pArgTemp->vt==VT_EMPTY)
        #ifdef _UNICODE
        VARTYPE vtTarget=vt;
        #else
        VARTYPE vtTarget=((vt==(USHORT)VT_BSTRA)?(USHORT)VT_BSTR:vt);
        #endif
        if(pArg->vt!=vtTarget){
          SCODE sc=VariantChangeType(pArgTemp,pArg,0,vtTarget);
          if(FAILED(sc)){
            *puArgErr=iArg;
            for(UINT iArg=0;iArg<pDispParams->cArgs;++iArg)
              VariantClear(&rgTempVars[iArg]);
            return sc;
          }
          XASSERT(pArgTemp->vt==vtTarget)
        }
        #ifndef _UNICODE
        if(vt==VT_BSTRA){
          if(pArg->vt!=vtTarget){
            XASSERT(pArgTemp->vt==VT_BSTR)
            BSTR bstrW=pArgTemp->bstrVal;
            pArgTemp->bstrVal=CWBSTR2ABSTR(bstrW,CP_ACP,FALSE);
            SysFreeString(bstrW);
          }else{
            pArgTemp->bstrVal=CWBSTR2ABSTR(pArg->bstrVal,CP_ACP,FALSE);
            pArgTemp->vt=VT_BSTR;
          }
          vt=VT_BSTR;
        }
        #endif
        pArg=pArgTemp;
      }
    }else{
      if(vt==VT_MARKER){
        iArg=pDispParams->cNamedArgs;
        iArgMin=0;
        bNamedArgs=TRUE;
        continue;
      }
      if(bNamedArgs||vt!=VT_VARIANT)
        break;
      static VARIANT vaDefault;
      vaDefault.vt=VT_ERROR;
      vaDefault.scode=DISP_E_PARAMNOTFOUND;
      pArg=&vaDefault;
    }

    if(vt&VT_BYREF){
      if((vt&~VT_BYREF)!=VT_EMPTY&&(vt&~VT_BYREF)!=VT_NULL){
        *(PVOID*)pStack=pArg->byref;
        pStack+=sizeof(PVOID);
      }
    }else{
      switch(vt){
        case VT_I1:
          *(LONG*)pStack=pArg->cVal;
        break;
        case VT_UI1:
          *(ULONG*)pStack=pArg->bVal;
        break;
        case VT_I2:
          *(LONG*)pStack=pArg->iVal;
        break;
        case VT_UI2:
          *(ULONG*)pStack=pArg->uiVal;
        break;
        case VT_I4:
          *(LONG*)pStack=pArg->lVal;
        break;
        case VT_UI4:
          *(ULONG*)pStack=pArg->ulVal;
        break;
        case VT_I8:
          *(LONGLONG*)pStack=pArg->llVal;
        break;
        case VT_UI8:
          *(ULONGLONG*)pStack=pArg->ullVal;
        break;
        case VT_R4:
          *(float*)pStack=pArg->fltVal;
        break;
        case VT_R8:
          *(double*)pStack=pArg->dblVal;
        break;
        case VT_DATE:
          *(DATE*)pStack=pArg->date;
        break;
        case VT_CY:
          *(CY*)pStack=pArg->cyVal;
        break;
        case VT_BSTR:
          *(PVOID*)pStack=(PVOID)pArg->bstrVal;
        break;
        case VT_ERROR:
          *(LONG*)pStack=pArg->scode;
        break;
        case VT_BOOL:
          *(LONG*)pStack=(V_BOOL(pArg)!=0);
        break;
        case VT_VARIANT:
          *(PVOID*)pStack=(PVOID)pArg;
        break;
        case VT_DISPATCH:
        case VT_UNKNOWN:
          *(PVOID*)pStack=(PVOID)pArg->punkVal;
        break;
        default:
          XASSERT(FALSE)
        break;
      }
      XASSERT(vt<(sizeof(_StackMinSize)/sizeof(_StackMinSize[0])))
      pStack+=_StackMinSize[vt];//min size needed, now, align...
    }
    pStack=(BYTE*)(((DWORD_PTR)pStack+(_ALIGN_STACK-1))&~(_ALIGN_STACK-1));
    XASSERT(((DWORD_PTR)pStack&(_ALIGN_STACK-1))==0)
  }
  if(iArg>0){
    *puArgErr=iArg;
    for(UINT iArg=0;iArg<pDispParams->cArgs;++iArg)
      VariantClear(&rgTempVars[iArg]);
    return DISP_E_BADPARAMCOUNT;
  }
  if(*pb!=0){
    *puArgErr=pDispParams->cArgs;
    for(UINT iArg=0;iArg<pDispParams->cArgs;++iArg)
      VariantClear(&rgTempVars[iArg]);
    return DISP_E_PARAMNOTOPTIONAL;
  }

	VARIANT result;
	result.vt=vtResult;
  switch(result.vt){
    case VT_R4:
      #ifdef __GNUC__
      __asm__ ("mov %0,%%esp"::"m"(pStackStart));
      #else
      __asm { mov esp, pStackStart }
      #endif
      result.fltVal=(this->*((float(XEvent::*)())pfn))();
    break;
    case VT_R8:
    case VT_DATE:
      #ifdef __GNUC__
      __asm__ ("mov %0,%%esp"::"m"(pStackStart));
      #else
      __asm { mov esp, pStackStart }
      #endif
      result.dblVal=(this->*((double(XEvent::*)())pfn))();
    break;
    case VT_I8:
    case VT_UI8:
    case VT_CY:
      #ifdef __GNUC__
      __asm__ ("mov %0,%%esp"::"m"(pStackStart));
      #else
      __asm { mov esp, pStackStart }
      #endif
      result.ullVal=(this->*((ULONGLONG(XEvent::*)())pfn))();
    break;
    default:
			#ifdef __GNUC__
      __asm__ ("mov %0,%%esp"::"m"(pStackStart));
      #else
      __asm { mov esp, pStackStart }
      #endif
      result.lVal=(this->*((DWORD_PTR(XEvent::*)())pfn))();
    break;
  }

	for(UINT iArg=0;iArg<pDispParams->cArgs;++iArg)
		VariantClear(&rgTempVars[iArg]);

	if(pvarResult!=NULL){
    pvarResult->vt=result.vt;
		switch(result.vt){
      case VT_I1:case VT_UI1:
        pvarResult->bVal=(BYTE)result.lVal;
			break;
      case VT_I2:case VT_UI2:
        pvarResult->iVal=(SHORT)result.lVal;
			break;
      case VT_I4:case VT_UI4:
        pvarResult->lVal=(LONG)result.lVal;
			break;
      case VT_I8:case VT_UI8:
        pvarResult->ullVal=result.ullVal;
			break;
      case VT_R4:
        pvarResult->fltVal=result.fltVal;
			break;
      case VT_R8:
        pvarResult->dblVal=result.dblVal;
			break;
      case VT_CY:
        pvarResult->cyVal=result.cyVal;
			break;
      case VT_DATE:
        pvarResult->date=result.dblVal;
			break;
      case VT_BSTR:
        pvarResult->bstrVal=(BSTR)(LONG_PTR)result.lVal;
			break;
      case VT_ERROR:
        pvarResult->scode=(SCODE)result.lVal;
			break;
      case VT_BOOL:
        V_BOOL(pvarResult)=((VARIANT_BOOL)result.lVal!=0?VARIANT_TRUE:VARIANT_FALSE);
      break;
      case VT_VARIANT:
        *pvarResult=result;
			break;
      case VT_DISPATCH:
      case VT_UNKNOWN:
        pvarResult->punkVal=(LPUNKNOWN)(LONG_PTR)result.lVal;
			break;
		}
	}else{
		switch(vtResult){
      case VT_BSTR:
        ::SysFreeString((BSTR)(LONG_PTR)result.lVal);
			break;
      case VT_DISPATCH:
      case VT_UNKNOWN:
        if((LPUNKNOWN)(LONG_PTR)result.lVal!=0)
          ((LPUNKNOWN)(LONG_PTR)result.lVal)->Release();
			break;
      case VT_VARIANT:
        VariantClear(&result);
			break;
		}
	}
	return S_OK;
}

//we need to disable to use alloca call...
#ifdef _MSC_VER
	#pragma runtime_checks("s",restore)
#endif

//manual definition of event entries
const XEventTableEntry XEvent::_entries[]={{0,0,0,0,0,NULL,-1}};

const XEventTable* XEvent::GetThisEventTable(){
  static const XEventTable _etable={NULL,_entries};
  return &_etable;
}

LPUNKNOWN XEvent::GetInterface(const VOID *iid){
  const XInterfaceEventTable* pIEventTable=GetInterfaceEventTable();
  XASSERT(pIEventTable!=NULL)
  DWORD lData1=((IID*)iid)->Data1;
	// IUnknown is a special case since nobody really implements *only* it!
	if(((DWORD*)&IID_IUnknown)[0]==lData1&&((DWORD*)&IID_IUnknown)[1]==((DWORD*)iid)[1]&&
    ((DWORD*)&IID_IUnknown)[2]==((DWORD*)iid)[2]&&((DWORD*)&IID_IUnknown)[3]==((DWORD*)iid)[3]){
    for(;pIEventTable->GetBaseEventTable;pIEventTable=(*pIEventTable->GetBaseEventTable)()){
      //map redefined by it self!
      XASSERT(pIEventTable!=(*pIEventTable->GetBaseEventTable)())
      XInterfaceEventTableEntry *pEntry=(XInterfaceEventTableEntry *)pIEventTable->pEntry;
      XASSERT(pEntry!=NULL)
      while(pEntry->piid){
				LPUNKNOWN lpUnk=GetInterfacePtr(this,pEntry);
				if(*(DWORD*)lpUnk!=0)
					return lpUnk;
        ++pEntry;
      }
    }
		return NULL;
	}
	for(;pIEventTable->GetBaseEventTable;pIEventTable=(*pIEventTable->GetBaseEventTable)()){
	  //map redefined by it self!
    XASSERT(pIEventTable!=(*pIEventTable->GetBaseEventTable)())
    XInterfaceEventTableEntry *pEntry=(XInterfaceEventTableEntry *)pIEventTable->pEntry;
    XASSERT(pEntry!=NULL)
    while(pEntry->piid){
      if(((DWORD*)pEntry->piid)[0]==lData1&&((DWORD*)pEntry->piid)[1]==((DWORD*)iid)[1]&&
				((DWORD*)pEntry->piid)[2]==((DWORD*)iid)[2]&&((DWORD*)pEntry->piid)[3]==((DWORD*)iid)[3]){
        LPUNKNOWN lpUnk=GetInterfacePtr(this,pEntry);
				if(*(DWORD*)lpUnk!=0)
					return lpUnk;
      }
      ++pEntry;
    }
  }
	return NULL;
}

LPUNKNOWN XEvent::QueryAggregates(const VOID *iid){
  const XInterfaceEventTable* pIEventTable=GetInterfaceEventTable();
  XASSERT(pIEventTable!=NULL)
  for(;pIEventTable->GetBaseEventTable;pIEventTable=(*pIEventTable->GetBaseEventTable)()){
    //map redefined by it self!
    XASSERT(pIEventTable!=(*pIEventTable->GetBaseEventTable)())
    XInterfaceEventTableEntry *pEntry=(XInterfaceEventTableEntry *)pIEventTable->pEntry;
    XASSERT(pEntry!=NULL)
    while(pEntry->piid!=NULL)
			++pEntry;
    while(pEntry->nOffset!=(size_t)-1){
			LPUNKNOWN lpQuery=GetInterfacePtr(this,pEntry);
			if(lpQuery!=NULL){
				LPUNKNOWN lpUnk=NULL;
				if((lpQuery->QueryInterface(*(IID*)iid,(PVOID*)&lpUnk)==S_OK)&&(lpUnk!=NULL))
					return lpUnk;
			}
			++pEntry;
		}
  }
	return NULL;
}

DWORD XEvent::InternalQueryInterface(const VOID *iid,LPVOID* ppvObj){
	if((*ppvObj=GetInterface(iid))!=NULL){
		InternalAddRef();
		return S_OK;
	}
	if((*ppvObj=QueryAggregates(iid))!=NULL)
		return S_OK;
	return (DWORD)E_NOINTERFACE;
}

DWORD XEvent::InternalAddRef(){
  XASSERT(GetInterfaceEventTable()!=NULL)
  // Not implemented in Mingw64 5.1
  #if defined(__MINGW64__)
    return ++m_dwRef;
  #else
    return InterlockedIncrement(&m_dwRef);
  #endif
}

DWORD XEvent::InternalRelease(){
  XASSERT(GetInterfaceEventTable()!=NULL)
	if(!m_dwRef)
		return 0;
  // Not implemented in Mingw64 5.1
  #if defined(__MINGW64__)
    return --m_dwRef;
  #else
    return InterlockedDecrement(&m_dwRef);
	#endif
}

const XInterfaceEventTableEntry XEvent::_ientries[]={{NULL,(size_t)-1}};

const XInterfaceEventTable* XEvent::GetThisInterfaceEventTable(){
  static const XInterfaceEventTable _ietable={NULL,_ientries};
  return &_ietable;
}

const XEventSinkTableEntry* XEvent::GetEventSinkEntry(UINT idCtrl,XSINKEVENT* pEvent){
  XASSERT(pEvent!=NULL)
  const XEventSinkTable* pSEventTable=GetEventSinkTable();
  size_t flag=(pEvent->m_eventKind!=XSINKEVENT::XS_EVENT);
  XASSERT(pSEventTable!=NULL)
  for(;pSEventTable->GetBaseEventTable;pSEventTable=(*pSEventTable->GetBaseEventTable)()){
    XASSERT(pSEventTable!=(*pSEventTable->GetBaseEventTable)())
    PXEventSinkTableEntry pEntry=(PXEventSinkTableEntry)pSEventTable->pEntry;
    XASSERT(pEntry!=NULL)
    while(pEntry->nPropOffset!=(size_t)-1){
      if((pEntry->lDispID==pEvent->m_dispid)&&(pEntry->nPropOffset==flag)){
        if(pEntry->uMaxId==(UINT)-1){
          if((pEntry->uMinId==(UINT)-1)||(pEntry->uMinId==idCtrl))
            return pEntry;
        }else{
          if((pEntry->uMinId<=idCtrl)&&(idCtrl<=pEntry->uMaxId))
            return pEntry;
        }
      }
      ++pEntry;
		}
  }
	return NULL;
}

const XEventSinkTableEntry XEvent::_sentries[]={{0,NULL,0,NULL,NULL,(size_t)-1,0,0}};

const XEventSinkTable* XEvent::GetThisEventSinkTable(){
  static const XEventSinkTable _setable={NULL,_sentries};
  return &_setable;
}
