#ifndef _ARRAY3_
#define _ARRAY3_

#define RANGE_CHECK 1

#include <iostream>
using namespace std;

template <class T>
class Array3 {
public:
    const int sizeX, sizeY, sizeZ;

    Array3(int sizeX, int sizeY, int sizeZ);
    ~Array3();

    T get(int x, int y, int z) const;
    void set(int x, int y, int z, T value);

private:
    T * arr;

    int getLinearIndex(int x, int y, int z) const;

#if RANGE_CHECK
    void rangeCheck(int x, int y, int z) const;
#endif
};

template <class T>
Array3<T>::Array3(int sizeX, int sizeY, int sizeZ) 
    : sizeX(sizeX), sizeY(sizeY), sizeZ(sizeZ)
{
    arr = new T[sizeX * sizeY * sizeZ];
}

template <class T>
Array3<T>::~Array3() {
    delete[] arr;
}

template <class T>
T Array3<T>::get(int x, int y, int z) const {
#if RANGE_CHECK
    rangeCheck(x, y, z);
#endif
    return arr[getLinearIndex(x, y, z)];
}

template <class T>
void Array3<T>::set(int x, int y, int z, T value) {
#if RANGE_CHECK
    rangeCheck(x, y, z);
#endif
    arr[getLinearIndex(x, y, z)] = value;
}

template <class T>
int Array3<T>::getLinearIndex(int x, int y, int z) const {
    return x + y * sizeX + z * sizeY * sizeX;
}

#if RANGE_CHECK
    template <class T>
    void Array3<T>::rangeCheck(int x, int y, int z) const {
        if (!(0 <= x && x < sizeX &&
              0 <= y && y < sizeY &&
              0 <= z && z < sizeZ))
            cerr << "range fail: " << x << "," << y << "," << z << " : " << sizeX << "," << sizeY << "," << sizeZ << endl;
    }
#endif


#endif