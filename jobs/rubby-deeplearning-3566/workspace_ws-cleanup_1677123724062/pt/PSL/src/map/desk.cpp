//
// Created by hao on 2022/10/12.
//

#include "desk.h"
#include "src/utils/utils.h"

using namespace dl_map;

template<typename T>
void RandomPointCloud(
        PointCloud<T> &point, const size_t N, const T max_range = 10)
{
    // Generating Random Point Cloud
    point.pts.resize(N);
    for (size_t i = 0; i < N; i++)
    {
        point.pts[i].x = max_range * (rand() % 1000) / T(1000);
        point.pts[i].y = max_range * (rand() % 1000) / T(1000);
        point.pts[i].z = max_range * (rand() % 1000) / T(1000);
    }
}

template<class POINT_TYPE, class TREE>
void KNNSearch(const TREE &tree, PointCloud<POINT_TYPE> &cloud)
{
    const size_t num_results = 5;
    std::cout << "Searching for " << num_results << " elements" << std::endl;

    POINT_TYPE queryPoint[3] = {0.5, 0.5, 0.5};
    size_t retIndex[num_results];
    POINT_TYPE outDistSqr[num_results];
    nanoflann::KNNResultSet<POINT_TYPE> resultSet(num_results);

    resultSet.init(retIndex, outDistSqr);
    tree.findNeighbors(resultSet, queryPoint, nanoflann::SearchParams(10));

    std::cout << "knnSearch(nn=" << num_results << "): \n";
    std::cout << "Results: " << std::endl;
    for (size_t i = 0; i < resultSet.size(); ++i)
    {
        std::cout << "#" << i << ",\t"
                  << "index: " << retIndex[i] << ",\t"
                  << "dist: " << outDistSqr[i] << ",\t"
                  << "point: (" << cloud.pts[retIndex[i]].x << ", "
                  << cloud.pts[retIndex[i]].y << ", "
                  << cloud.pts[retIndex[i]].z << ")" << std::endl;
    }
    std::cout << std::endl;
}

template<class POINT_TYPE, class TREE>
void RadiusSearch(const TREE &tree, PointCloud<POINT_TYPE> &cloud)
{
    POINT_TYPE queryPoint[3] = {0.5, 0.5, 0.5};
    std::cout << "Unsorted radius search" << std::endl;
    const POINT_TYPE radius = 0.3;
    std::vector<std::pair<size_t, POINT_TYPE>> indices_dists;
    nanoflann::RadiusResultSet<POINT_TYPE, size_t> resultSet(
            radius, indices_dists);

    tree.findNeighbors(resultSet, queryPoint, nanoflann::SearchParams());

    int matchCount = resultSet.size();
    std::cout << "radiusSearch(): radius=" << radius << " -> "
              << matchCount << " matches\n";

    for (size_t i = 0; i < matchCount; i++)
    {
        const auto &item = resultSet.m_indices_dists[i];
        std::cout << "#" << i << ",\t"
                  << "index: " << item.first << ",\t"
                  << "dist: " << item.second << ",\t"
                  << "point: (" << cloud.pts[item.first].Print() << ")"
                  << std::endl;
    }
    std::cout << "\n";

    if (matchCount > 0)
    {    // Get worst (furthest) point, without sorting:
        std::pair<size_t, POINT_TYPE> worst_pair = resultSet.worst_item();
        std::cout << "Worst pair: idx=" << worst_pair.first
                  << " dist=" << worst_pair.second
                  << " point: (" << cloud.pts[worst_pair.first].Print() << ")" << std::endl;
        std::cout << std::endl;
    }
};

template<class TREE>
void Add(const int N, TREE &tree)
{
    // add points in chunks at a time
    size_t chunk_size = 100;
    for (size_t i = 0; i < N; i = i + chunk_size)
    {
        size_t end = std::min<size_t>(i + chunk_size - 1, N - 1);
        // Inserts all points from [i, end]
        tree.addPoints(i, end);
    }

    // remove a point
    size_t removePointIndex = N - 1;
    tree.removePoint(removePointIndex);
}


template<typename num_t>
void KDTreeDemo(const size_t N)
{
    const int DIM = 3;

    using point_cloud = PointCloud<num_t>;
    using kd_tree = nanoflann::KDTreeSingleIndexDynamicAdaptor<
            nanoflann::L2_Simple_Adaptor<num_t, point_cloud>, point_cloud, DIM>;

    point_cloud cloud;
    const int MAX_LEAF = 10;
    kd_tree tree(DIM, cloud, {MAX_LEAF});

    RandomPointCloud(cloud, N);
    Add(N, tree);
    std::cout << "tree size: " << tree.TreeSize() << std::endl;
    KNNSearch<num_t, kd_tree>(tree, cloud);
    RadiusSearch<num_t, kd_tree>(tree, cloud);

//    cloud.pts.resize(cloud.pts.size()*0.5);
//    index.buildIndex();
}


dl_map::Desk::Desk()
{
    point_cloud cloud;
    kd_tree tree(DIM, cloud, {MAX_LEAF});

//    Demo();
}

dl_map::Desk::~Desk()
{

}

void dl_map::Desk::Demo() const
{
    srand(static_cast<unsigned int>(time(nullptr)));
    KDTreeDemo<float>(150);
    KDTreeDemo<double>(100000);
}
