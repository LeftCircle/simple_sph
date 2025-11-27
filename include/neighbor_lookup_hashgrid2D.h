#ifndef NEIGHBOR_LOOKUP_HASHGRID2D_H
#define NEIGHBOR_LOOKUP_HASHGRID2D_H


#include "neighbor_lookup2D.h"
#include <unordered_map>
#include <vector>


template<typename T>
class PointNeighborLookupHashGrid2 final : public PointNeighborLookup2<T> {
public:
    using ForEachNeighborFunc = typename PointNeighborLookup2<T>::ForEachNeighborFunc;

    PointNeighborLookupHashGrid2(const cato::Vec2i& resolution, double cellSize)
        : _resolution(resolution), _cellSize(cellSize) {}
    PointNeighborLookupHashGrid2(int resolutionX, int resolutionY, double cellSize)
        : _resolution(cato::Vec2i{resolutionX, resolutionY}), _cellSize(cellSize) {}
    
    void build(const std::vector<cato::Vec2T<T>>& points) override;

    void for_each_nearby_point(
        const cato::Vec2T<T>& origin,
        T radius,
        const ForEachNeighborFunc& callback) const override;

    size_t get_hashkey_from_position(const cato::Vec2T<T>& position) const;
   private:
    cato::Vec2i _get_bucket_index(const cato::Vec2T<T>& position) const;
    size_t _get_hashkey_from_bucket_index(const cato::Vec2i& bucketIndex) const;
    // Fills in the 8 nearby buckets for a given point. Assumes the diamter of the point is equal
    // to the cell size
    void _get_neighboring_buckets(const cato::Vec2T<T>& position, size_t* bucket_indeces) const;

private:
    double _cellSize;
    cato::Vec2i _resolution = cato::Vec2i{1, 1};
    // a copy of the input points
    std::vector<cato::Vec2T<T>> _points;
    // A vector of buckets that contains a list of all point indices in that bucket
    // The index into the vector corresponds to the voxel index hashed into a 1D value
    std::vector<std::vector<size_t>> _buckets;

};

using PointNeighborLookupHashGrid2f = PointNeighborLookupHashGrid2<float>;
using PointNeighborLookupHashGrid2d = PointNeighborLookupHashGrid2<double>;


#endif