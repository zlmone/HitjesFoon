#ifndef GUIWTAPPLICATION_H
#define GUIWTAPPLICATION_H

#include <Wt/WApplication>
#include <Wt/WBreak>
#include <Wt/WText>
#include <Wt/WTextArea>
#include <Wt/WLineEdit>
#include <Wt/WPushButton>
#include <Wt/WLabel>
#include <Wt/WIntValidator>
#include <Wt/WLengthValidator>
#include <Wt/WSound>
#include <Wt/WSuggestionPopup>
#include <Wt/WContainerWidget>
#include <Wt/WTemplate>
#include <Wt/WEnvironment>
#include <Wt/WSlider>

#include <string>
#include <iostream>

#include "Tools.h"
#include "gui/GUI.h"
#include "widgets/WFilledTemplate.h"
#include "Persistence.h"
#include "InputProcessor.h"
#include "AudioList.h"

using namespace Wt;
using namespace std;
using namespace boost;

class WidgetHome : public WApplication {
public:
    WidgetHome(const WEnvironment& env, GUI *gui, Persistence *persistence);
    virtual ~WidgetHome();

    void onPersistenceChange(string key);
    void processHitInput();
    void processSearchInput();
    void processVolumeChange(int volume);
    void processStop();
    void processPlayPause();

protected:
    GUI *gui;
    Persistence *persistence;
    WTemplate *home;
    WLineEdit *hitInputText;
    WText *hitHelpText;
    WLineEdit *searchInputText;
    WText *searchHelpText;
    WText *currentlyTypingText;
    WText *currentlyPlayingText;
    WText *playlistText;
    WSlider *volumeSlider;
    WPushButton *stopBtn;
    WPushButton *playpauseBtn;

    WContainerWidget *logContainer;
    WSound *logUpdate;

    WSuggestionPopup *searchSP;
    void initSearchSuggestions();

    void updatePlaylist();
    void updateLog();
private:
};

#endif // GUIWTAPPLICATION_H