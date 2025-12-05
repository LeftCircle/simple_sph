#ifndef NEIGHBOR_LOOKUP2D_H
#define NEIGHBOR_LOOKUP2D_H

#include <functional>
#include "vector.h"
#include <vector>


template<typename T>
class PointNeighborLookup2 {
public:
    //using ForEachNeighborFunc = std::function<void(size_t, const cato::Vec3T<T>&)>;
    typedef std::function<void(size_t, const cato::Vec2T<T>&)> ForEachNeighborFunc;

    PointNeighborLookup2() = default;
    virtual ~PointNeighborLookup2() = default;

    virtual void build(const std::vector<cato::Vec2T<T>>& points, const cato::Vec2T<T>& blc) = 0;

    virtual void for_each_nearby_point(
        const cato::Vec2T<T>& origin,
        T radius,
        const ForEachNeighborFunc& callback) const = 0;

};



#endif // NEIGHBOR_LOOKUP2D_H