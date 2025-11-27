#ifndef NEIGHBOR_LOOKUP_HASHGRID_H
#define NEIGHBOR_LOOKUP_HASHGRID_H


#include "neighbor_lookup.h"
#include <unordered_map>
#include <vector>


template<typename T>
class PointNeighborLookupHashGrid3 final : public PointNeighborLookup3<T> {
public:
    PointNeighborLookupHashGrid3(const cato::Vec3i& resolution, double cellSize)
        : _resolution(resolution), _cellSize(cellSize) {}
    PointNeighborLookupHashGrid3(int resolutionX, int resolutionY, int resolutionZ, double cellSize)
        : _resolution(cato::Vec3i{resolutionX, resolutionY, resolutionZ}), _cellSize(cellSize) {}
    
    void build(const std::vector<cato::Vec3T<T>>& points) override;

    void forEachNearbyPoint(
        const cato::Vec3T<T>& origin,
        T radius,
        const typename PointNeighborLookup3<T>::NeighborFunc& callback) const override;


private:
    double _cellSize;
    cato::Vec3i _resolution = cato::Vec3i{1, 1, 1};
    // a copy of the input points
    std::vector<cato::Vec3T<T>> _points;
    // A vector of buckets that contains a list of all point indices in that bucket
    // The index into the vector corresponds to the voxel index hashed into a 1D value
    std::vector<std::vector<size_t>> _buckets;

};


#endif