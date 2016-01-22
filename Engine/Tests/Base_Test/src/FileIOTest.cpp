#include "stdafx.h"
#include "FileIO.h"

using namespace Base;
using namespace std;

class FileIOTest : public ::testing::Test
{
protected:
  void SetUp() {}
  void TearDown() {}
};

TEST_F(FileIOTest, CreateFileIO)
{
  unique_ptr<cFileIO> pFile(cFileIO::CreateFileIO());
  EXPECT_TRUE(pFile != NULL);
}

TEST_F(FileIOTest, OpenSuccess)
{
  unique_ptr<cFileIO> pFile(cFileIO::CreateFileIO());
  pFile->OpenFile("test_ascii.txt", ios::out);
  EXPECT_STREQ("test_ascii.txt", pFile->GetFileName().GetData());
}

TEST_F(FileIOTest, OpenFail)
{
  unique_ptr<cFileIO> pFile(cFileIO::CreateFileIO());
  bool result = pFile->OpenFile("test_ascii_not_exist.txt", ios::in);
  EXPECT_FALSE(result);
}

TEST_F(FileIOTest, WriteLineFailOnFileModeInput)
{
  unique_ptr<cFileIO> pAsciiFile(cFileIO::CreateFileIO());
  pAsciiFile->OpenFile("test_ascii.txt", ios::in);
  bool result = pAsciiFile->WriteLine("Testing WriteLineFailOnFileModeInput.\n");
  pAsciiFile->CloseFile();
  EXPECT_FALSE(result);
}

TEST_F(FileIOTest, ReadLineFailOnFileModeOutput)
{
  unique_ptr<cFileIO> pAsciiFile(cFileIO::CreateFileIO());
  pAsciiFile->OpenFile("test_ascii.txt", ios::out);
  pAsciiFile->WriteLine("Testing ReadLineFailOnFileModeOutput\n");
  pAsciiFile->CloseFile();

  pAsciiFile->OpenFile("test_ascii.txt", ios::app);
  EXPECT_STREQ("", pAsciiFile->ReadLine().GetData());
  pAsciiFile->CloseFile();
}

TEST_F(FileIOTest, WriteLineAndReadLineSuccess)
{
  unique_ptr<cFileIO> pAsciiFile(cFileIO::CreateFileIO());
  pAsciiFile->OpenFile("test_ascii.txt", ios::out);
  bool result = pAsciiFile->WriteLine("Testing WriteLineAndReadLineSuccess\n");
  pAsciiFile->CloseFile();

  EXPECT_TRUE(result);

  pAsciiFile->OpenFile("test_ascii.txt", ios::in);
  EXPECT_STREQ("Testing WriteLineAndReadLineSuccess", pAsciiFile->ReadLine().GetData());
  pAsciiFile->CloseFile();
  EXPECT_LT(unsigned long(0), pAsciiFile->GetFileSize());
}

TEST_F(FileIOTest, WriteFailOnFileModeInput)
{
  unique_ptr<cFileIO> pAsciiFile(cFileIO::CreateFileIO());
  pAsciiFile->OpenFile("test_ascii.txt", ios::in);
  cString testString = "Testing WriteLineAndReadLineSuccess\n";
  bool result = pAsciiFile->Write(testString.GetData(), testString.GetLength());
  pAsciiFile->CloseFile();
  EXPECT_FALSE(result);
}

TEST_F(FileIOTest, ReadAllFailOnFileModeOutput)
{
  unique_ptr<cFileIO> pAsciiFile(cFileIO::CreateFileIO());
  pAsciiFile->OpenFile("test_ascii.txt", ios::out);
  pAsciiFile->WriteLine("Testing ReadAllFailOnFileModeOutput\n");
  pAsciiFile->CloseFile();

  pAsciiFile->OpenFile("test_ascii.txt", ios::app);
  EXPECT_STREQ("", pAsciiFile->ReadLine().GetData());
  pAsciiFile->CloseFile();
}

TEST_F(FileIOTest, WriteAndReadAllAsciiSuccess)
{
  unique_ptr<cFileIO> pAsciiFile(cFileIO::CreateFileIO());

  pAsciiFile->OpenFile("test_ascii.txt", ios::out);
  cString testString = "Testing WriteAndReadAllAsciiSuccess\n";
  bool result = pAsciiFile->Write(testString.GetData(), testString.GetLength());
  pAsciiFile->CloseFile();
  EXPECT_TRUE(result);

  pAsciiFile->OpenFile("test_ascii.txt", ios::in);
  char* pBuffer = new char[pAsciiFile->GetFileSize()];
  result = pAsciiFile->Read(pBuffer, pAsciiFile->GetFileSize());
  pAsciiFile->CloseFile();
  EXPECT_LT(unsigned long(0), pAsciiFile->GetFileSize());
  EXPECT_TRUE(result);
  SafeDeleteArray(&pBuffer);
}

TEST_F(FileIOTest, WriteAndReadSuccessBinary)
{
  struct rec
  {
    int x, y, z;
  };

  unique_ptr<cFileIO> pBinaryFile(cFileIO::CreateFileIO());
  pBinaryFile->OpenFile("test_binary.bin", ios::out | ios::binary);

  struct rec my_record;
  my_record.x = 1;
  my_record.y = 2;
  my_record.z = 3;
  bool result = pBinaryFile->Write(reinterpret_cast<const char*>(&my_record), sizeof(struct rec));

  EXPECT_TRUE(result);
  pBinaryFile->CloseFile();

  pBinaryFile->OpenFile("test_binary.bin", ios::in | ios::binary);
  char* pBuffer = new char[sizeof(struct rec)];
  result = pBinaryFile->Read(pBuffer, sizeof(struct rec));
  const struct rec* pRec = reinterpret_cast<const struct rec*>(pBuffer);
  pBinaryFile->CloseFile();

  EXPECT_TRUE(result);
  EXPECT_LT(unsigned long(0), pBinaryFile->GetFileSize());
  EXPECT_EQ(1, pRec->x);
  EXPECT_EQ(2, pRec->y);
  EXPECT_EQ(3, pRec->z);
  SafeDeleteArray(&pBuffer);
}

