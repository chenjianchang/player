/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "player.h"
#include "playercontrols.h"
#include "playlistmodel.h"
#include "videowidget.h"
#include "functions.h"
#include "help.h"
#include "g_variable.h"

#include <string>

#include <qmediaplaylist.h>
#include <QMediaMetaData>
#include <QMediaDevices>
#include <QAudioDevice>
#include <QAudioOutput>
#include <QMediaFormat>
#include <QtWidgets>
#include <QIcon>
#include <QMap>
#include <QVariant>


Player::Player(QWidget *parent)
    : QWidget(parent)
{
    this->setWindowIcon(QIcon(":/Icon/player.png"));   // edited by shaolang

//! [create-objs]
    m_player = new QMediaPlayer(this);
    m_audioOutput = new QAudioOutput(this);
    m_player->setAudioOutput(m_audioOutput);
//! [create-objs]
    connect(m_player, &QMediaPlayer::durationChanged, this, &Player::durationChanged);
    connect(m_player, &QMediaPlayer::positionChanged, this, &Player::positionChanged);
    connect(m_player, &QMediaPlayer::positionChanged, this, &Player::get_subtitle);
    connect(m_player, QOverload<>::of(&QMediaPlayer::metaDataChanged), this, &Player::metaDataChanged);
    connect(m_player, &QMediaPlayer::mediaStatusChanged, this, &Player::statusChanged);
    connect(m_player, &QMediaPlayer::bufferProgressChanged, this, &Player::bufferingProgress);
    connect(m_player, &QMediaPlayer::hasVideoChanged, this, &Player::videoAvailableChanged);
    connect(m_player, &QMediaPlayer::errorChanged, this, &Player::displayErrorMessage);
    connect(m_player, &QMediaPlayer::tracksChanged, this, &Player::tracksChanged);

//! [2]
    m_videoWidget = new VideoWidget(this);
    m_videoWidget->resize(1280, 720);
    m_player->setVideoOutput(m_videoWidget);

    m_playlistModel = new PlaylistModel(this);
    m_playlist = m_playlistModel->playlist();

    m_subtitle_box = new QTextEdit(this);   // edited by shaolang
    m_subtitle_box->document()->setDefaultTextOption(QTextOption(Qt::AlignHCenter));   // edited by shaolang
    m_subtitle_box->setStyleSheet("font-family: 'Microsoft YaHei UI'; background-color: rgb(10, 10, 10); font-size: 25px; color: white; selection-color: pink");
    m_subtitle_splitter = new QSplitter(Qt::Vertical);   // edited by shaolang

    m_subtitle_splitter->addWidget(m_videoWidget);   // edited by shaolang
    m_subtitle_splitter->addWidget(m_subtitle_box);   // edited by shaolang
    m_subtitle_splitter->setCollapsible(0, false);   // edited by shaolang
//! [2]

    connect(m_playlist, &QMediaPlaylist::currentIndexChanged, this, &Player::playlistPositionChanged);

    // player layout
    QBoxLayout *layout = new QVBoxLayout(this);
    m_playlist_splitter = new QSplitter(Qt::Horizontal);   // edited by shaolang

    // display
    // QBoxLayout *displayLayout = new QHBoxLayout;   // edited by shaolang
    // displayLayout->addWidget(m_videoWidget, 2);   // edited by shaolang
    m_playlist_splitter->addWidget(m_subtitle_splitter);   // edited by shaolang


#if !defined(Q_OS_ANDROID) && !defined(Q_OS_IOS)
    m_playlistView = new QListView();
    m_playlistView->setModel(m_playlistModel);
    m_playlistView->setCurrentIndex(m_playlistModel->index(m_playlist->currentIndex(), 0));
    connect(m_playlistView, &QAbstractItemView::activated, this, &Player::jump);

    // displayLayout->addWidget(m_playlistView);   // edited by shaolang
    m_playlist_splitter->addWidget(m_playlistView);   // edited by shaolang
    m_playlist_splitter->setCollapsible(0,false);   // edited by shaolang

#endif
    // layout->addLayout(displayLayout);   // edited by shaolang
    layout->addWidget(m_playlist_splitter);   // edited by shaolang


    // duration slider and label
    QHBoxLayout *hLayout = new QHBoxLayout;

    m_slider = new QSlider(Qt::Horizontal, this);
    m_slider->setRange(0, m_player->duration());
    connect(m_slider, &QSlider::sliderMoved, this, &Player::seek);
    hLayout->addWidget(m_slider);

    m_labelDuration = new QLabel();
    m_labelDuration->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    hLayout->addWidget(m_labelDuration);
    layout->addLayout(hLayout);

    // controls
    QBoxLayout *controlLayout = new QHBoxLayout;
    controlLayout->setContentsMargins(0, 0, 0, 0);

    QPushButton *openButton = new QPushButton(tr("Open"), this);
    connect(openButton, &QPushButton::clicked, this, &Player::open);
    controlLayout->addWidget(openButton);
    controlLayout->addStretch(1);

    PlayerControls *controls = new PlayerControls();
    controls->setState(m_player->playbackState());
    controls->setVolume(m_audioOutput->volume());
    controls->setMuted(controls->isMuted());

    connect(controls, &PlayerControls::play, m_player, &QMediaPlayer::play);
    connect(controls, &PlayerControls::pause, m_player, &QMediaPlayer::pause);
    connect(controls, &PlayerControls::stop, m_player, &QMediaPlayer::stop);
    connect(controls, &PlayerControls::next, m_playlist, &QMediaPlaylist::next);
    connect(controls, &PlayerControls::previous, this, &Player::previousClicked);
    connect(controls, &PlayerControls::changeVolume, m_audioOutput, &QAudioOutput::setVolume);
    connect(controls, &PlayerControls::changeMuting, m_audioOutput, &QAudioOutput::setMuted);
    connect(controls, &PlayerControls::changeRate, m_player, &QMediaPlayer::setPlaybackRate);
    connect(controls, &PlayerControls::stop, m_videoWidget, QOverload<>::of(&QVideoWidget::update));

    connect(m_player, &QMediaPlayer::playbackStateChanged, controls, &PlayerControls::setState);
    connect(m_audioOutput, &QAudioOutput::volumeChanged, controls, &PlayerControls::setVolume);
    connect(m_audioOutput, &QAudioOutput::mutedChanged, controls, &PlayerControls::setMuted);

    controlLayout->addWidget(controls);
    controlLayout->addStretch(1);

    m_timeline = new QToolButton();   // edited by shaolang
    m_timeline->setToolTip(tr("open timeline window"));
    m_timeline->setIcon(style()->standardIcon(QStyle::SP_ToolBarVerticalExtensionButton));   // edited by shaolang
    m_editor = new QToolButton();   // edited by shaolang
    m_editor->setToolTip(tr("open editor window"));
    m_editor->setIcon(style()->standardIcon(QStyle::SP_ToolBarHorizontalExtensionButton));   // edited by shaolang
    m_help = new QToolButton();   // edited by shaolang
    m_help->setToolTip(tr("help"));
    m_help->setIcon(style()->standardIcon(QStyle::SP_DialogHelpButton));   // edited by shaolang

    // add timeline and editor windows to this main window
    m_timeline_window = new timeline();
    connect(this, &Player::generate_pixmap, m_timeline_window, &timeline::generate_pcm_slot);
    connect(m_player, &QMediaPlayer::positionChanged, m_timeline_window, &timeline::player_move_sonic_waveform);

    m_editor_window = new editor();
    connect(m_timeline, &QToolButton::clicked, this, &Player::switch_timeline_window);
    connect(m_editor, &QToolButton::clicked, this, &Player::switch_editor_window);

    // add help window
    m_help_window = new help();
    connect(m_help, &QToolButton::clicked, this, &Player::switch_help_window);

    //
    m_fullScreenButton = new QPushButton(tr("FullScreen"), this);
    m_fullScreenButton->setCheckable(true);
    m_fullScreenButton->hide();   // edited by shaolang

    controlLayout->addWidget(m_timeline);   // edited by shaolang
    controlLayout->addWidget(m_editor);   // edited by shaolang
    controlLayout->addWidget(m_help);   // edited by shaolang
    controlLayout->addWidget(m_fullScreenButton);

//#if !defined(Q_OS_ANDROID) && !defined(Q_OS_IOS)
//    m_audioOutputCombo = new QComboBox(this);
//    m_audioOutputCombo->addItem(QString::fromUtf8("Default"), QVariant::fromValue(QAudioDevice()));
//    for (auto &deviceInfo: QMediaDevices::audioOutputs())
//        m_audioOutputCombo->addItem(deviceInfo.description(), QVariant::fromValue(deviceInfo));
//    connect(m_audioOutputCombo, QOverload<int>::of(&QComboBox::activated), this,
//            &Player::audioOutputChanged);
//    controlLayout->addWidget(m_audioOutputCombo);   // edited by shaolang
//#endif

    layout->addLayout(controlLayout);

    // tracks
    QGridLayout *tracksLayout = new QGridLayout;

    m_audioTracks = new QComboBox(this);
    m_audioTracks->hide();   // edited by shaolang
    connect(m_audioTracks, &QComboBox::activated, this, &Player::selectAudioStream);
    tracksLayout->addWidget(new QLabel(tr("Audio Tracks:")), 0, 0);
    tracksLayout->addWidget(m_audioTracks, 0, 1);


    m_videoTracks = new QComboBox(this);
    m_videoTracks->hide();   // edited by shaolang
    connect(m_videoTracks, &QComboBox::activated, this, &Player::selectVideoStream);
    tracksLayout->addWidget(new QLabel(tr("Video Tracks:")), 1, 0);
    tracksLayout->addWidget(m_videoTracks, 1, 1);


    m_subtitleTracks = new QComboBox(this);
    m_subtitleTracks->hide();   // edited by shaolang
    connect(m_subtitleTracks, &QComboBox::activated, this, &Player::selectSubtitleStream);
    tracksLayout->addWidget(new QLabel(tr("Subtitle Tracks:")), 2, 0);
    tracksLayout->addWidget(m_subtitleTracks, 2, 1);


    // layout->addLayout(tracksLayout);   // edited by shaolang


#if !defined(Q_OS_ANDROID) && !defined(Q_OS_IOS)
    // metadata

    // QLabel *metaDataLabel = new QLabel(tr("Metadata for file:"));     // edited by shaolang
    // layout->addWidget(metaDataLabel);     // edited by shaolang

    QGridLayout *metaDataLayout = new QGridLayout;
    int key = QMediaMetaData::Title;
    for (int i = 0; i < (QMediaMetaData::NumMetaData + 2) / 3; i++) {
        for (int j = 0; j < 6; j += 2) {
            m_metaDataLabels[key] = new QLabel(
                    QMediaMetaData::metaDataKeyToString(static_cast<QMediaMetaData::Key>(key)));
            if (key == QMediaMetaData::ThumbnailImage || key == QMediaMetaData::CoverArtImage)
                m_metaDataFields[key] = new QLabel;
            else
                m_metaDataFields[key] = new QLineEdit;
            m_metaDataLabels[key]->setDisabled(true);
            m_metaDataFields[key]->setDisabled(true);
            metaDataLayout->addWidget(m_metaDataLabels[key], i, j);
            metaDataLayout->addWidget(m_metaDataFields[key], i, j + 1);
            key++;
            if (key == QMediaMetaData::NumMetaData)
                break;
        }
    }

    // layout->addLayout(metaDataLayout);     // edited by shaolang
#endif

#if defined(Q_OS_QNX)
    // On QNX, the main window doesn't have a title bar (or any other decorations).
    // Create a status bar for the status information instead.
    m_statusLabel = new QLabel;
    m_statusBar = new QStatusBar;
    m_statusBar->addPermanentWidget(m_statusLabel);
    m_statusBar->setSizeGripEnabled(false); // Without mouse grabbing, it doesn't work very well.
    layout->addWidget(m_statusBar);
#endif

    setLayout(layout);

    if (!isPlayerAvailable()) {
        QMessageBox::warning(this, tr("Service not available"),
                             tr("The QMediaPlayer object does not have a valid service.\n"\
                                "Please check the media service plugins are installed."));

        controls->setEnabled(false);
        if (m_playlistView)
            m_playlistView->setEnabled(false);
        openButton->setEnabled(false);
        m_fullScreenButton->setEnabled(false);
    }

    metaDataChanged();

    // load location
    load_location();

    // load states
    load_spiltter_states();

    m_timeline_window->show();
    m_timeline_window->hide();

}

