#pragma once

#ifdef XCOMPOSITORNODE_EXPORTS
#define XCOMPOSITORNODE_API __declspec(dllexport)
#else
#define XCOMPOSITORNODE_API __declspec(dllimport)
#endif

#include "xAPI.h"
#include "xLogger.h"
#include "CNxBaseNode.h"

// This class is exported from the XCOMPOSITORNODE.dll
class XCOMPOSITORNODE_API CNxCompositorNode : public CNxBaseNode
{
public:
	CNxCompositorNode(
		wchar_t *in_pwszNodeName,
		ENsTreeNodeType     in_eNsTreeNodeType,
		HRESULT            *out_phr);


	/** Destructor of the class. *
	<b>Remark:</b><ul><li>None</ul>
	*/

	virtual ~CNxCompositorNode();

	virtual HRESULT __cdecl  ThreadNodeProcessLoop();
	HRESULT SetOutputFormat(ENsSurfaceFormat in_eSurfaceFormat);
	HRESULT SetMixSchedule(double ul_offsetbegin, double ul_offsetend);
private:
	SNsdPoolSurfaceDescription m_sPoolSurfaceDescription;
	SNsdPoolSampleDescription m_sPoolSampleDescription;

	ENsSurfaceFormat m_eSurfaceFormatOutput;
	uint64_t m_ui64CompositerCount;

	unsigned long m_ulOffsetBegin;
	unsigned long m_ulOffsetEnd;
};
