namespace ColourIds
{
    namespace midiKeyboardComponent
    {
        enum
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
    };

    namespace bubbleComponent
    {
        enum
        {
            background = 0x1000af0,
            /**< The colour to use for an outline around the bubble. */
            outline    = 0x1000af1
        };
    };

    namespace fileChooserDialogBox
    {
        enum
        {
            titleText = 0x1000850
        };
    };

    namespace directoryContentsDisplayComponent
    {
        enum
        {
            highlight       = 0x1000540,
            text            = 0x1000541,
            /**< The colour with which to draw the text in highlighted sections. */
            highlightedText = 0x1000542
        };
    };

    namespace fileBrowserComponent
    {
        enum
        {
            currentPathBoxBackground = 0x1000640,
            currentPathBoxText       = 0x1000641,
            currentPathBoxArrow      = 0x1000642,
            filenameBoxBackground    = 0x1000643,
            /**< The colour to use for the text of the filename TextEditor. */
            filenameBoxText          = 0x1000644
        };
    };

    namespace fileSearchPathListComponent
    {
        enum
        {
            background = 0x1004100
        };
    };

    namespace propertyComponent
    {
        enum
        {
            background = 0x1008300,
            labelText  = 0x1008301
        };
    };

    namespace textPropertyComponent
    {
        enum
        {
            background = 0x100e401,
            text       = 0x100e402,
            outline    = 0x100e403
        };
    };

    namespace booleanPropertyComponent
    {
        enum
        {
            background = 0x100e801,
            outline    = 0x100e803
        };
    };

    namespace groupComponent
    {
        enum
        {
            outline = 0x1005400,
            /**< The colour to use to draw the text label. */
            text    = 0x1005410
        };
    };

    namespace tabbedComponent
    {
        enum
        {
            background = 0x1005800,
            outline    = 0x1005801
        };
    };

    namespace tabbedButtonBar
    {
        enum
        {
            tabOutline   = 0x1005812,
            tabText      = 0x1005813,
            frontOutline = 0x1005814,
            frontText    = 0x1005815
        };
    };

    namespace scrollBar
    {
        enum
        {
            background = 0x1000300,
            thumb      = 0x1000400,
            /**< A base colour to use for the slot area of the bar. The look and feel will probably use variations on this colour. */
            track      = 0x1000401
        };
    };

    namespace sidePanel
    {
        enum
        {
            background          = 0x100f001,
            titleText           = 0x100f002,
            shadowBase          = 0x100f003,
            dismissButtonNormal = 0x100f004,
            dismissButtonOver   = 0x100f004,
            dismissButtonDown   = 0x100f005
        };
    };

    namespace progressBar
    {
        enum
        {
            background = 0x1001900,
            foreground = 0x1001a00
        };
    };

    namespace treeView
    {
        enum
        {
            background             = 0x1000500,
            lines                  = 0x1000501,
            dragAndDropIndicator   = 0x1000502,
            selectedItemBackground = 0x1000503,
            oddItems               = 0x1000504,
            /**< The colour to use to fill the backround of the even numbered items. */
            evenItems              = 0x1000505
        };
    };

    namespace comboBox
    {
        enum
        {
            background     = 0x1000b00,
            text           = 0x1000a00,
            outline        = 0x1000c00,
            button         = 0x1000d00,
            arrow          = 0x1000e00,
            /**< The colour that will be used to draw a box around the edge of the component when it has focus. */
            focusedOutline = 0x1000f00
        };
    };

    namespace textEditor
    {
        enum
        {
            background      = 0x1000200,
            text            = 0x1000201,
            highlight       = 0x1000202,
            highlightedText = 0x1000203,
            outline         = 0x1000205,
            focusedOutline  = 0x1000206,
            shadow          = 0x1000207
        };
    };

    namespace label
    {
        enum
        {
            background            = 0x1000280,
            text                  = 0x1000281,
            outline               = 0x1000282,
            backgroundWhenEditing = 0x1000283,
            textWhenEditing       = 0x1000284,
            /**< An optional border colour when the label is being edited. */
            outlineWhenEditing    = 0x1000285
        };
    };

    namespace tableHeaderComponent
    {
        enum
        {
            text       = 0x1003800,
            background = 0x1003810,
            outline    = 0x1003820,
            highlight  = 0x1003830
        };
    };

