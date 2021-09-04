#include "EzyLogger.h"
#include <algorithm>
#include <cstdarg>
#include <string>
#include <cstring>

#if defined(_WIN32) || defined(WINRT)
#include "Windows.h"
#elif defined(ANDROID)
#include <android/log.h>
#endif

#ifdef EZY_DEBUG
#define MAX_LOG_LENGTH 16 * 1024 //16KB log

EZY_NAMESPACE_START_WITH_ONLY(logger)

static void _log(const char *format, va_list args) {
	char* buf = new char[MAX_LOG_LENGTH];

	vsnprintf(buf, MAX_LOG_LENGTH - 3, format, args);
	strcat(buf, "\n");

	console(buf);

	delete[] buf;
}

void console(const char* buf) {
#if defined(ANDROID)
	__android_log_print(ANDROID_LOG_DEBUG, "debug", "%s", buf);
#elif defined(_WIN32) || defined(WINRT)
	int pos = 0;
	int len = strlen(buf);
	int bufferSize = len > MAX_LOG_LENGTH ? MAX_LOG_LENGTH : len;
	char* tempBuf = new char[bufferSize + 1];
	WCHAR* wszBuf = new WCHAR[bufferSize + 1];

	while (pos < len)
	{
		std::copy(buf + pos, buf + pos + bufferSize, tempBuf);

		tempBuf[bufferSize] = 0;

		MultiByteToWideChar(CP_UTF8, 0, tempBuf, -1, wszBuf, (bufferSize + 1));
		OutputDebugStringW(wszBuf);
		WideCharToMultiByte(CP_ACP, 0, wszBuf, -1, tempBuf, (bufferSize + 1), nullptr, FALSE);
		printf("%s", tempBuf);

		pos += bufferSize;
		bufferSize = len - pos;
		if (bufferSize > MAX_LOG_LENGTH) {
			bufferSize = MAX_LOG_LENGTH;
		}
	}

	fflush(stdout);

	delete[] tempBuf;
	delete[] wszBuf;
#else
	fprintf(stdout, "%s", buf);
	fflush(stdout);
#endif
}

void log(const char * format, ...) {
	va_list args;
	va_start(args, format);
	_log(format, args);
	va_end(args);
}

void hex(const char* buf, int len) {
	char* data = new char[len * 3 + 10];

	for (int i = 0; i<len; ++i) {
		sprintf(&data[i * 3], "%02X ", (unsigned char)buf[i]);
	}

	log("%s", data);

	delete[]data;
}

EZY_NAMESPACE_END_WITH

#else

EZY_NAMESPACE_START_WITH_ONLY(logger)

void log(const char * format, ...) {
}

void console(const char* log) {
}

void hex(const char* buf, int len) {
}

EZY_NAMESPACE_END_WITH

#endif


