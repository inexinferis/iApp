#ifndef _XTL_H_
#define _XTL_H_

#include "xobject.h"
#include "xutils.h"
#include "xevent.h"
#include <ole2.h>
#include <olectl.h>
#include <mbstring.h>
#include <shlobj.h>
#include <limits.h>

class XComBSTR{
  public:
    XComBSTR():m_str(NULL){}
    ~XComBSTR(){}

    XComBSTR(INT nSize,LPCOLESTR sz=NULL){
      XASSERT(nSize>=0)
      if(nSize==0)
        m_str=NULL;
      else{
        m_str=::SysAllocStringLen(sz,nSize);
        XASSERT(m_str!=NULL)
      }
    }

    XComBSTR(LPCOLESTR pSrc){
      if(pSrc==NULL)
        m_str=NULL;
      else{
        m_str=::SysAllocString(pSrc);
        XASSERT(m_str!=NULL)
      }
    }

    XComBSTR(XComBSTR& src){
      m_str=src.Copy();
    }

    XComBSTR(REFGUID guid){
      OLECHAR szGUID[64];
      ::StringFromGUID2(guid,szGUID,64);
      m_str=::SysAllocString(szGUID);
      XASSERT(m_str!=NULL)
    }

    XComBSTR(LPCSTR pSrc){
      if(pSrc!=NULL){
        m_str=CA2WBSTR(pSrc,CP_ACP,FALSE);
        XASSERT(m_str!=NULL)
      }else
        m_str=NULL;
    }

    XComBSTR(INT nSize,LPCSTR sz){
      XASSERT(nSize>0)
      if(nSize!=0&&sz==NULL){
        m_str=::SysAllocStringLen(NULL,nSize);
        XASSERT(m_str!=NULL)
        return;
      }
      m_str=CA2WBSTR(sz,CP_ACP,FALSE);
      XASSERT(m_str!=NULL)
    }

    XComBSTR& operator=(XComBSTR& src){
      if(m_str!=src.m_str){
        if(m_str)
          ::SysFreeString(m_str);
        m_str=src.Copy();
      }
      return *this;
    }

    XComBSTR& operator=(LPCOLESTR pSrc){
      if(m_str!=pSrc){
        if(m_str)
          ::SysFreeString(m_str);
        if(pSrc!=NULL){
          m_str=::SysAllocString(pSrc);
          XASSERT(m_str!=NULL)
        }else
          m_str=NULL;
      }
      return *this;
    }

    XComBSTR& operator=(LPCSTR pSrc){
      if(m_str)
        ::SysFreeString(m_str);
      if(pSrc!=NULL){
        m_str=CA2WBSTR(pSrc,CP_ACP,FALSE);
        XASSERT(m_str!=NULL)
      }else
        m_str=NULL;
      return *this;
    }

    UINT Length()const{
      if(m_str)
        return ::SysStringLen(m_str);
      return 0;
    }

    UINT ByteLength()const{
      if(m_str)
        return ::SysStringByteLen(m_str);
      return 0;
    }

    operator BSTR() const {
      return m_str;
    }

    BSTR* operator&(){
      return &m_str;
    }

    BSTR Copy()const{
      if(m_str!=NULL)
        return ::SysAllocStringByteLen((char*)m_str,::SysStringByteLen(m_str));
      else
        return ::SysAllocStringByteLen(NULL,0);
    }

    HRESULT CopyTo(BSTR* pbstr) const {
      XASSERT(pbstr!=NULL)
      if(pbstr==NULL)
        return E_POINTER;
      *pbstr=Copy();
      if((*pbstr==NULL)&&(m_str!=NULL))
        return E_OUTOFMEMORY;
      return S_OK;
    }

    HRESULT CopyTo(VARIANT *pvarDest)const{
      XASSERT(pvarDest!=NULL)
      HRESULT hRes=E_POINTER;
      if(pvarDest!=NULL){
        pvarDest->vt=VT_BSTR;
        pvarDest->bstrVal=Copy();
        if(pvarDest->bstrVal==NULL&&m_str!=NULL)
          hRes=E_OUTOFMEMORY;
        else
          hRes=S_OK;
      }
      return hRes;
    }

    VOID Attach(BSTR src){
      if(m_str!=src){
        if(m_str)
          ::SysFreeString(m_str);
        m_str=src;
      }
    }

    BSTR Detach(){
      BSTR s=m_str;
      m_str=NULL;
      return s;
    }

    VOID Free(){
      if(m_str){
        ::SysFreeString(m_str);
        m_str=NULL;
      }
    }

    BOOL operator!()const{
      return (m_str==NULL);
    }

    HRESULT AppendBSTR(BSTR p){
      if(::SysStringLen(p)==0)
        return S_OK;
      BSTR bstrNew=NULL;
      HRESULT hr=::VarBstrCat(m_str,p,&bstrNew);
      if(SUCCEEDED(hr)){
        if(m_str)
          ::SysFreeString(m_str);
        m_str=bstrNew;
      }
      return hr;
    }

    HRESULT Append(XComBSTR& bstrSrc){
      return AppendBSTR(bstrSrc.m_str);
    }

