#include <io.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <windows.h>
#include <libUUID/UUIDGenerator.h>
#include <libUUID/MD5Engine.h>

#pragma pack(push, 2)
typedef struct {
  WORD Reserved1;       // reserved, must be 0
  WORD ResourceType;    // type is 1 for icons
  WORD ImageCount;      // number of icons in structure (1)
  BYTE Width;           // icon width (32)
  BYTE Height;          // icon height (32)
  BYTE Colors;          // colors (0 means more than 8 bits per pixel)
  BYTE Reserved2;       // reserved, must be 0
  WORD Planes;          // color planes
  WORD BitsPerPixel;    // bit depth
  DWORD ImageSize;      // size of structure
  WORD ResourceID;      // resource ID
} GROUPICON;
#pragma pack(pop)

void InjectMainIcon(char *Where, char *What)
{
   HANDLE hWhere = BeginUpdateResource(Where, FALSE);

   char *buffer;    // buffer to store raw icon data
   long buffersize; // length of buffer
   int hFile;       // file handle
       
   hFile = open(What, O_RDONLY | O_BINARY);
   if (hFile == -1)
      return; // if file doesn't exist, can't be opened etc. 
       
   // calculate buffer length and load file into buffer
   buffersize = filelength(hFile);
   buffer = (char *)malloc(buffersize);
   read(hFile, buffer, buffersize);
   close(hFile);

   UpdateResource(
      hWhere,  // Handle to executable
      RT_ICON, // Resource type - icon
      MAKEINTRESOURCE(1), // Make the id 1
      0,
      (buffer+22), 
      // skip the first 22 bytes because this is the 
      // icon header&directory entry (if the file 
      // contains multiple images the directory entries
      // will be larger than 22 bytes
      buffersize-22  // length of buffer
     );


   // Again, we use this structure for educational purposes.
   // The icon header and directory entries can be read from 
   // the file.
   GROUPICON grData;

   // This is the header
   grData.Reserved1 = 0;     // reserved, must be 0
   grData.ResourceType = 1;  // type is 1 for icons
   grData.ImageCount = 1;    // number of icons in structure (1)

   // This is the directory entry
   grData.Width = 16;        // icon width (32)
   grData.Height = 16;       // icon height (32)
   grData.Colors = 0;        // colors (256)
   grData.Reserved2 = 0;     // reserved, must be 0
   grData.Planes = 2;        // color planes
   grData.BitsPerPixel = 32; // bit depth
   grData.ImageSize = buffersize - 22; // size of image
   grData.ResourceID = 1;       // resource ID is 1

   UpdateResource(
      hWhere,
      RT_GROUP_ICON,
      // RT_GROUP_ICON resources contain information
      // about stored icons
      MAKEINTRESOURCE(128),
      // MAINICON contains information about the
      // application's displayed icon
      0,
      &grData,
      // Pointer to this structure
      sizeof(GROUPICON)
     );

   delete buffer; // free memory
         
   // Perform the update, don't discard changes
   EndUpdateResource(hWhere, FALSE);
}


	#include <stdio.h>
	#include <windows.h>
	#include <tchar.h>
	#include <string>

