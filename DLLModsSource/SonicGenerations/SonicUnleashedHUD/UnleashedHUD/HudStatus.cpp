boost::shared_ptr<Sonic::CGameObject> status;

bool isWerehog = false;

static SharedPtrTypeless wooshHandle;
static SharedPtrTypeless selectHandle;
static SharedPtrTypeless switchHandle;

int currentStatIndex = 0;
int statusIndex = 0;

const char* intro() {
    return isWerehog ? "Intro_ev_Anim" : "Intro_so_Anim";
}

const char* select() {
    return isWerehog ? "select_ev_Anim" : "select_so_Anim";
}

const char* outro() {
    return isWerehog ? "Outro_ev_Anim" : "Outro_so_Anim";
}

const char* intro2() {
    return isWerehog ? "Intro_ev_Anim_2" : "Intro_so_Anim_2";
}

struct Stat {
    const char* name = "";
    int txt_num = 0;
    int level = 0;
    int  maxLevel = 0;
    int count = 0;
    float levelProgress = 0.0f;

    Chao::CSD::RCPtr<Chao::CSD::CScene> m_tag_bg_2;
    Chao::CSD::RCPtr<Chao::CSD::CScene> m_tag_txt_2;
    Chao::CSD::RCPtr<Chao::CSD::CScene> m_prgs_bg_2;
    Chao::CSD::RCPtr<Chao::CSD::CScene> m_prgs_bar_2;
    Chao::CSD::RCPtr<Chao::CSD::CScene> m_prgs_num_2;

    const char* TextNumIntToString() {
        switch (txt_num)
        {
        case 1:
            return "tag_txt_2";
            break;
        case 2:
            return "tag_txt_3";
            break;
        case 3:
            return "tag_txt_4";
            break;
        case 4:
            return "tag_txt_5";
            break;
        case 5:
            return "tag_txt_6";
            break;
        default:
            return "tag_txt_2";
            break;
        }
    }
};

// const char* _name, int _level, int _maxLevel, int _txt_num, int _count
Stat speed = { "speed", 9, 11, 1, 10 };
Stat ring_energy = { "ring_energy", 6, 6, 2, 0 };

Stat combat = { "combat", 0, 31, 1, 0 };
Stat strength = { "strength", 0, 11, 2, 0 };
Stat life = { "life", 0, 11, 3, 0 };
Stat unleash = { "unleash", 0, 11, 4, 0 };
Stat shield = { "shield", 0, 11, 5, 0 };

std::vector<Stat> statsDay = { speed, ring_energy };
std::vector<Stat> statsNight = { combat, strength, life, unleash, shield };

std::vector<Stat>& CurrentStats() {
    return isWerehog ? statsNight : statsDay;
}

Stat& CurrentStat() {
    return CurrentStats()[currentStatIndex];
}

int CurrentStatSizeMinusOne() {
    return CurrentStats().size() - 1;
}

class CHudStatus : public Sonic::CGameObject
{
    Chao::CSD::RCPtr<Chao::CSD::CProject> m_rcStatus;
    boost::shared_ptr<Sonic::CGameObjectCSD> m_spStatus;

    Chao::CSD::RCPtr<Chao::CSD::CScene> m_status_title;
    Chao::CSD::RCPtr<Chao::CSD::CScene> m_logo;

    Chao::CSD::RCPtr<Chao::CSD::CScene> m_medal_info;
    Chao::CSD::RCPtr<Chao::CSD::CScene> m_medal_s_gauge;
    Chao::CSD::RCPtr<Chao::CSD::CScene> m_medal_m_gauge;

    Chao::CSD::RCPtr<Chao::CSD::CScene> m_tag_bg_1;
    Chao::CSD::RCPtr<Chao::CSD::CScene> m_tag_txt_1;

    Chao::CSD::RCPtr<Chao::CSD::CScene> m_prgs_bg_1;
    Chao::CSD::RCPtr<Chao::CSD::CScene> m_prgs_bar_1;
    Chao::CSD::RCPtr<Chao::CSD::CScene> m_prgs_num_1;


    Chao::CSD::RCPtr<Chao::CSD::CScene> m_decide_bg;

    Chao::CSD::RCPtr<Chao::CSD::CScene> m_status_footer;

    float timeFooter = 0.0f;
    bool footerVisible = false;