    HRESULT Append(LPCOLESTR lpsz,INT nLen){
      if(lpsz==NULL||(m_str!=NULL&&nLen==0))
        return S_OK;
      else if(nLen<0)
        return E_INVALIDARG;
      UINT nOldLen=Length();
      BSTR b=::SysAllocStringLen(NULL,nOldLen+nLen);
      if(b==NULL)
        return E_OUTOFMEMORY;
      if(nOldLen){
        memcpy(b,m_str,nOldLen*sizeof(OLECHAR));
        ::SysFreeString(m_str);
      }
      memcpy(&b[nOldLen],lpsz,nLen*sizeof(OLECHAR));
      b[nOldLen+nLen]='\0';
      m_str=b;
      return S_OK;
    }

    HRESULT Append(LPCOLESTR lpsz){
      return Append(lpsz,wcslen(lpsz));
    }

    HRESULT Append(CHAR ch){
      OLECHAR chO=ch;
      return Append(&chO,1);
    }

    HRESULT Append(WCHAR ch){
      return Append(&ch,1);
    }

    HRESULT Append(LPCSTR lpsz){
      if(lpsz==NULL)
        return S_OK;
      XComBSTR bstrTemp(lpsz);
      return Append(bstrTemp);
    }

    HRESULT AppendBytes(PBYTE lpsz,INT nLen){
      if(lpsz==NULL||nLen==0)
        return S_OK;
      else if(nLen<0)
        return E_INVALIDARG;
      const UINT nOldLen=ByteLength();
      BSTR b=::SysAllocStringByteLen(NULL,nOldLen+nLen);
      if(b==NULL)
        return E_OUTOFMEMORY;
      if(nOldLen){
        memcpy(b,m_str,nOldLen);
        ::SysFreeString(m_str);
      }
      memcpy(&((char*)b)[nOldLen],lpsz,nLen);
      ((char*)b)[nOldLen+nLen]='\0';
      m_str=b;
      return S_OK;
    }

    HRESULT AssignBSTR(const BSTR bstrSrc){
      HRESULT hr = S_OK;
      if(m_str!=bstrSrc){
        if(m_str)
          ::SysFreeString(m_str);
        if(bstrSrc!=NULL){
          m_str=::SysAllocStringByteLen((char*)bstrSrc,::SysStringByteLen(bstrSrc));
          XASSERT(m_str!=NULL)
        }else
          m_str=NULL;
      }
      return hr;
    }

    HRESULT ToLower() {
      if(::SysStringLen(m_str)>0)
        ::CharLowerBuffW(m_str,Length());
      return S_OK;
    }

    HRESULT ToUpper(){
      if(::SysStringLen(m_str)>0)
        CharUpperBuffW(m_str,Length());
      return S_OK;
    }

    BOOL LoadString(HINSTANCE hInst,UINT nID){
      if(m_str){
        ::SysFreeString(m_str);
        m_str=NULL;
      }
      return LoadStringResource(hInst,nID,m_str);
    }

    BOOL LoadString(UINT nID){
      if(m_str){
        ::SysFreeString(m_str);
        m_str=NULL;
      }
      return LoadStringResource(NULL,nID,m_str);
    }

    XComBSTR& operator+=(XComBSTR& bstrSrc){
      HRESULT hr=AppendBSTR(bstrSrc.m_str);
      XASSERT(SUCCEEDED(hr))
      XUNUSED(hr)
      return *this;
    }

    XComBSTR& operator+=(LPCOLESTR pszSrc){
      HRESULT hr=Append(pszSrc);
      XASSERT(SUCCEEDED(hr))
      XUNUSED(hr)
      return *this;
    }

    XComBSTR& operator+=(LPCSTR lpsz){
      HRESULT hr=Append(lpsz);
      XASSERT(SUCCEEDED(hr))
      XUNUSED(hr)
      return *this;
    }

    bool operator==(const XComBSTR& bstrSrc)const{
      return ::VarBstrCmp(m_str,bstrSrc.m_str,LOCALE_USER_DEFAULT,0)==static_cast<HRESULT>(VARCMP_EQ);
    }

    bool operator==(LPCOLESTR pszSrc) const{
      XComBSTR bstr2(pszSrc);
      return operator==(bstr2);
    }

    bool operator==(LPOLESTR pszSrc) const{
      return operator==((LPCOLESTR)pszSrc);
    }

    bool operator==(int nNull) const{
      XASSERT(nNull==0)
      return (m_str==(BSTR)(INT_PTR)nNull);
    }

    bool operator==(LPCSTR pszSrc) const{
      XComBSTR bstr2(pszSrc);
      return operator==(bstr2);
    }

    bool operator!=(const XComBSTR& bstrSrc) const{
      return !operator==(bstrSrc);
    }

    bool operator!=(LPCOLESTR pszSrc) const{
      return !operator==(pszSrc);
    }

    bool operator!=(int nNull) const{
      return !operator==(nNull);
    }

    bool operator!=(LPOLESTR pszSrc) const{
      return operator!=((LPCOLESTR)pszSrc);
    }

    bool operator!=(LPCSTR pszSrc) const{
      return !operator==(pszSrc);
    }

    bool operator<(const XComBSTR& bstrSrc) const{
      return ::VarBstrCmp(m_str,bstrSrc.m_str,LOCALE_USER_DEFAULT,0)==static_cast<HRESULT>(VARCMP_LT);
    }

    bool operator<(LPCOLESTR pszSrc) const{
      XComBSTR bstr2(pszSrc);
      return operator<(bstr2);
    }

    bool operator<(LPOLESTR pszSrc) const{
      return operator<((LPCOLESTR)pszSrc);
    }

