#pragma once

//mac
#if defined(EZY_TARGET_OS_MAC) || defined(__APPLE__)
#endif

// iphone
#if defined(EZY_TARGET_OS_IPHONE)

#endif

// android
#if defined(ANDROID)

#endif

// win32
#if defined(_WIN32) && defined(_WINDOWS)
#define USE_WINSOCK_2
#endif

// linux
#if defined(LINUX) && !defined(__APPLE__)

#endif

// marmalade
#if defined(MARMALADE)

#endif

// bada
#if defined(SHP)

#endif

// qnx
#if defined(__QNX__)

#endif

// native client
#if defined(__native_client__)

#endif

// Emscripten
#if defined(EMSCRIPTEN)

#endif

// tizen
#if defined(TIZEN)

#endif

// qt5
#if defined(CC_TARGET_QT5)

#endif

// WinRT (Windows 8.1 Store/Phone App)
#if defined(WINRT)
#define USE_WINSOCK_2
#endif


#ifdef USE_WINSOCK_2
#pragma comment(lib, "ws2_32.lib")
#include <winsock2.h>
#include <ws2tcpip.h>
//#include <wspiapi.h>
#include <Ws2tcpip.h>
#define SYS_SOCKET_INVALID INVALID_SOCKET
#else 
#include <netdb.h>
#include <signal.h>
#include <unistd.h>
#include <netinet/in.h>
#define SYS_SOCKET_INVALID -1
#endif

#include <vector>
#include <mutex>
#include <functional>
#include <errno.h>
#include <cstdlib>
#include <cstring>

