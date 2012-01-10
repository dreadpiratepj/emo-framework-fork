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
#include "JavaGlue.h"
#include "Constants.h"
#include "Runtime.h"
#include "VmFunc.h"
#include "Engine.h"

extern emo::Engine* engine;

void initJavaGlueFunctions() {
    registerClassFunc(engine->sqvm, EMO_RUNTIME_CLASS, "nativeEcho",   emoJavaEcho);

    registerClass(engine->sqvm, EMO_NET_CLASS);
    registerClassFunc(engine->sqvm, EMO_NET_CLASS, "request",   emoJavaAsyncHttpRequest);
}

/*
 * Class:     com_emo_framework_EmoActivity
 * Method:    callback
 * Signature: (Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_com_emo_1framework_EmoActivity_callback
  (JNIEnv* env, jobject thiz, jstring jname, jstring jvalue, jstring jerrCode, jstring jerrMsg) {

    char* name;
    char* value;
    char* errCode;
    char* errMsg;

    const char* cname = env->GetStringUTFChars(jname, NULL);
    name = strdup(cname);
    env->ReleaseStringUTFChars(jname, cname);

    const char* cvalue = env->GetStringUTFChars(jvalue, NULL);
    value = strdup(cvalue);
    env->ReleaseStringUTFChars(jvalue, cvalue);

    const char* ccode = env->GetStringUTFChars(jerrCode, NULL);
    errCode = strdup(ccode);
    env->ReleaseStringUTFChars(jerrCode, ccode);

    const char* cmsg = env->GetStringUTFChars(jerrMsg, NULL);
    errMsg = strdup(cmsg);
    env->ReleaseStringUTFChars(jerrMsg, cmsg);

    callSqFunction_Bool_Strings(engine->sqvm, EMO_NAMESPACE, EMO_FUNC_ONCALLBACK, name, value, errCode, errMsg, false);

    free(name);
    free(value);
    free(errCode);
    free(errMsg);
}

namespace emo {
    std::string JavaGlue::echo(std::string value) {
    	return this->callString_String("echo", value);
    }

    void JavaGlue::asyncHttpRequest(std::string name, jint timeout, std::string url, std::string method, kvs_t* params) {
        int count = (params->size() * 2) + 2;

        JNIEnv* env;
        JavaVM* vm = engine->app->activity->vm;

        vm->AttachCurrentThread(&env, NULL);

        jobjectArray jstrArray= (jobjectArray)env->NewObjectArray(count,
             env->FindClass("java/lang/String"),
             env->NewStringUTF(""));

        env->SetObjectArrayElement(jstrArray, 0, env->NewStringUTF(url.c_str()));
        env->SetObjectArrayElement(jstrArray, 1, env->NewStringUTF(method.c_str()));

        int i = 2;
        kvs_t::iterator iter;
        for(iter = params->begin(); iter != params->end(); iter++) {
            env->SetObjectArrayElement(jstrArray, i, env->NewStringUTF(iter->first.c_str()));
            i++;
            env->SetObjectArrayElement(jstrArray, i, env->NewStringUTF(iter->second.c_str()));
            i++;
        }

        jclass clazz = env->GetObjectClass(engine->app->activity->clazz);
        jmethodID methodj = env->GetMethodID(clazz, "asyncHttpRequest", "(Ljava/lang/String;I[Ljava/lang/String;)V");
        env->CallVoidMethod(engine->app->activity->clazz, methodj,  env->NewStringUTF(name.c_str()), timeout, jstrArray);

        vm->DetachCurrentThread();
    }


    int JavaGlue::registerNativeMethods(JNIEnv* env, 
                              JNINativeMethod* gMethods, int numMethods) {

        jclass clazz = env->GetObjectClass(engine->app->activity->clazz);

        if (clazz == NULL) {
            return JNI_FALSE;
        } else  if (env->RegisterNatives(clazz, gMethods, numMethods) < 0) {
            return JNI_FALSE;
        }

        return JNI_TRUE;

    }

    bool JavaGlue::registerJavaGlue() {

        JNINativeMethod methods[] = {
            { "callback", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V",
                  (void *)Java_com_emo_1framework_EmoActivity_callback }

        };

        JNIEnv* env;
        JavaVM* vm = engine->app->activity->vm;
        vm->AttachCurrentThread(&env, NULL);

        int result = registerNativeMethods(env, 
                             methods, sizeof(methods) / sizeof(methods[0]));

        vm->DetachCurrentThread();

        return result == JNI_TRUE;
    }

    /*
     * Call java method with no parameter that returns void.
     */
    void JavaGlue::callVoid_Void(std::string methodName) {
        JNIEnv* env;
        JavaVM* vm = engine->app->activity->vm;

        vm->AttachCurrentThread(&env, NULL);

        jclass clazz = env->GetObjectClass(engine->app->activity->clazz);
        jmethodID methodj = env->GetMethodID(clazz, methodName.c_str(), "()V");
        env->CallVoidMethod(engine->app->activity->clazz, methodj);

        vm->DetachCurrentThread();
    }

    /*
     * Call java method with no parameter that returns string.
     */
    std::string JavaGlue::callVoid_String(std::string methodName) {
        std::string value;
        JNIEnv* env;
        JavaVM* vm = engine->app->activity->vm;

        vm->AttachCurrentThread(&env, NULL);

        jclass clazz = env->GetObjectClass(engine->app->activity->clazz);
        jmethodID methodj = env->GetMethodID(clazz, methodName.c_str(), "()Ljava/lang/String;");
        jstring jstr = (jstring)env->CallObjectMethod(engine->app->activity->clazz, methodj);
        if (jstr != NULL) {
            const char* str = env->GetStringUTFChars(jstr, NULL);
            value = str;
            env->ReleaseStringUTFChars(jstr, str);
        }
        vm->DetachCurrentThread();

        return value;
    }

    /*
     * Call java method with no parameter that returns boolean.
     */
    bool JavaGlue::callVoid_Bool(std::string methodName) {
    	jboolean value = false;

        JNIEnv* env;
        JavaVM* vm = engine->app->activity->vm;

        vm->AttachCurrentThread(&env, NULL);

        jclass clazz = env->GetObjectClass(engine->app->activity->clazz);
        jmethodID methodj = env->GetMethodID(clazz, methodName.c_str(), "()Z");
        value = env->CallBooleanMethod(engine->app->activity->clazz, methodj);
        vm->DetachCurrentThread();

        return value;
    }

    /*
     * Call java method with no parameter that returns int.
     */
    jint JavaGlue::callVoid_Int(std::string methodName) {
        JNIEnv* env;
        JavaVM* vm = engine->app->activity->vm;

        vm->AttachCurrentThread(&env, NULL);

        jclass clazz = env->GetObjectClass(engine->app->activity->clazz);
        jmethodID methodj = env->GetMethodID(clazz, methodName.c_str(), "()I");
        jint value = env->CallIntMethod(engine->app->activity->clazz, methodj);
        vm->DetachCurrentThread();

        return value;
    }

    /*  
     * Call java method with int parameter that returns int.
     */  
    jint JavaGlue::callInt_Int(std::string methodName, jint passValue) {
        JNIEnv* env;
        JavaVM* vm = engine->app->activity->vm;

        vm->AttachCurrentThread(&env, NULL);

        jclass clazz = env->GetObjectClass(engine->app->activity->clazz);
        jmethodID methodj = env->GetMethodID(clazz, methodName.c_str(), "(I)I");
        jint value = env->CallIntMethod(engine->app->activity->clazz, methodj, passValue);
        vm->DetachCurrentThread();

        return value;
    }   

    /*  
     * Call java method with int,int parameter that returns int.
     */  
    jint JavaGlue::callIntInt_Int(std::string methodName, jint passValue1, jint passValue2) {
        JNIEnv* env;
        JavaVM* vm = engine->app->activity->vm;

        vm->AttachCurrentThread(&env, NULL);

        jclass clazz = env->GetObjectClass(engine->app->activity->clazz);
        jmethodID methodj = env->GetMethodID(clazz, methodName.c_str(), "(II)I");
        jint value = env->CallIntMethod(engine->app->activity->clazz, methodj, passValue1, passValue2);
        vm->DetachCurrentThread();

        return value;
    }   

    /*
     * Call java method with one string parameter that returns string.
     */
    std::string JavaGlue::callString_String(std::string methodName, std::string passValue) {
        std::string value;

        JNIEnv* env;
        JavaVM* vm = engine->app->activity->vm;

        vm->AttachCurrentThread(&env, NULL);

        jclass clazz = env->GetObjectClass(engine->app->activity->clazz);
        jmethodID methodj = env->GetMethodID(clazz, methodName.c_str(), "(Ljava/lang/String;)Ljava/lang/String;");
        jstring jstr = (jstring)env->CallObjectMethod(engine->app->activity->clazz, methodj,  env->NewStringUTF(passValue.c_str()));
        if (jstr != NULL) {
            const char* str = env->GetStringUTFChars(jstr, NULL);
            value = str;
            env->ReleaseStringUTFChars(jstr, str);
        }
        vm->DetachCurrentThread();

        return value;
    }

    /*
     * Call java method with two string parameter that returns void.
     */
    void JavaGlue::callTwoString_Void(std::string methodName, std::string value1, std::string value2) {
        JNIEnv* env;
        JavaVM* vm = engine->app->activity->vm;

        vm->AttachCurrentThread(&env, NULL);

        jclass clazz = env->GetObjectClass(engine->app->activity->clazz);
        jmethodID methodj = env->GetMethodID(clazz, methodName.c_str(), "(Ljava/lang/String;Ljava/lang/String;)V");
        env->CallVoidMethod(engine->app->activity->clazz, methodj, env->NewStringUTF(value1.c_str()), env->NewStringUTF(value2.c_str()));
        vm->DetachCurrentThread();
    }

    /*
     * Call java method with (string,int) parameter that returns void.
     */
    void JavaGlue::callStringInt_Void(std::string methodName, std::string value1, jint value2) {
        JNIEnv* env;
        JavaVM* vm = engine->app->activity->vm;

        vm->AttachCurrentThread(&env, NULL);

        jclass clazz = env->GetObjectClass(engine->app->activity->clazz);
        jmethodID methodj = env->GetMethodID(clazz, methodName.c_str(), "(Ljava/lang/String;I)V");
        env->CallVoidMethod(engine->app->activity->clazz, methodj, env->NewStringUTF(value1.c_str()), value2);
        vm->DetachCurrentThread();
    }

    std::string JavaGlue::getDataFilePath(std::string name) {
        return this->callString_String("getDataFilePath", name);
    }

    void JavaGlue::setOrientationLandscape() {

    	int32_t orient = AConfiguration_getOrientation(engine->app->config);
    	if (orient != ACONFIGURATION_ORIENTATION_LAND) {
    		engine->stage->invertSize();
    	}

    	this->callVoid_Void("setOrientationLandscape");
    }

    void JavaGlue::setOrientationPortrait() {
    	int32_t orient = AConfiguration_getOrientation(engine->app->config);
    	if (orient != ACONFIGURATION_ORIENTATION_PORT) {
    		engine->stage->invertSize();
    	}

    	this->callVoid_Void("setOrientationPortrait");
    }

    std::string JavaGlue::getDeviceName() {
    	return this->callVoid_String("getDeviceName");
    }

    std::string JavaGlue::getOSVersion() {
    	return this->callVoid_String("getOSVersion");
    }

    int JavaGlue::getSDKVersion() {
    	return this->callVoid_Int("getSDKVersion");
    }

    int JavaGlue::getNativeOrientation(int width, int height) {
        int jorient = this->callIntInt_Int("getNativeOrientation", width, height);
        if (jorient == 0) {
            return OPT_ORIENTATION_LANDSCAPE;
        } else {
            return OPT_ORIENTATION_PORTRAIT;
        }
    }

    bool JavaGlue::isSimulator() {
    	return this->callVoid_Bool("isSimulator");
    }

    void JavaGlue::vibrate() {
    	this->callVoid_Void("vibrate");
    }

    bool JavaGlue::loadTextBitmap(Drawable* drawable, Image* image, bool forceUpdate) {
        emo::FontDrawable* fontDrawable = reinterpret_cast<emo::FontDrawable*>(drawable);

        image->filename = drawable->name;

        JNIEnv* env;
        JavaVM* vm = engine->app->activity->vm;

        vm->AttachCurrentThread(&env, NULL);

        jclass clazz = env->GetObjectClass(engine->app->activity->clazz);
        jmethodID methodj = env->GetMethodID(clazz, "loadTextBitmap", 
            "(Ljava/lang/String;ILjava/lang/String;ZZLjava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)[B");
        jbyteArray src = (jbyteArray)env->CallObjectMethod(engine->app->activity->clazz, methodj,
                                 env->NewStringUTF(fontDrawable->name.c_str()),
                                 (jint)fontDrawable->fontSize,
                                 env->NewStringUTF(fontDrawable->fontFace.c_str()),
                                 fontDrawable->isBold   ? JNI_TRUE : JNI_FALSE,
                                 fontDrawable->isItalic ? JNI_TRUE : JNI_FALSE,
                                 env->NewStringUTF(fontDrawable->param1.c_str()),
                                 env->NewStringUTF(fontDrawable->param2.c_str()),
                                 env->NewStringUTF(fontDrawable->param3.c_str()),
                                 env->NewStringUTF(fontDrawable->param4.c_str()),
                                 env->NewStringUTF(fontDrawable->param5.c_str()),
                                 env->NewStringUTF(fontDrawable->param6.c_str())
                                 );
        jsize size = env->GetArrayLength(src);

        jbyte* png = env->GetByteArrayElements(src, NULL);
        loadPngFromBytes((unsigned char*)png, size, image, forceUpdate);
        env->ReleaseByteArrayElements(src, png, JNI_ABORT);
        vm->DetachCurrentThread();

        return image->width > 0 && image->height > 0;
    }
}


