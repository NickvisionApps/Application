#include "Application.h"

namespace NickvisionApplication
{
	wxIMPLEMENT_APP(Application);

	Application::Application()
	{
		
	}

	Application::~Application()
	{

	}

	bool Application::OnInit()
	{
		wxInitAllImageHandlers();
		m_mainWindow = new Views::MainWindow();
		m_mainWindow->Show();
		return true;
	}
}