//  *******************************************************************************************************************
//  BTPrioritySelector   version:  1.0   Ankur Sheel  date: 2014/12/19
//  *******************************************************************************************************************
//
//  *******************************************************************************************************************
#ifndef __BTPRIORITYSELECTOR_H__
#define __BTPRIORITYSELECTOR_H__

#include "AIDefines.h"
#include "BTBaseSelectorNode.h"

namespace AI
{
	class cBTNodeFactory;
}

namespace AI
{

	class cBTPrioritySelectorNode
	: public cBTBaseSelectorNode
	{
	public:
		~cBTPrioritySelectorNode();
#ifdef UNITTEST
		int GetSize() { return m_Children.size(); }
#endif
	private:
		AI_API cBTPrioritySelectorNode();
		void VCalculateNextChildIndex() OVERRIDE;
		bool VIsDone() OVERRIDE;
		friend class cBTNodeFactory;
	};
}  // namespace AI
#endif  // __BTPRIORITYSELECTOR_H__
