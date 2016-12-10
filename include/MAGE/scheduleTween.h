#pragma once
#include "schedules.h"
#include "easing.h"

namespace mage {

/*
Tween Task: call a function (using a hook<float>) every update frame with a varying value. Done using tween(), which returns a task.
(These need an easing function, found in easing.h - if none (nullptr) is given, they will just use Linear)
(If cancelled, a tween will not stop in the middle but skip to the end unless skipOnCancel is set to false)
*/
class MAGEDLL scheduleTween : public schedule {
public:
	scheduleTween(sf::Time toTake, unsigned int phases, easingFunction ef, std::function<void(float)> callee);
	scheduleTween(sf::Time toTake, unsigned int phases, easingFunction ef, std::function<void(float)> callee, std::function<bool()> stopCond);

	void start();
	void run(); // if it is time, call the hook and stop the task.
	void end(); // if ended, make sure the tween FINISHED before calling the original end() function.

	unsigned int getFrame();
	sf::Time getFrameLength();

private:
	void notifyOnTween(float val);

public:
	hook<float> onTween;

	easingFunction easing;

	sf::Time length;
	unsigned int numFrames;

	bool skipOnEnd;

	float tLast;
};

} // namespace mage