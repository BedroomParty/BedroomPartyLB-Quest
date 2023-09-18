#include "PluginUI/ScoreInfoModal.hpp"
#include "main.hpp"
#include "Models/CustomLeaderboard.hpp"
#include "lapiz/shared/utilities/MainThreadScheduler.hpp"
#include "UnityEngine/Sprite.hpp"
#include "Utils/TimeUtils.hpp"
#include "UnityEngine/Application.hpp"
#include "Utils/Constants.hpp"
#include "PluginUI/TextHoverEffect.hpp"
#include "PluginUI/RainbowAnimation.hpp"

DEFINE_TYPE(BedroomPartyLB::UI, ScoreInfoModal);

namespace BedroomPartyLB::UI
{
    void ScoreInfoModal::PostParse()
    {
        TextHoverEffect::AddEffect(usernameScoreText, TMPro::FontStyles::Underline, TMPro::FontStyles::Normal);
    }

    void ScoreInfoModal::OnUserNameTextClick()
    {
        UnityEngine::Application::OpenURL(Constants::USER_PROFILE_LINK + currentEntry.userID.value_or(""));
    }

    void ScoreInfoModal::setScoreModalText(Models::BPLeaderboardEntry entry, int index)
    {
        getLogger().info("index: %i", index);
        currentEntry = entry;
        profileImageLoading->get_gameObject()->set_active(true);

        std::string time = TimeUtils::GetRelativeTimeString(entry.timeSet);
        dateScoreText->SetText(string_format("<size=4.8><color=white>%s</color></size>", time.c_str()));

        usernameScoreText->SetText(string_format("<size=180%%>%s</color>", entry.username.c_str()));
        usernameScoreText->set_richText(true);
        accScoreText->SetText(string_format("Accuracy: <size=%f><color=#ffd42a>%.2f%%</color></size>", infoFontSize, entry.accuracy));

        std::string scoreText = string_format("Score: <size=%f>%i</size>", infoFontSize, entry.modifiedScore);
        std::replace(scoreText.begin(), scoreText.end(), ',', ' ');
        scoreScoreText->SetText(scoreText);

        modifiersScoreText->SetText(string_format("Mods: <size=4.4>%s</size>", entry.modifiers.c_str()));

        ppScoreText->get_gameObject()->set_active(false);

        modifiersScoreText->get_gameObject()->set_active(!entry.modifiers.empty());

        UnityEngine::Object::Destroy(usernameScoreText->get_gameObject()->GetComponent<RainbowAnimation*>());
        leaderboard.get_bedroomPartyStaffAsync([entry, this](std::vector<std::string> staff)
        {
            if (std::find(staff.begin(), staff.end(), entry.userID) != staff.end())
            {
                usernameScoreText->get_gameObject()->AddComponent<RainbowAnimation*>();
            } 
        });

        fcScoreText->SetText(entry.fullCombo
                                 ? "<size=4><color=green>Full Combo!</color></size>"
                                 : string_format("Mistakes: <size=4><color=red>%i</color></size>", entry.badCuts + entry.misses));

        auto img = leaderboard.get_leaderboardViewController()->playerAvatars[index];
        auto load = leaderboard.get_leaderboardViewController()->avatarLoadings[index];
        profileImageLoading->set_active(true);
        profileImage->get_gameObject()->set_active(false);
        std::thread([this, img, load]()
        {
            while(load->get_gameObject()->get_activeSelf()) std::this_thread::sleep_for(std::chrono::milliseconds(300));
            Lapiz::Utilities::MainThreadScheduler::Schedule([this, img]()
            {
                profileImage->set_sprite(img->get_sprite());
                profileImageLoading->set_active(false);
                profileImage->get_gameObject()->set_active(img->get_sprite() && img->get_sprite()->m_CachedPtr.m_value);
            }); 
        }).detach();

        scoreInfo->Show(true, true, nullptr);
    }
}