void Player::load_spiltter_states(){

    QString  m_subtitle_splitter_state_ = "";
    QString m_playlist_splitter_state_ = "";

    //1 check and open database
    QSqlDatabase db;
    if(QSqlDatabase::contains("qt_sql_default_connection"))
    {
        db = QSqlDatabase::database("qt_sql_default_connection");
    }
    else
    {
        db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName(QString(source_path).append("/doc/CONFIG.db"));
    }
    // judge weather database is open
    if(!db.open())
    {
        qDebug() << "Error: Failed to connect database." << db.lastError();
    }
    else
    {
        // do something
        write_logs("player.cpp -- load_states", "339", "open database successfully!");
    }

    //2 define a query statement
    QSqlQuery sql_query(db);

    //7 select data
    QString select_sql = "SELECT m_subtitle_splitter, m_playlist_splitter FROM UI_CONFIG WHERE id = 1";
    if(!sql_query.exec(select_sql))
    {
        qDebug() << sql_query.lastError();
    }
    else
    {
        while(sql_query.next())
        {
            m_subtitle_splitter_state_ = sql_query.value(0).toString();
            m_playlist_splitter_state_ = sql_query.value(1).toString();
            write_logs("player.cpp -- load_states", "359", QString("m_subtitle_splitter_state:%1    m_playlist_splitter_state:%2").arg(m_subtitle_splitter_state_).arg(m_playlist_splitter_state_));
        }
    }

    //9 close database
    db.close();
    // restore states of m_subtitle_splitter and m_playlist_splitter
    this->m_subtitle_splitter->restoreState(QByteArray::fromBase64(m_subtitle_splitter_state_.toUtf8()));
    this->m_playlist_splitter->restoreState(QByteArray::fromBase64(m_playlist_splitter_state_.toUtf8()));
}

