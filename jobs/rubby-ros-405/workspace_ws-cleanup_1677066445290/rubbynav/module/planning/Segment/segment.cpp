#include "segment.h"

void RoomSegment::SegmentInit(const cv::Mat& Map , const char * path)//图像预处理
{
//    robot_config.setConfigPath(path);
//    goal.Init(robot_config);
    goal.Init();

    //二值化
    cv::Mat  threshold1,threshold2,opening_map,dilate_map,eroded_map;
    cv::threshold(Map,threshold1,80,255,cv::THRESH_BINARY);
    cv::bitwise_not(threshold1,threshold2);

    cv::Point anchor(-1, -1);
    cv::Mat  elem=getStructuringElement(cv::MORPH_RECT,cv::Size(1+2*goal.expand_pretreatment,1+2*goal.expand_pretreatment));
    cv::dilate(threshold2, eroded_map, elem, anchor, 1);

    //提取图像外轮廓
    std::vector < cv::Vec4i > hierarchy;
    std::vector<std::vector<cv::Point> > contours;
    cv::findContours(eroded_map, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
    cv::Mat lena1 =cv::Mat::zeros(cv::Size(threshold2.cols,threshold2.rows),CV_8UC1);
    int max=0;
    for (int idx = 0; idx < contours.size(); idx++)
    {
        if(cv::contourArea(contours[idx])>cv::contourArea(contours[max]))
        {
            max=idx;
        }
    }
    cv::drawContours(lena1, contours,max, 255, -1);

    std::vector<std::vector<cv::Point> > ().swap(contours);
    std::vector < cv::Vec4i >().swap(hierarchy);
    contours.clear();
    //形态学处理，开运算膨胀腐蚀
    cv::erode(lena1,dilate_map,elem, anchor, 1);
    elem=getStructuringElement(cv::MORPH_RECT,cv::Size(5,5));
    cv::morphologyEx(dilate_map, opening_map, cv::MORPH_OPEN,elem,anchor, 1);
    elem=getStructuringElement(cv::MORPH_RECT,cv::Size(3,3));
    cv::erode(opening_map, eroded_map, elem, anchor, 1);
    elem=getStructuringElement(cv::MORPH_RECT,cv::Size(3,3));
    cv::dilate(eroded_map,dilate_map,elem, anchor, 1);
    cv::findContours(dilate_map, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
    lena1 =cv::Mat::zeros(cv::Size(threshold2.cols,threshold2.rows),CV_8UC1);
    max=0;
    for (int idx = 0; idx < contours.size(); idx++)
    {
        if(cv::contourArea(contours[idx])>cv::contourArea(contours[max]))
        {
            max=idx;
        }
    }
    cv::drawContours(lena1, contours,max, 255, -1);
    processed_map=lena1;

}

void RoomSegment::SegmentRun(const cv::Mat& Map,SegmentInfo& info_)
{
    cv::Mat segmented_map,threshold1,threshold2;
    int x_max=0,x_min=10000,y_min=10000,y_max=0;
    //剪裁地图
    for(int i=0;i<processed_map.rows;i++)
    {
        for(int j=0;j<processed_map.cols;j++)
        {
            int  pv=processed_map.at<uchar>(j,i);
            if(pv==255)
            {
                if(y_max<j)
                {
                    y_max=j;
                }
                if(y_min>j)
                {
                    y_min=j;
                }
                if(x_max<i)
                {
                    x_max=i;
                }
                if(x_min>i)
                {
                    x_min=i;
                }
            }
        }
    }
    cv::Mat ROI=processed_map(cv::Rect((x_min)/10*10-30,(y_min)/10*10-30,(x_max-x_min)/10*10+60,(y_max-y_min)/10*10+60));
    std::vector < std::vector<cv::Point> > room_contours,save_contours; 

    //房间分割
    if (goal.room_segmentation_algorithm == 1)
        {
            MorphologicalSegmentation morphological_segmentation; //morphological segmentation method
            morphological_segmentation.segmentMap(ROI, segmented_map, goal.map_resolution,goal.room_area_factor_lower_limit_morphological, goal.room_area_factor_upper_limit_morphological,room_contours);
        }
        else if (goal.room_segmentation_algorithm == 2)
        {
            DistanceSegmentation distance_segmentation; //distance segmentation method
            distance_segmentation.segmentMap(ROI, segmented_map, goal.map_resolution, goal.room_area_factor_lower_limit_distance, goal.room_area_factor_upper_limit_distance,room_contours);
        }
        else if (goal.room_segmentation_algorithm == 3)
        {
            VoronoiSegmentation voronoi_segmentation; //voronoi segmentation method
            voronoi_segmentation.segmentMap(ROI, segmented_map,goal.map_resolution,goal.room_area_factor_lower_limit_voronoi, goal.room_area_factor_upper_limit_voronoi,
            goal.voronoi_neighborhood_index,  goal.max_iterations,  goal.min_critical_point_distance_factor,  goal.max_area_for_merging,room_contours,goal,false);
        }
        else
        {
                std::cout<<"room_segmentation_algorithm parameter error"<<std::endl;
        }

    //再次找区域
    std::vector < cv::Vec4i > hierarchy;
    cv::findContours(segmented_map, room_contours, hierarchy, cv::RETR_CCOMP, CV_CHAIN_APPROX_NONE);
    int max=0;
    for (int idx = 0; idx < room_contours.size(); idx++)
    {
        if(cv::contourArea(room_contours[idx])>cv::contourArea(room_contours[max]))
        {
            max=idx;
        }
    }
    cv::swap(room_contours[max],room_contours[room_contours.size()-1]);
    room_contours.pop_back();
    cv::Mat map=cv::Mat::zeros(segmented_map.rows,segmented_map.cols,CV_8UC1);
    cv::Scalar color ;
    color[0]=255;
    for (int idx = 0; idx < room_contours.size(); idx++)
    {
        color[0]=1+idx*1;
        if(!room_contours[idx].empty())
            cv::drawContours(map, room_contours, idx, color, cv::FILLED);
    }
    std::vector < std::vector<cv::Point>> ().swap(room_contours);
    std::vector < cv::Vec4i >().swap(hierarchy);
/*
    if(goal.publish_raw_map==1)
    {
       cv::Mat map_raw=cv::Mat::zeros(segmented_map.rows,segmented_map.cols,CV_8UC3);
        for(int i=0;i<room_contours.size();i++)
        {
            color[0]=255*(rand()%100)/100+10;
            color[1]=255*(rand()%100)/100+10;
            color[2]=255*(rand()%100)/100+10;
            cv::drawContours(map_raw, room_contours, i, color, cv::FILLED);
        }
        cv::imshow("raw", map_raw);
        cv::imwrite("../map/raw.png", map_raw);
    }
    */
    std::vector < std::vector<cv::Point>> door_contours;

    segmented_map.release();


    //房间融合
    RoomSegment::mergeroom.merge_room(map,ROI,goal,door_contours);
    ROI.release();
    //恢复地图
    cv::Rect r1((x_min)/10*10-30,(y_min)/10*10-30,(x_max-x_min)/10*10+60,(y_max-y_min)/10*10+60);
    map.copyTo(processed_map(r1));

    cv::Mat map_door_raw=cv::Mat::zeros(processed_map.rows,processed_map.cols,CV_8UC1);
    for(int i=0;i<door_contours.size();i++)
    {
        cv::Mat map_door=cv::Mat::zeros(map.rows,map.cols,CV_8UC1);
        map_door_raw=cv::Mat::zeros(processed_map.rows,processed_map.cols,CV_8UC1);
        if(!door_contours[i].empty())
            cv::drawContours(map_door, door_contours, i, 255, cv::FILLED);
        map_door.copyTo(map_door_raw(r1));
        cv::findContours(map_door_raw, save_contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_NONE);
        if(save_contours.size()!=0)
        {
                info_.door.push_back(save_contours[0]);
                save_contours.clear();
        }
    }

    //找区域
    for(int i=0;i<255;i++)
    {
        cv::threshold(processed_map,threshold1,i+1,0,cv::THRESH_TOZERO_INV);
        cv::threshold(threshold1,threshold2,i,0,cv::THRESH_TOZERO);
        cv::findContours(threshold2, save_contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_NONE);
        if(save_contours.size()!=0)
        {
                for(int idx=0;idx<save_contours.size();idx++)
                {
                        room_contours.push_back(save_contours[idx]);
                }
                save_contours.clear();
        }
        else
        {
            break;
        }
    }

    //轮廓膨胀
    RoomSegment::mergeroom.expand(processed_map,room_contours,goal.expand_points);

    //存信息
    info_.SegmentMap=processed_map;
    info_.SegNumber=room_contours.size();
    info_.border=room_contours;
/*
    if(goal.publish_color_map==1)
    {
        cv::Mat map_color=cv::Mat::zeros(processed_map.rows,processed_map.cols,CV_8UC3);
        for(int i=0;i<info_.SegNumber;i++)
        {
            color[0]=255*(rand()%100)/100+10;
            color[1]=255*(rand()%100)/100+10;
            color[2]=255*(rand()%100)/100+10;
            cv::drawContours(map_color, info_.border, i, color, cv::FILLED);
        }
        cv::imshow("colour", map_color);
        cv::imwrite("../map/color.png", map_color);
    }
*/
}

bool RoomSegment::SegmentOperation(const SegmentOperationInfo & operationInfo , SegmentInfo& info_)
{
    if(operationInfo.flag==0)
    {
        std::vector<std::vector<cv::Point>>::iterator it= info_.border.begin();
        std::vector<std::vector<cv::Point>>  temp_contours;
        for(int i=0;i<operationInfo.merge.size();i++)
        {
            temp_contours.push_back(info_.border[operationInfo.merge[i]]);
        }
        std::vector<std::vector<cv::Point>>  merge_contours;
        RoomSegment::mergeroom.mergeContours(info_.SegmentMap,temp_contours,merge_contours);
        std::vector<cv::Point>merge_contour=merge_contours[0];
        for(int i=0;i<operationInfo.merge.size();i++)
        {
            info_.border.erase(it+operationInfo.merge[i]-i);
        }
        info_.border.push_back(merge_contours[0]);
        info_.SegNumber=info_.border.size();

        cv::Mat map=cv::Mat::zeros(info_.SegmentMap.rows,info_.SegmentMap.cols,CV_8UC1);
        cv::Scalar color;
        for (int idx = 0; idx < info_.border.size(); idx++)
        {
            color[0]=1+idx*1;
            if(!info_.border[idx].empty())
                cv::drawContours(map, info_.border, idx, color, cv::FILLED);
        }
        info_.SegmentMap=map;
        for(int i=0;i<info_.door.size();i++)
        {
            if(cv::pointPolygonTest(merge_contours[0],info_.door[i][0],false)>0)
            {
                it=info_.door.begin();
                info_.door.erase(it+i);
                break;
            }
        }
        return 0;
    }
    if(operationInfo.flag==1)
    {
        std::vector<std::vector<cv::Point>>::iterator it= info_.border.begin();
        std::vector<std::vector<cv::Point>> segment_contours;
        std::vector<cv::Point>door_contour;
        if(operationInfo.split.second.first.x>info_.SegmentMap.cols||operationInfo.split.second.first.y>info_.SegmentMap.rows||
        operationInfo.split.second.second.x>info_.SegmentMap.cols||operationInfo.split.second.second.y>info_.SegmentMap.rows)
        {
            std::cout<<"Out of range"<<std::endl;
            return 1;
        }
        else if(cv::pointPolygonTest(info_.border[operationInfo.split.first],operationInfo.split.second.first,false)>0||cv::pointPolygonTest(info_.border[operationInfo.split.first],operationInfo.split.second.second,false)>0)
        {
            int max_x=0,min_x=1e6;
            int max_y=0,min_y=1e6;
            for(int i=0;i<info_.border[operationInfo.split.first].size();i++)
            {
                if(info_.border[operationInfo.split.first][i].x>max_x)
                    max_x=info_.border[operationInfo.split.first][i].x;
                if(info_.border[operationInfo.split.first][i].x<min_x)
                    min_x=info_.border[operationInfo.split.first][i].x;
                if(info_.border[operationInfo.split.first][i].y>max_y)
                    max_y=info_.border[operationInfo.split.first][i].y;
                if(info_.border[operationInfo.split.first][i].y<min_y)
                    min_y=info_.border[operationInfo.split.first][i].y;
            }
            if(operationInfo.split.second.first.x!=operationInfo.split.second.second.x)
            {
                int y1 =operationInfo.split.second.second.y-(double(operationInfo.split.second.second.y-operationInfo.split.second.first.y)/(operationInfo.split.second.second.x-operationInfo.split.second.first.x)*(operationInfo.split.second.second.x-max_x));
                int y2 =operationInfo.split.second.second.y-(double(operationInfo.split.second.second.y-operationInfo.split.second.first.y)/(operationInfo.split.second.second.x-operationInfo.split.second.first.x)*(operationInfo.split.second.second.x-min_x));
                RoomSegment::mergeroom.manual_segment(info_.SegmentMap,info_.border[operationInfo.split.first],cv::Point(max_x,y1),cv::Point(min_x,y2),segment_contours,door_contour);
            }
            else
            {
                RoomSegment::mergeroom.manual_segment(info_.SegmentMap,info_.border[operationInfo.split.first],cv::Point(operationInfo.split.second.second.x,min_y),cv::Point(operationInfo.split.second.second.x,max_y),segment_contours,door_contour);
            }
        }
        else
        {
          RoomSegment::mergeroom.manual_segment(info_.SegmentMap,info_.border[operationInfo.split.first],operationInfo.split.second.first,operationInfo.split.second.second,segment_contours,door_contour);
        }
        info_.border.erase(it+operationInfo.split.first);
        for(int i=0;i<segment_contours.size();i++)
        {
            info_.border.push_back(segment_contours[i]);
        }
        info_.door.push_back(door_contour);
        info_.SegNumber=info_.border.size();

        cv::Mat map=cv::Mat::zeros(info_.SegmentMap.rows,info_.SegmentMap.cols,CV_8UC1);
        cv::Scalar color;
        for (int idx = 0; idx < info_.border.size(); idx++)
        {
            color[0]=1+idx*1;
            if(!info_.border[idx].empty())
                cv::drawContours(map, info_.border, idx, color, cv::FILLED);
        }
        info_.SegmentMap=map;

        return 0;
    }
}

bool RoomSegment::LoadInfo(const char * path , SegmentInfo& info_)
{
    std::ifstream bin;
    std::vector < std::vector<cv::Point> > room_contours,door_contours;
    std::vector<cv::Point>room_contour;
    bin.open(path);
    bin.seekg(0 , std::ios::end);  //将指针指向文件的结尾
    int nLen = bin.tellg();   //获取文件的长度
    bin.seekg(0 , std::ios::beg);  //再将指针指向文件的开始，主要是为了读取数据
    char* buffer = new char[nLen];
    bin.read(buffer , nLen);  //读取文件中的内容
    bin.close();
    int j=0,cols=0,rows=0,sum=0,i=-1;
    //读取地图大小
    while(buffer[j]!=32)
    {
        sum=sum*10+buffer[j]-48;
        j++;
    }
    cols=sum;
    sum=0;
    j++;
    while(buffer[j]!=32)
    {
        sum=sum*10+buffer[j]-48;
        j++;
    }
    j++;
    rows=sum;
    //读取轮廓信息
    for(;j<nLen;j++)
    {
        sum=0;
        while(buffer[j]!=32)
        {
            sum=sum*10+buffer[j]-48;
            j++;
        }
        if(sum==i+1)
        {
            if(room_contour.size()!=0)
            {
                room_contours.push_back(room_contour);
            }
           room_contour.clear();
           i++;
        }
        else if(sum==0)
        {
            if(room_contour.size()!=0)
            {
                room_contours.push_back(room_contour);
            }
            room_contour.clear();
            j--;
            break;
        }
        else if(sum!=i+1)
        {
            cv::Point a;
            a.x=sum;
            j++;
            sum=0;
            while(buffer[j]!=32)
            {
                sum=sum*10+buffer[j]-48;
                j++;
            }
            a.y=sum;
            room_contour.push_back(a);
        }
    }
    i=-1;
    for(;j<nLen;j++)
    {
        sum=0;
        while(buffer[j]!=32)
        {
            sum=sum*10+buffer[j]-48;
            j++;
        }
        if(sum!=i+1)
        {
            cv::Point a;
            a.x=sum;
            j++;
            sum=0;
            while(buffer[j]!=32)
            {
                sum=sum*10+buffer[j]-48;
                j++;
            }
            a.y=sum;
            room_contour.push_back(a);
        }
        else
        {
            if(room_contour.size()!=0)
            {
                door_contours.push_back(room_contour);
            }
           room_contour.clear();
           i=i+1;
        }
        if(j==nLen-1)
        {
             if(room_contour.size()!=0)
            {
                door_contours.push_back(room_contour);
            }
            room_contour.clear();
        }
    }
    cv::Scalar color;
	cv::Mat map=cv::Mat::zeros(rows,cols,CV_8UC1);
    for (int idx = 0; idx < room_contours.size(); idx++)
    {
        color[0]=1+idx*1;
        if(!room_contours[idx].empty())
            cv::drawContours(map, room_contours, idx, color, cv::FILLED);
    }

    info_.border=room_contours;
    info_.SegmentMap=map;
    info_.door=door_contours;
    info_.SegNumber=room_contours.size();
    	for(int i=0;i<info_.SegNumber;i++)
	{
		color[0]=255*(rand()%100)/100+10;
		cv::drawContours(map, info_.border, i, color, cv::FILLED);
		cv::imshow("colour", map);
		cv::waitKey();
	}
}
bool RoomSegment::SaveInfo(const char * path , const SegmentInfo& info_)
{
    std::ofstream bin(path,std::ios_base::out | std::ios_base::binary);
    bin<<info_.SegmentMap.cols<<" "<<info_.SegmentMap.rows<<" ";
    for(int i=0;i<info_.border.size();i++)
    {
        bin<<i<<" ";
        for(int j=0;j<info_.border[i].size();j++)
        {
            bin<<info_.border[i][j].x<<" "<<info_.border[i][j].y<<" ";
        }
    }
    for(int i=0;i<info_.door.size();i++)
    {
        bin<<i<<" ";
        for(int j=0;j<info_.door[i].size();j++)
        {
            bin<<info_.door[i][j].x<<" "<<info_.door[i][j].y<<" ";
        }
    }
    bin.close();
}