#ifndef AGGRENDER_H
#define AGGRENDER_H

#ifdef AGGRENDER_EXPORTS
#define AGGRENDER_API __declspec(dllexport)
#else
#define AGGRENDER_API __declspec(dllimport)
#endif

AGGRENDER_API void RegAggRender();

#define Use_AggRender RegAggRender

#endif//AGGRENDER_H