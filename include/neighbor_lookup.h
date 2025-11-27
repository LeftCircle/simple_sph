#include <functional>
#ifndef NEIGHBOR_LOOKUP_H
#define NEIGHBOR_LOOKUP_H

#include "vector.h"
#include <vector>
// A base interface class for neighbor lookup. Heavily based on Doyub Kim's FLuid engine
// Developement book. 

template<typename T>
class PointNeighborLookup3 {
public:
    using NeighborFunc = std::function<void(size_t, const cato::Vec3T<T>&)>;

    PointNeighborLookup3() = default;
    virtual ~PointNeighborLookup3() = default;

    virtual void build(const std::vector<cato::Vec3T<T>>& points) = 0;

    virtual void forEachNearbyPoint(
        const cato::Vec3T<T>& origin,
        T radius,
        const NeighborFunc& callback) const = 0;

};


#endif