#ifndef _XUTILS_H_
#define _XUTILS_H_

#include "xobject.h"

/**************************************************************/
// Util Functions...
/**************************************************************/

INT GetStringLength(HINSTANCE hInstance,UINT uID);
UINT LoadStringExW(HINSTANCE hInstance,UINT uId,LPWSTR buffer,UINT buflen,WORD wLanguage=0);
UINT LoadStringExA(HINSTANCE hInstance,UINT uId,LPSTR buffer,UINT buflen,WORD wLanguage=0);
LPTSTR LoadStringBuffered(HINSTANCE hInstance,LPCTSTR lpszName);
LPTSTR LoadStringStatic(HINSTANCE hInstance,LPCTSTR lpszName);
LPTSTR FormatMsgEx(HINSTANCE hInstance,LPCTSTR nRsrcID,...);
LPTSTR FormatMsg(LPCTSTR lpszFormat,...);
LPSTR GUIDToString(const GUID* guid,LPSTR str,size_t sz);
VOID DumpGUID(const GUID* guid,LPVOID pvObj);

#ifdef UNICODE
  #define LoadStringEx LoadStringExW
#else
  #define LoadStringEx LoadStringExA
#endif

/**************************************************************/
// Strings types definition...
/**************************************************************/

#include <string>

typedef std::basic_string<char> XStringA;
typedef std::basic_string<wchar_t> XStringW;
typedef std::basic_string<TCHAR> XString;

/**************************************************************/
// XMsg REFERENCE...
/**************************************************************/

class XMsg{
  public:
    XMsg(){}

    XMsg(MSG &Msg){
      CopyMemory(&m_Msg,&Msg,sizeof(m_Msg));
    }

    virtual ~XMsg(){}

    BOOL TranslateMDISysAccel(HWND hWndClient){
      return ::TranslateMDISysAccel(hWndClient,&m_Msg);
    }

    BOOL TranslateMessage(){
      return ::TranslateMessage(&m_Msg);
    }

    INT TranslateAccelerator(HWND hWnd,HACCEL hAccTable){
      return ::TranslateAccelerator(hWnd,hAccTable,&m_Msg);
    }

    BOOL PeekMessage(HWND hWnd=NULL,UINT wMsgMin=0,UINT wMsgMax=0,UINT wRemMsg=PM_NOREMOVE){
      return ::PeekMessage(&m_Msg,hWnd,wMsgMin,wMsgMax,wRemMsg);
    }

    BOOL GetMessage(HWND hWnd=NULL,UINT wMsgFilterMin=0,UINT wMsgFilterMax=0){
      return ::GetMessage(&m_Msg,hWnd,wMsgFilterMin,wMsgFilterMax);
    }

    LONG DispatchMessage(){
      return ::DispatchMessage(&m_Msg);
    }

    BOOL CallMsgFilter(INT nCode){
      return ::CallMsgFilter(&m_Msg,nCode);
    }

    XMsg &operator=(MSG &Msg){
      CopyMemory(&m_Msg,&Msg,sizeof(m_Msg));
      return *this;
    }

    operator MSG(){
      return m_Msg;
    }

    operator const MSG*()const{
      return (MSG*)this;
    }

  //private:
    MSG m_Msg;
};

typedef XMsg XMSG,*PXMSG;

/*****************************************************************/
// XWndClass
/*****************************************************************/

class XWndClass:public XObject{
  public:
    XWndClass(ATOM atom=0):m_atom(atom){}
    XWndClass(HINSTANCE hinst,LPCTSTR lpszClassName){
      WNDCLASSEX wincl;
      m_atom=::GetClassInfoEx(hinst,lpszClassName,&wincl);
    }

    virtual ~XWndClass(){}

    ATOM GetAtom() const {
      return ( this != NULL ? m_atom : 0);
    }

    BOOL IsValid(){
      return (m_atom!=0);
    }

    ATOM Attach(ATOM atom){
      XASSERT(m_atom==0)
      m_atom=atom;
      return atom;
    }

    ATOM Detach(){
      XASSERT(m_atom!=0)
      ATOM atom=m_atom;
      m_atom=0;
      return atom;
    }