void Player::load_location()
{
    int player_x = 100, player_y = 100, player_w = 900, player_h = 450;
    int timeline_x = 100, timeline_y = 550, timeline_w = 900, timeline_h = 200;
    int editor_x = 1000, editor_y = 100, editor_w = 300, editor_h = 650;

    //1 check and open database
    QSqlDatabase db;
    if(QSqlDatabase::contains("qt_sql_default_connection"))
    {
        db = QSqlDatabase::database("qt_sql_default_connection");
    }
    else
    {
        db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName(QString(source_path).append("/doc/CONFIG.db"));
    }
    // judge weather database is open
    if(!db.open())
    {
        write_logs("player.cpp -- void Player::load_location()", "332", QString("Error: Failed to connect database.%1").append(db.lastError().text()));
    }
    else
    {
        // do something
        write_logs("player.cpp -- void Player::load_location()", "336", "database opened successfully!");
    }
    //2 define a query statement
    QSqlQuery sql_query(db);
    //7 select data
    QString select_sql = "SELECT player_x, player_y, player_w, player_h, "
                         "timeline_x, timeline_y, timeline_w, timeline_h, "
                         "editor_x, editor_y, editor_w, editor_h "
                         "FROM UI_CONFIG WHERE id = 1";
    if(!sql_query.exec(select_sql))
    {
        qDebug() << sql_query.lastError();
    }
    else
    {
        while(sql_query.next())
        {
            player_x = sql_query.value(0).toInt();
            player_y = sql_query.value(1).toInt();
            player_w = sql_query.value(2).toInt();
            player_h = sql_query.value(3).toInt();
            timeline_x = sql_query.value(4).toInt();
            timeline_y = sql_query.value(5).toInt();
            timeline_w = sql_query.value(6).toInt();
            timeline_h = sql_query.value(7).toInt();
            editor_x = sql_query.value(8).toInt();
            editor_y = sql_query.value(9).toInt();
            editor_w = sql_query.value(10).toInt();
            editor_h = sql_query.value(11).toInt();

            write_logs("palyer.cpp -- void Player::load_location()", "367", " -- successfully load data from database");
        }
    }
    //9 close database
    db.close();
    this->setGeometry(player_x, player_y, player_w, player_h);
    this->m_timeline_window->setGeometry(timeline_x, timeline_y, timeline_w, timeline_h);
    this->m_editor_window->setGeometry(editor_x, editor_y, editor_w, editor_h);

   // load location
}

