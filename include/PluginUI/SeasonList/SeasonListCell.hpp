#pragma once

#include "HMUI/ImageView.hpp"
#include "HMUI/TableCell.hpp"
#include "PluginUI/SeasonList/SeasonListData.hpp"
#include "TMPro/TextMeshProUGUI.hpp"
#include "custom-types/shared/macros.hpp"

#include "bsml/shared/BSML/Components/Backgroundable.hpp"
#include "UnityEngine/UI/Button.hpp"
#include "Models/SeasonData.hpp"

#ifndef DECLARE_OVERRIDE_METHOD_MATCH
#define DECLARE_OVERRIDE_METHOD_MATCH(retval, method, mptr, ...) \
    DECLARE_OVERRIDE_METHOD(retval, method, il2cpp_utils::il2cpp_type_check::MetadataGetter<mptr>::get(), __VA_ARGS__)
#endif

DECLARE_CLASS_CODEGEN(BedroomPartyLB::UI, SeasonListCell, HMUI::TableCell,

                      DECLARE_INSTANCE_FIELD(TMPro::TextMeshProUGUI*, seasonNumberText);
                      DECLARE_INSTANCE_FIELD(TMPro::TextMeshProUGUI*, seasonDescriptionText);
                      DECLARE_INSTANCE_FIELD(TMPro::TextMeshProUGUI*, rankText);
                      DECLARE_INSTANCE_FIELD(TMPro::TextMeshProUGUI*, ppText);
                      DECLARE_INSTANCE_FIELD(HMUI::ImageView*, seasonImage);
                      DECLARE_INSTANCE_FIELD(BSML::Backgroundable*, backgroundImage);

                      DECLARE_INSTANCE_FIELD(BedroomPartyLB::UI::SeasonListData*, tableData);

                      DECLARE_INSTANCE_METHOD(void, PostParse);
                      DECLARE_INSTANCE_METHOD(void, SeasonPlaylistClicked);
                      DECLARE_INSTANCE_METHOD(void, SeasonLeaderboardClicked);

                      DECLARE_INSTANCE_METHOD(void, FixedUpdate);
                      DECLARE_OVERRIDE_METHOD_MATCH(void, HighlightDidChange, &HMUI::SelectableCell::HighlightDidChange, HMUI::SelectableCell::TransitionType transitionType);

                      public:

                      static BedroomPartyLB::UI::SeasonListCell* CreateNewCell();

                      void set_seasonNumber(int num);
                      void set_seasonDescription(std::string_view desc);
                      void set_seasonRank(int rank);
                      void set_seasonPP(float pp);
                      void set_sprite(UnityEngine::Sprite* preview);
                      void Populate(BedroomPartyLB::Models::SeasonData& user);
                      bool downloadInProgress;
                      BedroomPartyLB::Models::SeasonData* _data;
)