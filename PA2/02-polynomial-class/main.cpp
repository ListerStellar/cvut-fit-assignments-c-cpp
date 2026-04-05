#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <climits>
#include <cmath>
#include <cfloat>
#include <cassert>
#include <unistd.h>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <span>
#include <algorithm>
#include <memory>
#include <compare>
#include <complex>
#endif /* __PROGTEST__ */

// keep this dummy version if you do not implement a real manipulator
std::ios_base & ( * poly_var ( const std::string & name ) ) ( std::ios_base & x ) {
    return [] ( std::ios_base & ios ) -> std::ios_base & { return ios; };
}

class CPolynomial {
public:
    // default constructor
    CPolynomial() {
        coeff.push_back(0);
    }
    friend std::ostream &operator<<(std::ostream &stream, const CPolynomial &a) {
        if (a.coeff.size() == 0 || !bool(a)) {
            stream << "0";
            return stream;
        }
        bool isNotFirst = false;
        for (int i = a.coeff.size()-1; i >= 0; i--) {
            if (a.coeff[i] != 0) {
                if (a.coeff[i] > 0)
                    if (i == 0)
                        stream << (isNotFirst ? " + " : "") << a.coeff[i];
                    else
                        if (a.coeff[i] == 1)
                            stream << (isNotFirst ? " + " : "") << "" << (a.coeff[i] == 1 ? "" : "*") << "x^" << i;
                        else
                            stream << (isNotFirst ? " + " : "") << a.coeff[i] << (a.coeff[i] == 1 ? "" : "*") << "x^" << i;
                else
                    if (i == 0)
                        stream << " - " << std::abs(a.coeff[i]);
                    else
                        if (a.coeff[i] == -1)
                            stream << (isNotFirst ? " - " : "- ") << "" << (a.coeff[i] == -1 ? "" : "*") << "x^" << i;
                        else
                            stream << (isNotFirst ? " - " : "- ") << std::abs(a.coeff[i]) << (a.coeff[i] == -1 ? "" : "*") << "x^" << i;
                isNotFirst = true;
            }
        }
        return stream;
        // stream << a.coeff[0] << " " << a.coeff[1] << " " << a.coeff[2] << " " << a.coeff[3];
        // return stream;
    }
    // operator * (polynomial, double)
    friend CPolynomial operator*(const CPolynomial &a, const CPolynomial &b) {
        CPolynomial result;
        result.coeff = std::vector<double>(a.coeff.size() + b.coeff.size() - 1, 0);
        for (size_t i = 0; i < a.coeff.size(); i++) {
            for (size_t j = 0; j < b.coeff.size(); j++) {
                result.coeff[i + j] += a.coeff[i] * b.coeff[j];
            }
        }
        return result;
    }
    friend CPolynomial operator*(const CPolynomial &a, const double b) {
        CPolynomial result;
        result.coeff = std::vector<double>(a.coeff.size(), 0);
        for (size_t i = 0; i < a.coeff.size(); i++) {
            result.coeff[i] = a.coeff[i] * b;
        }
        return result;
    }
    // operator *= (polynomial, double)
    CPolynomial &operator*=(const CPolynomial &a) {
        return *this = *this * a;
    }
    CPolynomial &operator*=(const double a) {
        return *this = *this * a;
    }
    // operator ==
    bool operator==(const CPolynomial &a) const {
        size_t cmpSize = std::min(a.coeff.size(), coeff.size());
        for (size_t i = 0; i < cmpSize; i++) {
            if (coeff[i] != a.coeff[i]) return false;
        }
        if (cmpSize != a.coeff.size()) {
            for (size_t i = cmpSize; i < a.coeff.size(); i++) {
                if (a.coeff[i] != 0) return false;
            }
        }
        if (cmpSize != coeff.size()) {
            for (size_t i = cmpSize; i < coeff.size(); i++) {
                if (coeff[i] != 0) return false;
            }
        }
        return true;
    }
    // operator !=
    bool operator!=(const CPolynomial &a) const {
        return !(*this == a);
    }
    // operator []
    double &operator[](size_t i) {
        if (i >= coeff.size()) {
            coeff.resize(i+1, 0);
        };
        return coeff[i];
    }
    const double &operator[](size_t i) const {
        return coeff[i];
    }
    // operator ()
    double operator()(double x) const {
        double result = 0, xPow = 1;
        for (size_t i = 0; i < coeff.size(); i++) {
            result += coeff[i] * xPow;
            xPow *= x;
        }
        return result;
    }
    // cast to bool
    explicit operator bool() const {
        bool result = false;
        for (size_t i = 0; i < coeff.size(); i++) {
            if (coeff[i] != 0) result = true;
        }
        return result;
    }
    // operator !
    bool operator!() const {
        return !(static_cast<bool>(*this));
    }
    // degree (), returns unsigned value
    double degree() const {
        double result = 0;
        for (size_t i = 0; i < coeff.size(); i++) {
            if (coeff[i] != 0) result = i;
        }
        return result;
    }
private:
    std::vector<double> coeff;
};

