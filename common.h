#pragma once

#include "targetver.h"
#include <stdio.h>
#include <tchar.h>
#include <atlbase.h>
#include <atlstr.h>
#include <string>
#include <vector>
#include <functional>
#include <memory>

using unique_handle = std::unique_ptr<std::remove_pointer_t<HANDLE>, std::function<BOOL(HANDLE)>>;
using unique_module = std::unique_ptr<std::remove_pointer_t<HMODULE>, std::function<BOOL(HMODULE)>>;