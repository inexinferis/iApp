#ifndef _XIMAGELIST_H_
#define _XIMAGELIST_H_

#include "xapp.h" //for map...

class XImageList:public XObject{
  DECLARE_DYNAMIC(XImageList)
  public:

    XImageList(HIMAGELIST hImageList=NULL):m_hImageList(hImageList),m_bAttached(FALSE){}

    XImageList(INT cx,INT cy,UINT nFlags,INT nInitial,INT nGrow){
      Attach(::ImageList_Create(cx,cy,nFlags,nInitial,nGrow));
    }

    virtual ~XImageList(){
      if(m_hImageList&&m_bAttached)
        Destroy();
    }

    BOOL IsValid()const{
      return (this==NULL?FALSE:m_hImageList!=NULL);
    }

    static XImageList* FromHandlePermanent(HIMAGELIST hImageList){
      XASSERT(hImageList!=NULL)
      XImageList* pIL=NULL;
      if(hImageList)
        pIL=XWinApp::m_XILMap.Find(hImageList);
      return pIL;
    }

    static XImageList* FromHandle(HIMAGELIST hImageList){
      XASSERT(hImageList!=NULL)
      XImageList* pIL=NULL;
      if(hImageList){
        pIL=FromHandlePermanent(hImageList);
        if(!pIL){
          pIL=new XImageList();
          if(pIL)pIL->Attach(hImageList);
        }
      }
      return pIL;
    }

    HIMAGELIST Attach(HIMAGELIST hImageList){
      XASSERT(m_hImageList==NULL)
      if(!hImageList)return NULL;
      m_bAttached=XWinApp::m_XILMap.Add(hImageList,this);
      m_hImageList=hImageList;
      return hImageList;
    }

    HIMAGELIST Detach(){
      HIMAGELIST hImageList=m_hImageList;
      if(hImageList){
        if(m_bAttached){
          XWinApp::m_XILMap.Remove(hImageList);
          m_bAttached=FALSE;
        }
        m_hImageList=NULL;
      }
      return hImageList;
    }

    HIMAGELIST GetSafeHandle()const{
      return (this==NULL)?NULL:m_hImageList;
    }

    HIMAGELIST Create(INT cx,INT cy,UINT nFlags,INT nInitial,INT nGrow=0){
      return Attach(::ImageList_Create(cx,cy,nFlags,nInitial,nGrow));
    }

    HIMAGELIST Create(HINSTANCE hInst,UINT nBitmapID,INT cx,INT nGrow=0,COLORREF crMask=CLR_DEFAULT,UINT uFlags=LR_DEFAULTSIZE){
      XASSERT(HIWORD(nBitmapID)==0)
      return Create(hInst,(LPCTSTR)(DWORD_PTR)nBitmapID,cx,nGrow,crMask,uFlags);
    }

    HIMAGELIST Create(HINSTANCE hInst,LPCTSTR lpszBitmapID,INT cx,INT nGrow=0,COLORREF crMask=CLR_DEFAULT,UINT uFlags=LR_DEFAULTSIZE){
      if(hInst==(HINSTANCE)-1){
        hInst=GetModuleHandle(_T("COMCTL32.DLL"));
        if(!hInst)return NULL;
      }
      return Attach(::ImageList_LoadImage(hInst,lpszBitmapID,cx,nGrow,crMask,IMAGE_BITMAP,uFlags));
    }

    HIMAGELIST Create(HIMAGELIST himagelist1,INT nImage1,HIMAGELIST himagelist2,INT nImage2,INT dx,INT dy){
      return Attach(::ImageList_Merge(himagelist1,nImage1,himagelist2,nImage2,dx,dy));
    }

    HIMAGELIST Create(HIMAGELIST hImageList){
      XASSERT(hImageList!=NULL)
      return Attach(::ImageList_Duplicate(hImageList));
    }

    INT GetImageCount()const{
      XASSERT(m_hImageList!=NULL)
      return ::ImageList_GetImageCount(m_hImageList);
    }

    BOOL IsEmpty()const{
      XASSERT(m_hImageList!=NULL)
      return (::ImageList_GetImageCount(m_hImageList)==0);
    }

    BOOL SetImageCount(UINT uNewCount){
      XASSERT(m_hImageList!=NULL)
      return ::ImageList_SetImageCount(m_hImageList, uNewCount);
    }

    BOOL Copy(INT iDst,INT iSrc,UINT uFlags=ILCF_MOVE){
      XASSERT(m_hImageList!=NULL)
      return ::ImageList_Copy(m_hImageList,iDst,m_hImageList,iSrc,uFlags);
    }

    BOOL Copy(INT iDst,HIMAGELIST hSrc,INT iSrc,UINT uFlags=ILCF_MOVE){
      XASSERT(m_hImageList!=NULL)
      XASSERT(hSrc!=NULL)
      return ::ImageList_Copy(m_hImageList,iDst,hSrc,iSrc,uFlags);
    }

