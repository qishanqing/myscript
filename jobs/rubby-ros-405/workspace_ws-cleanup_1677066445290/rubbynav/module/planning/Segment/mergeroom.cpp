#include <mergeroom.h>

void MergeRoom::merge_room(cv::Mat&Map,cv::Mat Map1,MapSegmentationGoal &goal,std::vector < std::vector<cv::Point>>&door)
{
    cv::Mat  threshold1,threshold2;
    std::vector < std::vector<cv::Point>> room_contours,save_contours,max_contour,boundary_contours,door_contours; 
    std::vector < cv::Vec4i > hierarchy;
    std::vector<cv::Point> room_contour;
    cv::threshold(Map1,threshold1,80,255,cv::THRESH_BINARY);
    cv::findContours(threshold1, max_contour, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);

    for(int i=0;i<255;i++)
    {
        
        cv::threshold(Map,threshold1,i+1,0,cv::THRESH_TOZERO_INV);
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
//只保留与外轮廓相同的点，修正区域
    std::vector<cv::Point> ::iterator reasult;
    for(int i=0;i<room_contours.size();i++)
    {
        for(int j=0;j<room_contours[i].size();j++)
        {
            reasult= find(max_contour[0].begin(),max_contour[0].end(),room_contours[i][j]);
            if(reasult!=max_contour[0].end())
            {
                room_contour.push_back(room_contours[i][j]);
            }
        }
        save_contours.push_back(room_contour);
        room_contour.clear();
    }      
    room_contours.clear();
    correct(Map1,save_contours,room_contours);
    cv::threshold(Map1,threshold1,80,255,cv::THRESH_BINARY);
    save_contours=room_contours;
    std::vector<cv::Point> door_contour;

    //找出外轮廓，每个轮廓去除与外轮廓相同的点，即为门
    for(int i=0;i<room_contours.size();i++)
    {
        for(int j=0;j<room_contours[i].size();j++)
        {
            reasult= find(max_contour[0].begin(),max_contour[0].end(),room_contours[i][j]);
            if(reasult==max_contour[0].end())
            {
                door_contour.push_back(room_contours[i][j]);
            }
        }
        boundary_contours.push_back(door_contour);
        door_contour.clear();
    }

    std::vector < std::vector<cv::Point>> ().swap(max_contour);

    for(int i=0;i<boundary_contours.size();i++)
    {
        for(int j=0;j<boundary_contours[i].size();j++)
        {
            if(j==0)
            {
                door_contour.push_back(boundary_contours[i][j]);
            }
            else if(abs(boundary_contours[i][j].x-boundary_contours[i][j-1].x)>=2||abs(boundary_contours[i][j].y-boundary_contours[i][j-1].y)>=2)
            {
                door_contours.push_back(door_contour);
                door_contour.clear();
            }
        door_contour.push_back(boundary_contours[i][j]);
        }
        if(door_contour.size()!=0)
        {
            door_contours.push_back(door_contour);
            door_contour.clear();
        }
    }
    std::vector<cv::Point> door_centre,boundary_centre;

    for(int i=0;i<door_contours.size();i++)
    {
        int sum_x=0,sum_y=0;
        for(int j=0;j<door_contours[i].size();j++)
        {
            sum_x=sum_x+door_contours[i][j].x;
            sum_y=sum_y+door_contours[i][j].y;
            if(j==door_contours[i].size()-1)
            {
                cv::Point centre;
                centre.x=sum_x/(j+1);
                centre.y=sum_y/(j+1);
                door_centre.push_back(centre);
            }
        }
    }
    boundary_contours.clear();
    for(int i=0;i<door_contours.size();i++)
    {
        boundary_contours.push_back(door_contours[i]);
        boundary_centre.push_back(door_centre[i]);
        for(int j=0;j<boundary_contours.size()-1;j++)
        {
        if(abs(door_centre[i].x-boundary_centre[j].x)<=10&&abs(door_centre[i].y-boundary_centre[j].y)<=10)
            {
                if(cv::arcLength(boundary_contours[j],false)>arcLength(door_contours[i],false))
                {
                    boundary_contours.pop_back();
                    boundary_centre.pop_back();
                }
                else
                {
                    cv::swap(boundary_contours[j],boundary_contours[boundary_contours.size()-1]);
                    boundary_contours.pop_back();
                    boundary_centre.pop_back();
                }
            }
        }
    }
    door_contours=boundary_contours;
    std::vector < std::vector<cv::Point>> ().swap(boundary_contours);

    std::vector<int > door_max_x,door_max_y,door_min_x,door_min_y;
    for(int i=0;i<door_contours.size();i++)
    {
        int max_x=0,min_x=10000,max_y=0,min_y=10000;
        for(int j=0;j<door_contours[i].size();j++)
        {
            if(door_contours[i][j].x>max_x)
                max_x=door_contours[i][j].x;
            if(door_contours[i][j].x<min_x)
            min_x=door_contours[i][j].x;
            if(door_contours[i][j].y>max_y)
            max_y=door_contours[i][j].y;
            if(door_contours[i][j].y<min_y)
            min_y=door_contours[i][j].y;
        }
        door_max_x.push_back(max_x);
        door_min_x.push_back(min_x);
        door_max_y.push_back(max_y);
        door_min_y.push_back(min_y);
    }

    std::vector<int> door_left,door_right;
    for(int i=0;i<door_contours.size();i++)
    {
        cv::Point left_text,right_text;
        for(int j=0;j<save_contours.size();j++)
        {    
            if(abs(door_max_x[i]-door_min_x[i])>abs(door_max_y[i]-door_min_y[i]))
                {
                    left_text.x=door_contours[i][3].x;
                    left_text.y=door_contours[i][3].y-3;
                    right_text.x=door_contours[i][3].x;
                    right_text.y=door_contours[i][3].y+3;
                }
            else
            {
                left_text.x=door_contours[i][3].x-3;
                left_text.y=door_contours[i][3].y;
                right_text.x=door_contours[i][3].x+3;
                right_text.y=door_contours[i][3].y;
            }
            if(cv::pointPolygonTest(save_contours[j],left_text,false)>0)
            {
                door_left.push_back(j);
            }
            if(cv::pointPolygonTest(save_contours[j],right_text,false)>0)
            {
                door_right.push_back(j);
            }
            if(door_left.size()==i+1&&door_right.size()==i+1)
            {
                break;
            }
        }
    }

    std::vector<std::vector<cv::Point>>::iterator it=door_contours.begin();
    for(int i=0;i<door_contours.size();i++)
    {
        bool  corridor_config=true;
        if(goal.corridor_separate==true)
        {
            corridor_config=((cv::contourArea(save_contours[door_left[i]])+cv::contourArea(save_contours[door_right[i]]))*goal.map_resolution*goal.map_resolution)<goal.corridor_maxarea;
        }
        if((cv::arcLength(door_contours[i],false)+1)*goal.map_resolution>goal.corridor_width_min&&(cv::arcLength(door_contours[i],false)+1)*goal.map_resolution<goal.corridor_width_max&&corridor_config)
        {
            std::vector<std::vector<cv::Point>>  merge_contour;
            mergeContours(Map,save_contours[door_left[i]],save_contours[door_right[i]],merge_contour);
            std::vector<std::vector<cv::Point>>::iterator it_left=save_contours.begin()+door_left[i];
            std::vector<std::vector<cv::Point>>::iterator it_right=save_contours.begin()+door_right[i];
            int left=door_left[i];
            int right=door_right[i];
            door_left.erase(door_left.begin()+i);
            door_right.erase(door_right.begin()+i);
            for(int j=0;j<door_left.size();j++)
            {
                if(door_left[j]==left ||door_left[j]==right)
                {
                    door_left[j]=save_contours.size();
                }
                if(door_right[j]==left ||door_right[j]==right)
                {
                    door_right[j]=save_contours.size();
                }
            }
            save_contours.erase(it_left);
            for(int j=0;j<door_left.size();j++)
            {
                if(door_left[j]>left)
                {
                    door_left[j]--;
                }
                if(door_right[j]>left)
                {
                    door_right[j]--;
                }
            }
            if(it_right>it_left)
            {
                it_right--;
                right--;
            }
            save_contours.erase(it_right);
            for(int j=0;j<door_left.size();j++)
            {
                if(door_left[j]>right)
                {
                    door_left[j]--;
                }
                if(door_right[j]>right)
                {
                    door_right[j]--;
               }
            }
            save_contours.push_back(merge_contour[0]);
            door_contours.erase(it+i);
            it=door_contours.begin();
            i--;
        }
    }

    for(int i=0;i<door_contours.size();i++)
    {
        if((cv::arcLength(door_contours[i],false)+1)*goal.map_resolution>goal.roomdoor_min&&(cv::arcLength(door_contours[i],false)+1)*goal.map_resolution<goal.roomdoor_max)
        {         
            double area=(cv::contourArea(save_contours[door_left[i]],false)+cv::contourArea(save_contours[door_right[i]],false));
            double max_area=std::max(cv::contourArea(save_contours[door_left[i]],false),cv::contourArea(save_contours[door_right[i]],false));
            area=area*goal.map_resolution*goal.map_resolution;
            max_area=max_area*goal.map_resolution*goal.map_resolution;
            if(area<goal.mergeroom_maxarea&&area>goal.mergeroom_minarea&&max_area>goal.roomarea_max)
            {
                std::vector<std::vector<cv::Point>>  merge_contour;
                mergeContours(Map,save_contours[door_left[i]],save_contours[door_right[i]],merge_contour);
                std::vector<std::vector<cv::Point>>::iterator it_left=save_contours.begin()+door_left[i];
                std::vector<std::vector<cv::Point>>::iterator it_right=save_contours.begin()+door_right[i];
                int left=door_left[i]; 
                int right=door_right[i];
                door_left.erase(door_left.begin()+i);
                door_right.erase(door_right.begin()+i);
                for(int j=0;j<door_left.size();j++)
                {
                    if(door_left[j]==left ||door_left[j]==right)
                    {
                        door_left[j]=save_contours.size();
                    }
                    if(door_right[j]==left ||door_right[j]==right)
                    {
                        door_right[j]=save_contours.size();
                    }
                }
                save_contours.erase(it_left);
                for(int j=0;j<door_left.size();j++)
                {
                    if(door_left[j]>left)
                    {
                        door_left[j]--;
                    }
                    if(door_right[j]>left)
                    {
                        door_right[j]--;
                    }
                }
                if(it_right>it_left)
                {
                    it_right--;
                    right--;
                }
                save_contours.erase(it_right);
                for(int j=0;j<door_left.size();j++)
                {
                    if(door_left[j]>right)
                    {
                        door_left[j]--;
                    }
                    if(door_right[j]>right)
                    {
                        door_right[j]--;
                    }
                }
                save_contours.push_back(merge_contour[0]);
                door_contours.erase(it+i);
                it=door_contours.begin();
                i--;
            }
        }
    }    

//门修正
    door_max_x.clear();
    door_min_x.clear();
    door_max_y.clear();
    door_min_y.clear();
        for(int i=0;i<door_contours.size();i++)
    {
        int max_x=0,min_x=10000,max_y=0,min_y=10000;
        for(int j=0;j<door_contours[i].size();j++)
        {
            if(door_contours[i][j].x>max_x)
                max_x=door_contours[i][j].x;
            if(door_contours[i][j].x<min_x)
            min_x=door_contours[i][j].x;
            if(door_contours[i][j].y>max_y)
            max_y=door_contours[i][j].y;
            if(door_contours[i][j].y<min_y)
            min_y=door_contours[i][j].y;
        }
        door_max_x.push_back(max_x);
        door_min_x.push_back(min_x);
        door_max_y.push_back(max_y);
        door_min_y.push_back(min_y);
    }
for(int i=0;i<door_contours.size();i++)
{
    if(door_max_x[i]-door_min_x[i]>door_max_y[i]-door_min_y[i])
    {
        door_contours[i]=horizontal(door_contours[i],threshold1);//水平
    }
    else
    {
        door_contours[i]=vertical(door_contours[i],threshold1);//水平
    }
}

for(int i=0;i<door_contours.size();i++)
{
        for(int j=0;j<door_contours[i].size();j++)
        {
            threshold1.at<uchar>(door_contours[i][j].y,door_contours[i][j].x)=0;      
        }
}      
cv::findContours(threshold1, save_contours, hierarchy, cv::RETR_CCOMP, CV_CHAIN_APPROX_NONE);
for (int i = 0; i < save_contours.size(); i++)
{
    if(hierarchy[i][3]!=-1)
    {
        save_contours=room_contours;
    }
}
room_contours=save_contours;

    cv::Mat map=cv::Mat::zeros(Map.rows,Map.cols,CV_8UC1);
    cv::Scalar color ;
    color[0]=255;
    for (int idx = 0; idx < room_contours.size(); idx++)
    {
        color[0]=1+idx*1;
        if(room_contours[idx].size()!=0)
            cv::drawContours(map, room_contours, idx, color, cv::FILLED);
    }
    Map=map;
    door=door_contours;
}
//融合两个区域
void MergeRoom::mergeContours(cv::Mat Map,std::vector<cv::Point> contour_a,std::vector<cv::Point> contour_b,std::vector < std::vector<cv::Point> >& room_contours)
{
    std::vector < cv::Vec4i > hierarchy;
    room_contours.push_back(contour_a);
    room_contours.push_back(contour_b);
    cv::Mat map=cv::Mat::zeros(Map.rows,Map.cols,CV_8UC1);
    cv::Scalar color ;
    color[0]=255;
    cv::drawContours(map, room_contours, -1, color, cv::FILLED);
    cv::findContours(map, room_contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
}
//融合多个区域
void MergeRoom::mergeContours(cv::Mat Map,std::vector < std::vector<cv::Point> > contours,std::vector<std::vector<cv::Point>>&room_contours)
{
    std::vector < cv::Vec4i > hierarchy;
    cv::Mat dilate_map,map=cv::Mat::zeros(Map.rows,Map.cols,CV_8UC1);
    cv::Scalar color ;
    color[0]=255;
    cv::drawContours(map, contours, -1, color, cv::FILLED);
    cv::Point anchor(-1, -1);
    cv::Mat  elem=getStructuringElement(cv::MORPH_RECT,cv::Size(3,3));
    cv::dilate(map,dilate_map,elem, anchor, 1);
    cv::erode(dilate_map, map, elem, anchor, 1);
    cv::findContours(map, room_contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
}
double MergeRoom::room_area(cv::Mat Map,std::vector<cv::Point> room,MapSegmentationGoal &goal)
{
    std::vector < std::vector<cv::Point> > room_contours;
    double area;
    room_contours.push_back(room);
    cv::Mat map=cv::Mat::zeros(Map.rows,Map.cols,CV_8UC1);
    if(room_contours[0].size()!=0)
        cv::drawContours(map, room_contours, 0, cv::Scalar(255), cv::FILLED);
    area=cv::countNonZero(map)*goal.map_resolution*goal.map_resolution;
    return area;
}
void MergeRoom::correct(cv::Mat Map1, std::vector < std::vector<cv::Point>>save_contours, std::vector < std::vector<cv::Point>>&correct_contours)
{
    std::vector < std::vector<cv::Point>>temp_contours;
    std::vector < cv::Vec4i > hierarchy;
    for(int i=0;i<save_contours.size();i++)
    {
        cv::Mat map=cv::Mat::zeros(Map1.rows,Map1.cols,CV_8UC1);
        cv::Scalar color ;
        color[0]=255;
        if(save_contours[i].size()!=0)
        {
            cv::drawContours(map, save_contours, i, color, cv::FILLED);
            cv::findContours(map, temp_contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
            correct_contours.push_back(temp_contours[0]);
            temp_contours.clear();
        }
    }
}

std::vector<cv::Point>MergeRoom::horizontal(std::vector<cv::Point> door_contour,cv::Mat threshold1)
{
    std::vector<cv::Point>horizontal_contour;
    int p_x,p_y;
    int j=-1;
    int min_count=door_contour.size()+6;
    int count=1;
    bool flag;
    for(int i=0;i<door_contour.size();i++)
    {
        count=1;
        flag=1;
        p_x=door_contour[i].x;
        p_y=door_contour[i].y;
        while(flag)
        {
            p_x++;
            if(threshold1.at<uchar>(p_y,p_x)==0)
            {
                flag=0;
            } 
            else
            {
                count++;
            }
        }
        p_x=door_contour[i].x;
        p_y=door_contour[i].y;
        flag=1;
        while(flag)
        {
             p_x--;
            if(threshold1.at<uchar>(p_y,p_x)==0)
            {
                flag=0;
            } 
            else
            {
                count++;
            }
        }
        if(count<min_count)
        {
            min_count=count;
            j=i;
        }
    }
    if(j<0)
    {
        return door_contour;
    }
    p_x=door_contour[j].x;
    p_y=door_contour[j].y;
    while (threshold1.at<uchar>(p_y,p_x)!=0)
    {
        p_x++;
    }
    p_x--;
    while (threshold1.at<uchar>(p_y,p_x)!=0)
    {
        horizontal_contour.push_back(cv::Point(p_x,p_y));
        p_x--;
    }
    return horizontal_contour;
}
std::vector<cv::Point>MergeRoom::vertical(std::vector<cv::Point> door_contour,cv::Mat threshold1)
{
    std::vector<cv::Point>vertical_contour;
    int p_x,p_y;
    int j=-1;
    int min_count=door_contour.size()+6;
    int count=1;
    bool flag;
    for(int i=0;i<door_contour.size();i++)
    {
        count=1;
        flag=1;
        p_x=door_contour[i].x;
        p_y=door_contour[i].y;
        while(flag)
        {
            p_y++;
            if(threshold1.at<uchar>(p_y,p_x)==0)
            {
                flag=0;
            } 
            else
            {
                count++;
            }
        }
        p_x=door_contour[i].x;
        p_y=door_contour[i].y;
        flag=1;
        while(flag)
        {
             p_y--;
            if(threshold1.at<uchar>(p_y,p_x)==0)
            {
                flag=0;
            } 
            else
            {
                count++;
            }
        }
        if(count<min_count)
        {
            min_count=count;
            j=i;
        }
    }
    if(j<0)
    {
        return door_contour;
    }
    p_x=door_contour[j].x;
    p_y=door_contour[j].y;
    while (threshold1.at<uchar>(p_y,p_x)!=0)
    {
        p_y++;
    }
    p_y--;
    while (threshold1.at<uchar>(p_y,p_x)!=0)
    {
        vertical_contour.push_back(cv::Point(p_x,p_y));
        p_y--;
    }
    return vertical_contour;
}

void MergeRoom::expand(cv::Mat map,std::vector<std::vector<cv::Point>>&room_contours,int n)
{
    cv::Mat map1,dilate_map;
    std::vector<std::vector<cv::Point>>expand_contours,temp_contours;
    std::vector < cv::Vec4i > hierarchy;
    for(int i=0;i<room_contours.size();i++)
    {
        map1=cv::Mat::zeros(map.rows,map.cols,CV_8UC1);
        if(room_contours[i].size()!=0)
            cv::drawContours(map1, room_contours, i, cv::Scalar(255), cv::FILLED);
        cv::Point anchor(-1, -1);
        cv::Mat  elem=getStructuringElement(cv::MORPH_RECT,cv::Size(2*n+1,2*n+1));
        cv::dilate(map1,dilate_map,elem, anchor, 1);
        cv::findContours(dilate_map, temp_contours, hierarchy, cv::RETR_CCOMP, CV_CHAIN_APPROX_NONE);
        expand_contours.push_back(temp_contours[0]);
        temp_contours.clear();
    }
    room_contours=expand_contours;
}
void MergeRoom::manual_segment(cv::Mat Map,std::vector<cv::Point> contour,cv::Point point_a,cv::Point point_b,std::vector<std::vector<cv::Point>>&manual_contours,std::vector<cv::Point>&door_contour)
{
    std::vector<std::vector<cv::Point>> temp_contours;
    std::vector < cv::Vec4i > hierarchy;
    temp_contours.push_back(contour);
    cv::Mat map=cv::Mat::zeros(Map.rows,Map.cols,CV_8UC1);
    cv::line(map,point_a,point_b,cv::Scalar(255),1,4);
    cv::findContours(map, manual_contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
    door_contour=manual_contours[0];
    manual_contours.clear();
    map=cv::Mat::zeros(Map.rows,Map.cols,CV_8UC1);
    if(temp_contours[0].size()!=0)
        cv::drawContours(map, temp_contours, 0, cv::Scalar(255), cv::FILLED);
    cv::line(map,point_a,point_b,cv::Scalar(0),1,4);
    cv::findContours(map, manual_contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
}