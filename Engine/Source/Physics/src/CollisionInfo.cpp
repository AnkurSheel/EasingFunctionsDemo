#include "stdafx.h"
#include "CollisionInfo.h"
#include "RigidBody.h"
#include "Shape.hxx"
#include "RectangleShape.h"
#include "Vector2.h"
#include "CircleShape.h"

using namespace Physics;
using namespace Base;

//  *******************************************************************************************************************
cCollisionInfo::cCollisionInfo(cRigidBody * const pBodyA, cRigidBody * const pBodyB)
	: m_pBodyA(pBodyA)
	, m_pBodyB(pBodyB)
	, m_PenetrationDistance(0.0f)
	, m_Collided(false)
{
}

//  *******************************************************************************************************************
cCollisionInfo::~cCollisionInfo()
{
}

//  *******************************************************************************************************************
cCollisionInfo::cCollisionInfo(const cCollisionInfo & other)
	: m_pBodyA(other.m_pBodyA)
	, m_pBodyB(other.m_pBodyB)
	, m_Normal(other.m_Normal)
	, m_PenetrationDistance(other.m_PenetrationDistance)
	, m_Collided(other.m_Collided)
{
}

//  *******************************************************************************************************************
void cCollisionInfo::Solve()
{
	if (isZero(m_pBodyA->GetInverseMass() + m_pBodyB->GetInverseMass()))
	{
		InfiniteMassCorrection();
		return;
	}

	if (m_pBodyA->GetCollisionShape()->VGetName() == cRectangleShape::m_Name)
	{
		if (m_pBodyB->GetCollisionShape()->VGetName() == cRectangleShape::m_Name)
		{
			AABBvsAABB();
		}
		else if (m_pBodyB->GetCollisionShape()->VGetName() ==  cCircleShape::m_Name)
		{
			AABBVsCircle();
		}
	}
	else if (m_pBodyA->GetCollisionShape()->VGetName() == cCircleShape::m_Name)
	{
		if (m_pBodyB->GetCollisionShape()->VGetName() == cRectangleShape::m_Name)
		{
			CircleVsAABB();
		}
		else if (m_pBodyB->GetCollisionShape()->VGetName() == cCircleShape::m_Name)
		{
			CircleVsCircle();
		}
	}
}

//  *******************************************************************************************************************
void cCollisionInfo::ApplyImpulse()
{
	if (m_pBodyA->VIsTrigger() == true || m_pBodyB->VIsTrigger() == true)
	{
		return;
	}

	// Calculate relative velocity
	cVector3 rv = m_pBodyA->GetLinearVelocity() - m_pBodyB->GetLinearVelocity();

	float velAlongNormal = rv.Dot(m_Normal);

	// Do not resolve if velocities are separating
	if (velAlongNormal > 0)
	{
		return;
	}

	// Calculate restitution
	float e = min(m_pBodyA->GetRestitution(), m_pBodyB->GetRestitution());

	// Calculate impulse scalar
	float j = -(1 + e) * velAlongNormal;
	j /= m_pBodyA->GetInverseMass() + m_pBodyB->GetInverseMass();

	// Apply impulse
	cVector3 impulse = j * m_Normal;
	m_pBodyA->VApplyImpulse(impulse);
	m_pBodyB->VApplyImpulse(impulse.GetReverse());
}

//  *******************************************************************************************************************
void cCollisionInfo::ApplyPositionCorrection()
{
	if (m_pBodyA->VIsTrigger() || m_pBodyB->VIsTrigger())
	{
		return;
	}

	const float slop = 0.05f;  // Penetration allowance
	const float percent = 0.8f;  // Penetration percentage to correct
	if (isZero(m_PenetrationDistance - slop))
	{
		return;
	}
	cVector3 correction = (m_PenetrationDistance / (m_pBodyA->GetInverseMass() + m_pBodyB->GetInverseMass())) * m_Normal * percent;
	m_pBodyA->ApplyPositionCorrection(correction);
	m_pBodyB->ApplyPositionCorrection(correction.GetReverse());
}

//  *******************************************************************************************************************
void cCollisionInfo::InfiniteMassCorrection()
{
	m_pBodyA->SetLinearVelocity(cVector3::Zero);
	m_pBodyB->SetLinearVelocity(cVector3::Zero);
}

