#include "uri.h"
#include <iostream>
#include <string>
#include <locale>

#ifdef _WIN32
#include <windows.h>
#include <wincon.h>
#endif

int main(int argc, char* argv[]) {
#ifdef _WIN32
	SetConsoleOutputCP(CP_UTF8);
#else
	std::locale::global(std::locale(""));
#endif

	if (argc != 2) {
		std::cerr << "Usage: " << argv[0] << " <string_to_check>" << std::endl;
		return 1;
	}

	std::string urlString = argv[1];
	Uri uri(urlString);
	std::cout << "The URL is " << (uri.IsBrowsable() ? "browsable" : "NOT browsable") << std::endl;

	return 0;
}