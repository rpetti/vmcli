// vmcli.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <Windows.h>
#include "VoicemeeterRemote.h"
#include <iostream>
#include <string>
#include <math.h>
#include "vmcli.h"

static T_VBVMR_INTERFACE iVMR;

int main(int argc, char * argv[])
{
	int rep = initVoicemeeter();
	if (!rep)
	{
		return -1;
	}
	for (int i = 1; i < argc; i++)
	{
		runCommand(argv[i]);
	}
	iVMR.VBVMR_Logout();
}

void WaitForUpdate()
{
	while (!iVMR.VBVMR_IsParametersDirty())
	{
		Sleep(50);
	}
	Sleep(50);
}

int GetParameterFloat(char* param, float* cur)
{
	WaitForUpdate();
	return iVMR.VBVMR_GetParameterFloat(param, cur);
}

int SetParameterFloat(char* param, float val)
{
	int ret = iVMR.VBVMR_SetParameterFloat(param, val);
	WaitForUpdate();
	return ret;
}

int SetParameters(char* param)
{
	int ret = iVMR.VBVMR_SetParameters(param);
	WaitForUpdate();
	return ret;
}

int runCommand(char cmdChar[])
{
	// !Parameter - Toggle value
	std::string cmd(cmdChar);
	std::size_t found = cmd.find("!");
	if (found == 0)
	{
		float cur;
		char param[100];
		strncpy_s(param, cmd.substr(1, cmd.length() - 1).c_str(), 100);
		GetParameterFloat(param, &cur);
		if (round(cur) == 1)
		{
			std::cout << "toggling " << param << " to 0\n";
			std::cout << SetParameterFloat(param, 0.0f);
		}
		else if (round(cur) == 0)
		{
			std::cout << "toggling " << param << " to 1\n";
			std::cout << SetParameterFloat(param, 1.0f);
		}
		return TRUE;
	}
	// pass through the rest
	char* paramline = const_cast<char*>(cmd.c_str());
	std::cout << "passing through " << paramline << "\n";
	SetParameters(paramline);
}

int initVoicemeeter()
{
	int rep = InitializeDLLInterfaces();
	if (rep < 0)
	{
		if (rep == -100)
		{
			std::cout << "Voicemeeter is not installed\n";
		}
		else
		{
			std::cout << "Error loading Voicemeeter dll\n";
		}
		return FALSE;
	}
	rep = iVMR.VBVMR_Login();
	if (rep != 0)
	{
		std::cout << "Error logging into Voicemeeter\n";
		return FALSE;
	}
	return TRUE;
}

/*******************************************************************************/
/**                           GET VOICEMEETER DIRECTORY                       **/
/*******************************************************************************/

static char uninstDirKey[] = "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall";

#define INSTALLER_UNINST_KEY	"VB:Voicemeeter {17359A74-1236-5467}"


void RemoveNameInPath(char* szPath)
{
	long ll;
	ll = (long)strlen(szPath);
	while ((ll > 0) && (szPath[ll] != '\\')) ll--;
	if (szPath[ll] == '\\') szPath[ll] = 0;
}

#ifndef KEY_WOW64_32KEY
#define KEY_WOW64_32KEY 0x0200
#endif

