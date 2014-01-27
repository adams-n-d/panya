/***************************************************************
 * Name:      vidtrackguiApp.cpp
 * Purpose:   Code for Application Class
 * Author:    xhong ()
 * Created:   2013-11-15
 * Copyright: xhong ()
 * License:
 **************************************************************/

#define _U(x) wxString((x), wxConvUTF8)

#ifdef WX_PRECOMP
#include "wx_pch.h"
#endif

#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__

#include "vidtrackguiApp.h"
#include "vidtrackguiMain.h"

IMPLEMENT_APP(vidtrackguiApp);

bool vidtrackguiApp::OnInit()
{

    vidtrackguiFrame* frame = new vidtrackguiFrame(_U("VidTracker-Emory Univ.-Dept. of Neuroscience"),wxPoint(50,50),wxSize(400,400));

    frame->Show(true);  //make the frame visible
    return true;  //indicate a successful initialization
}
