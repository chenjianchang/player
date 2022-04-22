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

#ifndef PLAYER_H
#define PLAYER_H

#include <QWidget>
#include <QMediaPlayer>
#include <qmediaplaylist.h>
#include <QMediaMetaData>
#include <QSplitter>   // edited by shaolang
#include <QFile>   // edited by shaolang
#include <QBuffer>
#include <QByteArray>
#include <QList>
#include <QCloseEvent>

#include "timeline.h"
#include "editor.h"

QT_BEGIN_NAMESPACE
class QAbstractItemView;
class QLabel;
class QMediaPlayer;
class QModelIndex;
class QPushButton;
class QComboBox;
class QSlider;
class QStatusBar;
class QVideoWidget;
class QTextEdit;
class QToolButton;
QT_END_NAMESPACE

class PlaylistModel;

class Player : public QWidget
{
    Q_OBJECT

public:
    explicit Player(QWidget *parent = nullptr);
    ~Player() = default;

    bool isPlayerAvailable() const;

    void addToPlaylist(const QList<QUrl> &urls);
    void closeEvent(QCloseEvent *event);   // edited by shaolang
    void load_location();   // edited by shaolang
    void load_spiltter_states();   // edited by shaolang

signals:
    void fullScreenChanged(bool fullScreen);
    void generate_pixmap(QString video_filename);

private slots:
    void open();
    void durationChanged(qint64 duration);
    void positionChanged(qint64 progress);
    void metaDataChanged();
    void tracksChanged();

    void previousClicked();

    void seek(int mseconds);
    void jump(const QModelIndex &index);
    void playlistPositionChanged(int);

    void statusChanged(QMediaPlayer::MediaStatus status);
    void bufferingProgress(float progress);
    void videoAvailableChanged(bool available);

    void selectAudioStream();
    void selectVideoStream();
    void selectSubtitleStream();

    void displayErrorMessage();

    void audioOutputChanged(int);

    void help_manual();   // edited by shaolang
    void get_subtitle(qint64 progress);   // edited by shaolang

    void switch_timeline_window();
    void switch_editor_window();
    void switch_help_window();

private:
    void setTrackInfo(const QString &info);
    void setStatusInfo(const QString &info);
    void handleCursor(QMediaPlayer::MediaStatus status);
    void updateDurationInfo(qint64 currentInfo);
    QString trackName(const QMediaMetaData &metaData, int index);

    QMediaPlayer *m_player = nullptr;
    QAudioOutput *m_audioOutput = nullptr;
    QMediaPlaylist *m_playlist = nullptr;
    QVideoWidget *m_videoWidget = nullptr;
    QSplitter *m_subtitle_splitter = nullptr;   // edited by shaolang
    QTextEdit *m_subtitle_box = nullptr;   // edited by shaolang
    QSlider *m_slider = nullptr;
    QLabel *m_labelDuration = nullptr;

    QToolButton *m_timeline = nullptr;   // edited by shaolang
    QToolButton *m_editor = nullptr;   // edited by shaolang
    QToolButton *m_help = nullptr;   // edited by shaolang

    QPushButton *m_fullScreenButton = nullptr;
    QComboBox *m_audioOutputCombo = nullptr;
    QLabel *m_statusLabel = nullptr;
    QStatusBar *m_statusBar = nullptr;

    QComboBox *m_audioTracks = nullptr;
    QComboBox *m_videoTracks = nullptr;
    QComboBox *m_subtitleTracks = nullptr;

    PlaylistModel *m_playlistModel = nullptr;
    QSplitter *m_playlist_splitter = nullptr;   // edited by shaolang
    QAbstractItemView *m_playlistView = nullptr;
    QString m_trackInfo;
    QString m_statusInfo;
    qint64 m_duration;

    QWidget *m_metaDataFields[QMediaMetaData::NumMetaData] = {};
    QLabel *m_metaDataLabels[QMediaMetaData::NumMetaData] = {};

    QList<QString> m_subtitle_stringlist_primary;   // edited by shaolang
    QList<QString> m_subtitle_stringlist_adjusted;   // edited by shaolang
    QList<qint64> m_subtitle_moment_time_list;   // edited by shaolang
    QList<QString> m_subtitle_moment_content_list;   // edited by shaolang

    timeline *m_timeline_window = nullptr;
    editor *m_editor_window = nullptr;   // edited by shaolang
    QWidget *m_help_window = nullptr;


};

#endif // PLAYER_H
