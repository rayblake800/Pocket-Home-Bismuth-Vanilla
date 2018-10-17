#include "NewDesktopAppEditor.h"
#include "DesktopEntryFormatError.h"
#include "DesktopEntryFileError.h"

static const constexpr char * newEntryTitleKey = "newEntryTitleKey";

AppMenu::NewDesktopAppEditor::NewDesktopAppEditor
(std::function<void() > onConfirm) :
AppMenuPopupEditor(localeText(newEntryTitleKey), onConfirm, true, true)
{
#    if JUCE_DEBUG
    setName("NewDesktopAppEditor");
#    endif
}


/*
 * Creates a new desktop entry file using the editor's field values.
 */
void AppMenu::NewDesktopAppEditor::commitEdits()
{
    using namespace juce;
    String entryName = getTitleField();
    entryName.removeCharacters("./\\");
    if (entryName.isNotEmpty())
    {
        try
        {
            DesktopEntry newEntry(entryName, entryName,
                    DesktopEntry::Type::application);
            newEntry.setIcon(getIconNameField());
            newEntry.setExec(getCommandField());
            newEntry.setCategories(getCategories());
            newEntry.setLaunchedInTerm(getTerminalCheckboxState());
            newEntry.writeFile();
        }
        // TODO: Explain the error when something goes wrong.
        catch(DesktopEntryFormatError e)
        {
            DBG("NewDesktopAppEditor::" << __func__
                    << ": Desktop entry format error:" << e.what());
        }
        catch(DesktopEntryFileError e)
        {
            DBG("NewDesktopAppEditor::" << __func__
                    << ": Desktop entry file error:" << e.what());
        }
    }
}
