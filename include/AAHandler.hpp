#pragma once

#include <QJsonObject>
#include <QString>
#include <QByteArray>

#include "openauto/Service/IAndroidAutoInterface.hpp"
#include "app/action.hpp"

class AAHandler : public QObject, public openauto::service::IAndroidAutoInterface {
    Q_OBJECT

   public:
    AAHandler();
    ~AAHandler();
    void mediaPlaybackUpdate(const aasdk::proto::messages::MediaInfoChannelPlaybackData& playback) override;
    void mediaMetadataUpdate(const aasdk::proto::messages::MediaInfoChannelMetadataData& metadata) override;
    void navigationStatusUpdate(const aasdk::proto::messages::NavigationStatus& navStatus) override;
    void navigationTurnEvent(const aasdk::proto::messages::NavigationTurnEvent& turnEvent) override;
    void navigationDistanceEvent(const aasdk::proto::messages::NavigationDistanceEvent& distanceEvent) override;
    void injectButtonPressHelper(aasdk::proto::enums::ButtonCode::Enum buttonCode, Action::ActionState actionState);
    QJsonObject get_nav_state() const { return convert_nav_to_json(); }

   private:
    aasdk::proto::messages::NavigationStatus nav_status_;
    aasdk::proto::messages::NavigationTurnEvent nav_turn_;
    aasdk::proto::messages::NavigationDistanceEvent nav_distance_;
    aasdk::proto::messages::NavigationStatus m_navigationStatus;

    QJsonObject convert_nav_to_json() const {
        QJsonObject obj;
        obj["navStatus"] = QString::number(static_cast<int>(nav_status_.status()));
        obj["streetName"] = QString::fromStdString(nav_turn_.street_name());
        obj["maneuverType"] = QString::number(static_cast<int>(nav_turn_.maneuvertype()));
        obj["maneuverDirection"] = QString::number(static_cast<int>(nav_turn_.maneuverdirection()));
        obj["roundaboutExitNumber"] = QString::number(static_cast<int>(nav_turn_.roundaboutexitnumber()));
        obj["roundaboutExitAngle"] = QString::number(static_cast<int>(nav_turn_.roundaboutexitangle()));
        obj["distanceEvent"] = QString::number(static_cast<int>(nav_distance_.meters()));
        //convert PNG to base64
        QByteArray imageBytes = QByteArray::fromStdString(nav_turn_.turnimage());
        QString base64Image = imageBytes.toBase64();
        obj["image"] = base64Image;
        return obj;
    }

   signals:
    void aa_media_metadata_update(const aasdk::proto::messages::MediaInfoChannelMetadataData& metadata);
    void aa_media_playback_update(const aasdk::proto::messages::MediaInfoChannelPlaybackData& playback);
    void aa_navigation_status_update(const aasdk::proto::messages::NavigationStatus& navStatus);
    void aa_navigation_turn_event(const aasdk::proto::messages::NavigationTurnEvent& turnEvent);
    void aa_navigation_distance_event(const aasdk::proto::messages::NavigationDistanceEvent& distanceEvent);
};
