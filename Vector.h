#ifndef LPVECTOR_H
#define LPVECTOR_H

namespace lp {

template<class T>
class Vector {
private:
    T* vector = nullptr;
    int _size = 0;

public:
    Vector(int size = 0) : _size(size) {
        this->vector = new T[size];
    }

	Vector(const Vector& v) : _size(v._size) {
		this->vector = new T[_size];
		for (int i = 0; i < this->_size; i++) {
			this->vector[i] = v.vector[i];
		}
	}

	Vector(Vector&& v) : _size(v._size) {
		this->vector = v.vector;
		v.vector = nullptr;
	}

    Vector& operator=(const Vector& v) {
        this = new Vector(v);

        return *this;
    }

    Vector& operator=(Vector&& v) {
        this->_size = v._size;
        this->vector = v.vector;
		v.vector = nullptr;

        return *this;
    }

    Vector& operator+=(const Vector& v) {
        for (int i = 0; i < this->_size; i++) {
            this->vector[i] += v.vector[i];
        }

        return *this;
    }

    Vector& operator-=(const Vector& v) {
        for (int i = 0; i < this->_size; i++) {
            this->vector[i] -= v.vector[i];
        }

        return *this;
    }

    Vector& operator*=(const T& value) {
        for (int i = 0; i < this->_size; i++) {
            this->vector[i] *= value;
        }

        return *this;
    }

    Vector& operator/=(const T& value) {
        for (int i = 0; i < this->_size; i++) {
            this->vector[i] /= value;
        }

        return *this;
    }

    Vector operator+(const Vector& v) const {
        Vector res(*this);
        res += v;
        return res;
    }

    Vector operator-(const Vector& v) const {
        Vector res(*this);
        res -= v;
        return res;
    }

    Vector operator*(const T& value) const {
        Vector res(*this);
        res *= value;
        return res;
    }

    Vector operator/(const T& value) const {
        Vector res(*this);
        res /= value;
        return res;
    }

    T& operator[](int index) {
        return this->vector[index];
    }

    void resize(int size) {
        delete this->vector;
        this->vector = new T[size];
        this->_size = size;
    }

    void swap(Vector& v) {
        T* t = v.vector;
        v.vector = this->vector;
        this->vector = t;

        int ts = v._size;
        v._size = this->_size;
        this->_size = ts;
    }

    int size() const {
        return this->_size;
    }

    ~Vector() {
        delete[] this->vector;
        this->vector = nullptr;
        this->_size = 0;
    }
};

}

#endif
