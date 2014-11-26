//|
//|
//| mm_file.cpp
//|
//| Copyright (c) 2003
//| Andrew Fedoniouk - andrew@terrainformatica.com
//|
//|

#include <stdio.h>

#include "mm_file.h"

namespace tool 
{

void *mm_file::open(const char *path, bool to_write)
{
    read_only = !to_write;

    hfile = INVALID_HANDLE_VALUE;
    hmap = INVALID_HANDLE_VALUE;
    ptr = 0;
   
    hfile = CreateFile(path, GENERIC_READ | (read_only? 0: GENERIC_WRITE), FILE_SHARE_READ | (read_only? 0: FILE_SHARE_WRITE), NULL,
      read_only?OPEN_EXISTING:CREATE_ALWAYS, 
      FILE_ATTRIBUTE_NORMAL, NULL);
    if (hfile != INVALID_HANDLE_VALUE) 
    {
        length = GetFileSize(hfile, 0);
        hmap = CreateFileMapping(hfile, NULL, read_only? PAGE_READONLY : PAGE_READWRITE, 0, read_only?0:0x10000000, NULL);
    }
    else
    {
      DWORD erno = GetLastError();
      printf("ERROR: mm file open <%s> failed %x\n",path, erno);
      return 0;
    }
        
    if (hfile != INVALID_HANDLE_VALUE && hmap == NULL)
    {
        close();
        return 0;
    }
    else
    {
        if (hmap != NULL)
          ptr = MapViewOfFile(hmap, read_only? FILE_MAP_READ : FILE_MAP_WRITE, 0, 0, 0);
        if(ptr == 0)
        {
          DWORD erno = GetLastError();
          printf("ERROR: map file %x\n", erno);
          close();
        }
    }
    return ptr;
}

void mm_file::close()
{
  if (hfile && hmap && ptr) {

      if(!read_only && length)
        if (!FlushViewOfFile(ptr, length)) 
        { 
            printf("Could not flush memory to disk.\n"); 
        } 

      UnmapViewOfFile(ptr);
      ptr = 0;
  }
  if (hmap) {
      CloseHandle(hmap);
      hmap = 0;
  }
  if (hfile != INVALID_HANDLE_VALUE)
  {
      if(!read_only && length)
      {
        SetFilePointer(hfile,length,0,FILE_BEGIN);
        SetEndOfFile(hfile);
      }
      CloseHandle(hfile);
      hfile = 0;
  }
}

};