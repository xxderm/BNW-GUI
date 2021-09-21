#include "VScrollAreaElement.h"

namespace BnwGUI
{
	namespace UI
	{

		void VScrollAreaElement::operator+=(BaseElement* child)
		{
			Childs.push_back(child);
			Childs.back()->SetParent(this);
			Childs.back()->SetRenderer(this->Renderer);
			Childs.back()->SetScreenSize(this->ScreenSize);
		}

	}
}