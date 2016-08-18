#ifndef PINCODEDLG_H
#define PINCODEDLG_H

/*
 * PinCodeDialog class declaration
 */
#include <wx/wx.h>

class PinCodeDlg: public wxDialog
{    
private:
    wxWindow* m_myParent;
    wxString m_sPassword;

    wxTextCtrl* m_PinCtrl;

public:

    wxString GetPassword()
    {
        return m_sPassword;
    }

    // Constructors
    PinCodeDlg( wxWindow* parent,
        wxWindowID id = wxID_ANY,
        const wxString& caption = wxT("Authentication PIN code"),
        int nbrRetries = -1);
    
    virtual ~PinCodeDlg()
    {
    }

    // implementation only
    void OnOK(wxCommandEvent& event);
    void OnCancel(wxCommandEvent& event);

private:

};
#endif // PINCODEDLG_H