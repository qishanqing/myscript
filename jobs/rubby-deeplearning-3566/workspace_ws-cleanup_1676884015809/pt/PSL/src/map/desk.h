//
// Created by hao on 2022/10/12.
//

#ifndef SAMPLE_DESK_H
#define SAMPLE_DESK_H

#include "3rdparty/nanoflann/include/nanoflann.hpp"
#include "src/utils/shape/point.h"

namespace dl_map
{
const int MAX_LEAF = 10;
const int DIM = 3;

template<typename T>
struct PointCloud
{
    using coord_t = T;  //!< The type of each coordinate

    std::vector<Point> pts;

    // Must return the number of data points
    inline size_t kdtree_get_point_count() const
    { return pts.size(); }

    // Returns the dim'th component of the idx'th point in the class:
    // Since this is inlined and the "dim" argument is typically an immediate
    // value, the
    //  "if/else's" are actually solved at compile time.
    inline T kdtree_get_pt(const size_t idx, const size_t dim) const
    {
        if (dim == 0)
            return pts[idx].x;
        else if (dim == 1)
            return pts[idx].y;
        else
            return pts[idx].z;
    }

    // Optional bounding-box computation: return false to default to a standard
    // bbox computation loop.
    //   Return true if the BBOX was already computed by the class and returned
    //   in "bb" so it can be avoided to redo it again. Look at bb.size() to
    //   find out the expected dimensionality (e.g. 2 or 3 for point clouds)
    template<class BBOX>
    bool kdtree_get_bbox(BBOX & /* bb */) const
    {
        return false;
    }
};


using point_type = psl::LOCATION_TYPE;
using point_cloud = PointCloud<point_type>;
using kd_tree = nanoflann::KDTreeSingleIndexDynamicAdaptor<
        nanoflann::L2_Simple_Adaptor<point_type, point_cloud>, point_cloud, DIM>;

class Desk
{
public:
    Desk();

    ~Desk();

    void Demo() const;
};
}


#endif //SAMPLE_DESK_H
