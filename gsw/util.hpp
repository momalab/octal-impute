#pragma once

#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "definitions.hpp"
#include "math.hpp"

namespace util
{ // begin of namespace
    using namespace definitions;
    using namespace math;
    using std::ifstream;
    using std::string;
    using std::stringstream;
    using std::vector;

    vector<vector<Ciphertext>> readMatrixCiphertext(string filename)
    {
        vector<vector<Ciphertext>> m;
        ifstream fin(filename);
        string line;
        while ( getline(fin, line) )
        {
            stringstream ss(line);
            string buff;
            vector<Ciphertext> v;
            while (ss >> buff) v.push_back( real(Ciphertext(buff)) );
            m.push_back(v);
        }
        return m;
    }

    vector<vector<uint64_t>> readMatrixReal(string filename)
    {
        vector<vector<uint64_t>> m;
        ifstream fin(filename);
        string line;
        while ( getline(fin, line) )
        {
            stringstream ss(line);
            string buff;
            vector<uint64_t> v;
            while (ss >> buff) v.push_back( real<float>(stof(buff)) );
            m.push_back(v);
        }
        return m;
    }

} // end of namespace
