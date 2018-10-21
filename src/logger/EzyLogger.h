#pragma once

#include "../EzyMacro.h"

EZY_NAMESPACE_START_WITH_ONLY(logger)

void log(const char * format, ...);
void console(const char* log);
void hex(const char* buf, int len);

EZY_NAMESPACE_END_WITH

