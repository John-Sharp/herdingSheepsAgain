#include "infoBox.h"

static const char * const addMainObjectText = "press key and choose object,"
" 'p' : point, 'v' : vertical line, 'h' : horizontal line";
static const char * addMainObjectPointText = "adding main point object, "
"click to place";
static const char * chooseVelocityText = "click to choose velocity";

static const char * contentText = addMainObjectText;
static bool needsRefresh = true;

void setTextToAddMainObject()
{
    if (contentText == addMainObjectText)
    {
        return;
    }
    needsRefresh = true;
    contentText = addMainObjectText;
}

void setTextToAddMainObjectPoint()
{
    if (contentText == addMainObjectPointText)
    {
        return;
    }
    needsRefresh = true;
    contentText = addMainObjectPointText;
}

void setTextToChooseVelocity()
{
    if (contentText == chooseVelocityText)
    {
        return;
    }
    needsRefresh = true;
    contentText = chooseVelocityText;
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

