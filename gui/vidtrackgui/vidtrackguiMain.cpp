/***************************************************************
 * Name:      vidtrackguiMain.cpp
 * Purpose:   Code for Application Frame
 * Author:    xhong ()
 * Created:   2013-11-15
 * Copyright: xhong ()
 * License:
 **************************************************************/
#include <stdio.h>

#define _U(x) wxString((x), wxConvUTF8)
#define _CU(x) (const_cast<char*>((const char*) x.mb_str()))

#ifdef WX_PRECOMP
#include "wx_pch.h"
#endif

#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__

#include "vidtrackguiMain.h"
#include <wx/statline.h>
#include <wx/stattext.h>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "../../vidtrack/project_IO.hpp"


using namespace std;
//using namespace cv;

int testfunction(/*wxGauge *ProBar, */vidtrackguiFrame *vid)
{

    for (int i=1; i<=10;i++){
        sleep(1);
        vid->ProBar-> SetValue(i*10);
        vid->Update();
    }
    wxMessageBox(_U("Xia said: testfunction for START is working."), _("Xia: testfuncion"),wxOK | wxICON_INFORMATION);
    return 0;
}

int analyzeWrapper(vidtrackguiFrame *gui)
{
    int t[4] = {0, 1, 1, 0};
    //(const_cast<char*>((const char*)mystring.mb_str()));
    //int retval = vidAnalyze(gui->StaticTextfp->GetLabel().c_str(), gui);
    int retval = vidAnalyze(   (const_cast<char*>((const char*)          gui->StaticTextfp->GetLabel().c_str()   ))        , t);
    if (retval != 0){
        // TODO: put error dialog
        cout << "Something went wrong :/" << endl;
    }
    wxString msg= _U("Tracking is done.");
    wxLogMessage(msg);
}

//helper functions
enum wxbuildinfoformat {
    short_f, long_f };

wxString wxbuildinfo(wxbuildinfoformat format)
{
    wxString wxbuild(wxVERSION_STRING);

    if (format == long_f )
    {
#if defined(__WXMSW__)
        wxbuild << _T("-Windows");
#elif defined(__WXMAC__)
        wxbuild << _T("-Mac");
#elif defined(__UNIX__)
        wxbuild << _T("-Linux");
#endif

#if wxUSE_UNICODE
        wxbuild << _T("-Unicode build");
#else
        wxbuild << _T("-ANSI build");
#endif // wxUSE_UNICODE
    }

    return wxbuild;
}

BEGIN_EVENT_TABLE(vidtrackguiFrame, wxFrame)  //event table: list of all events
    EVT_MENU(wxID_EXIT, vidtrackguiFrame::OnExit)
    EVT_MENU(wxID_ABOUT, vidtrackguiFrame::OnAbout)
    EVT_MENU(idOpen, vidtrackguiFrame::OnOpen)
    EVT_MENU(idOpenRecent, vidtrackguiFrame::OnOpenRecent)
    EVT_BUTTON(idBtnStart, vidtrackguiFrame::OnStart)
    EVT_BUTTON(idBtnStop, vidtrackguiFrame::OnStop)
    EVT_CHECKBOX(idLEDRedCB, vidtrackguiFrame::OnLEDRedCB)
    EVT_CHECKBOX(idLEDGreenCB, vidtrackguiFrame::OnLEDGreenCB)
    EVT_CHECKBOX(idLEDBlueCB, vidtrackguiFrame::OnLEDBlueCB)
END_EVENT_TABLE()

