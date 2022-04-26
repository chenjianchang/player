#ifndef MY_FFMPEG_H
#define MY_FFMPEG_H

extern "C" {
    #include <libavcodec/avcodec.h>
    #include <libavformat/avformat.h>
    #include <libavdevice/avdevice.h>
    #include <libavfilter/avfilter.h>
    #include <libavutil/error.h>
    #include <libavutil/dict.h>
    #include <libswscale/swscale.h>
    #include <libswresample/swresample.h>
}

#include <QString>
#include <QDebug>

int simplest_pcm16le_split(char *url);
int simplest_pcm16le_to_pcm8(char *url);
int simplest_pcm16le_cut_singlechannel(char *url,int start_num,int dur_num);
int simplest_pcm16le_to_wave(const char *pcmpath,int channels,int sample_rate,const char *wavepath);

bool getAudioInfo(const QString &filepath);
// in order to free memory, using this structor
struct AudioInfoGuard {
    // 格式化I/O上下文
    AVFormatContext *formatCtx = NULL;
    // 解码器
    const AVCodec *codec = NULL;
    // 解码器上下文
    AVCodecContext *codecCtx = NULL;

    ~AudioInfoGuard() {
        if(codecCtx){
            avcodec_free_context(&codecCtx);
        }
        if(formatCtx){
            avformat_close_input(&formatCtx);
            avformat_free_context(formatCtx);
        }
    }
};

#endif // MY_FFMPEG_H
