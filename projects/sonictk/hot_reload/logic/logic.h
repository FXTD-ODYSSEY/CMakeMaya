#ifdef __cplusplus

#define Shared extern "C"
#define Import extern "C"

#endif // __cplusplus


/// DLL machinery types
#ifdef _WIN32
// #include <Windows.h>

#define DLLExport __declspec(dllexport)
#define DLLImport __declspec(dllimport)

// typedef HMODULE DLLHandle;
// typedef FARPROC FuncPtr;

#elif __linux__ || __APPLE__

// NOTE: (sonictk) This will only work on GCC/Clang
#define DLLExport __attribute__ ((visibility ("default")))
#define DLLImport __attribute__ ((visibility ("default")))

typedef void * DLLHandle;
typedef void * FuncPtr;

#endif /* Platform layer for DLL machinery */


#include <maya/MPoint.h>

Shared
{
    DLLExport MVector getValue(MVector &v, float factor);
}

