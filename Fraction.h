#ifndef LPFRACTION_H
#define LPFRACTION_H

#include <iostream>
#include <sstream>

namespace lp {

template <class T>
T gcd(T a, T b) {
    if (b == 0) return a;
    return gcd(b, a % b);
}

template <class T>
T lcm(T a, T b) {
    T g = gcd(a, b);
    return a * b / g;
}

template <class T>
class Fraction {
private:
    T num;
    T denom;

    void reduce() {
        T n = num >= 0 ? num : -num;
        T g = gcd(n, denom);
        num /= g;
        denom /= g;
    }

public:
    Fraction() : num(0), denom(1) {}
    Fraction(T _numerator, T _denominator = 1) :
        num(_numerator), denom(_denominator) {
        if (denom < 0) {
            denom *= -1;
            num *= -1;
        }

        reduce();
    }

    Fraction& operator+=(const Fraction& f){
        this->num *= f.denom;
        this->num += this->denom * f.num;
        this->denom *= f.denom;
        this->reduce();

        return *this;
    }

    Fraction& operator-=(const Fraction& f) {
        this->num *= f.denom;
        this->num -= this->denom * f.num;
        this->denom *= f.denom;
        this->reduce();

        return *this;
    }

    Fraction& operator*=(const Fraction& f) {
        this->num *= f.num;
        this->denom *= f.denom;
        this->reduce();

        return *this;
    }

    Fraction& operator/=(const Fraction& f) {
        this->num *= f.denom;
        this->denom *= f.num;

        if (this->denom < 0) {
            this->denom *= -1;
            this->num *= -1;
        }

        this->reduce();

        return *this;
    }

    Fraction operator+(const Fraction& f) const {
        Fraction<T> res = *this;
        res += f;
        return res;
    }

    Fraction operator-(const Fraction& f) const {
        Fraction<T> res = *this;
        res -= f;
        return res;
    }

    Fraction operator*(const Fraction& f) const {
        Fraction<T> res = *this;
        res *= f;
        return res;
    }

    Fraction operator/(const Fraction& f) const {
        Fraction<T> res = *this;
        res /= f;
        return res;
    }

    bool operator==(const Fraction& f) const {
        return this->num == f.num && this->denom == f.denom;
    }

    bool operator!=(const Fraction& f) const {
        return !(*this == f);
    }

    bool operator<=(const Fraction& f) const {
        return this->num * f.denom <= this->denom * f.num;
    }

    bool operator>=(const Fraction& f) const {
        return this->num * f.denom >= this->denom * f.num;
    }

    bool operator<(const Fraction& f) const {
        return !(*this >= f);
    }

    bool operator>(const Fraction& f) const {
        return !(*this <= f);
    }

    T numerator() const {
        return num;
    }

    T denominator() const {
        return denom;
    }

    friend std::ostream& operator<<(std::ostream& out, const Fraction<T>& f) {
        out << f.num << '/' << f.denom;
        return out;
    }

    friend std::istream& operator>>(std::istream& in, Fraction<T>& f) {
        char c = ' ';
        std::stringstream ss;
        while (c <= ' ' && !in.eof()) in.get(c);

        do {
            ss << c;
            in.get(c);
        } while (c > ' ' && !in.eof() && c != '/');

        ss >> f.num;
        ss.clear();

        if (c != '/') {
            f.denom = 1;
            return in;
        }

        in.get(c);
        while (c > ' ' && !in.eof()) {
            ss << c;
            in.get(c);
        }
        ss >> f.denom;
        if (f.denom < 0) {
            f.num *= -1;
            f.denom *= -1;
        }

        f.reduce();
        return in;
    }
};

}

#endif