TEST_F(FileIOTest, WriteBinaryOpenAscii)
{
  struct rec
  {
    int x, y, z;
  };

  unique_ptr<cFileIO> pBinaryFile(cFileIO::CreateFileIO());
  pBinaryFile->OpenFile("test_binary.bin", ios::out | ios::binary);

  struct rec my_record;
  my_record.x = 1;
  my_record.y = 2;
  my_record.z = 3;
  bool result = pBinaryFile->Write(reinterpret_cast<const char*>(&my_record), sizeof(struct rec));

  EXPECT_TRUE(result);
  pBinaryFile->CloseFile();

  pBinaryFile->OpenFile("test_binary.bin", ios::in);
  char* pBuffer = new char[pBinaryFile->GetFileSize()];
  result = pBinaryFile->Read(pBuffer, pBinaryFile->GetFileSize());
  const struct rec* pRec = reinterpret_cast<const struct rec*>(pBuffer);
  pBinaryFile->CloseFile();

  EXPECT_TRUE(result);
  EXPECT_LT(unsigned long(0), pBinaryFile->GetFileSize());
  EXPECT_EQ(1, pRec->x);
  EXPECT_EQ(2, pRec->y);
  EXPECT_EQ(3, pRec->z);
  SafeDeleteArray(&pBuffer);
}

TEST_F(FileIOTest, WriteAsciiReadBinary)
{
  unique_ptr<cFileIO> pAsciiFile(cFileIO::CreateFileIO());

  pAsciiFile->OpenFile("test_ascii.txt", ios::out);
  cString testString = "Testing WriteAndReadAllAsciiSuccess\n";
  bool result = pAsciiFile->Write(testString.GetData(), testString.GetLength());
  pAsciiFile->CloseFile();
  EXPECT_TRUE(result);

  pAsciiFile->OpenFile("test_ascii.txt", ios::in | ios::binary);
  char* pBuffer = new char[pAsciiFile->GetFileSize()];
  result = pAsciiFile->Read(pBuffer, pAsciiFile->GetFileSize());
  pAsciiFile->CloseFile();
  EXPECT_LT(unsigned long(0), pAsciiFile->GetFileSize());
  EXPECT_TRUE(result);
  EXPECT_STRNE("Testing WriteAndReadAllAsciiSuccess\n", pBuffer);
  SafeDeleteArray(&pBuffer);
}

TEST_F(FileIOTest, WriteAndReadMoreThanFileSize)
{
  struct rec
  {
    int x, y, z;
  };

  unique_ptr<cFileIO> pBinaryFile(cFileIO::CreateFileIO());
  pBinaryFile->OpenFile("test_binary.bin", ios::out | ios::binary);

  struct rec my_record;
  my_record.x = 1;
  my_record.y = 2;
  my_record.z = 3;
  bool result = pBinaryFile->Write(reinterpret_cast<const char*>(&my_record), sizeof(struct rec));

  EXPECT_TRUE(result);
  pBinaryFile->CloseFile();

  pBinaryFile->OpenFile("test_binary.bin", ios::in | ios::binary);
  char* pBuffer = new char[100];
  result = pBinaryFile->Read(pBuffer, 100);
  const struct rec* pRec = reinterpret_cast<const struct rec*>(pBuffer);
  pBinaryFile->CloseFile();

  EXPECT_TRUE(result);
  EXPECT_LT(unsigned long(0), pBinaryFile->GetFileSize());
  EXPECT_EQ(1, pRec->x);
  EXPECT_EQ(2, pRec->y);
  EXPECT_EQ(3, pRec->z);
  EXPECT_TRUE(pBinaryFile->IsEOF());
  SafeDeleteArray(&pBuffer);
}

TEST_F(FileIOTest, WriteAndReadLessThanFileSize)
{
  struct rec
  {
    int x, y, z;
  };

  unique_ptr<cFileIO> pBinaryFile(cFileIO::CreateFileIO());
  pBinaryFile->OpenFile("test_binary.bin", ios::out | ios::binary);

  struct rec my_record;
  my_record.x = 1;
  my_record.y = 2;
  my_record.z = 3;
  bool result = pBinaryFile->Write(reinterpret_cast<const char*>(&my_record), sizeof(struct rec));

  EXPECT_TRUE(result);
  pBinaryFile->CloseFile();

  pBinaryFile->OpenFile("test_binary.bin", ios::in | ios::binary);
  char* pBuffer = new char[sizeof(int)];
  result = pBinaryFile->Read(pBuffer, sizeof(int));
  const int* pX = reinterpret_cast<const int*>(pBuffer);
  pBinaryFile->CloseFile();

  EXPECT_TRUE(result);
  EXPECT_LT(unsigned long(0), pBinaryFile->GetFileSize());
  EXPECT_EQ(1, *pX);
  EXPECT_FALSE(pBinaryFile->IsEOF());
  SafeDeleteArray(&pBuffer);
}