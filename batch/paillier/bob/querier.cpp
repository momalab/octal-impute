#include <fstream>
#include <iostream>
#include <thread>
#include <vector>
#include "ciphertext.hpp"
#include "constants.hpp"
#include "io.hpp"
#include "publickey.hpp"
#include "timer.hpp"
#include "util.hpp"

using namespace io;
using namespace std;
using namespace util;

PublicKey pk;
vector<vector<vector<Ciphertext>>> tagSNPs;
vector<vector<vector<Ciphertext>>> output;
vector<vector<uint64_t>> weights;
size_t nThreads = 1;
size_t nTargets;
size_t nIndividuals;
size_t nInputs;
size_t nOutputs;
vector<vector<Ciphertext>> packedOutput;
size_t shift;
size_t multiplier;
size_t nItems;

void threadQuery(size_t init)
{
    for ( size_t t=init; t<nTargets; t+=nThreads )
    {
        size_t offset = t * nInputs;
        vector<vector<Ciphertext>> mult(nIndividuals, vector<Ciphertext>(nOutputs, pk.zero ) );
        for ( size_t p=0; p<nIndividuals; p++ )
        {
            for ( size_t q=0; q<nOutputs; q++ )
            {
                for ( size_t r=0; r<nInputs; r++ )
                {
                    size_t ro = offset + r;
                    if ( weights[ro][q] ) mult[p][q] += tagSNPs[t][p][r] * weights[ro][q];
                }
            }
        }
        output[t] = mult;
    }
}

// void threadPack(size_t init)
// {
//     for ( size_t idx=init; idx<nTargets; idx+=nThreads )
//     {
//         const auto & mult = output[idx];
//         vector<Ciphertext> pack;
//         size_t counter = 0;
//         for ( const auto & v : mult )
//         {
//             for ( const auto & i : v )
//             {
//                 if ( !counter ) pack.push_back(i);
//                 else pack.back() = pack.back() * multiplier + i;
//                 counter = (counter+1) % nItems;
//             }
//         }
//         packedOutput[idx] = pack;
//     }
// }

int main(int argc, char * argv[])
{
    if (argc < 6)
    {
        cout << "Usage: querier encrypted_query weights fileout n_threads keysize\n";
        return 1;
    }

    cout << "QUERY STARTED!\n";

    auto us = constants::us;
    cout << "Loading time (s): " << flush;
    Timer t1;
    ifstream finPK(constants::filePublicKey);
    pk = PublicKey(finPK);
    pk.fillExpTable();
    tagSNPs = readMatrix3DCiphertext(argv[1], pk);
    weights = readMatrixFloat2Unsigned(argv[2], constants::scale);
    auto fileout = string(argv[3]);
    nThreads = stoul(argv[4]);
    size_t keysize = stoul(argv[5]);
    nTargets = tagSNPs.size();
    nIndividuals = tagSNPs[0].size();
    nInputs = tagSNPs[0][0].size();
    nOutputs = weights[0].size();
    cout << (double(t1.get())/us) << "\n";
    // cout << "nTargets    : " << nTargets     << '\n';
    // cout << "nIndividuals: " << nIndividuals << '\n';
    // cout << "nInputs     : " << nInputs      << '\n';
    // cout << "nOutputs    : " << nOutputs     << '\n';

    cout << "Multiplication time (s): " << flush;
    auto timer = Timer();
    output.resize( nTargets );
    vector<thread> threads(nThreads);
    for ( size_t q=0; q<nThreads; q++ ) threads[q] = thread( threadQuery, q );
    for ( size_t i=0; i<nThreads; i++ ) threads[i].join();
    cout << (double(timer.get())/us) << "\n";

    // cout << "Packing time (s): " << flush;
    // Timer t3;
    // shift = sizeCeilPowerOfTwo( constants::scale * nInputs / 3 );
    // multiplier = 1 << shift;
    // nItems = ( keysize - constants::margin ) / shift;
    // packedOutput.resize( nTargets );
    // vector<thread> threadsPack(nThreads);
    // for ( size_t q=0; q<nThreads; q++ ) threadsPack[q] = thread( threadPack, q );
    // for ( size_t i=0; i<nThreads; i++ ) threadsPack[i].join();
    // cout << (double(t3.get())/us) << "\n";

    // cout << "Writing time (s): " << flush;
    // Timer t4;
    // ofstream fout(fileout);
    // for ( const auto & pack : packedOutput )
    // {
    //     for ( const auto & c : pack ) fout << c << " ";
    //     fout << "\n";
    // }
    // cout << (double(t4.get())/us) << "\n";

    cout << "Writing time (s): " << flush;
    timer = Timer();
    ofstream fout(fileout);
    for ( const auto & m : output )
    {
        for ( const auto & v : m )
        {
            for ( const auto & c : v ) fout << c << " ";
            fout << "\n";
        }
        fout << "-\n";
    }
    cout << (double(timer.get())/us) << "\n";

    cout << "QUERY FINISHED!\n";
}