    float statDelay = 0.17f;
    float y = 0.0f;

    float statDelayStart = 0.0f;

    float switchCooldown = 0.0f;

    float EndDelay;

    bool exit = false;

    float selectDelay = -1;

    float quitSoundTimer = 0.0f;

    float medalSubImage = 0.0f;

public:

    void HudRemove() {
        RemoveStats();

        Chao::CSD::CProject::DestroyScene(m_rcStatus.Get(), m_status_title);

        Chao::CSD::CProject::DestroyScene(m_rcStatus.Get(), m_logo);
        Chao::CSD::CProject::DestroyScene(m_rcStatus.Get(), m_medal_info);

        Chao::CSD::CProject::DestroyScene(m_rcStatus.Get(), m_medal_s_gauge);

        Chao::CSD::CProject::DestroyScene(m_rcStatus.Get(), m_medal_m_gauge);

        Chao::CSD::CProject::DestroyScene(m_rcStatus.Get(), m_tag_bg_1);

        Chao::CSD::CProject::DestroyScene(m_rcStatus.Get(), m_tag_txt_1);

        Chao::CSD::CProject::DestroyScene(m_rcStatus.Get(), m_prgs_bg_1);

        Chao::CSD::CProject::DestroyScene(m_rcStatus.Get(), m_prgs_bar_1);

        Chao::CSD::CProject::DestroyScene(m_rcStatus.Get(), m_prgs_num_1);

        Chao::CSD::CProject::DestroyScene(m_rcStatus.Get(), m_status_footer);

        Chao::CSD::CProject::DestroyScene(m_rcStatus.Get(), m_decide_bg);

        if (m_spStatus) {
            m_spStatus->SendMessage(m_spStatus->m_ActorID, boost::make_shared<Sonic::Message::MsgKill>());
            m_spStatus = nullptr;
        }

        m_rcStatus = nullptr;
    }

    void RemoveStats() {
        for (int i = 0; i < statsDay.size(); i++)
        {
            Stat& s = statsDay[i];
            Chao::CSD::CProject::DestroyScene(m_rcStatus.Get(), s.m_tag_bg_2);
            Chao::CSD::CProject::DestroyScene(m_rcStatus.Get(), s.m_tag_txt_2);
            Chao::CSD::CProject::DestroyScene(m_rcStatus.Get(), s.m_prgs_bg_2);
            Chao::CSD::CProject::DestroyScene(m_rcStatus.Get(), s.m_prgs_bar_2);
            Chao::CSD::CProject::DestroyScene(m_rcStatus.Get(), s.m_prgs_num_2);
        }

        for (int i = 0; i < statsNight.size(); i++)
        {
            Stat& s = statsNight[i];
            Chao::CSD::CProject::DestroyScene(m_rcStatus.Get(), s.m_tag_bg_2);
            Chao::CSD::CProject::DestroyScene(m_rcStatus.Get(), s.m_tag_txt_2);
            Chao::CSD::CProject::DestroyScene(m_rcStatus.Get(), s.m_prgs_bg_2);
            Chao::CSD::CProject::DestroyScene(m_rcStatus.Get(), s.m_prgs_bar_2);
            Chao::CSD::CProject::DestroyScene(m_rcStatus.Get(), s.m_prgs_num_2);
        }
    }

    void ToggleStats() {
        isWerehog = !isWerehog;

        //Exp Bar
        m_tag_bg_1->SetHideFlag(false);
        CSDCommon::PlayAnimation(*m_tag_bg_1, intro(), Chao::CSD::eMotionRepeatType_PlayOnce, 1.0, 0.0);

        m_tag_txt_1->SetHideFlag(false);
        CSDCommon::PlayAnimation(*m_tag_txt_1, intro(), Chao::CSD::eMotionRepeatType_PlayOnce, 1.0, 0.0);

        m_prgs_bg_1->SetHideFlag(false);
        CSDCommon::PlayAnimation(*m_prgs_bg_1, intro(), Chao::CSD::eMotionRepeatType_PlayOnce, 1.0, 0.0);

        m_prgs_bar_1->SetHideFlag(false);
        CSDCommon::PlayAnimation(*m_prgs_bar_1, intro(), Chao::CSD::eMotionRepeatType_PlayOnce, 1.0, 0.0);

        m_prgs_num_1->SetHideFlag(false);
        CSDCommon::PlayAnimation(*m_prgs_num_1, intro(), Chao::CSD::eMotionRepeatType_PlayOnce, 1.0, 0.0);

        m_decide_bg->SetHideFlag(false);
        CSDCommon::PlayAnimation(*m_decide_bg, intro(), Chao::CSD::eMotionRepeatType_PlayOnce, 0.0, 100.0, 100.0);

        m_status_title->SetHideFlag(false);
        CSDCommon::PlayAnimation(*m_status_title, intro(), Chao::CSD::eMotionRepeatType_PlayOnce, 0.0, m_status_title->m_MotionEndFrame);

        m_logo->SetHideFlag(false);
        CSDCommon::PlayAnimation(*m_logo, "Switch_Anim", Chao::CSD::eMotionRepeatType_PlayOnce, 1.0, 0, 20.0, false, !isWerehog);

        currentStatIndex = 0;
        statusIndex = 0;
        y = 0.0f;

        RemoveStats();
        AddAllStats();
    }

