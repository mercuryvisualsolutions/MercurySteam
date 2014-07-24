#include "sessionmanager.h"

Session::SessionManager::SessionManager()
{
    _init();
}

Session::SessionManager::~SessionManager()
{
    //clean properties panels
    for(auto &pair: _sessionsPropertiesPanels)
    {
        delete pair.second;
        pair.second = nullptr;
    }

    _sessionsPropertiesPanels.clear();
}

Ms::Widgets::MPropertiesPanel *Session::SessionManager::getSessionPropertiesPanel(const std::string &sessionId)
{
    if(_sessionsPropertiesPanels.find(sessionId) == _sessionsPropertiesPanels.end())
    {
        Ms::Widgets::MPropertiesPanel *panel = new Ms::Widgets::MPropertiesPanel();

        _sessionsPropertiesPanels[sessionId] = panel;
    }

    return _sessionsPropertiesPanels[sessionId];
}

void Session::SessionManager::_init()
{
}