void Player::help_manual()
{

}

bool Player::isPlayerAvailable() const
{
    return m_player->isAvailable();
}

void Player::open()
{
    QFileDialog fileDialog(this);
    fileDialog.setAcceptMode(QFileDialog::AcceptOpen);
    fileDialog.setWindowTitle(tr("Open Files"));
    fileDialog.setDirectory(QStandardPaths::standardLocations(QStandardPaths::MoviesLocation).value(0, QDir::homePath()));
    if (fileDialog.exec() == QDialog::Accepted)
        addToPlaylist(fileDialog.selectedUrls());

//    generate_subtitle(fileDialog.selectedUrls()[0].toLocalFile(), m_subtitle_stringlist_primary, m_subtitle_stringlist_adjusted, m_subtitle_moment_time_list, m_subtitle_moment_content_list);

}

void Player::switch_timeline_window()
{
    if (this->m_timeline_window->isHidden())
    {
        this->m_timeline_window->show();
    }
    else
    {
        this->m_timeline_window->hide();
    }
}

void Player::switch_editor_window()
{
    if (this->m_editor_window->isHidden())
    {
        this->m_editor_window->show();
    }
    else
    {
        this->m_editor_window->hide();
    }  
}

void Player::switch_help_window()
{
    if (this->m_help_window->isHidden())
    {
        this->m_help_window->show();
    }
    else
    {
        this->m_help_window->hide();
    }
}

