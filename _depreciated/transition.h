#pragma once

// TODO

// derivable class for transitions between maps/levels
// default transition (transition) uses the same technique as boxWindow
// derive and pass to certain Game functions to change
// (by default there is no transition between game states)
/*
class MAGEDLL transition :
public sf::Drawable
{
transition(Game* gm);

virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

bool isBlockingGameView();
void block(float multiplier = 1.f);
void unblock(float multiplier = 1.f);
public:
float percentComplete;
};
*/