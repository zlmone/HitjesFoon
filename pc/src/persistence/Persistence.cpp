#include "Persistence.h"

Persistence::Persistence(WServer &server) : server(server) {
    intData.connect(boost::bind(&Persistence::postOnChange, this, _1));
    stringData.connect(boost::bind(&Persistence::postOnChange, this, _1));
    intVectorData.connect(boost::bind(&Persistence::postOnChange, this, _1));
    stringVectorData.connect(boost::bind(&Persistence::postOnChange, this, _1));
}

Persistence::~Persistence() {

}

void Persistence::onChangeCallback(const PersistenceCallback &callback) {
    WApplication *app = WApplication::instance();
    lock_guard<mutex> guard(callbackMutex);
    callbacks[app->sessionId()] = callback;
}

void Persistence::postOnChange(const string &key) {
    WApplication *app = WApplication::instance();
    for (auto &callback : callbacks) {
        string sessionId = callback.first;
        if (app && app->sessionId() == sessionId) {
            callback.second(key);
        } else {
            server.post(sessionId, boost::bind(callback.second, key));
        }
    }
}


PersistenceData<int> &Persistence::getIntData() {
    return intData;
}

PersistenceData<string> &Persistence::getStringData() {
    return stringData;
}

PersistenceData<vector<int>> &Persistence::getIntVectorData() {
    return intVectorData;
}

PersistenceData<vector<string>> &Persistence::getStringVectorData() {
    return stringVectorData;
}