#pragma once

namespace eoraptor
{
	class ImageResourceEnumerator {
	public:
		using ImageResourceCallback = std::function<void(const void* resourcePtr, size_t size, long resourceId)>;
		HRESULT EnumerateImageResourcesInFile(const std::wstring& path, const ImageResourceCallback& callback);
		BOOL OnResourceNameFound(HMODULE moduleHandle, PCWSTR lpszType, PWSTR lpszName, LONG_PTR lParam);
	private:
		ImageResourceCallback _callback;
	};
}