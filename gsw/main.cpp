#include <iostream>
#include "timer.hpp"
#include "util.hpp"

using namespace std;
using namespace util;

const SecureUint<2> bit = _2_Ep;
Ciphertext encrypt(uint64_t m)
{
    Ciphertext c;
    for ( size_t i=0; i<SZ; i++ ) c[i] = bit[ (m>>i)&1 ];
    return c;
}

int main(int argc, char * argv[])
{
    if (argc < 5)
    {
        cout << "Usage: bob encrypted_inputs weights biases fileout\n";
        return 1;
    }

    cout << "Loading time (s): " << flush;
    Timer t1;

    auto inputs = readMatrixCiphertext(argv[1]);
    auto weights = readMatrixReal(argv[2]);
    auto biases = readMatrixReal(argv[3]);
    auto fileout = string(argv[4]);
    cout << (double(t1.get())/us) << "\n";

    cout << "Encrypting biases (s): " << flush;
    Timer t2;
    vector<Ciphertext> ebiases;
    for ( const auto & b : biases ) ebiases.push_back( encrypt(b[0]) );
    cout << (double(t2.get())/us) << "\n";

    Ciphertext zero = _0_Ep;
    size_t qr = inputs.size();
    size_t qc = inputs[0].size();
    size_t wc = weights[0].size();

    // matrix multiplication Z=I.W
    cout << "Matrix multiplication (s):\n" << flush;
    Timer t3;
    vector<vector<Ciphertext>> m( qr, vector<Ciphertext>(wc, zero) );
    for ( size_t i=0; i<qr; i++ )
    {
        for ( size_t j=0; j<wc; j++ )
        {
            for ( size_t k=0; k<qc; k++ )
            {
                cout << i << " x " << j << " x " << k << "\n";
                m[i][j] += multiply( inputs[i][k], weights[k][j] );
            }
        }
    }
    cout << "Matrix multiplication (s): " << flush;
    cout << (double(t3.get())/us) << "\n";

    // matrix addition Z+=B
    cout << "Adding encrypted biases (s): " << flush;
    Timer t4;
    for ( size_t i=0; i<qr; i++ )
        for ( size_t j=0; j<wc; j++ ) m[i][j] += ebiases[j];
    cout << (double(t4.get())/us) << "\n";

    // e ^ z
    cout << "Exponentiation time (s): " << flush;
    Timer t5;
    // vector<Ciphertext> factorials = { _1_Ep, _1_Ep, _2_Ep, _6_Ep, _24_Ep, _120_Ep, _720_Ep };
    vector<Ciphertext> factorials = { _1_Ep, _1_Ep, _2_Ep, _6_Ep, _24_Ep };
    vector<Ciphertext> realFactorials;
    for ( const auto & f : factorials ) realFactorials.push_back( real(f) );
    for ( auto & v : m ) for ( auto & c : v ) c = taylor(c, realFactorials);
    cout << (double(t5.get())/us) << "\n";

    cout << "Writing time (s): " << flush;
    Timer t6;
    ofstream fout(fileout);
    for ( auto & v : m )
    {
        for ( auto & c : v ) fout << c << " ";
        fout << "\n";
    }
    cout << (double(t6.get())/us) << "\n";
}
