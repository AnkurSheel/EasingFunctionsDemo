//  *******************************************************************************************************************
//  BTSequenceNode   version:  1.0   Ankur Sheel  date: 2014/12/09
//  *******************************************************************************************************************
//
//  *******************************************************************************************************************
#ifndef __BTSEQUENCENODE_H__
#define __BTSEQUENCENODE_H__

#include "AIDefines.h"
#include "BTCompositeNode.h"
namespace AI
{
	class cBTNodeFactory;
}

namespace AI
{
	class cBTSequenceNode
		: public cBTCompositeNode
	{
	public:
		~cBTSequenceNode();
		void VOnInitialize(void * pOwner) OVERRIDE;
		BT_STATUS::Enum VOnUpdate(void * pOwner, float deltaTime) OVERRIDE;
		int GetActiveChildIndex() const { return m_CurrentChildIndex; }

	private:
		AI_API cBTSequenceNode();

	private:
		int m_CurrentChildIndex;

		friend class cBTNodeFactory;
	};
}  // namespace AI
#endif  // __BTSEQUENCENODE_H__
