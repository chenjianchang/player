#include "my_ffmpeg.h"


/*
 * Split Left and Right channel of 16LE PCM file.
 * @param url Location of PCM file.
 */
int simplest_pcm16le_split(char *url){
    FILE *fp=fopen(url,"rb+");
    FILE *fp1=fopen("output_l.pcm","wb+");
    FILE *fp2=fopen("output_r.pcm","wb+");

    unsigned char *sample=(unsigned char *)malloc(4);

    while(!feof(fp)){
        fread(sample,1,4,fp);
        //L
        fwrite(sample,1,2,fp1);
        //R
        fwrite(sample+2,1,2,fp2);
    }

    free(sample);
    fclose(fp);
    fclose(fp1);
    fclose(fp2);
    return 0;
}


/*
 * Convert PCM-16 data to PCM-8 data.
 * @param url Location of PCM file.
 */
int simplest_pcm16le_to_pcm8(char *url){
    FILE *fp=fopen(url,"rb+");
    FILE *fp1=fopen("output_8.pcm","wb+");

    int cnt=0;

    unsigned char *sample=(unsigned char *)malloc(4);

    while(!feof(fp)){

        short *samplenum16=NULL;
        char samplenum8=0;
        unsigned char samplenum8_u=0;
        fread(sample,1,4,fp);
        //(-32768-32767)
        samplenum16=(short *)sample;
        samplenum8=(*samplenum16)>>8;
        //(0-255)
        samplenum8_u=samplenum8+128;
        //L
        fwrite(&samplenum8_u,1,1,fp1);

        samplenum16=(short *)(sample+2);
        samplenum8=(*samplenum16)>>8;
        samplenum8_u=samplenum8+128;
        //R
        fwrite(&samplenum8_u,1,1,fp1);
        cnt++;
    }
    printf("Sample Cnt:%d\n",cnt);

    free(sample);
    fclose(fp);
    fclose(fp1);
    return 0;
}


/*
 * Cut a 16LE PCM single channel file.
 * @param url        Location of PCM file.
 * @param start_num  start point
 * @param dur_num    how much point to cut
 */
int simplest_pcm16le_cut_singlechannel(char *url,int start_num,int dur_num){
    FILE *fp=fopen(url,"rb+");
    FILE *fp1=fopen("output_cut.pcm","wb+");
    FILE *fp_stat=fopen("output_cut.txt","wb+");

    unsigned char *sample=(unsigned char *)malloc(2);

    int cnt=0;
    while(!feof(fp)){
        fread(sample,1,2,fp);
        if(cnt>start_num&&cnt<=(start_num+dur_num)){
            fwrite(sample,1,2,fp1);

            short samplenum=sample[1];
            samplenum=samplenum*256;
            samplenum=samplenum+sample[0];

            fprintf(fp_stat,"%6d,",samplenum);
            if(cnt%10==0)
                fprintf(fp_stat,"\n",samplenum);
        }
        cnt++;
    }

    free(sample);
    fclose(fp);
    fclose(fp1);
    fclose(fp_stat);
    return 0;
}


/*
 * Convert PCM16LE raw data to WAVE format
 * @param pcmpath      Input PCM file.
 * @param channels     Channel number of PCM file.
 * @param sample_rate  Sample rate of PCM file.
 * @param wavepath     Output WAVE file.
 */
int simplest_pcm16le_to_wave(const char *pcmpath,int channels,int sample_rate,const char *wavepath)
{

    typedef struct WAVE_HEADER{
        char         fccID[4];
        unsigned   long    dwSize;
        char         fccType[4];
    }WAVE_HEADER;

    typedef struct WAVE_FMT{
        char         fccID[4];
        unsigned   long       dwSize;
        unsigned   short     wFormatTag;
        unsigned   short     wChannels;
        unsigned   long       dwSamplesPerSec;
        unsigned   long       dwAvgBytesPerSec;
        unsigned   short     wBlockAlign;
        unsigned   short     uiBitsPerSample;
    }WAVE_FMT;

    typedef struct WAVE_DATA{
        char       fccID[4];
        unsigned long dwSize;
    }WAVE_DATA;


    if(channels==0||sample_rate==0){
        channels = 2;
        sample_rate = 44100;
    }
    int bits = 16;

    WAVE_HEADER   pcmHEADER;
    WAVE_FMT   pcmFMT;
    WAVE_DATA   pcmDATA;

    unsigned   short   m_pcmData;
    FILE   *fp,*fpout;

    fp=fopen(pcmpath, "rb");
    if(fp == NULL) {
        printf("open pcm file error\n");
        return -1;
    }
    fpout=fopen(wavepath,   "wb+");
    if(fpout == NULL) {
        printf("create wav file error\n");
        return -1;
    }
    //WAVE_HEADER
    memcpy(pcmHEADER.fccID,"RIFF",strlen("RIFF"));
    memcpy(pcmHEADER.fccType,"WAVE",strlen("WAVE"));
    fseek(fpout,sizeof(WAVE_HEADER),1);
    //WAVE_FMT
    pcmFMT.dwSamplesPerSec=sample_rate;
    pcmFMT.dwAvgBytesPerSec=pcmFMT.dwSamplesPerSec*sizeof(m_pcmData);
    pcmFMT.uiBitsPerSample=bits;
    memcpy(pcmFMT.fccID,"fmt ",strlen("fmt "));
    pcmFMT.dwSize=16;
    pcmFMT.wBlockAlign=2;
    pcmFMT.wChannels=channels;
    pcmFMT.wFormatTag=1;

    fwrite(&pcmFMT,sizeof(WAVE_FMT),1,fpout);

    //WAVE_DATA;
    memcpy(pcmDATA.fccID,"data",strlen("data"));
    pcmDATA.dwSize=0;
    fseek(fpout,sizeof(WAVE_DATA),SEEK_CUR);

    fread(&m_pcmData,sizeof(unsigned short),1,fp);
    while(!feof(fp)){
        pcmDATA.dwSize+=2;
        fwrite(&m_pcmData,sizeof(unsigned short),1,fpout);
        fread(&m_pcmData,sizeof(unsigned short),1,fp);
    }

    pcmHEADER.dwSize=44+pcmDATA.dwSize;

    rewind(fpout);
    fwrite(&pcmHEADER,sizeof(WAVE_HEADER),1,fpout);
    fseek(fpout,sizeof(WAVE_FMT),SEEK_CUR);
    fwrite(&pcmDATA,sizeof(WAVE_DATA),1,fpout);

    fclose(fp);
    fclose(fpout);

    return 0;
}


