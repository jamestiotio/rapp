//--------------------------------------------------------------------------//
/// Copyright (c) 2017 Milos Tosic. All Rights Reserved.                   ///
/// License: http://www.opensource.org/licenses/BSD-2-Clause               ///
//--------------------------------------------------------------------------//

#include <multi_app_pch.h>

struct app1 : public rapp::App
{
	RAPP_CLASS(app1)

	rapp::WindowHandle	m_window;

	virtual ~app1() {}

	int init(int32_t /*_argc*/, const char* const* /*_argv*/, rtmLibInterface* /*_libInterface = 0*/)
	{
		static const rapp::InputBinding bindings[] =
		{
			{ NULL, "exit", 1, { rapp::KeyboardState::Key::KeyQ,   rapp::KeyboardState::Modifier::LCtrl  }},
			{ NULL, "exit", 1, { rapp::KeyboardState::Key::KeyQ,   rapp::KeyboardState::Modifier::RCtrl  }},
			{ NULL, "hide", 1, { rapp::KeyboardState::Key::Tilde,  rapp::KeyboardState::Modifier::NoMods }},
			RAPP_INPUT_BINDING_END
		};

		rapp::inputAddBindings("bindings", bindings);
		rapp::cmdAdd("exit", cmdExit,			0, "quits application");
		rapp::cmdAdd("hide", cmdHideConsole,	0, "hides console");

		uint32_t width, height;
		rapp::windowGetDefaultSize(&width, &height);
		m_width		= width;
		m_height	= height;

		m_window = rapp::appGraphicsInit(this, m_width, m_height);
		return 0;
	}

	void suspend() {}
	void resume() {}
	void update(float /*_time*/) {}

	void draw()
	{
		// Set view 0 default viewport.
		bgfx::setViewRect(0, 0, 0, (uint16_t)m_width, (uint16_t)m_height);

		// This dummy draw call is here to make sure that view 0 is cleared
		// if no other draw calls are submitted to view 0.
		bgfx::touch(0);

		// Use debug font to print information about this example.
		bgfx::dbgTextClear();
		bgfx::dbgTextPrintf(0, 1, 0x17, "rapp/samples/multi_app");
		bgfx::dbgTextPrintf(0, 2, 0x37, m_description);

		// debug input
		rapp::inputDgbGamePads();
		rapp::inputDgbMouse();
		rapp::inputDgbKeyboard();
		rapp::inputDgbTouch();

		// Advance to next frame. Rendering thread will be kicked to 
		// process submitted rendering primitives.
		bgfx::frame();
	}

	void drawGUI()
	{
		ImGui::SetNextWindowPos(ImVec2(10.0f, 666.0f), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize(ImVec2(300.0f, 80.0f), ImGuiCond_FirstUseEver);
		ImGui::Begin("ImGui dialog");
		ImGui::Separator();
		ImGui::Button("Button", ImVec2(180.0f,23.0f));
		ImGui::End();
	}
	
	void shutDown()
	{
		rtm::Console::custom(255, 255, 0, 1, "Shutting down app\n", (uint32_t)rtm::Thread::getThreadID());
		rapp::appGraphicsShutdown(this, m_window);
		rapp::inputRemoveBindings("bindings");
	}

	static int cmdExit(rapp::App* _app, void* _userData, int _argc, char const* const* _argv)
	{
		RTM_UNUSED_3(_userData, _argv, _argc);
		_app->quit();
		return 0;
	}

	static int cmdHideConsole(rapp::App* _app, void* _userData, int _argc, char const* const* _argv)
	{
		RTM_UNUSED_3(_userData, _argv, _argc);
		rapp::cmdConsoleToggle(_app);
		return 0;
	}
};

RAPP_REGISTER(app1, "app1", "Application 1");
