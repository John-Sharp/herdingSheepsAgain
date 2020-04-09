#ifndef INFO_BOX_H
#define INFO_BOX_H

#include "../textBoxActor.h"

textProvider hasRefreshedInfoBoxText(textBoxActor * t);
void setTextToAddMainObject();
void setTextToAddMainObjectPoint();
void setTextToAddMainObjectVLine();
void setTextToAddMainObjectHLine();
void setTextToAddOtherObjectPoint();
void setTextToAddOtherObjectVLine();
void setTextToAddOtherObjectHLine();
void setTextToChooseVelocity();
void setTextToChooseDimensions();
void setTextToChooseOtherObject();
void setTextToRunning();
void setTextToPaused();
#endif

