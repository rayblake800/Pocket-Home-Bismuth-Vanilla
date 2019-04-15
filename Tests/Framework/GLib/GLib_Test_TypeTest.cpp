#include "JuceHeader.h"
#include "GLib_TypeHelper.h"

namespace GLib { namespace Test { class TypeTest; } }

/**
 * @brief  Tests storing and reading different value types from GLib's
 *         GVariant data structures.
 */
class GLib::Test::TypeTest : public juce::UnitTest
{
public:
    TypeTest() : juce::UnitTest("GLib::Variant TypeHelper testing",
            "GLib") {}

    /**
     * @brief  Prints the name and GVariant format string used by a data type.
     *
     * @tparam T  The data type to print.
     */
    template<typename T>
    void typeLog()
    {
        using namespace GLib;
        using namespace VariantTypes;
        logMessage(String("Typename ")
                + TypeHelper<T>::getTypeName()
                + String(", format string ")
                + TypeHelper<T>::getFormatString());
    }

    /**
     * @brief  Tests that a value of a particular type can be copied into a
     *         GVariant and read back from the GVariant without changing its
     *         value.
     *
     * @tparam T            The type of value being tested.
     *
     * @param initialValue  The value to try copying into and out of a GVariant.
     *
     * @param defaultValue  A default value to compare against the value
     *                      returned by the GVariant.
     */
    template<typename T>
    void copyTest(T initialValue, T defaultValue)
    {
        using namespace GLib;
        using namespace VariantTypes;
        String typeName = TypeHelper<T>::getTypeName();
        beginTest(typeName + " copy testing");
        expect(initialValue != defaultValue,
                "Initial value must not match the default value");
        GVariant* copyVar = TypeHelper<T>::getVariant(initialValue);
        expect(copyVar != nullptr, String("Copied ") + typeName
                + " variant was null!");
        T copiedValue = TypeHelper<T>::getValue(copyVar, defaultValue);
        expect(initialValue == copiedValue,
                typeName + " output doesn't match input!");
        g_variant_unref(copyVar);
    }

    /**
     * @brief  Runs copy tests on a variety of different data types.
     */
    void runTest() override
    {
        using namespace GLib;
        using namespace VariantTypes;
        beginTest("Type value testing");
        typeLog<bool>();
        typeLog<int>();
        typeLog<Handle>();
        typeLog<String>();
        typeLog<Array<bool> >();
        typeLog<Array<String> >();
        typeLog<Array<double> >();

        copyTest<String>("Testing..", "");
        copyTest<int>(99, -1);
        copyTest<bool>(true, false);
        copyTest< Array<int> > ({1, 1, 2, 3, 5, 8, 13}, {});
    }
};

static GLib::Test::TypeTest test;
