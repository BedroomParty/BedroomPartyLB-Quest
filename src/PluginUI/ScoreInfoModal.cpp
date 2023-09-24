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
        UnityEngine::Application::OpenURL(Constants::USER_PROFILE_LINK + currentEntry.userID);
    }

    void ScoreInfoModal::OnInfoButtonClick()
    {
        isMoreInfo = !isMoreInfo;
        moreInfoButton->get_gameObject()->set_active(!isMoreInfo);
        backInfoButton->get_gameObject()->set_active(isMoreInfo);
        moreModalInfo->set_active(isMoreInfo);
        mainModalInfo->set_active(!isMoreInfo);
    }

    void ScoreInfoModal::Close(){
        if (!scoreInfo || !scoreInfo->m_CachedPtr.m_value) return;
        isMoreInfo = true;
        OnInfoButtonClick();
        scoreInfo->Hide(false, nullptr);
    }

    void ScoreInfoModal::setScoreModalText(Models::BPLeaderboardEntry entry, int index)
    {
        isMoreInfo = true;
        OnInfoButtonClick();

        currentEntry = entry;
        profileImageLoading->get_gameObject()->set_active(true);

        std::string time = TimeUtils::GetRelativeTimeString(entry.timeSet);
        dateScoreText->SetText(string_format("<size=4.8><color=white>%s</color></size>", time.c_str()));

        std::string username = !entry.username.empty() ? entry.username : "<color=red>Error</color>";
        if (entry.userID == "76561199077754911") username = "<color=#6488ea>" + StringUtils::banishToOz(entry.username) + "</color>";
        usernameScoreText->SetText(string_format("<size=180%%>%s</color>", username.c_str()));
        usernameScoreText->set_richText(true);
        
        std::string normalAcc = string_format("Accuracy: <size=%f><color=#ffd42a>%s%%</color></size>", infoFontSize, StringUtils::format_float(entry.accuracy).c_str());
        std::string fcAcc = string_format("FC Accuracy: <size=%f><color=#ffd42a>%s%%</color></size>", infoFontSize, StringUtils::format_float(entry.fcAcc).c_str());
        TextHoverEffect::AddEffect(accScoreText, TMPro::FontStyles::Normal, TMPro::FontStyles::Normal, normalAcc, fcAcc);

        std::string scoreText = string_format("Score: <size=%f>%i</size>", infoFontSize, entry.modifiedScore);
        std::replace(scoreText.begin(), scoreText.end(), ',', ' ');
        scoreScoreText->SetText(scoreText);

        modifiersScoreText->SetText(string_format("Mods: <size=4.4>%s</size>", entry.modifiers.c_str()));

        ppScoreText->get_gameObject()->set_active(false);

        modifiersScoreText->get_gameObject()->set_active(!entry.modifiers.empty());

        UnityEngine::Object::Destroy(usernameScoreText->get_gameObject()->GetComponent<RainbowAnimation*>());
        leaderboard.get_bedroomPartyStaffAsync([entry, this](std::vector<std::string> staff)
        {
            if (!entry.username.empty() && std::find(staff.begin(), staff.end(), entry.userID) != staff.end())
            {
                usernameScoreText->get_gameObject()->AddComponent<RainbowAnimation*>();
            } 
        });

        fcScoreText->SetText(entry.fullCombo
            ? "<size=4><color=#43e03a>Full Combo!</color></size>"
            : string_format("Mistakes: <size=4><color=#f0584a>%i</color></size>", entry.badCuts + entry.misses));

        pauses->SetText(string_format("Pauses: <size=%f><color=#ffd42a>%i</size>", infoFontSize, entry.pauses));
        perfectStreak->SetText(string_format("Perfect Streak: <size=%f><color=#ffd42a>%i</size>", infoFontSize, entry.perfectStreak));


        std::string accLeftNum = string_format("Left Hand Acc: <size=%f><color=#ffd42a>%s", infoFontSize, StringUtils::format_float(entry.accL).c_str());
        std::string accLeftPerc = string_format("Left Hand Acc: <size=%f><color=#ffd42a>%s%%", infoFontSize, StringUtils::format_float(float(entry.accL)/115*100).c_str());
        TextHoverEffect::AddEffect(avgHandAccLeft, TMPro::FontStyles::Normal, TMPro::FontStyles::Normal, accLeftNum, accLeftPerc);

        std::string accRightNum = string_format("Right Hand Acc: <size=%f><color=#ffd42a>%s", infoFontSize, StringUtils::format_float(entry.accR).c_str());
        std::string accRightPerc = string_format("Right Hand Acc: <size=%f><color=#ffd42a>%s%%", infoFontSize, StringUtils::format_float(float(entry.accR)/115*100).c_str());
        TextHoverEffect::AddEffect(avgHandAccRight, TMPro::FontStyles::Normal, TMPro::FontStyles::Normal, accRightNum, accRightPerc);
        
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