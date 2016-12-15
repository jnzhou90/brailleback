/*
 * Copyright (C) 2009 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package com.google.tts;

import com.google.tts.ITtsCallbackBeta;

import android.content.Intent;

/**
 * AIDL for the TTS Service
 * ITts.java is autogenerated from this.
 *
 * {@hide}
 */
interface ITtsBeta {
    int setSpeechRate(in String callingApp, in int speechRate);

    int setPitch(in String callingApp, in int pitch);

    int speak(in String callingApp, in String text, in int queueMode, in String[] params);

    boolean isSpeaking();

    int stop(in String callingApp);

    void addSpeech(in String callingApp, in String text, in String packageName, in int resId);

    void addSpeechFile(in String callingApp, in String text, in String filename);

    String[] getLanguage();

    int isLanguageAvailable(in String language, in String country, in String variant, in String[] params);

    int setLanguage(in String callingApp, in String language, in String country, in String variant);

    boolean synthesizeToFile(in String callingApp, in String text, in String[] params, in String outputDirectory);

    int playEarcon(in String callingApp, in String earcon, in int queueMode, in String[] params);

    void addEarcon(in String callingApp, in String earcon, in String packageName, in int resId);

    void addEarconFile(in String callingApp, in String earcon, in String filename);

    int registerCallback(in String callingApp, ITtsCallbackBeta cb);

    int unregisterCallback(in String callingApp, ITtsCallbackBeta cb);

    int playSilence(in String callingApp, in long duration, in int queueMode, in String[] params);
    
    int setEngineByPackageName(in String enginePackageName);

    String getDefaultEngine();

    boolean areDefaultsEnforced();
}
