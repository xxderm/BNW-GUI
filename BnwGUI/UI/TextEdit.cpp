#include <BnwGUI/UI/TextEdit.h>
#include <BnwGUI/BnwGUI.h>

namespace BnwGUI
{
	namespace UI
	{

		TextEditElement::TextEditElement()
		{
			state = SDL_GetKeyboardState(NULL);
			Position = { 0, 0 };
			Size = { 0.1, 0.03 };
		}

		void TextEditElement::Render()
		{
			if (!Text.empty())
			{
				ContentSize = Renderer->GetTextSize(Text, (float)TextSize / (float)Renderer->GetMaxFontSize(), 1);
				TextScale = GetNormalizedDeviceCoords(
					static_cast<int>((ScreenSize.x / 2.f) + (ContentSize.x / 2.f)), 
					static_cast<int>((ScreenSize.y / 2.f) - (ContentSize.y / 2.f + 2)),
					ScreenSize);
			}

			// Render input area
			this->Renderer->RenderRect(
				BaseElement::GetResultPos(),
				Size,
				InputAreaColor
			);

			auto resPos = BaseElement::GetResultPos();
			TextPos = glm::vec2( (resPos.x - Size.x + TextScale.x + 0.0085), resPos.y);

			if (TextScale.x >= Size.x)
			{
				//TextPos.x = resPos.x - TextScale.x + Size.x;
				auto textCenter = TextScale.x;
				auto offset = textCenter - Size.x;
				TextPos.x = ( resPos.x - offset ) - 0.01f;
			}

			glm::vec2 ResultTextPos = NormalizedDevCoordsToWindowsCoords(
				TextPos.x, TextPos.y,
				ScreenSize
			) - glm::vec2(ContentSize.x / 2, ContentSize.y / 2);

			// Render text
			if (!Text.empty())
			{
				BeginScissor(BaseElement::GetResultPos() + glm::vec2(0.0025, 0.), Size - glm::vec2(0.0025, 0.));
				(float)this->Renderer->RenderText(
					Text,
					TextColor, 
					static_cast<float>(TextSize) / static_cast<float>(Renderer->GetMaxFontSize()),
					ResultTextPos
				);
				EndScissor();
			}

			CurrentCharIndex = Text.size() - 1;
		}

		EventType TextEditElement::HandleEvent(SDL_Event* e)
		{
			auto startPos = Position;
			Position.y += -TextScale.y;
			auto toReturn = BaseElement::HandleEvent(e);
			Position = startPos;

			if (BaseElement::Active)
			{
				if (e->type == SDL_TEXTINPUT)
				{
					std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
					std::wstring wide = converter.from_bytes(e->text.text);
					Text += wide;
				}

				if (e->type == SDL_KEYDOWN)
				{
					if (e->key.keysym.sym == SDLK_BACKSPACE)
						if (!Text.empty())
							Text.erase(CurrentCharIndex);
				}

				if (state[SDL_SCANCODE_LCTRL] && state[SDL_SCANCODE_C])
					printf(":RET");

				if (state[SDL_SCANCODE_LCTRL] && state[SDL_SCANCODE_V])
					printf(":INS");
			}

			return toReturn;
		}

	}
}