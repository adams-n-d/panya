/* Test functions for project_IO.hpp
written November 12, 2013
Bob Chien
*/
#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>


system.Drawing;

int TEST_vidAnalyze(void){

    Mat test_vid = imread("/home/bob/vidtrack/led_sample.avi");
    if(test_vid.data == NULL){
        cout << "could not load test image!" << endl;
        exit(1);
    }


    Bitmap img = new Bitmap("/home/bob/vidtrack/led_sample.avi");
     for (int i = 0; i < img.GetWidth; i++)
     {
         for (int j = 0; j < img.GetHeight; j++)
       {
            colorChannel = img.GetPixel(i,j);


             if(i!=Scalar(0,255,0).i || j!=Scalar(0,255,0).j){
            cout << "Can not processing the circleing method!!" << endl;
            exit(1);
        }

    }

}



    //int cntr;
    //for(cntr=0;cntr < numframes; cntr++){
       // cap >> frame;
        //frame << cap;
        //cap.read(frame);

        //if(1) //track method logic goes here
          //  britestPxl = findLED(frame, colorChannel);

        //circle(frame, britestPxl, 5.0, Scalar(0,255,0), -1, 8);

       // outVid << frame;

    //}

/*  cout << "Brightest green pixel: (" << maxLoc.x << ", " << maxLoc.y << ")" << endl;
    cout << CV_MAT_TYPE(frame.type()) << endl;
    cout << frame.depth() << endl;
    cout << frame.channels() << endl;
 */
    if (colorChannel == 1)
    {
    cout << "Test for green pixel capturing: passed" << endl;
    }
    return 0;
}
