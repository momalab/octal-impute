#pragma once

#include "e3int.h"

namespace definitions
{ // begin of namespace
    const size_t SZ = 16;
    using Ciphertext = SecureInt<SZ>;
    size_t us=1000000;
} // end of namespace
