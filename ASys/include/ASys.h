#ifndef ASYS_H
#define ASYS_H

#ifdef ASYS_EXPORTS
#define ASYS_API __declspec(dllexport)
#else
#define ASYS_API __declspec(dllimport)
#endif

#include "AFile.h"
#include "AMemFile.h"
#include "ADirectoryListor.h"

#endif//ASYS_H