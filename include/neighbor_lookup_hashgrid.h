#ifndef NEIGHBOR_LOOKUP_HASHGRID_H
#define NEIGHBOR_LOOKUP_HASHGRID_H


#include "neighbor_lookup.h"
#include <unordered_map>
#include <vector>


template<typename T>
class PointNeighborLookupHashGrid3 final : public PointNeighborLookup3<T> {
public:
    using ForEachNeighborFunc = typename PointNeighborLookup3<T>::ForEachNeighborFunc;

    PointNeighborLookupHashGrid3(const cato::Vec3i& resolution, double cellSize)
        : _resolution(resolution), _cellSize(cellSize) {}
    PointNeighborLookupHashGrid3(int resolutionX, int resolutionY, int resolutionZ, double cellSize)
        : _resolution(cato::Vec3i{resolutionX, resolutionY, resolutionZ}), _cellSize(cellSize) {}
    
    void build(const std::vector<cato::Vec3T<T>>& points) override;

    void for_each_nearby_point(
        const cato::Vec3T<T>& origin,
        T radius,
        const ForEachNeighborFunc& callback) const override;

    size_t get_hashkey_from_position(const cato::Vec3T<T>& position) const;

   private:
    cato::Vec3i _get_bucket_index(const cato::Vec3T<T>& position) const;
    size_t _get_hashkey_from_bucket_index(const cato::Vec3i& bucketIndex) const;
    // Fills in the 8 nearby buckets for a given point. Assumes the diamter of the point is equal
    // to the cell size
    void _get_neighboring_buckets(const cato::Vec3T<T>& position, size_t* bucket_indeces) const;

private:
    double _cellSize;
    cato::Vec3i _resolution = cato::Vec3i{1, 1, 1};
    // a copy of the input points
    std::vector<cato::Vec3T<T>> _points;
    // A vector of buckets that contains a list of all point indices in that bucket
    // The index into the vector corresponds to the voxel index hashed into a 1D value
    std::vector<std::vector<size_t>> _buckets;

};

using PointNeighborLookupHashGrid3f = PointNeighborLookupHashGrid3<float>;
using PointNeighborLookupHashGrid3d = PointNeighborLookupHashGrid3<double>;


#endif