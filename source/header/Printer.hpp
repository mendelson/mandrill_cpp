#ifndef PRINTER_h
#define PRINTER_H

// #include "Observer.h"
// #include "FramesManager.h"

class Printer : public Observer
{
public:
	Printer();
	~Printer();
	void Update(FramesManager* theChangedSubject);
	void setSubject(FramesManager* subject);

private:
	void showLastFrameIndex();
};

#endif