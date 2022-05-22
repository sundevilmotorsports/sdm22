#include "Logger.h"

Logger::Logger(){
    SD.begin(BUILTIN_SDCARD);

    // get previously used run number, use incremented and then save it back into EEPROM
    byte runNo = EEPROM.read(RUN_NO_ADDR);
    // if runNo == 255, then we need to reset it since byte's max size is 255
    if(runNo == 255){
        runNo = 0;
        path = "run" + String(runNo) + "/";
    }
    else{
        path = "run" + String(++runNo) + "/";
    }
    SD.mkdir(path.c_str());
    EEPROM.write(RUN_NO_ADDR, runNo);
}

void Logger::initializeFile(String filename, std::vector<String> columns){
    
    // create SDMFile object
    SDMFile sdmFile;
    sdmFile.name = path + filename + ".csv";
    sdmFile.currentRow.resize(columns.size() + 1); // add an extra column for timestamp
    sdmFile.previousRow.resize(columns.size() + 1);

    // initialize
    for(size_t i = 0; i < sdmFile.currentRow.size(); i++){
        sdmFile.currentRow[i] = SENTINEL_VAL;
        sdmFile.previousRow[i] = SENTINEL_VAL;
    }

    // setup columns
    sdmFile.columns.insert({"timestamp (s)", 0});
    for(size_t i = 0; i < columns.size(); i++){
        sdmFile.columns.insert({columns[i], i+1});
    }
    
    // create csv header
    File file = SD.open(sdmFile.name.c_str(), FILE_WRITE);
    if(file){
        file.print("timestamp (s)");
        for(auto& c : columns){
            file.print(", " + c);
        }
        file.println();
        file.close();
    }

    // save it to our hashmap
    files.insert({sdmFile.name, sdmFile});
}

bool Logger::addData(String filename, String column, float data){
    String key = path + filename + ".csv";
    auto pair = files[key].columns.find(column);
    if(pair != files[key].columns.end()){
        files[key].currentRow[pair->second] = data;
        return true;
    }
    else return false;
    
}

bool Logger::writeRow(String filename){
    String key = path + filename + ".csv";
    File file = SD.open(key.c_str(), FILE_WRITE);
    if(file){
        // first, make timestamp
        files[key].currentRow[0] = millis() / 1000.0;

        // TODO: if a data thing is missing, use previousRow data for it
        for(int i = 0; i < files[key].currentRow[i]; i++){
            if(files[key].currentRow[i] == SENTINEL_VAL)
                files[key].currentRow[i] = files[key].previousRow[i];
        }

        // print data
        for(size_t i = 0; i < files[key].currentRow.size(); i++){
            file.print(files[key].currentRow[i]);
            // if we arent at the last column, print a comma
            if(i != files[key].currentRow.size() - 1)
                file.print(", ");
        }
        file.println();
        file.close();

        // copy currentRow to previousRow
        for(int i = 0; i < files[key].previousRow.size(); i++){
            files[key].previousRow[i] = files[key].currentRow[i];
        }
        // reset currentRow
        for(auto& c : files[key].currentRow)
            c = SENTINEL_VAL;
        
        return true;
    }
    else return false;
}