    namespace listBox
    {
        enum
        {
            background = 0x1002800,
            outline    = 0x1002810,
            /**< The preferred colour to use for drawing text in the listbox. */
            text       = 0x1002820
        };
    };

    namespace toolbar
    {
        enum
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
    };

    namespace slider
    {
        enum
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
    };

    namespace tooltipWindow
    {
        enum
        {
            background = 0x1001b00,
            text       = 0x1001c00,
            /**< The colour to use to draw an outline around the tooltip. */
            outline    = 0x1001c10
        };
    };

    namespace alertWindow
    {
        enum
        {
            background = 0x1001800,
            text       = 0x1001810,
            /**< An optional colour to use to draw a border around the window. */
            outline    = 0x1001820
        };
    };

    namespace documentWindow
    {
        enum
        {
            text = 0x1005701
        };
    };

    namespace resizableWindow
    {
        enum
        {
            background = 0x1005700
        };
    };

    namespace popupMenu
    {
        enum
        {
            background            = 0x1000700,
            text                  = 0x1000600,
            headerText            = 0x1000601,
            highlightedBackground = 0x1000900,
            highlightedText       = 0x1000800
        };
    };

    namespace lassoComponent
    {
        enum
        {
            lassoFill    = 0x1000440,
            lassoOutline = 0x1000441
        };
    };

    namespace caretComponent
    {
        enum
        {
            caret = 0x1000204
        };
    };

    namespace toggleButton
    {
        enum
        {
            text         = 0x1006501,
            tick         = 0x1006502,
            /**< The colour to use for the disabled tick mark. */
            tickDisabled = 0x1006503
        };
    };

    namespace textButton
    {
        enum
        {
            button   = 0x1000100,
            buttonOn = 0x1000101,
            textOff  = 0x1000102,
            /**< The colour to use for the button's text.when the button's toggle state is "on". */
            textOn   = 0x1000103
        };
    };

    namespace hyperlinkButton
    {
        enum
        {
            text = 0x1001f00
        };
    };

    namespace drawableButton
    {
        enum
        {
            text         = 0x1004010,
            textOn       = 0x1004013,
            background   = 0x1004011,
            backgroundOn = 0x1004012
        };
    };

    namespace colourSelector
    {
        enum
        {
            background = 0x1007000,
            /**< the colour used for the labels next to the sliders. */
            labelText  = 0x1007001
        };
    };

    namespace keyMappingEditorComponent
    {
        enum
        {
            background = 0x100ad00,
            text       = 0x100ad01
        };
    };

    namespace codeEditorComponent
    {
        enum
        {
            background           = 0x1004500,
            highlight            = 0x1004502,
            defaultText          = 0x1004503,
            lineNumberBackground = 0x1004504,
            lineNumberText       = 0x1004505
        };
    };

    namespace jucer_CommonHeaders
    {
        enum
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
    };

    namespace pageComponent
    {
        enum
        {
            background = 0x1900500
        };
    };

    namespace overlaySpinner
    {
        enum
        {
            background = 0x1900400,
            text       = 0x1900401
        };
    };

    namespace drawableImageButton
    {
        enum
        {
            image0 = 0x1900000,
            image1 = 0x1900001,
            image2 = 0x1900002,
            image3 = 0x1900003,
            image4 = 0x1900004
        };
    };

    namespace drawableImageComponent
    {
        enum
        {
            image0 = 0x1900000,
            image1 = 0x1900001,
            image2 = 0x1900002,
            image3 = 0x1900003,
            image4 = 0x1900004
        };
    };

    namespace switchComponent
    {
        enum
        {
            background = 0x100f000,
            handle     = 0x100f001,
            handleOff  = 0x100f002
        };
    };

    namespace fileSelectTextEditor
    {
        enum
        {
            fileWindow = 0x1900300,
            text       = 0x1900301
        };
    };

    namespace listEditor
    {
        enum
        {
            background       = 0x1900100,
            listItem         = 0x1900101,
            selectedListItem = 0x1900102,
            text             = 0x1900103
        };
    };

    namespace appMenuButton
    {
        enum
        {
            text       = 0x1900200,
            background = 0x1900201,
            selection  = 0x1900202,
            border     = 0x1900203
        };
    };

};