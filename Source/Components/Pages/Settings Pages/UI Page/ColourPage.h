/**
 * @file ColorPage.h
 */
#pragma once
#include "ColourConfigFile.h"
#include "ColourPicker.h"
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
        
        /**
         * Gets the text of a specific row item.
         * 
         * @param index  The index of the row to access.
         * 
         * @return  The text printed at the given row item index, or the empty
         *          string if the index is invalid.
         */
        juce::String getRowText(int index) const;

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
        virtual void colourChanged(const int colourID,
                const juce::Identifier& colourKey,
                const juce::Colour newColour) override;
        
        int textHeight;
        
        const juce::Array<juce::Identifier>& colourKeys;
        
        juce::Array<juce::Colour> colours;
    };

    ColourListModel listModel;
    juce::ListBox colourList;
    ColourPicker colourPicker;

};

