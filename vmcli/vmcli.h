#pragma once

void WaitForUpdate();

int GetParameterFloat(char* param, float* cur);

int SetParameterFloat(char* param, float val);

int SetParameters(char* param);

int runCommand(char cmd[]);

int initVoicemeeter();

void RemoveNameInPath(char* szPath);

BOOL __cdecl RegistryGetVoicemeeterFolder(char* szDir);

long InitializeDLLInterfaces(void);
