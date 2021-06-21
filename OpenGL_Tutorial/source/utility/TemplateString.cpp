#include "TemplateString.h"

string TemplateString::replacerTemplate = R"(%(KEY)%)";
regex TemplateString::replacerTemplateRegex = regex("KEY");