    ATOM RegisterClass(LPCTSTR lpszClassName,UINT style,HICON hIcon,HICON hIconSm,HCURSOR hCursor,HBRUSH hbrBackground,
      LPCTSTR lpszMenuName=NULL,int cbClsExtra=0,int cbWndExtra=0,WNDPROC lpfnWndProc=NULL,HINSTANCE hInstance=NULL);

    ATOM RegisterClass(WNDCLASS *lpWndClass){
      XASSERT(m_atom==0)
      return Attach(::RegisterClass(lpWndClass));
    }

    ATOM RegisterClassEx(WNDCLASSEX *lpwcx){
      XASSERT(m_atom==0)
      return Attach(::RegisterClassEx(lpwcx));
    }

    BOOL UnregisterClass(HINSTANCE hInstance){
      XASSERT(m_atom!=0)
      return ::UnregisterClass(MAKEINTATOM(m_atom),hInstance);
    }

    BOOL GetClassInfoEx(HINSTANCE hinst,LPWNDCLASSEX lpwcx){
      XASSERT(m_atom!=0)
      return ::GetClassInfoEx(hinst,MAKEINTATOM(m_atom),lpwcx);
    }

    BOOL GetClassInfo(HINSTANCE hinst,LPWNDCLASS lpWndClass){
      XASSERT(m_atom!=0)
      return ::GetClassInfo(hinst,MAKEINTATOM(m_atom),lpWndClass);
    }

    operator ATOM(){
      return m_atom;
    }

    XWndClass &operator=(ATOM atom){
      XASSERT(m_atom==0)
      m_atom=atom;
      return *this;
    }

  private:
    ATOM m_atom;
};

typedef XWndClass XWNDCLASS,*PXWNDCLASS;

class CA2W{
  public:
    CA2W(LPCSTR psz,UINT ncp=CP_ACP,BOOL autofree=TRUE):m_pwsz(NULL),m_length(0),m_autofree(autofree){
      Convert(psz,ncp);
    }
    virtual ~CA2W(){
      if(m_pwsz&&m_autofree)
        free(m_pwsz);
    }
    operator LPWSTR()const{
      return m_pwsz;
    }
    UINT Length()const{
      return m_length;
    }
    VOID Free(){
      if(m_pwsz){
        free(m_pwsz);
        m_pwsz=NULL;
      }
    }
  private:
    VOID Convert(LPCSTR psz,UINT ncp){
      if(psz==NULL)
        return;
      if((m_length=::MultiByteToWideChar(ncp,0,psz,-1,NULL,0))){
        m_pwsz=(LPWSTR)malloc(m_length);
        if(!::MultiByteToWideChar(ncp,0,psz,-1,m_pwsz,m_length))
          m_length=(UINT)-1;
      }
    }
  public:
    LPWSTR m_pwsz;UINT m_length;BOOL m_autofree;
  private:
    CA2W(const CA2W&);
    CA2W& operator=(const CA2W&);
};

class CA2WBSTR{
  public:
    CA2WBSTR(LPCSTR psz,UINT ncp=CP_ACP,BOOL autofree=TRUE):m_pbsz(NULL),m_length(0),m_autofree(autofree){
      Convert(psz,ncp);
    }
    virtual ~CA2WBSTR(){
      if(m_pbsz&&m_autofree)
        ::SysFreeString(m_pbsz);
    }
    operator BSTR()const{
      return m_pbsz;
    }
    UINT Length()const{
      return m_length;
    }
    VOID Free(){
      if(m_pbsz){
        ::SysFreeString(m_pbsz);
        m_pbsz=NULL;
      }
    }
  private:
    VOID Convert(LPCSTR psz,UINT ncp){
      if(psz==NULL)
        return;
      if((m_length=::MultiByteToWideChar(ncp,0,psz,-1,NULL,0))){
        m_pbsz=::SysAllocStringLen(NULL,m_length);
        if(!::MultiByteToWideChar(ncp,0,psz,-1,m_pbsz,m_length))
          m_length=(UINT)-1;
      }
    }
  public:
    BSTR m_pbsz;UINT m_length;BOOL m_autofree;
  private:
    CA2WBSTR(const CA2WBSTR&);
    CA2WBSTR& operator=(const CA2WBSTR&);
};