    void Select(int index, bool up, bool reverse, bool quit = false) {

        if (quit) {
            CSDCommon::PlayAnimation(*m_decide_bg, select(), Chao::CSD::eMotionRepeatType_PlayOnce, 1, 0);
            CSDCommon::PlayAnimation(*CurrentStats()[index - 1].m_tag_bg_2, select(), Chao::CSD::eMotionRepeatType_PlayOnce, 1, 0, 0, false, true);
            CSDCommon::PlayAnimation(*CurrentStats()[index - 1].m_prgs_bg_2, select(), Chao::CSD::eMotionRepeatType_PlayOnce, 1, 0, 0, false, true);
            CSDCommon::PlayAnimation(*CurrentStats()[index - 1].m_prgs_num_2, select(), Chao::CSD::eMotionRepeatType_PlayOnce, 1, 0, 0, false, true);
            CSDCommon::PlayAnimation(*CurrentStats()[index - 1].m_prgs_bar_2, select(), Chao::CSD::eMotionRepeatType_PlayOnce, 1, 0, 0, false, true);
        }
        else if(!quit && !reverse && !up) {
            CSDCommon::PlayAnimation(*CurrentStats()[index].m_tag_bg_2, select(), Chao::CSD::eMotionRepeatType_PlayOnce, 1, 0);
            CSDCommon::PlayAnimation(*CurrentStats()[index].m_prgs_bg_2, select(), Chao::CSD::eMotionRepeatType_PlayOnce, 1, 0);
            CSDCommon::PlayAnimation(*CurrentStats()[index].m_prgs_num_2, select(), Chao::CSD::eMotionRepeatType_PlayOnce, 1, 0);
            CSDCommon::PlayAnimation(*CurrentStats()[index].m_prgs_bar_2, select(), Chao::CSD::eMotionRepeatType_PlayOnce, 1, 0);
        }

        if (!quit && reverse && up && index != CurrentStatSizeMinusOne()) {
            CSDCommon::PlayAnimation(*CurrentStats()[std::clamp(index + 1, 0, CurrentStatSizeMinusOne())].m_tag_bg_2, select(), Chao::CSD::eMotionRepeatType_PlayOnce, 1, 0, 0, false, true);
            CSDCommon::PlayAnimation(*CurrentStats()[std::clamp(index + 1, 0, CurrentStatSizeMinusOne())].m_prgs_bg_2, select(), Chao::CSD::eMotionRepeatType_PlayOnce, 1, 0, 0, false, true);
            CSDCommon::PlayAnimation(*CurrentStats()[std::clamp(index + 1, 0, CurrentStatSizeMinusOne())].m_prgs_num_2, select(), Chao::CSD::eMotionRepeatType_PlayOnce, 1, 0, 0, false, true);
            CSDCommon::PlayAnimation(*CurrentStats()[std::clamp(index + 1, 0, CurrentStatSizeMinusOne())].m_prgs_bar_2, select(), Chao::CSD::eMotionRepeatType_PlayOnce, 1, 0, 0, false, true);
        }
        else if (!quit && reverse && up && index == CurrentStatSizeMinusOne())
            CSDCommon::PlayAnimation(*m_decide_bg, select(), Chao::CSD::eMotionRepeatType_PlayOnce, 1, 0, 0, false, true);
        else if (!quit && reverse && !up) {
            CSDCommon::PlayAnimation(*CurrentStats()[std::clamp(index - 1, 0, CurrentStatSizeMinusOne())].m_tag_bg_2, select(), Chao::CSD::eMotionRepeatType_PlayOnce, 1, 0, 0, false, true);
            CSDCommon::PlayAnimation(*CurrentStats()[std::clamp(index - 1, 0, CurrentStatSizeMinusOne())].m_prgs_bg_2, select(), Chao::CSD::eMotionRepeatType_PlayOnce, 1, 0, 0, false, true);
            CSDCommon::PlayAnimation(*CurrentStats()[std::clamp(index - 1, 0, CurrentStatSizeMinusOne())].m_prgs_num_2, select(), Chao::CSD::eMotionRepeatType_PlayOnce, 1, 0, 0, false, true);
            CSDCommon::PlayAnimation(*CurrentStats()[std::clamp(index - 1, 0, CurrentStatSizeMinusOne())].m_prgs_bar_2, select(), Chao::CSD::eMotionRepeatType_PlayOnce, 1, 0, 0, false, true);
        }
    }

