boost::shared_ptr<Sonic::CGameObject> status;

bool isWerehog = false;

static SharedPtrTypeless wooshHandle;
static SharedPtrTypeless selectHandle;
static SharedPtrTypeless switchHandle;

class Stat {
    const char* name;
    int txt_num;
    int level;
    int maxLevel;
    int count;

    Chao::CSD::RCPtr<Chao::CSD::CScene> m_tag_bg_2;
    Chao::CSD::RCPtr<Chao::CSD::CScene> m_tag_txt_2;
    Chao::CSD::RCPtr<Chao::CSD::CScene> m_prgs_bg_2;
    Chao::CSD::RCPtr<Chao::CSD::CScene> m_prgs_bar_2;
    Chao::CSD::RCPtr<Chao::CSD::CScene> m_prgs_num_2;
public:
    const char* &GetName() {
        return name;
    }

    int& GetTextNum() {
        return txt_num;
    }

    int& GetLevel() {
        return level;
    }

    int& GetMaxLevel() {
        return maxLevel;
    }

    int& GetCount() {
        return count;
    }

    Chao::CSD::RCPtr<Chao::CSD::CScene>& GetStatScene(const char* name) {
        if (name == "m_tag_bg_2") {
            return m_tag_bg_2;
        }

        if (name == "m_tag_txt_2") {
            return m_tag_txt_2;
        }

        if (name == "m_prgs_bg_2") {
            return m_prgs_bg_2;
        }

        if (name == "m_prgs_bar_2") {
            return m_prgs_bar_2;
        }

        if (name == "m_prgs_num_2") {
            return m_prgs_num_2;
        }
    }

    Stat() {
        txt_num = 2;
        level = 0;
        count = 0;
        maxLevel = 0;
        name = "null";
    }

    Stat(const char* _name, int _level, int _maxLevel, int _txt_num, int _count) {
        name = _name;
        level = _level;
        maxLevel = _maxLevel;
        txt_num = _txt_num;
        count = _count;
    }
};

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
    int currentStatIndex = 0;
    float y = 0.0f;

    float statDelayStart = 0.0f;

    float switchCooldown = 0.0f;

    float EndDelay;

    bool exit = false;

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

    int statusIndex;

    std::vector<Stat> statsDay = {};
    std::vector<Stat> statsNight = {};

    float selectDelay = -1;

    float quitSoundTimer = 0.0f;

    float medalSubImage = 0.0f;

