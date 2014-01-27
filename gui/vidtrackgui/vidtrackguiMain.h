/***************************************************************
 * Name:      vidtrackguiMain.h
 * Purpose:   Defines Application Frame
 * Author:    xhong ()
 * Created:   2013-11-15
 * Copyright: xhong ()
 * License:
 **************************************************************/

#ifndef VIDTRACKGUIMAIN_H
#define VIDTRACKGUIMAIN_H

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "vidtrackguiApp.h"

#include <wx/menu.h>
//#include <wx/button.h>
//#include <wx/statline.h>
//#include <wx/filedlg.h>

class vidtrackguiFrame: public wxFrame
{
    public:
        vidtrackguiFrame(const wxString& title, const wxPoint& pos, const wxSize& size);
        ~vidtrackguiFrame();
        wxGauge* ProBar;
        wxStaticText *StaticTextfp;   //for displaying the file path

    protected:
        enum
        {
            idOpen,
            idOpenRecent,
            idBtnStart,
            idBtnStop,
            idProBar,
            idLEDRedCB,
            idLEDGreenCB,
            idLEDBlueCB
        };

    private:
        void OnExit(wxCommandEvent& event);
        void OnAbout(wxCommandEvent& event);
        void OnOpen(wxCommandEvent& event);
        void OnOpenRecent(wxCommandEvent& event);
        void OnStart(wxCommandEvent& event);
        void OnStop(wxCommandEvent& event);
        void OnLEDRedCB(wxCommandEvent& event);
        void OnLEDGreenCB(wxCommandEvent& event);
        void OnLEDBlueCB(wxCommandEvent& event);
        DECLARE_EVENT_TABLE();

};

#endif // VIDTRACKGUIMAIN_H