    void AddStatCommon() {
        currentStatIndex = std::clamp(currentStatIndex, 0, CurrentStatSizeMinusOne());

        CurrentStat().m_tag_bg_2 = m_rcStatus->CreateScene("tag_bg_2");
        CurrentStat().m_tag_bg_2->SetHideFlag(false);
        CurrentStat().m_tag_bg_2->SetPosition(0.0f, y);
        CSDCommon::PlayAnimation(*CurrentStat().m_tag_bg_2, intro2(), Chao::CSD::eMotionRepeatType_PlayOnce, 1.0, 0.0);

        CurrentStat().m_tag_txt_2 = m_rcStatus->CreateScene(CurrentStat().TextNumIntToString());
        CurrentStat().m_tag_txt_2->SetHideFlag(false);
        CSDCommon::PlayAnimation(*CurrentStat().m_tag_txt_2, (CurrentStat().txt_num < 3) ? intro2() : "Intro_ev_Anim_2", Chao::CSD::eMotionRepeatType_PlayOnce, 1.0, 0.0);

        CurrentStat().m_prgs_bg_2 = m_rcStatus->CreateScene("prgs_bg_2");
        CurrentStat().m_prgs_bg_2->SetHideFlag(false);
        CurrentStat().m_prgs_bg_2->SetPosition(0.0f, y);
        CSDCommon::PlayAnimation(*CurrentStat().m_prgs_bg_2, intro2(), Chao::CSD::eMotionRepeatType_PlayOnce, 1.0, 0.0);

        CurrentStat().m_prgs_num_2 = m_rcStatus->CreateScene("prgs_num_2");
        CurrentStat().m_prgs_num_2->SetHideFlag(false);
        CurrentStat().m_prgs_num_2->SetPosition(0.0f, y);

        if (CurrentStat().level >= CurrentStat().maxLevel) {
            CurrentStat().m_prgs_num_2->GetNode("num")->SetHideFlag(1);
            CurrentStat().m_prgs_num_2->GetNode("img")->SetHideFlag(1);
            CurrentStat().m_prgs_num_2->GetNode("txt")->SetHideFlag(0);
        }
        else
            CurrentStat().m_prgs_num_2->GetNode("num")->SetText(std::to_string(CurrentStat().level).c_str());

        if (CurrentStat().count == 0) {
            CurrentStat().m_prgs_num_2->GetNode("num_2")->SetHideFlag(1);
            CurrentStat().m_prgs_num_2->GetNode("img_2")->SetHideFlag(1);
        }
        else
            CurrentStat().m_prgs_num_2->GetNode("num_2")->SetText(std::to_string(CurrentStat().count).c_str());

        CSDCommon::PlayAnimation(*CurrentStat().m_prgs_num_2, intro2(), Chao::CSD::eMotionRepeatType_PlayOnce, 1.0, 0.0);

        CurrentStat().m_prgs_bar_2 = m_rcStatus->CreateScene("prgs_bar_2");
        CurrentStat().m_prgs_bar_2->SetHideFlag(false);
        CurrentStat().m_prgs_bar_2->SetPosition(0.0f, y);
        CSDCommon::PlayAnimation(*CurrentStat().m_prgs_bar_2, intro2(), Chao::CSD::eMotionRepeatType_PlayOnce, 1.0, 0.0);
        y += (isWerehog ? 64.0f : 73.0f);
    }

