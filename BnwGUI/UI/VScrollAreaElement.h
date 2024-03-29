#ifndef VSCROLL_AREA_ELEMENT_H
#define VSCROLL_AREA_ELEMENT_H

#include "BaseElement.h"

namespace BnwGUI
{
	namespace UI
	{

		class VScrollAreaElement final : public BaseElement
		{
			std::vector<BaseElement*> Childs;
			glm::vec2 MoveAreaPos;
			glm::vec2 MoveAreaSize;
			glm::vec2 MoveButtonPos;
			glm::vec2 MoveButtonSize;
		public:

			void Render() override;

			EventType HandleEvent(SDL_Event* e) override;

			void SetScreenSize(glm::vec2 ss) override;

			void operator+=(BaseElement* child);

		};

	}
}

#endif