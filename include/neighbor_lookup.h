#ifndef NEIGHBOR_LOOKUP_H
#define NEIGHBOR_LOOKUP_H

#include <functional>
#include "vector.h"
#include <vector>
// A base interface class for neighbor lookup. Heavily based on Doyub Kim's FLuid engine
// Developement book. 

template<typename T>
class PointNeighborLookup3 {
public:
    //using ForEachNeighborFunc = std::function<void(size_t, const cato::Vec3T<T>&)>;
    typedef std::function<void(size_t, const cato::Vec3T<T>&)> ForEachNeighborFunc;

    PointNeighborLookup3() = default;
    virtual ~PointNeighborLookup3() = default;

    virtual void build(const std::vector<cato::Vec3T<T>>& points) = 0;

    virtual void for_each_nearby_point(
        const cato::Vec3T<T>& origin,
        T radius,
        const ForEachNeighborFunc& callback) const = 0;

};



#endif