    bool operator<(LPCSTR pszSrc) const{
      XComBSTR bstr2(pszSrc);
      return operator<(bstr2);
    }

    bool operator>(const XComBSTR& bstrSrc) const{
      return ::VarBstrCmp(m_str,bstrSrc.m_str,LOCALE_USER_DEFAULT,0)==static_cast<HRESULT>(VARCMP_GT);
    }

    bool operator>(LPCOLESTR pszSrc) const{
      XComBSTR bstr2(pszSrc);
      return operator>(bstr2);
    }

    bool operator>(LPOLESTR pszSrc) const{
      return operator>((LPCOLESTR)pszSrc);
    }

    bool operator>(LPCSTR pszSrc) const{
      XComBSTR bstr2(pszSrc);
      return operator>(bstr2);
    }

    HRESULT WriteToStream(IStream* pStream){
      XASSERT(pStream!=NULL)
      if(pStream==NULL)
        return E_INVALIDARG;
      ULONG cb;
      ULONG cbStrLen=XComBSTR::GetStreamSize(m_str);
      XASSERT(cbStrLen>=sizeof(ULONG))
      cbStrLen-=sizeof(ULONG);
      HRESULT hr=pStream->Write((void*)&cbStrLen,sizeof(cbStrLen),&cb);
      if(FAILED(hr))
        return hr;
      if(cbStrLen==0)
        return S_OK;
      return pStream->Write((void*)m_str,cbStrLen,&cb);
    }

    HRESULT ReadFromStream(IStream* pStream){
      XASSERT(m_str==NULL&&pStream!=NULL)
      if(m_str!=NULL||pStream==NULL)
        return E_INVALIDARG;
      ULARGE_INTEGER nBegOffset;
      LARGE_INTEGER nZeroOffset;
      nZeroOffset.QuadPart=0L;
      HRESULT hrSeek=pStream->Seek(nZeroOffset,STREAM_SEEK_CUR,&nBegOffset);
      ULONG cbRead=0;
      ULONG cbStrLen=0;
      HRESULT hr=pStream->Read(reinterpret_cast<void*>(&cbStrLen),sizeof(cbStrLen),&cbRead);
      if(SUCCEEDED(hr)){
        if(sizeof(cbStrLen)!=cbRead||cbStrLen<sizeof(OLECHAR))
          hr=E_FAIL;
        else if(cbStrLen>0x100000)
          hr=E_ACCESSDENIED;
        else{
          m_str=::SysAllocStringByteLen(NULL,cbStrLen);
          if(!m_str)
            hr=E_OUTOFMEMORY;
          else{
            hr=pStream->Read(reinterpret_cast<void*>(m_str),cbStrLen,&cbRead);
            if(SUCCEEDED(hr)){
              if(cbRead!=cbStrLen)
                hr=E_FAIL;
              else if(m_str[cbRead/sizeof(OLECHAR)]!=0)
                hr=E_FAIL;
            }
            if(FAILED(hr)){
              ::SysFreeString(m_str);
              m_str=NULL;
            }
          }
        }
      }
      if(FAILED(hr)&&SUCCEEDED(hrSeek)){
        LARGE_INTEGER nOffset;
        nOffset.QuadPart=nBegOffset.QuadPart;
        pStream->Seek(nOffset,STREAM_SEEK_SET,NULL);
      }
      return hr;
    }

    HRESULT BSTRToArray(LPSAFEARRAY *ppArray){
      if(m_str)
        return VectorFromBstr(m_str,ppArray);
      return E_FAIL;
    }

    HRESULT ArrayToBSTR(const SAFEARRAY *pSrc){
      if(m_str)
        ::SysFreeString(m_str);
      return BstrFromVector((LPSAFEARRAY)pSrc, &m_str);
    }

    static BOOL LoadStringResource(HINSTANCE hInstance,UINT uID,BSTR& bstrText){
      XASSERT(bstrText==NULL)
      if(bstrText!=NULL)
        return FALSE;
      LPWSTR pBuf=NULL;
      INT nLength=::LoadStringW(hInstance,uID,(LPWSTR)&pBuf,0);
      if(nLength)
        bstrText=::SysAllocStringLen(pBuf,nLength);
      return (bstrText!=NULL)?TRUE:FALSE;
    }

    static ULONG GetStreamSize(BSTR bstr){
      ULONG ulSize=sizeof(ULONG);
      if(bstr!=NULL)
        ulSize+=SysStringByteLen(bstr)+sizeof(OLECHAR);
      return ulSize;
    }

	BSTR m_str;
};

class XOleVariant : public tagVARIANT{
  public:
    XOleVariant(){
      ZeroMemory(this,sizeof(*this));
    }

    ~XOleVariant(){
      Clear();
    }

    XOleVariant(const VARIANT& varSrc){
      ZeroMemory(this,sizeof(*this));
      ::VariantCopy(this,(LPVARIANT)&varSrc);
    }

    XOleVariant(const VARIANT* pSrc){
      ZeroMemory(this,sizeof(*this));
      ::VariantCopy(this,(LPVARIANT)pSrc);
    }

    XOleVariant(const XOleVariant& varSrc){
      ZeroMemory(this,sizeof(*this));
      ::VariantCopy(this,(LPVARIANT)&varSrc);
    }

    XOleVariant(LPCTSTR lpszSrc){
      vt=VT_EMPTY;*this=lpszSrc;
    }

    XOleVariant(BYTE nSrc){
      vt=VT_UI1;bVal=nSrc;
    }

