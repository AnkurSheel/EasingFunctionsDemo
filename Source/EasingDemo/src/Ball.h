//  *******************************************************************************************************************
//  Ball   version:  1.0   Ankur Sheel  date: 2013/05/13
//  *******************************************************************************************************************
//  purpose:	
//  *******************************************************************************************************************
#ifndef Ball_h__
#define Ball_h__

#include "BaseEntity.h"
#include "Vector3.h"
#include "TweenManager.h"

namespace Base
{
	class cVector3;
	class cTweenManager;

	template<class BaseType, class SubType> 
	BaseType * GenericObjectCreationFunction();

	struct EasingTransitions;
}

namespace GameBase
{
	class cPhysicsComponent;
}

class cBall 
	: public GameBase::cBaseEntity
{
public:
	~cBall();
	Base::cString GetCurrentTransition();
	Base::cString GetCurrentEasingType();
	void OnKeyPressed(int key);

public:
	static const Base::cHashedString	m_EntityType;	///< The component name

private:
	cBall();
	void VInitialize() OVERRIDE;
	virtual void VOnUpdate(const float deltaTime) OVERRIDE;
	void VCleanup() OVERRIDE;
	void VOnRestart() OVERRIDE;

	Base::cHashedString VGetEntityType() const OVERRIDE { return m_EntityType; }
	unsigned long VGetHashedID() const OVERRIDE { return m_EntityType.GetHash(); }

	cBall * CastToBall();

private:
	float m_ElapsedTime;
	Base::EasingTransitions::Enum m_CurrentTransition;
	Base::EasingType::Enum m_CurrenEasingType;

private:
	template<class BaseType, class SubType> 
	friend BaseType * Base::GenericObjectCreationFunction();
};
#endif // Ball_h__