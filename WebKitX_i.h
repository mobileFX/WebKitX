

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0555 */
/* at Mon Dec 15 03:01:33 2014
 */
/* Compiler settings for WebKitX.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 7.00.0555 
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


#ifndef __WebKitX_i_h__
#define __WebKitX_i_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef ___DWebKitX_FWD_DEFINED__
#define ___DWebKitX_FWD_DEFINED__
typedef interface _DWebKitX _DWebKitX;
#endif 	/* ___DWebKitX_FWD_DEFINED__ */


#ifndef ___DWebKitXEvents_FWD_DEFINED__
#define ___DWebKitXEvents_FWD_DEFINED__
typedef interface _DWebKitXEvents _DWebKitXEvents;
#endif 	/* ___DWebKitXEvents_FWD_DEFINED__ */


#ifndef __WebKitX_FWD_DEFINED__
#define __WebKitX_FWD_DEFINED__

#ifdef __cplusplus
typedef class WebKitX WebKitX;
#else
typedef struct WebKitX WebKitX;
#endif /* __cplusplus */

#endif 	/* __WebKitX_FWD_DEFINED__ */


/* header files for imported files */
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 



#ifndef __WebKitXLib_LIBRARY_DEFINED__
#define __WebKitXLib_LIBRARY_DEFINED__

/* library WebKitXLib */
/* [control][version][uuid] */ 


EXTERN_C const IID LIBID_WebKitXLib;

#ifndef ___DWebKitX_DISPINTERFACE_DEFINED__
#define ___DWebKitX_DISPINTERFACE_DEFINED__

/* dispinterface _DWebKitX */
/* [uuid] */ 


EXTERN_C const IID DIID__DWebKitX;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("ECEC4ADA-DE49-4358-B708-C3410F8CDDDA")
    _DWebKitX : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _DWebKitXVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _DWebKitX * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _DWebKitX * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _DWebKitX * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _DWebKitX * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _DWebKitX * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _DWebKitX * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _DWebKitX * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } _DWebKitXVtbl;

    interface _DWebKitX
    {
        CONST_VTBL struct _DWebKitXVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _DWebKitX_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _DWebKitX_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _DWebKitX_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _DWebKitX_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define _DWebKitX_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define _DWebKitX_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define _DWebKitX_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___DWebKitX_DISPINTERFACE_DEFINED__ */


#ifndef ___DWebKitXEvents_DISPINTERFACE_DEFINED__
#define ___DWebKitXEvents_DISPINTERFACE_DEFINED__

/* dispinterface _DWebKitXEvents */
/* [uuid] */ 


EXTERN_C const IID DIID__DWebKitXEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("3EE33618-BC27-40E9-B73D-87A49DEF0C8A")
    _DWebKitXEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _DWebKitXEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _DWebKitXEvents * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _DWebKitXEvents * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _DWebKitXEvents * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _DWebKitXEvents * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _DWebKitXEvents * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _DWebKitXEvents * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _DWebKitXEvents * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } _DWebKitXEventsVtbl;

    interface _DWebKitXEvents
    {
        CONST_VTBL struct _DWebKitXEventsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _DWebKitXEvents_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _DWebKitXEvents_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _DWebKitXEvents_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _DWebKitXEvents_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define _DWebKitXEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define _DWebKitXEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define _DWebKitXEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___DWebKitXEvents_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_WebKitX;

#ifdef __cplusplus

class DECLSPEC_UUID("D9EA077F-4E5F-4BBD-9ACD-02E7021CDEFE")
WebKitX;
#endif
#endif /* __WebKitXLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


