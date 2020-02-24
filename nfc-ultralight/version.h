#pragma once

#include "..\..\..\units\macro-defs.h"

#define BUILD__NAME__A				"nfc-ultralight"
#define BUILD__NAME_FULL__A			BUILD__NAME__A ".exe"
#define BUILD__NAME__W				WIDE(BUILD__NAME__A)
#define BUILD__NAME_FULL__W			WIDE(BUILD__NAME_FULL__A)

#ifdef _UNICODE
	#define BUILD__NAME				BUILD__NAME__W
	#define BUILD__NAME_FULL		BUILD__NAME_FULL__W
#else
	#define BUILD__NAME				BUILD__NAME__A
	#define BUILD__NAME_FULL		BUILD__NAME_FULL__A
#endif

#define BUILD__VERSION_MAJOR		0
#define BUILD__VERSION_MINOR		0
#define BUILD__BUILD_MAJOR			__COUNTER__
#define BUILD__BUILD_MINOR			0

#define BUILD__VERSION__A			STR(BUILD__VERSION_MAJOR) "." STR(BUILD__VERSION_MINOR) "." STR(BUILD__BUILD_MAJOR) "." STR(BUILD__BUILD_MINOR)
#define BUILD__VERSION__W			STR(BUILD__VERSION_MAJOR) L"." STR(BUILD__VERSION_MINOR) L"." STR(BUILD__BUILD_MAJOR) L"." STR(BUILD__BUILD_MINOR)
#ifdef _UNICODE
	#define BUILD__VERSION			BUILD__VERSION__W
#else
	#define BUILD__VERSION			BUILD__VERSION__A
#endif


#define BUILD__DATE_YEAR			__DATE__[7] __DATE__[8] __DATE__[9] __DATE__[10]
#define BUILD__DATE_MONTH		(\
	((__DATE__[0] == L'J') && (__DATE__[1] == L'a') && (__DATE__[2] == L'n')) ? L"01" :		\
	((__DATE__[0] == L'F') && (__DATE__[1] == L'e') && (__DATE__[2] == L'b')) ? L"02" :		\
	((__DATE__[0] == L'M') && (__DATE__[1] == L'a') && (__DATE__[2] == L'r')) ? L"03" :		\
	((__DATE__[0] == L'A') && (__DATE__[1] == L'p') && (__DATE__[2] == L'r')) ? L"04" :		\
	((__DATE__[0] == L'M') && (__DATE__[1] == L'a') && (__DATE__[2] == L'y')) ? L"05" :		\
	((__DATE__[0] == L'J') && (__DATE__[1] == L'u') && (__DATE__[2] == L'n')) ? L"06" :		\
	((__DATE__[0] == L'J') && (__DATE__[1] == L'u') && (__DATE__[2] == L'l')) ? L"07" :		\
	((__DATE__[0] == L'A') && (__DATE__[1] == L'u') && (__DATE__[2] == L'g')) ? L"08" :		\
	((__DATE__[0] == L'S') && (__DATE__[1] == L'e') && (__DATE__[2] == L'p')) ? L"09" :		\
	((__DATE__[0] == L'O') && (__DATE__[1] == L'c') && (__DATE__[2] == L't')) ? L"10" :		\
	((__DATE__[0] == L'N') && (__DATE__[1] == L'o') && (__DATE__[2] == L'v')) ? L"11" :		\
	((__DATE__[0] == L'D') && (__DATE__[1] == L'e') && (__DATE__[2] == L'c')) ? L"12" :		\
	L"??")
#define BUILD__DATE_DAY				__DATE__[4] __DATE__[5]

#define BUILD__DATE					STR(BUILD__DATE_YEAR /*L"-" BUILD__DATE_MONTH L'-'*/ BUILD__DATE_DAY)
#define BUILD__TIME					__TIME__

#define BUILD__MSC					_MSC_VER
