#ifndef PINCODE_H
#define PINCODE_H

#include <wx/wx.h>
#include <wx/wxprec.h>
#include <wx/cmdline.h>

#define PINCODE_OK 					0
#define PINCODE_NOT_ENTERED			1
#define PINCODE_BUFFER_TOO_SMALL	2
#define PINCODE_BUFFER_UNDEFINED	3

class PinCodeApp : public wxApp
{
private:
	long		nbrRetries;
	wxString	hash;
	bool		authentication;
	int			ret_exit;
	wxString	ret_msg;
	wxString	path;

public:
	PinCodeApp();

	void SetNbrRetries(long retries);

	void SetHash(char *h);

	virtual void OnInitCmdLine(wxCmdLineParser& parser);
	
	virtual bool OnCmdLineParsed(wxCmdLineParser& parser);

	virtual bool OnInit();

	virtual int OnRun();

	virtual int OnExit();
};

static const wxCmdLineEntryDesc g_cmdLineDesc[] =
{
	{ wxCMD_LINE_SWITCH, "h", "help", "displays help on the command line parameters", wxCMD_LINE_VAL_NONE, wxCMD_LINE_OPTION_HELP },
	{ wxCMD_LINE_SWITCH, "a", "authentication", "Request authentation dialog, default signature dialog", wxCMD_LINE_VAL_NONE, wxCMD_LINE_PARAM_OPTIONAL },
	{ wxCMD_LINE_OPTION, "d", "data", "Hash value to be signed by eId <hex-string>", wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL },
	{ wxCMD_LINE_OPTION, "r", "retries", "Number of retries left <number(max 3)>", wxCMD_LINE_VAL_NUMBER, wxCMD_LINE_PARAM_OPTIONAL },
	{ wxCMD_LINE_OPTION, "p", "path", "IPC path to return PINCODE <number(max 3)>", wxCMD_LINE_VAL_STRING, wxCMD_LINE_OPTION_MANDATORY },
	{ wxCMD_LINE_NONE }
};

wxDECLARE_APP(PinCodeApp);

#endif