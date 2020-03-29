#include "infoBox.h"

static const char * const addMainObjectText = "press key and choose object,"
" 'p' : point, 'v' : vertical line, 'h' : horizontal line";
static const char * const chooseOtherObjectText = "press key and choose "
"another object, 'p' : point, 'v' : vertical line, 'h' : "
"horizontal line, '<space>' : run";

static const char * addMainObjectPointText = "adding main point object, "
"click to place";
static const char * addMainObjectVLineText = "adding main vertical line"
" object, click to place";
static const char * addMainObjectHLineText = "adding main horizontal line"
" object, click to place";
static const char * chooseVelocityText = "click to choose velocity";
static const char * chooseDimensionText = "click to choose dimension";

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

void setTextToAddMainObjectVLine()
{
    setText(addMainObjectVLineText);
}

void setTextToAddMainObjectHLine()
{
    setText(addMainObjectHLineText);
}

void setTextToChooseVelocity()
{
    setText(chooseVelocityText);
}

void setTextToChooseDimensions()
{
    setText(chooseDimensionText);
}

void setTextToChooseOtherObject()
{
    setText(chooseOtherObjectText);
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
