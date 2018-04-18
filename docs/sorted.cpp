Array<int> getIdsForDefault(ComponentConfigFile::DefaultColors defaultColor)
{
    using namespace ColourIds;
    switch(defaultColor)
    {
    case windowBackground:
        return
        {
            tabbedComponent::background,
            sidePanel::background,
            treeView::background,
            tooltipWindow::background,
            alertWindow::background,
            resizableWindow::background,
            colourSelector::background,
            keyMappingEditorComponent::background,
            jucer_CommonHeaders::background,
            pageComponent::background,
            jucer_CommonHeaders::contentHeaderBackground,
            fileSelectTextEditor::fileWindow,
            jucer_CommonHeaders::inactiveTabBackground
        };

    case widgetBackground:
        return
        {
            scrollBar::background,
            progressBar::background,
            slider::background,
            drawableButton::background,
            overlaySpinner::background,
            switchComponent::background,
            appMenuButton::background,
            drawableButton::backgroundOn,
            textButton::button,
            toolbar::buttonMouseOverBackground,
            fileBrowserComponent::currentPathBoxBackground,
            jucer_CommonHeaders::defaultButtonBackground,
            jucer_CommonHeaders::secondaryBackground,
            jucer_CommonHeaders::userButtonBackground,
            jucer_CommonHeaders::widgetBackground
        };

    case widgetOff:
        return
        {
            comboBox::button,
            jucer_CommonHeaders::defaultIcon,
            sidePanel::dismissButtonDown,
            switchComponent::handleOff,
            jucer_CommonHeaders::inactiveTabIcon,
            toggleButton::tickDisabled,
            scrollBar::track,
            slider::track
        };

    case widgetOn:
        return
        {
            jucer_CommonHeaders::activeTabIcon,
            toolbar::buttonMouseDownBackground,
            sidePanel::dismissButtonNormal,
            sidePanel::dismissButtonOver,
            progressBar::foreground,
            switchComponent::handle,
            slider::rotarySliderFill,
            scrollBar::thumb,
            slider::thumb,
            toggleButton::tick,
            jucer_CommonHeaders::treeIcon
        };

    case menuBackground:
        return
        {
            bubbleComponent::background,
            fileSearchPathListComponent::background,
            propertyComponent::background,
            comboBox::background,
            tableHeaderComponent::background,
            listBox::background,
            toolbar::background,
            popupMenu::background,
            listEditor::background,
            jucer_CommonHeaders::secondaryButtonBackground
        };

    case outline:
        return
        {
            appMenuButton::border,
            toolbar::editingModeOutline,
            tabbedButtonBar::frontOutline,
            lassoComponent::lassoOutline,
            bubbleComponent::outline,
            textPropertyComponent::outline,
            booleanPropertyComponent::outline,
            groupComponent::outline,
            tabbedComponent::outline,
            comboBox::outline,
            textEditor::outline,
            label::outline,
            tableHeaderComponent::outline,
            listBox::outline,
            tooltipWindow::outline,
            alertWindow::outline,
            slider::rotarySliderOutline,
            tabbedButtonBar::tabOutline,
            slider::textBoxOutline
        };

    case focusedOutline:
        return
        {
            comboBox::focusedOutline,
            textEditor::focusedOutline,
            label::outlineWhenEditing,
            toolbar::separator
        };

    case textField:
        return
        {
            textPropertyComponent::background,
            booleanPropertyComponent::background,
            textEditor::background,
            codeEditorComponent::background,
            label::backgroundWhenEditing,
            fileBrowserComponent::filenameBoxBackground,
            listEditor::listItem,
            jucer_CommonHeaders::secondaryWidgetBackground,
            slider::textBoxBackground
        };

    case text:
        return
        {
            comboBox::arrow,
            fileBrowserComponent::currentPathBoxArrow,
            fileBrowserComponent::currentPathBoxText,
            codeEditorComponent::defaultText,
            jucer_CommonHeaders::defaultText,
            treeView::dragAndDropIndicator,
            treeView::evenItems,
            fileBrowserComponent::filenameBoxText,
            tabbedButtonBar::frontText,
            popupMenu::headerText,
            propertyComponent::labelText,
            toolbar::labelText,
            colourSelector::labelText,
            lassoComponent::lassoFill,
            treeView::lines,
            treeView::oddItems,
            tabbedButtonBar::tabText,
            directoryContentsDisplayComponent::text,
            textPropertyComponent::text,
            groupComponent::text,
            comboBox::text,
            textEditor::text,
            label::text,
            tableHeaderComponent::text,
            listBox::text,
            tooltipWindow::text,
            alertWindow::text,
            documentWindow::text,
            popupMenu::text,
            toggleButton::text,
            hyperlinkButton::text,
            drawableButton::text,
            keyMappingEditorComponent::text,
            overlaySpinner::text,
            fileSelectTextEditor::text,
            listEditor::text,
            appMenuButton::text,
            slider::textBoxText,
            textButton::textOff,
            textButton::textOn,
            drawableButton::textOn,
            label::textWhenEditing,
            fileChooserDialogBox::titleText,
            sidePanel::titleText,
            jucer_CommonHeaders::widgetText
        };

    case highlightedTextField:
        return
        {
            textButton::buttonOn,
            jucer_CommonHeaders::defaultHighlight,
            directoryContentsDisplayComponent::highlight,
            textEditor::highlight,
            tableHeaderComponent::highlight,
            codeEditorComponent::highlight,
            popupMenu::highlightedBackground,
            codeEditorComponent::lineNumberBackground,
            treeView::selectedItemBackground,
            listEditor::selectedListItem,
            appMenuButton::selection,
            slider::textBoxHighlight
        };

    case highlightedText:
        return
        {
            caretComponent::caret,
            jucer_CommonHeaders::codeEditorLineNumber,
            jucer_CommonHeaders::defaultHighlightedText,
            directoryContentsDisplayComponent::highlightedText,
            textEditor::highlightedText,
            popupMenu::highlightedText,
            codeEditorComponent::lineNumberText
        };

    default:
        return {};
    }
}
