// nfc-ultralight.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "version.h"
#include "..\..\..\units\system-types.h"
#include "..\..\..\units\compile-time\build.h"
#include "..\..\..\units\nfc\scard.h"

namespace nfc {
	static string_t device_name(_in unsigned id = 0) {
		return L"ACS ACR122 " + std::to_wstring(id);
	}
}

int wmain(
	_in argc_t argc, _in argv_t argv
) {
	std::wcout << L"Module: " BUILD__NAME_FULL << std::endl <<
		L"Version: " << BUILD__VERSION << std::endl <<
		L"Build: " << build::date() << L" " << build::time() << std::endl;

	nfc::scard::context context;
	nfc::scard::device device(context);
	const auto &device_names = device.enum_all();
	const auto &scard = device.connect(nfc::device_name().c_str(), nfc::scard::device::share_mode::direct);
	const auto is_ok = device.disconnect(scard.handle);
	return 0;
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