vidtrackguiFrame::vidtrackguiFrame(const wxString &title, const wxPoint& pos, const wxSize& size)
    : wxFrame(NULL,wxID_ANY,title,pos, size)  // the main frame and layout of all the objects inside
{
    //-------------------- menu settings ------------------
    // menu File
    wxMenu *menuFile = new wxMenu;
    // menu File -> Open
    menuFile->Append(idOpen, "&Open...\tCtrl-O",
                     ("Choose a video file to open."));
    // menu File -> Open Recent
    wxMenu *menuOpenRecent = new wxMenu;
    // menuOpenRecent->Append(id, wxT(""));
    menuFile->AppendSubMenu(menuOpenRecent,"&Open Recent\tCtrl-R",
                     "Choose a file from recently opened files.");
    menuFile->AppendSeparator();
    // menu File -> Quit
    menuFile->Append(wxID_EXIT);
    // menu Help
    wxMenu *menuHelp = new wxMenu;
    // menu Help -> About
    menuHelp->Append(wxID_ABOUT);

    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append( menuFile, "&File" );
    menuBar->Append( menuHelp, "&Help" );
    SetMenuBar( menuBar );  //tell the frame to show the menu bar

    CreateStatusBar();  //Status bar on the bottom
    SetStatusText( "Welcome to Video Tracker - Emory" );

    Centre(); //center the frame

    wxBoxSizer *bSizer1 = new wxBoxSizer(wxVERTICAL);  //bigger one
    wxBoxSizer *bSizer2 = new wxBoxSizer(wxHORIZONTAL); //inside bSizer1
    // ---------------- checkbox settings, for tracking method -------------------
    wxStaticText *StaticTextcm = new wxStaticText(this, wxID_ANY, wxT("Choose a method:"), wxDefaultPosition, wxSize(150,7), 0);
    StaticTextcm-> SetFont(wxFont(12, 74, 90, 70, false, wxT("Arial")));
    wxCheckBox *LEDRedCB = new wxCheckBox(this, idLEDRedCB, wxT("LED Red"), wxDefaultPosition,wxSize(100,7));
    wxCheckBox *LEDGreenCB = new wxCheckBox(this, idLEDGreenCB, wxT("LED Green"), wxDefaultPosition,wxSize(100,7));
    wxCheckBox *LEDBlueCB = new wxCheckBox(this, idLEDBlueCB, wxT("LED Blue"), wxDefaultPosition,wxSize(100,7));
    wxStaticLine *StaticLine1 = new wxStaticLine(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL);

    // ---------------- progress bar settings -------------------
    ProBar = new wxGauge(this,idProBar, 100,wxPoint(20,15),wxSize(200,30),wxGA_HORIZONTAL);
    //ProBar-> SetValue(50);  //ProBar->GetValue();

    // ---------------- button settings -------------------
    wxButton* BtnStart = new wxButton(this, idBtnStart,wxT("Start"),wxPoint(5,10),wxSize(50,30));
    wxButton* BtnStop = new wxButton(this, idBtnStop,wxT("Stop"),wxPoint(10,15),wxSize(50,30));
    //----------------- showing information (filename, method..)-------------

    wxStaticText *StaticTextfn = new wxStaticText(this, wxID_ANY, "File: ", wxDefaultPosition, wxDefaultSize, 0);
    StaticTextfp = new wxStaticText(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, 0);
    wxStaticLine *StaticLine2 = new wxStaticLine(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL);

       // add all the objects above
    bSizer2->Add(BtnStart, 1, wxALL, 5);
    bSizer2->Add(BtnStop, 1, wxALL, 5);
    bSizer1->Add(StaticTextcm, 1, wxALL, 5);
    bSizer1->Add(LEDRedCB,1,wxALL|wxEXPAND,5);
    bSizer1->Add(LEDGreenCB,1,wxLEFT,5);
    bSizer1->Add(LEDBlueCB,1,wxALL,5);
    bSizer1->Add(StaticLine1, 0, wxALL|wxEXPAND, 5);
    bSizer1->Add(StaticTextfn, 1, wxALL, 5);
    bSizer1->Add(StaticTextfp, 1, wxALL, 5);
    bSizer1->Add(StaticLine2, 0, wxALL|wxEXPAND, 5);
    bSizer1->Add(ProBar,1,wxALL,5);
    bSizer1->Add(bSizer2,1,wxALL,5);
    this->SetSizer(bSizer1);
    //this->Layout();
}


vidtrackguiFrame::~vidtrackguiFrame()
{
}

void vidtrackguiFrame::OnExit(wxCommandEvent &event)
{
    Close(true); // use parameter 'true': no veto poewr such as ask 'do you really want to close?'
    //Destroy;
}

void vidtrackguiFrame::OnAbout(wxCommandEvent &event)
{
    wxString msg="This is a rat tracking software. Please see help documentation.";
    wxMessageBox(msg, _("Welcome!"),wxOK | wxICON_INFORMATION);
}

void vidtrackguiFrame::OnOpen(wxCommandEvent &event)
{
    wxString filename = wxFileSelector();
    StaticTextfp->SetLabel(filename);
}
void vidtrackguiFrame::OnOpenRecent(wxCommandEvent &event)
{
    wxString filename = wxFileSelector();
}
void vidtrackguiFrame::OnLEDRedCB(wxCommandEvent &event)
{
    wxString trackmethod = "LEDRed";
}

void vidtrackguiFrame::OnLEDGreenCB(wxCommandEvent &event)
{
    wxString trackmethod = "LEDGreen";
}
void vidtrackguiFrame::OnLEDBlueCB(wxCommandEvent &event)
{
    wxString trackmethod = "LEDBlue";
}
void vidtrackguiFrame::OnStart(wxCommandEvent &event)
{
   analyzeWrapper(this);
}
void vidtrackguiFrame::OnStop(wxCommandEvent &event)
{
   wxString msg="Xia said: stop tracking";
   wxLogMessage(msg);
}
