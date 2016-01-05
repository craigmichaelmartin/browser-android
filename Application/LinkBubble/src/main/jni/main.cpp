#include <string.h>
#include <jni.h>
#include "com_linkbubble_adblock_ABPFilterParser.h"
#include "ABPFilterParser.h"

#include <android/log.h>

#define  LOG_TAG    "ABPFilterParser"

#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)


ABPFilterParser parser;

/*
 * Class:     com_linkbubble_adblock_ABPFilterParser
 * Method:    init
 * Signature: ()Ljava/lang/void;
 */
JNIEXPORT void JNICALL Java_com_linkbubble_adblock_ABPFilterParser_init(JNIEnv *env, jobject obj, jbyteArray array) {
    jbyte *bufferPtr = env->GetByteArrayElements(array, NULL);
    jsize lengthOfArray = env->GetArrayLength(array);
    parser.deserialize((char *)bufferPtr);

    // ABPFilterParser uses the data directly for the life of the execution, so we should not release here.
    // env->ReleaseByteArrayElements(array, bufferPtr, 0);
}

/*
 * Class:     com_linkbubble_adblock_ABPFilterParser
 * Method:    stringFromJNI
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jboolean JNICALL Java_com_linkbubble_adblock_ABPFilterParser_shouldBlock(JNIEnv *env, jobject obj, jstring baseHost, jstring input, jstring filterOption) {
    const char *nativeBaseHost = env->GetStringUTFChars(baseHost, 0);
    const char *nativeInput = env->GetStringUTFChars(input, 0);
    const char *nativeFilterOption = env->GetStringUTFChars(filterOption, 0);

    FilterOption currentOption = FONoFilterOption;
    if (0 == strcmp(nativeFilterOption, "/css"))
    {
        currentOption = FOStylesheet;
    }
    else if (0 == strcmp(nativeFilterOption, "image/"))
    {
        currentOption = FOImage;
    }
    else if (0 == strcmp(nativeFilterOption, "javascript"))
    {
        currentOption = FOScript;
    }

    bool shouldBlock = parser.matches(nativeInput, currentOption, nativeBaseHost);

    env->ReleaseStringUTFChars(input, nativeInput);
    env->ReleaseStringUTFChars(baseHost, nativeBaseHost);
    env->ReleaseStringUTFChars(filterOption, nativeFilterOption);

    return shouldBlock ? JNI_TRUE : JNI_FALSE;
}