SQInteger emoJavaEcho(HSQUIRRELVM v) {
    SQInteger nargs = sq_gettop(v);
    for(SQInteger n = 1; n <= nargs; n++) {
		const SQChar *str;
    	if (sq_gettype(v, n) == OT_STRING) {
            sq_tostring(v, n);
            sq_getstring(v, -1, &str);
            sq_poptop(v);
        } else {
            str = NULL;
        }
    	if (str != NULL) {
            std::string value = engine->javaGlue->echo(str);
	    	sq_pushstring(v, value.c_str(), -1);
    	}
    }
	
	return 1;
}

SQInteger emoJavaAsyncHttpRequest(HSQUIRRELVM v) {
    const SQChar* name;
    SQInteger nargs = sq_gettop(v);
    if (nargs >= 2 && sq_gettype(v, 2) == OT_STRING) {
        sq_tostring(v, 2);
        sq_getstring(v, -1, &name);
        sq_poptop(v);
    } else {
        sq_pushinteger(v, ERR_INVALID_PARAM);
        return 1;
    }

    const SQChar* url;
    const SQChar* method;
    SQInteger timeout = 5000;

    if (nargs >= 3 && sq_gettype(v, 3) == OT_STRING) {
        sq_tostring(v, 3);
        sq_getstring(v, -1, &url);
        sq_poptop(v);
    } else {
        sq_pushinteger(v, ERR_INVALID_PARAM);
        return 1;
    }

    if (nargs >= 4 && sq_gettype(v, 4) == OT_STRING) {
        sq_tostring(v, 4);
        sq_getstring(v, -1, &method);
        sq_poptop(v);
    } else {
        method = "GET";
    }

    if (nargs >= 5 && sq_gettype(v, 5) != OT_NULL) {
        sq_getinteger(v, 5, &timeout);
    }

    kvs_t* params = new kvs_t();
    std::string param_key;
    for(SQInteger n = 6; n <= nargs; n++) {

        const SQChar* param;
        sq_tostring(v, n);
        sq_getstring(v, -1, &param);
        sq_poptop(v);

        if (n % 2 == 0) {
            param_key = param;
        } else if (!param_key.empty()) {
            params->insert(std::make_pair(param_key, param));
            param_key.clear();
        }
    }

	if (url != NULL) {
        engine->javaGlue->asyncHttpRequest(name, timeout, url, method, params);
	}
	

    delete params;

    sq_pushinteger(v, EMO_NO_ERROR);
	return 1;
}

