#pragma once

#include <string_view>
#include "beatsaber-hook/shared/utils/typedefs.h"

// This is a cmake generated header to include my png and bsml files

struct IncludedAsset {

    IncludedAsset(uint8_t* start, uint8_t* end) : array(reinterpret_cast<Array<uint8_t>*>(start)) {
        array->klass = nullptr;
        array->monitor = nullptr;
        array->bounds = nullptr;
        array->max_length = end - start - 33;
        *(end - 1)= '\0';
    }
    
    operator ArrayW<uint8_t>() const {
        init();
        return array;
    }

    operator std::string_view() const {
        return { reinterpret_cast<char*>(array->values), array->Length() };
    }
    
    operator std::span<uint8_t>() const {
        return { array->values, array->Length() };
    }

    void init() const {
        if(!array->klass)
            array->klass = classof(Array<uint8_t>*);
    }

    private:
        Array<uint8_t>* array;

};

#define DECLARE_FILE(name)                         \
    extern "C" uint8_t _binary_##name##_start[];  \
    extern "C" uint8_t _binary_##name##_end[];    \
    const IncludedAsset name { _binary_##name##_start, _binary_##name##_end};

namespace IncludedAssets {

	DECLARE_FILE(Arrow_png)
	DECLARE_FILE(Blank_png)
	DECLARE_FILE(Bug_png)
	DECLARE_FILE(Globe_png)
	DECLARE_FILE(Info_png)
	DECLARE_FILE(LeaderboardView_bsml)
	DECLARE_FILE(Link_png)
	DECLARE_FILE(Lmao_png)
	DECLARE_FILE(Logo_png)
	DECLARE_FILE(Noti_png)
	DECLARE_FILE(PanelView_bsml)
	DECLARE_FILE(Pixel_png)
	DECLARE_FILE(Player_png)
	DECLARE_FILE(SeasonListCell_bsml)
	DECLARE_FILE(infoModal_bsml)
	DECLARE_FILE(pp_png)
	DECLARE_FILE(seasonSelectModal_bsml)

}
