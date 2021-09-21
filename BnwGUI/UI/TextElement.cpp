#include "TextElement.h"
#include <BnwGUI/UI/WindowElement.h>
#include <BnwGUI/BnwGUI.h>
#include <iostream>
namespace BnwGUI
{
	namespace UI
	{

		void TextElement::Render()
		{
			contentSize = Renderer->GetTextSize(Content, (float)TextSize / (float)Renderer->GetMaxFontSize(), true);
			Size = GetNormalizedDeviceCoords( ((ScreenSize.x / 2.) + (contentSize.x / 2.)), ((ScreenSize.y / 2.) - (contentSize.y / 2. + 2)), ScreenSize);

			auto ResultPosition = BaseElement::GetResultPos();
						
			//Renderer->RenderRect(ResultPosition, Size, { 1, 0, 0, 1 });

			this->Renderer->RenderText(
				Content,
				TextColor, (float)TextSize / (float)Renderer->GetMaxFontSize(),
				NormalizedDevCoordsToWindowsCoords(
					ResultPosition.x, ResultPosition.y,
					ScreenSize
				) - glm::vec2(contentSize.x / 2, contentSize.y / 2)
			);
		}

		EventType TextElement::HandleEvent(SDL_Event* e)
		{
			auto startPos = Position;
			Position.y += -Size.y;
			auto toReturn = BaseElement::HandleEvent(e);
			Position = startPos;
			return toReturn;
		}

	}
}