    XOleVariant(LPCTSTR lpszSrc,VARTYPE vtSrc){
      #ifdef _UNICODE
      XASSERT(vtSrc==VT_BSTR)
      #else
      XASSERT(vtSrc==VT_BSTR||vtSrc==VT_BSTRT)
      #endif
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

    XOleVariant(PBYTE pData,INT_PTR nSize){
      SetByteArray(pData,nSize);
    }

    XOleVariant(const HGLOBAL &hData,ULONG dwDataLength){
      SetLongBinary(hData,dwDataLength);
    }

    XOleVariant(SHORT nSrc,VARTYPE vtSrc=VT_I2){
      XASSERT(vtSrc==VT_I2||vtSrc==VT_BOOL)
      if(vtSrc==VT_BOOL){
        vt=VT_BOOL;
        V_BOOL(this)=(!nSrc)?VARIANT_FALSE:VARIANT_TRUE;
      }else{
        vt=VT_I2;
        iVal=nSrc;
      }
    }

    XOleVariant(LONG lSrc, VARTYPE vtSrc = VT_I4){
      XASSERT(vtSrc==VT_I4||vtSrc==VT_ERROR||vtSrc==VT_BOOL||
        vtSrc==VT_UINT||vtSrc==VT_INT||vtSrc==VT_UI4||vtSrc==VT_HRESULT);
      switch(vtSrc){
        case VT_ERROR:
          vt=VT_ERROR;
          scode=lSrc;
        break;
        case VT_BOOL:
          vt=VT_BOOL;
          V_BOOL(this)=(!lSrc)?VARIANT_FALSE:VARIANT_TRUE;
        break;
        case VT_INT:
          vt=VT_INT;
          V_INT(this)=lSrc;
        break;
        case VT_UINT:
          vt=VT_UINT;
          V_UINT(this)=lSrc;
        break;
        case VT_HRESULT:
          vt=VT_HRESULT;
          V_ERROR(this)=lSrc;
        break;
        case VT_UI4:
          vt=VT_UI4;
          lVal=lSrc;
        break;
        default:
          vt=VT_I4;
          lVal=lSrc;
        break;
      }
    }

  #if (_WIN32_WINNT >= 0x0501)

    XOleVariant(LONGLONG nSrc){
      vt=VT_I8;llVal=nSrc;
    }

    XOleVariant(ULONGLONG nSrc){
      vt=VT_UI8;ullVal=nSrc;
    }

  #endif

    XOleVariant(float fltSrc){
      vt=VT_R4;fltVal=fltSrc;
    }

    XOleVariant(double &dblSrc){
      vt=VT_R8;dblVal=dblSrc;
    }

    XOleVariant(const DATE &m_dt){
      vt=VT_DATE;date=m_dt;
    }

    XOleVariant(const CURRENCY &m_cur){
      vt=VT_CY;cyVal=m_cur;
    }

    XOleVariant(LPCITEMIDLIST pidl){
      ZeroMemory(this,sizeof(*this));
      if(pidl!=NULL){
        UINT cbTotal=0;
        SAFEARRAY *psa=NULL;
        LPCITEMIDLIST pidlWalker=pidl;
        while (pidlWalker->mkid.cb){
          cbTotal+=pidlWalker->mkid.cb;
          pidlWalker=(LPCITEMIDLIST)(((LPBYTE)pidlWalker)+pidlWalker->mkid.cb);
        }
        cbTotal+=sizeof(ITEMIDLIST);
        psa = SafeArrayCreateVector(VT_UI1, 0, cbTotal);
        if(psa!=NULL){
          memcpy(psa->pvData,(LPBYTE)pidl,cbTotal);
          vt=VT_ARRAY|VT_UI1;
          parray=psa;
        }
      }
    }

    XOleVariant(const SAFEARRAY& saSrc, VARTYPE vtSrc){
      ZeroMemory(this,sizeof(*this));
      vt=(VARTYPE)(vtSrc|VT_ARRAY);
      ::SafeArrayCopy((LPSAFEARRAY)&saSrc,&parray);
    }

    XOleVariant(const SAFEARRAY *pSrc, VARTYPE vtSrc){
      ZeroMemory(this,sizeof(*this));
      vt=(VARTYPE)(vtSrc | VT_ARRAY);
      ::SafeArrayCopy((LPSAFEARRAY)pSrc,&parray);
    }

    BOOL operator==(const VARIANT& var)const{
      if(&var==this)
        return TRUE;
      if(var.vt!=vt)
        return FALSE;
      switch (vt){
        case VT_EMPTY:case VT_NULL:
          return TRUE;
        case VT_BOOL:
          return V_BOOL(&var)==V_BOOL(this);
        case VT_I1:
          return var.cVal==cVal;
        case VT_UI1:
          return var.bVal==bVal;
        case VT_I2:
          return var.iVal==iVal;
        case VT_UI2:
          return var.uiVal==uiVal;
        case VT_I4:
          return var.lVal==lVal;
        case VT_UI4:
          return var.ulVal==ulVal;
        case VT_I8:
          return var.llVal==llVal;
        case VT_UI8:
          return var.ullVal==ullVal;
        case VT_CY:
          return (var.cyVal.Hi==cyVal.Hi&&var.cyVal.Lo==cyVal.Lo);
        case VT_R4:
          return var.fltVal==fltVal;
        case VT_R8:
          return var.dblVal==dblVal;
        case VT_DATE:
          return var.date==date;
        case VT_BSTR:
          return SysStringByteLen(var.bstrVal)==SysStringByteLen(bstrVal)&&memcmp(var.bstrVal,bstrVal,SysStringByteLen(bstrVal))==0;
        case VT_ERROR:
          return var.scode==scode;
        case VT_DISPATCH:case VT_UNKNOWN:
          return var.punkVal==punkVal;
        default:
          if(V_ISARRAY(this)&&!(vt&VT_BYREF))
            return CompareSafeArrays(var.parray, parray);
          else
            XASSERT(FALSE)
        break;
      }
      return FALSE;
    }

    BOOL operator==(const VARIANT* pSrc)const{
      return (*this==*pSrc);
    }

    BOOL operator==(const SAFEARRAY& saSrc)const{
      XASSERT(V_ISARRAY(this))
      if(V_ISARRAY(this))
        return CompareSafeArrays(parray,(LPSAFEARRAY)&saSrc);
      return FALSE;
    }

    BOOL operator==(const SAFEARRAY *pSrc)const{
      XASSERT(V_ISARRAY(this))
      if(V_ISARRAY(this))
        return CompareSafeArrays(parray,(LPSAFEARRAY)pSrc);
      return FALSE;
    }

    const XOleVariant& operator=(const VARIANT& varSrc){
      if(static_cast<LPVARIANT>(this)!=&varSrc)
        ::VariantCopy(this,(LPVARIANT)&varSrc);
      return *this;
    }

    const XOleVariant& operator=(const VARIANT* pSrc){
      if(static_cast<const VARIANT*>(this)!=pSrc)
        ::VariantCopy(this,(LPVARIANT)pSrc);
      return *this;
    }

    const XOleVariant& operator=(const XOleVariant& varSrc){
      if(this != &varSrc)
        ::VariantCopy(this,(LPVARIANT)&varSrc);
      return *this;
    }

    const XOleVariant& operator=(const LPCTSTR lpszSrc){
      Clear();
      vt=VT_BSTR;
      if(lpszSrc==NULL)
        bstrVal=NULL;
      else
        bstrVal=XComBSTR(lpszSrc);
      return *this;
    }

    const XOleVariant& operator=(BYTE nSrc){
      if (vt != VT_UI1){
        Clear();
        vt=VT_UI1;
      }
      bVal=nSrc;
      return *this;
    }

    const XOleVariant& operator=(short nSrc){
      if(vt==VT_I2)
        iVal=nSrc;
      else if(vt==VT_BOOL)
        V_BOOL(this)=(!nSrc)?VARIANT_FALSE:VARIANT_TRUE;
      else{
        Clear();
        vt=VT_I2;
        iVal=nSrc;
      }
      return *this;
    }

    const XOleVariant& operator=(long lSrc){
      if(vt==VT_I4)
        lVal=lSrc;
      else if(vt==VT_ERROR)
        scode=lSrc;
      else if(vt==VT_BOOL)
        V_BOOL(this)=(!lSrc)?VARIANT_FALSE:VARIANT_TRUE;
      else{
        Clear();
        vt=VT_I4;
        lVal=lSrc;
      }
      return *this;
    }

  #if (_WIN32_WINNT >= 0x0501) || defined(_ATL_SUPPORT_VT_I8)
    const XOleVariant& operator=(LONGLONG nSrc){
      if(vt!=VT_I8){
        Clear();
        vt=VT_I8;
      }
      llVal=nSrc;
      return *this;
    }

    const XOleVariant& operator=(ULONGLONG nSrc){
      if(vt!=VT_UI8){
        Clear();
        vt=VT_UI8;
      }
      ullVal=nSrc;
      return *this;
    }
  #endif

    const XOleVariant& operator=(float fltSrc){
      if(vt!=VT_R4){
        Clear();
        vt=VT_R4;
      }
      fltVal=fltSrc;
      return *this;
    }

    const XOleVariant& operator=(double dblSrc){
      if(vt!=VT_R8){
        Clear();
        vt=VT_R8;
      }
      dblVal=dblSrc;
      return *this;
    }

    const XOleVariant& operator=(const DATE &m_dt){
      if(vt!=VT_DATE){
        Clear();
        vt=VT_DATE;
      }
      date=m_dt;
      return *this;
    }

    const XOleVariant& operator=(const CURRENCY &m_cur){
      if(vt!=VT_CY){
        Clear();
        vt=VT_CY;
      }
      cyVal=m_cur;
      return *this;
    }

    operator LPVARIANT(){
      return this;
    }

    operator const VARIANT*()const{
      return this;
    }

    VOID Attach(VARIANT& varSrc){
      Clear();
      memcpy(this,&varSrc,sizeof(varSrc));
      varSrc.vt=VT_EMPTY;
    }

    VARIANT Detach(){
      VARIANT varResult=*this;
      vt=VT_EMPTY;
      return varResult;
    }

    VOID ChangeType(VARTYPE vartype,LPVARIANT pSrc=NULL){
      if(pSrc==NULL)
        pSrc=this;
      if(pSrc!=this||vartype!=vt){
        HRESULT hr=::VariantChangeType(this,pSrc,0,vartype);
        XASSERT(hr==NOERROR)
        XUNUSED(hr)
      }
    }

    VOID Clear(){
      HRESULT hr=::VariantClear(this);
      XASSERT(hr==NOERROR)
      XUNUSED(hr)
    }

    VOID SetString(LPCTSTR lpszSrc,VARTYPE vtSrc);
    BOOL GetByteArray(BYTE* pDest,ULONG *pSize);
    VOID SetByteArray(BYTE* pData,INT_PTR nSize);
    VOID SetLongBinary(const HGLOBAL &hData,ULONG dwDataLength);

    //Safe Array Methods...
    VOID CreateArray(VARTYPE vtSrc,DWORD dwDims,DWORD* rgElements);
    VOID CreateArray(VARTYPE vtSrc,DWORD dwDims,SAFEARRAYBOUND* rgsabound);
    VOID CreateOneDimArray(VARTYPE vtSrc,DWORD dwElements,const void* pvSrcData,LONG nLBound=0);
    VOID CreateOneDimArray(DWORD dwSize);
    DWORD ArrayGetOneDimSize();
    VOID ArrayResizeOneDim(DWORD dwElements);

    DWORD ArrayGetDim(){
      XASSERT(V_ISARRAY(this))
      if(V_ISARRAY(this))
        return ::SafeArrayGetDim(parray);
      return 0;
    }

    DWORD ArrayGetElemSize(){
      XASSERT(V_ISARRAY(this))
      if(V_ISARRAY(this))
        return ::SafeArrayGetElemsize(parray);
      return 0;
    }

    VOID ArrayAccessData(void** ppvData){
      XASSERT(V_ISARRAY(this))
      if(V_ISARRAY(this))
        ::SafeArrayAccessData(parray,ppvData);
    }

    VOID ArrayUnaccessData(){
      XASSERT(V_ISARRAY(this))
      if(V_ISARRAY(this))
        ::SafeArrayUnaccessData(parray);
    }

    VOID ArrayAllocData(){
      XASSERT(V_ISARRAY(this))
      if(V_ISARRAY(this))
        ::SafeArrayAllocData(parray);
    }

    VOID ArrayAllocDescriptor(DWORD dwDims){
      XASSERT(V_ISARRAY(this))
      if(V_ISARRAY(this))
        ::SafeArrayAllocDescriptor(dwDims,&parray);
    }

    VOID ArrayCopy(LPSAFEARRAY* ppsa){
      XASSERT(V_ISARRAY(this))
      if(V_ISARRAY(this))
        ::SafeArrayCopy(parray,ppsa);
    }

    VOID ArrayGetLBound(DWORD dwDim, long* pLbound){
      XASSERT(V_ISARRAY(this))
      if(V_ISARRAY(this))
        ::SafeArrayGetLBound(parray,dwDim,pLbound);
    }

    VOID ArrayGetUBound(DWORD dwDim, long* pUbound){
      XASSERT(V_ISARRAY(this))
      if(V_ISARRAY(this))
        ::SafeArrayGetUBound(parray,dwDim,pUbound);
    }

    VOID ArrayGetElement(long* rgIndices, void* pvData){
      XASSERT(V_ISARRAY(this))
      if(V_ISARRAY(this))
        ::SafeArrayGetElement(parray,rgIndices,pvData);
    }

    VOID ArrayPtrOfIndex(long* rgIndices, void** ppvData){
      XASSERT(V_ISARRAY(this))
      if(V_ISARRAY(this))
        ::SafeArrayPtrOfIndex(parray,rgIndices,ppvData);
    }

    VOID ArrayPutElement(long* rgIndices, void* pvData){
      XASSERT(V_ISARRAY(this))
      if(V_ISARRAY(this))
        ::SafeArrayPutElement(parray,rgIndices,pvData);
    }

    VOID ArrayRedim(SAFEARRAYBOUND* psaboundNew){
      XASSERT(V_ISARRAY(this))
      if(V_ISARRAY(this))
        ::SafeArrayRedim(parray,psaboundNew);
    }

    VOID ArrayLock(){
      XASSERT(V_ISARRAY(this))
      if(V_ISARRAY(this))
        ::SafeArrayLock(parray);
    }

    VOID ArrayUnlock(){
      XASSERT(V_ISARRAY(this))
      if(V_ISARRAY(this))
        ::SafeArrayUnlock(parray);
    }

    VOID ArrayDestroy(){
      XASSERT(V_ISARRAY(this))
      if(V_ISARRAY(this)){
        ::SafeArrayDestroy(parray);
        Detach();
      }
    }

    VOID ArrayDestroyData(){
      XASSERT(V_ISARRAY(this))
      if(V_ISARRAY(this))
        ::SafeArrayDestroyData(parray);
    }

    VOID ArrayDestroyDescriptor(){
      XASSERT(V_ISARRAY(this))
      if(V_ISARRAY(this))
        ::SafeArrayDestroyDescriptor(parray);
    }

  private:

    VOID CopyBinaryData(const PVOID pvSrc,DWORD dwSize);
    static BOOL CompareSafeArrays(SAFEARRAY* parray1, SAFEARRAY* parray2);
};

class XOleDispatchDriver{
  public:
    XOleDispatchDriver(LPDISPATCH lpDispatch=NULL,BOOL bAutoRelease=TRUE):m_lpDispatch(lpDispatch),m_bAutoRelease(bAutoRelease){}
    XOleDispatchDriver(const XOleDispatchDriver& dispatchSrc);
    ~XOleDispatchDriver(){
      Release();
    }

