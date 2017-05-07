#include "common.h"
#include "FileEnumerator.h"
#include "ImageResourceEnumerator.h"
#include "lodepng\lodepng.h"
#include "lodepng\lodepng_util.h"

int wmain(int argc, wchar_t *argv[], wchar_t *envp[])
{
	size_t totalBytesWasted = 0;
	eoraptor::FileEnumerator fileEnumerator;
	eoraptor::ImageResourceEnumerator imageEnumerator;

	wprintf(L"eoraptor - Scan for wasteful PNG text chunks in PE files\n");

	try
	{
		fileEnumerator.EnumerateFilesAtPath(argv[1], [&imageEnumerator, &totalBytesWasted](const std::wstring& filePath, const std::system_error* error) {
			wprintf(L"%s\n", filePath.c_str());

			if (error != NULL) {
				printf("Error: %s", error->what());
				return;
			}

			size_t moduleBytesWasted = 0;
			imageEnumerator.EnumerateImageResourcesInFile(filePath, [&filePath, &moduleBytesWasted, &totalBytesWasted](const void* resourcePtr, size_t size, long resourceId) {
				auto imagePtr = reinterpret_cast<const unsigned char*>(resourcePtr);
				std::vector<unsigned char> encodedImage(imagePtr, imagePtr + size);
				std::vector<unsigned char> decodedImage;

				unsigned int width;
				unsigned int height;
				lodepng::State state;
				if (lodepng::decode(decodedImage, width, height, state, encodedImage.data(), size) == 0)
				{
					if ((state.info_png.text_num > 0) || (state.info_png.itext_num > 0))
					{
						size_t bytesWasted = 0;
						for (auto i = 0u; i < state.info_png.text_num; i++)
						{
							bytesWasted += std::string(state.info_png.text_strings[i]).length();
							bytesWasted += std::string(state.info_png.text_keys[i]).length();
						}
						for (auto i = 0u; i < state.info_png.itext_num; i++)
						{
							bytesWasted += std::string(state.info_png.itext_strings[i]).length();
							bytesWasted += std::string(state.info_png.itext_keys[i]).length();
						}

						totalBytesWasted += bytesWasted;
						moduleBytesWasted += bytesWasted;

						wprintf(L" - Resource: %d\n", resourceId);
						wprintf(L" - Wasted Bytes: %d\n\n", bytesWasted);
					}
				}
			});

			wprintf(L" - Total: %d\n\n", moduleBytesWasted);
		});

		wprintf(L"Total bytes wasted: %d\n", totalBytesWasted);
	}
	catch (std::system_error& e)
	{
		printf("Error: %s", e.what());
	}

	return 0;
}