#include <fstream>
#include <iostream>
#include <thread>
#include <vector>
#include "constants.hpp"
#include "ciphertext.hpp"
#include "io.hpp"
#include "publickey.hpp"
#include "timer.hpp"
#include "util.hpp"

using namespace io;
using namespace std;
using namespace util;

vector<vector<vector<Ciphertext>>> encQuery;
vector<vector<vector<Number>>> packedQuery;
size_t nThreads = 1;
PublicKey pk;

void threadEncrypt(size_t init)
{
    for ( size_t idx=init; idx<packedQuery.size(); idx+=nThreads )
    {
        const auto & dm = packedQuery[idx];
        vector<vector<Ciphertext>> em;
        for ( const auto & dv : dm )
        {
            vector<Ciphertext> ev;
            for ( const auto & di : dv ) ev.push_back( pk.encrypt(di, init) );
            em.push_back(ev);
        }
        encQuery[idx] = em;
    }
}

int main(int argc, char * argv[])
{
    if (argc < 4)
    {
        cout << "Usage: encrytor filein fileout n_threads\n";
        return 1;
    }

    cout << "ENCRYPTION STARTED!\n";

    auto us = constants::us;
    cout << "Loading time (s): " << flush;
    auto timer = Timer();
    auto plaintexts = readMatrix3DUnsigned(argv[1]);
    auto fileout = string( argv[2] );
    nThreads = stoul(argv[3]);
    ifstream finPK(constants::filePublicKey);
    pk = PublicKey(finPK, nThreads);
    size_t keysize = pk.keySize;
    pk.fillExpTable();
    auto nTargets = plaintexts.size();
    auto nIndividuals = plaintexts[0].size();
    auto nInputs = plaintexts[0][0].size();
    cout << (double(timer.get())/us) << "\n";

    // cout << nTargets << "x" << nIndividuals << "x" << nInputs << '\n';

    cout << "Packing time (s): " << flush;
    timer = Timer();
    auto shift = sizeCeilPowerOfTwo(nInputs) + constants::bitPrecision;
    auto slots = keysize / shift;
    auto remainder = nIndividuals % slots;
    auto nCiphers = nIndividuals / slots + ( remainder > 0 );
    packedQuery = vector<vector<vector<Number>>> (
        nTargets, vector<vector<Number>>(
            nCiphers, vector<Number>(nInputs, 0)
        )
    );
    for ( size_t t=0; t < nTargets; t++ )
    {
        for ( int i=nIndividuals-1; i>=0; i-- )
        {
            auto c = i / slots;
            auto idx = c * slots;
            for ( size_t j=0; j<nInputs; j++ )
            {
                packedQuery[t][c][j] <<= shift;
                packedQuery[t][c][j]  += plaintexts[t][i][j];
            }
        }
    }
    cout << (double(timer.get())/us) << "\n";

    cout << "Encryption time (s): " << flush;
    timer = Timer();
    // vector<vector<vector<Ciphertext>>> encQuery;
    // for ( const auto & m : packedQuery )
    // {
    //     vector<vector<Ciphertext>> em;
    //     for ( const auto & v : m )
    //     {
    //         vector<Ciphertext> ev;
    //         for ( auto i : v ) ev.push_back( pk.encrypt(i) );
    //         em.push_back(ev);
    //     }
    //     encQuery.push_back(em);
    // }

    encQuery.resize(nTargets);
    vector<thread> threads(nThreads);
    for ( size_t q=0; q<nThreads; q++ ) threads[q] = thread( threadEncrypt, q );
    for ( size_t i=0; i<nThreads; i++ ) threads[i].join();
    cout << (double(timer.get())/us) << "\n";

    // {
    // cout << "Encryption time (s): " << flush;
    // Timer t2;
    // vector<vector<vector<Ciphertext>>> encQuery;
    // size_t i = 0;
    // for ( const auto & m : plaintexts )
    // {
    //     cout << i++ << " " << flush;
    //     vector<vector<Ciphertext>> em;
    //     for ( const auto & v : m )
    //     {
    //        vector<Ciphertext> ev;
    //        for ( auto i : v ) ev.push_back( pk.encrypt(i) );
    //        em.push_back(ev);
    //     }
    //     encQuery.push_back(em);
    // }
    // cout << (double(t2.get())/us) << "\n";

    cout << "Writing time (s): " << flush;
    timer = Timer();
    ofstream fout(fileout);
    for ( const auto & m : encQuery )
    {
        for ( const auto & v : m )
        {
            for ( const auto & i : v ) fout << i << " ";
            fout << "\n";
        }
        fout << "-\n";
    }
    cout << (double(timer.get())/us) << "\n";

    cout << "ENCRYPTION FINISHED!\n";
}
