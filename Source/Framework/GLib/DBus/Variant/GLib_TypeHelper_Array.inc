// Partial template specialization for handling Array types
// Only include in TypeHelper.h!
template<typename T>
struct TypeHelper<VariantTypes::Array<T> >
{
    static const GVariantType* getGVariantType()
    {
        return G_VARIANT_TYPE_ARRAY;
    }

    static bool isType (GVariant* variant)
    {
        return g_variant_is_of_type (variant, G_VARIANT_TYPE_ARRAY);
    }

    static VariantTypes::String getTypeName()
    {
        return VariantTypes::String ("Array<")
            + TypeHelper<T>::getTypeName() + ">";
    }

    static VariantTypes::String getFormatString()
    {
        return VariantTypes::String ("a") + TypeHelper<T>::getFormatString();
    }

    static VariantTypes::Array<T> getValue (GVariant* variant,
            VariantTypes::Array<T> defaultValue)
    {
        if (variant == nullptr ||
                !TypeHelper<VariantTypes::Array<T>>::isType (variant))
        {
            return defaultValue;
        }
        VariantTypes::Array<T> varArray;
        gsize childCount = g_variant_n_children (variant);
        if (childCount == 0)
        {
            return varArray;
        }
        gsize dataSize = g_variant_get_size (variant) / childCount;

        const T* arrayData = static_cast<const T*>
            (g_variant_get_fixed_array (variant, &childCount, dataSize));
        for (int i = 0; i < childCount; i++)
        {
            varArray.add (arrayData[i]);
        }
        return varArray;
    }

    static GVariant* getVariant (VariantTypes::Array<T> value)
    {
        GVariant** array = new (std::nothrow) GVariant*[value.size()];
        if (array == nullptr)
        {
            DBG (__func__ << ":Failed to allocate memory!");
            return nullptr;
        }
        for (int i = 0; i < value.size(); i++)
        {
            array[i] = TypeHelper<T>::getVariant (value[i]);
            // Values should not be null!
            jassert (array[i] != nullptr);
        }
        GVariant* newVariant = g_variant_new_array
            (TypeHelper<T>::getGVariantType(), array, value.size());
        delete[] array;
        return newVariant;
    }
};
