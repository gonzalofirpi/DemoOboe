//
// Created by Usuario on 23/5/2023.
//

#ifndef DEMO_AUDIOENGINE_H
#define DEMO_AUDIOENGINE_H


class AudioEngine {

    #include <string.h>
    #include <stdio.h>
    #include <oboe/Oboe.h>
    #include <cstdint>
    #include <jni.h>

    constexpr int32_t kBufferSizeInFrames = 1920;  // Tamaño del buffer de audio

    class MyAudioStreamCallback : public oboe::AudioStreamCallback {
    public:
        DataCallbackResult onAudioReady(oboe::AudioStream* audioStream, void* audioData, int32_t numFrames) override {
            // Obtener los datos de audio del buffer
            int16_t* samples = static_cast<int16_t*>(audioData);

            // Procesar los datos de audio
            // Aquí puedes calcular las ondas de audio o realizar cualquier otra operación

            return DataCallbackResult::Continue;
        }
    };

    extern "C" JNIEXPORT jlong
    Java_AudioEngine_createEngine(JNIEnv *env, jobject /* this */) {
        return reinterpret_cast<jlong>(new MyAudioStreamCallback());
    }

    extern "C" JNIEXPORT void JNICALL
    Java_AudioEngine_deleteEngine(JNIEnv *env, jobject /* this */, jlong engineHandle) {
        delete reinterpret_cast<MyAudioStreamCallback*>(engineHandle);
    }

    extern "C" JNIEXPORT void JNICALL
    Java_AudioEngine_startStream(JNIEnv *env, jobject /* this */, jlong engineHandle) {
        MyAudioStreamCallback* callback = reinterpret_cast<MyAudioStreamCallback*>(engineHandle);

        oboe::AudioStreamBuilder builder;
        builder.setSharingMode(oboe::SharingMode::Exclusive)
                .setPerformanceMode(oboe::PerformanceMode::LowLatency)
                .setCallback(callback)
                .setFormat(oboe::AudioFormat::I16)
                .setChannelCount(oboe::ChannelCount::Mono)
                .setSampleRate(44100)
                .setFramesPerCallback(kBufferSizeInFrames);

        oboe::AudioStream* stream = nullptr;
        builder.openStream(&stream);

        if (stream) {
            stream->start();
        }
    }

    extern "C" JNIEXPORT void JNICALL
    Java_AudioEngine_stopStream(JNIEnv *env, jobject /* this */, jlong engineHandle) {
        MyAudioStreamCallback* callback = reinterpret_cast<MyAudioStreamCallback*>(engineHandle);
        oboe::AudioStream* stream = callback->getAudioStream();

        if (stream) {
            stream->stop();
            stream->close();
        }
    }

};


#endif //DEMO_AUDIOENGINE_H
