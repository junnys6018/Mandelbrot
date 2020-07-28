#include <Jolt.h>
using namespace Jolt;

class MandelbrotLayer : public Layer
{
public:
	MandelbrotLayer()
		:Layer("Mandelbrot")
	{

	}

	virtual void OnAttach() override
	{

	}
	virtual void OnDetach() override
	{

	}

	virtual void OnUpdate(float ts) override
	{

	}

	virtual void OnImGuiRender() override
	{

	}
private:
};

class MandelbrotApp : public Application
{
public:
	MandelbrotApp()
		:Application("Mandelbrot")
	{
		PushLayer(new MandelbrotLayer());
	}
};


int main()
{
	MandelbrotApp* app = new MandelbrotApp();
	app->Run();

	return 0;
}