    BOOL IsValid()const{
      return (this==NULL?FALSE:m_lpDispatch!=NULL);
    }

    BOOL Create(REFCLSID clsid,HRESULT *pHr=NULL);
    BOOL Create(LPCTSTR lpszProgID,HRESULT *pHr=NULL);

    LPDISPATCH FromObject(LPUNKNOWN lpIUnknown,BOOL bAutoRelease=TRUE);
    LPDISPATCH Attach(LPDISPATCH lpDispatch,BOOL bAutoRelease=TRUE);
    LPDISPATCH Detach();
    LPDISPATCH Release();

    HRESULT SetProperty(DISPID dwDispID,VARTYPE vtProp,...);
    HRESULT GetProperty(DISPID dwDispID,VARTYPE vtProp,PVOID pvProp)const;
    HRESULT InvokeHelper(DISPID dwDispID,WORD wFlags,VARTYPE vtRet,PVOID pvRet,const WCHAR* pbParamInfo,...)const;

    const XOleDispatchDriver& operator=(const XOleDispatchDriver& dispatchSrc);

    operator LPDISPATCH(){
      return m_lpDispatch;
    }

    LPDISPATCH m_lpDispatch;
    BOOL m_bAutoRelease;
};

template <class T>
class _NoAddRefReleaseXComPtr:public T{
	private:
		STDMETHOD_(ULONG, AddRef)()=0;
		STDMETHOD_(ULONG, Release)()=0;
};

template <class T,const GUID &G>
class XComPtr{
  public:

