#include "common.h"
#include "FileEnumerator.h"

void eoraptor::FileEnumerator::EnumerateFilesAtPath(const std::wstring& path, const eoraptor::FileEnumeratorCallback& callback)
{
	WIN32_FIND_DATA findData = {};
	unique_handle findHandle(FindFirstFile((path + L"\\*").c_str(), &findData), FindClose);
	
	if (findHandle.get() == INVALID_HANDLE_VALUE)
	{
		std::system_error& err = std::system_error(std::error_code(GetLastError(), std::system_category()));
		const wchar_t* err_path = path.c_str();
		size_t len = wcslen(err_path) * 2 + 2;
		size_t c_len;
		char* errmsg = new char[len];
		wcstombs_s(&c_len, errmsg, len, err_path, len);
		printf("%s: %s\n\n", errmsg, err.what());
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
