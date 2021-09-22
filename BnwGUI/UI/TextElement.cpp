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
			Size = GetNormalizedDeviceCoords( 
				static_cast<int>((ScreenSize.x / 2.f) + (contentSize.x / 2.f)), 
				static_cast<int>((ScreenSize.y / 2.f) - (contentSize.y / 2.f + 2)),
				ScreenSize);

			auto ResultPosition = BaseElement::GetResultPos();
						
			//Renderer->RenderRect(ResultPosition, Size, { 1, 0, 0, 1 });

			if(Parent)
				BeginScissor(BaseElement::GetResultPos(), Parent->GetSize());
			this->Renderer->RenderText(
				Content,
				TextColor, (float)TextSize / (float)Renderer->GetMaxFontSize(),
				NormalizedDevCoordsToWindowsCoords(
					ResultPosition.x, ResultPosition.y,
					ScreenSize
				) - glm::vec2(contentSize.x / 2, contentSize.y / 2)
			);
			if(Parent)
				EndScissor();
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