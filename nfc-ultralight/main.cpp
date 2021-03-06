// nfc-ultralight.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <windows.h>
#include "application.h"
#include "..\..\..\units\system-types.h"

result_t wmain(
	_in argc_t argc, _in argv_t argv_s[]
) {
	try {
		application application;
		application.get_info(L"nfc-ultralight", {0, 0, 0, 0});

		if (!application.parse_args(--argc, ++argv_s))
			return Winapi::GetLastError();
		if (!application.run())
			return Winapi::GetLastError();

		return SCARD_S_SUCCESS;
	}
	catch (...) {
		return -1;
	}
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
