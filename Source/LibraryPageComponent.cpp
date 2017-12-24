#include "LibraryPageComponent.h"

#include "PokeLookAndFeel.h"
#include "Main.h"
#include "Utils.h"

LibraryPageComponent::LibraryPageComponent() :
  AppListComponent(),
  backButton(createImageButton("Back",
                               createImageFromFile(assetFile("nextIcon.png"))))
{
  bgColor = Colour(PokeLookAndFeel::chipPurple);
  
  backButton->addListener(this);
  backButton->setAlwaysOnTop(true);
  addAndMakeVisible(backButton);
}
LibraryPageComponent::~LibraryPageComponent() {}

void LibraryPageComponent::paint(Graphics &g) {
  g.fillAll(bgColor);
}