    void AddStat(bool woosh = true) {
        if (currentStatIndex >= CurrentStats().size())
            return;

        AddStatCommon();

        if (woosh) 
            Common::PlaySoundStatic(wooshHandle, 1000029);
    }

    void AddAllStats() {
        selectDelay = 0.65f;
        for (int i = 0; i < CurrentStats().size(); i++)
        {
            currentStatIndex = i;
            AddStatCommon();
        }
    }

    void Outro() {
        // Quit Button
        CSDCommon::PlayAnimation(*m_decide_bg, intro(), Chao::CSD::eMotionRepeatType_PlayOnce, 1.0, 0.0, 0.0, false, true);

        // Header
        CSDCommon::PlayAnimation(*m_status_title, outro(), Chao::CSD::eMotionRepeatType_PlayOnce, 1.0, 0.0);

        // Character Portrait
        CSDCommon::PlayAnimation(*m_logo, intro(), Chao::CSD::eMotionRepeatType_PlayOnce, 1.0, 0.0, 0.0, false, true);

        // Bottom Buttons
        m_status_footer->SetHideFlag(true);

        //Medals (Top Right)
        CSDCommon::PlayAnimation(*m_medal_info, "Intro_Anim", Chao::CSD::eMotionRepeatType_PlayOnce, 1.0, 0.0, 0.0, false, true);

        CSDCommon::PlayAnimation(*m_medal_s_gauge, "Intro_Anim", Chao::CSD::eMotionRepeatType_PlayOnce, 1.0, 0.0, 0.0, false, true);

        CSDCommon::PlayAnimation(*m_medal_m_gauge, "Intro_Anim", Chao::CSD::eMotionRepeatType_PlayOnce, 1.0, 0.0, 0.0, false, true);

        currentStatIndex = std::clamp(currentStatIndex, 0,  CurrentStatSizeMinusOne());

        for (int i = 0; i < CurrentStats().size(); i++)
        {
            Stat& s = CurrentStats()[i];
            CSDCommon::PlayAnimation(*s.m_tag_bg_2, intro2(), Chao::CSD::eMotionRepeatType_PlayOnce, 1.0, 0.0, 0.0, false, true);
            CSDCommon::PlayAnimation(*s.m_tag_txt_2, (CurrentStat().txt_num < 3) ? intro2() : "Intro_ev_Anim_2", Chao::CSD::eMotionRepeatType_PlayOnce, 1.0, 0.0, 0.0, false, true);
            CSDCommon::PlayAnimation(*s.m_prgs_num_2, intro2(), Chao::CSD::eMotionRepeatType_PlayOnce, 1.0, 0.0, 0.0, false, true);
            CSDCommon::PlayAnimation(*s.m_prgs_bg_2, intro2(), Chao::CSD::eMotionRepeatType_PlayOnce, 1.0, 0.0, 0.0, false, true);
            CSDCommon::PlayAnimation(*s.m_prgs_bar_2, intro2(), Chao::CSD::eMotionRepeatType_PlayOnce, 1.0, 0.0, 0.0, false, true);
        }

        // Exp Bar
        CSDCommon::PlayAnimation(*m_tag_bg_1, intro(), Chao::CSD::eMotionRepeatType_PlayOnce, 1.0, 0.0, 0.0, false, true);

        CSDCommon::PlayAnimation(*m_tag_txt_1, intro(), Chao::CSD::eMotionRepeatType_PlayOnce, 1.0, 0.0, 0.0, false, true);

        CSDCommon::PlayAnimation(*m_prgs_bg_1, intro(), Chao::CSD::eMotionRepeatType_PlayOnce, 1.0, 0.0, 0.0, false, true);

        CSDCommon::PlayAnimation(*m_prgs_bar_1, intro(), Chao::CSD::eMotionRepeatType_PlayOnce, 1.0, 0.0, 0.0, false, true);

        CSDCommon::PlayAnimation(*m_prgs_num_1, intro(), Chao::CSD::eMotionRepeatType_PlayOnce, 1.0, 0.0, 0.0, false, true);

        EndDelay = 1.5f;
        exit = true;
    }

