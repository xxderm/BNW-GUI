#ifndef WINDOW_ELEMENT_H
#define WINDOW_ELEMENT_H

#include "BaseElement.h"
#include <string>

namespace BnwGUI
{

	class Render;

	namespace UI
	{		

		struct WindowElementFormat
		{
			std::string Name;			
			glm::vec4 Color = glm::vec4(0.8);
			int Texture = -1;
			ElementParameter Parameter = ElementParameter::UNTITLED_NO_EXIT_BUTTON;
		}; 

		class WindowElement final : public BaseElement
		{
			std::vector<BaseElement *> Childs;
			WindowElementFormat Format;
			float HeaderHeight = 0.05f;
			glm::vec4 HeaderColor = glm::vec4(0.4);
			glm::vec2 ExitScale = glm::vec2(0.025, 0.025);
			glm::vec4 ExitColor = glm::vec4(0.9, 0.0, 0.0, 1.0);
		public:
			
			friend class ButtonElement;

			WindowElement(
				std::string name,
				glm::vec2 center, glm::vec2 scale,
				glm::vec4 color = glm::vec4(0.8),
				ElementParameter parameter = ElementParameter::UNTITLED_NO_EXIT_BUTTON
			);

			WindowElement(
				std::string name,
				glm::vec2 center, glm::vec2 scale,
				int texture,
				ElementParameter parameter = ElementParameter::UNTITLED_NO_EXIT_BUTTON
			);

			WindowElement(WindowElementFormat& Format);

			void Render() override;

			EventType HandleEvent(SDL_Event* e) override;

			void SetScreenSize(glm::vec2 ss) override;

			void SetRenderer(BnwGUI::Render *Renderer);

			void operator+=(BaseElement *child);

		};

	}
}

#endif // !WINDOW_ELEMENT_H
