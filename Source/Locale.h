/**
 * @file Locale.h
 *
 *
 *
 */

#pragma once
#include <map>
#include "JuceHeader.h"

class Locale
{
public:
	Locale();
	virtual ~Locale();

	String getLocale();

	class LocalizedTextConsumer
	{
	friend class Locale;
	public:
		LocalizedTextConsumer(Locale& locale);
		virtual ~LocalizedTextConsumer();

		String localizedText(String key);
	private:
		virtual StringArray getStringKeys() = 0;
		Locale& locale;
	}
private:
	std::map<String,String> localeStrings;
	Array<LocalizedTextConsumer*> textConsumers;
};
