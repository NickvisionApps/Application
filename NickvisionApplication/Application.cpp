#include "Application.h"
#include "Views/SplashScreen.h"

namespace NickvisionApplication
{
	wxIMPLEMENT_APP(Application);

	using namespace NickvisionApplication::Views;

	Application::Application()
	{
		
	}

	Application::~Application()
	{

	}

	bool Application::OnInit()
	{
		wxInitAllImageHandlers();
		m_mainWindow = new MainWindow();
		SplashScreen splash(m_mainWindow);
		splash.ShowModal();
		m_mainWindow->Show();
		return true;
	}
}