static bool isPlaylist(const QUrl &url) // Check for ".m3u" playlists.
{
    if (!url.isLocalFile())
        return false;
    const QFileInfo fileInfo(url.toLocalFile());
    return fileInfo.exists() && !fileInfo.suffix().compare(QLatin1String("m3u"), Qt::CaseInsensitive);
}

void Player::addToPlaylist(const QList<QUrl> &urls)
{
    const int previousMediaCount = m_playlist->mediaCount();
    for (auto &url: urls) {
        if (isPlaylist(url))
            m_playlist->load(url);
        else
            m_playlist->addMedia(url);
    }
    if (m_playlist->mediaCount() > previousMediaCount) {
        auto index = m_playlistModel->index(previousMediaCount, 0);
        if (m_playlistView)
            m_playlistView->setCurrentIndex(index);
        jump(index);
    }
}

void Player::durationChanged(qint64 duration)
{
    m_duration = duration / 1000;
    m_slider->setMaximum(duration);
}

void Player::positionChanged(qint64 progress)
{
    if (!m_slider->isSliderDown())
        m_slider->setValue(progress);

    updateDurationInfo(progress / 1000);
}

void Player::get_subtitle(qint64 progress)
{
    for (qsizetype i = 0; i < m_subtitle_moment_time_list.size(); ++i) {

            if (progress <= m_subtitle_moment_time_list[i])
            {
                if (  (i % 2) == 1  )
                {
                   this->m_subtitle_box->setText( m_subtitle_moment_content_list[i / 2]);
                }
                break;
            }
    }
}

