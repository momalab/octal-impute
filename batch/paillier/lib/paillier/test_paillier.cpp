#include <iostream>
#include "number.hpp"
#include "publickey.hpp"
#include "secretkey.hpp"

using namespace std;

int main()
{
    cout << '\n';
    cout << "--------------------\n";
    cout << "| Testing Paillier |\n";
    cout << "--------------------\n";
    cout << '\n';

    size_t keysize = 1024;
    Number a = 10;
    Number b = 5;
    Number c = 3;

    cout << "Key generation .. " << flush;
    SecretKey sk(keysize);
    cout << "ok\n";

    cout << "Get public key .. " << flush;
    auto pk = sk.getPublicKey();
    cout << "ok\n";

    cout << "Fill exponentiation table .. " << flush;
    pk.fillExpTable();
    cout << "ok\n";

    cout << "Encrypt .. " << flush;
    auto ea = pk.encrypt(a);
    auto eb = pk.encrypt(b);
    auto ec = pk.encrypt(c);
    cout << "ok\n";

    cout << "Decrypt .. " << flush;
    auto da = sk.decrypt(ea);
    auto db = sk.decrypt(eb);
    auto dc = sk.decrypt(ec);
    cout << "ok\t" << flush;

    cout << da << " " << db << " " << dc << '\n';

    cout << "Add .. " << flush;
    auto eab = ea + eb;
    auto eac = ea + ec;
    auto ebc = eb + ec;

    auto dab = sk.decrypt(eab);
    auto dac = sk.decrypt(eac);
    auto dbc = sk.decrypt(ebc);
    cout << "ok\t" << flush;

    cout << dab << " " << dac << " " << dbc << '\n';

    cout << "Multiply .. " << flush;
    auto ea0 = ea * 0;
    auto eb1 = eb * 1;
    auto ec2 = ec * 2;

    auto da0 = sk.decrypt(ea0);
    auto db1 = sk.decrypt(eb1);
    auto dc2 = sk.decrypt(ec2);
    cout << "ok\t" << flush;

    cout << da0 << " " << db1 << " " << dc2 << '\n';
}
