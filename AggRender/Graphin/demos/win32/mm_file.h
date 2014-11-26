//|
//|
//| mm_file.h
//|
//| Copyright (c) 2003
//| Andrew Fedoniouk - andrew@terrainformatica.com
//|
//| CONTENT:
//| class <B>mm_file</B> - access to memory mapped file
//| template table<RECORD> - persistent flat table
//|
//|

#ifndef __mm_file_h
#define __mm_file_h

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <assert.h>

namespace tool 
{

class mm_file 
{
    HANDLE hfile;
    HANDLE hmap;

  protected:
    bool read_only;
    void*  ptr;
    size_t length;
  public:
    mm_file(): hfile(0),hmap(0),ptr(0),length(0),read_only(true) {}
    virtual ~mm_file() { close(); }

    void *open(const char *path, bool to_write = false);
    void  close();

    void * data() { return ptr; }
    size_t size() { return length; }
    void   size(size_t sz) { assert(!read_only); length = sz; }
};





}



#endif