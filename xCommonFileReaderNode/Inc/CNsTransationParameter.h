//==========================================================================;
//
// (c) Copyright CS Team, 2017. All rights reserved. 
//
// This code and information is provided "as is" without warranty of any kind, 
// either expressed or implied, including but not limited to the implied 
// warranties of merchantability and/or fitness for a particular purpose.
//
//--------------------------------------------------------------------------;
//
//----------------------------------------------------------------
//   Birth Date:       Nov. 29 2006
//   Operating System: Win7
//   Author:           hanzt
//----------------------------------------------------------------
//   
//   Beijing China. 
//----------------------------------------------------------------

//////////////////////////////////////////////////////////////////////////
// CNsTransationParameter.h
//

#pragma once

#include "nsPoolManagerAPI.h"
#include "cnsdpoolelementallocator.h"
#include "xAPI.h"


class CNsTransationParameter :	public CNsPoolElement,
	public INsTransationParameter
	                            
{
public:
	CNsTransationParameter(HRESULT *out_phr);

    virtual ~CNsTransationParameter(); 

//
// COM interface
//

    NSDECLARE_IUNKNOWN;

    virtual STDMETHODIMP NonDelegatingQueryInterface(REFIID in_rIID, void **out_ppv); 

public:
//
// INsTransationParameter interface
//
	virtual HRESULT __stdcall  SetTransationParam(double  in_dParam);
	virtual HRESULT __stdcall  GetTransationParam(double  *out_dParam);

//
protected:
   CNsCriticalSection   m_csTransactionParamLock;
   double m_dTransactionParam;
};

// eof
