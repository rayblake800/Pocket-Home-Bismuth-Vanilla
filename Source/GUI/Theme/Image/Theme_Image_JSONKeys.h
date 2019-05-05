#pragma once
/**
 * @file  Theme_Image_JSONKeys.h
 *
 * @brief  Provides the keys for all configurable component image resources
 *         defined in the imageResource.json configuration file.
 */

#include "Config_DataKey.h"

namespace Theme { namespace Image { namespace JSONKeys {

// Background image used on the HomePage:
static const Config::DataKey homeBackground
    ("home page background", Config::DataKey::stringType);

// Background image used on the LoginPage:
static const Config::DataKey loginBackground
    ("login page background", Config::DataKey::stringType);

// Default AppMenu icon image:
static const Config::DataKey defaultIcon
    ("default icon", Config::DataKey::stringType);

// Image displayed on the LoginPage:
static const Config::DataKey loginImage
    ("login image", Config::DataKey::stringType);

// ControlWidget wifi icon:
static const Config::DataKey controlWidgetWifi
    ("ControlWidget wifi icon", Config::DataKey::stringType);

static const std::vector<Config::DataKey> imageKeys
{
    homeBackground,
    loginBackground,
    defaultIcon,
    loginImage,
    controlWidgetWifi
};

//######################### UI Component Data ##############################
//Defines all component types managed in the config file
static const juce::Identifier menuFrame(
        "menu frame");
static const juce::Identifier batteryIcon(
        "battery");
static const juce::Identifier signalIcon(
        "signal strength");
static const juce::Identifier powerButton(
        "power button");
static const juce::Identifier settingsButton(
        "settings button");
static const juce::Identifier popupMenu(
        "popup menu");
static const juce::Identifier navButton(
        "navigation buttons");
static const juce::Identifier settingsListBtn(
        "settings list button");
static const juce::Identifier spinner(
        "loading spinner");
static const juce::Identifier lockIcon(
        "lock icon");
static const juce::Identifier volumeSlider(
        "volume slider");
static const juce::Identifier brightnessSlider(
        "brightness slider");
static const juce::Identifier cancelButton(
        "cancel button");
static const juce::Identifier confirmButton(
        "confirm button");

static const juce::Array<juce::Identifier> components =
{
    menuFrame,
    batteryIcon,
    signalIcon,
    powerButton,
    settingsButton,
    popupMenu,
    navButton,
    settingsListBtn,
    spinner,
    lockIcon,
    volumeSlider,
    brightnessSlider,
    cancelButton,
    confirmButton
};
}}}