    void Start() {
       m_tag_bg_1->SetHideFlag(false);
       CSDCommon::PlayAnimation(*m_tag_bg_1, intro(), Chao::CSD::eMotionRepeatType_PlayOnce, 1.0, 0.0);

       m_tag_txt_1->SetHideFlag(false);
       CSDCommon::PlayAnimation(*m_tag_txt_1, intro(), Chao::CSD::eMotionRepeatType_PlayOnce, 1.0, 0.0);

       m_prgs_bg_1->SetHideFlag(false);
       CSDCommon::PlayAnimation(*m_prgs_bg_1, intro(), Chao::CSD::eMotionRepeatType_PlayOnce, 1.0, 0.0);

       m_prgs_bar_1->SetHideFlag(false);
       CSDCommon::PlayAnimation(*m_prgs_bar_1, intro(), Chao::CSD::eMotionRepeatType_PlayOnce, 1.0, 0.0);

       m_prgs_num_1->SetHideFlag(false);
       m_prgs_num_1->GetNode("num")->SetText("69");
       CSDCommon::PlayAnimation(*m_prgs_num_1, intro(), Chao::CSD::eMotionRepeatType_PlayOnce, 1.0, 0.0);

       m_decide_bg->SetHideFlag(false);
       CSDCommon::PlayAnimation(*m_decide_bg, intro(), Chao::CSD::eMotionRepeatType_PlayOnce, 1.0, 0.0);

       m_status_footer->SetHideFlag(true);
       CSDCommon::PlayAnimation(*m_status_footer, "Usual_Anim_2", Chao::CSD::eMotionRepeatType_PlayOnce, 1.0, 0.0);

       m_status_title->SetHideFlag(false);
       CSDCommon::PlayAnimation(*m_status_title, intro(), Chao::CSD::eMotionRepeatType_PlayOnce, 1.0, 0.0);

       m_logo->SetHideFlag(false);
       CSDCommon::PlayAnimation(*m_logo, intro(), Chao::CSD::eMotionRepeatType_PlayOnce, 1.0, 0.0);

       m_medal_info->SetHideFlag(false);
       CSDCommon::PlayAnimation(*m_medal_info, "Intro_Anim", Chao::CSD::eMotionRepeatType_PlayOnce, 1.0, 0.0);

       m_medal_s_gauge->SetHideFlag(false);
       CSDCommon::PlayAnimation(*m_medal_s_gauge, "Intro_Anim", Chao::CSD::eMotionRepeatType_PlayOnce, 1.0, 0.0);

       m_medal_m_gauge->SetHideFlag(false);
       CSDCommon::PlayAnimation(*m_medal_m_gauge, "Intro_Anim", Chao::CSD::eMotionRepeatType_PlayOnce, 1.0, 0.0);

       medalSubImage = 0.0f;

       timeFooter = 0.0f;
       footerVisible = false;

       statDelayStart = 0.75f;

       currentStatIndex = 0;
       y = 0.0f;

       quitSoundTimer = 0.0f;

       exit = false;
    }

    void AddCallback
    (
        const Hedgehog::Base::THolder<Sonic::CWorld>& worldHolder,
        Sonic::CGameDocument* pGameDocument,
        const boost::shared_ptr<Hedgehog::Database::CDatabase>& spDatabase
    ) override
    {
        isWerehog = false;

        Sonic::CApplicationDocument::GetInstance()->AddMessageActor("GameObject", this);
        pGameDocument->AddUpdateUnit("1", this);

        // initialize ui
        Sonic::CCsdDatabaseWrapper wrapper(m_pMember->m_pGameDocument->m_pMember->m_spDatabase.get());

        auto spCsdProject = wrapper.GetCsdProject("ui_status");
        m_rcStatus = spCsdProject->m_rcProject;

        // Exp Bar
        m_tag_bg_1 = m_rcStatus->CreateScene("tag_bg_1");

        m_tag_txt_1 = m_rcStatus->CreateScene("tag_txt_1");

        m_prgs_bg_1 = m_rcStatus->CreateScene("prgs_bg_1");

        m_prgs_bar_1 = m_rcStatus->CreateScene("prgs_bar_1");

        m_prgs_num_1 = m_rcStatus->CreateScene("prgs_num_1");

        // Quit Button
        m_decide_bg = m_rcStatus->CreateScene("decide_bg");

        // Bottom Buttons
        m_status_footer = m_rcStatus->CreateScene("status_footer");

        // Header
        m_status_title = m_rcStatus->CreateScene("status_title");

        // Character Portrait
        m_logo = m_rcStatus->CreateScene("logo");

        //Medals (Top Right)
        m_medal_info = m_rcStatus->CreateScene("medal_info");

        m_medal_s_gauge = m_rcStatus->CreateScene("medal_s_gauge");

        m_medal_m_gauge = m_rcStatus->CreateScene("medal_m_gauge");

        m_spStatus = boost::make_shared<Sonic::CGameObjectCSD>(m_rcStatus, 0.5f, "HUD_B2", false);
        Sonic::CGameDocument::GetInstance()->AddGameObject(m_spStatus, "main", this);

        Start();
    }

