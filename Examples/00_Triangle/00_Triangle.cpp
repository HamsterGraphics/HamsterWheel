/*
 * Copyright (c) 2024-2025 HamsterGraphics
 *
 * https://github.com/HamsterGraphics/HamsterWheel
 *
 */

#include "IApplication.h"
#include "IGraphics.h"

#include <memory>

class TriangleApp : public hg::IApplication
{
public:
	virtual bool InitSettings() override
	{
		memset(&AppSettings, 0, sizeof(AppSettings));
		AppSettings.EnableConsole = true;
		return true;
	}

	virtual bool Init() override
	{
		memset(&m_graphicsInfo, 0, sizeof(m_graphicsInfo));
		Graphics_Init(&m_graphicsInfo);
		return true;
	}

	virtual void Shutdown() override
	{
	}

	virtual void Update(float deltaTime) override
	{
	}

	virtual void Render() override
	{
	}

	virtual const char* GetName() const override
	{
		return "00_Triangle";
	}

private:
	GraphicsInfo m_graphicsInfo;
};

DEFINE_APP_MAIN(TriangleApp);
