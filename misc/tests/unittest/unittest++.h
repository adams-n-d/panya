/* Test functions for vidtrack tracking.method
written November 10, 2013
Bob Chien and Nathan Adams
*/


#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>



int TEST_findLED(void){

    Mat test_img = imread("/home/bob/vidtrack/vidtrack/findLEDtest.png");
    //Mat test_img = imread("/home/bob/Downloads/opencv-2.4.6.1/samples/c/puzzle.png");
    if(test_img.data == NULL){
        cout << "could not load test image!" << endl;
        exit(1);
    }

    //For our test image, the brightest pixel for all three channels is at (0, 0)
    int x = 0;
    int y = 0;

    Point bright;
    int i;
    for(i=0; i <= 2; ++i){
        bright = findLED(test_img, i);
        //cout << "x, y coords for i = " << i;
        //cout << "\t" << bright.x<< ", " << bright.y << endl;


        if(x!=bright.x || y!=bright.y){
            cout << "Brightest pixel in unexpected location!!" << endl;
            exit(1);
        }
    }


    cout << "Test for findLED: passed" << endl;
    return 0;
}

