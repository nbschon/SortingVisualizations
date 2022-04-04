//
// Created by Noah Schonhorn on 10/27/21.
//

#include "ListElement.h"

ListElement::ListElement(int value, SDL_FRect renderRect)
{
    this->value = value;
    this->renderRect = renderRect;
}

int ListElement::getValue() const
{
    return value;
}

void ListElement::setValue(int value)
{
    ListElement::value = value;
}

const SDL_FRect &ListElement::getRenderRect() const
{
    return renderRect;
}

void ListElement::setRenderRect(const SDL_FRect &renderRect)
{
    ListElement::renderRect = renderRect;
}


