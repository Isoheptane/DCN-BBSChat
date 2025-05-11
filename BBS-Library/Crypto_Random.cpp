#include "pch.h"
#include "Crypto.h"


#define WIN32_NO_STATUS
#include <windows.h>
#undef WIN32_NO_STATUS

#include <winternl.h>
#include <ntstatus.h>
#include <winerror.h>
#include <stdio.h>
#include <bcrypt.h>
#pragma comment(lib, "bcrypt.lib")
#include <sal.h>

int Crypto::Random::generate(uint8_t* data, size_t length) {
    NTSTATUS Status = BCryptGenRandom(
        NULL,                       // Alg Handle pointer; NUll is passed as BCRYPT_USE_SYSTEM_PREFERRED_RNG flag is used
        data,                     // Address of the buffer that recieves the random number(s)
        length,                 // Size of the buffer in bytes
        BCRYPT_USE_SYSTEM_PREFERRED_RNG); // Flags                  

    if (!NT_SUCCESS(Status))
    {
        return -1;
    }
    return 0;
}