#pragma once

#ifdef FOOLIB_EXPORTS
#define FOOLIB_API __declspec(dllexport)
#else
#define FOOLIB_API __declspec(dllimport)
#endif

extern "C" FOOLIB_API void dofoo();
