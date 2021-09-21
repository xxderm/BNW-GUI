#ifndef TEXT_EDIT_ELEMENT_H
#define TEXT_EDIT_ELEMENT_H

#include "BaseElement.h"
#include <glm/glm.hpp>
#include <string>

#ifdef _WIN32
#include <Windows.h>
#endif

namespace BnwGUI
{
	namespace UI
	{

		class TextEditElement final : public BaseElement
		{
			std::wstring Text;
			int TextSize = 15;
			glm::vec2 TextScale;
			glm::vec4 TextColor = { 0, 0, 0, 1 };
			glm::vec4 InputAreaColor = { 1, 1, 1, 1 };
			glm::vec2 TextPos;
			glm::vec2 ContentSize;
			int CurrentCharIndex;
			std::vector<int> SelectedCharIndices;
			const Uint8* state;
			glm::vec2 IndicatorPos;
			glm::vec2 IndicatorSize;
			glm::vec2 IndicatorColour;
		public:

			TextEditElement();

			void Render() override;

			EventType HandleEvent(SDL_Event* e) override;

			void SetText(std::wstring text) { Text = text; }

			void SetTextSize(int size) { TextSize = size; }

			void SetTextColor(glm::vec4 col) { TextColor = col; }

		};

	}
}

#endif // !BUTTON_ELEMENT_H
