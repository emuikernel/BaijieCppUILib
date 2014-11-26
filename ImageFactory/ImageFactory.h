#ifndef IMAGEFACTORY_H
#define IMAGEFACTORY_H

#ifdef IMAGEFACTORY_EXPORTS
#define IMAGEFACTORY_API __declspec(dllexport)
#else
#define IMAGEFACTORY_API __declspec(dllimport)
#endif

IMAGEFACTORY_API void RegImageFactory();

#define Use_ImageFactory RegImageFactory

#endif//IMAGEFACTORY_H