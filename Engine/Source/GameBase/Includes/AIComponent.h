//  *******************************************************************************************************************
//  AIComponent   version:  1.0   Ankur Sheel  date: 2015/05/07
//  *******************************************************************************************************************
//
//  *******************************************************************************************************************

#ifndef AICOMPONENT_H__
#define AICOMPONENT_H__

#include "BaseComponent.h"
#include "GameBaseDefines.h"

namespace AI
{
  class cBTBehaviorTree;
}

namespace Base
{
  template <class BaseType, class SubType>
  shared_ptr<BaseType> GenericObjectCreationFunction();
}

namespace GameBase
{
  class cAIComponent : public cBaseComponent
  {
  public:
    ~cAIComponent();
    GAMEBASE_API void VInitialize(const Utilities::IXMLNode* const pXMLNode) OVERRIDE;
    GAMEBASE_API void VCleanup() OVERRIDE;
    shared_ptr<Utilities::IXMLNode> VGenerateXml() const OVERRIDE;
    GAMEBASE_API static Base::cHashedString GetName() { return m_Name; }
    void SetBrain(shared_ptr<AI::cBTBehaviorTree> pBrain) { m_pBrain = pBrain; }
    void Update(const float deltaTime);

  private:
    UINT64 VGetHashedID() const { return m_Name.GetHash(); }
    cAIComponent();

  private:
    shared_ptr<AI::cBTBehaviorTree> m_pBrain;
    static Base::cHashedString m_Name;  ///< The component name

  private:
    template <class BaseType, class SubType>
    friend shared_ptr<BaseType> Base::GenericObjectCreationFunction();
  };
}  // namespace GameBase
#endif  // AICOMPONENT_H__
