#pragma once

#include "rapidjson-macros/shared/macros.hpp"
#include <string>
#include "UnityEngine/Sprite.hpp"
#include "UnityEngine/Texture2D.hpp"
#include "UnityEngine/Object.hpp"
#include "main.hpp"

// making this a rapidjson-macros class for when it'll eventually be data pulled from the api
namespace BedroomPartyLB::Models
{
    DECLARE_JSON_CLASS(SeasonData, 
        NAMED_VALUE_DEFAULT(int, seasonNumber, 0, "seasonNumber");
        NAMED_VALUE_DEFAULT(std::string, seasonDescription, "", "seasonDescription");
        NAMED_VALUE_DEFAULT(std::string, seasonImageLink, "", "seasonImage");
        NAMED_VALUE_DEFAULT(int, seasonRank, -1, "seasonRank");
        NAMED_VALUE_DEFAULT(float, seasonPP, -1, "seasonPP");

        public:
        UnityEngine::Sprite* seasonImageSprite;

        SeasonData() = default;
        SeasonData(int num, std::string desc, int rank, int pp, std::string link) : seasonNumber(num), seasonDescription(desc), seasonRank(rank), seasonPP(pp), seasonImageLink(link){}
        SeasonData(int num, std::string desc, int rank, int pp, UnityEngine::Sprite* sprite) : seasonNumber(num), seasonDescription(desc), seasonRank(rank), seasonPP(pp), seasonImageSprite(sprite){}
        
        ~SeasonData()
        {
            getLogger().info("destroying season data");
            if (seasonImageSprite == nullptr || seasonImageSprite->m_CachedPtr.m_value == nullptr) return;
            if (seasonImageSprite->get_texture() == nullptr || seasonImageSprite->get_texture()->m_CachedPtr.m_value == nullptr) 
            {
                UnityEngine::Object::Destroy(seasonImageSprite); return;
            }
            UnityEngine::Object::Destroy(seasonImageSprite->get_texture());
            UnityEngine::Object::Destroy(seasonImageSprite);
        }
    )
}