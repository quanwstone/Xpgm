

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


 /* File created by MIDL compiler version 8.00.0603 */
/* at Fri Oct 27 16:29:23 2017
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


#ifdef __cplusplus
extern "C"{
#endif 


#include <rpc.h>
#include <rpcndr.h>

#ifdef _MIDL_USE_GUIDDEF_

#ifndef INITGUID
#define INITGUID
#include <guiddef.h>
#undef INITGUID
#else
#include <guiddef.h>
#endif

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        DEFINE_GUID(name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8)

#else // !_MIDL_USE_GUIDDEF_

#ifndef __IID_DEFINED__
#define __IID_DEFINED__

typedef struct _IID
{
    unsigned long x;
    unsigned short s1;
    unsigned short s2;
    unsigned char  c[8];
} IID;

#endif // __IID_DEFINED__

#ifndef CLSID_DEFINED
#define CLSID_DEFINED
typedef IID CLSID;
#endif // CLSID_DEFINED

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        const type name = {l,w1,w2,{b1,b2,b3,b4,b5,b6,b7,b8}}

#endif !_MIDL_USE_GUIDDEF_

MIDL_DEFINE_GUID(IID, IID_INxTransCode,0x221C6680,0x050B,0x4B2A,0xA3,0xDF,0x14,0x5F,0xE1,0xF9,0xBD,0x4E);


MIDL_DEFINE_GUID(IID, LIBID_xTransCodeATLLib,0xA9706DE5,0x4949,0x4192,0x89,0xE9,0x10,0xBB,0x14,0xC7,0x2C,0x48);


MIDL_DEFINE_GUID(IID, DIID__INxTransCodeEvents,0x7E8B5055,0xA38D,0x40DC,0xB0,0xE8,0x34,0x22,0x6D,0x14,0xE1,0xE6);


MIDL_DEFINE_GUID(CLSID, CLSID_NxTransCode,0xE0A5A2E3,0x942B,0x41C4,0xA5,0xEE,0x41,0x5C,0xA7,0x65,0x57,0xB6);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



