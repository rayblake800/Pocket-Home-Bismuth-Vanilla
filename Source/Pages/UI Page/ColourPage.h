/**
 * @file ColorPage.h
 */
#pragma once
#include "Configurable.h"
#include "PageComponent.h"
#include "SwitchComponent.h"

class ColourPage : public PageComponent
{
public:
    ColourPage();
    virtual ~ColourPage();
private:
    void pageResized() override;

    class ColourListModel : public ListBoxModel, public Configurable
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

        void loadConfigProperties(ConfigFile* config, String key) override;
        int textHeight;
        Array<String> colourKeys;
        Array<Colour> colours;
    };


    ColourListModel listModel;
    ListBox colourList;
    SwitchComponent testSwitch;

};