#ifndef __PROGTEST__
bool smallDiff ( double a, double b ) {
    // TODO
    return true;
}

bool dumpMatch ( const CPolynomial & x, const std::vector<double> & ref ) {
    // TODO
    return true;
}

int main () {
    CPolynomial a, b, c;
    std::ostringstream out, tmp;

    a[0] = -10;
    a[1] = 3.5;
    a[3] = 1;
    // assert ( smallDiff ( a ( 2 ), 5 ) );
    out . str ("");
    out << a;
    assert ( out . str () == "x^3 + 3.5*x^1 - 10" );
    c = a * -2;
    // assert ( c . degree () == 3
    //          && dumpMatch ( c, std::vector<double>{ 20.0, -7.0, -0.0, -2.0 } ) );

    out . str ("");
    out << c;
    assert ( out . str () == "- 2*x^3 - 7*x^1 + 20" );
    out . str ("");
    out << b;
    assert ( out . str () == "0" );
    b[5] = -1;
    b[2] = 3;
    out . str ("");
    out << b;
    assert ( out . str () == "- x^5 + 3*x^2" );
    c = a * b;
    // assert ( c . degree () == 8
    //          && dumpMatch ( c, std::vector<double>{ -0.0, -0.0, -30.0, 10.5, -0.0, 13.0, -3.5, 0.0, -1.0 } ) );

    out . str ("");
    out << c;
    assert ( out . str () == "- x^8 - 3.5*x^6 + 13*x^5 + 10.5*x^3 - 30*x^2" );
    a *= 5;
    // assert ( a . degree () == 3
    //          && dumpMatch ( a, std::vector<double>{ -50.0, 17.5, 0.0, 5.0 } ) );

    a *= b;
    // assert ( a . degree () == 8
    //          && dumpMatch ( a, std::vector<double>{ 0.0, 0.0, -150.0, 52.5, -0.0, 65.0, -17.5, -0.0, -5.0 } ) );

    assert ( a != b );
    b[5] = 0;
    assert ( static_cast<bool> ( b ) );
    assert ( ! ! b );
    b[2] = 0;
    assert ( !(a == b) );
    a *= 0;
    //assert ( a . degree () == 0
    //       && dumpMatch ( a, std::vector<double>{ 0.0 } ) );

    assert ( a == b );
    assert ( ! static_cast<bool> ( b ) );
    assert ( ! b );

    // // bonus - manipulators

    // out . str ("");
    // out << poly_var ( "y" ) << c;
    // assert ( out . str () == "- y^8 - 3.5*y^6 + 13*y^5 + 10.5*y^3 - 30*y^2" );
    // out . str ("");
    // tmp << poly_var ( "abc" );
    // out . copyfmt ( tmp );
    // out << c;
    // assert ( out . str () == "- abc^8 - 3.5*abc^6 + 13*abc^5 + 10.5*abc^3 - 30*abc^2" );
    return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
