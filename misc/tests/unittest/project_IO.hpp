#include <stdio.h>
#include <opencv2/opencv.hpp>
#include "trackingMethods.hpp"


using namespace cv;
using namespace std;

/*reads a video file, marks the pixel coordinates and writes to file*/

int vidAnalyze(const char* filename){
    VideoCapture cap(filename);
    //equivalent to:
    //VideoCapture cap = open(filename);

    int numframes = cap.get(CV_CAP_PROP_FRAME_COUNT);

    if(!cap.isOpened()){
        cout << "Cannot open file!!!!!" << endl;
        return -1;
    }

    Size S = Size((int) cap.get(CV_CAP_PROP_FRAME_WIDTH),    // Acquire input size
                  (int) cap.get(CV_CAP_PROP_FRAME_HEIGHT));

    VideoWriter outVid("outVid.avi", cap.get(CV_CAP_PROP_FOURCC),cap.get(CV_CAP_PROP_FPS), S);

    Point greenPxl;
    Point redPxl;
    Mat frame; //Holds a single frame of video

//channels: blue == 0, green == 1, red == 2
    int green = 1;
    int red = 2;

    cout << "greenX, greenY, redX, redY" << endl;

    int cntr;
    for(cntr=0;cntr < numframes; cntr++){
        cout << "frame: " << cntr << endl;
        cap >> frame;
        //frame << cap;
        //cap.read(frame);

        if(1){ //track method logic goes here
            greenPxl = findLED(frame, green);
            redPxl = findLED(frame, red);
            //cout << greenPxl.x << "," << greenPxl.y << "," << redPxl.x << "," redPxl.y << endl;
        }
        circle(frame, greenPxl, 5.0, Scalar(0,255,0), -1, 8);
        circle(frame, redPxl, 5.0, Scalar(0,0,255), -1, 8);

        outVid << frame;

    }


//iterate through GrnPnt, Redpnt vector and find distances, ID if genuine or false
    /*distance function goes here */
   /* int numframes = cap.get(CV_CAP_PROP_FRAME_COUNT);
    greenPxl = findLED(frame, green);

    Point a= numframes.greenPxl(x1.y1)
    Point b= (numframes-1).greenPxl(x2.y2)

   float EucliDis(Point& a , point& b){


    Point distance = a - b;

    return cv::sqrt(distance.x1*distance.x2 + distance.y1*distance.y2);


    }*/



    //get each green point and red point from each frame

    /*vector<Point> GreenPxl;
    vector<Point> RedPxl;*/



    // count the distance in each frame
    /*for (i=1, i< numframes, i++){




    }*/





/*  cout << "Brightest green pixel: (" << maxLoc.x << ", " << maxLoc.y << ")" << endl;
    cout << CV_MAT_TYPE(frame.type()) << endl;
    cout << frame.depth() << endl;
    cout << frame.channels() << endl;
 */


    return 0;
}
