#pragma once

#include <string>

using std::string;

namespace constants
{
    const string fileSecretKey = "secret.key";
    const string filePublicKey = "public.key";
    const uint64_t margin = 64; // bits
    const uint64_t bitPrecision = 8;
    const uint64_t scale = 1<<bitPrecision; //10000; // bits
    const uint64_t us = 1000000;
}
