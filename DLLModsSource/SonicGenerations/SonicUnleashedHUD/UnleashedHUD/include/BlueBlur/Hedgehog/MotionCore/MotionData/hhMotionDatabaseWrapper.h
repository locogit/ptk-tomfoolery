﻿#pragma once

#include <Hedgehog/Base/Type/hhSharedString.h>

namespace Hedgehog::Database
{
    class CDatabase;
}

namespace Hedgehog::Motion
{
    class CLightMotionData;
    class CMotionDatabaseWrapper;

    static inline BB_FUNCTION_PTR(void, __thiscall, fpCDatabaseGetLightMotionData, 0x7599C0,
        CMotionDatabaseWrapper* This, boost::shared_ptr<CLightMotionData>& spLightMotionData, const Hedgehog::Base::CSharedString& name, uint32_t unknown);

    class CMotionDatabaseWrapper
    {
    public:
        Hedgehog::Database::CDatabase* m_pDatabase;

        CMotionDatabaseWrapper(Hedgehog::Database::CDatabase* pDatabase) : m_pDatabase(pDatabase)
        {
        }

        boost::shared_ptr<CLightMotionData> GetLightMotionData(const Base::CSharedString& name, uint32_t unknown = 0)
        {
            boost::shared_ptr<CLightMotionData> spLightMotionData;
            fpCDatabaseGetLightMotionData(this, spLightMotionData, name, unknown);
            return spLightMotionData;
        }
    };

    BB_ASSERT_OFFSETOF(CMotionDatabaseWrapper, m_pDatabase, 0x0);
    BB_ASSERT_SIZEOF(CMotionDatabaseWrapper, 0x4);
}