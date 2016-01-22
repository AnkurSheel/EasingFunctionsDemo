#include "stdafx.h"
#include "MessageDispatchManager.h"
#include "Timer.h"
#include "EntityManager.hxx"
#include "Telegram.h"
#include "BaseEntity.h"

using namespace Utilities;
using namespace AI;
using namespace Base;
using namespace GameBase;

//  *******************************************************************************************************************
cMessageDispatchManager::cMessageDispatchManager()
	: m_pTimer(cTimer::CreateTimer())
{
	m_pTimer->StartTimer();
	m_pEntityManager = cServiceLocator::GetInstance()->GetService<IEntityManager>();
}

//  *******************************************************************************************************************
cMessageDispatchManager::~cMessageDispatchManager()
{
	m_pTimer.reset();
	m_pEntityManager.reset();
}

//  *******************************************************************************************************************
void cMessageDispatchManager::VDispatchMessage(const double delay, const int senderID, const int recieverID,
	const unsigned msgId, shared_ptr<void> pExtraInfo)
{
	shared_ptr<IEntityManager> pEntityManager = MakeStrongPtr<IEntityManager>(m_pEntityManager);
	if (pEntityManager != NULL)
	{
		return;
	}

	shared_ptr<IBaseEntity> pReciever = pEntityManager->VGetEntityFromID(recieverID);
	Telegram telegram(senderID, recieverID, msgId, 0.0, pExtraInfo);
	if (delay <= 0.0)
	{
		SP_LOG(2, "Sending msg immediately")(msgId)(pEntityManager->VGetEntityNameFromID(senderID))(pEntityManager->VGetEntityNameFromID(recieverID));
		Discharge(pReciever, telegram);
	}
	else
	{
		SP_LOG(2, "Sending msg immediately")(msgId)(delay)(pEntityManager->VGetEntityNameFromID(senderID))(pEntityManager->VGetEntityNameFromID(recieverID));
		double dCurrentTime = m_pTimer->GetRunningTime();
		telegram.m_DispatchTime = dCurrentTime + delay;
		m_PriorityQueue.insert(telegram);
	}
}

//  *******************************************************************************************************************
void cMessageDispatchManager::VOnUpdate()
{
	m_pTimer->OnUpdate();
	DispatchDelayedMessage();
}

//  *******************************************************************************************************************
void cMessageDispatchManager::DispatchDelayedMessage()
{
	shared_ptr<IEntityManager> pEntityManager = MakeStrongPtr<IEntityManager>(m_pEntityManager);
	if (pEntityManager == NULL)
	{
		return;
	}
	double currentTime = m_pTimer->GetRunningTime();
	while (!m_PriorityQueue.empty()
		&& m_PriorityQueue.begin()->m_DispatchTime < currentTime
		&& m_PriorityQueue.begin()->m_DispatchTime > 0)
	{
		Telegram telegram = *m_PriorityQueue.begin();
		shared_ptr<IBaseEntity> pReciever = pEntityManager->VGetEntityFromID(telegram.m_ReceiverID);
		Discharge(pReciever, telegram);
		m_PriorityQueue.erase(m_PriorityQueue.begin());
	}
}

//  *******************************************************************************************************************
void cMessageDispatchManager::Discharge(shared_ptr<IBaseEntity>const pReceiver, const AI::Telegram& msg)
{
	if (pReceiver->VOnHandleMessage(msg))
	{
		if (!m_pEntityManager.expired())
		{
			SP_LOG(2, "Message Handled")(msg.m_MsgID)(MakeStrongPtr(m_pEntityManager)->VGetEntityName(pReceiver));
		}
	}
	else
	{
		if (!m_pEntityManager.expired())
		{
			SP_LOG(2, "Message Not Handled")(msg.m_MsgID)(MakeStrongPtr(m_pEntityManager)->VGetEntityName(pReceiver));
		}
	}
}

//  *******************************************************************************************************************
void IMessageDispatchManager::CreateAsService()
{
	cServiceLocator::GetInstance()->Register<IMessageDispatchManager>(shared_ptr<IMessageDispatchManager>(DEBUG_NEW cMessageDispatchManager()));
}
