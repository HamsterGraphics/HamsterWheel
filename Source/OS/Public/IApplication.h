﻿/*
 * Copyright (c) 2024-2025 HamsterGraphics
 *
 * https://github.com/HamsterGraphics/HamsterWheel
 *
 */

#pragma once

#include <Delegates/MulticastDelegate.hpp>

#include "Config.h"

 ///////////////////////////////////////////////////////
 // Input
 ///////////////////////////////////////////////////////
enum InputValue
{
	INPUT_MOUSE_X,
	INPUT_MOUSE_Y,
	INPUT_MOUSE_LB,
	INPUT_MOUSE_MB,
	INPUT_MOUSE_RB,
	INPUT_MOUSE_XB,
	INPUT_MOUSE_WHEEL,

	MAX_INPUT_VALUE
};

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

class HG_API IApplication
{
public:
	virtual bool Init() = 0;
	virtual void Shutdown() = 0;
	virtual void Update(float deltaTime) = 0;
	virtual void Render() = 0;

	virtual const char* GetName() const = 0;
	virtual AppSettings& GetSettings() { return m_settings; }

	float InputValue[MAX_INPUT_VALUE];

protected:
	AppSettings m_settings;
};

}

#define DEFINE_APP_MAIN(AppClass) \
	int AppMain(int argc, char** argv, hg::IApplication* pApp); \
	int main(int argc, char** argv) \
	{ \
		static AppClass app = {}; \
		return AppMain(argc, argv, &app); \
	}