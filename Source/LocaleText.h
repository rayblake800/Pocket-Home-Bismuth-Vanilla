///**
// * @file LocaleText.h
// *
// *
// *
// */
//
//#pragma once
//#include <map>
//#include "JuceHeader.h"
//
//class LocaleText
//{
//public:
//	LocaleText();
//	virtual ~LocaleText();
//
//	String getLocaleName();
//
//	class LocalizedTextConsumer
//	{
//	friend class LocaleText;
//	public:
//		LocalizedTextConsumer(LocaleText& locale);
//		virtual ~LocalizedTextConsumer();
//
//		String localizedText(String key);
//	private:
//		virtual StringArray getStringKeys() = 0;
//		LocaleText& localeText;
//	}
//private:
//	std::map<String,String> localeStrings;
//	Array<LocalizedTextConsumer*> textConsumers;
//};
