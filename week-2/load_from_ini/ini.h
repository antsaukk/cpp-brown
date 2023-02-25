#pragma once

#include <sstream>
#include <string>
#include <string_view>
#include <unordered_map>

using namespace std;

namespace Ini {

using Section = unordered_map<string, string>;

class Document {
public:
  Section& AddSection(string name);
  string FillInSection(Section& section, istream& input);

  const Section& GetSection(const string& name) const
  {
    return sections.at(name);
  }

  size_t SectionCount() const
  {
    return sections.size();
  }

private:
  unordered_map<string, Section> sections;
};

pair<string_view, string_view> Split(string_view line, char by) {
  size_t pos = line.find(by);
  string_view left = line.substr(0, pos);

  if (pos < line.size() && pos + 1 < line.size()) {
    return {left, line.substr(pos + 1)};
  } else {
    return {left, string_view()};
  }
}

Section& Document::AddSection(string name)
{
  return sections[name];
}

string Document::FillInSection(Section& section, istream& input)
{
  string content;
  while (getline(input, content))
  {
    if (!content.empty()) {
      if (content[0] != '[') {
        auto [key, value] = Split(content, '=');
        section[string(key)] = string(value);
      } else {
        return content;
      }
    }
  }

  return "EOF";
}

Document Load(istream& input) {
  Document doc;
  string lineFromIni = "EOF";

  while (getline(input, lineFromIni))
  {
    if (!lineFromIni.empty() && lineFromIni[0] == '[')
      break;
  }

  while (lineFromIni != "EOF" && lineFromIni != "")
  {
    string sectionName;
    sectionName = lineFromIni.substr(1, lineFromIni.size() - 2);
    Section& newSection = doc.AddSection(sectionName);
    lineFromIni = doc.FillInSection(newSection, input);
  }

  return doc;
}

}