//
// Created by Ziye Fan on 30/3/2017.
//

#ifndef AUDIOVIS_FFT_HH
#define AUDIOVIS_FFT_HH


#include <complex>
#include <cmath>
#include <algorithm>

// Reference:
//   http://www.drdobbs.com/cpp/a-simple-and-efficient-fft-implementatio/199500857?pgno=3

template<unsigned N, typename T=float>
class DanielsonLanczos {
    DanielsonLanczos<N/2, T> next;
public:
    void apply(T* data) {
        next.apply(data);
        next.apply(data + N);

        T tempr, tempi, c, s;
        for (unsigned i = 0; i < N; i += 2) {
            c = cos(i * M_PI / N);
            s = -sin(i * M_PI / N);
            tempr = data[i + N] * c - data[i + N + 1] * s;
            tempi = data[i + N] * s + data[i + N + 1] * c;
            data[i + N] = data[i] - tempr;
            data[i + N + 1] = data[i + 1] - tempi;
            data[i] += tempr;
            data[i + 1] += tempi;
        }
    }
};

template<typename T>
class DanielsonLanczos<1, T> {
public:
    void apply(T* data) {}
};

template<typename T>
void scramble(T* data, unsigned N) {
    // reverse-binary reindexing
    int n = N<<1;
    int j=1, m;
    for (int i=1; i<n; i+=2) {
        if (j>i) {
            std::swap(data[j-1], data[i-1]);
            std::swap(data[j], data[i]);
        }
        m = N;
        while (m>=2 && j>m) {
            j -= m;
            m >>= 1;
        }
        j += m;
    };
}

template<unsigned P, typename T=float>
class GFFT {
    enum { N = 1 << P };
    DanielsonLanczos<N,T> recursion;
public:
    void fft(T* data) {
        scramble(data, N);
        recursion.apply(data);
    }
};

#endif //AUDIOVIS_FFT_HH
