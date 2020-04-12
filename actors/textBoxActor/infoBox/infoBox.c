#include "infoBox.h"

static const char * const chooseObjectText = "press key and choose "
"object, 'p' : point, 'v' : vertical line, 'h' : "
"horizontal line, 'r' : rectangle '<space>' : run";

static const char * addObjectPointText = "adding another point object, "
"click to place";
static const char * addObjectVLineText = "adding another vertical line"
" object, click to place";
static const char * addObjectHLineText = "adding another horizontal line"
" object, click to place";
static const char * addObjectRectText = "adding another rectangle"
" object, click to place";
static const char * chooseVelocityText = "click to choose velocity";
static const char * chooseDimensionText = "click to choose dimension";
static const char * runningText = "running, press <space> to pause";
static const char * pausedText = "paused, press <space> to continue, "
"<right> to skip forward a frame, <ctrl>+<right> for 10 frames";

static const char * contentText = chooseObjectText;
static bool needsRefresh = true;

static void setText(const char * text)
{
    if (contentText == text)
    {
        return;
    }
    needsRefresh = true;
    contentText = text;
}

void setTextToAddObjectPoint()
{
    setText(addObjectPointText);
}

void setTextToAddObjectVLine()
{
    setText(addObjectVLineText);
}

void setTextToAddObjectHLine()
{
    setText(addObjectHLineText);
}

void setTextToAddObjectRect()
{
    setText(addObjectRectText);
}

void setTextToChooseVelocity()
{
    setText(chooseVelocityText);
}

void setTextToChooseDimensions()
{
    setText(chooseDimensionText);
}

void setTextToChooseObject()
{
    setText(chooseObjectText);
}

void setTextToRunning()
{
    setText(runningText);
}

void setTextToPaused()
{
    setText(pausedText);
}

void getInfoBoxText(textBoxActor * t, textReceiver tr)
{
    tr(contentText);
    return;
}

textProvider hasRefreshedInfoBoxText(textBoxActor * t)
{
    if (!needsRefresh)
    {
        return NULL;
    }
    needsRefresh = false;
    return getInfoBoxText;
}
