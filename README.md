# eoraptor
Simple tool to scan for wasteful PNG text chunks in PE files.

Keep in mind, the tool wasn't designed to measure exactly (i.e. it doesn't count infrastructure needed to host text chunks). Also, the tool hardcodes .dll, .mui, and .exe as supported file types so be sure to tweak [FileEnumerator.cpp](FileEnumerator.cpp#L27-L29) as needed.

# usage
eoraptor <path to start search, e.g. C:\Windows\System32>

# contributing
Compiled with Visual Studio 2017 on Windows 10 Creators Update.

Pull requests welcome!

# special thanks
* Lode Vandevenne for [LodePNG for C (ISO C90) and C++](http://lodev.org/lodepng/)
