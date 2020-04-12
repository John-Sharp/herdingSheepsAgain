#ifndef INFO_BOX_H
#define INFO_BOX_H

#include "../textBoxActor.h"

textProvider hasRefreshedInfoBoxText(textBoxActor * t);
void setTextToChooseObject();
void setTextToAddObjectPoint();
void setTextToAddObjectVLine();
void setTextToAddObjectHLine();
void setTextToAddObjectRect();
void setTextToChooseVelocity();
void setTextToChooseDimensions();
void setTextToRunning();
void setTextToPaused();
#endif