    XComPtr()throw(){
      p=NULL;
    }

    XComPtr(T* lp)throw(){
      p=lp;
      if(p!=NULL)
        p->AddRef();
    }

    XComPtr(const XComPtr<T,G>& lp)throw(){
      p=lp.p;
      if(p!=NULL)
        p->AddRef();
    }

    XComPtr(XComPtr<T,G>& lp)throw(){
      p=lp.p;
      lp.p=NULL;
    }

    ~XComPtr()throw(){
      if(p)
        Release();
    }

    operator T*()const throw(){
      return p;
    }

    T& operator*()const{
      XASSERT(p!=NULL)
      return *p;
    }

    T** operator&()throw(){
      XASSERT(p==NULL)
      return &p;
    }

    _NoAddRefReleaseXComPtr<T>* operator->()const throw(){
      XASSERT(p!=NULL)
      return (_NoAddRefReleaseXComPtr<T>*)p;
    }

    bool operator!()const throw(){
      return (p==NULL);
    }

    bool operator<(T* pT)const throw(){
      return p<pT;
    }

    bool operator!=(T* pT)const{
      return !operator==(pT);
    }

    bool operator==(T* pT)const throw(){
      return p==pT;
    }

    T* operator=(T* lp)throw(){
      if(*this!=lp)
        return static_cast<T*>(XComPtrAssign((IUnknown**)&p, lp));
      return *this;
    }

