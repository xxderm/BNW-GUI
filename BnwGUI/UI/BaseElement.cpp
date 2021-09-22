#include <BnwGUI/UI/BaseElement.h>
#include <BnwGUI/UI/WindowElement.h>
#ifdef _WIN32
#include <Windows.h>
#endif // !_WIN32
#include  <gl/GL.h>

namespace BnwGUI
{
	namespace UI
	{
		const bool BaseElement::Hover(glm::vec2 mouse, glm::vec2 pos)
		{
			auto xmin = pos.x - Size.x;
			auto xmax = pos.x + Size.x;
			auto ymin = pos.y + Size.y;
			auto ymax = pos.y - Size.y;
			if (
				(mouse.x >= (xmin) &&
					mouse.x <= (xmax)) &&
				(mouse.y <= ymin && mouse.y >= ymax)
				)
			{
				return true;
			}
			return false;
		}

		const glm::vec2 BaseElement::GetResultPos()
		{
			auto ResultPosition = Position;
			if (Parent)
				ResultPosition = Parent->GetResultPos() + Position;
			return ResultPosition;
		}

		EventType BaseElement::HandleEvent(SDL_Event* e)
		{
			int x, y;
			SDL_GetMouseState(&x, &y);			
			if (e->type == SDL_MOUSEBUTTONDOWN)
			{
				Active = false;
				if (Parent)
				{
					if (Hover(
						GetNormalizedDeviceCoords(x, y, ScreenSize), Parent->GetResultPos() + Position)
						)
					{
						if (OnClickFunc)
							OnClickFunc();
						Active = true;
						return EventType::ON_CLICK_BUTTON;
					}
				}
				else
				{
					if (Hover(GetNormalizedDeviceCoords(x, y, ScreenSize), Position))
					{
						if (OnClickFunc)
							OnClickFunc();
						Active = true;
						return EventType::ON_CLICK_BUTTON;
					}
				}
			}
			return EventType::ON_VOID;
		}

		void BaseElement::BeginScissor(const glm::vec2& pos, const glm::vec2& size)
		{
			auto sciPos = NormalizedDevCoordsToWindowsCoords(pos.x, pos.y, ScreenSize);
			auto sciSize = NormalizedDevCoordsToWindowsCoords(size.x - (1.0f - size.x), size.y - (1.0f - size.y), ScreenSize);
			glEnable(GL_SCISSOR_TEST);
			glScissor( 
				static_cast<GLint>(sciPos.x - (sciSize.x / 2)), 
				static_cast<GLint>(sciPos.y - (sciSize.y / 2)), 
				static_cast<GLsizei>(sciSize.x), 
				static_cast<GLsizei>(sciSize.y));
		}

		void BaseElement::EndScissor()
		{
			glDisable(GL_SCISSOR_TEST);
		}

		void BaseElement::SetRenderer(BnwGUI::Render* Renderer)
		{
			this->Renderer = Renderer;
		}

		void BaseElement::SetParent(BaseElement *parent)
		{
			this->Parent = parent;
		}

		void BaseElement::OnClickEvent(std::function<void(void)> func)
		{
			OnClickFunc = func;
		}

	}
}