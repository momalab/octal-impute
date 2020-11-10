#include <fstream>
#include <iostream>
#include <vector>
#include "constants.hpp"
#include "ciphertext.hpp"
#include "publickey.hpp"
#include "timer.hpp"
#include "util.hpp"

using namespace std;
using namespace util;

int main(int argc, char * argv[])
{
    if (argc < 3)
    {
        cout << "Usage: encrytor filein fileout\n";
        return 1;
    }

    cout << "ENCRYPTION STARTED!\n";

    auto us = constants::us;
    cout << "Loading time (s): " << flush;
    Timer t1;
    auto plaintexts = readMatrix3DUnsigned(argv[1]);
    auto fileout = string( argv[2] );
    ifstream finPK(constants::filePublicKey);
    PublicKey pk(finPK);
    cout << (double(t1.get())/us) << "\n";

    cout << "Encryption time (s): " << flush;
    Timer t2;
    vector<vector<vector<Ciphertext>>> encQuery;
    for ( const auto & m : plaintexts )
    {
        vector<vector<Ciphertext>> em;
        for ( const auto & v : m )
        {
           vector<Ciphertext> ev;
           for ( auto i : v ) ev.push_back( pk.encrypt(i) );
           em.push_back(ev);
        }
        encQuery.push_back(em);
    }
    cout << (double(t2.get())/us) << "\n";

    cout << "Writing time (s): " << flush;
    Timer t3;
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
    cout << (double(t3.get())/us) << "\n";

    cout << "ENCRYPTION FINISHED!\n";
}
