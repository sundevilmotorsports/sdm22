#include "Logger.h"

Logger::Logger(){
    SD.begin(BUILTIN_SDCARD);
    // TODO: get the previously used run number, increment it, save back to eeprom
    // and use that for the mkdir number
    // TODO if SD.begin fails, set a boolean to false (which means we shouldnt log)
    path = "run01/"; // tmp
    SD.mkdir(path.c_str());
}

void Logger::initializeFile(String filename, std::vector<String> columns){
    
    // create SDMFile object
    SDMFile sdmFile;
    sdmFile.name = path + filename + ".csv";
    sdmFile.currentRow.resize(columns.size() + 1); // add an extra column for timestamp
    sdmFile.columns.insert({"timestamp (s)", 0});
    for(size_t i = 0; i < columns.size(); i++){
        sdmFile.columns.insert({columns[i], i+1});
    }
    
    // create csv header
    File file = SD.open(sdmFile.name.c_str(), FILE_WRITE);
    if(file){
        file.print("time");
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

        // print data
        for(size_t i = 0; i < files[key].currentRow.size(); i++){
            file.print(files[key].currentRow[i]);
            // if we arent at the last column, print a comma
            if(i != files[key].currentRow.size() - 1)
                file.print(", ");
        }
        file.println();
        file.close();

        // reset currentRow
        for(auto& c : files[key].currentRow)
            c = 0.0;
        
        return true;
    }
    else return false;
}
