//  *******************************************************************************************************************
//  BTBaseSelectorNode   version:  1.0   Ankur Sheel  date: 2014/12/19
//  *******************************************************************************************************************
//
//  *******************************************************************************************************************
#ifndef __BTBASESELECTORNODE_H__
#define __BTBASESELECTORNODE_H__

#include "BTCompositeNode.h"

namespace AI
{
	class cBTBaseSelectorNode
	: public cBTCompositeNode
	{
	public:
		virtual ~cBTBaseSelectorNode();
		AI_API int GetCurrentChildIndex() const { return m_CurrentChildIndex; }

	protected:
		virtual void VCalculateNextChildIndex() = 0;
		virtual bool VIsDone() = 0;
		AI_API void VOnInitialize(void * pOwner) OVERRIDE;
		AI_API BT_STATUS::Enum VOnUpdate(void * pOwner, float deltaTime) OVERRIDE;

	protected:
		AI_API cBTBaseSelectorNode();

	protected:
		int m_CurrentChildIndex;
	};
}  // namespace AI
#endif  // __BTBASESELECTORNODE_H__
