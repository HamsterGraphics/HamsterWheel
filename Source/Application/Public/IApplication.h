/*
 * Copyright (c) 2024-2025 HamsterGraphics
 *
 * https://github.com/HamsterGraphics/HamsterWheel
 *
 */

#pragma once

#include <ApplicationConfig.h>

 ///////////////////////////////////////////////////////
 // Input
 ///////////////////////////////////////////////////////
typedef enum InputKey
{
	INPUT_MOUSE_X,
	INPUT_MOUSE_Y,
	INPUT_MOUSE_LB,
	INPUT_MOUSE_MB,
	INPUT_MOUSE_RB,
	INPUT_MOUSE_XB,
	INPUT_MOUSE_WHEEL,

	MAX_INPUT_VALUE
} InputKey;

typedef struct InputInfo
{
	float InputValue[MAX_INPUT_VALUE];
} InputInfo;

bool Input_Init(InputInfo* pInfo);
float Input_GetValue(InputKey key);

///////////////////////////////////////////////////////
// Application
///////////////////////////////////////////////////////
namespace hg
{

typedef struct AppSettings
{
	int32 WindowPosX;
	int32 WindowPosY;
	int32 WindowWidth;
	int32 WindowHeight;

	// Don't create GUI window. Run in batch mode.
	bool Faceless : 1;
	bool EnableConsole : 1;
	bool Padding : 6;
} AppSettings;

class HG_APP_API IApplication
{
public:
	virtual bool InitSettings() = 0;
	virtual bool Init() = 0;
	virtual void Shutdown() = 0;
	virtual void Update(float deltaTime) = 0;
	virtual void Render() = 0;

	virtual const char* GetName() const = 0;

	AppSettings AppSettings;
	float InputValue[MAX_INPUT_VALUE];
};

}

#define DEFINE_APP_MAIN(AppClass) \
	int AppMain(int argc, char** argv, hg::IApplication* pApp); \
	int main(int argc, char** argv) \
	{ \
		static AppClass app = {}; \
		return AppMain(argc, argv, &app); \
	}