#define APPMENU_IMPLEMENTATION_ONLY
#include "DesktopEntry/FormatError.h"
#include "DesktopEntry/FileError.h"
#include "DesktopEntry/EntryFile.h"
#include "DesktopEntry/Loader.h"
#include "AppMenu/Components/Editors/NewDesktopAppEditor.h"


/* NewConfigItemEditor shares a localized object class key inherited from
   AppMenu::PopupEditor. */

/* Localized title text key: */
static const juce::Identifier newEntryTitleKey = "newEntryTitleKey";

/*
 * Creates a new editor, setting any additional actions for the editor to 
 * perform on confirmation.
 */
AppMenu::NewDesktopAppEditor::NewDesktopAppEditor
(std::function<void() > onConfirm) :
AppMenu::PopupEditor(onConfirm, true, true)
{
#    if JUCE_DEBUG
    setName("NewDesktopAppEditor");
#    endif
    setEditorTitle(localeText(newEntryTitleKey));
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
            DesktopEntry::EntryFile newEntry(entryName, entryName,
                    DesktopEntry::EntryFile::Type::application);
            newEntry.setIcon(getIconNameField());
            newEntry.setExec(getCommandField());
            newEntry.setCategories(getCategories());
            newEntry.setLaunchedInTerm(getTerminalCheckboxState());
            newEntry.writeFile();

            // Update loaded entries:
            DesktopEntry::Loader entryLoader;
            entryLoader.scanForChanges();
        }
        // TODO: Explain the error to the user when something goes wrong.
        catch(DesktopEntry::FormatError e)
        {
            DBG("NewDesktopAppEditor::" << __func__
                    << ": Desktop entry format error:" << e.what());
        }
        catch(DesktopEntry::FileError e)
        {
            DBG("NewDesktopAppEditor::" << __func__
                    << ": Desktop entry file error:" << e.what());
        }
    }
}
