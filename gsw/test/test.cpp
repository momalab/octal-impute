#include <iostream>
#include "test_math.hpp"

using namespace math;
using namespace std;

int main()
{
    vector<uint64_t> n(1<<bitsize);
    for ( uint64_t i=0; i<n.size(); i++ ) n[i] = real(i);

    cout << "bitsize : " << bitsize  << "\n";
    cout << "decimals: " << decimals << "\n";

    cout << "\n";

    cout << "5 * 3 = 15\n";
    cout << "real: " <<         n[5]  << " * " <<         n[3]  << " = " <<         multiply(n[5], n[3])  << "\n";
    cout << "frac: " <<    frac(n[5]) << " * " <<    frac(n[3]) << " = " <<    frac(multiply(n[5], n[3])) << "\n";
    cout << "int : " << integer(n[5]) << " * " << integer(n[3]) << " = " << integer(multiply(n[5], n[3])) << "\n";

    cout << "\n";

    cout << "10 / 3 = 3.333...\n";
    cout << "real: " <<         n[10]  << " / " <<         n[3]  << " = " <<         divide(n[10], n[3])  << "\n";
    cout << "frac: " <<    frac(n[10]) << " / " <<    frac(n[3]) << " = " <<    frac(divide(n[10], n[3])) << "\n";
    cout << "int : " << integer(n[10]) << " / " << integer(n[3]) << " = " << integer(divide(n[10], n[3])) << "\n";

    cout << "\n";

    cout << "5! = 120\n";
    cout << "real: " <<         factorial(n[5])  << "\n";
    cout << "frac: " <<    frac(factorial(n[5])) << "\n";
    cout << "int : " << integer(factorial(n[5])) << "\n";

    cout << "\n";

    cout << "5 ^ 3 = 125\n";
    cout << "real: " <<         n[5]  << " ^ " << 3 << " = " <<         exponentiate(n[5], 3)  << "\n";
    cout << "frac: " <<    frac(n[5]) << " ^ " << 3 << " = " <<    frac(exponentiate(n[5], 3)) << "\n";
    cout << "int : " << integer(n[5]) << " ^ " << 3 << " = " << integer(exponentiate(n[5], 3)) << "\n";

    cout << "\n";

    cout << "5.2 * 1.45 = 7.54\n";
    uint64_t a = real<uint64_t>( float(5.2 ) );
    uint64_t b = real<uint64_t>( float(1.45) );
    cout << "real: " <<         a  << " * " <<         b  << " = " <<         multiply(a,b)  << "\n";
    cout << "frac: " <<    frac(a) << " * " <<    frac(b) << " = " <<    frac(multiply(a,b)) << "\n";
    cout << "int : " << integer(a) << " * " << integer(b) << " = " << integer(multiply(a,b)) << "\n";

    cout << "\n";

    cout << "0.77 ^ 7 = 0.160485233\n";
    a = real<uint64_t>( float(0.77) );
    b = 7;
    cout << "real: " <<         a  << " * " << b << " = " <<         exponentiate(a,b)  << "\n";
    cout << "frac: " <<    frac(a) << " * " << b << " = " <<    frac(exponentiate(a,b)) << "\n";
    cout << "int : " << integer(a) << " * " << b << " = " << integer(exponentiate(a,b)) << "\n";

    cout << "\n";

    cout << "-0.77 ^ 7 = -0.160485233\n";
    a = real<int64_t>( float(-0.77) );
    b = 7;
    cout << "real: " <<         a  << " * " << b << " = " <<         exponentiate(a,b)  << "\n";
    cout << "frac: " <<    frac(a) << " * " << b << " = " <<    frac(exponentiate(a,b)) << "\n";
    cout << "int : " << integer(a) << " * " << b << " = " << integer(exponentiate(a,b)) << "\n";

    cout << "\n";

    cout << "e ^ 3.2 = 24.532530197\n";
    a = real<uint64_t>( float(3.2) );
    cout << "real: e ^ " <<         a  << " = " <<         taylor(a)  << "\n";
    cout << "frac: e ^ " <<    frac(a) << " = " <<    frac(taylor(a)) << "\n";
    cout << "frac: e ^ " << integer(a) << " = " << integer(taylor(a)) << "\n";

    cout << "\n";

    cout << "e ^ (-1.2) = 0.301194212\n";
    a = real<int64_t>( float(-1.2) );
    cout << "real: e ^ " <<         a  << " = " <<         taylor(a)  << "\n";
    cout << "frac: e ^ " <<    frac(a) << " = " <<    frac(taylor(a)) << "\n";
    cout << "int : e ^ " << integer(a) << " = " << integer(taylor(a)) << "\n";
}
