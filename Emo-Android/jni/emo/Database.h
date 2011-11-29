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
#ifndef EMO_DATABASE_H
#define EMO_DATABASE_H

#include <string>
#include <vector>
#include <jni.h>
#include "sqlite3.h"
#include "squirrel.h"

void initDatabaseFunctions();

namespace emo {
    class Database {
    public:
        Database();
        ~Database();
        std::string getPath(std::string name);
        std::string create(std::string name, jint mode);
        int lastError;
        std::string lastErrorMessage;
        bool openOrCreate(std::string name, jint mode);
        bool open(std::string name);
        bool close();
        bool deleteDatabase(std::string name);
        bool openOrCreatePreference();
        bool openPreference();
        std::string getPreference(std::string key);
        bool setPreference(std::string key, std::string value);
        std::vector<std::string> getPreferenceKeys();
        bool deletePreference(std::string key);
    protected:
        sqlite3* db;
        bool isOpen;
        int  exec(std::string sql);
        int  exec_count(std::string sql, int* count);
        int  query_vector(std::string sql, std::vector<std::string>* values);
    };
}

SQInteger emoDatabaseOpenOrCreate(HSQUIRRELVM v);
SQInteger emoDatabaseOpen(HSQUIRRELVM v);
SQInteger emoDatabaseClose(HSQUIRRELVM v);
SQInteger emoDatabaseGetPath(HSQUIRRELVM v);
SQInteger emoDatabaseGetLastError(HSQUIRRELVM v);
SQInteger emoDatabaseGetLastErrorMessage(HSQUIRRELVM v);
SQInteger emoDatabaseOpenPreference(HSQUIRRELVM v);
SQInteger emoDatabaseOpenOrCreatePreference(HSQUIRRELVM v);
SQInteger emoDatabaseGetPreference(HSQUIRRELVM v);
SQInteger emoDatabaseSetPreference(HSQUIRRELVM v);
SQInteger emoDatabaseDeletePreference(HSQUIRRELVM v);
SQInteger emoDatabaseGetPreferenceKeys(HSQUIRRELVM v);
SQInteger emoDatabaseDeleteDatabase(HSQUIRRELVM v);
#endif