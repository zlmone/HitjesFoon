#ifndef WPLAYLIST_H
#define WPLAYLIST_H


#include <Wt/WApplication>
#include <Wt/WPushButton>
#include <Wt/WText>

#include "gui/GUI.h"
#include "widgets/WFilledTemplate.h"
#include "InputProcessor.h"
#include "persistence/Persistence.h"

using namespace std;
using namespace boost;
using namespace Wt;


class WPlaylist : public WFilledTemplate {
public:
    WPlaylist(GUI &gui, Persistence &persistence, WContainerWidget *parent = NULL);
    WPlaylist(GUI &gui, Persistence &persistence, const WString &text, WContainerWidget *parent = NULL);
    virtual ~WPlaylist();

    void buildWidget();
    void onPersistenceChange(const string &key);
protected:
    GUI &gui;
    Persistence &persistence;

    WText *currentlyTypingText;
    WText *currentlyPlayingText;
    WText *playlistText;

    void updatePlaylist();
private:
};

#endif // WPLAYLIST_H
