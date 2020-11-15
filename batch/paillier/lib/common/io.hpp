#pragma once

#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include "ciphertext.hpp"
#include "publickey.hpp"

using std::ifstream;
using std::string;
using std::stringstream;
using std::vector;

namespace io
{ // begin of namespace

vector<vector<vector<Ciphertext>>> readMatrix3DCiphertext(string filename, PublicKey & pk)
{
    vector<vector<vector<Ciphertext>>> mout;
    vector<vector<Ciphertext>> m;
    ifstream fin(filename);
    string line;
    while ( getline(fin, line) )
    {
        if (line == "-")
        {
            mout.push_back(m);
            m = vector<vector<Ciphertext>>();
        }
        else
        {
            vector<Ciphertext> v;
            stringstream ss(line);
            string buff;
            while ( ss >> buff ) v.push_back( Ciphertext(buff, pk.n2, pk.zero) );
            m.push_back(v);
        }
    }
    return mout;
}

vector<vector<vector<uint64_t>>> readMatrix3DUnsigned(string filename)
{
    vector<vector<vector<uint64_t>>> mout;
    vector<vector<uint64_t>> m;
    ifstream fin(filename);
    string line;
    while ( getline(fin, line) )
    {
        if (line == "-")
        {
            mout.push_back(m);
            m = vector<vector<uint64_t>>();
        }
        else
        {
            vector<uint64_t> v;
            stringstream ss(line);
            string buff;
            while ( ss >> buff ) v.push_back( stoul(buff) );
            m.push_back(v);
        }
    }
    return mout;
}

vector<vector<Ciphertext>> readMatrixCiphertext(string filename, PublicKey & pk)
{
    vector<vector<Ciphertext>> m;
    ifstream fin(filename);
    string line;
    while ( getline(fin, line) )
    {
        vector<Ciphertext> v;
        stringstream ss(line);
        string buff;
        while ( ss >> buff ) v.push_back( Ciphertext(buff, pk.n2, pk.zero) );
        m.push_back(v);
    }
    return m;
}

vector<vector<uint64_t>> readMatrixFloat2Unsigned(string filename, uint64_t scale)
{
    vector<vector<uint64_t>> m;
    ifstream fin(filename);
    string line;
    while ( getline(fin, line) )
    {
        vector<uint64_t> v;
        stringstream ss(line);
        string buff;
        while ( ss >> buff ) v.push_back( uint64_t(stof(buff)*scale) );
        m.push_back(v);
    }
    return m;
}

vector<vector<uint64_t>> readMatrixUnsigned(string filename)
{
    vector<vector<uint64_t>> m;
    ifstream fin(filename);
    string line;
    while ( getline(fin, line) )
    {
        vector<uint64_t> v;
        stringstream ss(line);
        string buff;
        while ( ss >> buff ) v.push_back( stoul(buff) );
        m.push_back(v);
    }
    return m;
}

} // end of namespace
