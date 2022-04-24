#include "timeline.h"
#include "my_ffmpeg.h"
#include "g_variable.h"

#include <QWidget>
#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QString>
#include <QStringList>
#include <QPushButton>
#include <QDebug>
#include <stdio.h>


timeline::timeline(){

    this->setWindowTitle(tr("timeline"));
    this->setWindowIcon(QIcon(":/Icon/timeline_editor.png"));
    this->hide();

    // create UI
    current_moment = new QLineEdit(this);
    current_moment->setParent(this);
    current_moment->setAlignment(Qt::AlignCenter);
    current_moment->setFixedWidth(150);
    current_moment->setText("current moment");
    spacer = new QSpacerItem(10,10, QSizePolicy::Expanding);
    zoom = new QLabel(tr("Zoom:"));
    zoom->setParent(this);
    zoom_box = new QComboBox();
    zoom_box->addItem("0.5");
    zoom_box->addItem("1");
    zoom_box->addItem("2");
    zoom_box->addItem("3");
    zoom_box->addItem("4");

    sonic_panel_frame = new sonic_panel();

    sonic_waveform_frame = new sonic_waveform();
    sonic_waveform_frame->setParent(this);
    sonic_waveform_frame->show();

    slider = new QSlider(Qt::Horizontal);

    h_layout = new QHBoxLayout;
    v_layout = new QVBoxLayout;

    h_layout->addWidget(current_moment);
    h_layout->addSpacerItem(spacer);
    h_layout->addWidget(zoom);
    h_layout->addWidget(zoom_box);

    v_layout->addLayout(h_layout);
    v_layout->addWidget(sonic_panel_frame);
    v_layout->addWidget(slider);
    this->setLayout(v_layout);

    this->sonic_panel_frame->setAttribute(Qt::WA_TransparentForMouseEvents);  // set sonic_panel_frame mouse event

    connect(this, &timeline::send_info_to_sonic_waveform_frame,
            this->sonic_waveform_frame, &sonic_waveform::receive_info_from_timeline);


}

void timeline::resizeEvent(QResizeEvent *event){
    Q_UNUSED(event);
    sonic_waveform_width = this->sonic_panel_frame->width();
    sonic_waveform_height = this->sonic_panel_frame->height();
//    this->sonic_waveform_frame->resize(sonic_waveform_width, sonic_waveform_height);
    this->sonic_waveform_frame->move(this->sonic_panel_frame->x(), this->sonic_panel_frame->y());
    this->sonic_waveform_frame->lower();
    qDebug() << "resize";
    emit send_info_to_sonic_waveform_frame(this->sonic_panel_frame->x(), this->sonic_panel_frame->y(),
                                           sonic_waveform_width, sonic_waveform_height);
}

void timeline::generate_pcm_slot(QString video_filename){

    qDebug() << "generate_pcm_slot ...";
    generate_pcm *generate_pcm_thread = new generate_pcm(video_filename);
    connect(generate_pcm_thread, &generate_pcm::resultReady, this, &timeline::generate_pixmaps_slot);
    generate_pcm_thread->run();

    delete generate_pcm_thread;
}

void timeline::generate_pixmaps_slot(QString pcm_filename){

    generate_pix *generate_pixmaps_thread = new generate_pix(pcm_filename, sonic_waveform_width, sonic_waveform_height);
    connect(generate_pixmaps_thread, &generate_pix::resultReady, this->sonic_waveform_frame, &sonic_waveform::add_pixmaps);
    generate_pixmaps_thread->run();

    delete generate_pixmaps_thread;
    qDebug() << "generate_pixmaps_slot ...";
}

