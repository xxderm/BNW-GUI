#include <SDL.h>
#undef main
#include <BnwGUI/BnwGUI.h>
#include <BnwGUI/UI/ButtonElement.h>
#include <BnwGUI/UI/TextElement.h>
#include <BnwGUI/UI/TextEdit.h>
#include <iostream>
#include <memory>

using namespace BnwGUI;
using namespace UI;

int main()
{	
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	auto wnd = SDL_CreateWindow(
		"Buttons example",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		800,
		600,
		SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_RENDERER_PRESENTVSYNC
	);
	//SDL_RENDERER_ACCELERATED |
	auto ctx = SDL_GL_CreateContext(wnd);
	glewInit();
	SDL_Event event;

	GuiManager manager;
	manager.Init("C:/Windows/Fonts/Arial.ttf", { 800, 600 }, 72);

	WindowElement* window1 = new WindowElement(
		"Test", glm::vec2(-0.21, -0.2), glm::vec2(0.3, 0.5),
		-1, BnwGUI::UI::ElementParameter::UNTITLED_NO_EXIT_BUTTON
	);

	manager += window1;


	BaseElement* text = new TextElement();
	text->SetPosition({ 0., 0.4 });
	GetElement<TextElement>(text)->SetContent(L"швапива0ы9гпты5r7y6rteyuпшзагтывгЗШАТЫВШАИЫВзывтаspfdginsP(ISUGDFBSPIGD");
	GetElement<TextElement>(text)->SetTextSize(24);
	GetElement<TextElement>(text)->SetTextColor({ 0, 0, 0, 1 });

	BaseElement* textEdit = new TextEditElement();
	GetElement<ButtonElement>(textEdit)->SetPosition({ 0.0, 0.2 });
	GetElement<ButtonElement>(textEdit)->SetContent(L"edit...");


	BaseElement* button1 = new ButtonElement();
	BaseElement* button2 = new ButtonElement();	

	GetElement<ButtonElement>(button2)->SetPosition({ 0.0, -0.2 });

	*window1 += button1;
	*window1 += button2;
	*window1 += textEdit;
	*window1 += text;

	window1->OnClickEvent([]() {printf("WIN"); });


	button1->OnClickEvent( std::move(
		[&]() mutable -> void
		{
			printf("%d\n", SDL_GetTicks());
		})
	);	

	int countedFrames = 0;
	while (true)
	{
		float avgFPS = countedFrames / (SDL_GetTicks() / 1000.f);
		if (avgFPS > 2000000)
			avgFPS = 0;
		//printf("%f\n", avgFPS);
		
		int width, height;
		SDL_GetWindowSize(wnd, &width, &height);
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_WINDOWEVENT &&
				event.window.event == SDL_WINDOWEVENT_RESIZED)
			{
				SDL_GetWindowSize(wnd, &width, &height);
				glViewport(0, 0, width, height);
				manager.Resize({ width, height });
			}
			manager.HandleEvent(&event);
		}

		glClearColor(1, 1, 1, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		
		manager.Render();		

		SDL_GL_SwapWindow(wnd);
		++countedFrames;
	}

	return 0;
}