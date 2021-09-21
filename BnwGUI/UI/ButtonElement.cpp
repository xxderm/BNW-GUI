#include <BnwGUI/UI/ButtonElement.h>
#include <BnwGUI/UI/WindowElement.h>
#include <BnwGUI/BnwGUI.h>

namespace BnwGUI
{
	namespace UI
	{

		void ButtonElement::Render()
		{
			auto ResultPosition = BaseElement::GetResultPos();

			auto contentSize = Renderer->GetTextSize(Content,  (float)TextSize / (float)Renderer->GetMaxFontSize(), true);

			this->Renderer->RenderRect(
				ResultPosition,
				Size,
				ButtonColor
			);

						
			this->BeginScissor(ResultPosition, Size);
			this->Renderer->RenderText(
				Content,
				TextColor,  (float)TextSize / (float)Renderer->GetMaxFontSize(),
				NormalizedDevCoordsToWindowsCoords(
					ResultPosition.x, ResultPosition.y,
					ScreenSize
				) - glm::vec2(contentSize.x / 2, contentSize.y / 2)
			);
			this->EndScissor();

		}

		EventType ButtonElement::HandleEvent(SDL_Event* e)
		{
			return BaseElement::HandleEvent(e);
		}

	}
}