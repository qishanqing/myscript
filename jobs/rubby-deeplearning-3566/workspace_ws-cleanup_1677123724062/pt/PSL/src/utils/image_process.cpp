#include "image_process.h"

ImageProcess::PointType g_last, g_first;

// 画由点组成的虚线
void
ImageProcess::DrawDottedLine1(cv::Mat img, PointType p1, PointType p2, cv::Scalar color
                              , int thickness)
{
    float n = 15; //虚点间隔
    float w = p2.x - p1.x, h = p2.y - p1.y;
    float l = sqrtf(w * w + h * h);
    int m = l / n;
    n = l / m; // 矫正虚点间隔，使虚点数为整数

    circle(img, p1, 1, color, thickness); // 画起点
    circle(img, p2, 1, color, thickness); // 画终点
    // 画中间点
    if (p1.y == p2.y) // 水平线：y = m
    {
        float x1 = MIN(p1.x, p2.x);
        float x2 = MAX(p1.x, p2.x);
        for (float x = x1 + n; x < x2; x = x + n)
            circle(img, PointType(x, p1.y), 1, color, thickness);
    }
    else if (p1.x == p2.x) // 垂直线, x = m
    {
        float y1 = MIN(p1.y, p2.y);
        float y2 = MAX(p1.y, p2.y);
        for (float y = y1 + n; y < y2; y = y + n)
            circle(img, PointType(p1.x, y), 1, color, thickness);
    }
    else // 倾斜线，与x轴、y轴都不垂直或平行
    {
        // 直线方程的两点式：(y-y1)/(y2-y1)=(x-x1)/(x2-x1) -> y = (y2-y1)*(x-x1)/(x2-x1)+y1
        float m = n * abs(w) / l;
        float k = h / w;
        float x1 = MIN(p1.x, p2.x);
        float x2 = MAX(p1.x, p2.x);
        for (float x = x1 + m; x < x2; x = x + m)
            circle(img, PointType(x, k * (x - p1.x) + p1.y), 1, color, thickness);
    }
}

// 画由线组成的虚线
void
ImageProcess::DrawDottedLine2(cv::Mat img, PointType p1, PointType p2, cv::Scalar color
                              , int thickness)
{
    float n = 15; //线长度
    float w = p2.x - p1.x, h = p2.y - p1.y;
    float l = sqrtf(w * w + h * h);
    // 矫正线长度，使线个数为奇数
    int m = l / n;
    m = m % 2 ? m : m + 1;
    n = l / m;

    circle(img, p1, 1, color, thickness); // 画起点
    circle(img, p2, 1, color, thickness); // 画终点
    // 画中间点
    if (p1.y == p2.y) //水平线：y = m
    {
        float x1 = MIN(p1.x, p2.x);
        float x2 = MAX(p1.x, p2.x);
        for (float x = x1, n1 = 2 * n; x < x2; x = x + n1)
            line(img, PointType(x, p1.y), PointType(x + n, p1.y), color, thickness);
    }
    else if (p1.x == p2.x) //垂直线, x = m
    {
        float y1 = MIN(p1.y, p2.y);
        float y2 = MAX(p1.y, p2.y);
        for (float y = y1, n1 = 2 * n; y < y2; y = y + n1)
            line(img, PointType(p1.x, y), PointType(p1.x, y + n), color, thickness);
    }
    else // 倾斜线，与x轴、y轴都不垂直或平行
    {
        // 直线方程的两点式：(y-y1)/(y2-y1)=(x-x1)/(x2-x1) -> y = (y2-y1)*(x-x1)/(x2-x1)+y1
        float n1 = n * abs(w) / l;
        float k = h / w;
        float x1 = MIN(p1.x, p2.x);
        float x2 = MAX(p1.x, p2.x);
        for (float x = x1, n2 = 2 * n1; x < x2; x = x + n2)
        {
            cv::Point p3 = PointType(x, k * (x - p1.x) + p1.y);
            cv::Point p4 = PointType(x + n1, k * (x + n1 - p1.x) + p1.y);
            line(img, p3, p4, color, thickness);
        }
    }
}

// 矫正坐标
void ImageProcess::Correct(PointType &p, const PointType &p1, const PointType &p2)
{
    if (p.x < p1.x)
        p.x = p1.x;
    else if (p.x > p2.x)
        p.x = p2.x;
    if (p.y < p1.y)
        p.y = p1.y;
    else if (p.y > p2.y)
        p.y = p2.y;
}