    INT Add(HBITMAP hbmImage,HBITMAP hbmMask=NULL){
      XASSERT(m_hImageList!=NULL)
      return ::ImageList_Add(m_hImageList,hbmImage,hbmMask);
    }

    INT Add(HBITMAP hbmImage,COLORREF crMask){
      XASSERT(m_hImageList!=NULL)
      return ::ImageList_AddMasked(m_hImageList,hbmImage,crMask);
    }

    INT Add(HICON hIcon){
      XASSERT(m_hImageList!=NULL)
      return ::ImageList_ReplaceIcon(m_hImageList,-1,hIcon);
    }

    BOOL Remove(INT nImage=-1){
      XASSERT(m_hImageList!=NULL)
      return ::ImageList_Remove(m_hImageList,nImage);
    }

    BOOL Replace(INT nImage,HBITMAP hbmImage,HBITMAP hbmMask){
      XASSERT(m_hImageList!=NULL)
      return ::ImageList_Replace(m_hImageList,nImage,hbmImage,hbmMask);
    }

    INT Replace(INT nImage,HICON hIcon){
      XASSERT(m_hImageList!=NULL)
      return ::ImageList_ReplaceIcon(m_hImageList,nImage,hIcon);
    }

    HICON ExtractIcon(INT nImage){
      XASSERT(m_hImageList!=NULL)
      return ::ImageList_ExtractIcon(NULL,m_hImageList,nImage);
    }

    HBITMAP ExtractBitmap(HDC hdc,INT nImage){
      XASSERT(m_hImageList!=NULL&&hdc!=NULL)
      HBITMAP hBmp=NULL;IMAGEINFO ii;
      if(::ImageList_GetImageInfo(m_hImageList,nImage,&ii)){
        INT iWidth=ii.rcImage.right-ii.rcImage.left;
        INT iHeight=ii.rcImage.bottom-ii.rcImage.top;
        HDC dcMem=::CreateCompatibleDC(hdc);
        hBmp=CreateCompatibleBitmap(hdc,iWidth,iHeight);
        HBITMAP hBmpOld=(HBITMAP)::SelectObject(dcMem,hBmp);
        if(!::ImageList_Draw(m_hImageList,nImage,dcMem,0,0,ILD_NORMAL)){
          ::DeleteObject(hBmp);
          hBmp=NULL;
        }
        ::SelectObject(dcMem,hBmpOld);
        ::DeleteDC(dcMem);
      }
      return hBmp;
    }

    BOOL Draw(HDC hDC,INT nImage,POINT pt,UINT nStyle=ILD_NORMAL){
      XASSERT(m_hImageList!=NULL&&hDC!=NULL)
      return ::ImageList_Draw(m_hImageList,nImage,hDC,pt.x,pt.y,nStyle);
    }

    BOOL DrawEx(HDC hDC,INT nImage,POINT pt,SIZE sz,COLORREF clrBk=CLR_DEFAULT,COLORREF clrFg=CLR_DEFAULT,UINT nStyle=ILD_NORMAL){
      XASSERT(m_hImageList!=NULL&&hDC!=NULL)
      return ::ImageList_DrawEx(m_hImageList,nImage,hDC,pt.x,pt.y,sz.cx,sz.cy,clrBk,clrFg,nStyle);
    }

    COLORREF SetBkColor(COLORREF cr){
      XASSERT(m_hImageList!=NULL)
      return ::ImageList_SetBkColor(m_hImageList,cr);
    }

    COLORREF GetBkColor()const{
      XASSERT(m_hImageList!=NULL)
      return ::ImageList_GetBkColor(m_hImageList);
    }

    BOOL SetOverlayImage(INT nImage,INT nOverlay){
      XASSERT(m_hImageList!=NULL)
      return ::ImageList_SetOverlayImage(m_hImageList,nImage,nOverlay);
    }

    BOOL GetImageInfo(INT nImage,IMAGEINFO* pImageInfo)const{
      XASSERT(m_hImageList!=NULL)
      return ::ImageList_GetImageInfo(m_hImageList,nImage,pImageInfo);
    }

    HBITMAP GetImage(INT index){
      XASSERT(m_hImageList!=NULL)
      IMAGEINFO pImageInfo;
      if(::ImageList_GetImageInfo(m_hImageList,index,&pImageInfo))
        return pImageInfo.hbmImage;
      return NULL;
    }

    BOOL GetImageRect(INT index,LPRECT rc){
      XASSERT(m_hImageList!=NULL)
      IMAGEINFO pImageInfo;
      if(::ImageList_GetImageInfo(m_hImageList,index,&pImageInfo)){
        CopyRect(rc,&pImageInfo.rcImage);
        return TRUE;
      }
      return FALSE;
    }

