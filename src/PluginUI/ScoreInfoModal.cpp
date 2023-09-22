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
    using namespace TMPro;
    void ScoreInfoModal::PostParse()
    {
        TextHoverEffect::AddEffect(usernameScoreText, TMPro::FontStyles::Underline, TMPro::FontStyles::Normal);
        isMoreInfo = false;
    }

    void ScoreInfoModal::OnUserNameTextClick()
    {
        UnityEngine::Application::OpenURL(Constants::USER_PROFILE_LINK + currentEntry.userID.value_or(""));
    }

    void ScoreInfoModal::OnInfoButtonClick()
    {
        isMoreInfo = !isMoreInfo;
        moreInfoButton->GetComponentInChildren<TextMeshProUGUI*>()->SetText(isMoreInfo ? "Back" : "More Info");
        moreModalInfo->set_active(isMoreInfo);
        mainModalInfo->set_active(!isMoreInfo);
    }

    void ScoreInfoModal::setScoreModalText(Models::BPLeaderboardEntry entry, int index)
    {
        currentEntry = entry;
        profileImageLoading->get_gameObject()->set_active(true);

        std::string time = TimeUtils::GetRelativeTimeString(entry.timeSet);
        dateScoreText->SetText(string_format("<size=4.8><color=white>%s</color></size>", time.c_str()));

        std::string username = !entry.username.empty() ? entry.username : "<color=red>Error</color>";
        usernameScoreText->SetText(string_format("<size=180%%>%s</color>", username.c_str()));
        usernameScoreText->set_richText(true);
        accScoreText->SetText(string_format("Accuracy: <size=%f><color=#ffd42a>%s%%</color></size>", infoFontSize, StringUtils::format_float(entry.accuracy).c_str()));

        std::string scoreText = string_format("Score: <size=%f>%i</size>", infoFontSize, entry.modifiedScore);
        std::replace(scoreText.begin(), scoreText.end(), ',', ' ');
        scoreScoreText->SetText(scoreText);

        modifiersScoreText->SetText(string_format("Mods: <size=4.4>%s</size>", entry.modifiers.c_str()));

        ppScoreText->get_gameObject()->set_active(false);

        modifiersScoreText->get_gameObject()->set_active(!entry.modifiers.empty());

        UnityEngine::Object::Destroy(usernameScoreText->get_gameObject()->GetComponent<RainbowAnimation*>());
        leaderboard.get_bedroomPartyStaffAsync([entry, this](std::vector<std::string> staff)
        {
            if (!entry.username.empty() && entry.userID.has_value() && std::find(staff.begin(), staff.end(), entry.userID.value()) != staff.end())
            {
                usernameScoreText->get_gameObject()->AddComponent<RainbowAnimation*>();
            } 
        });

        fcScoreText->SetText(entry.fullCombo
            ? "<size=4><color=#43e03a>Full Combo!</color></size>"
            : string_format("Mistakes: <size=4><color=#f0584a>%i</color></size>", entry.badCuts + entry.misses));

        pauses->SetText(string_format("Pauses: <size=%f>%i</size>", infoFontSize, entry.pauses));
        perfectStreak->SetText(string_format("Perfect Streak: <size=%f><color=#ffd42a>%i</size>", infoFontSize, entry.perfectStreak));
        // avgHandAccLeft->SetText(string_format("Left Hand Acc: <size=%f><color=#ffd42a>%s</color> (<color=#ffd42a>%s%%</color>)</size>", infoFontSize, StringUtils::format_float(entry.accL).c_str(), StringUtils::format_float(float(entry.accL)/115*100).c_str()));
        // avgHandAccRight->SetText(string_format("Right Hand Acc: <size=%f><color=#ffd42a>%s</color> (<color=#ffd42a>%s%%</color>)</size>", infoFontSize, StringUtils::format_float(entry.accR).c_str(), StringUtils::format_float(float(entry.accR)/115*100).c_str()));
        avgHandAccLeft->SetText(string_format("Left Hand TD: <size=%f><color=#ffd42a>%s</size>", infoFontSize, StringUtils::format_float(entry.accL).c_str()));
        avgHandAccRight->SetText(string_format("Right Hand TD: <size=%f><color=#ffd42a>%s</size>", infoFontSize, StringUtils::format_float(entry.accR).c_str()));
        
        avgHandTDLeft->SetText(string_format("Left Hand TD: <size=%f><color=#ffd42a>%s</size>", infoFontSize, StringUtils::format_float(entry.tdL).c_str()));
        avgHandTDRight->SetText(string_format("Right Hand TD: <size=%f><color=#ffd42a>%s</size>", infoFontSize, StringUtils::format_float(entry.tdR).c_str()));

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