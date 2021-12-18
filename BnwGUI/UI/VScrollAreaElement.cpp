#include <BnwGUI/BnwGUI.h>
#include "VScrollAreaElement.h"

namespace BnwGUI
{
	namespace UI
	{

		void VScrollAreaElement::Render()
		{
			/* Main area */
			Renderer->RenderRect(GetResultPos(), Size, { 0.4f, 0.4f, 0.4f, 1 });
			
			/* Move area */
			MoveAreaSize = { 0.01, Size.y };
			MoveAreaPos = GetResultPos() + glm::vec2(Size.x - MoveAreaSize.x, 0.f);
			Renderer->RenderRect(MoveAreaPos, MoveAreaSize, { 0.6f, 0.6f, 0.6f, 1 });

			/* Move button */
			MoveButtonSize = { 0.01, 0.02 };
			MoveButtonPos = GetResultPos() + glm::vec2(Size.x - MoveButtonSize.x, 0.f);
			Renderer->RenderRect(MoveButtonPos, MoveButtonSize, { 0.8f, 0.8f, 0.8f, 1 });

			BeginStencil(GetResultPos(), Size - glm::vec2(0.02, 0));
			for (auto& child : Childs)
			{		
				child->Render();
			}			
			EndStencil(GetResultPos(), Size - glm::vec2(0.02, 0));
		}

		void VScrollAreaElement::SetScreenSize(glm::vec2 ss)
		{
			BaseElement::SetScreenSize(ss);
			for (auto& child : Childs)
			{
				child->SetScreenSize(ss);
			}
		}

		EventType VScrollAreaElement::HandleEvent(SDL_Event* e)
		{
			bool isHoverChild = false;
			for (auto& child : Childs)
			{
				if (child->HandleEvent(e) == EventType::ON_CLICK_BUTTON)
					isHoverChild = true;
			}
			if (!isHoverChild)
				return BaseElement::HandleEvent(e);
			return EventType::ON_VOID;
		}

		void VScrollAreaElement::operator+=(BaseElement* child)
		{
			Childs.push_back(child);
			Childs.back()->SetParent(this);
			Childs.back()->SetRenderer(this->Renderer);
			Childs.back()->SetScreenSize(this->ScreenSize);
		}

	}
}