BOOL __cdecl RegistryGetVoicemeeterFolder(char* szDir)
{
	char szKey[256];
	char sss[1024];
	DWORD nnsize = 1024;
	HKEY hkResult;
	LONG rep;
	DWORD pptype = REG_SZ;
	sss[0] = 0;

	// build Voicemeeter uninstallation key
	strcpy_s(szKey, uninstDirKey);
	strcat_s(szKey, "\\");
	strcat_s(szKey, INSTALLER_UNINST_KEY);

	// open key
	rep = RegOpenKeyEx(HKEY_LOCAL_MACHINE, szKey, 0, KEY_READ, &hkResult);
	if (rep != ERROR_SUCCESS)
	{
		// if not present we consider running in 64bit mode and force to read 32bit registry
		rep = RegOpenKeyEx(HKEY_LOCAL_MACHINE, szKey, 0, KEY_READ | KEY_WOW64_32KEY, &hkResult);
	}
	if (rep != ERROR_SUCCESS) return FALSE;
	// read uninstall profram path
	rep = RegQueryValueEx(hkResult, "UninstallString", 0, &pptype, (unsigned char*)sss, &nnsize);
	RegCloseKey(hkResult);

	if (pptype != REG_SZ) return FALSE;
	if (rep != ERROR_SUCCESS) return FALSE;
	// remove name to get the path only
	RemoveNameInPath(sss);
	if (nnsize > 512) nnsize = 512;
	strncpy_s(szDir, 512, sss, nnsize);

	return TRUE;
}


/*******************************************************************************/
/**                                GET DLL INTERFACE                          **/
/*******************************************************************************/

static HMODULE G_H_Module = NULL;

//if we directly link source code (for development only)
#ifdef VBUSE_LOCALLIB

long InitializeDLLInterfaces(void)
{
	iVMR.VBVMR_Login = VBVMR_Login;
	iVMR.VBVMR_Logout = VBVMR_Logout;
	iVMR.VBVMR_RunVoicemeeter = VBVMR_RunVoicemeeter;
	iVMR.VBVMR_GetVoicemeeterType = VBVMR_GetVoicemeeterType;
	iVMR.VBVMR_GetVoicemeeterVersion = VBVMR_GetVoicemeeterVersion;
	iVMR.VBVMR_IsParametersDirty = VBVMR_IsParametersDirty;
	iVMR.VBVMR_GetParameterFloat = VBVMR_GetParameterFloat;
	iVMR.VBVMR_GetParameterStringA = VBVMR_GetParameterStringA;
	iVMR.VBVMR_GetParameterStringW = VBVMR_GetParameterStringW;

	iVMR.VBVMR_GetLevel = VBVMR_GetLevel;
	iVMR.VBVMR_GetMidiMessage = VBVMR_GetMidiMessage;
	iVMR.VBVMR_SetParameterFloat = VBVMR_SetParameterFloat;
	iVMR.VBVMR_SetParameters = VBVMR_SetParameters;
	iVMR.VBVMR_SetParametersW = VBVMR_SetParametersW;
	iVMR.VBVMR_SetParameterStringA = VBVMR_SetParameterStringA;
	iVMR.VBVMR_SetParameterStringW = VBVMR_SetParameterStringW;

	iVMR.VBVMR_Output_GetDeviceNumber = VBVMR_Output_GetDeviceNumber;
	iVMR.VBVMR_Output_GetDeviceDescA = VBVMR_Output_GetDeviceDescA;
	iVMR.VBVMR_Output_GetDeviceDescW = VBVMR_Output_GetDeviceDescW;
	iVMR.VBVMR_Input_GetDeviceNumber = VBVMR_Input_GetDeviceNumber;
	iVMR.VBVMR_Input_GetDeviceDescA = VBVMR_Input_GetDeviceDescA;
	iVMR.VBVMR_Input_GetDeviceDescW = VBVMR_Input_GetDeviceDescW;

#ifdef VMR_INCLUDE_AUDIO_PROCESSING_EXAMPLE
	iVMR.VBVMR_AudioCallbackRegister = VBVMR_AudioCallbackRegister;
	iVMR.VBVMR_AudioCallbackStart = VBVMR_AudioCallbackStart;
	iVMR.VBVMR_AudioCallbackStop = VBVMR_AudioCallbackStop;
	iVMR.VBVMR_AudioCallbackUnregister = VBVMR_AudioCallbackUnregister;
#endif
	return 0;
}

//Dynamic link to DLL in 'C' (regular use)
#else

