#pragma once

#ifdef BARLIB_EXPORTS
#define BARLIB_API __declspec(dllexport)
#else
#define BARLIB_API __declspec(dllimport)
#endif

extern "C" BARLIB_API void dobar();
