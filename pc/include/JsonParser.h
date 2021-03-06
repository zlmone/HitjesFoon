#ifndef JSONPARSER_H
#define JSONPARSER_H

#include "rapidjson/document.h"
#include "Tools.h"
#include "gui/GUI.h"
#include <stdio.h>
#include <vector>

using namespace std;
using namespace rapidjson;

class JsonParser {
public:
    JsonParser(GUI &gui);
    virtual ~JsonParser();

    void parse(const char *jsonString);
    int getTotalResults();
    const char * getVideoId();
    vector<string> getVideoIds();
    const char *getVideoTitle();
    vector<string> getVideoTitles();
protected:
    GUI &gui;
    Document root;
private:
    JsonParser(const JsonParser &that) = delete;
};

#endif // JSONPARSER_H
