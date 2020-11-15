#include <fstream>
#include <iostream>
#include "constants.hpp"
#include "ciphertext.hpp"
#include "io.hpp"
#include "publickey.hpp"
#include "secretkey.hpp"
#include "timer.hpp"

using namespace io;
using namespace std;

int main(int argc, char * argv[])
{
    if ( argc < 2 )
    {
        cout << "Usage: keygen bitsize\n";
        return 1;
    }

    auto us = constants::us;
    auto keysize = stoul( argv[1] );
    cout << "GENERATING KEYS (key size: " << keysize << " bits)\n";

    cout << "Generation time (s): " << flush;
    Timer t1;
    SecretKey sk(keysize);
    auto pk = sk.getPublicKey();
    cout << (double(t1.get())/us) << "\n";

    pk.fillExpTable();
    Number number = 5;
    auto dnumber = sk.decrypt( pk.encrypt( number ) );
    cout << "CMP " << (number == dnumber) << " " << number << " " << dnumber << "\n";

    cout << "Writing time (s): " << flush;
    Timer t2;
    ofstream foutSK(constants::fileSecretKey);
    foutSK << sk;
    ofstream foutPK(constants::filePublicKey);
    foutPK << pk;
    cout << (double(t2.get())/us) << "\n";

    cout << "KEYS GENERATED!\n";
}
