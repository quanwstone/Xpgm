

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.00.0603 */
/* at Fri Oct 27 09:43:59 2017
 */
/* Compiler settings for xTransCodeATL.idl:
    Oicf, W1, Zp8, env=Win64 (32b run), target_arch=AMD64 8.00.0603 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __xTransCodeATL_i_h__
#define __xTransCodeATL_i_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __INxTransCode_FWD_DEFINED__
#define __INxTransCode_FWD_DEFINED__
typedef interface INxTransCode INxTransCode;

#endif 	/* __INxTransCode_FWD_DEFINED__ */


#ifndef ___INxTransCodeEvents_FWD_DEFINED__
#define ___INxTransCodeEvents_FWD_DEFINED__
typedef interface _INxTransCodeEvents _INxTransCodeEvents;

#endif 	/* ___INxTransCodeEvents_FWD_DEFINED__ */


#ifndef __NxTransCode_FWD_DEFINED__
#define __NxTransCode_FWD_DEFINED__

#ifdef __cplusplus
typedef class NxTransCode NxTransCode;
#else
typedef struct NxTransCode NxTransCode;
#endif /* __cplusplus */

#endif 	/* __NxTransCode_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


/* interface __MIDL_itf_xTransCodeATL_0000_0000 */
/* [local] */ 

typedef /* [public][public] */ 
enum __MIDL___MIDL_itf_xTransCodeATL_0000_0000_0001
    {
        keNsWrapperFileTypeIDL_Invalid	= -1,
        keNsWrapperFileTypeIDL_MXF_DNxHR	= ( keNsWrapperFileTypeIDL_Invalid + 1 ) ,
        keNsWrapperFileTypeIDL_MXF_XAVC_MC	= ( keNsWrapperFileTypeIDL_MXF_DNxHR + 1 ) ,
        keNsWrapperFileTypeIDL_MOV	= ( keNsWrapperFileTypeIDL_MXF_XAVC_MC + 1 ) ,
        keNsWrapperFileTypeILD_Last	= ( keNsWrapperFileTypeIDL_MOV + 1 ) 
    } 	ENsWrapperFileTypeIDL;



extern RPC_IF_HANDLE __MIDL_itf_xTransCodeATL_0000_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_xTransCodeATL_0000_0000_v0_0_s_ifspec;

#ifndef __INxTransCode_INTERFACE_DEFINED__
#define __INxTransCode_INTERFACE_DEFINED__

