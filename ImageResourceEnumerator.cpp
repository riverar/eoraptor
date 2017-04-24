#include "common.h"
#include "ImageResourceEnumerator.h"

BOOL __stdcall OnResourceNameFoundProxy(HMODULE moduleHandle, PCWSTR pszType, PWSTR pszName, LONG_PTR lParam)
{
	return reinterpret_cast<eoraptor::ImageResourceEnumerator*>(lParam)->OnResourceNameFound(moduleHandle, pszType, pszName, lParam);
}

HRESULT eoraptor::ImageResourceEnumerator::EnumerateImageResourcesInFile(const std::wstring& path, const ImageResourceCallback& callback)
{
	_callback = callback;
	unique_module moduleHandle(LoadLibraryEx(path.c_str(), nullptr, LOAD_LIBRARY_AS_DATAFILE), FreeLibrary);
	if (moduleHandle.get() == INVALID_HANDLE_VALUE)
	{
		throw std::system_error(std::error_code(GetLastError(), std::system_category()));
	}
	EnumResourceNamesEx(moduleHandle.get(), L"IMAGE", OnResourceNameFoundProxy, reinterpret_cast<LONG_PTR>(this), RESOURCE_ENUM_LN, 0);
	return S_OK;
}

BOOL eoraptor::ImageResourceEnumerator::OnResourceNameFound(HMODULE moduleHandle, PCWSTR pszType, PWSTR lpszName, LONG_PTR lParam)
{
	auto resourceHandle = FindResource(moduleHandle, lpszName, pszType);
	if (resourceHandle == INVALID_HANDLE_VALUE)
	{
		throw std::system_error(std::error_code(GetLastError(), std::system_category()));
	}
	auto resourcePtr = LockResource(LoadResource(moduleHandle, resourceHandle));
	_callback(resourcePtr, SizeofResource(moduleHandle, resourceHandle), reinterpret_cast<long>(lpszName));
	FreeResource(resourceHandle);
	return TRUE; // Continue
}
