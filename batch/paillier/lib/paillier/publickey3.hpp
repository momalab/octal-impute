#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include "ciphertext.hpp"
#include "number.hpp"
#include "util.hpp"

using std::istream;
using std::ostream;
using std::string;
using std::stringstream;
using std::vector;

class SecretKey;

class PublicKey
{
    private:
        Number g, n; //, rN;
        vector<Number> rN;
        size_t nSeeds;
        friend class SecretKey;
        void init(const Number & g, const Number & n, size_t nSeeds);

        vector<Number> expTable, gnTable;
        Number exp(const Number &, const Number &, const Number &);

    public:
        Number n2;
        Ciphertext zero;
        size_t keySize;

        PublicKey() { }
        PublicKey(istream & in, size_t nSeeds=1);
        PublicKey(const Number & g, const Number & n, size_t nSeeds=1) { init(g, n, nSeeds); }
        PublicKey(const PublicKey & pk, size_t nSeeds=1) : PublicKey(pk.g, pk.n, nSeeds) { }

        inline operator string() const { return string(g)+" "+string(n); }
        inline PublicKey & operator=(const PublicKey & pk) { init(pk.g, pk.n, pk.nSeeds); return *this; }
        Ciphertext encrypt(const Number & m, size_t idx=0);
        void fillExpTable();
        Number getG() const { return g; }
        Number getN() const { return n; }
        Number getN2() const { return n2; }
        Number random(size_t idx=0);

        friend inline ostream & operator<<(ostream & os, const PublicKey & pk) { return os << string(pk); }
};

PublicKey::PublicKey(istream & in, size_t nSeeds)
{
    string s;
    in >> s; g = s;
    in >> s; n = s;
    init(g, n, nSeeds);
}

void PublicKey::init(const Number & g, const Number & n, size_t nSeeds)
{
    this->g = g;
    this->n = n;
    this->nSeeds = nSeeds;
    n2 = n*n;
    rN.resize(nSeeds);
    for ( size_t i=0; i<nSeeds; i++ )
    {
        do { rN[i] = Number::random(n); }
        while ( Number::gcd(rN[i], n) != 1 );
        rN[i].pow(n, n2);
    }
    // fillExpTable();
    // zero = encrypt(0);
    this->keySize = util::sizeCeilPowerOfTwo(n);
}

Ciphertext PublicKey::encrypt(const Number & m, const size_t idx)
{
    // auto r = random(idx);
    auto r = Number::random(n);
    auto gnr = Number::pow(g, n*r, n2);
    // auto gm = Number::pow(g, m, n2);
    auto gm = exp(g, m, n2);
    auto c = gm*gnr;
    c %= n2;
    return Ciphertext(c, n2, zero);
}

Number PublicKey::exp(const Number & b, const Number & x, const Number & mod)
{
    // std::cout << b << " ^ " << x << " % " << mod << " = " << std::flush;
    auto e = x;
    Number r = 1;
    size_t i = 0;
    while ( e > 0 )
    {
        auto bit = e;
        bit &= 1;
        if ( bit == 1 )
        {
            // std::cout << " ( " << expTable[i] << " ) ";
            r *= expTable[i];
            r %= mod;
        }
        e >>= 1;
        i++;
    }
    // std::cout << r << '\n';
    return r;
}

void PublicKey::fillExpTable()
{
    expTable.clear();
    expTable.push_back(g);
    for ( size_t i=1; i < keySize; i++ )
    {
        auto mul = expTable[i-1];
        mul *= mul;
        mul %= n2;
        expTable.push_back( mul );
    }
    zero = encrypt(0);
}

Number PublicKey::random(size_t idx)
{
    rN[idx] *= rN[idx];
    rN[idx] %= n2;
    return Number(rN[idx]);
}
