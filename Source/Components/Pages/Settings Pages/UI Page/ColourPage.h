/**
 * @file ColorPage.h
 */
#pragma once
#include "ColourConfigFile.h"
#include "PageComponent.h"

class ColourPage : public PageComponent
{
public:
    ColourPage();

    virtual ~ColourPage() { }
private:
    void pageResized() override;

    class ColourListModel : public juce::ListBoxModel,
            public ColourConfigFile::Listener
    {
    public:
        ColourListModel();
        virtual ~ColourListModel();
        
        int getNumRows() override;

        void listResized(juce::ListBox& list);

        void paintListBoxItem(
                int rowNumber,
                juce::Graphics& g,
                int width,
                int height,
                bool rowIsSelected) override;

        juce::Component* refreshComponentForRow
        (int rowNumber, bool isRowSelected, juce::Component* toUpdate) override;

        void selectedRowsChanged(int lastRowSelected) override;


    private:
        void colourValueChanged(int colourID, juce::String colourKey,
                juce::Colour newColour) override;
        
        int textHeight;
        
        juce::StringArray colourKeys;
        
        juce::Array<juce::Colour> colours;
    };

    ColourListModel listModel;
    juce::ListBox colourList;

};

