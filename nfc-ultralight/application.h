#pragma once

#include "..\..\..\units\system-types.h"
#include "..\..\..\units\macro-defs.h"
#include "..\..\..\units\string.h"
#include <any>

class application {
public:
	enum class workmode {
#ifdef _DEBUG
		debug = 0,
#endif
		dump_create = 1,
		dump_analyze
	};
	struct version {
		unsigned major, minor, build_major, build_minor;
		string_t to_string() const;
	};
public:
	application();
public:
	bool parse_args(_in argc_t argc, _in argv_t argv_s[]) noexcept;
	void get_info(_in cstr_t name, _in const version &version);
	bool run();
protected:
//#ifdef _DEBUG
//	struct args__debug {
//	};
//#endif
	struct args__dump_create {
		cstr_t filename;
	};
	struct args__dump_analyze {
		cstr_t filename;
	};
protected:
#ifdef _DEBUG
	static bool run__debug(/*_in const args__debug &args*/);
#endif
	//workmode get_workmode() const noexcept;
	static bool run__dump_create(_in const args__dump_create &args);
	static bool run__dump_analyze(_in const args__dump_analyze &args);
private:
	workmode m__workmode;
	std::any m__args;
};