void Player::metaDataChanged()
{
    auto metaData = m_player->metaData();
    setTrackInfo(QString("%1 - %2")
            .arg(metaData.value(QMediaMetaData::AlbumArtist).toString())
            .arg(metaData.value(QMediaMetaData::Title).toString()));

#if !defined(Q_OS_ANDROID) && !defined(Q_OS_IOS)
    for (int i = 0; i < QMediaMetaData::NumMetaData; i++) {
        if (QLineEdit* field = qobject_cast<QLineEdit*>(m_metaDataFields[i]))
            field->clear();
        else if (QLabel* label = qobject_cast<QLabel*>(m_metaDataFields[i]))
            label->clear();
        m_metaDataFields[i]->setDisabled(true);
        m_metaDataLabels[i]->setDisabled(true);
    }

    for (auto &key : metaData.keys()) {
        int i = int(key);
        if (key == QMediaMetaData::CoverArtImage) {
            QVariant v = metaData.value(key);
            if (QLabel *cover = qobject_cast<QLabel*>(m_metaDataFields[key])) {
                QImage coverImage = v.value<QImage>();
                cover->setPixmap(QPixmap::fromImage(coverImage));
            }
        } else if (key == QMediaMetaData::ThumbnailImage) {
            QVariant v = metaData.value(key);
            if (QLabel *thumbnail = qobject_cast<QLabel*>(m_metaDataFields[key])) {
                QImage thumbnailImage = v.value<QImage>();
                thumbnail->setPixmap(QPixmap::fromImage(thumbnailImage));
            }
        } else if (QLineEdit *field = qobject_cast<QLineEdit*>(m_metaDataFields[key])) {
            QString stringValue = metaData.stringValue(key);
            field->setText(stringValue);
        }
        m_metaDataFields[i]->setDisabled(false);
        m_metaDataLabels[i]->setDisabled(false);
    }
#endif
}

QString Player::trackName(const QMediaMetaData &metaData, int index)
{
    QString name;
    QString title = metaData.stringValue(QMediaMetaData::Title);
    QLocale::Language lang = metaData.value(QMediaMetaData::Language).value<QLocale::Language>();

    if (title.isEmpty()) {
        if (lang == QLocale::Language::AnyLanguage)
            name = tr("Track %1").arg(index+1);
        else
            name = QLocale::languageToString(lang);
    } else {
        if (lang == QLocale::Language::AnyLanguage)
            name = title;
        else
            name = QString("%1 - [%2]").arg(title).arg(QLocale::languageToString(lang));
    }
    return name;
}

void Player::tracksChanged()
{
    m_audioTracks->clear();
    m_videoTracks->clear();
    m_subtitleTracks->clear();

    const auto audioTracks = m_player->audioTracks();
    m_audioTracks->addItem(QString::fromUtf8("No audio"), -1);
    for (int i = 0; i < audioTracks.size(); ++i)
        m_audioTracks->addItem(trackName(audioTracks.at(i), i), i);
    m_audioTracks->setCurrentIndex(m_player->activeAudioTrack() + 1);

    const auto videoTracks = m_player->videoTracks();
    m_videoTracks->addItem(QString::fromUtf8("No video"), -1);
    for (int i = 0; i < videoTracks.size(); ++i)
        m_videoTracks->addItem(trackName(videoTracks.at(i), i), i);
    m_videoTracks->setCurrentIndex(m_player->activeVideoTrack() + 1);

    m_subtitleTracks->addItem(QString::fromUtf8("No subtitles"), -1);
    const auto subtitleTracks = m_player->subtitleTracks();
    for (int i = 0; i < subtitleTracks.size(); ++i)
        m_subtitleTracks->addItem(trackName(subtitleTracks.at(i), i), i);
    m_subtitleTracks->setCurrentIndex(m_player->activeSubtitleTrack() + 1);
}

