#include "jwt-cpp/jwt.h"

#ifndef CPP_LEARN_MY_DLL_H
#define CPP_LEARN_MY_DLL_H

#ifdef WIN32
#define EXPORT_DLL __declspec(dllexport)
#else
#define EXPORT_DLL
#endif
extern "C" {
	EXPORT_DLL char* verifyToken(const char* token, const char* publicKey);

	EXPORT_DLL char* decodeTokenToJson(const char* token);

	EXPORT_DLL int add(int a, int b);
}

#endif
