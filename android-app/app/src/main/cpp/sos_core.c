#include <jni.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

JNIEXPORT jstring JNICALL
Java_com_shiro_sosapp_MainActivity_buildSosPayload(
        JNIEnv *env, jobject thiz, jstring name, jdouble latitude, jdouble longitude) {
    (void) thiz;

    const char *name_chars = (*env)->GetStringUTFChars(env, name, 0);
    time_t now = time(NULL);
    struct tm *utc = gmtime(&now);

    char ts[32];
    if (utc != NULL) {
        strftime(ts, sizeof(ts), "%Y-%m-%dT%H:%M:%SZ", utc);
    } else {
        strncpy(ts, "unknown-time", sizeof(ts));
        ts[sizeof(ts) - 1] = '\0';
    }

    char out[512];
    snprintf(
            out,
            sizeof(out),
            "SOS ALERT\nUser: %s\nTime(UTC): %s\nLat: %.6f\nLon: %.6f\nStatus: Needs immediate assistance.",
            name_chars,
            ts,
            (double) latitude,
            (double) longitude
    );

    (*env)->ReleaseStringUTFChars(env, name, name_chars);
    return (*env)->NewStringUTF(env, out);
}

JNIEXPORT jint JNICALL
Java_com_shiro_sosapp_MainActivity_severityScore(
        JNIEnv *env, jobject thiz, jstring payload) {
    (void) thiz;
    const char *text = (*env)->GetStringUTFChars(env, payload, 0);
    int score = 40;
    if (strstr(text, "SOS") != NULL) score += 30;
    if (strstr(text, "immediate") != NULL) score += 20;
    if (strstr(text, "Lat:") != NULL && strstr(text, "Lon:") != NULL) score += 10;
    if (score > 100) score = 100;
    (*env)->ReleaseStringUTFChars(env, payload, text);
    return score;
}
