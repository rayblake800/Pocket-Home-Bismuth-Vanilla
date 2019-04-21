#pragma once
/**
 * @file  Layout_Component_JSONKeys.h
 *
 * @brief  Provides the keys for all parameters in the layout.json
 *         configuration file.
 */

#include "Config_DataKey.h"

namespace Layout { namespace Component { namespace JSONKeys {

//######################### Text Size Keys #################################

static const Config::DataKey smallText
    ("small text", Config::DataKey::doubleType);
static const Config::DataKey mediumText
    ("medium text", Config::DataKey::doubleType);
static const Config::DataKey largeText
    ("large text", Config::DataKey::doubleType);

//######################### UI Component Data ##############################

//Defines all component types managed in the config file
static const juce::Identifier scrollingAppMenu
        ("scrolling app menu");
static const juce::Identifier pagedAppMenu
        ("paged app menu");
static const juce::Identifier menuFrame
        ("app menu frame");
static const juce::Identifier batteryIcon
        ("home page battery icon");
static const juce::Identifier batteryPercent
        ("battery percent text");
static const juce::Identifier clockLabel
        ("home page clock label");
static const juce::Identifier homeIPLabel
        ("home page IP label");
static const juce::Identifier wifiIcon
        ("home page Wifi icon");
static const juce::Identifier powerButton
        ("power page button");
static const juce::Identifier settingsButton
        ("settings page button");
static const juce::Identifier popupMenu
        ("popup editor menu");
static const juce::Identifier pageLeft
        ("left arrow button");
static const juce::Identifier pageRight
        ("right arrow button");
static const juce::Identifier pageUp
        ("up arrow button");
static const juce::Identifier pageDown
        ("down arrow button");
static const juce::Identifier settingsListBtn
        ("settings list page button");
static const juce::Identifier spinner
        ("loading spinner");

static const std::vector<Config::DataKey>& basic =
{
    smallText,
    mediumText,
    largeText
};

static const juce::Array<juce::Identifier> components =
{
    scrollingAppMenu,
    pagedAppMenu,
    menuFrame,
    batteryIcon,
    batteryPercent,
    clockLabel,
    homeIPLabel,
    wifiIcon,
    powerButton,
    settingsButton,
    popupMenu,
    pageLeft,
    pageRight,
    pageUp,
    pageDown,
    settingsListBtn,
    spinner
};

}}}
