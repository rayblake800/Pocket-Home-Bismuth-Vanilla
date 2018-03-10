#pragma once
#include "../../JuceLibraryCode/JuceHeader.h"
#include "PopupEditorComponent.h"

class PopupColorPicker : public PopupEditorComponent {
public:
    PopupColorPicker();
    virtual ~PopupColorPicker();
private:
    ColourSelector colorPicker;
};