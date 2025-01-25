#pragma once

#include <QLabel>
#include <QMediaPlayer>
#include <QStackedLayout>
#include <QString>
#include <QVideoWidget>
#include <QComboBox>
#include <QRadioButton>
#include <QCamera>
#include <QCheckBox>
#include <QCameraViewfinder>
#include <QCameraViewfinderSettings>

#include <gst/gst.h>
#include <gst/app/gstappsrc.h>
#include <gst/app/gstappsink.h>
#include <gst/video/video.h>
#include <QGlib/Error>
#include <QGlib/Connect>
#include <QGst/Init>
#include <QGst/Bus>
#include <QGst/Pipeline>
#include <QGst/Parse>
#include <QGst/Message>
#include <QGst/Utils/ApplicationSink>
#include <QGst/Utils/ApplicationSource>
#include <QGst/Ui/VideoWidget>
#include <QGst/ElementFactory>
#include <QGst/Quick/VideoSurface>
#include <QtQml/QQmlContext>
#include <QtQuickWidgets/QQuickWidget>

#include "app/config.hpp"
#include "DashLog.hpp"

#include "app/pages/page.hpp"

////////////////////////////////////////////////

#include <QPair>
#include <QtWidgets>
#include <QPluginLoader>
#include <QMap>

#include "canbus/socketcanbus.hpp"
#include "obd/message.hpp"
#include "obd/command.hpp"
#include "app/widgets/selector.hpp"
#include "app/widgets/dialog.hpp"


/////////////////////////////////////////////////

class Arbiter;

//////////////////////////////////////////////////////////

typedef std::function<double(double, bool)> obd_decoder_t;
typedef QPair<QString, QString> units_t;

//////////////////////////////////////////////////////////

/*
class Gauge : public QWidget {
    Q_OBJECT

   public:
    enum Orientation { BOTTOM, RIGHT };

    Gauge(units_t units, QFont value_font, QFont unit_font, Orientation orientation, int rate,
          std::vector<Command> cmds, int precision, obd_decoder_t decoder, QWidget *parent = nullptr);

    inline void start() { this->timer->start(this->rate); }
    inline void stop() { this->timer->stop(); }
    void can_callback(QByteArray payload);

   private:
    QString format_value(double value);
    QString null_value();
    QLabel *value_label;

    obd_decoder_t decoder;
    std::vector<Command> cmds;

    bool si;
    int rate;
    int precision;
    QTimer *timer;

   signals:
    void toggle_unit(bool si);
};

*/




class CameraPage : public QWidget, public Page {
    Q_OBJECT

   public:
    CameraPage(Arbiter &arbiter, QWidget *parent = nullptr);

    void init() override;

   private:
    void get_plugins();
    void load_plugin();
    QWidget *dialog_body();
    QWidget *can_bus_toggle_row();
    QWidget *si_units_row_widget();

    QMap<QString, int> capabilities;
    QMap<QString, QFileInfo> plugins;
    QStringList can_devices;
    QStringList serial_devices;
    QMap<QString, QString> paired_bt_devices;
    QPluginLoader *active_plugin;
    Selector *plugin_selector;
    Config *config;

    Arbiter &arbiter;
    //QWidget *speedo_tach_widget();
    // QWidget *mileage_data_widget();
    //QWidget *engine_data_widget();
    //QWidget *coolant_temp_widget();
    //QWidget *engine_load_widget();

    //std::vector<Gauge *> gauges;

    QHBoxLayout* main_layout;  // Store the main layout
    QWidget* plugin_container; // Container for plugin widgets
    
    void initializeLayout();
    void integratePluginWidgets(const QList<QWidget*>& widgets);




    class VideoContainer : public QWidget {
       public:
        VideoContainer(QWidget *parent = nullptr, CameraPage *page = nullptr);

       private:
        void resizeEvent(QResizeEvent *event);

        CameraPage *page;
    };

    class Settings : public QWidget {
       public:
        Settings(Arbiter &arbiter, QWidget *parent = nullptr);
        QSize sizeHint() const override;

       private:
        QWidget *settings_widget();
        QWidget *camera_overlay_row_widget();
        QWidget *camera_overlay_width_row_widget();
        QWidget *camera_overlay_height_row_widget();
        QWidget *camera_overlay_width_widget();
        QWidget *camera_overlay_height_widget();

        Arbiter &arbiter;
        Config *config;
    };

    QWidget *connect_widget();
    QWidget *network_camera_widget();
    QWidget *local_camera_widget();
    QWidget *local_cam_selector();
    QPushButton *connect_button();
    QWidget *network_cam_selector();
    QWidget *selector_widget(QWidget *selection);
    void populate_local_cams();
    void connect_network_stream();
    void connect_local_stream();
    bool local_cam_available(const QString &device);
    QSize choose_video_resolution();
    void count_down();
    void connect_cam();

//////////////////////////////////////////////////////////////////////////////////////
    void handlePageChange(int pageId);
    void setupPageChangeHandler();
    static const int CCC_CAMERA_PAGE_ID = 3;  // Define the camera page ID constant
    int previousPageId = -1;

    //Arbiter &arbiter;  // Add this line to the private section
//////////////////////////////////////////////////////////////////////////////////////


    //Config *config;
    QLabel *status;
    QMediaPlayer *player;
    QList<QPair<QString, QString>> local_cams;
    QComboBox *cams_dropdown;
    QWidget *local_video_widget;
    QWidget *remote_video_widget;
    QCameraViewfinderSettings local_cam_settings;
    QCamera *local_cam;
    QTimer *reconnect_timer;
    int reconnect_in_secs;
    QString reconnect_message;
    int local_index;

    bool connected = false;

    void init_gstreamer_pipeline(std::string vidLaunchStr_, bool sync = false);
    void disconnect_stream();

    static GstPadProbeReturn convertProbe(GstPad *pad, GstPadProbeInfo *info, void *);
    static gboolean busCallback(GstBus *, GstMessage *message, gpointer *);
    void showEvent(QShowEvent *event);

    QGst::ElementPtr videoSink_;
    QQuickWidget *videoWidget_;
    GstElement *vidPipeline_;
    GstAppSrc *vidSrc_;
    QWidget *videoContainer_;
    QGst::Quick::VideoSurface *surface_;

   signals:
    void connected_network();
    void connected_local();
    void disconnected();
    void autoconnect_disabled();
    void next_cam();
    void prev_cam();
};

