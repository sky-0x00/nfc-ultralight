#include <windows.h>
#include <cassert>
#include <iostream>
#include <sstream>
#include <fstream>
#include <map>
#include <iomanip>

#include "application.h"
#include "..\..\..\units\compile-time\build.h"
#include "..\..\..\units\nfc\mf0ulx1.h"

struct workmode {
	application::workmode id;
	struct name {
		char_t shrt;
		string_t full;
		bool check(_in const string_t &argv) const;
	};
	name name;
};
bool workmode::name::check(
	_in const string_t &argv
) const {
	{
		char_t name[] = L"-?";	name[1] = shrt;
		if (name == argv)
			return true;
	} {
		if ((L"--" + full) == argv)
			return true;
	}
	return false;
}


static const workmode workmode_s[] {
	{application::workmode::dump_create,	{L'c', L"dump-create"}},
	{application::workmode::dump_analyze,	{L'a', L"dump-analyze"}},
	{application::workmode::dump_restore,	{L'r', L"dump-restore"}}
};
static bool check_workmode(
	_in const string_t &argv, _out application::workmode &wm
) noexcept {
	for (const auto workmode: workmode_s)
		if (workmode.name.check(argv)) {
			wm = workmode.id;
			return true;
		}
	Winapi::SetLastError(ERROR_NOT_FOUND);
	return false;
}
static application::workmode check_workmode(
	_in const string_t &argv
) {
	application::workmode workmode(static_cast<application::workmode>(0));
	if (check_workmode(argv, workmode))
		return workmode;
	throw Winapi::GetLastError();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
string_t application::version::to_string(
) const {
	return (std::wostringstream() << major << L'.' << minor << L'.' << build_major << L'.' << build_minor).str();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
application::application(
) :
	m__workmode(static_cast<workmode>(0))
{}

//application::workmode application::get_workmode(
//) const {
//	assert(0 != static_cast<unsigned>(m__workmode));
//	return m__workmode;
//}

void application::get_info(
	_in cstr_t name, _in const version &version
) {
	std::wcout <<
		L"app-name:   " << name << std::endl <<
		L"version:    " << version.to_string() << std::endl <<
		L"build-time: " << build::to_string() << std::endl <<
		L"start-time: " << ::run().to_string() << std::endl;
}

bool application::parse_args(
	_in argc_t argc, _in argv_t argv_s[]
) noexcept {

#ifdef _DEBUG
	if (0 == argc) {
		m__workmode = workmode::debug;
		return true;
	}	
#endif

	if (argc < 1) {
		Winapi::SetLastError(ERROR_BAD_ARGUMENTS);
		return false;
	}
	if (!check_workmode(argv_s[0], m__workmode))
		return false;

	switch (m__workmode) {

		case workmode::dump_create:
			if (2 != argc) {
				Winapi::SetLastError(ERROR_BAD_ARGUMENTS);
				return false;
			}
			//const auto argv = argv_s[1];
			m__args = std::make_any<args__dump_create>(args__dump_create{argv_s[1]});
			return true;

		case workmode::dump_analyze:
			if (2 != argc) {
				Winapi::SetLastError(ERROR_BAD_ARGUMENTS);
				return false;
			}
			//const auto argv = argv_s[1];
			m__args = std::make_any<args__dump_analyze>(args__dump_analyze{argv_s[1]});
			return true;

		case workmode::dump_restore:
			if (2 != argc) {
				Winapi::SetLastError(ERROR_BAD_ARGUMENTS);
				return false;
			}
			//const auto argv = argv_s[1];
			m__args = std::make_any<args__dump_restore>(args__dump_restore{ argv_s[1] });
			return true;

	}

	Winapi::SetLastError(ERROR_INTERNAL_ERROR);
	return false;
}

bool application::run(
) {
	//assert(0 != static_cast<unsigned>(m__workmode));
	switch (m__workmode) {

#ifdef _DEBUG
		case workmode::debug:
			std::wcout << L"work-mode:  debug" << std::endl;
			return run__debug();
#endif

		case workmode::dump_create: 
			std::wcout << L"work-mode:  dump-create" << std::endl; 
			{			
				const auto &args = std::any_cast<const args__dump_create&>(m__args);
				std::wcout << L"    file-name: \"" << args.filename << L'\"' << std::endl << std::endl;
				return run__dump_create(args);
			}
			break;

		case workmode::dump_analyze:
			std::wcout << L"work-mode:  dump-analyze" << std::endl;
			{
				const auto &args = std::any_cast<const args__dump_analyze&>(m__args);
				std::wcout << L"    file-name: \"" << args.filename << L'\"' << std::endl << std::endl;
				return run__dump_analyze(args);
			}
			break;

		case workmode::dump_restore:
			std::wcout << L"work-mode:  dump-restore" << std::endl;
			{
				const auto &args = std::any_cast<const args__dump_restore&>(m__args);
				std::wcout << L"    file-name: \"" << args.filename << L'\"' << std::endl << std::endl;
				return run__dump_restore(args);
			}
			break;
	}
	assert(true);
	return false;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
namespace nfc {
	static string_t device_name(_in unsigned id = 0) {
		return /*L"ACS ACR122 "*/ L"ACS ACR122U PICC Interface " + std::to_wstring(id);
	}
	class block_info {
	public:
		static const unsigned pages_per_block = 4;		// ������ 4 �������� � ����� (4x4=16 ����)
		const unsigned count;							// ����� ����� ������ 5 ��� MF0UL11 (5x4=20 �������), 10 ��� MF0UL21 (10x4+1=41 ��������)
	
	protected:
		block_info(
			_in unsigned count
		) :
			count(count)
		{}
	public:
		block_info(
			_in scard_mfu::version mfu_version
		) :
			block_info(static_cast<unsigned>(mfu_version) * 5)
		{
			static_assert(1 == static_cast<unsigned>(scard_mfu::version::v1), "1 == v1");
			static_assert(2 == static_cast<unsigned>(scard_mfu::version::v2), "2 == v2");
		}
	
	};
}

/*static*/ bool application::run__dump_create(
	_in const args__dump_create &args
) {
	nfc::device::context context;
	nfc::device device(context);
	const auto &device_names = device.enum_all();

	std::wcout << "device, connecting...";
	const nfc::scard_mfu &scard = device.connect(nfc::device_name().c_str(), nfc::device::share_mode::exclusive);
	if (!scard.get_handle()) {
		std::wcout << L" error, #" << Winapi::GetLastError() << std::endl;
		return false;
	}
	
	std::wcout << " ok" << std::endl << L"file-out, open for writing...";
	std::ofstream fout(args.filename, std::ios_base::out|std::ios_base::binary);
	const auto is_fout = fout.is_open();
	if (!is_fout) {
		std::wcout << L" error, #" << Winapi::GetLastError() << std::endl;
		goto exit;
	} {
		std::wcout << " ok";
		if (ERROR_ALREADY_EXISTS == Winapi::GetLastError())
			std::wcout << L", re-write (already-exist)";
		std::wcout << std::endl << L"device, reading..." << std::endl;
		const nfc::block_info block_info(nfc::scard_mfu::version::v1);

		for (unsigned i = 0; i < block_info.count; ++i) {
			const auto page_begin = i * block_info.pages_per_block;
			std::wcout << "    block " << i << L", page " << page_begin << L'-' << page_begin + block_info.pages_per_block - 1 << L"...";
			nfc::data data;
			if (scard.command__read(page_begin, data)) {
				const auto size = data.size();
				fout.write(reinterpret_cast<const char*>(data.data()), size);
				std::wcout << L" ok, " << size << L" byte(s)";
			} else
				std::wcout << L" error, #" << Winapi::GetLastError();
			std::wcout << std::endl;
		}
		fout.close();

		//is_ok = scard.command__get_version(data_v);
		//is_ok = scard.read(0, data_r);
		//is_ok = scard.fast_read({0,8}, data_fr);
	}

exit:
	std::wcout << "device, disconnecting...";
	if (device.disconnect(scard.get_handle()))
		std::wcout << L" ok";
	else
		std::wcout << L" error, #" << Winapi::GetLastError();
	std::wcout << std::endl;

	return is_fout;
}

/*static*/ bool application::run__dump_restore(
	_in const args__dump_restore &args
) {
	nfc::device::context context;
	nfc::device device(context);
	const auto &device_names = device.enum_all();

	std::wcout << "device, connecting...";
	const nfc::scard_mfu &scard = device.connect(nfc::device_name().c_str(), nfc::device::share_mode::exclusive);
	if (!scard.get_handle()) {
		std::wcout << L" error, #" << Winapi::GetLastError() << std::endl;
		return false;
	}

	std::wcout << " ok" << std::endl << L"file-in, open for reading...";
	std::ifstream fin(args.filename, std::ios_base::in | std::ios_base::binary);
	const auto is_fin = fin.is_open();
	if (!is_fin) {
		std::wcout << L" error, #" << Winapi::GetLastError() << std::endl;
		goto exit;
	} {
		std::wcout << " ok" << std::endl << L"file-in, reading...";
		nfc::data data;
		fin.read();

		std::wcout << "device, writing..." << std::endl;
		const nfc::block_info block_info(nfc::scard_mfu::version::v1);

		for (unsigned i = 0; i < block_info.count; ++i) {
			std::wcout << "    block " << i << L"..." << std::endl;
			for (unsigned j = 0; j < block_info.pages_per_block; ++j) {
				const auto page = i * block_info.pages_per_block + j;
				std::wcout << "        page " << page << L"...";
				//fin.read(reinterpret_cast<const char*>(data.data()), size);
				//if (scard.command__read(page_begin, data))
				std::wcout << " ok" << std::endl;
			}
		}
		fin.close();

		//is_ok = scard.command__get_version(data_v);
		//is_ok = scard.read(0, data_r);
		//is_ok = scard.fast_read({0,8}, data_fr);
	}

exit:
	std::wcout << "device, disconnecting...";
	if (device.disconnect(scard.get_handle()))
		std::wcout << L" ok";
	else
		std::wcout << L" error, #" << Winapi::GetLastError();
	std::wcout << std::endl;

	return is_fin;
}

/*static*/ bool application::run__dump_analyze(
	_in const args__dump_analyze &args
) {
	std::wcout << L"open file for reading...";
	std::ifstream fin(args.filename, std::ios_base::in|std::ios_base::binary);
	if (!fin.is_open()) {
		std::wcout << L" error, #" << Winapi::GetLastError() << std::endl;
		return false;
	} 

	auto get_filesize = [](
		_in std::ifstream &fin
	) -> unsigned {
		fin.seekg(0, std::ios_base::end);
		const auto size = static_cast<unsigned>(fin.tellg());
		fin.seekg(0, std::ios_base::beg);
		return size;
	};
	const auto filesize = get_filesize(fin);
	std::wcout << L" ok, " << filesize << L" byte(s)" << std::endl;

	std::wcout << L"checking data format... ";
	{
		const nfc::block_info block_info(nfc::scard_mfu::version::v1);			
		if ((block_info.count * block_info.pages_per_block) != (filesize >> 2)) {
			std::wcout << L" error, file-size mismatch" << std::endl;
		}
		std::wcout << L" ok, " << block_info.count << L" block(s) = " << block_info.count * block_info.pages_per_block << L" page(s)" << std::endl;
	}	

	//std::wcout << L"reading... ";
	nfc::data buffer(filesize);
	fin.read(reinterpret_cast<char*>(buffer.data()), filesize);
	if (filesize != fin.gcount()) {
		//std::wcout << L" error, partial data readed: only " << fin.gcount() << L'/' << filesize << L" byte(s)" << std::endl;
		return false;
	}
	fin.close();

	std::wcout << L"analyzing..." << std::endl;
	const auto pc_buffer_dec = reinterpret_cast<nfc::scard_mfu::pc_data_dec>(buffer.data());
	std::wcout << L"    uid: " << std::hex << std::noshowbase << std::uppercase << std::setfill(L'0');
	for (const auto &byte: pc_buffer_dec->uid())
		std::wcout << std::setw(2) << byte;
	
	std::wcout << std::endl;
	return true;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
/*static*/ bool application::run__debug(
	/*_in const args__debug &args*/
) {
	nfc::device::context context;
	nfc::device device(context);
	const auto &device_names = device.enum_all();

	std::wcout << std::endl << "connecting...";
	const nfc::scard_mfu &scard = device.connect(nfc::device_name().c_str(), nfc::device::share_mode::exclusive);
	if (!scard.get_handle()) {
		std::wcout << L" error, #" << Winapi::GetLastError() << std::endl;
		return false;
	}
	std::wcout << " ok" << std::endl;

	nfc::data data;
	if (!scard.command__get_status(data))
		goto exit;
	if (!scard.command__antenna_switch(true))
		goto exit;
	if (!scard.command__antenna_switch(false))
		goto exit;
	//SCardControl(scard.get_handle(), IOCTL_SMARTCARD_SET_CARD_TYPE);

	// ...

	exit:
	std::wcout << "disconnecting...";
	if (device.disconnect(scard.get_handle()))
		std::wcout << L" ok";
	else
		std::wcout << L" error, #" << Winapi::GetLastError();
	std::wcout << std::endl;

	return true;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
