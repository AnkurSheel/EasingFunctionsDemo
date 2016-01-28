//  *******************************************************************************************************************
//  Singleton   version:  1.0   Ankur Sheel  date: 2015/11/25
//  *******************************************************************************************************************
//
//  *******************************************************************************************************************
#ifndef SINGLETON_H__
#define SINGLETON_H__

#define DECLARE_SINGLETON(type, API) \
public: \
  API static std::unique_ptr<type>& GetInstance(); \
  API static void Destroy(); \
  \
private: \
  static std::unique_ptr<type> s_pInstance;

#define IMPLEMENT_SINGLETON(type) \
std::unique_ptr<type> type::s_pInstance = nullptr; \
  \
std::unique_ptr<type>& type::GetInstance() \
{ \
  if (s_pInstance == nullptr) \
  { \
    s_pInstance = std::unique_ptr<type>(DEBUG_NEW type()); \
  } \
  return s_pInstance; \
} \
\
void type::Destroy() \
{ \
  s_pInstance.reset(); \
  s_pInstance = nullptr; \
} \

#endif  // SINGLETON_H__
