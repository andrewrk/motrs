#ifndef _ARRAY3_
#define _ARRAY3_

#include "version.h"
#include "Debug.h"

#ifndef RELEASE
#define RANGE_CHECK 1
#else
#define RANGE_CHECK 0
#endif

#include <iostream>
#include <cstring>

template <class T>
class Array3 {
public:
    Array3(int sizeX, int sizeY, int sizeZ);
    ~Array3();

    T get(int x, int y, int z) const;
    void set(int x, int y, int z, T value);

    // resize, keeping contents if possible.
    // extra spaces are populated with fillValue
    void redim(int newSizeX, int newSizeY, int newSizeZ, T fillValue);
    void redim(int newSizeX, int newSizeY, int newSizeZ);

    inline int sizeX(){ return m_sizeX; }
    inline int sizeY(){ return m_sizeY; }
    inline int sizeZ(){ return m_sizeZ; }


    void clear();
private:
    T * m_arr;
    int m_sizeX, m_sizeY, m_sizeZ;

    int getLinearIndex(int x, int y, int z) const;

#if RANGE_CHECK
    void rangeCheck(int x, int y, int z) const;
#endif
};

template <class T>
Array3<T>::Array3(int _sizeX, int _sizeY, int _sizeZ)
    : m_sizeX(_sizeX), m_sizeY(_sizeY), m_sizeZ(_sizeZ)
{
    m_arr = new T[m_sizeX * m_sizeY * m_sizeZ];
}

template <class T>
Array3<T>::~Array3() {
    delete[] m_arr;
}

template <class T>
T Array3<T>::get(int x, int y, int z) const {
#if RANGE_CHECK
    rangeCheck(x, y, z);
#endif
    return m_arr[getLinearIndex(x, y, z)];
}

template <class T>
void Array3<T>::set(int x, int y, int z, T value) {
#if RANGE_CHECK
    rangeCheck(x, y, z);
#endif
    m_arr[getLinearIndex(x, y, z)] = value;
}

template <class T>
void Array3<T>::clear() {
    std::memset(m_arr, 0, m_sizeX * m_sizeY * m_sizeZ * sizeof(T));
}

template <class T>
int Array3<T>::getLinearIndex(int x, int y, int z) const {
    return x + y * m_sizeX + z * m_sizeY * m_sizeX;
}

#if RANGE_CHECK
    template <class T>
    void Array3<T>::rangeCheck(int x, int y, int z) const {
        if(! (0 <= x && x < m_sizeX &&
            0 <= y && y < m_sizeY &&
            0 <= z && z < m_sizeZ) )
        {
            std::cerr << "range fail: " << x << "," << y << "," << z
                << " : " << m_sizeX << "," << m_sizeY << "," << m_sizeZ << std::endl;
            Debug::assert(false, "(the above range fail)");
        }
    }
#endif

template <class T>
void Array3<T>::redim(int newSizeX, int newSizeY, int newSizeZ) {
    T * new_arr = new T[newSizeX * newSizeY * newSizeZ];
    for(int x=0; x<newSizeX && x<m_sizeX; ++x) {
        for( int y=0; y<newSizeY && y<m_sizeY; ++y) {
            for( int z=0; z<newSizeZ && z<m_sizeZ; ++z) {
                new_arr[x + y * newSizeX + z * newSizeY * newSizeX] =
                    m_arr[x + y * m_sizeX + z * m_sizeY * m_sizeX];
            }
        }
    }
    delete[] m_arr;
    m_arr = new_arr;
    m_sizeX = newSizeX;
    m_sizeY = newSizeY;
    m_sizeZ = newSizeZ;
}

template <class T>
void Array3<T>::redim(int newSizeX, int newSizeY, int newSizeZ, T fill) {
    T * new_arr = new T[newSizeX * newSizeY * newSizeZ];
    for(int x=0; x<newSizeX; ++x) {
        for( int y=0; y<newSizeY; ++y) {
            for( int z=0; z<newSizeZ; ++z) {
                if( x < m_sizeX && y < m_sizeY && z < m_sizeZ ) {
                    new_arr[x + y * newSizeX + z * newSizeY * newSizeX] =
                        m_arr[x + y * m_sizeX + z * m_sizeY * m_sizeX];
                } else {
                    new_arr[x + y * newSizeX + z * newSizeY * newSizeX] =
                        fill;
                }
            }
        }
    }
    delete[] m_arr;
    m_arr = new_arr;
    m_sizeX = newSizeX;
    m_sizeY = newSizeY;
    m_sizeZ = newSizeZ;
}

#endif
