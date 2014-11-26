#ifndef GDIRENDER_H
#define GDIRENDER_H

#ifdef GDIRENDER_EXPORTS
#define GDIRENDER_API __declspec(dllexport)
#else
#define GDIRENDER_API __declspec(dllimport)
#endif

GDIRENDER_API void RegGdiRender();

#define Use_GdiRender RegGdiRender

#endif//AGGRENDER_H