//
// Created by Noah Schonhorn on 10/27/21.
//

#ifndef SORTINGVISUALIZATION_LISTELEMENT_H
#define SORTINGVISUALIZATION_LISTELEMENT_H

#include "SDL.h"

class ListElement
{
private:
    int value;
    SDL_FRect renderRect;

public:
    ListElement(int value, SDL_FRect renderRect);

    int getValue() const;

    void setValue(int value);

    const SDL_FRect &getRenderRect() const;

    void setRenderRect(const SDL_FRect &renderRect);

    friend bool operator == (const ListElement& element1, const ListElement& element2)
    {
        return element1.value == element2.value;
    }

    friend bool operator != (const ListElement& element1, const ListElement& element2)
    {
        return element1.value != element2.value;
    }

    friend bool operator < (const ListElement& element1, const ListElement& element2)
    {
        return element1.value < element2.value;
    }

    friend bool operator > (const ListElement& element1, const ListElement& element2)
    {
        return element1.value > element2.value;
    }

    friend bool operator <= (const ListElement& element1, const ListElement& element2)
    {
        return element1.value <= element2.value;
    }

    friend bool operator >= (const ListElement& element1, const ListElement& element2)
    {
        return element1.value >= element2.value;
    }
};


#endif //SORTINGVISUALIZATION_LISTELEMENT_H
