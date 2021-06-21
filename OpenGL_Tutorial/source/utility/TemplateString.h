#pragma once
#include <map>
#include <regex>
#include <string>
#include <vector>


using namespace std;

class TemplateString
{
private:
	string templateString;
	static string replacerTemplate;
	static regex replacerTemplateRegex;

	string currentStringWithSettedKeysAndValues;

	map<string, string> settedKeysValues;
		
	static string replaceKeyInString(const string& str, const pair<string, string>& keyValue)
	{
		return replaceKeyInString(str, keyValue.first, keyValue.second);
	}

	static string replaceKeyInString(const string& str, const string& key, const string& value)
	{
		const string keyReplacer = regex_replace(replacerTemplate, replacerTemplateRegex, key);
		const regex keyReplacerRegex(keyReplacer);

		return regex_replace(str, keyReplacerRegex, value);
	}

	void addOrReplaceKeyValue(const string& key, const string& value)
	{
		settedKeysValues.insert_or_assign(key, value);
	}

	string getTemplateStringReplacedUsingMap()
	{
		string tempString = templateString;
		for (auto && settedKeysValue : settedKeysValues)
		{
			tempString = replaceKeyInString(tempString, settedKeysValue);
		}
		return tempString;
	}

	void updateCurrentString()
	{
		currentStringWithSettedKeysAndValues = getTemplateStringReplacedUsingMap();
	}
public:
	TemplateString(const string& templateString)
		:templateString(templateString)
	{
	}

	void setKeys(const vector<pair<string, string>>& keysValues)
	{
		for (auto && keyValue : keysValues)
		{
			setKey(keyValue);
		}
	}

	void setKey(const pair<string, string>& keyValue)
	{
		return setKey(keyValue.first, keyValue.second);
	}
	
	void setKey(const string& key, const string& value)
	{
		addOrReplaceKeyValue(key, value);
		updateCurrentString();
	}

	void clearKeysValues()
	{
		settedKeysValues.clear();
	}

	string getString() const
	{
		return currentStringWithSettedKeysAndValues;
	}

	void setString(const string& string)
	{
		clearKeysValues();
		templateString = string;
	}
};
