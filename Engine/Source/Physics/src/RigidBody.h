//  *******************************************************************************************************************
//  RigidBody version:  1.0   Ankur Sheel  date: 2013/05/23
//  *******************************************************************************************************************
//  purpose:
//  *******************************************************************************************************************
#ifndef RigidBody_h
#define RigidBody_h

#include "RigidBody.hxx"
#include "vector3.h"

namespace Physics
{
	class IShape;
	class cQTNode;
}

namespace Physics
{
	class cRigidBody
		: public IRigidBody
	{
	public:
		cRigidBody();
		~cRigidBody();
		cRigidBody(const cRigidBody & other);
		cRigidBody & operator =(const cRigidBody & other);

		bool VIsTrigger() const OVERRIDE { return m_IsTrigger; }
		void VApplyImpulse(const Base::cVector3 & force) OVERRIDE;
		int VGetID() const OVERRIDE { return m_ID; }

		void OnCreated(shared_ptr<const stRigidBodyDef> pDef, const int ID);
		bool Sync(const float alpha);
		void IntegrateForces(const float timestep);
		void IntegrateVelocity(const float timestep);
		void ApplyGravity(const float timeStep);
		void SetLinearVelocity(const Base::cVector3 & velocity) { m_LinearVelocity = velocity; }
		void ApplyPositionCorrection(const Base::cVector3 & correction);

		cQTNode * const GetNode() const { return m_pNode; }
		void SetNode(cQTNode * const pNode) { m_pNode = pNode; }
		IShape * GetCollisionShape() const { return m_pCollisionShape; }
		bool IsInitialized() const { return m_Initialized; }
		bool IsKinematic() const { return m_Kinematic; }
		float GetInverseMass() const { return m_InverseMass; }
		float GetRestitution() const { return m_MaterialData.restitution; }
		Base::cVector3 GetLinearVelocity() const { return m_LinearVelocity; }
		bool IsActive() const { return m_Active; }

	private:
		void VInitialize(const Base::cVector3 & position) OVERRIDE;
		void VApplyForce(const Base::cVector3 & Direction, const float Newtons) OVERRIDE;
		void VUpdateCollisionShape(const Base::cVector3 & minBound, const Base::cVector3 & maxBound) OVERRIDE;
		void VGetBounds(Base::cVector3& minBound, Base::cVector3 & maxBound) OVERRIDE;
		void VStop() OVERRIDE;
		void VSetActive(bool active) OVERRIDE;
		void VSetAsTrigger(bool isTrigger) OVERRIDE { m_IsTrigger = isTrigger; }
		Base::cVector3 VGetRenderPosition() const OVERRIDE { return m_RenderPosition; }
		void OnMoved();

	private:
		Base::cVector3 m_LinearVelocity;
		Base::cVector3 m_Position;
		Base::cVector3 m_PreviousPosition;
		Base::cVector3 m_RenderPosition;
		Base::cVector3 m_Force;
		IShape * m_pCollisionShape;
		cQTNode * m_pNode;
		stMaterialData m_MaterialData;
		float m_InverseMass;
		float m_LinearDamping;
		float m_TopSpeed;
		int m_ID;
		bool m_ApplyGravity;
		bool m_Initialized;
		bool m_Kinematic;
		bool m_IsTrigger;
		bool m_Active;
	};
}  // namespace Physics
#endif  // RigidBody_h
