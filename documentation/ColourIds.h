namespace ColourIds
{
    enum midiKeyboardComponent
    {
        whiteNote              = 0x1005000,
        blackNote              = 0x1005001,
        keySeparatorLine       = 0x1005002,
        mouseOverKeyOverlay    = 0x1005003,
        keyDownOverlay         = 0x1005004,
        textLabel              = 0x1005005,
        upDownButtonBackground = 0x1005006,
        upDownButtonArrow      = 0x1005007,
        shadow                 = 0x1005008
    };

    enum bubbleComponent
    {
        background = 0x1000af0,
        /**< The colour to use for an outline around the bubble. */
        outline    = 0x1000af1
    };

    enum fileChooserDialogBox
    {
        titleText = 0x1000850
    };

    enum directoryContentsDisplayComponent
    {
        highlight       = 0x1000540,
        text            = 0x1000541,
        /**< The colour with which to draw the text in highlighted sections. */
        highlightedText = 0x1000542
    };

    enum fileBrowserComponent
    {
        currentPathBoxBackground = 0x1000640,
        currentPathBoxText       = 0x1000641,
        currentPathBoxArrow      = 0x1000642,
        filenameBoxBackground    = 0x1000643,
        /**< The colour to use for the text of the filename TextEditor. */
        filenameBoxText          = 0x1000644
    };

    enum fileSearchPathListComponent
    {
        background = 0x1004100
    };

    enum propertyComponent
    {
        background = 0x1008300,
        labelText  = 0x1008301
    };

    enum textPropertyComponent
    {
        background = 0x100e401,
        text       = 0x100e402,
        outline    = 0x100e403
    };

    enum booleanPropertyComponent
    {
        background = 0x100e801,
        outline    = 0x100e803
    };

    enum groupComponent
    {
        outline = 0x1005400,
        /**< The colour to use to draw the text label. */
        text    = 0x1005410
    };

    enum tabbedComponent
    {
        background = 0x1005800,
        outline    = 0x1005801
    };

    enum tabbedButtonBar
    {
        tabOutline   = 0x1005812,
        tabText      = 0x1005813,
        frontOutline = 0x1005814,
        frontText    = 0x1005815
    };

    enum scrollBar
    {
        background = 0x1000300,
        thumb      = 0x1000400,
        /**< A base colour to use for the slot area of the bar. The look and feel will probably use variations on this colour. */
        track      = 0x1000401
    };

    enum sidePanel
    {
        background          = 0x100f001,
        titleText           = 0x100f002,
        shadowBase          = 0x100f003,
        dismissButtonNormal = 0x100f004,
        dismissButtonOver   = 0x100f004,
        dismissButtonDown   = 0x100f005
    };

    enum progressBar
    {
        background = 0x1001900,
        foreground = 0x1001a00
    };

    enum treeView
    {
        background             = 0x1000500,
        lines                  = 0x1000501,
        dragAndDropIndicator   = 0x1000502,
        selectedItemBackground = 0x1000503,
        oddItems               = 0x1000504,
        /**< The colour to use to fill the backround of the even numbered items. */
        evenItems              = 0x1000505
    };

    enum comboBox
    {
        background     = 0x1000b00,
        text           = 0x1000a00,
        outline        = 0x1000c00,
        button         = 0x1000d00,
        arrow          = 0x1000e00,
        /**< The colour that will be used to draw a box around the edge of the component when it has focus. */
        focusedOutline = 0x1000f00
    };

    enum textEditor
    {
        background      = 0x1000200,
        text            = 0x1000201,
        highlight       = 0x1000202,
        highlightedText = 0x1000203,
        outline         = 0x1000205,
        focusedOutline  = 0x1000206,
        shadow          = 0x1000207
    };

    enum label
    {
        background            = 0x1000280,
        text                  = 0x1000281,
        outline               = 0x1000282,
        backgroundWhenEditing = 0x1000283,
        textWhenEditing       = 0x1000284,
        /**< An optional border colour when the label is being edited. */
        outlineWhenEditing    = 0x1000285
    };

    enum tableHeaderComponent
    {
        text       = 0x1003800,
        background = 0x1003810,
        outline    = 0x1003820,
        highlight  = 0x1003830
    };

    enum listBox
    {
        background = 0x1002800,
        outline    = 0x1002810,
        /**< The preferred colour to use for drawing text in the listbox. */
        text       = 0x1002820
    };

    enum toolbar
    {
        background                = 0x1003200,
        separator                 = 0x1003210,
        buttonMouseOverBackground = 0x1003220,
        buttonMouseDownBackground = 0x1003230,
        labelText                 = 0x1003240,
        /**< A colour to use for an outline around buttons when
                                                       the customisation dialog is active and the mouse moves over them. */
        editingModeOutline        = 0x1003250
    };

    enum slider
    {
        background          = 0x1001200,
        thumb               = 0x1001300,
        track               = 0x1001310,
        rotarySliderFill    = 0x1001311,
        rotarySliderOutline = 0x1001312,
        textBoxText         = 0x1001400,
        textBoxBackground   = 0x1001500,
        textBoxHighlight    = 0x1001600,
        /**< The colour to use for a border around the text-editor box. */
        textBoxOutline      = 0x1001700
    };

    enum tooltipWindow
    {
        background = 0x1001b00,
        text       = 0x1001c00,
        /**< The colour to use to draw an outline around the tooltip. */
        outline    = 0x1001c10
    };

    enum alertWindow
    {
        background = 0x1001800,
        text       = 0x1001810,
        /**< An optional colour to use to draw a border around the window. */
        outline    = 0x1001820
    };

    enum documentWindow
    {
        text = 0x1005701
    };

    enum resizableWindow
    {
        background = 0x1005700
    };

    enum popupMenu
    {
        background            = 0x1000700,
        text                  = 0x1000600,
        headerText            = 0x1000601,
        highlightedBackground = 0x1000900,
        highlightedText       = 0x1000800
    };

    enum lassoComponent
    {
        lassoFill    = 0x1000440,
        lassoOutline = 0x1000441
    };

    enum caretComponent
    {
        caret = 0x1000204
    };

    enum toggleButton
    {
        text         = 0x1006501,
        tick         = 0x1006502,
        /**< The colour to use for the disabled tick mark. */
        tickDisabled = 0x1006503
    };

    enum textButton
    {
        button   = 0x1000100,
        buttonOn = 0x1000101,
        textOff  = 0x1000102,
        /**< The colour to use for the button's text.when the button's toggle state is "on". */
        textOn   = 0x1000103
    };

    enum hyperlinkButton
    {
        text = 0x1001f00
    };

    enum drawableButton
    {
        text         = 0x1004010,
        textOn       = 0x1004013,
        background   = 0x1004011,
        backgroundOn = 0x1004012
    };

    enum colourSelector
    {
        background = 0x1007000,
        /**< the colour used for the labels next to the sliders. */
        labelText  = 0x1007001
    };

    enum keyMappingEditorComponent
    {
        background = 0x100ad00,
        text       = 0x100ad01
    };

    enum codeEditorComponent
    {
        background           = 0x1004500,
        highlight            = 0x1004502,
        defaultText          = 0x1004503,
        lineNumberBackground = 0x1004504,
        lineNumberText       = 0x1004505
    };

    enum jucer_CommonHeaders
    {
        background                = 0x2340000,
        secondaryBackground       = 0x2340001,
        defaultText               = 0x2340002,
        widgetText                = 0x2340003,
        defaultButtonBackground   = 0x2340004,
        secondaryButtonBackground = 0x2340005,
        userButtonBackground      = 0x2340006,
        defaultIcon               = 0x2340007,
        treeIcon                  = 0x2340008,
        defaultHighlight          = 0x2340009,
        defaultHighlightedText    = 0x234000a,
        codeEditorLineNumber      = 0x234000b,
        activeTabIcon             = 0x234000c,
        inactiveTabBackground     = 0x234000d,
        inactiveTabIcon           = 0x234000e,
        contentHeaderBackground   = 0x234000f,
        widgetBackground          = 0x2340010,
        secondaryWidgetBackground = 0x2340011
    };

    enum pageComponent
    {
        background = 0x1900500
    };

    enum overlaySpinner
    {
        background = 0x1900400,
        text       = 0x1900401
    };

    enum drawableImageButton
    {
        image0 = 0x1900000,
        image1 = 0x1900001,
        image2 = 0x1900002,
        image3 = 0x1900003,
        image4 = 0x1900004
    };

    enum drawableImageComponent
    {
        image0 = 0x1900000,
        image1 = 0x1900001,
        image2 = 0x1900002,
        image3 = 0x1900003,
        image4 = 0x1900004
    };

    enum switchComponent
    {
        background = 0x100f000,
        handle     = 0x100f001,
        handleOff  = 0x100f002
    };

    enum fileSelectTextEditor
    {
        fileWindow = 0x1900300,
        text       = 0x1900301
    };

    enum listEditor
    {
        background       = 0x1900100,
        listItem         = 0x1900101,
        selectedListItem = 0x1900102,
        text             = 0x1900103
    };

    enum appMenuButton
    {
        text       = 0x1900200,
        background = 0x1900201,
        selection  = 0x1900202,
        border     = 0x1900203
    };

};