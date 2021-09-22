#include <BnwGUI/BnwGUI.h>
#include "VScrollAreaElement.h"

namespace BnwGUI
{
	namespace UI
	{

		void VScrollAreaElement::Render()
		{
			Renderer->RenderRect(GetResultPos(), Size, { 1, 0, 0, 1 });
			for (auto& child : Childs)
				child->Render();
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