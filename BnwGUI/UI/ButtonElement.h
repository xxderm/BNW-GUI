#ifndef BUTTON_ELEMENT_H
#define BUTTON_ELEMENT_H

#include "BaseElement.h"
#include <glm/glm.hpp>
#include <string>

namespace BnwGUI
{
	namespace UI
	{

		class ButtonElement final : public BaseElement
		{
			std::wstring Content = L"Button";
			int TextSize = 14;			
			glm::vec4 TextColor = { 1, 1, 1, 1 };
			glm::vec4 ButtonColor = { 0, 0, 0, 1 };			
		public:
			
			void Render() override;

			EventType HandleEvent(SDL_Event* e) override;

			void SetContent(std::wstring content) { Content = content; }

			void SetTextSize(int size) { TextSize = size; }

			void SetTextColor(glm::vec4 col) { TextColor = col; }

			void SetButtonColor(glm::vec4 col) { ButtonColor = col; }

		};

	}
}

#endif // !BUTTON_ELEMENT_H
