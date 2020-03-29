#include "infoBox.h"

static const char * const addMainObjectText = "press key and choose object,"
" 'p' : point, 'v' : vertical line, 'h' : horizontal line";
static const char * addMainObjectPointText = "adding main point object, "
"click to place";
static const char * chooseVelocityText = "click to choose velocity";

static const char * contentText = addMainObjectText;
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

void setTextToAddMainObject()
{
    setText(addMainObjectText);
}

void setTextToAddMainObjectPoint()
{
    setText(addMainObjectPointText);
}

void setTextToChooseVelocity()
{
    setText(chooseVelocityText);
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
