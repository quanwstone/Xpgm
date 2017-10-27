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

#include "stdafx.h"

#include "CNsTransationParameter.h"
#include "xLogAPI.h"
#include "xLogger.h"


//////////////////////////////////////////////////////////////////////////
// CNsTransationParameter.h
//

CNsTransationParameter::CNsTransationParameter(HRESULT *out_phr) 
: CNsPoolElement(L"NewAutoVideo - AttachedAudioSample", NULL)
{
	*out_phr =  NS_NOERROR;
	int temp = 0;
	int j = temp++;
}


CNsTransationParameter::~CNsTransationParameter()
{

}


STDMETHODIMP CNsTransationParameter::NonDelegatingQueryInterface(REFIID in_rIID, void **out_ppv) 
{
	if (in_rIID == IID_INsTransationParameter)
	{
		return NsGetInterface((INsTransationParameter*)this, out_ppv);
	}
	else
	{
		return CNsPoolElement::NonDelegatingQueryInterface(in_rIID, out_ppv);
	}
}


HRESULT __stdcall CNsTransationParameter::SetTransationParam(double  in_dParam)
{
	HRESULT hr = NS_NOERROR;
	CNsAutoLock lock(&m_csTransactionParamLock);
	m_dTransactionParam = in_dParam;
	return hr;
}
HRESULT __stdcall CNsTransationParameter::GetTransationParam(double  *out_dParam)
{
	HRESULT hr = NS_NOERROR;
	CNsAutoLock lock(&m_csTransactionParamLock);

	*out_dParam = m_dTransactionParam;

	return hr;
}
