#include "gui/GUIWt.h"


GUIWt::GUIWt(PRINT_LEVEL msglevel) : GUINull(msglevel), persistence(NULL) {
}

GUIWt::~GUIWt() {
    SAFE_DELETE(persistence);
}

void GUIWt::setServer(WServer &server) {
    persistence = new Persistence(server);
//    persistence->getStringData().initVal("log", string(""));
    vector<string> logv;
    logv.push_back("");
    persistence->getStringVectorData().initVal("logv", logv);
    persistence->getIntData().initVal("logc", 0);
    persistence->getIntData().initVal("playback-state", STOP);
    // Listen to all persistence changes
    persistence->onChangeCallback(boost::bind(&GUIWt::onPersistenceChange, this, _1));
}

void GUIWt::onPersistenceChange(const string &key) {
    try {
        // Try to convert to integer
        stoi(key);
    } catch (std::invalid_argument e) {
        // Couldn't convert, just ignore
        return;
    }
    // Try to retrieve as hitje
    const Hitje &hitje = persistence->getHitjeData().getVal(key);
    // If successful, propagate changes to listeners
    events().hitjeChangeSig(hitje);
}

WApplication *GUIWt::createApplication(const WEnvironment& env) {
    if (persistence == NULL) {
        throw "Persistence not created yet, first set server object";
    }
    AHitjesfoon *app = new AHitjesfoon(env, *this, *persistence);
    app->buildWidget();

//    app->setCssTheme("polished");
    WBootstrapTheme *bootstrapTheme = new WBootstrapTheme(app);
    bootstrapTheme->setVersion(WBootstrapTheme::Version3);
    bootstrapTheme->setResponsive(true);
    app->setTheme(bootstrapTheme);

    // load the default bootstrap3 (sub-)theme
    app->useStyleSheet("resources/themes/bootstrap/3/bootstrap-theme.min.css");

    app->messageResourceBundle().use(app->appRoot() + "language/home");
    app->messageResourceBundle().use(app->appRoot() + "language/hitjeslist");
    app->messageResourceBundle().use(app->appRoot() + "language/configuration");
    app->messageResourceBundle().use(app->appRoot() + "language/404");
    app->messageResourceBundle().use(app->appRoot() + "templates/home");
    app->messageResourceBundle().use(app->appRoot() + "templates/hitjeslist");
    app->messageResourceBundle().use(app->appRoot() + "templates/configuration");
    app->messageResourceBundle().use(app->appRoot() + "templates/404");

    return app;
}


void GUIWt::printlevel(PRINT_LEVEL level, const char *format, va_list args) {
    if (persistence != NULL) {
        char buf[1000];
        // Copy arguments, as list can only be iterated once
        va_list argsCpy;
        va_copy(argsCpy, args);
        vsnprintf(buf, sizeof(buf), format, argsCpy);

        logAppend(level, buf);
        va_end(argsCpy);
    }
    GUINull::printlevel(level, format, args);
}


void GUIWt::setPlaying() {
    persistence->getIntData().setVal("playback-state", PLAY);
}

void GUIWt::setPaused() {
    persistence->getIntData().setVal("playback-state", PAUSE);
}

void GUIWt::setStopped() {
    persistence->getIntData().setVal("playback-state", STOP);
}

void GUIWt::setPhoneNum(int num) {
    persistence->getIntData().setVal("phone-num", num);
}

void GUIWt::setPlaylist(const vector<int> &playlist) {
    persistence->getIntVectorData().setVal("phone-playlist", playlist);
}

void GUIWt::setSpeakerVolume(int volume) {
    events().speakerVolumeSig(volume);
    persistence->getIntData().setVal("volume-speaker", volume);
}

void GUIWt::setPhoneVolume(int volume) {
    events().phoneVolumeSig(volume);
    persistence->getIntData().setVal("volume-phone", volume);
}

void GUIWt::setHitje(Hitje hitje) {
    persistence->getHitjeData().setVal(to_string(hitje.hitIndex), hitje);
}

void GUIWt::logAppend(PRINT_LEVEL level, string text) {
    string style = "";
    string el;
    bool nlfirst = (text.size() > 1) && (*(text.cbegin()) == '\n');
    bool nlsecond = (*(text.cend() - 1) == '\n');

    // append log text
//    persistence->getStringData().addVal("log", text);
    // Don't print stringent log data to web interface
    // (both slow and crashes!)
    if (level > LINFO) {
        return;
    }

    // end with newline, erase
    if (nlfirst) {
        text.erase(text.begin());
    }
    if (nlsecond) {
        text.erase(text.end() - 1);
    }
    if (!text.empty()) {
        if (level == LERROR) {
            style = "style=\"color:red\"";
        } else if (level == LWARNING) {
            style = "style=\"color:orange\"";
        } else {
//            style = "style=\"color:green\"";
        }

        text = Utils::htmlEncode(text, Utils::HtmlEncodingFlag::EncodeNewLines);
        el = "<span " + style + ">" + text + "</span>\n";
        persistence->getStringVectorData().manipulateVal("logv", boost::bind(&GUIWt::logHTML, this, _1, el, nlfirst, nlsecond));
    }
}

void GUIWt::logHTML(vector<string> &logv, const string &el, bool nlfirst, bool nlsecond) {
    int c = 0;
    string &last = *(logv.end() - 1);
    if (nlfirst && !last.empty()) {
        logv.push_back(el);
        c++;
    } else {
        last += el;
    }
    if (nlsecond) {
        logv.push_back("");
        c++;
    }
    // Don't store more than 500 elements to prevent memory leaking
    // TODO erasing at start is inefficient, use list instead?
    if (logv.size() > 500) {
        logv.erase(logv.begin(), logv.end() - 500);
    }
    if (c) {
        persistence->getIntData().addVal("logc", c);
    }
}

int GUIWt::getKeystroke() {
    WApplication *app = WApplication::instance();
    if (app == NULL) {
        // No user input, get from terminal
        return GUINull::getKeystroke();
    } else {
        // Get input from the users browser and process
        printlevel(LWARNING, "function getKeystroke not implemented yet\n");
    }
    return 0;
}