public:
    void HudRemove() {
        if (!m_rcStatus || !m_spStatus) { return; }

        RemoveStats();

        if(m_status_title) Chao::CSD::CProject::DestroyScene(m_rcStatus.Get(), m_status_title);
        if(m_logo) Chao::CSD::CProject::DestroyScene(m_rcStatus.Get(), m_logo);

        if(m_medal_info) Chao::CSD::CProject::DestroyScene(m_rcStatus.Get(), m_medal_info);
        if(m_medal_s_gauge) Chao::CSD::CProject::DestroyScene(m_rcStatus.Get(), m_medal_s_gauge);
        if(m_medal_m_gauge) Chao::CSD::CProject::DestroyScene(m_rcStatus.Get(), m_medal_m_gauge);

        if(m_tag_bg_1) Chao::CSD::CProject::DestroyScene(m_rcStatus.Get(), m_tag_bg_1);
        if(m_tag_txt_1) Chao::CSD::CProject::DestroyScene(m_rcStatus.Get(), m_tag_txt_1);
        if(m_prgs_bg_1) Chao::CSD::CProject::DestroyScene(m_rcStatus.Get(), m_prgs_bg_1);
        if(m_prgs_bar_1) Chao::CSD::CProject::DestroyScene(m_rcStatus.Get(), m_prgs_bar_1);
        if(m_prgs_num_1) Chao::CSD::CProject::DestroyScene(m_rcStatus.Get(), m_prgs_num_1);

        if(m_status_footer) Chao::CSD::CProject::DestroyScene(m_rcStatus.Get(), m_status_footer);
        if(m_decide_bg) Chao::CSD::CProject::DestroyScene(m_rcStatus.Get(), m_decide_bg);

        if(m_spStatus) m_spStatus->SendMessage(m_spStatus->m_ActorID, boost::make_shared<Sonic::Message::MsgKill>());
        if(m_spStatus) m_spStatus = nullptr;

        if(m_rcStatus) m_rcStatus = nullptr;
    }

    void RemoveStats() {
        for (Stat& s : statsDay) {
            Chao::CSD::CProject::DestroyScene(m_rcStatus.Get(), s.GetStatScene("m_tag_bg_2"));
            Chao::CSD::CProject::DestroyScene(m_rcStatus.Get(), s.GetStatScene("m_tag_txt_2"));
            Chao::CSD::CProject::DestroyScene(m_rcStatus.Get(), s.GetStatScene("m_prgs_bg_2"));
            Chao::CSD::CProject::DestroyScene(m_rcStatus.Get(), s.GetStatScene("m_prgs_num_2"));
            Chao::CSD::CProject::DestroyScene(m_rcStatus.Get(), s.GetStatScene("m_prgs_bar_2"));
        }

        for (Stat& s : statsNight) {
            Chao::CSD::CProject::DestroyScene(m_rcStatus.Get(), s.GetStatScene("m_tag_bg_2"));
            Chao::CSD::CProject::DestroyScene(m_rcStatus.Get(), s.GetStatScene("m_tag_txt_2"));
            Chao::CSD::CProject::DestroyScene(m_rcStatus.Get(), s.GetStatScene("m_prgs_bg_2"));
            Chao::CSD::CProject::DestroyScene(m_rcStatus.Get(), s.GetStatScene("m_prgs_num_2"));
            Chao::CSD::CProject::DestroyScene(m_rcStatus.Get(), s.GetStatScene("m_prgs_bar_2"));
        }
    }

    void ToggleStats() {
        isWerehog = !isWerehog;

        //Exp Bar
        if (m_tag_bg_1) {
            m_tag_bg_1->SetHideFlag(false);
            CSDCommon::PlayAnimation(*m_tag_bg_1, intro(), Chao::CSD::eMotionRepeatType_PlayOnce, 1.0, 0.0);
        }

        if (m_tag_txt_1) {
            m_tag_txt_1->SetHideFlag(false);
            CSDCommon::PlayAnimation(*m_tag_txt_1, intro(), Chao::CSD::eMotionRepeatType_PlayOnce, 1.0, 0.0);
        }

        if (m_prgs_bg_1) {
            m_prgs_bg_1->SetHideFlag(false);
            CSDCommon::PlayAnimation(*m_prgs_bg_1, intro(), Chao::CSD::eMotionRepeatType_PlayOnce, 1.0, 0.0);
        }

        if (m_prgs_bar_1) {
            m_prgs_bar_1->SetHideFlag(false);
            CSDCommon::PlayAnimation(*m_prgs_bar_1, intro(), Chao::CSD::eMotionRepeatType_PlayOnce, 1.0, 0.0);
        }

        if (m_prgs_num_1) {
            m_prgs_num_1->SetHideFlag(false);
            CSDCommon::PlayAnimation(*m_prgs_num_1, intro(), Chao::CSD::eMotionRepeatType_PlayOnce, 1.0, 0.0);
        }

        if (m_decide_bg) {
            m_decide_bg->SetHideFlag(false);
            CSDCommon::PlayAnimation(*m_decide_bg, intro(), Chao::CSD::eMotionRepeatType_PlayOnce, 0.0, 100.0, 100.0);
        }

        if (m_status_title) {
            m_status_title->SetHideFlag(false);
            CSDCommon::PlayAnimation(*m_status_title, intro(), Chao::CSD::eMotionRepeatType_PlayOnce, 0.0, m_status_title->m_MotionEndFrame);
        }

        if (m_logo) {
            m_logo->SetHideFlag(false);
            CSDCommon::PlayAnimation(*m_logo, "Switch_Anim", Chao::CSD::eMotionRepeatType_PlayOnce, 1.0, 0, 20.0, false, !isWerehog);
        }

        currentStatIndex = 0;
        statusIndex = 0;
        y = 0.0f;

        RemoveStats();
        AddAllStats();
    }

    void Select(int index, bool up, bool reverse, bool quit = false) {
        std::vector<Stat> currentStats = isWerehog ? statsNight : statsDay;
        int max = isWerehog ? 4 : 1;
        if (quit) {
            if(m_decide_bg) CSDCommon::PlayAnimation(*m_decide_bg, select(), Chao::CSD::eMotionRepeatType_PlayOnce, 1, 0);
            if(currentStats[index - 1].GetStatScene("m_tag_bg_2")) CSDCommon::PlayAnimation(*currentStats[index - 1].GetStatScene("m_tag_bg_2"), select(), Chao::CSD::eMotionRepeatType_PlayOnce, 1, 0, 0, false, true);
            if(currentStats[index - 1].GetStatScene("m_prgs_bg_2")) CSDCommon::PlayAnimation(*currentStats[index - 1].GetStatScene("m_prgs_bg_2"), select(), Chao::CSD::eMotionRepeatType_PlayOnce, 1, 0, 0, false, true);
            if(currentStats[index - 1].GetStatScene("m_prgs_num_2")) CSDCommon::PlayAnimation(*currentStats[index - 1].GetStatScene("m_prgs_num_2"), select(), Chao::CSD::eMotionRepeatType_PlayOnce, 1, 0, 0, false, true);
            if(currentStats[index - 1].GetStatScene("m_prgs_bar_2")) CSDCommon::PlayAnimation(*currentStats[index - 1].GetStatScene("m_prgs_bar_2"), select(), Chao::CSD::eMotionRepeatType_PlayOnce, 1, 0, 0, false, true);
        }
        else {
            if (reverse) {
                if (up && index == (isWerehog ? 4 : 1)) {
                    if(m_decide_bg) CSDCommon::PlayAnimation(*m_decide_bg, select(), Chao::CSD::eMotionRepeatType_PlayOnce, 1, 0, 0, false, true);
                }

                if (up) {
                    if(currentStats[std::clamp(index + 1, 0, max)].GetStatScene("m_tag_bg_2")) CSDCommon::PlayAnimation(*currentStats[std::clamp(index + 1, 0, max)].GetStatScene("m_tag_bg_2"), select(), Chao::CSD::eMotionRepeatType_PlayOnce, 1, 0, 0, false, true);
                    if(currentStats[std::clamp(index + 1, 0, max)].GetStatScene("m_prgs_bg_2")) CSDCommon::PlayAnimation(*currentStats[std::clamp(index + 1, 0, max)].GetStatScene("m_prgs_bg_2"), select(), Chao::CSD::eMotionRepeatType_PlayOnce, 1, 0, 0, false, true);
                    if(currentStats[std::clamp(index + 1, 0, max)].GetStatScene("m_prgs_num_2")) CSDCommon::PlayAnimation(*currentStats[std::clamp(index + 1, 0, max)].GetStatScene("m_prgs_num_2"), select(), Chao::CSD::eMotionRepeatType_PlayOnce, 1, 0, 0, false, true);
                    if(currentStats[std::clamp(index + 1, 0, max)].GetStatScene("m_prgs_bar_2")) CSDCommon::PlayAnimation(*currentStats[std::clamp(index + 1, 0, max)].GetStatScene("m_prgs_bar_2"), select(), Chao::CSD::eMotionRepeatType_PlayOnce, 1, 0, 0, false, true);
                }
                else {
                    if(currentStats[std::clamp(index + 1, 0, max)].GetStatScene("m_tag_bg_2")) CSDCommon::PlayAnimation(*currentStats[std::clamp(index + 1, 0, max)].GetStatScene("m_tag_bg_2"), select(), Chao::CSD::eMotionRepeatType_PlayOnce, 1, 0, 0, false, true);
                    if(currentStats[std::clamp(index + 1, 0, max)].GetStatScene("m_prgs_bg_2")) CSDCommon::PlayAnimation(*currentStats[std::clamp(index + 1, 0, max)].GetStatScene("m_prgs_bg_2"), select(), Chao::CSD::eMotionRepeatType_PlayOnce, 1, 0, 0, false, true);
                    if(currentStats[std::clamp(index + 1, 0, max)].GetStatScene("m_prgs_num_2")) CSDCommon::PlayAnimation(*currentStats[std::clamp(index + 1, 0, max)].GetStatScene("m_prgs_num_2"), select(), Chao::CSD::eMotionRepeatType_PlayOnce, 1, 0, 0, false, true);
                    if(currentStats[std::clamp(index + 1, 0, max)].GetStatScene("m_prgs_bar_2")) CSDCommon::PlayAnimation(*currentStats[std::clamp(index + 1, 0, max)].GetStatScene("m_prgs_bar_2"), select(), Chao::CSD::eMotionRepeatType_PlayOnce, 1, 0, 0, false, true);
                }
            }

            if(currentStats[index].GetStatScene("m_tag_bg_2")) CSDCommon::PlayAnimation(*currentStats[index].GetStatScene("m_tag_bg_2"), select(), Chao::CSD::eMotionRepeatType_PlayOnce, 1, 0);

            if(currentStats[index].GetStatScene("m_prgs_bg_2")) CSDCommon::PlayAnimation(*currentStats[index].GetStatScene("m_prgs_bg_2"), select(), Chao::CSD::eMotionRepeatType_PlayOnce, 1, 0);

            if(currentStats[index].GetStatScene("m_prgs_num_2")) CSDCommon::PlayAnimation(*currentStats[index].GetStatScene("m_prgs_num_2"), select(), Chao::CSD::eMotionRepeatType_PlayOnce, 1, 0);

            if(currentStats[index].GetStatScene("m_prgs_bar_2")) CSDCommon::PlayAnimation(*currentStats[index].GetStatScene("m_prgs_bar_2"), select(), Chao::CSD::eMotionRepeatType_PlayOnce, 1, 0);
        }
    }

    void AddStat() {
        if ((isWerehog && currentStatIndex >= 5) || (!isWerehog && currentStatIndex >= 2)) { 
            return; 
        }

        currentStatIndex = std::clamp(currentStatIndex, 0, (isWerehog ? 4 : 1));
        Stat curStat = isWerehog ? statsNight[currentStatIndex] : statsDay[currentStatIndex];
        curStat.GetStatScene("m_tag_bg_2") = m_rcStatus->CreateScene("tag_bg_2");
        if (curStat.GetStatScene("m_tag_bg_2")) curStat.GetStatScene("m_tag_bg_2")->SetHideFlag(false);
        if (curStat.GetStatScene("m_tag_bg_2")) curStat.GetStatScene("m_tag_bg_2")->SetPosition(0.0f, y);
        if (curStat.GetStatScene("m_tag_bg_2")) CSDCommon::PlayAnimation(*curStat.GetStatScene("m_tag_bg_2"), intro2(), Chao::CSD::eMotionRepeatType_PlayOnce, 1.0, 0.0);

        const char* txtScene;

        switch (curStat.GetTextNum())
        {
        case 1:
            txtScene = "tag_txt_2";
            break;
        case 2:
            txtScene = "tag_txt_3";
            break;
        case 3:
            txtScene = "tag_txt_4";
            break;
        case 4:
            txtScene = "tag_txt_5";
            break;
        case 5:
            txtScene = "tag_txt_6";
            break;
        default:
            txtScene = "tag_txt_2";
            break;
        }

        curStat.GetStatScene("m_tag_txt_2") = m_rcStatus->CreateScene(txtScene);
        if (curStat.GetStatScene("m_tag_txt_2")) curStat.GetStatScene("m_tag_txt_2")->SetHideFlag(false);
        if (curStat.GetStatScene("m_tag_txt_2")) CSDCommon::PlayAnimation(*curStat.GetStatScene("m_tag_txt_2"), (curStat.GetTextNum() < 3) ? intro2() : "Intro_ev_Anim_2", Chao::CSD::eMotionRepeatType_PlayOnce, 1.0, 0.0);

        curStat.GetStatScene("m_prgs_bg_2") = m_rcStatus->CreateScene("prgs_bg_2");
        if (curStat.GetStatScene("m_prgs_bg_2")) curStat.GetStatScene("m_prgs_bg_2")->SetHideFlag(false);
        if (curStat.GetStatScene("m_prgs_bg_2")) curStat.GetStatScene("m_prgs_bg_2")->SetPosition(0.0f, y);
        if (curStat.GetStatScene("m_prgs_bg_2")) CSDCommon::PlayAnimation(*curStat.GetStatScene("m_prgs_bg_2"), intro2(), Chao::CSD::eMotionRepeatType_PlayOnce, 1.0, 0.0);

        curStat.GetStatScene("m_prgs_num_2") = m_rcStatus->CreateScene("prgs_num_2");
        if (curStat.GetStatScene("m_prgs_num_2")) curStat.GetStatScene("m_prgs_num_2")->SetHideFlag(false);
        if (curStat.GetStatScene("m_prgs_num_2")) curStat.GetStatScene("m_prgs_num_2")->SetPosition(0.0f, y);

        if (curStat.GetLevel() >= curStat.GetMaxLevel()) {
            if (curStat.GetStatScene("m_prgs_num_2")) curStat.GetStatScene("m_prgs_num_2")->GetNode("num")->SetHideFlag(1);
            if (curStat.GetStatScene("m_prgs_num_2")) curStat.GetStatScene("m_prgs_num_2")->GetNode("img")->SetHideFlag(1);
            if (curStat.GetStatScene("m_prgs_num_2")) curStat.GetStatScene("m_prgs_num_2")->GetNode("txt")->SetHideFlag(0);
        }
        else {
            if (curStat.GetStatScene("m_prgs_num_2")) curStat.GetStatScene("m_prgs_num_2")->GetNode("num")->SetText(std::to_string(curStat.GetLevel()).c_str());
        }

        if (curStat.GetCount() == 0) {
            if (curStat.GetStatScene("m_prgs_num_2")) curStat.GetStatScene("m_prgs_num_2")->GetNode("num_2")->SetHideFlag(1);
            if (curStat.GetStatScene("m_prgs_num_2")) curStat.GetStatScene("m_prgs_num_2")->GetNode("img_2")->SetHideFlag(1);
        }
        else {
            if (curStat.GetStatScene("m_prgs_num_2")) curStat.GetStatScene("m_prgs_num_2")->GetNode("num_2")->SetText(std::to_string(curStat.GetCount()).c_str());
        }

        if (curStat.GetStatScene("m_prgs_num_2")) CSDCommon::PlayAnimation(*curStat.GetStatScene("m_prgs_num_2"), intro2(), Chao::CSD::eMotionRepeatType_PlayOnce, 1.0, 0.0);

        curStat.GetStatScene("m_prgs_bar_2") = m_rcStatus->CreateScene("prgs_bar_2");
        if (curStat.GetStatScene("m_prgs_bar_2")) curStat.GetStatScene("m_prgs_bar_2")->SetHideFlag(false);
        if (curStat.GetStatScene("m_prgs_bar_2")) curStat.GetStatScene("m_prgs_bar_2")->SetPosition(0.0f, y);
        if (curStat.GetStatScene("m_prgs_bar_2")) CSDCommon::PlayAnimation(*curStat.GetStatScene("m_prgs_bar_2"), intro2(), Chao::CSD::eMotionRepeatType_PlayOnce, 1.0, 0.0);

        y += (isWerehog ? 64.0f : 73.0f);

        statDelay = 0.17f;

        Common::PlaySoundStatic(wooshHandle, 1000029);
    }

    void AddAllStats() {
        for (int i = 0; i < (isWerehog ? 5 : 2); i++)
        {
            currentStatIndex = i;
            Stat curStat = isWerehog ? statsNight[currentStatIndex] : statsDay[currentStatIndex];
            curStat.GetStatScene("m_tag_bg_2") = m_rcStatus->CreateScene("tag_bg_2");
            if (curStat.GetStatScene("m_tag_bg_2")) curStat.GetStatScene("m_tag_bg_2")->SetHideFlag(false);
            if (curStat.GetStatScene("m_tag_bg_2")) curStat.GetStatScene("m_tag_bg_2")->SetPosition(0.0f, y);
            if (curStat.GetStatScene("m_tag_bg_2")) CSDCommon::PlayAnimation(*curStat.GetStatScene("m_tag_bg_2"), intro2(), Chao::CSD::eMotionRepeatType_PlayOnce, 1.0, 0.0);

            const char* txtScene;

            switch (curStat.GetTextNum())
            {
            case 1:
                txtScene = "tag_txt_2";
                break;
            case 2:
                txtScene = "tag_txt_3";
                break;
            case 3:
                txtScene = "tag_txt_4";
                break;
            case 4:
                txtScene = "tag_txt_5";
                break;
            case 5:
                txtScene = "tag_txt_6";
                break;
            default:
                txtScene = "tag_txt_2";
                break;
            }

            curStat.GetStatScene("m_tag_txt_2") = m_rcStatus->CreateScene(txtScene);
            if (curStat.GetStatScene("m_tag_txt_2")) curStat.GetStatScene("m_tag_txt_2")->SetHideFlag(false);
            if (curStat.GetStatScene("m_tag_txt_2")) CSDCommon::PlayAnimation(*curStat.GetStatScene("m_tag_txt_2"), (curStat.GetTextNum() < 3) ? intro2() : "Intro_ev_Anim_2", Chao::CSD::eMotionRepeatType_PlayOnce, 1.0, 0.0);

            curStat.GetStatScene("m_prgs_bg_2") = m_rcStatus->CreateScene("prgs_bg_2");
            if (curStat.GetStatScene("m_prgs_bg_2")) curStat.GetStatScene("m_prgs_bg_2")->SetHideFlag(false);
            if (curStat.GetStatScene("m_prgs_bg_2")) curStat.GetStatScene("m_prgs_bg_2")->SetPosition(0.0f, y);
            if (curStat.GetStatScene("m_prgs_bg_2")) CSDCommon::PlayAnimation(*curStat.GetStatScene("m_prgs_bg_2"), intro2(), Chao::CSD::eMotionRepeatType_PlayOnce, 1.0, 0.0);

            curStat.GetStatScene("m_prgs_num_2") = m_rcStatus->CreateScene("prgs_num_2");
            if (curStat.GetStatScene("m_prgs_num_2")) curStat.GetStatScene("m_prgs_num_2")->SetHideFlag(false);
            if (curStat.GetStatScene("m_prgs_num_2")) curStat.GetStatScene("m_prgs_num_2")->SetPosition(0.0f, y);

            if (curStat.GetLevel() >= curStat.GetMaxLevel()) {
                if (curStat.GetStatScene("m_prgs_num_2")) curStat.GetStatScene("m_prgs_num_2")->GetNode("num")->SetHideFlag(1);
                if (curStat.GetStatScene("m_prgs_num_2")) curStat.GetStatScene("m_prgs_num_2")->GetNode("img")->SetHideFlag(1);
                if (curStat.GetStatScene("m_prgs_num_2")) curStat.GetStatScene("m_prgs_num_2")->GetNode("txt")->SetHideFlag(0);
            }
            else {
                if (curStat.GetStatScene("m_prgs_num_2")) curStat.GetStatScene("m_prgs_num_2")->GetNode("num")->SetText(std::to_string(curStat.GetLevel()).c_str());
            }

            if (curStat.GetCount() == 0) {
                if (curStat.GetStatScene("m_prgs_num_2")) curStat.GetStatScene("m_prgs_num_2")->GetNode("num_2")->SetHideFlag(1);
                if (curStat.GetStatScene("m_prgs_num_2")) curStat.GetStatScene("m_prgs_num_2")->GetNode("img_2")->SetHideFlag(1);
            }
            else {
                if (curStat.GetStatScene("m_prgs_num_2")) curStat.GetStatScene("m_prgs_num_2")->GetNode("num_2")->SetText(std::to_string(curStat.GetCount()).c_str());
            }

            if (curStat.GetStatScene("m_prgs_num_2")) CSDCommon::PlayAnimation(*curStat.GetStatScene("m_prgs_num_2"), intro2(), Chao::CSD::eMotionRepeatType_PlayOnce, 1.0, 0.0);

            curStat.GetStatScene("m_prgs_bar_2") = m_rcStatus->CreateScene("prgs_bar_2");
            if (curStat.GetStatScene("m_prgs_bar_2")) curStat.GetStatScene("m_prgs_bar_2")->SetHideFlag(false);
            if (curStat.GetStatScene("m_prgs_bar_2")) curStat.GetStatScene("m_prgs_bar_2")->SetPosition(0.0f, y);
            if (curStat.GetStatScene("m_prgs_bar_2")) CSDCommon::PlayAnimation(*curStat.GetStatScene("m_prgs_bar_2"), intro2(), Chao::CSD::eMotionRepeatType_PlayOnce, 1.0, 0.0);

            y += (isWerehog ? 64.0f : 73.0f);
        }
        selectDelay = 0.65f;
    }

    void Outro() {
        // Quit Button
        if(m_decide_bg) CSDCommon::PlayAnimation(*m_decide_bg, intro(), Chao::CSD::eMotionRepeatType_PlayOnce, 1.0, 0.0, 0.0, false, true);

        // Header
        if(m_status_title) CSDCommon::PlayAnimation(*m_status_title, outro(), Chao::CSD::eMotionRepeatType_PlayOnce, 1.0, 0.0);

        // Character Portrait
        if(m_logo) CSDCommon::PlayAnimation(*m_logo, intro(), Chao::CSD::eMotionRepeatType_PlayOnce, 1.0, 0.0, 0.0, false, true);

        // Bottom Buttons
        if(m_status_footer) m_status_footer->SetHideFlag(true);

        //Medals (Top Right)
        if(m_medal_info) CSDCommon::PlayAnimation(*m_medal_info, "Intro_Anim", Chao::CSD::eMotionRepeatType_PlayOnce, 1.0, 0.0, 0.0, false, true);

        if(m_medal_s_gauge) CSDCommon::PlayAnimation(*m_medal_s_gauge, "Intro_Anim", Chao::CSD::eMotionRepeatType_PlayOnce, 1.0, 0.0, 0.0, false, true);

        if(m_medal_m_gauge) CSDCommon::PlayAnimation(*m_medal_m_gauge, "Intro_Anim", Chao::CSD::eMotionRepeatType_PlayOnce, 1.0, 0.0, 0.0, false, true);

        currentStatIndex = std::clamp(currentStatIndex, 0, (isWerehog ? 4 : 1));
        Stat curStat = isWerehog ? statsNight[currentStatIndex] : statsDay[currentStatIndex];

        std::vector<Stat> currentStats = isWerehog ? statsNight : statsDay;
        for (size_t i = 0; i < currentStats.size(); i++)
        {
            Stat s = currentStats[i];
            if (s.GetStatScene("m_tag_bg_2")) CSDCommon::PlayAnimation(*s.GetStatScene("m_tag_bg_2"), intro2(), Chao::CSD::eMotionRepeatType_PlayOnce, 1.0, 0.0, 0.0, false, true);
            if (s.GetStatScene("m_tag_txt_2")) CSDCommon::PlayAnimation(*s.GetStatScene("m_tag_txt_2"), (curStat.GetTextNum() < 3) ? intro2() : "Intro_ev_Anim_2", Chao::CSD::eMotionRepeatType_PlayOnce, 1.0, 0.0, 0.0, false, true);
            if (s.GetStatScene("m_prgs_num_2")) CSDCommon::PlayAnimation(*s.GetStatScene("m_prgs_num_2"), intro2(), Chao::CSD::eMotionRepeatType_PlayOnce, 1.0, 0.0, 0.0, false, true);
            if (s.GetStatScene("m_prgs_bar_2")) CSDCommon::PlayAnimation(*s.GetStatScene("m_prgs_bar_2"), intro2(), Chao::CSD::eMotionRepeatType_PlayOnce, 1.0, 0.0, 0.0, false, true);
        }

        // Exp Bar
        if(m_tag_bg_1) CSDCommon::PlayAnimation(*m_tag_bg_1, intro(), Chao::CSD::eMotionRepeatType_PlayOnce, 1.0, 0.0, 0.0, false, true);

        if(m_tag_txt_1) CSDCommon::PlayAnimation(*m_tag_txt_1, intro(), Chao::CSD::eMotionRepeatType_PlayOnce, 1.0, 0.0, 0.0, false, true);

        if(m_prgs_bg_1) CSDCommon::PlayAnimation(*m_prgs_bg_1, intro(), Chao::CSD::eMotionRepeatType_PlayOnce, 1.0, 0.0, 0.0, false, true);

        if(m_prgs_bar_1) CSDCommon::PlayAnimation(*m_prgs_bar_1, intro(), Chao::CSD::eMotionRepeatType_PlayOnce, 1.0, 0.0, 0.0, false, true);

        if(m_prgs_num_1) CSDCommon::PlayAnimation(*m_prgs_num_1, intro(), Chao::CSD::eMotionRepeatType_PlayOnce, 1.0, 0.0, 0.0, false, true);

        EndDelay = 1.0f;
        exit = true;
    }

    void AddCallback
    (
        const Hedgehog::Base::THolder<Sonic::CWorld>& worldHolder,
        Sonic::CGameDocument* pGameDocument,
        const boost::shared_ptr<Hedgehog::Database::CDatabase>& spDatabase
    ) override
    {
        // const char* _name, int _level, int _maxLevel, int _txt_num, int _count
        statsDay = { Stat("speed", 9, 11, 1, 10), Stat("ring_energy", 6, 6, 2, 0) };

        statsNight = { Stat("combat", 0, 31, 1, 0), Stat("strength", 0, 11, 2, 0), Stat("life", 0, 11, 3, 0), Stat("unleash", 0, 11, 4, 0), Stat("shield", 0, 11, 5, 0) };

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

    void Start() {

        if (m_tag_bg_1) {
            m_tag_bg_1->SetHideFlag(false);
            CSDCommon::PlayAnimation(*m_tag_bg_1, intro(), Chao::CSD::eMotionRepeatType_PlayOnce, 1.0, 0.0);
        }

        if (m_tag_txt_1) {
            m_tag_txt_1->SetHideFlag(false);
            CSDCommon::PlayAnimation(*m_tag_txt_1, intro(), Chao::CSD::eMotionRepeatType_PlayOnce, 1.0, 0.0);
        }

        if (m_prgs_bg_1) {
            m_prgs_bg_1->SetHideFlag(false);
            CSDCommon::PlayAnimation(*m_prgs_bg_1, intro(), Chao::CSD::eMotionRepeatType_PlayOnce, 1.0, 0.0);
        }

        if (m_prgs_bar_1) {
            m_prgs_bar_1->SetHideFlag(false);
            CSDCommon::PlayAnimation(*m_prgs_bar_1, intro(), Chao::CSD::eMotionRepeatType_PlayOnce, 1.0, 0.0);
        }

        if (m_prgs_num_1) {
            m_prgs_num_1->SetHideFlag(false);
            m_prgs_num_1->GetNode("num")->SetText("69");
            CSDCommon::PlayAnimation(*m_prgs_num_1, intro(), Chao::CSD::eMotionRepeatType_PlayOnce, 1.0, 0.0);
        }

        if (m_decide_bg) {
            m_decide_bg->SetHideFlag(false);
            CSDCommon::PlayAnimation(*m_decide_bg, intro(), Chao::CSD::eMotionRepeatType_PlayOnce, 1.0, 0.0);
        }

        if (m_status_footer) {
            m_status_footer->SetHideFlag(true);
            CSDCommon::PlayAnimation(*m_status_footer, "Usual_Anim_2", Chao::CSD::eMotionRepeatType_PlayOnce, 1.0, 0.0);
        }

        if (m_status_title) {
            m_status_title->SetHideFlag(false);
            CSDCommon::PlayAnimation(*m_status_title, intro(), Chao::CSD::eMotionRepeatType_PlayOnce, 1.0, 0.0);
        }

        if (m_logo) {
            m_logo->SetHideFlag(false);
            CSDCommon::PlayAnimation(*m_logo, intro(), Chao::CSD::eMotionRepeatType_PlayOnce, 1.0, 0.0);
        }

        if (m_medal_info) {
            m_medal_info->SetHideFlag(false);
            CSDCommon::PlayAnimation(*m_medal_info, "Intro_Anim", Chao::CSD::eMotionRepeatType_PlayOnce, 1.0, 0.0);
        }

        if (m_medal_s_gauge) {
            m_medal_s_gauge->SetHideFlag(false);
            CSDCommon::PlayAnimation(*m_medal_s_gauge, "Intro_Anim", Chao::CSD::eMotionRepeatType_PlayOnce, 1.0, 0.0);
        }

        if (m_medal_m_gauge) {
            m_medal_m_gauge->SetHideFlag(false);
            CSDCommon::PlayAnimation(*m_medal_m_gauge, "Intro_Anim", Chao::CSD::eMotionRepeatType_PlayOnce, 1.0, 0.0);
        }

        medalSubImage = 0.0f;

        timeFooter = 0.0f;
        footerVisible = false;

        statDelayStart = 0.75f;

        currentStatIndex = 0;
        y = 0.0f;

        quitSoundTimer = 0.0f;

        exit = false;
    }

    void UpdateParallel
    (
        const Hedgehog::Universe::SUpdateInfo& updateInfo
    ) override
    {
        Sonic::SPadState const* padState = &Sonic::CInputState::GetInstance()->GetPadState();

        if (!exit) {
            if (statDelayStart != -1) {
                statDelayStart -= updateInfo.DeltaTime;
                if (statDelayStart <= 0) {
                    AddStat();
                    selectDelay = 0.65f;
                    statDelayStart = -1;
                }
            }
            else {
                if (selectDelay != -1) {
                    if (selectDelay <= 0) {
                        Select(statusIndex, false, false);
                        selectDelay = -1;
                    }
                    else {
                        selectDelay -= updateInfo.DeltaTime;
                    }
                }
                statDelay -= updateInfo.DeltaTime;
                if (statDelay <= 0) {
                    currentStatIndex++;
                    AddStat();
                    currentStatIndex = std::clamp(currentStatIndex, 0, (isWerehog ? 4 : 1));
                }
            }

            if (quitSoundTimer >= 1.06f) {
                Common::PlaySoundStatic(wooshHandle, 1000029);
                quitSoundTimer = -1;
            }
            else if(quitSoundTimer != -1) {
                quitSoundTimer += updateInfo.DeltaTime;
            }

            medalSubImage += updateInfo.DeltaTime * 25.0f;
            if (medalSubImage >= 30) {
                medalSubImage = 0;
            }

            if (m_medal_info) {
                m_medal_info->GetNode("sun")->SetPatternIndex(std::clamp(medalSubImage, 0.0f, 29.0f));
                m_medal_info->GetNode("sun_shade")->SetPatternIndex(std::clamp(medalSubImage, 0.0f, 29.0f));

                m_medal_info->GetNode("moon")->SetPatternIndex(std::clamp(medalSubImage, 0.0f, 29.0f));
                m_medal_info->GetNode("moon_shade")->SetPatternIndex(std::clamp(medalSubImage, 0.0f, 29.0f));
            }

            if (selectDelay == -1 && statDelayStart == -1) {
                if (switchCooldown <= 0) {
                    if (padState->IsTapped(Sonic::eKeyState_LeftBumper) || padState->IsTapped(Sonic::eKeyState_RightBumper)) {
                        ToggleStats();
                        switchCooldown = 0.5f;
                        Common::PlaySoundStatic(switchHandle, 1000028);
                    }
                }
                else {
                    switchCooldown -= updateInfo.DeltaTime;
                }
            }

            if (selectDelay == -1 && statDelayStart == -1) {
                if (padState->IsTapped(Sonic::eKeyState_LeftStickUp) || padState->IsTapped(Sonic::eKeyState_DpadUp)) {
                    if (statusIndex == (isWerehog ? 5 : 2)) {
                        statusIndex = (isWerehog ? 4 : 1);
                        Select(statusIndex, true, true);
                        Common::PlaySoundStatic(selectHandle, 1000039);
                    }
                    else if(statusIndex != 0) {
                        if (statusIndex - 1 >= 0) {
                            statusIndex--;
                        }
                        Select(statusIndex, true, true);
                        Common::PlaySoundStatic(selectHandle, 1000039);
                    }
                }
                if (padState->IsTapped(Sonic::eKeyState_LeftStickDown) || padState->IsTapped(Sonic::eKeyState_DpadDown)) {
                    if (statusIndex + 1 <= (isWerehog ? 4 : 1)) {
                        statusIndex++;
                        Select(statusIndex, false, true);
                        Common::PlaySoundStatic(selectHandle, 1000039);
                    }
                    else if (statusIndex + 1 == (isWerehog ? 5 : 2)) {
                        statusIndex = (isWerehog ? 5 : 2);
                        Select(statusIndex, false, true, true);
                        Common::PlaySoundStatic(selectHandle, 1000039);
                    }
                }
            }

            if (!footerVisible) {
                if (timeFooter >= 1.06f) {
                    if(m_status_footer) m_status_footer->SetHideFlag(false);
                    footerVisible = true;
                }
                else { if(m_status_footer) m_status_footer->SetHideFlag(true); timeFooter += updateInfo.DeltaTime; }
            }
        }
        else {
            EndDelay -= updateInfo.DeltaTime;
        }
    }

    void Kill() {
        HudRemove();
        SendMessage(m_ActorID, boost::make_shared<Sonic::Message::MsgKill>());
    }

    bool IsQuitFinish() {
        return EndDelay <= 0.0f;
    }

    bool IsQuit() {
        return statusIndex == (isWerehog ? 5 : 2);
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