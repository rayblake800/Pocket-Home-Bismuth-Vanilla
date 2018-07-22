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

    class ColourListModel : public ListBoxModel,
            public ColourConfigFile::Listener
    {
    public:
        ColourListModel();
        virtual ~ColourListModel();
        int getNumRows() override;

        void listResized(ListBox& list);

        void paintListBoxItem(int rowNumber, Graphics& g, int width, int height,
                bool rowIsSelected) override;

        Component* refreshComponentForRow(int rowNumber, bool isRowSelected,
                Component* toUpdate) override;

        void selectedRowsChanged(int lastRowSelected) override;


    private:
        void colourValueChanged(int colourID, String colourKey,
                Colour newColour) override;
        int textHeight;
        StringArray colourKeys;
        Array<Colour> colours;
    };

    ColourListModel listModel;
    ListBox colourList;

};