/*
 * Get audio info
 * channels had been deprecated, according to source code, use ch_layout  -- nb_channels
 */
bool getAudioInfo(const QString &filepath)
{
    // ffmpeg use utf8
    QByteArray temp = filepath.toUtf8();
    const char *path = temp.constData();

    // use destructor to free memory
    AudioInfoGuard guard;

    // open ifstream to read header of file
    // the stream shoud be closed with avformat_close_input
    // return 0 if read successfully
    int result = avformat_open_input(&guard.formatCtx, path, NULL, NULL);
    if (result !=0 || guard.formatCtx == NULL){
        return false;
    }

    // find stream infomation and save it in AVFormatContext
    // return 0 if successfully
    if (avformat_find_stream_info(guard.formatCtx, NULL) < 0) {
        return false;
    }

    // fetch meta info like song names, singers etc.
    // AVDictionaryEntry *tag = NULL;
    // while (tag = av_dict_get(fmt_ctx->metadata, "", tag, AV_DICT_IGNORE_SUFFIX))
    // {
    //     qDebug() << tag->key << tag->value;
    // }

    qDebug() << "filepath: " << filepath;
    //duration/AV_TIME_BASE -- second
    qDebug() << "duration: " << guard.formatCtx->duration/(AV_TIME_BASE/1000.0) << "ms";
    // file formats, such as -- wav
    qDebug() << "format: " << guard.formatCtx->iformat->name << ":" << guard.formatCtx->iformat->long_name;
    // this is the file container's bit rates
    qDebug() << "bit rate: " << guard.formatCtx->bit_rate << "bps";
    qDebug() << "n stream: " << guard.formatCtx->nb_streams;

    for (unsigned int i = 0; i < guard.formatCtx->nb_streams; i++)
    {
        #if 1
            // AVStream is a structor saving video/audio data
            AVStream *in_stream = guard.formatCtx -> streams[i];

            // type is audio
            if(in_stream->codecpar->codec_type == AVMEDIA_TYPE_AUDIO){
                // find decoder with the corresponding codec ID
                // failed return NULL
                guard.codec = avcodec_find_decoder(in_stream->codecpar->codec_id);

                if(guard.codec == NULL){
                    return false;
                }

                // define AVCodecContext -- guard.codecCtx
                // it should be freed by avcodec_free_context
                // if failed, return NULL
                guard.codecCtx = avcodec_alloc_context3(guard.codec);
                if (guard.codecCtx == NULL){
                    return false;
                }

                // use codec parameters to fill the codec context
                // copy -- AVCodecParameters to -- AVCodecContext
                // return >= 0 if successfully
                if(avcodec_parameters_to_context(guard.codecCtx, in_stream->codecpar) < 0){
                    return false;
                }

                // open codec
                // use AVCodec to initialize AVCodecContext
                // but it should be allocated context use -- avcodec_alloc_context3()
                // return 0 if successfully
                if(avcodec_open2(guard.codecCtx, guard.codec, nullptr)!=0){
                    return false;
                }

                // sample rate
                qDebug() << "sample rate: " << guard.codecCtx->sample_rate;
                // channels
                qDebug() << "channels: " << guard.codecCtx->ch_layout.nb_channels;
                qDebug() << "sample bit: " << guard.codecCtx->sample_fmt<< ":" << (av_get_bytes_per_sample(guard.codecCtx->sample_fmt) << 3);
                // audio bit rate
                qDebug() << "bit rate: " << guard.codecCtx->bit_rate;
                // codec name, such as PCM
                qDebug() << "codec name: " << guard.codec->name << ":" << guard.codec->long_name;
                return true;
            }
        #else
            // below was deprecated
            AVStream *in_stream = fmt_ctx->streams[i];
            AVCodecContext *avctx = in_stream->codec;
            if (avctx->codec_type == AVMEDIA_TYPE_VIDEO){
                // video info -- omit
            }
            else if (avctx->codec_type == AVMEDIA_TYPE_AUDIO)
            {
                // audio info
                qDebug() << "sample rate" << in_stream->codec->sample_rate;
                qDebug() << "channels" << in_stream->codec->channels;

                qDebug() << "sample bit" << in_stream->codec->sample_fmt << ":" << (av_get_bytes_per_sample(in_stream->codec->sample_fmt)<<3);

                AVCodec *codec = avcodec_find_decoder(avctx->codec_id);
                if(codec == NULL){
                    return;
                }
                qDebug() << "codec name" << codec->name << ":" << codec->long_name;
                return true;
            }
        #endif
    }
    return false;
}

