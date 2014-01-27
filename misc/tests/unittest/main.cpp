#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "project_IO.hpp"
#include "unittest++.h"
#include "UnitVid.h"

using namespace cv;
using namespace std;




int main(int argc, char** argv){

    // write code to test projectIO.hpp
    /*Test(FailSpectacularly)
    {
        CHECK(false);
    }*/

    int err;
    err = TEST_findLED();
    assert(err==0);
cout<< " findLED passed" << endl;


    err = TEST_vidAnalyze();
    assert(err==0);

    cout<<err<<endl;

// use assert() functions to make sure the logic works
    return 0;
}
