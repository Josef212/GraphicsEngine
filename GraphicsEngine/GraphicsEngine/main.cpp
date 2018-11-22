#include <Windows.h>
#include <iostream>

#include "App.h"
//#include <SDL.h>

// -------------------------------------------------------------------------------
// Globals

enum MainState
{
	MAIN_CREATION = 0,
	MAIN_START = 1,
	MAIN_UPDATE = 2,
	MAIN_FINISH = 3,
	MAIN_EXIT = 4
};

App* app = nullptr;

// -------------------------------------------------------------------------------

int main(int argc, char** argv)
{
	LOG(CONSOLE_TEXT_GREEN, "Starting graphics engine.");

	int ret = EXIT_FAILURE;
	MainState state = MAIN_CREATION;

	while (state != MAIN_EXIT)
	{
		switch (state)
		{
		case MAIN_CREATION:

			LOG(LOG_INFO, "==================== App creation =======================");

			app = new App();
			state = MAIN_START;

			break;

		case MAIN_START:

			LOG(LOG_INFO, "==================== App Init =======================");

			if (app->Init())
			{
				state = MAIN_UPDATE;
				LOG(LOG_INFO, "==================== App update =======================");
			}
			else
			{
				LOG(LOG_ERROR, "App init error!");
				state = MAIN_EXIT;
			}

			break;

		case MAIN_UPDATE:
		{
			const auto tmp = app->Update();

			if (tmp == UpdateReturn::UPDT_ERROR)
			{
				LOG(LOG_ERROR, "App update error!");
				state = MAIN_EXIT;
			}

			if (tmp == UpdateReturn::UPDT_STOP) state = MAIN_FINISH;

			break;
		}

		case MAIN_FINISH:

			LOG(LOG_INFO, "==================== App clean up =======================");

			if (app->CleanUp())
				ret = EXIT_SUCCESS;
			else
				LOG(LOG_ERROR, "App clean up error.");

			state = MAIN_EXIT;

			break;
		}
	}

	RELEASE(app);

	LOG(LOG_INFO, "Exiting the engine.");

#ifdef _DEBUG	
	//PAUSE
#else

#endif

	return ret;
}