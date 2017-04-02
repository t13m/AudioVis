//
// Created by Ziye Fan on 1/4/2017.
//

#ifndef AUDIOVIS_F0ESTIMATOR_HH
#define AUDIOVIS_F0ESTIMATOR_HH


class F0Estimator {
public:
    static float EstimatePeriod(
            const float *x, const int n,
            const int minP, const int maxP,
            float & q);
};


#endif //AUDIOVIS_F0ESTIMATOR_HH