/* interface INxTransCode */
/* [unique][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_INxTransCode;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("221C6680-050B-4B2A-A3DF-145FE1F9BD4E")
    INxTransCode : public IDispatch
    {
    public:
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE TransCode_Begin( 
            /* [in] */ BSTR in_bstrFile) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE TransElement_Begin( void) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE TransElement_AddVideoFile( 
            /* [in] */ BSTR in_bstrFile,
            /* [in] */ unsigned long in_ulIn,
            /* [in] */ unsigned long in_ulOut,
            /* [in] */ BSTR in_bstrCam) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE TransElement_AddAudioFile( 
            /* [in] */ BSTR in_bstrFile,
            /* [in] */ unsigned long in_ulIn,
            /* [in] */ unsigned long in_ulOut) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE TransElement_End( 
            /* [in] */ BOOL bUseFFmpeg) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE TransCode_End( void) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE TransCode_SetWrapperFileType( 
            /* [in] */ ENsWrapperFileTypeIDL in_eWrapperFileTypeIDL) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE TransElement_GetCurrentPositionInFrames( 
            /* [out] */ ULONGLONG *out_iCurrentFramePosition) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE TransElement_SetFramesReaderStatus( 
            /* [in] */ BOOL in_bStatus) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE TransElement_SetMixSchedule( 
            /* [in] */ double in_ulMixStart,
            /* [in] */ double in_ulMixEnd) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct INxTransCodeVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            INxTransCode * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            INxTransCode * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            INxTransCode * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            INxTransCode * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            INxTransCode * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            INxTransCode * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            INxTransCode * This,
            /* [annotation][in] */ 
            _In_  DISPID dispIdMember,
            /* [annotation][in] */ 
            _In_  REFIID riid,
            /* [annotation][in] */ 
            _In_  LCID lcid,
            /* [annotation][in] */ 
            _In_  WORD wFlags,
            /* [annotation][out][in] */ 
            _In_  DISPPARAMS *pDispParams,
            /* [annotation][out] */ 
            _Out_opt_  VARIANT *pVarResult,
            /* [annotation][out] */ 
            _Out_opt_  EXCEPINFO *pExcepInfo,
            /* [annotation][out] */ 
            _Out_opt_  UINT *puArgErr);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *TransCode_Begin )( 
            INxTransCode * This,
            /* [in] */ BSTR in_bstrFile);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *TransElement_Begin )( 
            INxTransCode * This);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *TransElement_AddVideoFile )( 
            INxTransCode * This,
            /* [in] */ BSTR in_bstrFile,
            /* [in] */ unsigned long in_ulIn,
            /* [in] */ unsigned long in_ulOut,
            /* [in] */ BSTR in_bstrCam);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *TransElement_AddAudioFile )( 
            INxTransCode * This,
            /* [in] */ BSTR in_bstrFile,
            /* [in] */ unsigned long in_ulIn,
            /* [in] */ unsigned long in_ulOut);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *TransElement_End )( 
            INxTransCode * This,
            /* [in] */ BOOL bUseFFmpeg);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *TransCode_End )( 
            INxTransCode * This);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *TransCode_SetWrapperFileType )( 
            INxTransCode * This,
            /* [in] */ ENsWrapperFileTypeIDL in_eWrapperFileTypeIDL);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *TransElement_GetCurrentPositionInFrames )( 
            INxTransCode * This,
            /* [out] */ ULONGLONG *out_iCurrentFramePosition);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *TransElement_SetFramesReaderStatus )( 
            INxTransCode * This,
            /* [in] */ BOOL in_bStatus);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *TransElement_SetMixSchedule )( 
            INxTransCode * This,
            /* [in] */ double in_ulMixStart,
            /* [in] */ double in_ulMixEnd);
        
        END_INTERFACE
    } INxTransCodeVtbl;

    interface INxTransCode
    {
        CONST_VTBL struct INxTransCodeVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define INxTransCode_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define INxTransCode_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define INxTransCode_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define INxTransCode_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define INxTransCode_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define INxTransCode_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define INxTransCode_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define INxTransCode_TransCode_Begin(This,in_bstrFile)	\
    ( (This)->lpVtbl -> TransCode_Begin(This,in_bstrFile) ) 

#define INxTransCode_TransElement_Begin(This)	\
    ( (This)->lpVtbl -> TransElement_Begin(This) ) 

#define INxTransCode_TransElement_AddVideoFile(This,in_bstrFile,in_ulIn,in_ulOut,in_bstrCam)	\
    ( (This)->lpVtbl -> TransElement_AddVideoFile(This,in_bstrFile,in_ulIn,in_ulOut,in_bstrCam) ) 

#define INxTransCode_TransElement_AddAudioFile(This,in_bstrFile,in_ulIn,in_ulOut)	\
    ( (This)->lpVtbl -> TransElement_AddAudioFile(This,in_bstrFile,in_ulIn,in_ulOut) ) 

#define INxTransCode_TransElement_End(This,bUseFFmpeg)	\
    ( (This)->lpVtbl -> TransElement_End(This,bUseFFmpeg) ) 

#define INxTransCode_TransCode_End(This)	\
    ( (This)->lpVtbl -> TransCode_End(This) ) 

#define INxTransCode_TransCode_SetWrapperFileType(This,in_eWrapperFileTypeIDL)	\
    ( (This)->lpVtbl -> TransCode_SetWrapperFileType(This,in_eWrapperFileTypeIDL) ) 

#define INxTransCode_TransElement_GetCurrentPositionInFrames(This,out_iCurrentFramePosition)	\
    ( (This)->lpVtbl -> TransElement_GetCurrentPositionInFrames(This,out_iCurrentFramePosition) ) 

#define INxTransCode_TransElement_SetFramesReaderStatus(This,in_bStatus)	\
    ( (This)->lpVtbl -> TransElement_SetFramesReaderStatus(This,in_bStatus) ) 

#define INxTransCode_TransElement_SetMixSchedule(This,in_ulMixStart,in_ulMixEnd)	\
    ( (This)->lpVtbl -> TransElement_SetMixSchedule(This,in_ulMixStart,in_ulMixEnd) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __INxTransCode_INTERFACE_DEFINED__ */



#ifndef __xTransCodeATLLib_LIBRARY_DEFINED__
#define __xTransCodeATLLib_LIBRARY_DEFINED__

/* library xTransCodeATLLib */
/* [version][uuid] */ 


EXTERN_C const IID LIBID_xTransCodeATLLib;

#ifndef ___INxTransCodeEvents_DISPINTERFACE_DEFINED__
#define ___INxTransCodeEvents_DISPINTERFACE_DEFINED__

/* dispinterface _INxTransCodeEvents */
/* [uuid] */ 


EXTERN_C const IID DIID__INxTransCodeEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("7E8B5055-A38D-40DC-B0E8-34226D14E1E6")
    _INxTransCodeEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _INxTransCodeEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _INxTransCodeEvents * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _INxTransCodeEvents * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _INxTransCodeEvents * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _INxTransCodeEvents * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _INxTransCodeEvents * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _INxTransCodeEvents * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _INxTransCodeEvents * This,
            /* [annotation][in] */ 
            _In_  DISPID dispIdMember,
            /* [annotation][in] */ 
            _In_  REFIID riid,
            /* [annotation][in] */ 
            _In_  LCID lcid,
            /* [annotation][in] */ 
            _In_  WORD wFlags,
            /* [annotation][out][in] */ 
            _In_  DISPPARAMS *pDispParams,
            /* [annotation][out] */ 
            _Out_opt_  VARIANT *pVarResult,
            /* [annotation][out] */ 
            _Out_opt_  EXCEPINFO *pExcepInfo,
            /* [annotation][out] */ 
            _Out_opt_  UINT *puArgErr);
        
        END_INTERFACE
    } _INxTransCodeEventsVtbl;

    interface _INxTransCodeEvents
    {
        CONST_VTBL struct _INxTransCodeEventsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _INxTransCodeEvents_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _INxTransCodeEvents_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _INxTransCodeEvents_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _INxTransCodeEvents_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define _INxTransCodeEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define _INxTransCodeEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define _INxTransCodeEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___INxTransCodeEvents_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_NxTransCode;

#ifdef __cplusplus

class DECLSPEC_UUID("E0A5A2E3-942B-41C4-A5EE-415CA76557B6")
NxTransCode;
#endif
#endif /* __xTransCodeATLLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  BSTR_UserSize(     unsigned long *, unsigned long            , BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserMarshal(  unsigned long *, unsigned char *, BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserUnmarshal(unsigned long *, unsigned char *, BSTR * ); 
void                      __RPC_USER  BSTR_UserFree(     unsigned long *, BSTR * ); 

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


