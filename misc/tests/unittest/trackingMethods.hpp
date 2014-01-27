
#include <stdio.h>
#include <opencv2/opencv.hpp>



using namespace cv;
using namespace std;

///// distance tracking (11/21/2013)
/*double euclideanDistance(Point A, Point B){





    return 0.0;
}*/


//returns the point of the brightest pixel, on the channel of the passed color
Point findLED(InputArray _imgFrame, int colorChnl){

    Mat imgFrame = _imgFrame.getMat();

    double minVal; double maxVal; Point minLoc; Point maxLoc;
    Point pt;

/* //measures by total intensity not individual color channels...wrong datatype here, doesn't work
    Vec3b intensity = frame.at<Vec3b>(0, 0);
    uchar blue = intensity.val[0];
    uchar green = intensity.val[1];
    uchar red = intensity.val[2];
*/

    //separate image into color channels
    vector<Mat> spl;
    split(imgFrame, spl);
    /*cout << "got here" << endl;*/
    minMaxLoc( spl[ colorChnl ], &minVal, &maxVal, &minLoc, &maxLoc);
/*cout << "not here" <<endl;*/

    return maxLoc;
}
