#pragma once

namespace eoraptor
{
	using FileEnumeratorCallback = std::function<void(std::wstring filePath, std::system_error* error)>;

	class FileEnumerator {
	public:
		void EnumerateFilesAtPath(const std::wstring& path, const eoraptor::FileEnumeratorCallback& action);
	};
}
