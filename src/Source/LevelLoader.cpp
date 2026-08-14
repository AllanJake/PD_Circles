#include "../Header/LevelLoader.h"

#include <cstring>
#include <string>
#include <vector>

struct ParseState {
    PlaydateAPI* pd = nullptr;
    std::vector<CircleLevelData>* circles = nullptr;

    CircleLevelData currentCircle;
    bool insideCirclesArray = false;
    bool insideCircleObject = false;
    bool insideTagsArray = false;
};

int ReadFileForJson(void* userdata, uint8_t* buf, int bufSize) {
    PlaydateAPI* pd = nullptr;
    SDFile* file = static_cast<SDFile*>(userdata);

    extern PlaydateAPI* gPlaydateAPIForJsonRead;
    pd = gPlaydateAPIForJsonRead;
    
    int result = pd->file->read(file, buf, static_cast<unsigned int>(bufSize));
    return result > 0 ? result : 0;
}

// This global is only used for ReadFileForJson because json_reader gives us
// one userdata pointer, which we use for SDFile*
PlaydateAPI* gPlaydateAPIForJsonRead = nullptr;

void DecodeError(json_decoder* decoder, const char* error, int linenum) {
    auto* state = static_cast<ParseState*>(decoder->userdata);
    if (state && state->pd) {
        state->pd->system->logToConsole("Level JSON decode error on line %d: %s", linenum, error);
    }
}

void WillDecodeSublist(json_decoder* decoder, const char* name, json_value_type type) {
    auto* state = static_cast<ParseState*>(decoder->userdata);
    if (!state) return;

    // The root field 'circles' is an array.
    if (name && std::strcmp(name, "circles") == 0 && type == kJSONArray) {
        state->insideCirclesArray = true;
        return;
    }

    // Each item inside circels is an object/table
    // Depending on SDK internals, array item names may be null, so this
    // intentionally keys off the fact that we are currently inside circles
    if (state->insideCirclesArray && type == kJSONTable) {
        state ->currentCircle = CircleLevelData{};
        state->insideCircleObject = true;
        return;
    }

    // 'tags' is an array inside a circle object
    if (state->insideCircleObject && name && std::strcmp(name, "tags") == 0 && type == kJSONArray) {
        state->insideTagsArray = true;
        return;
    }

}
void DidDecodeTableValue(json_decoder* decoder, const char* key, json_value value) {
    auto* state = static_cast<ParseState*>(decoder->userdata);
    if (!state || !state->insideCircleObject || !key) return;

    CircleLevelData& circle = state->currentCircle;

    if (std::strcmp(key, "id") == 0) {
        if (char* s = json_stringValue(value)) {
            circle.id = s;
        }
    }
    else if (std::strcmp(key, "parentId") == 0) {
        if (char* s = json_stringValue(value)) {
            circle.parentId = s;
        }
    }
    else if (std::strcmp(key, "x") == 0) {
        circle.x = json_floatValue(value);
    }
    else if (std::strcmp(key, "y") == 0) {
        circle.y = json_floatValue(value);
    }
    else if (std::strcmp(key, "radius") == 0) {
        circle.radius = json_floatValue(value);
    }
}

void DidDecodeArrayValue(json_decoder* decoder, int pos, json_value value) {
    auto* state = static_cast<ParseState*>(decoder->userdata);
    if (!state) return;

    if (state->insideTagsArray && value.type == kJSONString) {
        if (char* s = json_stringValue(value)) {
            state->currentCircle.tags.push_back(s);
        }
    }
}

void* DidDecodeSublist(json_decoder* decoder, const char* name, json_value_type type) {
    auto* state = static_cast<ParseState*>(decoder->userdata);
    if (!state) return nullptr;

    if (state->insideTagsArray && type == kJSONArray) {
        state->insideTagsArray = false;
        return nullptr;
    }

    if (state->insideCircleObject && type == kJSONTable) {
        if (!state->currentCircle.id.empty()) {
            state->circles->push_back(state->currentCircle);
        }
        state->insideCircleObject = false;
        return nullptr;
    }

    if (state->insideCirclesArray && name && std::strcmp(name, "circles") == 0 && type == kJSONArray) {
        state->insideCirclesArray = false;
        return nullptr;
    }
    return nullptr;
}

bool LevelLoader::LoadLevel(PlaydateAPI* pd, const char* path, std::vector<CircleLevelData>& outCircles) {
    outCircles.clear();

    SDFile* file = pd->file->open(path, kFileRead);
    if (!file) {
        pd->system->logToConsole("Could not open level JSON '%s': %s", path, pd->file->geterr());
        return false;
    }

    ParseState state;
    state.pd = pd;
    state.circles = &outCircles;

    json_decoder decoder{};
    decoder.decodeError = DecodeError;
    decoder.willDecodeSublist = WillDecodeSublist;
    decoder.didDecodeTableValue = DidDecodeTableValue;
    decoder.didDecodeArrayValue = DidDecodeArrayValue;
    decoder.didDecodeSublist = DidDecodeSublist;
    decoder.userdata = &state;

    gPlaydateAPIForJsonRead = pd;

    json_reader reader;
    reader.read = ReadFileForJson;
    reader.userdata = file;

    int decodeResult = pd->json->decode(&decoder, reader, nullptr);

    pd->file->close(file);
    gPlaydateAPIForJsonRead = nullptr;

    if (decodeResult == 0) {
        pd->system->logToConsole("Failed to decode level JSON '%s'", path);
        return false;
    }
    if (outCircles.empty()) {
        pd->system->logToConsole("Level JSON '%s' decoded but contained no circles", path);
        return false;
    }

    pd->system->logToConsole("Loaded leve '%s' with %d circles", path, static_cast<int>(outCircles.size()));
    return true;
}