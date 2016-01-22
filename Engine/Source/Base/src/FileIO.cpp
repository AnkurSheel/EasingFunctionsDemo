#include "stdafx.h"
#include "FileIO.h"
#include <errno.h>
#include <fstream>

using namespace Base;
using namespace std;

class cFileIO::cFileIOImpl
{
public:
  cFileIOImpl();
  ~cFileIOImpl();
  bool OpenFile(const cString& fileName, const ios_base::openmode mode);
  void CloseFile();
  bool WriteLine(const cString& line);
  virtual bool Write(const char* pBuffer, uint64 size);
  bool VReadAll(const char* pBuffer);
  bool Read(char* const pBuffer, uint64 size);
  cString ReadLine();
  void Flush();
  const cString& GetFileName() { return m_FileName; }
  bool IsEOF() const { return m_FileStream.eof(); }
  uint64 GetFileSize() const { return m_FileSize; }
  uint32 GetCurrentInputPosition();
  void SetCurrentInputPosition(const int relativePosition, ios_base::seekdir dir);

protected:
  fstream m_FileStream;
  cString m_FileName;
  uint64 m_FileSize;
};

//  *******************************************************************************************************************
cFileIO::cFileIO()
  : m_pImpl(DEBUG_NEW cFileIOImpl())
{
}

//  *******************************************************************************************************************
cFileIO::~cFileIO()
{
}

//  *******************************************************************************************************************
bool cFileIO::OpenFile(const cString& fileName, const ios_base::openmode mode)
{
  return m_pImpl->OpenFile(fileName, mode);
}

//  *******************************************************************************************************************
void cFileIO::CloseFile()
{
  m_pImpl->CloseFile();
}

//  *******************************************************************************************************************
bool cFileIO::WriteLine(const cString& text)
{
  return m_pImpl->WriteLine(text);
}

//  *******************************************************************************************************************
bool cFileIO::Write(const char* pBuffer, uint64 size)
{
  return m_pImpl->Write(pBuffer, size);
}

//  *******************************************************************************************************************
bool cFileIO::Read(char* const pBuffer, uint64 size)
{
  return m_pImpl->Read(pBuffer, size);
}

//  *******************************************************************************************************************
cString cFileIO::ReadLine()
{
  return m_pImpl->ReadLine();
}

//  *******************************************************************************************************************
void cFileIO::Flush()
{
  m_pImpl->Flush();
}

//  *******************************************************************************************************************
const cString& cFileIO::GetFileName()
{
  return m_pImpl->GetFileName();
}

//  *******************************************************************************************************************
bool cFileIO::IsEOF() const
{
  return m_pImpl->IsEOF();
}

//  *******************************************************************************************************************
uint64 cFileIO::GetFileSize() const
{
  return m_pImpl->GetFileSize();
}

//  *******************************************************************************************************************
uint32 cFileIO::GetCurrentInputPosition()
{
  return m_pImpl->GetCurrentInputPosition();
}

//  *******************************************************************************************************************
void cFileIO::SetCurrentInputPosition(const int relativePosition, ios_base::seekdir dir)
{
  m_pImpl->SetCurrentInputPosition(relativePosition, dir);
}

//  *******************************************************************************************************************
unique_ptr<cFileIO> cFileIO::CreateFileIO()
{
  return unique_ptr<cFileIO>(DEBUG_NEW cFileIO());
}

//  *******************************************************************************************************************
cFileIO::cFileIOImpl::cFileIOImpl()
  : m_FileSize(0)
{
}

//  *******************************************************************************************************************
cFileIO::cFileIOImpl::~cFileIOImpl()
{
  CloseFile();
}

//  *******************************************************************************************************************
bool cFileIO::cFileIOImpl::OpenFile(const cString& fileName, const ios_base::openmode mode)
{
  if (m_FileStream.is_open())
  {
    CloseFile();
  }

  if (fileName.IsEmpty())
  {
    SP_ASSERT_WARNING(false).SetCustomMessage("FileName is empty");
    return false;
  }

  m_FileName = fileName;
  m_FileStream.open(m_FileName.GetData(), mode);

  SP_ASSERT(m_FileStream.good()).SetCustomMessage("Could not open file " + m_FileName + " : " + strerror(errno));
  if (!m_FileStream.good())
  {
    return false;
  }
  SP_LOG(2, "Opened file: " + m_FileName);
  if (mode != ios_base::out)
  {
    m_FileStream.seekg(0, ios::end);
    m_FileSize = static_cast<uint64>(m_FileStream.tellg());
    m_FileStream.seekg(0, ios::beg);
  }
  SP_LOG(3, "Size of File " + m_FileName + cStringUtilities::MakeFormatted(" : %d bytes", m_FileSize));
  return true;
}

//  *******************************************************************************************************************
void cFileIO::cFileIOImpl::CloseFile()
{
  if (m_FileStream.is_open())
  {
    m_FileStream.close();
  }
}

//  *******************************************************************************************************************
bool cFileIO::cFileIOImpl::WriteLine(const cString& line)
{
  if (m_FileStream.good())
  {
    m_FileStream << line.GetData();
  }
  if (m_FileStream.good())
  {
    m_FileSize += line.GetLength();
    return true;
  }
  return false;
}

//  ********************************************************************************************************************
bool cFileIO::cFileIOImpl::Write(const char* pBuffer, uint64 size)
{
  if (m_FileStream.good())
  {
    m_FileStream.write(pBuffer, size);
  }
  if (m_FileStream.good())
  {
    m_FileSize += size;
    return true;
  }
  return false;
}

//  ********************************************************************************************************************
void cFileIO::cFileIOImpl::Flush()
{
  SP_ASSERT_ERROR(m_FileStream.good()).SetCustomMessage("File not in good state : " + m_FileName);
  if (m_FileStream.good())
  {
    m_FileStream.flush();
  }
}

//  ********************************************************************************************************************
bool cFileIO::cFileIOImpl::Read(char* const pBuffer, uint64 size)
{
  SP_ASSERT_ERROR(m_FileStream.good()).SetCustomMessage("File not in good state : " + m_FileName);

  if (!m_FileStream.good())
  {
    return false;
  }

  memset(pBuffer, 0, size);
  m_FileStream.read(pBuffer, size);
  if (m_FileStream.fail() && m_FileStream.eof())
  {
    return true;
  }
  SP_ASSERT_ERROR(m_FileStream.good()).SetCustomMessage("Error in reading file: " + m_FileName);

  return m_FileStream.good();
}

//  ********************************************************************************************************************
cString cFileIO::cFileIOImpl::ReadLine()
{
  string str;
  getline(m_FileStream, str);
  return str;
}

//  ********************************************************************************************************************
uint32 cFileIO::cFileIOImpl::GetCurrentInputPosition()
{
  return m_FileStream.tellg();
}

//  ********************************************************************************************************************
void cFileIO::cFileIOImpl::SetCurrentInputPosition(const int relativePosition, ios_base::seekdir dir)
{
  m_FileStream.seekg(relativePosition, dir);
}
