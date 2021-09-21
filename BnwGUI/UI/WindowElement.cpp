#include <BnwGUI/UI/WindowElement.h>
#include <BnwGUI/BnwGUI.h>

namespace BnwGUI
{
	namespace UI
	{

		WindowElement::WindowElement(
			std::string name, 
			glm::vec2 center, glm::vec2 scale,
			glm::vec4 color,
			ElementParameter parameter)
		{
			Format.Name = name;
			Position = center;
			Size = scale;
			Format.Color = color;
			Format.Parameter = parameter;
		}

		WindowElement::WindowElement(
			std::string name,
			glm::vec2 center, glm::vec2 scale,
			int texture, ElementParameter parameter)
		{
			Format.Name = name;
			Position = center;
			Size = scale;
			Format.Texture = texture;
			Format.Parameter = parameter;
		}

		WindowElement::WindowElement(WindowElementFormat& Format)
		{
			this->Format = Format;
		}

		void WindowElement::Render()
		{	
			Renderer->RenderRect(Position, Size, Format.Color, Format.Texture);

			switch (Format.Parameter)
			{
			case ElementParameter::UNTITLED_NO_EXIT_BUTTON:				
				break;
			case ElementParameter::TITLE_WITH_EXIT_BUTTON:		
				/* Render title */
				Renderer->RenderRect(
					glm::vec2(Position.x, Position.y + Size.y + HeaderHeight), 
					glm::vec2(Size.x, HeaderHeight),
					HeaderColor, Format.Texture
				);
				Renderer->RenderRect(
					glm::vec2(
						Position.x + (Size.x - (ExitScale.x + ExitScale.x)),
						Position.y + Size.y + HeaderHeight),
					ExitScale,
					ExitColor, Format.Texture
				);
				break;
			case ElementParameter::WITH_TITLE:
				/* Render title */
				Renderer->RenderRect(
					glm::vec2(Position.x, Position.y + Size.y + HeaderHeight),
					glm::vec2(Size.x, HeaderHeight),
					HeaderColor, Format.Texture
				);
				break;
			case ElementParameter::WITH_EXIT_BUTTON:
				Renderer->RenderRect(
					glm::vec2(Position.x, Position.y + Size.y + HeaderHeight),
					glm::vec2(Size.x, HeaderHeight),
					HeaderColor, Format.Texture
				);
				Renderer->RenderRect(
					glm::vec2(
						Position.x + (Size.x - (ExitScale.x + ExitScale.x)),
						Position.y + Size.y + HeaderHeight),
					ExitScale,
					ExitColor, Format.Texture
				);
				break;
			default:
				break;
			}

			for (auto& child : Childs)
				child->Render();
		}

		EventType WindowElement::HandleEvent(SDL_Event* e)
		{
			bool isHoverChild = false;
			for (auto& child : Childs)
			{				
				if (child->HandleEvent(e) == EventType::ON_CLICK_BUTTON)
					isHoverChild = true;
			}
			if(!isHoverChild)
				return BaseElement::HandleEvent(e);
			return EventType::ON_VOID;
		}

		void WindowElement::SetScreenSize(glm::vec2 ss)
		{
			BaseElement::SetScreenSize(ss);
			for (auto& child : Childs)
			{
				child->SetScreenSize(ss);
			}
		}

		void WindowElement::SetRenderer(BnwGUI::Render* Renderer)
		{
			this->Renderer = Renderer;
		}

		void WindowElement::operator+=(BaseElement *child)
		{
			Childs.push_back(child);
			Childs.back()->SetParent(this);
			Childs.back()->SetRenderer(this->Renderer);
			Childs.back()->SetScreenSize(this->ScreenSize);
		}

	}
}