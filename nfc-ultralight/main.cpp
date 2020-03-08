// nfc-ultralight.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "version.h"
#include "..\..\..\units\system-types.h"
#include "..\..\..\units\compile-time\build.h"
#include "..\..\..\units\nfc\mf0ulx1.h"

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

	nfc::device::context context;
	nfc::device device(context);
	const auto &device_names = device.enum_all();
	const nfc::scard_mfu &scard = device.connect(nfc::device_name().c_str(), nfc::device::share_mode::exclusive);
	
	nfc::data data_r, data_fr, data_v;
	bool is_ok = false;

	is_ok = scard.command__get_version(data_v);
	//is_ok = scard.read(0, data_r);
	//is_ok = scard.fast_read({0,8}, data_fr);

	is_ok = device.disconnect(scard.get_handle());
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
