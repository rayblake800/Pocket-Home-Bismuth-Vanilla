#include "Info_IPLabel.h"
#include "Util_Commands.h"
#include "Config_MainFile.h"
#include "Testing_DelayUtils.h"
#include "JuceHeader.h"

namespace Info
{
    namespace Test
    {
        class IPLabelTest;
    }
}

/**
 * @brief  Tests that the Info::IPLabel class correctly loads local and/or
 *         public IP addresses using the Config::MainFile resource's IP label
 *         settings.
 */
class Info::Test::IPLabelTest : public juce::UnitTest
{
private:
    /**
     * @brief  Updates an IP label with new settings, then gets the label's
     *         text.
     *
     * @param label         An IPLabel to update.
     *
     * @param showLocalIP   Whether the label should print the local IP address.
     *
     * @param showPublicIP  Whether the label should print the public IP
     *                      address.
     *
     * @return              The label's text after the update.
     */
    juce::String getUpdatedLabelText(IPLabel& label, const bool showLocalIP,
            const bool showPublicIP)
    {
        Config::MainFile mainConfig;
        mainConfig.setIPLabelPrintsLocal(showLocalIP);
        mainConfig.setIPLabelPrintsPublic(showPublicIP);
        juce::String labelText = label.getText();
        Testing::DelayUtils::idleUntil([&label, &labelText]()
        {
            juce::String updatedText = label.getText();
            if (updatedText != labelText)
            {
                labelText = updatedText;
                return true;
            }
            return false;
        }, 400, 10000);
        return labelText;
    }

public:
    IPLabelTest() : juce::UnitTest("IPLabel Testing",
            "Info") {}

    void runTest() override
    {
        // Save existing config options:
        Config::MainFile mainConfig;
        bool showLocalIP = mainConfig.getIPLabelPrintsLocal();
        bool showPublicIP = mainConfig.getIPLabelPrintsPublic();

        IPLabel ipLabel;
        beginTest("Local IP loading");
        juce::String localLabel = getUpdatedLabelText(ipLabel, true, false);
        logMessage(localLabel);
        expect(localLabel.isNotEmpty(), "Failed to set local IP label text.");

        beginTest("Public IP loading");
        juce::String publicLabel = getUpdatedLabelText(ipLabel, false, true);
        logMessage(publicLabel);
        expect(publicLabel.isNotEmpty(), "Failed to set public IP label text.");

        beginTest("Dual IP loading");
        juce::String dualLabel = getUpdatedLabelText(ipLabel, true, true);
        logMessage(dualLabel);
        expectGreaterOrEqual(dualLabel.length(),
                (localLabel.length() + publicLabel.length()),
                "Failed to fully show both IP address types.");

        beginTest("Empty IP loading");
        juce::String emptyLabel = getUpdatedLabelText(ipLabel, false, false);
        juce::String failureText
                = "Label should be empty when both IP types are disabled.";
        failureText += " Actual text:" + emptyLabel;
        expect(emptyLabel.isEmpty(), failureText);

        // Restore original config options:
        mainConfig.setIPLabelPrintsLocal(showLocalIP);
        mainConfig.setIPLabelPrintsPublic(showPublicIP);
    }
};

static Info::Test::IPLabelTest test;