class CW2A{
  public:
    CW2A(LPCWSTR pwsz,UINT ncp=CP_ACP,BOOL autofree=TRUE):m_psz(NULL),m_length(0),m_autofree(autofree){
      Convert(pwsz,ncp);
    }
    virtual ~CW2A(){
      if(m_psz&&m_autofree)
        free(m_psz);
    }
    operator LPSTR()const{
      return m_psz;
    }
    UINT Length()const{
      return m_length;
    }
    VOID Free(){
      if(m_psz){
        free(m_psz);
        m_psz=NULL;
      }
    }
  private:
    void Convert(LPCWSTR pwsz,UINT ncp){
      if(pwsz==NULL)
        return;
      if((m_length=::WideCharToMultiByte(ncp,0,pwsz,-1,NULL,0,NULL,NULL))){
        m_psz=(LPSTR)malloc(m_length*sizeof(WCHAR));
        if(!::WideCharToMultiByte(ncp,0,pwsz,-1,m_psz,m_length,NULL,NULL))
          m_length=(UINT)-1;
      }
    }

  public:
    LPSTR m_psz;UINT m_length;BOOL m_autofree;
  private:
    CW2A(const CW2A&);
    CW2A& operator=(const CW2A&);
};

class CWBSTR2A{
  public:
    CWBSTR2A(BSTR pwsz,UINT ncp=CP_ACP,BOOL autofree=TRUE):m_psz(NULL),m_length(0),m_autofree(autofree){
      Convert(pwsz,ncp);
    }
    virtual ~CWBSTR2A(){
      if(m_psz&&m_autofree)
        free(m_psz);
    }
    operator LPSTR()const{
      return m_psz;
    }
    UINT Length()const{
      return m_length;
    }
    VOID Free(){
      if(m_psz){
        free(m_psz);
        m_psz=NULL;
      }
    }
  private:
    void Convert(BSTR pwsz,UINT ncp){
      if(pwsz==NULL)
        return;
      if((m_length=::WideCharToMultiByte(ncp,0,pwsz,-1,NULL,0,NULL,NULL))){
        m_psz=(LPSTR)malloc(m_length*sizeof(WCHAR));
        if(!::WideCharToMultiByte(ncp,0,pwsz,-1,m_psz,m_length,NULL,NULL))
          m_length=(UINT)-1;
      }
    }

  public:
    LPSTR m_psz;UINT m_length;BOOL m_autofree;
  private:
    CWBSTR2A(const CWBSTR2A&);
    CWBSTR2A& operator=(const CWBSTR2A&);
};

class CWBSTR2ABSTR{
  public:
    CWBSTR2ABSTR(BSTR pwsz,UINT ncp=CP_ACP,BOOL autofree=TRUE):m_psz(NULL),m_length(0),m_autofree(autofree){
      Convert(pwsz,ncp);
    }
    virtual ~CWBSTR2ABSTR(){
      if(m_psz&&m_autofree)
        SysStringLen(m_psz);
    }
    operator BSTR()const{
      return m_psz;
    }
    UINT Length()const{
      return m_length;
    }
    VOID Free(){
      if(m_psz){
        SysStringLen(m_psz);
        m_psz=NULL;
      }
    }
  private:
    void Convert(BSTR pwsz,UINT ncp){
      if(pwsz==NULL)
        return;
      if((m_length=::WideCharToMultiByte(ncp,0,pwsz,SysStringLen(pwsz),NULL,0,NULL,NULL))){
        m_psz=SysAllocStringByteLen(NULL,m_length);
        if(!::WideCharToMultiByte(ncp,0,pwsz,-1,(LPSTR)m_psz,m_length,NULL,NULL))
          m_length=(UINT)-1;
      }
    }

  public:
    BSTR m_psz;UINT m_length;BOOL m_autofree;
  private:
    CWBSTR2ABSTR(const CWBSTR2ABSTR&);
    CWBSTR2ABSTR& operator=(const CWBSTR2ABSTR&);
};

#ifdef _UNICODE
  #define TSTR2WSTR(s) s
#else
  #define TSTR2WSTR(s) CA2W(s)
#endif

#ifndef _UNICODE
  #define TSTR2ASTR(s) s
#else
  #define TSTR2ASTR(s) CW2A(s)
#endif

#ifdef _UNICODE
  #define CBSTR2TSTR(s) s