    void MedalUpdate(const Hedgehog::Universe::SUpdateInfo& updateInfo) {

        medalSubImage += updateInfo.DeltaTime * 25.0f;

        if (medalSubImage >= 30)
            medalSubImage = 0;

        m_medal_info->GetNode("sun")->SetPatternIndex(std::clamp(medalSubImage, 0.0f, 29.0f));
        m_medal_info->GetNode("sun_shade")->SetPatternIndex(std::clamp(medalSubImage, 0.0f, 29.0f));

        m_medal_info->GetNode("moon")->SetPatternIndex(std::clamp(medalSubImage, 0.0f, 29.0f));
        m_medal_info->GetNode("moon_shade")->SetPatternIndex(std::clamp(medalSubImage, 0.0f, 29.0f));
    }

    void ControlCharacterSwitch(const Hedgehog::Universe::SUpdateInfo& updateInfo) {
        if (selectDelay != -1 || statDelayStart != -1) 
            return;

        if (switchCooldown > 0) {
            switchCooldown -= updateInfo.DeltaTime;
            return;
        }

        Sonic::SPadState const* padState = &Sonic::CInputState::GetInstance()->GetPadState();

        if (padState->IsTapped(Sonic::eKeyState_LeftBumper) || padState->IsTapped(Sonic::eKeyState_RightBumper)) {
            ToggleStats();
            switchCooldown = 0.5f;
            Common::PlaySoundStatic(switchHandle, 1000028);
        }
    }

    void ControlStatSelection() {
        if (selectDelay != -1 || statDelayStart != -1)
            return;

        Sonic::SPadState const* padState = &Sonic::CInputState::GetInstance()->GetPadState();

        bool isUp = padState->IsTapped(Sonic::eKeyState_LeftStickUp) || padState->IsTapped(Sonic::eKeyState_DpadUp);

        if (isUp && statusIndex == CurrentStats().size()) {
            statusIndex = CurrentStatSizeMinusOne();
            Select(statusIndex, true, true);
            Common::PlaySoundStatic(selectHandle, 1000039);
        }
        else if (isUp && statusIndex != CurrentStats().size() && statusIndex != 0) {
            statusIndex = (statusIndex - 1 >= 0) ? statusIndex - 1 : statusIndex;
            Select(statusIndex, true, true);
            Common::PlaySoundStatic(selectHandle, 1000039);
        }

        bool isDown = padState->IsTapped(Sonic::eKeyState_LeftStickDown) || padState->IsTapped(Sonic::eKeyState_DpadDown);

        if (isDown && statusIndex + 1 <= CurrentStatSizeMinusOne()) {
            statusIndex++;
            Select(statusIndex, false, true);
            Common::PlaySoundStatic(selectHandle, 1000039);
        }
        else if (isDown && statusIndex + 1 == CurrentStats().size()) {
            statusIndex = CurrentStats().size();
            Select(statusIndex, false, true, true);
            Common::PlaySoundStatic(selectHandle, 1000039);
        }
    }

    void FooterTimer(const Hedgehog::Universe::SUpdateInfo& updateInfo) {
        if (footerVisible)
            return;

        bool finished = timeFooter >= 1.06f;

        m_status_footer->SetHideFlag(!finished);
        footerVisible = finished;

        if (!finished)
            timeFooter += updateInfo.DeltaTime;
    }

