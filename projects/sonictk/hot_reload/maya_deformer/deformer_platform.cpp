#include "deformer_platform.h"
#include <maya/MString.h>
#include <maya/MGlobal.h>
#include <iostream>

inline int stringReplace(const char *input,
						 char *output,
						 const char token,
						 const char replace,
						 unsigned int size)
{
	sizet len = strlen(input);
	if (len <= 0)
	{
		return 0;
	}
	int replaced = 0;
	unsigned int i = 0;
	for (; i < size && i < (len + 1) && input[i] != '\0'; ++i)
	{
		if (input[i] == token)
		{
			output[i] = replace;
			replaced++;
		}
		else
		{
			output[i] = input[i];
		}
	}
	output[i] = '\0';

	return replaced;
}

int convertPathSeparatorsToOSNative(char *filename)
{
	sizet len = strlen(filename);
	char *tmp = new char[len + 1];
	int replaced = stringReplace(filename,
								 tmp,
								 kWin32PathSeparator,
								 kPathDelimiter,
								 (unsigned int)len + 1);
	if (replaced <= 0)
	{
		return replaced;
	}

	strncpy(filename, tmp, len + 1);

	return replaced;
}

inline void OSPrintLastError()
{
	char errMsg[256];
	DWORD errCode = GetLastError();
	if (errCode == 0)
	{
		return;
	}
	FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
				   NULL,
				   errCode,
				   MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
				   (LPTSTR)errMsg,
				   sizeof(errMsg),
				   NULL);

	perror(errMsg);
}

inline DLLHandle loadSharedLibrary(const char *filename)
{
	DLLHandle libHandle = LoadLibrary((LPCTSTR)filename);
	if (!libHandle)
	{
		OSPrintLastError();
		return NULL;
	}

	return libHandle;
}

inline int unloadSharedLibrary(DLLHandle handle)
{
	if (!handle)
	{
		perror("The handle is not valid! Cannot unload!\n");
		return -1;
	}

	BOOL result = FreeLibrary(handle);
	if (result == 0)
	{
		OSPrintLastError();
		return -2;
	}

	return 0;
}

inline FuncPtr loadSymbolFromLibrary(DLLHandle handle, const char *symbol)
{
	FuncPtr symbolAddr = GetProcAddress(handle, (LPCSTR)symbol);
	if (!symbolAddr)
	{
		OSPrintLastError();
	}

	return symbolAddr;
}

FileTime getLastWriteTime(const char *filename)
{
	FileTime result = -1;

	FILETIME lastWriteTime;
	WIN32_FILE_ATTRIBUTE_DATA data;
	if (GetFileAttributesEx((LPCTSTR)filename, GetFileExInfoStandard, &data))
	{
		lastWriteTime = data.ftLastWriteTime;
	}
	else
	{
		OSPrintLastError();
		return result;
	}

	result = (FileTime)lastWriteTime.dwHighDateTime << sizeof(DWORD) | lastWriteTime.dwLowDateTime;

	return result;
}

MString getDeformerLogicLibraryPath(const char *pluginPath)
{
	if (strlen(pluginPath) <= 0)
	{
		return MString();
	}
	char pathDelimiter[2] = {kPathDelimiter, '\0'};
	MString delimiter(pathDelimiter);
	MString pluginPathStr(pluginPath);
	MString libFilename = pluginPathStr + delimiter + kDeformerLogicLibraryName;

	return libFilename;
}

LibraryStatus loadDeformerLogicDLL(DeformerLogicLibrary &library)
{
	const char *libFilenameC = kPluginLogicLibraryPath.asChar();
	MGlobal::displayInfo("kPluginLogicLibraryPath: " + kPluginLogicLibraryPath);

	FileTime lastModified = getLastWriteTime(libFilenameC);
	library.lastModified = lastModified;

	DLLHandle handle = loadSharedLibrary(libFilenameC);
	if (!handle)
	{
		MGlobal::displayError("Unable to load logic library!");
		library.handle = NULL;
		library.lastModified = {};
		library.isValid = false;

		return LibraryStatus_InvalidLibrary;
	}

	library.handle = handle;

	FuncPtr getValueFuncAddr = loadSymbolFromLibrary(handle, "getValue");
	if (!getValueFuncAddr)
	{
		MGlobal::displayError("Could not find symbols in library!");
		return LibraryStatus_InvalidSymbol;
	}

	library.deformCB = (DeformFunc)getValueFuncAddr;
	library.isValid = true;

	MGlobal::displayInfo("Loaded library from: " + kPluginLogicLibraryPath);

	return LibraryStatus_Success;
}

LibraryStatus unloadDeformerLogicDLL(DeformerLogicLibrary &library)
{
	if (!kLogicLibrary.isValid)
	{
		return LibraryStatus_InvalidHandle;
	}
	int unload = unloadSharedLibrary(kLogicLibrary.handle);
	if (unload != 0)
	{
		MGlobal::displayError("Unable to unload shared library!");
		return LibraryStatus_UnloadFailure;
	}

	library.deformCB = NULL;
	library.lastModified = {};
	library.isValid = false;
	MGlobal::displayInfo("unloadDeformerLogicDLL kLogicLibrary isValid: " + library.isValid ? "true" : "false");

	return LibraryStatus_Success;
}
