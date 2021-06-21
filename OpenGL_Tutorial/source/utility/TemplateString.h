#pragma once
#include <regex>
#include <string>
#include <vector>


using namespace std;

class TemplateString
{
private:
	string templateString;
	const string replacerTemplate = R"(%(KEY)%)";
	const regex replacerTemplateRegex = regex("KEY");

	string replaceKeyInString(const string& str, const pair<string, string>& keyValue) const
	{
		return replaceKeyInString(str, keyValue.first, keyValue.second);
	}

	string replaceKeyInString(const string& str, const string& key, const string& value) const
	{
		const string keyReplacer = regex_replace(replacerTemplate, replacerTemplateRegex, key);
		const regex keyReplacerRegex(keyReplacer);

		return regex_replace(str, keyReplacerRegex, value);
	}
public:
	TemplateString(const string& templateString)
		:templateString(templateString)
	{
	}

	string replaceKeys(const vector<pair<string, string>>& keyValues) const
	{
		string tempString = templateString;
		for (auto && keyValue : keyValues)
		{
			tempString = replaceKeyInString(tempString, keyValue);
		}
		return tempString;
	}

	string replaceKey(const pair<string, string>& keyValue) const
	{
		return replaceKey(keyValue.first, keyValue.second);
	}
	
	string replaceKey(const string& key, const string& value) const
	{
		return replaceKeyInString(templateString, key, value);
	}

	string getString() const
	{
		return templateString;
	}
};
