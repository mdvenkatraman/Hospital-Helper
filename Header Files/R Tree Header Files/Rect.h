#pragma once
#include <algorithm>
#include <limits>

struct Rect
{
    //start "empty" so expand () snaps to the real value
    //make starting value as large as possible so min() will snap to the real value
    double minLat = std::numeric_limits<double>::max();
    double maxLat = -std::numeric_limits<double>::max();
    double minLon = std::numeric_limits<double>::max();
    double maxLon = -std::numeric_limits<double>::max();

    //box around one point
    static Rect fromPoint(double lat, double lon)
    {
        Rect r;
        r.minLat = r.maxLat = lat;
        r.minLon = r.maxLon = lon;
        return r;
    }

    void expand(const Rect& other) {
        minLat = std::min(minLat, other.minLat);
        maxLat = std::max(maxLat, other.maxLat);
        minLon = std::min(minLon, other.minLon);
        maxLon = std::max(maxLon, other.maxLon);
    }

    double area() const {
        return (maxLat - minLat) * (maxLon - minLon);
    }

    //how much bigger the box would get if "other" was added to it
    double enlargementToInclude(const Rect& other) const {
        Rect expanded = *this;
        expanded.expand(other);
        return expanded.area() - area();
    }

    //computing distances squared to avoid extra unnecessary calculations later

    //squared distance from a point to the rectangle, used for min neighbors
    double minDistSq(double lat, double lon) const {
        double dLat = 0.0;
        if (lat < minLat) {
            dLat = minLat - lat;
        } else if (lat > maxLat) {
            dLat = lat - maxLat;
        }

        double dLon = 0.0;
        if (lon < minLon) {
            dLon = minLon - lon;
        } else if (lon > maxLon) {
            dLon = lon - maxLon;
        }

        return dLat * dLat + dLon * dLon;
    }
};
