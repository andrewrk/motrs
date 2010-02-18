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

    // add or remove columns, shifting the data to the right
    void expandLeft(int amount, T fillValue);
    void expandTop(int amount, T fillValue);
    void expandRight(int amount, T fillValue);
    void expandBottom(int amount, T fillValue);

    inline int sizeX(){ return m_sizeX; }
    inline int sizeY(){ return m_sizeY; }
    inline int sizeZ(){ return m_sizeZ; }

    void deleteRowZ(int index);
    void swapRowZ(int i, int j);

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
                    new_arr[x + y * newSizeX + z * newSizeY * newSizeX] = fill;
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

template <class T>
void Array3<T>::deleteRowZ(int index)
{
    T * new_arr = new T[m_sizeX * m_sizeY * (m_sizeZ-1)];
    for(int x=0; x<m_sizeX; ++x) {
        for( int y=0; y<m_sizeY; ++y) {
            for( int z=0; z<m_sizeZ; ++z) {
                if( z < index ) {
                    new_arr[getLinearIndex(x,y,z)] = m_arr[getLinearIndex(x,y,z)];
                } else if( z > index ) {
                    new_arr[x + y * m_sizeX + (z-1) * m_sizeY * m_sizeX] =
                        m_arr[getLinearIndex(x,y,z)];
                }
            }
        }
    }
    delete[] m_arr;
    m_arr = new_arr;
    --m_sizeZ;
}

template <class T>
void Array3<T>::swapRowZ(int i, int j)
{
    for(int x=0; x<m_sizeX; ++x) {
        for(int y=0; y<m_sizeY; ++y) {
            T tmp = m_arr[getLinearIndex(x,y,i)];
            m_arr[getLinearIndex(x,y,i)] = m_arr[getLinearIndex(x,y,j)];
            m_arr[getLinearIndex(x,y,j)] = tmp;
        }
    }
}

template <class T>
void Array3<T>::expandLeft(int amount, T fill)
{
    int newSizeX = m_sizeX + amount;
    T * new_arr = new T[newSizeX * m_sizeY * m_sizeZ];
    for(int x=0; x<m_sizeX; ++x) {
        for(int y=0; y<m_sizeY; ++y) {
            for(int z=0; z<m_sizeZ; ++z) {
                int newX = x + amount;

                if( newX >= 0 && newX < newSizeX ) {
                    new_arr[newX + y * newSizeX + z * m_sizeY * newSizeX] =
                            m_arr[x + y * m_sizeX + z * m_sizeY * m_sizeX];
                }
            }
        }
    }

    // add the fill
    for(int x=0; x<amount; ++x) {
        for(int y=0; y<m_sizeY; ++y) {
            for(int z=0; z<m_sizeZ; ++z) {
                new_arr[x + y * newSizeX + z * m_sizeY * newSizeX] = fill;
            }
        }
    }


    delete[] m_arr;
    m_arr = new_arr;
    m_sizeX = newSizeX;
}

template <class T>
void Array3<T>::expandTop(int amount, T fill)
{
    int newSizeY = m_sizeY + amount;
    T * new_arr = new T[m_sizeX * newSizeY * m_sizeZ];
    for(int x=0; x<m_sizeX; ++x) {
        for(int y=0; y<m_sizeY; ++y) {
            for(int z=0; z<m_sizeZ; ++z) {
                int newY = y + amount;

                if( newY >= 0 && newY < newSizeY ) {
                    new_arr[x + newY * m_sizeX + z * newSizeY * m_sizeX] =
                            m_arr[x + y * m_sizeX + z * m_sizeY * m_sizeX];
                }
            }
        }
    }

    // add the fill
    for(int y=0; y<amount; ++y) {
        for(int x=0; x<m_sizeX; ++x) {
            for(int z=0; z<m_sizeZ; ++z) {
                new_arr[x + y * m_sizeX + z * newSizeY * m_sizeX] = fill;
            }
        }
    }


    delete[] m_arr;
    m_arr = new_arr;
    m_sizeY = newSizeY;
}

template <class T>
void Array3<T>::expandRight(int amount, T fill)
{
    redim(m_sizeX + amount, m_sizeY, m_sizeZ, fill);
}

template <class T>
void Array3<T>::expandBottom(int amount, T fill)
{
    redim(m_sizeX, m_sizeY + amount, m_sizeZ, fill);
}

#endif
