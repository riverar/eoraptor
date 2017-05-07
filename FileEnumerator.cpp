#include "common.h"
#include "FileEnumerator.h"

void eoraptor::FileEnumerator::EnumerateFilesAtPath(const std::wstring& path, const eoraptor::FileEnumeratorCallback& callback)
{
	WIN32_FIND_DATA findData = {};
	unique_handle findHandle(FindFirstFile((path + L"\\*").c_str(), &findData), FindClose);
	
	if (findHandle.get() == INVALID_HANDLE_VALUE)
	{
		std::system_error& e = std::system_error(std::error_code(GetLastError(), std::system_category()));
		printf("%s: %s\n\n", path.c_str, e.what());
		return;
	}

	do
	{
		auto fileName = std::wstring(findData.cFileName);
		if ((fileName != L".") && (fileName != L".."))
		{
			if (!(findData.dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT))
			{
				if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				{
					eoraptor::FileEnumerator::EnumerateFilesAtPath(path + L"\\" + findData.cFileName, callback);
				}
				else
				{
					if ((fileName.find(L".dll") != std::wstring::npos) ||
						(fileName.find(L".exe") != std::wstring::npos) ||
						(fileName.find(L".mui") != std::wstring::npos))
					{
						wchar_t combinedPath[MAX_PATH] = {};
						PathCombine(combinedPath, path.c_str(), findData.cFileName);
						callback(std::wstring(combinedPath));
					}
				}
			}
		}
	} while (FindNextFile(findHandle.get(), &findData));
}
