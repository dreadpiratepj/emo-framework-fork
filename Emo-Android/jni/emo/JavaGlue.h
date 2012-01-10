// Copyright (c) 2011 emo-framework project
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
// 
// * Redistributions of source code must retain the above copyright notice,
//   this list of conditions and the following disclaimer.
// * Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
// * Neither the name of the project nor the names of its contributors may be
//   used to endorse or promote products derived from this software without
//   specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS 
// FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
// OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
// OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, 
// EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
// 
#ifndef EMO_JAVAGLUE_H
#define EMO_JAVAGLUE_H

#include <jni.h>
#include <string>
#include <squirrel.h>
#include "Types.h"

void initJavaGlueFunctions();

/*
 * Class:     com_emo_framework_EmoActivity
 * Method:    callback
 * Signature: (Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_com_emo_1framework_EmoActivity_callback
  (JNIEnv *, jobject, jstring, jstring, jstring, jstring);

namespace emo {
    class JavaGlue {
    public:
        std::string echo(std::string echo);
        void asyncHttpRequest(std::string name, jint timeout, std::string url, std::string method, kvs_t* params);
        bool registerJavaGlue();
        int registerNativeMethods(JNIEnv* env, JNINativeMethod* gMethods, int numMethods);
        void callVoid_Void(std::string methodName);
        std::string callVoid_String(std::string methodName);
        bool callVoid_Bool(std::string methodName);
        jint callVoid_Int(std::string methodName);
        jint callInt_Int(std::string methodName, jint passValue);
        jint callIntInt_Int(std::string methodName, jint passValue1, jint passValue2);
        std::string callString_String(std::string methodName, std::string passValue);
        void callTwoString_Void(std::string methodName, std::string value1, std::string value2);
        void callStringInt_Void(std::string methodName, std::string value1, jint value2);
        void setOrientationLandscape();
        void setOrientationPortrait();
        std::string getDeviceName();
        std::string getOSVersion();
        int getSDKVersion();
        int getNativeOrientation(int width, int height);
        bool isSimulator();
        void vibrate();
        std::string getDataFilePath(std::string name);
        bool loadTextBitmap(Drawable* drawable, Image* image, bool forceUpdate);
    };
}

SQInteger emoJavaEcho(HSQUIRRELVM v);
SQInteger emoJavaAsyncHttpRequest(HSQUIRRELVM v);
#endif