    void StatInitialization(const Hedgehog::Universe::SUpdateInfo& updateInfo) {
        if (statDelayStart != -1) 
            statDelayStart -= updateInfo.DeltaTime;

        if (statDelayStart != -1 && statDelayStart <= 0) {
            AddStat();
            selectDelay = 0.65f;
            statDelayStart = -1;
            statDelay = 0.17f;
        }

        if(statDelayStart == -1) 
            statDelay -= updateInfo.DeltaTime;

        if (statDelayStart == -1 && selectDelay != -1 && selectDelay <= 0) {
            Select(statusIndex, false, false);
            selectDelay = -1;
        }
        else if (statDelayStart == -1 && selectDelay != -1 && selectDelay > 0)
            selectDelay -= updateInfo.DeltaTime;

        if (statDelayStart == -1 && statDelay <= 0) {
            currentStatIndex++;
            AddStat();
            statDelay = 0.17f;
            currentStatIndex = std::clamp(currentStatIndex, 0, CurrentStatSizeMinusOne());
        }
    }

    void UpdateParallel
    (
        const Hedgehog::Universe::SUpdateInfo& updateInfo
    ) override
    {
        if (exit) {
            EndDelay -= updateInfo.DeltaTime;
            return;
        }

        //CurrentStat().Update(updateInfo.DeltaTime);

        StatInitialization(updateInfo);

        if (quitSoundTimer >= 1.06f) {
            Common::PlaySoundStatic(wooshHandle, 1000029);
            quitSoundTimer = -1;
        }
        else if (quitSoundTimer != -1)
            quitSoundTimer += updateInfo.DeltaTime;

        MedalUpdate(updateInfo);

        ControlCharacterSwitch(updateInfo);

        ControlStatSelection();

        FooterTimer(updateInfo);
    }

    void Kill() {
        SendMessage(m_ActorID, boost::make_shared<Sonic::Message::MsgKill>());
    }

    bool IsQuitFinish() {
        bool animsDone = m_tag_bg_1->m_MotionFrame >= m_tag_bg_1->m_MotionEndFrame &&
            m_tag_txt_1->m_MotionFrame >= m_tag_txt_1->m_MotionEndFrame &&
            m_prgs_bg_1->m_MotionFrame >= m_prgs_bg_1->m_MotionEndFrame &&
            m_prgs_bar_1->m_MotionFrame >= m_prgs_bar_1->m_MotionEndFrame &&
            m_prgs_num_1->m_MotionFrame >= m_prgs_num_1->m_MotionEndFrame &&
            m_decide_bg->m_MotionFrame >= m_decide_bg->m_MotionEndFrame &&
            m_status_footer->m_MotionFrame >= m_status_footer->m_MotionEndFrame &&
            m_medal_info->m_MotionFrame >= m_medal_info->m_MotionEndFrame &&
            m_medal_s_gauge->m_MotionFrame >= m_medal_s_gauge->m_MotionEndFrame &&
            m_medal_m_gauge->m_MotionFrame >= m_medal_m_gauge->m_MotionEndFrame;

        for (int i = 0; i < CurrentStats().size(); i++)
        {
            Stat& s = CurrentStats()[i];
            if (s.m_tag_bg_2->m_MotionFrame < s.m_tag_bg_2->m_MotionEndFrame ||
                s.m_tag_txt_2->m_MotionFrame < s.m_tag_txt_2->m_MotionEndFrame ||
                s.m_prgs_bg_2->m_MotionFrame < s.m_prgs_bg_2->m_MotionEndFrame ||
                s.m_prgs_bar_2->m_MotionFrame < s.m_prgs_bar_2->m_MotionEndFrame ||
                s.m_prgs_num_2->m_MotionFrame < s.m_prgs_num_2->m_MotionEndFrame)
                animsDone = false;
        }

        return EndDelay <= 0.0f && animsDone;
    }

    bool IsQuit() {
        return statusIndex == CurrentStats().size();
    }

    ~CHudStatus() {
        HudRemove();
    }
};

void HudStatus::Start() {
    status = boost::make_shared<CHudStatus>();
    Sonic::CGameDocument::GetInstance()->AddGameObject(status);
}

void HudStatus::End() {
    ((CHudStatus*)status.get())->Outro();
}

bool HudStatus::CanEnd() {
    return ((CHudStatus*)status.get())->IsQuit();
}

bool HudStatus::EndDelayFinish() {
    return ((CHudStatus*)status.get())->IsQuitFinish();
}

void HudStatus::Kill() {
    if (status) {
        ((CHudStatus*)status.get())->Kill();
    }
}

void HudStatus::Install() {

}