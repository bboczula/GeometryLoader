#include "CBaseWindow.h"
#include "CWindowsApp.h"
#include "CEngine.h"

class CWindowsTest : public CWindowsApp
{
	CEngine* engine;
public:
	CWindowsTest() : CWindowsApp(nullptr)
	{
		registerWindow();
		createWindow("AutoStack v.1.0", 800, 600);
	}
private:
	void initialize() override
	{
		engine = new CEngine(this->getHwndPtr());
	}
	void update() override
	{
	}
	void cleanup() override
	{
		delete engine;
	}
};

int main(int argc, char* argv[])
{
	CWindowsTest* window = new CWindowsTest();
	window->start();
	delete window;

	return 0;
}