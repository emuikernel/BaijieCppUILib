#ifndef SKIARENDER_H
#define SKIARENDER_H

#ifdef SKIARENDER_EXPORTS
#define SKIARENDER_API __declspec(dllexport)
#else
#define SKIARENDER_API __declspec(dllimport)
#endif

SKIARENDER_API void RegSkiaRender();

#define Use_SkiaRender RegSkiaRender

#endif//AGGRENDER_H