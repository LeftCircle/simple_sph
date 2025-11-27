#include "neighbor_lookup_hashgrid.h"


template<typename T>
void PointNeighborLookupHashGrid3<T>::build(const std::vector<cato::Vec3T<T>>& points) {
	_points.clear();
	_buckets.clear();
	_points = points;
	size_t numBuckets = static_cast<size_t>(_resolution.x * _resolution.y * _resolution.z);
	_buckets.resize(numBuckets);

	// Now we have to place each point index into the appropriate bucket
	for (size_t i = 0; i < points.size(); ++i) {
		size_t bucket_index = get_hashkey_from_position(points[i]);
		_buckets[bucket_index].push_back(i);
	}
}

template<typename T>
void PointNeighborLookupHashGrid3<T>::for_each_nearby_point(
	const cato::Vec3T<T>& origin,
	T radius,
	const ForEachNeighborFunc& callback) const {
	
	if (_buckets.empty()) {
		return;
	}
	
	size_t neighbor_buckets[8];
	_get_neighboring_buckets(origin, neighbor_buckets);

	const T rsq = radius * radius;
	for (int i = 0; i < 8; ++i) {
		size_t bucket_index = neighbor_buckets[i];
		const std::vector<size_t>& bucket = _buckets[bucket_index];
		for (size_t point_index : bucket) {
			const cato::Vec3T<T>& point = _points[point_index];
			T m_sq = (point - origin).magnitude_squared();
			if (m_sq <= rsq) {
				callback(point_index, point);
			}
		}
	}
}


template<typename T>
size_t PointNeighborLookupHashGrid3<T>::get_hashkey_from_position(const cato::Vec3T<T>& position) const {
	cato::Vec3i bucketIndex = _get_bucket_index(position);
	return _get_hashkey_from_bucket_index(bucketIndex);
}

template<typename T>
cato::Vec3i PointNeighborLookupHashGrid3<T>::_get_bucket_index(const cato::Vec3T<T>& position) const {
	int ix = static_cast<int>(std::floor(position.x / _cellSize));
	int iy = static_cast<int>(std::floor(position.y / _cellSize));
	int iz = static_cast<int>(std::floor(position.z / _cellSize));
	return cato::Vec3i{ ix, iy, iz };
}

template<typename T>
size_t PointNeighborLookupHashGrid3<T>::_get_hashkey_from_bucket_index(const cato::Vec3i& bucketIndex) const {
	// Wrap around for negative indices
	int ix = bucketIndex.x % _resolution.x;
	if (ix < 0) ix += _resolution.x;
	int iy = bucketIndex.y % _resolution.y;
	if (iy < 0) iy += _resolution.y;
	int iz = bucketIndex.z % _resolution.z;
	if (iz < 0) iz += _resolution.z;

	return static_cast<size_t>(iz * _resolution.y * _resolution.x + iy * _resolution.x + ix);
}

template<typename T>
void PointNeighborLookupHashGrid3<T>::_get_neighboring_buckets(const cato::Vec3T<T>& position, size_t* bucket_indeces) const {
	cato::Vec3i centerBucket = _get_bucket_index(position);
	cato::Vec3i neighbor_buckets[8];

	for (int i = 0; i < 8; ++i) {
		neighbor_buckets[i] = centerBucket;
	}

	cato::Vec3T<T> center_pos = cato::Vec3T<T>{
		(centerBucket.x + 0.5) * static_cast<T>(_cellSize),
		(centerBucket.y + 0.5) * static_cast<T>(_cellSize),
		(centerBucket.z + 0.5) * static_cast<T>(_cellSize)
	};
	// Let's say neighbors 0, 1, 2, 3 have the same x
	// neighbors 0, 1, 4, 5 have the same y
	// neighbors 0, 2, 4, 5 have the same z
	int x_offset = position.x < center_pos.x ? -1 : 1;
	int y_offset = position.y < center_pos.y ? -1 : 1;
	int z_offset = position.z < center_pos.z ? -1 : 1;
	
	// x offsets:
	neighbor_buckets[4].x += x_offset;
	neighbor_buckets[5].x += x_offset;
	neighbor_buckets[6].x += x_offset;
	neighbor_buckets[7].x += x_offset;

	// y offsets:
	neighbor_buckets[2].y += y_offset;
	neighbor_buckets[3].y += y_offset;
	neighbor_buckets[6].y += y_offset;
	neighbor_buckets[7].y += y_offset;

	// z offsets:
	neighbor_buckets[1].z += z_offset;
	neighbor_buckets[3].z += z_offset;
	neighbor_buckets[5].z += z_offset;
	neighbor_buckets[7].z += z_offset;

	// Now let's get the hashkeys
	for (int i = 0; i < 8; ++i) {
		bucket_indeces[i] = _get_hashkey_from_bucket_index(neighbor_buckets[i]);
	}
}


// Explicit template instantiations
template class PointNeighborLookupHashGrid3<float>;
template class PointNeighborLookupHashGrid3<double>;