void Player::previousClicked()
{
    // Go to previous track if we are within the first 5 seconds of playback
    // Otherwise, seek to the beginning.
    if (m_player->position() <= 5000) {
        m_playlist->previous();
    } else {
        m_player->setPosition(0);
    }
}

void Player::jump(const QModelIndex &index)
{
    if (index.isValid()) {
        m_playlist->setCurrentIndex(index.row());
    }
}

void Player::playlistPositionChanged(int currentItem)
{
    if (m_playlistView)
        m_playlistView->setCurrentIndex(m_playlistModel->index(currentItem, 0));
    m_player->setSource(m_playlist->currentMedia());

    generate_subtitle(  (m_playlist->currentMedia()).toLocalFile(), m_subtitle_stringlist_primary, m_subtitle_stringlist_adjusted, m_subtitle_moment_time_list, m_subtitle_moment_content_list);
    // emit signal to timeline window, to generate the sonic waveform pixmap
    // emit generate_pixmap((m_playlist->currentMedia()).toLocalFile());
}

void Player::seek(int mseconds)
{
    m_player->setPosition(mseconds);
}

void Player::statusChanged(QMediaPlayer::MediaStatus status)
{
    handleCursor(status);

    // handle status message
    switch (status) {
    case QMediaPlayer::NoMedia:
    case QMediaPlayer::LoadedMedia:
        setStatusInfo(QString());
        break;
    case QMediaPlayer::LoadingMedia:
        setStatusInfo(tr("Loading..."));
        break;
    case QMediaPlayer::BufferingMedia:
    case QMediaPlayer::BufferedMedia:
        setStatusInfo(tr("Buffering %1%").arg(qRound(m_player->bufferProgress()*100.)));
        break;
    case QMediaPlayer::StalledMedia:
        setStatusInfo(tr("Stalled %1%").arg(qRound(m_player->bufferProgress()*100.)));
        break;
    case QMediaPlayer::EndOfMedia:
        QApplication::alert(this);
        m_playlist->next();
        break;
    case QMediaPlayer::InvalidMedia:
        displayErrorMessage();
        break;
    }
}

void Player::handleCursor(QMediaPlayer::MediaStatus status)
{
#ifndef QT_NO_CURSOR
    if (status == QMediaPlayer::LoadingMedia ||
        status == QMediaPlayer::BufferingMedia ||
        status == QMediaPlayer::StalledMedia)
        setCursor(QCursor(Qt::BusyCursor));
    else
        unsetCursor();
#endif
}

void Player::bufferingProgress(float progress)
{
    if (m_player->mediaStatus() == QMediaPlayer::StalledMedia)
        setStatusInfo(tr("Stalled %1%").arg(qRound(progress*100.)));
    else
        setStatusInfo(tr("Buffering %1%").arg(qRound(progress*100.)));
}

void Player::videoAvailableChanged(bool available)
{
    if (!available) {
        disconnect(m_fullScreenButton, &QPushButton::clicked, m_videoWidget, &QVideoWidget::setFullScreen);
        disconnect(m_videoWidget, &QVideoWidget::fullScreenChanged, m_fullScreenButton, &QPushButton::setChecked);
        m_videoWidget->setFullScreen(false);
    } else {
        connect(m_fullScreenButton, &QPushButton::clicked, m_videoWidget, &QVideoWidget::setFullScreen);
        connect(m_videoWidget, &QVideoWidget::fullScreenChanged, m_fullScreenButton, &QPushButton::setChecked);

        if (m_fullScreenButton->isChecked())
            m_videoWidget->setFullScreen(true);
    }
}

void Player::selectAudioStream()
{
    int stream = m_audioTracks->currentData().toInt();
    m_player->setActiveAudioTrack(stream);
}

