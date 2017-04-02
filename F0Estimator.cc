//
// Created by Ziye Fan on 1/4/2017.
//

#include "F0Estimator.hh"
#include <cassert>
#include <vector>
#include <cmath>
#include <algorithm>

float F0Estimator::EstimatePeriod(const float *x, const int n, const int minP, const int maxP, float &q) {
    assert( minP > 1 );
    assert( maxP > minP );
    assert( n >= 2 * maxP );
    assert( x != NULL );

    q = 0;

    // Compute NAC
    std::vector<float> nac(maxP + 2);
    for (int p = minP - 1; p <= maxP + 1; p ++) {
        float nac_p = 0;
        float E1 = 0, E2 = 0;
        for (int i = 0; i < n - p; i ++) {
            nac_p += x[i] * x[i + p];
            E1 += x[i] * x[i];
            E2 += x[i + p] * x[i + p];
        }
        nac[p] = nac_p / sqrtf(E1 * E2);
    }

    // Get the highest peak
    auto bestP = std::distance(nac.begin(), std::max_element(nac.begin(), nac.end()));

    // Check if it is illegal
    if (bestP < minP || bestP > maxP) {
        q = 0;
        return 0;
    }

    q = nac[bestP];

    float mid   = nac[bestP];
    float left  = nac[bestP - 1];
    float right = nac[bestP + 1];

    float shift = 0.5f * (right - left) / (2.0f * mid - left - right);
    float pEst = bestP + shift;

    return pEst;
    // WIP

}