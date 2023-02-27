//
// Created by lcy on 23-1-10.
//

#ifndef SEGMENT_MAPSEGMENTATIONGOAL_H
#define SEGMENT_MAPSEGMENTATIONGOAL_H

#include "config.h"

class MapSegmentationGoal
{
public:
    double map_resolution;
    float robot_radius;
    int room_segmentation_algorithm;

    int  expand_pretreatment;

    bool publish_segmented_map;

    double room_area_factor_upper_limit_morphological;
    double room_area_factor_lower_limit_morphological;

    double room_area_factor_upper_limit_distance;
    double room_area_factor_lower_limit_distance;

    double room_area_factor_upper_limit_voronoi;
    double room_area_factor_lower_limit_voronoi;

    int voronoi_neighborhood_index;
    int max_iterations;
    double min_critical_point_distance_factor;

    double wall_around_case_a;
    double wall_around_case_c;
    double room_area_lower_limit;
    double contact_length_max_ratio_b;
    double contact_length_max_ratio_c;
    double neighbor_room_length_max_ratio_c1;
    double neighbor_room_length_max_ratio_c2;
    double contact_length_max_ratio_d1;
    double contact_length_max_ratio_d2;
    double contact_length_max_ratio_e;
    double max_area_for_merging;

    double corridor_width_min;
    double corridor_width_max;
    double corridor_maxarea;
    bool corridor_separate;

    double roomdoor_min;
    double roomdoor_max;
    double roomarea_max;
    double mergeroom_maxarea;
    double mergeroom_minarea;
    int expand_points;

    bool publish_raw_map;
    bool publish_door_map;
    bool publish_color_map;


    void Init(){
        map_resolution=s_config.readParam<double>("map_resolution");
        robot_radius=s_config.readParam<float>("robot_radius");
        room_segmentation_algorithm=s_config.readParam<int>("room_segmentation_algorithm");
        expand_pretreatment=s_config.readParam<int>("expand_pretreatment");
        publish_segmented_map=s_config.readParam<bool>("publish_segmented_map");
        room_area_factor_upper_limit_morphological=s_config.readParam<double>("room_area_factor_upper_limit_morphological");
        room_area_factor_lower_limit_morphological=s_config.readParam<double>("room_area_factor_lower_limit_morphological");
        room_area_factor_upper_limit_distance=s_config.readParam<double>("room_area_factor_upper_limit_distance");
        room_area_factor_lower_limit_distance=s_config.readParam<double>("room_area_factor_lower_limit_distance");
        room_area_factor_upper_limit_voronoi=s_config.readParam<double>("room_area_factor_upper_limit_voronoi");
        room_area_factor_lower_limit_voronoi=s_config.readParam<double>("room_area_factor_lower_limit_voronoi");
        voronoi_neighborhood_index=s_config.readParam<int>("voronoi_neighborhood_index");
        max_iterations=s_config.readParam<int>("max_iterations");
        min_critical_point_distance_factor=s_config.readParam<double>("min_critical_point_distance_factor");
        wall_around_case_a=s_config.readParam<double>("wall_around_case_a");
        wall_around_case_c=s_config.readParam<double>("wall_around_case_c");
        room_area_lower_limit=s_config.readParam<double>("room_area_lower_limit");
        contact_length_max_ratio_b=s_config.readParam<double>("contact_length_max_ratio_b");
        contact_length_max_ratio_c=s_config.readParam<double>("contact_length_max_ratio_c");
        neighbor_room_length_max_ratio_c1=s_config.readParam<double>("neighbor_room_length_max_ratio_c1");
        neighbor_room_length_max_ratio_c2=s_config.readParam<double>("neighbor_room_length_max_ratio_c2");
        contact_length_max_ratio_d1=s_config.readParam<double>("contact_length_max_ratio_d1");
        contact_length_max_ratio_d2=s_config.readParam<double>("contact_length_max_ratio_d2");
        contact_length_max_ratio_e=s_config.readParam<double>("contact_length_max_ratio_e");
        max_area_for_merging=s_config.readParam<double>("max_area_for_merging");
        corridor_width_min=s_config.readParam<double>("corridor_width_min");
        corridor_width_max=s_config.readParam<double>("corridor_width_max");
        corridor_maxarea=s_config.readParam<double>("corridor_maxarea");
        corridor_separate=s_config.readParam<bool>("corridor_separate");
        roomdoor_min=s_config.readParam<double>("roomdoor_min");
        roomdoor_max=s_config.readParam<double>("roomdoor_max");
        roomarea_max=s_config.readParam<double>("roomarea_max");
        mergeroom_maxarea=s_config.readParam<double>("mergeroom_maxarea");
        mergeroom_minarea=s_config.readParam<double>("mergeroom_minarea");
        expand_points=s_config.readParam<int>("expand_points");

        publish_raw_map=s_config.readParam<bool>("publish_raw_map");
        publish_door_map=s_config.readParam<bool>("publish_door_map");
        publish_color_map=s_config.readParam<bool>("publish_color_map");

    }


};

#endif //SEGMENT_MAPSEGMENTATIONGOAL_H
