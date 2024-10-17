/*
 * Copyright (c) 2024-2025 HamsterGraphics
 *
 * https://github.com/HamsterGraphics/HamsterWheel
 *
 */

#include <IApplication.h>
#include <IGraphics.h>

class TriangleApp : public hg::IApplication
{
public:
	TriangleApp()
	{
		memset(&m_settings, 0, sizeof(m_settings));
		m_settings.EnableConsole = true;

		memset(&m_graphicsInfo, 0, sizeof(m_graphicsInfo));
	}

	virtual bool Init() override
	{
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