#include "stdafx.h"
#include "BTInverterDecoratorNode.h"

using namespace AI;

//  ********************************************************************************************************************
cBTInverterDecoratorNode::cBTInverterDecoratorNode()
{
}

//  ********************************************************************************************************************
cBTInverterDecoratorNode::~cBTInverterDecoratorNode()
{
}

//  ********************************************************************************************************************
BT_STATUS::Enum cBTInverterDecoratorNode::VOnUpdate(void * pOwner, float deltaTime)
{
	BT_STATUS::Enum result = cBTDecoratorNode::VOnUpdate(pOwner, deltaTime);
	if (result == BT_STATUS::Success)
	{
		return BT_STATUS::Failure;
	}
	if (result == BT_STATUS::Failure)
	{
		return BT_STATUS::Success;
	}
	return result;
}
