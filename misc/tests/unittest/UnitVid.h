
#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

int TEST_vidAnalyze(void){
    //read the video
    /*VideoCapture cap("/home/bob/vidtrack/LedTestingVideo.avi");
    if(!cap.isOpened()){
        cout << "Cannot open file!!!!!" << endl;
        return -1;
    }*/
    //call the function actual track the bright point
    cout << "hi" <<endl;
    vidAnalyze("/home/bob/vidtrack/vidtrack/3frameledtest.avi");
    cout << "bye" << endl;
    VideoCapture cap("outVid.avi");


    int numframes = cap.get(CV_CAP_PROP_FRAME_COUNT);
    Mat frame;

    Point bright;
    int i;
    for(i=0; i <= 2; ++i){
        cout << "i is " << i << endl;
        cap >> frame;
        bright = findLED(frame, i);

        int x[3] = {0,63,0};
        int y[3] = {0,63,63};
cout << "made int arrays" << endl;
cout << "Pixel: " << bright.x<< ", " << bright.y << endl;


        if (x[i]!=bright.x || y[i]!=bright.y){
            cout << "Brightest pixel in unexpected location!!" << endl;
            exit(1);
        };
    };

    cout << "Test for vidAnalyze: passed" << endl;
    return 0;
}