    template <typename Q>
    T* operator=(const XComPtr<Q,G>& lp)throw(){
      if(!IsEqualObject(lp))
        return static_cast<T*>(XComQIPtrAssign((IUnknown**)&p, lp, G));
      return *this;
    }

    T* operator=(const XComPtr<T,G>& lp)throw(){
      if(*this!=lp)
    		return static_cast<T*>(XComPtrAssign((IUnknown**)&p, lp));
      return *this;
    }

    T* operator=(XComPtr<T,G>& lp)throw(){
      if (*this!=lp){
        if(p!=NULL)
          Release();
        p=lp.p;
        lp.p=NULL;
      }
      return *this;
    }

    void Release()throw(){
      if(p){
        p->Release();
        p=NULL;
      }
    }

    bool IsEqualObject(IUnknown* pOther)throw(){
      if(p==NULL&&pOther==NULL)
        return true;
      if(p==NULL||pOther==NULL)
        return false;
      XComPtr<IUnknown,IID_IUnknown> punk1;
      XComPtr<IUnknown,IID_IUnknown> punk2;
      p->QueryInterface(IID_IUnknown,(void**)&punk1);
      pOther->QueryInterface(IID_IUnknown,(void**)&punk2);
      return (punk1==punk2);
    }

    void Attach(T* p2)throw(){
      if(p)
        Release();
      p=p2;
    }

    T* Detach()throw(){
      T* pt=p;
      p=NULL;
      return pt;
    }

    BOOL IsValid(){
      return (this==NULL?FALSE:p!=NULL);
    }

    HRESULT CopyTo(T** ppT)throw(){
      XASSERT(ppT!=NULL)
      if(ppT==NULL)
        return E_POINTER;
      *ppT=p;
      if(p)
        p->AddRef();
      return S_OK;
    }

    HRESULT SetSite(IUnknown* punkParent)throw(){
      return XSetChildSite(p,punkParent);
    }

    HRESULT Advise(IUnknown* pUnk,const IID& iid,LPDWORD pdw)throw(){
      return XAdvise(p,pUnk,iid,pdw);
    }

    HRESULT CoCreateInstance(REFCLSID rclsid,LPUNKNOWN pUnkOuter=NULL,DWORD dwClsContext=CLSCTX_ALL)throw(){
      XASSERT(p==NULL)
      return ::CoCreateInstance(rclsid,pUnkOuter,dwClsContext,G,(void**)&p);
    }

