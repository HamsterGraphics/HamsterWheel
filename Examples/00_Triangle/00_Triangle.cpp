#include <IApplication.h>

class TriangleApp : public hg::IApplication
{
public:
	virtual bool Init() override
	{
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
};

DEFINE_APP_MAIN(TriangleApp);