long InitializeDLLInterfaces(void)
{
	char szDllName[1024];
	memset(&iVMR, 0, sizeof(T_VBVMR_INTERFACE));

	//get folder where is installed Voicemeeter
	if (RegistryGetVoicemeeterFolder(szDllName) == FALSE)
	{
		// voicemeeter not installed
		return -100;
	}
	//use right dll according O/S type
	if (sizeof(void*) == 8) strcat_s(szDllName, "\\VoicemeeterRemote64.dll");
	else strcat_s(szDllName, "\\VoicemeeterRemote.dll");

	// Load Dll
	G_H_Module = LoadLibrary(szDllName);
	if (G_H_Module == NULL) return -101;

	// Get function pointers
	iVMR.VBVMR_Login = (T_VBVMR_Login)GetProcAddress(G_H_Module, "VBVMR_Login");
	iVMR.VBVMR_Logout = (T_VBVMR_Logout)GetProcAddress(G_H_Module, "VBVMR_Logout");
	iVMR.VBVMR_RunVoicemeeter = (T_VBVMR_RunVoicemeeter)GetProcAddress(G_H_Module, "VBVMR_RunVoicemeeter");
	iVMR.VBVMR_GetVoicemeeterType = (T_VBVMR_GetVoicemeeterType)GetProcAddress(G_H_Module, "VBVMR_GetVoicemeeterType");
	iVMR.VBVMR_GetVoicemeeterVersion = (T_VBVMR_GetVoicemeeterVersion)GetProcAddress(G_H_Module, "VBVMR_GetVoicemeeterVersion");

	iVMR.VBVMR_IsParametersDirty = (T_VBVMR_IsParametersDirty)GetProcAddress(G_H_Module, "VBVMR_IsParametersDirty");
	iVMR.VBVMR_GetParameterFloat = (T_VBVMR_GetParameterFloat)GetProcAddress(G_H_Module, "VBVMR_GetParameterFloat");
	iVMR.VBVMR_GetParameterStringA = (T_VBVMR_GetParameterStringA)GetProcAddress(G_H_Module, "VBVMR_GetParameterStringA");
	iVMR.VBVMR_GetParameterStringW = (T_VBVMR_GetParameterStringW)GetProcAddress(G_H_Module, "VBVMR_GetParameterStringW");
	iVMR.VBVMR_GetLevel = (T_VBVMR_GetLevel)GetProcAddress(G_H_Module, "VBVMR_GetLevel");
	iVMR.VBVMR_GetMidiMessage = (T_VBVMR_GetMidiMessage)GetProcAddress(G_H_Module, "VBVMR_GetMidiMessage");

	iVMR.VBVMR_SetParameterFloat = (T_VBVMR_SetParameterFloat)GetProcAddress(G_H_Module, "VBVMR_SetParameterFloat");
	iVMR.VBVMR_SetParameters = (T_VBVMR_SetParameters)GetProcAddress(G_H_Module, "VBVMR_SetParameters");
	iVMR.VBVMR_SetParametersW = (T_VBVMR_SetParametersW)GetProcAddress(G_H_Module, "VBVMR_SetParametersW");
	iVMR.VBVMR_SetParameterStringA = (T_VBVMR_SetParameterStringA)GetProcAddress(G_H_Module, "VBVMR_SetParameterStringA");
	iVMR.VBVMR_SetParameterStringW = (T_VBVMR_SetParameterStringW)GetProcAddress(G_H_Module, "VBVMR_SetParameterStringW");

	iVMR.VBVMR_Output_GetDeviceNumber = (T_VBVMR_Output_GetDeviceNumber)GetProcAddress(G_H_Module, "VBVMR_Output_GetDeviceNumber");
	iVMR.VBVMR_Output_GetDeviceDescA = (T_VBVMR_Output_GetDeviceDescA)GetProcAddress(G_H_Module, "VBVMR_Output_GetDeviceDescA");
	iVMR.VBVMR_Output_GetDeviceDescW = (T_VBVMR_Output_GetDeviceDescW)GetProcAddress(G_H_Module, "VBVMR_Output_GetDeviceDescW");
	iVMR.VBVMR_Input_GetDeviceNumber = (T_VBVMR_Input_GetDeviceNumber)GetProcAddress(G_H_Module, "VBVMR_Input_GetDeviceNumber");
	iVMR.VBVMR_Input_GetDeviceDescA = (T_VBVMR_Input_GetDeviceDescA)GetProcAddress(G_H_Module, "VBVMR_Input_GetDeviceDescA");
	iVMR.VBVMR_Input_GetDeviceDescW = (T_VBVMR_Input_GetDeviceDescW)GetProcAddress(G_H_Module, "VBVMR_Input_GetDeviceDescW");

#ifdef VMR_INCLUDE_AUDIO_PROCESSING_EXAMPLE
	iVMR.VBVMR_AudioCallbackRegister = (T_VBVMR_AudioCallbackRegister)GetProcAddress(G_H_Module, "VBVMR_AudioCallbackRegister");
	iVMR.VBVMR_AudioCallbackStart = (T_VBVMR_AudioCallbackStart)GetProcAddress(G_H_Module, "VBVMR_AudioCallbackStart");
	iVMR.VBVMR_AudioCallbackStop = (T_VBVMR_AudioCallbackStop)GetProcAddress(G_H_Module, "VBVMR_AudioCallbackStop");
	iVMR.VBVMR_AudioCallbackUnregister = (T_VBVMR_AudioCallbackUnregister)GetProcAddress(G_H_Module, "VBVMR_AudioCallbackUnregister");
#endif
	// check pointers are valid
	if (iVMR.VBVMR_Login == NULL) return -1;
	if (iVMR.VBVMR_Logout == NULL) return -2;
	if (iVMR.VBVMR_RunVoicemeeter == NULL) return -2;
	if (iVMR.VBVMR_GetVoicemeeterType == NULL) return -3;
	if (iVMR.VBVMR_GetVoicemeeterVersion == NULL) return -4;
	if (iVMR.VBVMR_IsParametersDirty == NULL) return -5;
	if (iVMR.VBVMR_GetParameterFloat == NULL) return -6;
	if (iVMR.VBVMR_GetParameterStringA == NULL) return -7;
	if (iVMR.VBVMR_GetParameterStringW == NULL) return -8;
	if (iVMR.VBVMR_GetLevel == NULL) return -9;
	if (iVMR.VBVMR_SetParameterFloat == NULL) return -10;
	if (iVMR.VBVMR_SetParameters == NULL) return -11;
	if (iVMR.VBVMR_SetParametersW == NULL) return -12;
	if (iVMR.VBVMR_SetParameterStringA == NULL) return -13;
	if (iVMR.VBVMR_SetParameterStringW == NULL) return -14;
	if (iVMR.VBVMR_GetMidiMessage == NULL) return -15;

	if (iVMR.VBVMR_Output_GetDeviceNumber == NULL) return -30;
	if (iVMR.VBVMR_Output_GetDeviceDescA == NULL) return -31;
	if (iVMR.VBVMR_Output_GetDeviceDescW == NULL) return -32;
	if (iVMR.VBVMR_Input_GetDeviceNumber == NULL) return -33;
	if (iVMR.VBVMR_Input_GetDeviceDescA == NULL) return -34;
	if (iVMR.VBVMR_Input_GetDeviceDescW == NULL) return -35;

#ifdef VMR_INCLUDE_AUDIO_PROCESSING_EXAMPLE
	if (iVMR.VBVMR_AudioCallbackRegister == NULL) return -40;
	if (iVMR.VBVMR_AudioCallbackStart == NULL) return -41;
	if (iVMR.VBVMR_AudioCallbackStop == NULL) return -42;
	if (iVMR.VBVMR_AudioCallbackUnregister == NULL) return -43;
#endif
	return 0;
}


#endif

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
