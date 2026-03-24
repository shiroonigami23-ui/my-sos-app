#include <jni.h>
#include <stdio.h>

#include "geo_math.h"
#include "sos_engine.h"

JNIEXPORT jdouble JNICALL
Java_com_shiro_sosapp_MainActivity_distanceFromBaseKm(
    JNIEnv *env, jobject thiz, jdouble lat, jdouble lon, jdouble baseLat, jdouble baseLon
) {
    (void)env;
    (void)thiz;
    return distance_km((double)lat, (double)lon, (double)baseLat, (double)baseLon);
}

JNIEXPORT jstring JNICALL
Java_com_shiro_sosapp_MainActivity_buildSosPayload(
    JNIEnv *env, jobject thiz, jstring name, jdouble latitude, jdouble longitude, jdouble baseLat, jdouble baseLon
) {
    (void)thiz;
    const char *name_chars = (*env)->GetStringUTFChars(env, name, 0);
    double km = distance_km((double)latitude, (double)longitude, (double)baseLat, (double)baseLon);
    char out[640];
    build_sos_payload(name_chars, (double)latitude, (double)longitude, km, out, sizeof(out));
    (*env)->ReleaseStringUTFChars(env, name, name_chars);
    return (*env)->NewStringUTF(env, out);
}

JNIEXPORT jint JNICALL
Java_com_shiro_sosapp_MainActivity_severityScore(
    JNIEnv *env, jobject thiz, jstring payload, jdouble latitude, jdouble longitude, jdouble baseLat, jdouble baseLon
) {
    (void)thiz;
    const char *text = (*env)->GetStringUTFChars(env, payload, 0);
    double km = distance_km((double)latitude, (double)longitude, (double)baseLat, (double)baseLon);
    int score = severity_score(text, km);
    (*env)->ReleaseStringUTFChars(env, payload, text);
    return score;
}

JNIEXPORT jstring JNICALL
Java_com_shiro_sosapp_MainActivity_payloadDigest(
    JNIEnv *env, jobject thiz, jstring payload
) {
    (void)thiz;
    const char *text = (*env)->GetStringUTFChars(env, payload, 0);
    unsigned int crc = crc32_text(text);
    char out[32];
    snprintf(out, sizeof(out), "CRC32-%08X", crc);
    (*env)->ReleaseStringUTFChars(env, payload, text);
    return (*env)->NewStringUTF(env, out);
}