// 鼠标回调函数
void ImageProcess::MouseCallback(int event, int x, int y, int flags, void *param)
{
    static size_t i = 0; // 鼠标操作步骤的标识
    switch (event)
    {
        case cv::EVENT_LBUTTONDOWN: // 鼠标左键点击
            if (i == 0)
            {
                g_first = PointType(x, y);        // 保存线的起点
                g_last = PointType(x + 1, y + 1); // 保存线的终点
                i = 1;
            }
            break;
        case cv::EVENT_MOUSEMOVE: // 鼠标移动
            if (i == 1)
                g_last = PointType(x, y); // 刷新线的终点
            break;
        case cv::EVENT_LBUTTONUP: // 鼠标左键释放
            if (i == 1)
                i = 2; // 不再画线
            break;
        case cv::EVENT_RBUTTONDOWN: // 鼠标右键点击，清除内容
            i = 0;
            g_first = PointType(0, 0);
            g_last = PointType(0, 0);
            break;
        default:
            break;
    }
}

void ImageProcess::DrawDottedRect(cv::Mat img, cv::Rect box
                                  , cv::Scalar color, int thickness)
{
    DrawDottedLine2(img, cv::Point(box.x, box.y), cv::Point(box.x + box.width, box.y)
                    , color, thickness);
    DrawDottedLine2(img, cv::Point(box.x, box.y), cv::Point(box.x, box.y + box.height)
                    , color, thickness);
    DrawDottedLine2(img, cv::Point(box.x + box.width, box.y), cv::Point(box.x + box.width,
            box.y + box.height), color, thickness);
    DrawDottedLine2(img, cv::Point(box.x, box.y + box.height), cv::Point(box.x + box.width
                                                                         ,
            box.y + box.height), color, thickness);
}

bool ImageProcess::GetRatio(const cv::Mat &src, const cv::Size size
                            , float &ratioWidth, float &ratioHeight)
{
    if (src.empty() or src.cols == 0 or src.rows == 0)
    {
        return false;
    }
    if (size.height == 0 or size.width == 0)
    {
        return false;
    }

    ratioWidth = src.cols / float(size.width);
    ratioHeight = src.rows / float(size.height);

    return true;
}

bool ImageProcess::ResizeByPadding(const cv::Mat src, cv::Mat &dst, const cv::Size size)
{
    float ratioWidth = 1.0;
    float ratioHeight = 1.0;
    float ratio = 1.0;
    int widthAdd = 0;
    int heightAdd = 0;
    cv::Scalar value(128);

    if (not(GetRatio(src, size, ratioWidth, ratioHeight)))
    {
        return false;
    }

    bool heightBigger = ratioHeight > ratioWidth;

    if (src.channels() == 3)
    {
        value = cv::Scalar(128, 128, 128);
    }

    if (heightBigger)
    {
        ratio = 1 / ratioHeight;
        widthAdd = size.width - src.cols * ratio;
    }
    else
    {
        ratio = 1 / ratioWidth;
        heightAdd = size.height - src.rows * ratio;

    }

    cv::resize(src, dst, cv::Size(0, 0), ratio, ratio);

    // TODO : split
    copyMakeBorder(dst, dst, (heightAdd + 1) >> 1, (heightAdd + 1) >> 1
                   , (widthAdd + 1) >> 1, (widthAdd + 1) >> 1
                   , cv::BORDER_CONSTANT, cv::Scalar(128, 128, 128));

    return true;
}

bool ImageProcess::LetterBox(const cv::Mat src, cv::Mat &dst, const cv::Size size)
{
    int widthOrg = src.cols;
    int heightOrg = src.rows;

    float ratio = size.width / (float) MAX(widthOrg, heightOrg);

    int widthUnpad = int(widthOrg * ratio);
    int heightUnpad = int(heightOrg * ratio);

    int widthPad = round(((size.width - widthUnpad) % 32) / 2);
    int heightPad = round(((size.height - heightUnpad) % 32) / 2);

    cv::resize(src, dst, cv::Size(widthUnpad, heightUnpad));
    cv::copyMakeBorder(dst, dst, heightPad, heightPad, widthPad, widthPad
                     , cv::BORDER_CONSTANT, cv::Scalar(128, 128, 128));

    return false;
}

//correct(g_last, Point2f(0, 0), Point2f(img.cols, img.rows));
//draw_dotted_line1(img, g_first, g_last, Scalar(0, 255, 0), 2); // 模式1画由点组成的虚线
//draw_dotted_line2(img, g_first, g_last, Scalar(0, 255, 0), 2); // 模式2画由线组成的虚线
