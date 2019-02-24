#pragma once
/**
 * @file  Theme_Colour_ConfigPage.h
 *
 * @brief  Provides user controls for editing configurable UI colors.
 */

#include "Theme_Colour_ConfigListener.h"
#include "ColourPicker.h"
#include "Page_Component.h"

namespace Theme { namespace Colour { class ConfigPage; } }

class Theme::Colour::ConfigPage : public Page::Component
{
public:
    ConfigPage();

    virtual ~ConfigPage() { }
private:
    void pageResized() override;

    class ColourListModel : public juce::ListBoxModel,
            public Theme::Colour::ConfigListener
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
        /**
         * @brief Listens for updates to generic UI element categories.
         *
         * @param key  The key of an updated UI element category.
         */
        virtual void configValueChanged(const juce::Identifier& key) override;

        /**
         * @brief Listens for updates to all tracked juce ColorId values.
         *
         * @param colourId   The updated ColorId value.
         *
         * @param colourKey  The key to either the updated color, or to its
         *                   generic category color if its specific color isn't
         *                   set.
         *
         * @param newColour  The new color value.
         */
        virtual void colourChanged(const int colourId,
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

