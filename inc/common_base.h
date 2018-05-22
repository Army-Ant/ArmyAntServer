#ifndef COMMON_BASE_H_20180522
#define COMMON_BASE_H_20180522

#ifdef _WIN32

#ifndef AAS_USE_STATIC

#if defined ARMYANTSERVER_EXPORTS
#define ARMYANTSERVER_API __declspec(dllexport)
#else
#define ARMYANTSERVER_API __declspec(dllimport)
#endif

#else // AAS_USE_STATIC -> true

#define ARMYANTSERVER_API

#endif // AAS_USE_STATIC

#else // _WIN32 -> _UNIX

#define ARMYANTSERVER_API

#endif // _WIN32

#endif // COMMON_BASE_H_20180522