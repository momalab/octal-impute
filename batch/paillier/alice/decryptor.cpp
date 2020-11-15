#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <thread>
#include <vector>
#include "ciphertext.hpp"
#include "constants.hpp"
#include "io.hpp"
#include "secretkey.hpp"
#include "timer.hpp"
#include "util.hpp"

using namespace io;
using namespace std;
using namespace util;

SecretKey sk;
// vector<vector<Number>> decPacked;
// vector<vector<Ciphertext>> encProbs;
vector<vector<vector<Ciphertext>>> encProbs;
vector<vector<vector<Number>>> decPacked;
size_t nThreads = 1;

void threadDecrypt(size_t init)
{
    for ( size_t idx=init; idx<encProbs.size(); idx+=nThreads )
    {
        const auto & em = encProbs[idx];
        vector<vector<Number>> m;
        for ( const auto & ev : em )
        {
            vector<Number> v;
            for ( const auto & ei : ev ) v.push_back( sk.decrypt(ei) );
            m.push_back(v);
        }
        decPacked[idx] = m;
    }
}

// void threadDecrypt(size_t init)
// {
//     for ( size_t idx=init; idx<encProbs.size(); idx+=nThreads )
//     {
//         const auto & ev = encProbs[idx];
//         vector<Number> v;
//         for ( const auto & ei : ev ) v.push_back( sk.decrypt(ei) );
//         decPacked[idx] = v;
//     }
// }

// vector<Number> unpack(vector<Number> & packed, size_t shift, size_t max, size_t mask)
// {
//     vector<Number> unpacked;
//     for ( auto & i : packed )
//     {
//         vector<Number> v;
//         for ( size_t idx=0; idx<max; idx++ )
//         {
//             v.push_back(mask & i);
//             i >>= shift;
//         }
//         reverse(v.begin(), v.end());
//         unpacked.insert( unpacked.end(), v.begin(), v.end() );
//     }
//     return unpacked;
// }

int main( int argc, char * argv[] )
{
    if ( argc < 7 )
    {
        cout << "Usage: decryptor filein fileout n_threads keysize n_individuals n_inputs n_outputs\n";
        return 1;
    }

    cout << "DECRYPTION STARTED!\n";

    auto us = constants::us;
    cout << "Loading time (s): " << flush;
    Timer t1;
    ifstream finSK(constants::fileSecretKey);
    sk = SecretKey(finSK);
    auto pk = sk.getPublicKey();
    // encProbs = readMatrixCiphertext(argv[1], pk);
    encProbs = readMatrix3DCiphertext(argv[1], pk);
    auto fileout = string(argv[2]);
    nThreads = stoul(argv[3]);
    size_t nTargets = encProbs.size();
    size_t keysize = stoul(argv[4]);
    size_t nIndividuals = stoul(argv[5]);
    size_t nInputs = stoul(argv[6]);
    size_t nOutputs = stoul(argv[7]);
    cout << (double(t1.get())/us) << "\n";

    // cout << "Decryption time (s): " << flush;
    // Timer t2;
    // decPacked.resize(nTargets);
    // vector<thread> threads(nThreads);
    // for ( size_t q=0; q<nThreads; q++ ) threads[q] = thread( threadDecrypt, q );
    // for ( size_t i=0; i<nThreads; i++ ) threads[i].join();
    // cout << (double(t2.get())/us) << "\n";

    cout << "Decryption time (s): " << flush;
    auto timer = Timer();
    decPacked.resize(nTargets);
    vector<thread> threads(nThreads);
    for ( size_t q=0; q<nThreads; q++ ) threads[q] = thread( threadDecrypt, q );
    for ( size_t i=0; i<nThreads; i++ ) threads[i].join();
    cout << (double(timer.get())/us) << "\n";

    // cout << "Unpacking time (s): " << flush;
    // Timer t3;
    // auto shift = sizeCeilPowerOfTwo( constants::scale * nInputs / 3 );
    // auto multiplier = 1 << shift;
    // auto max = ( keysize - constants::margin ) / shift;
    // auto mask = multiplier-1;
    // vector<vector<vector<Number>>> decProbs;
    // for ( auto & packed : decPacked )
    // {
    //     auto unpacked = unpack(packed, shift, max, mask);
    //     vector<vector<Number>> m;
    //     for ( size_t i=0, idx=0; i<nIndividuals; i++ )
    //     {
    //         vector<Number> v;
    //         for ( size_t j=0; j<nOutputs; j++ ) v.push_back( unpacked[idx++] );
    //         m.push_back(v);
    //     }
    //     decProbs.push_back(m);
    // }
    // cout << (double(t3.get())/us) << "\n";

    cout << "Unpacking time (s): " << flush;
    timer = Timer();
    auto shift = sizeCeilPowerOfTwo(nInputs) + constants::bitPrecision;
    auto mask = (1 << shift) - 1;
    auto slots = keysize / shift;
    vector<vector<vector<Number>>> decProbs (
        nTargets, vector<vector<Number>>(
            nIndividuals, vector<Number>(nOutputs, 0)
        )
    );
    for ( size_t t=0; t < nTargets; t++ )
    {
        for ( size_t i=0; i<nIndividuals; i++ )
        {
            auto c = i / slots;
            for ( size_t j=0; j<nOutputs; j++ )
            {
                decProbs[t][i][j] = decPacked[t][c][j];
                decProbs[t][i][j] &= mask;
                decPacked[t][c][j] >>= shift;
            }
        }
    }
    cout << (double(timer.get())/us) << "\n";

    cout << "Writing time (s): " << flush;
    Timer t4;
    ofstream fout(fileout);
    for ( const auto & m : decProbs )
    {
        for ( const auto & v : m )
        {
            double normalizer = 0;
            for ( const auto & i : v ) normalizer += unsigned(i);
            for ( const auto & i : v ) fout << fixed << setprecision(3) << (unsigned(i)/normalizer) << " ";
            // for ( const auto & i : v ) fout << i << " ";
            fout << "\n";
        }
        fout << "-\n";
    }
    cout << (double(t4.get())/us) << "\n";

    cout << "DECRYPTION FINISHED!\n";
}