void Player::selectVideoStream()
{
    int stream = m_videoTracks->currentData().toInt();
    m_player->setActiveVideoTrack(stream);
}

void Player::selectSubtitleStream()
{
    int stream = m_subtitleTracks->currentData().toInt();
    m_player->setActiveSubtitleTrack(stream);
}

void Player::setTrackInfo(const QString &info)
{
    m_trackInfo = info;

    if (m_statusBar) {
        m_statusBar->showMessage(m_trackInfo);
        m_statusLabel->setText(m_statusInfo);
    } else {
        if (!m_statusInfo.isEmpty())
            setWindowTitle(QString("%1 | %2").arg(m_trackInfo).arg(m_statusInfo));
        else
            setWindowTitle(m_trackInfo);
    }
}

void Player::setStatusInfo(const QString &info)
{
    m_statusInfo = info;

    if (m_statusBar) {
        m_statusBar->showMessage(m_trackInfo);
        m_statusLabel->setText(m_statusInfo);
    } else {
        if (!m_statusInfo.isEmpty())
            setWindowTitle(QString("%1 | %2").arg(m_trackInfo).arg(m_statusInfo));
        else
            setWindowTitle(m_trackInfo);
    }
}

void Player::displayErrorMessage()
{
    if (m_player->error() == QMediaPlayer::NoError)
        return;
    setStatusInfo(m_player->errorString());
}

void Player::updateDurationInfo(qint64 currentInfo)
{
    QString tStr;
    if (currentInfo || m_duration) {
        QTime currentTime((currentInfo / 3600) % 60, (currentInfo / 60) % 60,
            currentInfo % 60, (currentInfo * 1000) % 1000);
        QTime totalTime((m_duration / 3600) % 60, (m_duration / 60) % 60,
            m_duration % 60, (m_duration * 1000) % 1000);
        QString format = "mm:ss";
        if (m_duration > 3600)
            format = "hh:mm:ss";
        tStr = currentTime.toString(format) + " / " + totalTime.toString(format);
    }
    m_labelDuration->setText(tStr);
}

void Player::audioOutputChanged(int index)
{
    auto device = m_audioOutputCombo->itemData(index).value<QAudioDevice>();
    m_player->audioOutput()->setDevice(device);
}

void Player::closeEvent(QCloseEvent *event){    // edited by shaolang
    // prompt to save
    if (maybeSave()) {
        event->accept();
    } else {
        event->ignore();
    }

    // do something before quit application
    QList<qint16> ui_configuration;
    ui_configuration.append(this->geometry().x());
    ui_configuration.append(this->geometry().y());
    ui_configuration.append(this->geometry().width());
    ui_configuration.append(this->geometry().height());
    ui_configuration.append(m_timeline_window->geometry().x());
    ui_configuration.append(m_timeline_window->geometry().y());
    ui_configuration.append(m_timeline_window->geometry().width());
    ui_configuration.append(m_timeline_window->geometry().height());
    ui_configuration.append(m_editor_window->geometry().x());
    ui_configuration.append(m_editor_window->geometry().y());
    ui_configuration.append(m_editor_window->geometry().width());
    ui_configuration.append(m_editor_window->geometry().height());

    update_database("UI_CONFIG", ui_configuration);

    write_logs("player.cpp -- Player::closeEvent", "775", "update database successfully!");
    QList<QString> splitter_states;
    QByteArray m_subtitle_splitter_state = m_subtitle_splitter->saveState();
    QByteArray m_playlist_splitter_state = m_playlist_splitter->saveState();
    splitter_states.append(QString::fromUtf8(m_subtitle_splitter_state.toBase64()));
    splitter_states.append(QString::fromUtf8(m_playlist_splitter_state.toBase64()));

    update_database("UI_CONFIG", splitter_states);

    // delete every other windows
    delete m_timeline_window;
    delete m_editor_window;
    delete m_help_window;

}