#else
  #define CBSTR2TSTR(s) CW2A(s)
#endif

class XRSRC{
  public:
    XRSRC(HRSRC hRsrc=NULL):m_hRsrc(hRsrc),m_hGlobal(NULL),m_hModule(NULL){}
    virtual ~XRSRC(){
      if(m_hGlobal)
        Free();
    }

    BOOL IsValid(){
      return (m_hRsrc!=NULL);
    }

    static XRSRC* FromHandle(HRSRC hRsrc){
      return new XRSRC(hRsrc);
    }

    HRSRC Attach(HRSRC hRsrc){
      XASSERT(m_hRsrc==NULL)
      m_hRsrc=hRsrc;
      return hRsrc;
    }

    HRSRC Detach(){
      XASSERT(m_hRsrc!=NULL)
      HRSRC hRsrc=m_hRsrc;
      m_hRsrc=NULL;
      return hRsrc;
    }

    HRSRC GetSafeHandle() const {
      return (this == NULL ? NULL : m_hRsrc);
    }

    HRSRC Find(HMODULE hModule,LPCTSTR lpName,LPCTSTR lpType){
      m_hModule=hModule;
      return Attach(::FindResource(hModule,lpName,lpType));
    }

    HRSRC FindEx(HMODULE hModule,LPCTSTR lpType,LPCTSTR lpName,WORD wLanguage){
      m_hModule=hModule;
      return Attach(::FindResourceEx(hModule,lpType,lpName,wLanguage));
    }

    BOOL Free(){
      if(m_hGlobal&&::FreeResource(m_hGlobal)){
        m_hGlobal=NULL;
        return TRUE;
      }
      return FALSE;
    }

    LPVOID Lock(){
      if(m_hGlobal)
        return ::LockResource(m_hGlobal);
      return NULL;
    }

    HGLOBAL Load(){
      m_hGlobal=::LoadResource(m_hModule,m_hRsrc);
      return m_hGlobal;
    }

    DWORD Size(){
      return ::SizeofResource(m_hModule,m_hRsrc);
    }

  public:
    HRSRC m_hRsrc;
    HGLOBAL m_hGlobal;
  private:
    HMODULE m_hModule;
};

class XCriticalSection:public XObject{
  public:
    XCriticalSection(){::InitializeCriticalSection(&m_cs);}
    XCriticalSection(DWORD dwSpinCount){::InitializeCriticalSectionAndSpinCount(&m_cs,dwSpinCount);}

    virtual ~XCriticalSection(){::DeleteCriticalSection(&m_cs);}

    void Enter(){::EnterCriticalSection(&m_cs);}
    void Leave(){::LeaveCriticalSection(&m_cs);}

    ULONG SetSpinCount(ULONG nSpinCount){return ::SetCriticalSectionSpinCount(&m_cs,nSpinCount);}
    BOOL TryEnter(){return ::TryEnterCriticalSection(&m_cs);}

    operator LPCRITICAL_SECTION(){return &m_cs;}

  private:
    XCriticalSection(const XCriticalSection&);
    XCriticalSection& operator=(const XCriticalSection&);

    CRITICAL_SECTION m_cs;
};

template <class T,class O>
class XObjectMap:public XObject{
  public:
    XObjectMap(){}
    virtual ~XObjectMap(){}

    BOOL Add(T hData, O pObj){
      BOOL bRet=FALSE;
      m_sect.Enter();
      //map only if not mapped...
      if(m_Map.find(hData)==m_Map.end()){
        m_Map[hData]=pObj;
        bRet=TRUE;
      }
      m_sect.Leave();
      return bRet;
    }

    BOOL Remove(T hData){
      BOOL bRet=FALSE;
      m_sect.Enter();
      //unmap only if is mapped...
      if(m_Map.find(hData)!=m_Map.end()){
        m_Map.erase(hData);
        bRet=TRUE;
      }
      m_sect.Leave();
      return bRet;
    }

    O Find(T hData){
      O pObj=(O)NULL;
      m_sect.Enter();
      if(m_Map.find(hData)!=m_Map.end())
        pObj=m_Map[hData];
      m_sect.Leave();
      return pObj;
    }