//  *******************************************************************************************************************
void cCollisionInfo::CircleVsCircle()
{
	cVector3 centerDelta = m_pBodyA->GetCollisionShape()->VGetCenter() -  m_pBodyB->GetCollisionShape()->VGetCenter();

	float radiusSum = m_pBodyA->GetCollisionShape()->VGetRadius() + m_pBodyB->GetCollisionShape()->VGetRadius();
	float radiusSumSquared = radiusSum * radiusSum;

	m_Collided = (centerDelta.LengthSquared() <= radiusSumSquared);

	if (m_Collided)
	{
		float distance = centerDelta.Length();
		if (!isZero(distance))
		{
			m_PenetrationDistance = radiusSum - distance;
			m_Normal = centerDelta / distance;
		}
		else
		{
			m_PenetrationDistance = m_pBodyA->GetCollisionShape()->VGetRadius();
			m_Normal = cVector3(1, 0, 0);
		}
	}
}

//  *******************************************************************************************************************
void cCollisionInfo::AABBvsAABB()
{
	cVector3 centerDelta = m_pBodyA->GetCollisionShape()->VGetCenter() -  m_pBodyB->GetCollisionShape()->VGetCenter();
	cVector3 overlap = centerDelta;
	overlap.AbsTo();

	cVector3 halfExtentSum = m_pBodyA->GetCollisionShape()->VGetHalfExtents() + m_pBodyB->GetCollisionShape()->VGetHalfExtents();
	overlap = halfExtentSum - overlap;

	m_Collided = overlap.x > 0 && overlap.y > 0;
	if (m_Collided)
	{
		cVector2 smallestOverlap = cVector2(overlap.x, overlap.y);
		smallestOverlap = smallestOverlap.MinorAxis();

		cVector3 smallestOverlap3 = cVector3(smallestOverlap.x, smallestOverlap.y, 0.0f);
		m_Normal = (centerDelta * smallestOverlap3).MajorAxis();
		m_PenetrationDistance = (overlap * m_Normal).Length();
		m_Normal.Normalize();
	}
}

//  *******************************************************************************************************************
void cCollisionInfo::CircleVsAABB()
{
	std::swap(m_pBodyA, m_pBodyB);
	AABBVsCircle();
	m_Normal.NegTo();
}

//  *******************************************************************************************************************
void cCollisionInfo::AABBVsCircle()
{
	cVector3 centerDelta = m_pBodyA->GetCollisionShape()->VGetCenter() -  m_pBodyB->GetCollisionShape()->VGetCenter();
	cVector3 halfExtentBodyA = m_pBodyA->GetCollisionShape()->VGetHalfExtents();

	// Closest point on A to center of B
	cVector3 closestPoint = centerDelta;
	closestPoint.x = Clamp<float>(closestPoint.x, -halfExtentBodyA.x, halfExtentBodyA.x);
	closestPoint.y = Clamp<float>(closestPoint.y, -halfExtentBodyA.y, halfExtentBodyA.y);

	bool inside = false;

	// Circle is inside the AABB, so we need to clamp the circle's center to the closest edge
	if (centerDelta == closestPoint)
	{
		inside = true;
		if (centerDelta.MajorAxis() == cVector3(1.0f, 0.0f, 0.0f))
		{
			if (closestPoint.x > 0)
			{
				closestPoint.x = halfExtentBodyA.x;
			}
			else
			{
				closestPoint.x = - halfExtentBodyA.x;
			}
		}
		else
		{
			if (closestPoint.y > 0)
			{
				closestPoint.y = halfExtentBodyA.y;
			}
			else
			{
				closestPoint.y = - halfExtentBodyA.y;
			}
		}
	}
	cVector3 normal = centerDelta - closestPoint;
	float distance = normal.LengthSquared();
	float radius = m_pBodyB->GetCollisionShape()->VGetRadius();

	// Early out of the radius is shorter than distance to closest point and Circle not inside the AABB
	if (distance > (radius * radius) && !inside)
	{
		return;
	}

	distance = normal.Length();
	m_PenetrationDistance = radius + distance;

	m_Normal = centerDelta;
	if (inside)
	{
		m_Normal.NegTo();
	}
	m_Normal.Normalize();
}
