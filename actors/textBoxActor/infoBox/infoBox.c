#include "infoBox.h"

static const char * const addMainObjectText = "press key and choose object,"
" 'p' : point, 'v' : vertical line, 'h' : horizontal line";
static const char * addMainObjectPointText = "adding main point object";

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
    return getInfoBoxText;
}