    O operator[](T hData){
      O pObj=(O)NULL;
      m_sect.Enter();
      if(m_Map.find(hData)!=m_Map.end())
        pObj=m_Map[hData];
      m_sect.Leave();
      return pObj;
    }

    INT_PTR Size()const{
      return m_Map.size();
    }

    BOOL IsEmpty()const{
      return m_Map.empty();
    }

    VOID RemoveAll(){
      m_Map.clear();
    }

    // for Iteration
    typename std::map<T,O>::iterator Begin(){
      return m_Map.begin();
    }

    typename std::map<T,O>::iterator End(){
      return m_Map.end();
    }

    typename std::map<T,O>::const_iterator Begin()const{
      return m_Map.begin();
    }

    typename std::map<T,O>::const_iterator End()const{
      return m_Map.end();
    }

	private:
    XCriticalSection m_sect;
    std::map<T,O> m_Map;
};

template <class T>
class XSharedMem{
  public:

    XSharedMem(UINT uSize=0):m_uIndex(0),m_hFile(NULL),m_hView(NULL){
      m_uSize=uSize;
      if(!m_uSize)
        m_uSize=1;
    }

    XSharedMem(LPCTSTR SMName,UINT uSize=0):m_uIndex(0),m_hFile(NULL),m_hView(NULL){
      m_uSize=uSize;
      if(!m_uSize)
        m_uSize=1;
      Open(SMName);
    }

    ~XSharedMem(){
      Destroy();
    }

    BOOL Create(LPCTSTR SMName){
      m_hFile=CreateFileMapping(INVALID_HANDLE_VALUE,NULL,PAGE_READWRITE,0,sizeof(T)*m_uSize*3,SMName);
      if(!m_hFile)return FALSE;
      m_hView=MapViewOfFile(m_hFile,FILE_MAP_ALL_ACCESS,0,0,0);
      if(!m_hView){
        CloseHandle(m_hFile);
        return FALSE;
      }
      return TRUE;
    }

    BOOL Open(LPCSTR SMName){
      XASSERT(SMName!=NULL)
      if(SMName){
        m_hFile=OpenFileMapping(FILE_MAP_ALL_ACCESS,TRUE,SMName);
        if(!m_hFile)return FALSE;
        m_hView=MapViewOfFile(m_hFile,FILE_MAP_ALL_ACCESS,0,0,0);
        if(!m_hView){
          CloseHandle(m_hFile);
          return FALSE;
        }
        return TRUE;
      }
      return FALSE;
    }

    UINT Size(){
      return m_uSize;
    }

    BOOL Destroy(){
      if(m_hView&&m_hFile){
        if(UnmapViewOfFile(m_hView))
        return CloseHandle(m_hFile);
      }
      return FALSE;
    }

    BOOL operator==(const T t){
      XASSERT(m_hView!=NULL)
      if(!this||!m_hView)return FALSE;
      BOOL bRet=(((T*)m_hView)[m_uIndex]==t);
      if(m_uIndex)m_uIndex=0;
      return bRet;
    }

    BOOL operator!=(const T t){
      XASSERT(m_hView!=NULL)
      if(!this||!m_hView)return TRUE;
      return !(*this==t);
    }

    XSharedMem &operator[](UINT i){
      XASSERT(m_hView!=NULL)
  		if(this&&m_hView&&i<m_uSize)
        m_uIndex=i;
      return *this;
  	}

    T *operator->(){
      XASSERT(m_hView!=NULL)
      T *pRet=NULL;
      if(this&&m_hView){
        pRet=&((T*)m_hView)[m_uIndex];
        if(m_uIndex)m_uIndex=0;
      }
      return pRet;
    }

  	XSharedMem &operator=(const T t){
      XASSERT(m_hView!=NULL)
      if(this&&m_hView){
        ((T*)m_hView)[m_uIndex]=t;
        if(m_uIndex)m_uIndex=0;
      }
      return *this;
    }

  	operator T(){
  	  XASSERT(m_hView!=NULL)
  	  T ret=0;
  	  if(this&&m_hView){
        ret=((T*)m_hView)[m_uIndex];
        if(m_uIndex)m_uIndex=0;
      }
      return ret;
    }

  private:
    UINT m_uIndex,m_uSize;
    HANDLE m_hFile;
    LPVOID m_hView;
};

#endif /* _XUTILS_H_ */
