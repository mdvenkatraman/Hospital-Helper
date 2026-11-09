#pragma once
#include "Point.h"
#include <vector>
#include <cmath>
#include <limits>

class BruteForce {
public:

    static const Hospital* findNearest(const std::vector<Hospital>& hospitals, double lat, double lon) {
        if (hospitals.empty()) return nullptr;

        const Hospital* bestHospital = nullptr;
        double minDstSq = std::numeric_limits<double>::max();

        for (const auto& h : hospitals) {
            double dLat = h.latitude - lat;
            double dLon = h.longitude - lon;
            double dstSq = (dLat * dLat) + (dLon * dLon);

            if (dstSq < minDstSq) {
                minDstSq = dstSq;
                bestHospital = &h;
            }
        }
        return bestHospital;
    }
};