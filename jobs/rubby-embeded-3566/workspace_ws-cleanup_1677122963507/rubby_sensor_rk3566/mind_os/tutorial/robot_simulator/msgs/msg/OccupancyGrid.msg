include "Header.msg"
include "Grid.msg"

namespace tutorial.slam

message OccupancyGrid
{
    header: tutorial.base.Header
    width: int
    height: int
    origin_x: double
    origin_y: double
    resolution: float
    mdata: [int8_t]
    grid_data: [base.Grid]
}