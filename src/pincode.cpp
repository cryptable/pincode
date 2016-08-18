#include <stdio.h>
#include <pincode.h>
#include <PinCodeDlg.h>
#include <SignPinCodeDlg.h>
#include <wx/msgout.h>
extern "C" {
#include <nanomsg/nn.h>
#include <nanomsg/pipeline.h>
}

#define RET_OK					0
#define RET_INVALID_ARGUMENTS	101

#define PINCODE_AUTHENTICATION  1
#define PINCODE_SIGNATURE       2

/**
* @brief: Print the usage of the pincode application
*/
void print_usage(int verbose) {
	if (verbose) {
		printf("pincode [-a] [-r <number>] [-d <hex-string>] -p <ipc-path>\n");
	}
}

void send_json_msg(const  char *path, char *msg, int msg_lg) {

  	int sock = nn_socket (AF_SP, NN_PUSH);
	if (sock < 0) {
		printf("nn_socket() failed %d\n", errno);
		goto cleanup;
	}

	int conn = nn_connect (sock, path);
	if (sock < 0) {
		printf("nn_connect() failed %d\n", errno);
		goto cleanup;
	}

	printf ("SENDING json with length [%d]\n", msg_lg);
	int bytes = nn_send (sock, msg, msg_lg, 0);
	if (bytes != msg_lg) {
		printf("nn_send() failed %d\n", errno);
		goto cleanup;
	}

cleanup:

	if (sock >= 0) {
		nn_close(sock);
	}
}

const char json_failed[] = "{\"result_code\":101,\"result_data\":\"JSON message creation failed\"}";

/**
* @brief: print the json result
*/
void wx_print_json(const char *path, int error_code, wxString &result) {

  	char *json_msg = (char *)result.mb_str(wxConvUTF8).data();
  	int  msg_len = 50 + strlen(json_msg);

  	char *msg = NULL;
  	msg = (char *) malloc(msg_len);
  	if (msg == NULL) {
  		send_json_msg(path, (char *)json_failed, sizeof(json_failed));
  		goto cleanup;
  	}
  	memset(msg, 0, msg_len);
  	sprintf(msg,"{\"result_code\":%d,\"result_data\":\"%s\"}", error_code, json_msg);

  	send_json_msg(path, msg, strlen(msg));

cleanup:
	if (msg) {
		free(msg);
	}
}

int isnumber(const char *str) {
	while (*str != '\0') {
		if (!isdigit(*str)) {
			return 0;
		}
		str++;
	}
	return 1;
}

wxIMPLEMENT_APP(PinCodeApp);

PinCodeApp::PinCodeApp() {

	authentication = false;
	ret_exit = 0;
	nbrRetries = -1;
	wxApp::wxApp();

}

bool PinCodeApp::OnInit() {

	if (!wxApp::OnInit())
		return false;

	return true;

}

int PinCodeApp::OnRun() {
	ret_msg.Clear();

	wxWindow* parent = wxGetActiveWindow();

	if (authentication) {
		PinCodeDlg pinCodeDlg(parent, wxID_ANY, wxT("Authentication PIN code"), nbrRetries);
		if (pinCodeDlg.ShowModal() == wxID_OK)
		{
			ret_msg = pinCodeDlg.GetPassword();
		}
	}
	else {
		SignPinCodeDlg signPINCodeDlg(parent, wxID_ANY, wxT("Signature PIN code"), nbrRetries, hash);
		if (signPINCodeDlg.ShowModal() == wxID_OK)
		{
			ret_msg = signPINCodeDlg.GetPassword();
		}
	}
	if (parent) {
		parent->SetFocus();
	}

	wx_print_json(path.mbc_str().data(), ret_exit, ret_msg);
	return 0;
}

int PinCodeApp::OnExit() {
	return ret_exit;
}

void PinCodeApp::OnInitCmdLine(wxCmdLineParser& parser) {
	parser.SetDesc(g_cmdLineDesc);
	// must refuse '/' as parameter starter or cannot use "/path" style paths
	parser.SetSwitchChars(wxT("-"));
}

bool PinCodeApp::OnCmdLineParsed(wxCmdLineParser& parser) {

	authentication = parser.Found(wxT("a"));

	if (parser.Found(wxT("d"), &hash)) {
		const char *h = hash.c_str();
		char c = '\0';
		int  cntr = 0;

		// Basic Hexidecimal validation
		while ((c = toupper(h[cntr])) != '\0') {
			if ((c < '0' || c > '9') && (c != 'A') && (c != 'B') && (c != 'C') && (c != 'D') && (c != 'E') && (c != 'F')) {
				ret_msg = wxT("hash has invalid hexadecimal characters");
				ret_exit = RET_INVALID_ARGUMENTS;
			}
			cntr++;
			// check on maximum hash size (512)
			if (cntr > 128) {
				ret_msg = wxT("hash parameter exceed maximal value");
				ret_exit = RET_INVALID_ARGUMENTS;
			}
		}
	}
	else {
		if (!authentication) {
			ret_msg = wxT("missing numeric argument for --hash for signature");
			ret_exit = RET_INVALID_ARGUMENTS;
		}
	}
	if (parser.Found(wxT("r"), &nbrRetries)) {
		if (nbrRetries > 3) {
			ret_msg = wxT("exceeding maximum retries (3) for eId");
			ret_exit = RET_INVALID_ARGUMENTS;
		}
	}
	parser.Found(wxT("p"), &path);

	if (ret_exit) {
		wx_print_json(path.mbc_str().data(), ret_exit, ret_msg);
		return false;
	}
	else {
		return true;
	}
}
