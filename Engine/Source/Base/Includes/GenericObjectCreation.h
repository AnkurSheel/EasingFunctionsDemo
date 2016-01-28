//  *******************************************************************************************************************
//  GenericObjectCreation version:  1.0   Ankur Sheel  date: 2013/05/07
//  *******************************************************************************************************************
//  purpose:
//  *******************************************************************************************************************
#ifndef GenericObjectCreation_h__
#define GenericObjectCreation_h__

namespace Base
{
  template <class BaseType, class SubType>
  shared_ptr<BaseType> GenericObjectCreationFunction()
  {
    return shared_ptr<BaseType>(DEBUG_NEW SubType);
  }

  template <class BaseType, class SubType>
  shared_ptr<BaseType> GenericObjectDuplicationFunction(shared_ptr<BaseType> const pObject)
  {
    shared_ptr<SubType> pSubObject = dynamic_pointer_cast<SubType>(pObject);
    if (pSubObject != NULL)
    {
      return shared_ptr<BaseType>(std::make_shared<SubType>(*pSubObject));
    }
    return NULL;
  }

  template <class BaseClass, class IdType>
  class GenericObjectFactory
  {
    typedef shared_ptr<BaseClass> (*ObjectCreationFunction)(void);
    std::map<IdType, ObjectCreationFunction> m_creationFunctions;

    typedef shared_ptr<BaseClass> (*ObjectDuplicationFunction)(shared_ptr<BaseClass> const);
    std::map<IdType, ObjectDuplicationFunction> m_DuplicationFunctions;

  public:
    template <class SubClass>
    bool Register(IdType id)
    {
      auto findIt = m_creationFunctions.find(id);
      if (findIt == m_creationFunctions.end())
      {
        m_creationFunctions[id] = &GenericObjectCreationFunction<BaseClass, SubClass>;
        m_DuplicationFunctions[id] = &GenericObjectDuplicationFunction<BaseClass, SubClass>;
        return true;
      }
      return false;
    }

    shared_ptr<BaseClass> Create(IdType id)
    {
      auto findIt = m_creationFunctions.find(id);
      if (findIt != m_creationFunctions.end())
      {
        ObjectCreationFunction pFunc = findIt->second;
        return pFunc();
      }

      return NULL;
    }

    shared_ptr<BaseClass> Duplicate(IdType id, shared_ptr<BaseClass> const pObject)
    {
      auto findIt = m_DuplicationFunctions.find(id);
      if (findIt != m_DuplicationFunctions.end())
      {
        ObjectDuplicationFunction pFunc = findIt->second;
        return pFunc(pObject);
      }
      return NULL;
    }
  };
}  // namespace Base
#endif  // GenericObjectCreation_h__
