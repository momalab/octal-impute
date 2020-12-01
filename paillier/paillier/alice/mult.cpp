#include <fstream>
#include <iostream>
#include <vector>
#include "ciphertext.hpp"
#include "constants.hpp"
#include "publickey.hpp"
#include "util.hpp"

using namespace std;
using namespace util;

int main(int argc, char * argv[])
{
    ifstream finPK(constants::filePublicKey);
    PublicKey pk(finPK);

    auto tagSNPs = readMatrix3DCiphertext(argv[1], pk);
    auto fileout = string(argv[2]);
    vector<vector<uint64_t>> weights = { {2,1,1},{1,2,1},{1,1,2} };

    size_t nTargets = tagSNPs.size();
    size_t nIndividuals = tagSNPs[0].size();
    size_t nInputs = tagSNPs[0][0].size();
    size_t nOutputs = weights[0].size();

    vector<vector<vector<Ciphertext>>> output;
    for ( size_t t=0; t<nTargets; t++ )
    {
        size_t offset = 0; //t * nInputs;
        vector<vector<Ciphertext>> mult(nIndividuals, vector<Ciphertext>(nOutputs, pk.encrypt(0) ) );
        for ( size_t p=0; p<nIndividuals; p++ )
        {
            for ( size_t q=0; q<nOutputs; q++ )
            {
                for ( size_t r=0; r<nInputs; r++ )
                {
                    size_t ro = offset + r;
                    if ( weights[ro][q] ) mult[p][q] += tagSNPs[t][p][ro] * weights[ro][q];
                }
            }
        }
        output.push_back(mult);
    }

    ofstream fout(fileout);
    for ( const auto & mult : output )
    {
        for ( const auto & v : mult )
        {
            for ( const auto & i : v )
            {
                fout << i << " ";
            }
            fout << "\n";
        }
        fout << "-\n";
    }
}
