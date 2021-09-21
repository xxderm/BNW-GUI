#ifndef TEXT_ELEMENT_H
#define TEXT_ELEMENT_H

#include "BaseElement.h"
#include <glm/glm.hpp>
#include <string>

namespace BnwGUI
{
	namespace UI
	{

		class TextElement final : public BaseElement
		{
			std::wstring Content = L"Text";
			int TextSize = 14;
			glm::vec4 TextColor = { 1, 1, 1, 1 };
			glm::vec2 contentSize;
		public:

			void Render() override;

			EventType HandleEvent(SDL_Event* e) override;

			const std::wstring& GetContent() const { return Content; }

			void SetContent(std::wstring content) { Content = content; }

			void SetTextSize(int size) { TextSize = size; }

			void SetTextColor(glm::vec4 col) { TextColor = col; }

		};

	}
}

#endif // !BUTTON_ELEMENT_H
