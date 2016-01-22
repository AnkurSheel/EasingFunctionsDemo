//  *******************************************************************************************************************
//  BTRepeatDecoratorNode   version:  1.0   Ankur Sheel  date: 2014/12/30
//  *******************************************************************************************************************
//
//  *******************************************************************************************************************
#ifndef __BTREPEATDECORATORNODE_H__
#define __BTREPEATDECORATORNODE_H__

#include "BTDecoratorNode.h"
#include "Optional.h"

namespace AI
{
	class cBTLoopDecoratorNode
		: public cBTDecoratorNode
	{
		DEFINE_NODE_PROPERTY(cBTLoopDecoratorNode, Base::tOptional<int>, Repeats)

	public:
		~cBTLoopDecoratorNode();

	private:
		AI_API cBTLoopDecoratorNode();
		void VOnInitialize(void * pOwner) OVERRIDE;
		BT_STATUS::Enum VOnUpdate(void * pOwner, float deltaTime) OVERRIDE;
		void VReset() OVERRIDE;

	private:
		int m_Count;

		friend class cBTNodeFactory;
	};
}  // namespace AI
#endif  // __BTREPEATDECORATORNODE_H__