#pragma pack(push,2)
    struct ICONDIRENTRY
    {
        BYTE bWidth;
        BYTE bHeight;
        BYTE bColorCount;
        BYTE bReserved;
        WORD wPlanes;
        WORD wBitCount;
        DWORD dwBytesInRes;
        DWORD dwImageOffset;
    };

    struct ICONDIR
    {
        WORD idReserved;
        WORD idType;
        WORD idCount;
        //ICONDIRENTRY idEntries;
    };

    struct GRPICONDIRENTRY
    {
        BYTE bWidth;
        BYTE bHeight;
        BYTE bColorCount;
        BYTE bReserved;
        WORD wPlanes;
        WORD wBitCount;
        DWORD dwBytesInRes;
        WORD nID;
    };
    struct GRPICONDIR
    {
        WORD idReserved;
        WORD idType;
        WORD idCount;
        GRPICONDIRENTRY idEntries;
    };
	#pragma pack(pop) 
    //////////////////////////////////////////////
    //函数说明：修改EXE图标
    //
    //参    数：IconFile 图标文件
    //              ExeFile 被修改的EXE文件
    //
    //返回值： 成功为True，否则False
    /////////////////////////////////////////////
    bool ChangeExeIcon(const std::string& IconFile, const std::string& ExeFile)
    {
        ICONDIR stID;
        ICONDIRENTRY stIDE;
        GRPICONDIR stGID;
        HANDLE hFile;
        DWORD nSize, nGSize, dwReserved;
        HANDLE hUpdate;
        PBYTE pIcon, pGrpIcon;
        BOOL ret;
        hFile = CreateFileA(IconFile.c_str(), GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        if (hFile == INVALID_HANDLE_VALUE)
        {
           return false;
        }
        ZeroMemory(&stID, sizeof(ICONDIR));
        ret = ReadFile(hFile, &stID, sizeof(ICONDIR), &dwReserved, NULL);
        ZeroMemory(&stIDE, sizeof(ICONDIRENTRY));
        ret = ReadFile(hFile, &stIDE, sizeof(ICONDIRENTRY), &dwReserved, NULL);
        nSize = stIDE.dwBytesInRes;
        pIcon = (PBYTE)malloc(nSize);
        SetFilePointer(hFile, stIDE.dwImageOffset, NULL, FILE_BEGIN);
        ret = ReadFile(hFile, (LPVOID)pIcon, nSize, &dwReserved, NULL);
        if (!ret)
        {
           CloseHandle(hFile);
           return false;
        }
        ZeroMemory(&stGID, sizeof(GRPICONDIR));
        stGID.idCount = stID.idCount;
        stGID.idReserved = 0;
        stGID.idType = 1;
        CopyMemory(&stGID.idEntries, &stIDE, 12);
        stGID.idEntries.nID = 0;
        nGSize = sizeof(GRPICONDIR);
		pGrpIcon = (PBYTE)malloc(nGSize);
		CopyMemory(pGrpIcon, &stGID, nGSize);

		stGID.idEntries.nID = 1;

        hUpdate = BeginUpdateResourceA(ExeFile.c_str(), false);
		ret = UpdateResourceA(hUpdate, RT_ICON, MAKEINTRESOURCE(stGID.idEntries.nID), 0, (LPVOID)pIcon, nSize);
		ret = UpdateResourceA(hUpdate, RT_GROUP_ICON, MAKEINTRESOURCE(128), 0, (LPVOID)pGrpIcon, nGSize);
		ret = EndUpdateResourceA(hUpdate, false);
        if (!ret)
        {
           CloseHandle(hFile);
           return false;
        }
        CloseHandle(hFile);
        return true;
    }

	char ss[] = {'1','2','3','4','5','6','7','8','9','8','8'};//{"__@key@__123456789__@key@__"};

void Md5File( void )
{
	_uuid_::MD5Engine	Md51;

	_string_type s21 = Md51.digestToFile("e:\\lysTest\\Code\\ProjectCode\\bulid\\x64\\NetControl\\NetControl.sln");
	

	_uuid_::MD5Engine	Md5;
	FILE*  file = fopen("e:\\lysTest\\Code\\ProjectCode\\bulid\\x64\\NetControl\\NetControl.sln","rb");
	if( file != NULL )
	{
		int nLength = 0;       //number of bytes read from the file  
		const int nBufferSize = 1024; //checksum the file in blocks of 1024 bytes  
		BYTE Buffer[nBufferSize];   //buffer for data read from the file  
		while ( (nLength = fread(Buffer, 1, nBufferSize, file) ) > 0 )  
		{
			Md5.update(Buffer, nLength);
		}

		fclose(file);
		_string_type s = _uuid_::DigestEngine::digestToHex(Md5.digest());
		int i = 0;
	}
}

void main(int argc, char * argv[])
{
	_uuid_::UUIDGenerator ger;
	while(true)
	{
		printf("uuid:%s\r\n",ger.create().toString().c_str());
	}

	//static char s[]={"__@key@__123456789__@key@__"};

	Md5File();

	/*ChangeExeIcon("E:\\lysTest\\Code\\NetControl\\SVN\\trunk\\build1064\\bin\\Debug\\SkinBox.ico",
		"E:\\lysTest\\Code\\NetControl\\SVN\\trunk\\build1064\\bin\\Release\\SampleUUID.exe");*/

   /*if (argc != 3)
   {
     printf("Usage: chicon [executable filename] [icon filename]");
     return;
   }*/

   /*InjectMainIcon("E:\\lysTest\\Code\\NetControl\\SVN\\trunk\\build1064\\bin\\Release\\SampleUUID.exe",
	   "E:\\lysTest\\Code\\NetControl\\SVN\\trunk\\build1064\\bin\\Release\\message01.ico");*/

   //InjectMainIcon(argv[1], argv[2]);
}