    BOOL GetImageSize(INT index,LPSIZE sz){
      XASSERT(m_hImageList!=NULL)
      IMAGEINFO pImageInfo;
      if(::ImageList_GetImageInfo(m_hImageList,index,&pImageInfo)){
        sz->cx=pImageInfo.rcImage.right-pImageInfo.rcImage.left;
        sz->cy=pImageInfo.rcImage.bottom-pImageInfo.rcImage.top;
        return TRUE;
      }
      return FALSE;
    }

    HICON GetIcon(INT i,UINT flags=ILD_NORMAL){
      XASSERT(m_hImageList!=NULL)
      return ::ImageList_GetIcon(m_hImageList,i,flags);
    }

    BOOL GetIconSize(INT *cx,INT *cy){
      XASSERT(m_hImageList!=NULL)
      return ::ImageList_GetIconSize(m_hImageList,cx,cy);
    }

    BOOL SetIconSize(INT cx,INT cy){
      XASSERT(m_hImageList!=NULL)
      return ::ImageList_SetIconSize(m_hImageList,cx,cy);
    }

    BOOL BeginDrag(INT nImage,POINT pt){
      XASSERT(m_hImageList!=NULL)
      return ::ImageList_BeginDrag(m_hImageList,nImage,pt.x, pt.y);
    }

    static VOID EndDrag(){
      ::ImageList_EndDrag();
    }

    static BOOL DragMove(POINT pt){
      return ::ImageList_DragMove(pt.x,pt.y);
    }

    BOOL SetDragCursorImage(INT nDrag,POINT pt){
      XASSERT(m_hImageList!=NULL)
      return ::ImageList_SetDragCursorImage(m_hImageList,nDrag,pt.x,pt.y);
    }

    static BOOL DragShowNolock(BOOL bShow){
      return ::ImageList_DragShowNolock(bShow);
    }

    static HIMAGELIST GetDragImage(LPPOINT lpPoint,LPPOINT lpPointHotSpot){
      return ::ImageList_GetDragImage(lpPoint,lpPointHotSpot);
    }

    static BOOL DragEnter(HWND hWndLock,POINT pt){
      return ::ImageList_DragEnter(hWndLock,pt.x,pt.y);
    }

    static BOOL DragLeave(HWND hWndLock){
      return ::ImageList_DragLeave(hWndLock);
    }

    BOOL DrawIndirect(IMAGELISTDRAWPARAMS* pimldp){
      XASSERT(m_hImageList!=NULL)
      pimldp->himl=m_hImageList;
      return ::ImageList_DrawIndirect(pimldp);
    }

    BOOL DrawIndirect(HDC hDC,INT nImage,POINT pt,SIZE sz,POINT ptOrigin,UINT fStyle=ILD_NORMAL,
      DWORD dwRop=SRCCOPY,COLORREF rgbBack=CLR_DEFAULT,COLORREF rgbFore=CLR_DEFAULT,
      DWORD fState=ILS_NORMAL,DWORD Frame=ILS_NORMAL,COLORREF crEffect=CLR_DEFAULT){
      XASSERT(hDC!=NULL)
      IMAGELISTDRAWPARAMS drawing;
      drawing.i=nImage;
      drawing.hdcDst=hDC;
      drawing.x=pt.x;
      drawing.y=pt.y;
      drawing.cx=sz.cx;
      drawing.cy=sz.cy;
      drawing.xBitmap=ptOrigin.x;
      drawing.yBitmap=ptOrigin.y;
      drawing.rgbBk=rgbBack;
      drawing.rgbFg=rgbFore;
      drawing.fStyle=fStyle;
      drawing.dwRop=dwRop;
      #if (_WIN32_WINNT >= 0x0501)
      drawing.fState=fState;
      drawing.Frame=Frame;
      drawing.crEffect=crEffect;
      #else
      XUNUSED_ALWAYS(fState)
      XUNUSED_ALWAYS(Frame)
      XUNUSED_ALWAYS(crEffect)
      #endif
      return DrawIndirect(&drawing);
    }

    HIMAGELIST Read(LPSTREAM pstm){
      XASSERT(m_hImageList==NULL)
      XASSERT(pstm!=NULL)
      return Attach(::ImageList_Read(pstm));
    }

    BOOL Write(LPSTREAM pstm){
      XASSERT(m_hImageList!=NULL)
      XASSERT(pstm!=NULL)
      return ::ImageList_Write(m_hImageList,pstm);
    }

    BOOL Destroy(){
      if(m_hImageList)
        return ::ImageList_Destroy(Detach());
      return FALSE;
    }

    XImageList &operator=(const HIMAGELIST hImageList){
      XASSERT(m_hImageList==NULL)
      m_hImageList=hImageList;
      return *this;
    }

    operator HIMAGELIST()const{
      return m_hImageList;
    }

  private:
    HIMAGELIST m_hImageList;
    BOOL m_bAttached;
};

#endif //_XIMAGELIST_H_
