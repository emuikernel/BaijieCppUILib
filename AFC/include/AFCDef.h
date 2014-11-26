#ifndef AFCDEF_H
#define AFCDEF_H

#ifdef AFC_EXPORTS
#define AFC_API __declspec(dllexport)
#else
#define AFC_API __declspec(dllimport)
#endif


#endif//AFCDEF_H