void timeline::nothing()
{
//    qDebug() << "pixmaps are all generated!";
//    qDebug() << "av_version_info" << av_version_info();
//    qDebug() << "avcodec_version" << avcodec_version();
//    const char *ifilename = "C:/Users/riben/Desktop/player/temp/introduction.mp4";
//    qDebug() << "in_ifilename=" << ifilename;
//    AVFormatContext *ifmt_ctx = NULL;
//    // 1. open file
//    int ret = avformat_open_input(&ifmt_ctx, ifilename, NULL, NULL);
//    if(ret < 0){
//        char buf[1024] = {0};
//        av_strerror(ret, buf, sizeof(buf) - 1);
//        printf("open:%s failed:%s\n", ifilename, buf);
//        return;
//    }
//    // find the position of audio stream in the file
//    int streamIndex = 0;
//    for (uint i = 0; i < ifmt_ctx -> nb_streams; ++i){
//        enum AVMediaType avMediaType = ifmt_ctx ->streams[i]->codecpar->codec_type;
//        if(avMediaType == AVMEDIA_TYPE_AUDIO){
//            streamIndex = i;
//        }
//    }
//    // audio parameters
//    AVCodecParameters *avCodecParameters = ifmt_ctx->streams[streamIndex]->codecpar;

//    // 2. read stream
//    ret = avformat_find_stream_info(ifmt_ctx, NULL);
//    if (ret < 0)
//    {
//        char buf[1024] = { 0 };
//        av_strerror(ret, buf, sizeof(buf) - 1);
//        printf("avformat_find_stream_info %s failed:%s\n", ifilename, buf);
//        avformat_close_input(&ifmt_ctx);
//        return;
//    }
//    // 3. print messages
//    qDebug() << QString("\n==== av_dump_format in_filename:%1 ===\n").arg(ifilename);

//    av_dump_format(ifmt_ctx, 0, ifilename, 0);
//    qDebug() << QString("\n==== av_dump_format finish =======\n\n");
//    qDebug() << QString("media name:%1").arg(ifmt_ctx->url);
//    qDebug() << QString("stream number:%1").arg(ifmt_ctx->nb_streams);
//    printf("media average ratio:%lldkbps\n",(int64_t)(ifmt_ctx->bit_rate/1024));
//    qDebug() << QString("media average ratio:%1 bps").arg((int64_t)(ifmt_ctx->bit_rate/1024));

//    // duration
//    int total_seconds = (ifmt_ctx->duration) / AV_TIME_BASE;  // 1000us = 1ms, 1000ms = 1s
//    printf("audio duration: %02d:%02d:%02d\n",
//           total_seconds / 3600, (total_seconds % 3600) / 60, (total_seconds % 60));
//    qDebug() << QString("audio duration: %1:%2:%3")
//                .arg(total_seconds / 3600, 2, 10, QChar('0'))
//                .arg((total_seconds % 3600) / 60, 2, 10, QChar('0'))
//                .arg((total_seconds % 60), 2, 10, QChar('0'));

//    // 4. read stream info
//    // audio
//    int audioindex = av_find_best_stream(ifmt_ctx, AVMEDIA_TYPE_AUDIO, -1, -1, NULL, 0);

//    if (audioindex < 0) {
//        printf("av_find_best_stream %s eror.", av_get_media_type_string(AVMEDIA_TYPE_AUDIO));
//        qDebug() << QString("av_find_best_stream %1 eror.").arg(av_get_media_type_string(AVMEDIA_TYPE_AUDIO));
//        return;
//    }
//    AVStream *audio_stream = ifmt_ctx->streams[audioindex];
//    printf("----- Audio info:\n");
//    printf("index: %d\n", audio_stream->index);
//    printf("sample arate: %d Hz\n", audio_stream->codecpar->sample_rate); // sample rate
//    printf("sample format: %d\n", audio_stream->codecpar->format); // sample format -- AV_SAMPLE_FMT_FLTP:8
//    printf("audio codec: %d\n", audio_stream->codecpar->codec_id); // audio codec -- AV_CODEC_ID_MP3:86017 AV_CODEC_ID_AAC:86018
//    if (audio_stream->duration != AV_NOPTS_VALUE) {
//        int audio_duration = audio_stream->duration * av_q2d(audio_stream->time_base);
//        printf("audio duration: %02d:%02d:%02d\n",
//               audio_duration / 3600, (audio_duration % 3600) / 60, (audio_duration % 60));
//    }

//    // video
//    int videoindex = av_find_best_stream(ifmt_ctx, AVMEDIA_TYPE_VIDEO, -1, -1, NULL, 0);
//    if (videoindex < 0) {
//        printf("av_find_best_stream %s eror.", av_get_media_type_string(AVMEDIA_TYPE_VIDEO));
//        return;
//    }
//    AVStream *video_stream = ifmt_ctx->streams[videoindex];
//    printf("----- Video info:\n");
//    printf("index: %d\n", video_stream->index);
//    printf("fps: %lf\n", av_q2d(video_stream->avg_frame_rate)); // frame rate
//    printf("width: %d, height:%d \n", video_stream->codecpar->width, video_stream->codecpar->height);
//    printf("video codec: %d\n", video_stream->codecpar->codec_id); // video codec -- AV_CODEC_ID_H264: 27
//    if (video_stream->duration != AV_NOPTS_VALUE) {
//        int video_duration = video_stream->duration * av_q2d(video_stream->time_base);
//        printf("audio duration: %02d:%02d:%02d\n",
//               video_duration / 3600, (video_duration % 3600) / 60, (video_duration % 60));
//    }

//    // 5. read frame
//    AVPacket *pkt = av_packet_alloc();
//    int pkt_count = 0;
//    int print_max_count = 10;
//    printf("\n-----av_read_frame start\n");
//    while (1)
//    {
//        ret = av_read_frame(ifmt_ctx, pkt);
//        if (ret < 0) {
//            printf("av_read_frame end\n");
//            break;
//        }

//        if(pkt_count++ < print_max_count)
//        {
//            if (pkt->stream_index == audioindex)
//            {
//                printf("audio pts: %lld\n", pkt->pts);
//                printf("audio dts: %lld\n", pkt->dts);
//                printf("audio size: %d\n", pkt->size);
//                printf("audio pos: %lld\n", pkt->pos);
//                printf("audio duration: %lf\n\n",
//                       pkt->duration * av_q2d(ifmt_ctx->streams[audioindex]->time_base));
//            }
//            else if (pkt->stream_index == videoindex)
//            {
//                printf("video pts: %lld\n", pkt->pts);
//                printf("video dts: %lld\n", pkt->dts);
//                printf("video size: %d\n", pkt->size);
//                printf("video pos: %lld\n", pkt->pos);
//                printf("video duration: %lf\n\n",
//                       pkt->duration * av_q2d(ifmt_ctx->streams[videoindex]->time_base));
//            }
//            else
//            {
//                printf("unknown stream_index:%d\n", pkt->stream_index);
//            }
//        }
//        av_packet_unref(pkt);
//    }

//    // 6. last
//    if(pkt)
//        av_packet_free(&pkt);
//    if(ifmt_ctx)
//        avformat_close_input(&ifmt_ctx);


//    getchar(); // pause
//    return;
}





















