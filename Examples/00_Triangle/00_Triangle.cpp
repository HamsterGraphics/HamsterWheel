/*
 * Copyright (c) 2024-2025 HamsterGraphics
 *
 * https://github.com/HamsterGraphics/HamsterWheel
 *
 */

#include "IApplication.h"
#include "IGraphics.h"
#include "IOperatingSystem.h"

#include <memory>

class TriangleApp : public hg::IApplication
{
public:
	virtual bool InitSettings() override
	{
		memset(&AppSettings, 0, sizeof(AppSettings));
		return true;
	}

	virtual bool Init() override
	{
		LOG_TRACE("Init graphics module.");
		m_graphicsContext = std::make_unique<GraphicsContext>();
		GraphicsContextCreateInfo contextCreateInfo = {};
		contextCreateInfo.EnableStablePowerMode = false;
		contextCreateInfo.Debug.EnableGPUBasedValidation = true;
		contextCreateInfo.Debug.EnableSynchronizedCommandQueueValidation = true;
		Graphics_Init(contextCreateInfo, m_graphicsContext.get());

		return true;
	}

	virtual void Shutdown() override
	{
		Graphics_Shutdown(m_graphicsContext.get());
	}

	virtual void Update(float deltaTime) override
	{
	}

	virtual void Render() override
	{
		Graphics_BeginFrame(m_graphicsContext.get());
		Graphics_EndFrame(m_graphicsContext.get());
	}

	virtual const char* GetName() const override
	{
		return "00_Triangle";
	}

private:
	std::unique_ptr<GraphicsContext> m_graphicsContext;
};

DEFINE_APP_MAIN(TriangleApp);
