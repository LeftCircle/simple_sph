#include "neighbor_lookup_hashgrid2D.h"


template<typename T>
void PointNeighborLookupHashGrid2<T>::build(const std::vector<cato::Vec2T<T>>& points) {
	_points.clear();
	_buckets.clear();
	_points = points;
	size_t numBuckets = static_cast<size_t>(_resolution.x * _resolution.y);
	_buckets.resize(numBuckets);

	// Now we have to place each point index into the appropriate bucket
	for (size_t i = 0; i < points.size(); ++i) {
		size_t bucket_index = get_hashkey_from_position(points[i]);
		_buckets[bucket_index].push_back(i);
	}
}

template<typename T>
void PointNeighborLookupHashGrid2<T>::for_each_nearby_point(
	const cato::Vec2T<T>& origin,
	T radius,
	const ForEachNeighborFunc& callback) const {
	
	if (_buckets.empty()) {
		return;
	}
	
	size_t neighbor_buckets[4];
	_get_neighboring_buckets(origin, neighbor_buckets);

	const T rsq = radius * radius;
	for (int i = 0; i < 4; ++i) {
		size_t bucket_index = neighbor_buckets[i];
		const std::vector<size_t>& bucket = _buckets[bucket_index];
		for (size_t point_index : bucket) {
			const cato::Vec2T<T>& point = _points[point_index];
			T m_sq = (point - origin).magnitude_squared();
			if (m_sq <= rsq) {
				callback(point_index, point);
			}
		}
	}
}


template<typename T>
size_t PointNeighborLookupHashGrid2<T>::get_hashkey_from_position(const cato::Vec2T<T>& position) const {
	cato::Vec2i bucketIndex = _get_bucket_index(position);
	return _get_hashkey_from_bucket_index(bucketIndex);
}

template<typename T>
cato::Vec2i PointNeighborLookupHashGrid2<T>::_get_bucket_index(const cato::Vec2T<T>& position) const {
	int ix = static_cast<int>(std::floor(position.x / _cellSize));
	int iy = static_cast<int>(std::floor(position.y / _cellSize));
	return cato::Vec2i{ ix, iy };
}

template<typename T>
size_t PointNeighborLookupHashGrid2<T>::_get_hashkey_from_bucket_index(const cato::Vec2i& bucketIndex) const {
	// Wrap around for negative indices
	int ix = bucketIndex.x % _resolution.x;
	if (ix < 0) ix += _resolution.x;
	int iy = bucketIndex.y % _resolution.y;
	if (iy < 0) iy += _resolution.y;

	return static_cast<size_t>(iy * _resolution.x + ix);
}

template<typename T>
void PointNeighborLookupHashGrid2<T>::_get_neighboring_buckets(const cato::Vec2T<T>& position, size_t* bucket_indeces) const {
	cato::Vec2i centerBucket = _get_bucket_index(position);
	cato::Vec2i neighbor_buckets[4];

	for (int i = 0; i < 4; ++i) {
		neighbor_buckets[i] = centerBucket;
	}

	cato::Vec2T<T> center_pos = cato::Vec2T<T>{
		(centerBucket.x + 0.5) * static_cast<T>(_cellSize),
		(centerBucket.y + 0.5) * static_cast<T>(_cellSize)
	};
	
	int x_offset = position.x < center_pos.x ? -1 : 1;
	int y_offset = position.y < center_pos.y ? -1 : 1;
	
    // 0 and 1 have same x
    // 0 and 2 have same y

	// x offsets:
	neighbor_buckets[2].x += x_offset;
	neighbor_buckets[3].x += x_offset;

	// y offsets:
	neighbor_buckets[1].y += y_offset;
    neighbor_buckets[3].y += y_offset;


	// Now let's get the hashkeys
	for (int i = 0; i < 4; ++i) {
		bucket_indeces[i] = _get_hashkey_from_bucket_index(neighbor_buckets[i]);
	}
}


// Explicit template instantiations
template class PointNeighborLookupHashGrid2<float>;
template class PointNeighborLookupHashGrid2<double>;