#ifndef PICKNAMEDLG_H
#define PICKNAMEDLG_H

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif


class PickNameDlg : public wxDialog
{
    public:
        PickNameDlg(wxWindow *parent);
        virtual ~PickNameDlg();

        wxString GetName();
    private:
        wxWindow* m_parent;
        wxTextCtrl* nameTextCtrl;
};

#endif