    HRESULT CoCreateInstance(LPCOLESTR szProgID,LPUNKNOWN pUnkOuter=NULL,DWORD dwClsContext=CLSCTX_ALL)throw(){
      CLSID clsid;
      HRESULT hr=CLSIDFromProgID(szProgID,&clsid);
      XASSERT(p == NULL)
      if(SUCCEEDED(hr))
        hr=::CoCreateInstance(clsid,pUnkOuter,dwClsContext,G,(void**)&p);
      return hr;
    }

    template <class Q,GUID &GQ> HRESULT QueryInterface(Q** pp)const throw(){
      XASSERT(pp!=NULL)
      return p->QueryInterface(GQ,(void**)pp);
    }

    static IUnknown* XComPtrAssign(IUnknown** pp,IUnknown* lp){
      if(pp==NULL)
        return NULL;
      if(lp!=NULL)
        lp->AddRef();
      if (*pp)
        (*pp)->Release();
      *pp=lp;
      return lp;
    }

    static IUnknown* XComQIPtrAssign(IUnknown** pp,IUnknown* lp,REFIID riid){
      if(pp==NULL)
        return NULL;
      IUnknown* pTemp=*pp;
      *pp=NULL;
      if(lp!=NULL)
        lp->QueryInterface(riid,(void**)pp);
      if(pTemp)
        pTemp->Release();
      return *pp;
    }

     static HRESULT XSetChildSite(IUnknown* punkChild,IUnknown* punkParent){
      if (punkChild == NULL)
        return E_POINTER;
      HRESULT hr;
      IObjectWithSite* pChildSite=NULL;
      hr=punkChild->QueryInterface(IID_IObjectWithSite,(void**)&pChildSite);
      if(SUCCEEDED(hr)&&pChildSite!=NULL){
        hr=pChildSite->SetSite(punkParent);
        pChildSite->Release();
      }
      return hr;
    }

    HRESULT XAdvise(IUnknown* pUnkCP,IUnknown* pUnk,const IID& iid,LPDWORD pdw){
      if(pUnkCP == NULL)
        return E_INVALIDARG;
      XComPtr<IConnectionPointContainer,IID_IConnectionPointContainer> pCPC;
      XComPtr<IConnectionPoint,IID_IConnectionPoint> pCP;
      HRESULT hRes=pUnkCP->QueryInterface(IID_IConnectionPointContainer,(void**)&pCPC);
      if (SUCCEEDED(hRes))
        hRes=pCPC->FindConnectionPoint(iid,&pCP);
      if (SUCCEEDED(hRes))
        hRes=pCP->Advise(pUnk, pdw);
      return hRes;
    }

    HRESULT XUnadvise(IUnknown* pUnkCP,const IID& iid,DWORD dw){
      if(pUnkCP == NULL)
        return E_INVALIDARG;
      XComPtr<IConnectionPointContainer,IID_IConnectionPointContainer> pCPC;
      XComPtr<IConnectionPoint,IID_IConnectionPoint> pCP;
      HRESULT hRes=pUnkCP->QueryInterface(IID_IConnectionPointContainer,(void**)&pCPC);
      if (SUCCEEDED(hRes))
        hRes=pCPC->FindConnectionPoint(iid,&pCP);
      if (SUCCEEDED(hRes))
        hRes=pCP->Unadvise(dw);
      return hRes;
    }

    T* p;
};

template <class T,const IID &I>
class XComQIPtr:public XComPtr<T,I>{
  public:

    XComQIPtr()throw(){}

    XComQIPtr(T* lp)throw():XComPtr<T,I>(lp){}

    XComQIPtr(const XComQIPtr<T,I>& lp)throw():XComPtr<T,I>(lp.p){}

    XComQIPtr(XComQIPtr<T,I>& lp)throw():XComPtr<T,I>(lp.p){}

    XComQIPtr(IUnknown* lp)throw(){
      if(lp!=NULL)
        lp->QueryInterface(I,(void **)&(this->p));
    }

    T* operator=(T* lp)throw(){
      if(*this!=lp)
        return static_cast<T*>(this->XComPtrAssign((IUnknown**)&(this->p),lp));
      return *this;
    }

    T* operator=(const XComQIPtr<T,I>& lp)throw(){
      if(*this!=lp)
        return static_cast<T*>(this->XComPtrAssign((IUnknown**)&(this->p),lp.p));
      return *this;
    }

    T* operator=(IUnknown* lp)throw(){
      if(*this!=lp)
        return static_cast<T*>(this->XComQIPtrAssign((IUnknown**)&(this->p),lp,I));
      return *this;
    }
};

class XEnumUnknown:public XEvent{
  public:
    XEnumUnknown(LPUNKNOWN *pvEnum,UINT nCount,BOOL bNeedFree=FALSE);
    virtual ~XEnumUnknown();
  protected:
    UINT m_nSize;
    UINT m_nCurPos;
    LPUNKNOWN *m_pvEnum;
    XEnumUnknown* m_pClonedFrom;
    BOOL m_bNeedFree;
  public:
    BEGIN_INTERNAL_INTERFACE(XEnumUnknown,Enum,IEnumUnknown)
      STDMETHOD(Next)(ULONG,LPUNKNOWN*,ULONG*);
      STDMETHOD(Skip)(ULONG);
      STDMETHOD(Reset)();
      STDMETHOD(Clone)(IEnumUnknown**);
    END_INTERNAL_INTERFACE(XEnumUnknown,Enum)
  DECLARE_INTERFACE_EVENT_TABLE()
};

#endif //_XTL_H_
