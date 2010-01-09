#include "Character.h"

Character::Character(ResourceFile * resourceFile, std::string resourceName)
{

}

Character::~Character()
{

}

void Character::nextFrame()
{
    curAnimation->nextFrame();
}

