#ifndef ANALYSER_H
#define ANALYSER_H

// #include "Observer.h"
// #include "FramesManager.h"

class Analyser : public Observer
{
public:
	Analyser();
	~Analyser();
	void Update();
	// void setSubject(FramesManager* subject);

private:
	void showLastFrameIndex();
};

#endif
