#include "AAHandler.hpp"


AAHandler::AAHandler() :
QObject()
{

}

AAHandler::~AAHandler()
{

}

void AAHandler::injectButtonPressHelper(aasdk::proto::enums::ButtonCode::Enum buttonCode, Action::ActionState actionState)
{
    switch(actionState)
    {
        case(Action::ActionState::Activated):
            this->injectButtonPress(buttonCode, openauto::projection::ButtonEventType::PRESS);
            break;
        case(Action::ActionState::Deactivated):
            this->injectButtonPress(buttonCode, openauto::projection::ButtonEventType::RELEASE);
            break;
        case(Action::ActionState::Triggered):
        default:
            this->injectButtonPress(buttonCode, openauto::projection::ButtonEventType::NONE);
            break;   
    }
}

void AAHandler::mediaPlaybackUpdate(const aasdk::proto::messages::MediaInfoChannelPlaybackData& playback)
{
    emit aa_media_playback_update(playback);
}

void AAHandler::mediaMetadataUpdate(const aasdk::proto::messages::MediaInfoChannelMetadataData& metadata)
{
    emit aa_media_metadata_update(metadata);
}


void AAHandler::navigationStatusUpdate(const aasdk::proto::messages::NavigationStatus& navStatus)
{
    nav_status_ = navStatus;
    emit aa_navigation_status_update(navStatus);
}
void AAHandler::navigationTurnEvent(const aasdk::proto::messages::NavigationTurnEvent& turnEvent)
{
    nav_turn_ = turnEvent;
    emit aa_navigation_turn_event(turnEvent);
}
void AAHandler::navigationDistanceEvent(const aasdk::proto::messages::NavigationDistanceEvent& distanceEvent)
{
    nav_distance_ = distanceEvent;
    emit aa_navigation_distance_event(distanceEvent);
}
