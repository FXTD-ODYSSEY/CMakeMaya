#ifndef DEFORMER_PLATFORM_H
#define DEFORMER_PLATFORM_H

#include <maya/MString.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/// For unity builds, this definition is used for easy grepping.
#define globalVar static
/// For non-unity builds, see above
#define localVar static

/// This is the prototype for the function that will be dynamically hotloaded.

#ifdef _WIN32
#define NOMINMAX
#include <windows.h>

globalVar const char *kDeformerLogicLibraryName = "logic.dll";
typedef HMODULE DLLHandle;
typedef FARPROC FuncPtr;

#elif __linux__ || __APPLE__
globalVar const char *kDeformerLogicLibraryName = "logic.so";

#endif // Library filename

enum LibraryStatus
{
	LibraryStatus_Failure = INT_MIN,
	LibraryStatus_InvalidLibrary,
	LibraryStatus_InvalidSymbol,
	LibraryStatus_InvalidHandle,
	LibraryStatus_UnloadFailure,
	LibraryStatus_Success = 0
};

/// This is a data structure that contains information about the state of a DLL
/// that contains all the so-called *business logic* required for the deformer
/// to do its work.
typedef size_t sizet;
typedef uint64_t FileTime;
typedef MVector (*DeformFunc)(MVector &, float);
struct DeformerLogicLibrary
{
	DLLHandle handle;
	FileTime lastModified;

	DeformFunc deformCB;
	bool isValid;
};

/// This is initialized to the path of the deformer's **business logic** DLL
/// whenever the plugin is initialized.
extern MString kPluginLogicLibraryPath;

/// This is the global reference to the *business logic* DLL that is loaded.
extern DeformerLogicLibrary kLogicLibrary;

static const char kWin32PathSeparator = '\\';
static const char kPathDelimiter = '\\';
static const unsigned int kMaxPathLen = MAX_PATH;

/**
 * This function gets the full path to the *business logic* DLL. This file may/may
 * not exist on disk yet at the time this path is formatted.
 *
 * @param pluginPath	The path to the host Maya plugin DLL. Must use the OS-specific
 * 					path separators.
 *
 * @return				The path to the *business logic* DLL.
 */

int convertPathSeparatorsToOSNative(char *filename);
FileTime getLastWriteTime(const char *filename);
MString getDeformerLogicLibraryPath(const char *pluginPath);

LibraryStatus loadDeformerLogicDLL(DeformerLogicLibrary &library);

LibraryStatus unloadDeformerLogicDLL(DeformerLogicLibrary &library);

#endif /* DEFORMER_PLATFORM_H */
