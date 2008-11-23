///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Apr 16 2008)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __wxplaybackcontrolpanel__
#define __wxplaybackcontrolpanel__

#include <wx/slider.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/string.h>
#include <wx/sizer.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/bmpbuttn.h>
#include <wx/button.h>
#include <wx/stattext.h>
#include <wx/panel.h>

///////////////////////////////////////////////////////////////////////////

#define ID_DISPLAY 1000
#define ID_PLAYSLIDER 1001
#define ID_FIRSTFRAME 1002
#define ID_FASTREWIND 1003
#define ID_PREVIOUSFRAME 1004
#define ID_PLAY 1005
#define ID_NEXTFRAME 1006
#define ID_FASTFORWARD 1007
#define ID_LASTFRAME 1008
#define ID_SHUTTLE 1009

///////////////////////////////////////////////////////////////////////////////
/// Class AllInOnePlaybackControlGUI
///////////////////////////////////////////////////////////////////////////////
class AllInOnePlaybackControlGUI : public wxPanel 
{
	private:
	
	protected:
		wxSlider* m_playbackslider;
		wxBitmapButton* m_firstframe;
		wxBitmapButton* m_fastrewind;
		wxBitmapButton* m_previousframe;
		wxBitmapButton* m_play;
		wxBitmapButton* m_nextframe;
		wxBitmapButton* m_fastforward;
		wxBitmapButton* m_lastframe;
		wxSlider* m_shuttle;
		wxStaticText* m_display;
	
	public:
		AllInOnePlaybackControlGUI( wxWindow* parent, wxWindowID id = ID_DISPLAY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxTAB_TRAVERSAL );
		~AllInOnePlaybackControlGUI();
	
};

#endif //__wxplaybackcontrolpanel__
