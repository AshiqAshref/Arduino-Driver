#ifndef AV_FUNCTIONS_H
#define AV_FUNCTIONS_H
#include <Arduino.h>
#include <RTClib.h>
#include <ArduinoJson.h>
#include <IO_PINS.h>


class AV_Functions {
    static bool validate_box_Json_local(JsonDocument doc){
        if( doc.size()==0) return false;
        if(!doc["a"].is<uint16_t>()) return false;
        if(!doc["b"].is<uint8_t>()) return false;
        if( doc["b"].as<uint8_t>()==0) return false;
        return true;
    }

public:
    AV_Functions()=default;
    ~AV_Functions()=default;
    static void waitForInput(){
        flush_();
        while(!Serial.available()) {}
        Serial.flush();
        while(Serial.available())Serial.read();
    }
    static void flush_(){
        Serial.flush();
        while(Serial.available())Serial.read();
    }

    static String get_current_formated_Time(byte mode=24);
    static uint32_t get_current_unix_time();
    static String get_formated_Time(const DateTime &curr_time, const byte mode=12){
        if(mode == 12)
            return
                beautifyTime(curr_time.twelveHour())+":"
                +beautifyTime(curr_time.minute())+":"
                +beautifyTime(curr_time.second())+" "
                +(curr_time.isPM()? "p": " ");
        return
            beautifyTime(curr_time.hour())+":"
            +beautifyTime(curr_time.minute()) +":"
            +beautifyTime(curr_time.second());
    }
    static String beautifyTime(const uint8_t h_m_s){
        if(h_m_s<10)
            return '0'+static_cast<String>(h_m_s);
        return static_cast<String>(h_m_s);
    }
    static void printBin(const byte aByte){
        Serial.print('(');
        for (int8_t aBit = 7; aBit >= 0; aBit--) {
            Serial.print(bitRead(aByte, aBit) ? '1' : '0');
            if(aBit==4) Serial.print(' ');
        }
        Serial.print(')');
    }
    static void printlnBin(const byte aByte){
        printBin(aByte);
        Serial.println();
    }
    static JsonDocument simplify_Json(const JsonDocument &doc){
        JsonDocument reminder_doc;
        if(doc["time"].is<String>())
            reminder_doc["t"] = doc["time"].as<String>();
        else reminder_doc["t"] = "";

        const auto med_array = reminder_doc["m"].to<JsonArray>();
        const size_t med_array_size = doc["medicines"].size();
        if(med_array_size>0) {
            for (size_t j = 0; j < med_array_size; j++) {
                JsonDocument med_doc;
                if (doc["medicines"][j]["medBox"].is<int>())
                    med_doc["b"] = doc["medicines"][j]["medBox"];
                else med_doc["b"] = 0;

                if (doc["medicines"][j]["dosage"].is<int>())
                    med_doc["d"] = doc["medicines"][j]["dosage"];
                else med_doc["d"] = 0;

                // ReSharper disable once CppExpressionWithoutSideEffects
                med_array.add(med_doc);
            }
        }else {
            med_array.add(JsonDocument());
        }
        return reminder_doc;
    }
    static JsonDocument unsimplify_Json(const JsonDocument &doc){
        JsonDocument main_doc;
        JsonDocument empty_doc;

        auto main_doc_array= main_doc.to<JsonArray>();
        size_t doc_array_size = doc.size();
        if(doc_array_size==0)
            return empty_doc;
        for(size_t i=0; i<doc_array_size; i++){
            JsonDocument reminder_doc;

            if(doc[i]["t"].is<String>()) {
                reminder_doc["time"] = doc[i]["t"].as<String>();
            }else return empty_doc;

            if(doc[i]["rv"].is<int>()) {
                reminder_doc["rv"] = doc[i]["rv"].as<String>();
            }else return empty_doc;

            auto med_array = reminder_doc["medicines"].to<JsonArray>();
            if(doc[i]["m"].size()>0) {
                size_t med_array_size = doc[i]["m"].size();
                for(size_t j=0; j<med_array_size; j++){
                    JsonDocument med_doc;
                    if(doc[i]["m"][j]["b"].is<int>()) {
                        med_doc["medBox"] = doc[i]["m"][j]["b"];
                    }else return empty_doc;
                    if(doc[i]["m"][j]["d"].is<int>()) {
                        med_doc["dosage"] = doc[i]["m"][j]["d"];
                    }else return empty_doc;

                    // ReSharper disable once CppExpressionWithoutSideEffects
                    med_array.add(med_doc);
                }
            }else return empty_doc;
            // ReSharper disable once CppExpressionWithoutSideEffects
            main_doc_array.add(reminder_doc);
        }
        return main_doc;
    }

    static bool validate_box_Json(JsonDocument doc){
        if(doc.as<JsonArray>().isNull())
            return validate_box_Json_local(doc);
        for (int i=0; i<doc.size(); ++i)
            if(!validate_box_Json_local(doc[i])) return false;
        return true;
    }


    static byte extractHour(const String &formated_time){
        return formated_time.substring(0, 2).toInt();
    }
    static byte extractMinute(const String &formated_time){
        return formated_time.substring(3, 5).toInt();
    }

    static void beepFor(const uint16_t delay_){
        digitalWrite(OUTPUT_PIN_BEEPER,LOW);
        delay(delay_);
        digitalWrite(OUTPUT_PIN_BEEPER,HIGH);
    }

    static uint8_t checkForButtonPress(const uint8_t * buttons, const uint8_t size){
        for(uint8_t i=0; i<size; i++) {
            const uint8_t button = checkForButtonPress(buttons[i]);
            if(button!=0) return button;
        }return 0;
    }
    static uint8_t checkForButtonPress(const uint8_t button){
        if(digitalRead(button)) {
            beepFor(100);
            while(digitalRead(button)){}
            return button;
        }return 0;
    }
};
#endif //AV_FUNCTIONS_H
