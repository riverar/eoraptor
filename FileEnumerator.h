#pragma once

namespace eoraptor
{
	using FileEnumeratorCallback = std::function<void(std::wstring filePath)>;

	class FileEnumerator {
	public:
		void EnumerateFilesAtPath(const std::wstring& path, const eoraptor::FileEnumeratorCallback& action);
	};
}
