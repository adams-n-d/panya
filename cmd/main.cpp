#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "project_IO.hpp"
#include <vector>
#include <string>
#include "optionparser.h"

using namespace cv;
using namespace std;

typedef vector<string> cmdArgs;

 enum  optionIndex { LED, VIDOUT, UNKNOWN, HELP, PLUS };
 const option::Descriptor usage[] =
 {
    {LED, 0, "l", "led",      option::Arg::Optional, "--led gr\n or --led g\n\n"},
    {VIDOUT ,0, "v", "video", option::Arg::None, "--video, -v \tOutput video file\n"},
  {UNKNOWN, 0,"" , ""    ,option::Arg::None, "USAGE: example [options]\n\n"
                                             "Options:" },
  {HELP,    0,"" , "help",option::Arg::None, "  --help  \tPrint usage and exit." },
  {PLUS,    0,"p", "plus",option::Arg::None, "  --plus, -p  \tIncrement count." },
  {UNKNOWN, 0,"" ,  ""   ,option::Arg::None, "\nExamples:\n"
                                             "  example --unknown -- --this_is_no_option\n"
                                             "  example -unk --plus -ppp file1 file2\n" },
  {0,0,0,0,0,0}
 };

//void parseOpts()

int main(int argc, char* argv[])
 {
     /* OPTION PARSING - in progress******************
     //setup option parsing
   argc-=(argc>0); argv+=(argc>0); // skip program name argv[0] if present
   option::Stats  stats(usage, argc, argv);
   option::Option options[stats.options_max], buffer[stats.buffer_max];
   option::Parser parse(usage, argc, argv, options, buffer);

   if (parse.error())
     return 1;

   if (options[HELP] || argc == 0) {
     option::printUsage(std::cout, usage);
     return 0;
   }
   if (options[VIDOUT]) cout << "video out YES" << endl;
   else cout << "no vid out" << endl;
cout << "v, led, h\t" << VIDOUT << ", " << LED << ", " << HELP << endl;
   cout << "LEDs: " << options[LED].type() << endl;
   cout << "vid out: " << options[VIDOUT].type() <<endl;
if (options[LED]) cout << "LED: " << options[LED].name << endl;
cout <<options[LED].desc << endl;
    //Parse options
   std::cout << "--plus count: " <<
     options[LED].count() << "\n";

   for (option::Option* opt = options[UNKNOWN]; opt; opt = opt->next())
     std::cout << "Unknown option: " << opt->name << "\n";

   for (int i = 0; i < parse.nonOptionsCount(); ++i)
     std::cout << "Non-option #" << i << ": " << parse.nonOption(i) << "\n";

cout << "done w optionparser" << endl;

*/
    cmdArgs Args(&argv[0], &argv[0 + argc]);
cout << argc << endl << argv[0] << endl << argv[1] << Args.size() << endl;

    for (int i = 0; i < Args.size(); ++i)
    {
        cout << Args[i] << endl;
    }
    //need to modularize option parsing (see optionparser.h)
   // exit(0);

    int b = 0;
    int g = 0;
    int r = 0;
    int v = 0;


    int i = 0;
    while(argv[1][i] != NULL){
        if(argv[1][i] == 'b') b = 1;
        if(argv[1][i] == 'g') g = 1;
        if(argv[1][i] == 'r') r = 1;
        if(argv[1][i] == 'v') v = 1;
        if(argv[1][i] == 'h') printHelp();
        ++i;
    }

    if(argc <= 2){
        cout << "Please run using at least one tracking option and one input file" << endl;
        cout << "Run '$ vidtrack -h' for help." << endl;
        return 0;
    }

    int mtds = b+g+r;
    if(mtds == 0){
        cout << "Please run command with at least one of the following options:" << endl;
        cout << "(b, g, r)" << endl;
        return 0;
    }
    int clrs[] = {b, g, r,v};



    int files = 2;
    //when video is analyzed, read a new chunk
    while(files < argc){
    //int retval = vidAnalyze(argv[files], clrs);
    int retval = vidAnalyze(Args[files], clrs);
    if (retval != 0){
        cout << "Something went wrong :/" << endl;
        exit(1);
    }
    files++;
    }

    return 0;
}
