#ifndef CGAMEEXPORTER_H
#define CGAMEEXPORTER_H

#include "CGameProject.h"
#include <Common/CUniqueID.h>
#include <Common/Flags.h>
#include <Common/TString.h>
#include <Common/types.h>
#include <map>

class CGameExporter
{
    // Project
    CGameProject *mpProject;

    // Directories
    TWideString mGameDir;
    TWideString mExportDir;
    TWideString mDiscDir;
    TWideString mResDir;
    TWideString mWorldsDir;
    TWideString mCookedDir;
    TWideString mCookedResDir;
    TWideString mCookedWorldsDir;

    // Resources
    TWideStringList mWorldPaks;
    TWideStringList mResourcePaks;

    struct SResourceInstance
    {
        TWideString PakFile;
        CUniqueID ResourceID;
        CFourCC ResourceType;
        u32 PakOffset;
        u32 PakSize;
        bool Compressed;
        bool Exported;
    };
    std::map<u64, SResourceInstance> mResourceMap;

    struct SResourcePath
    {
        TWideString Dir;
        TWideString Name;
    };
    std::map<u64, SResourcePath> mResourcePaths;

public:
    CGameExporter(const TString& rkInputDir, const TString& rkOutputDir);
    bool Export();
    void LoadResource(const CUniqueID& rkID, std::vector<u8>& rBuffer);

protected:
    void CopyDiscData();
    void LoadAssetList();
    void LoadPaks();
    void LoadResource(const SResourceInstance& rkResource, std::vector<u8>& rBuffer);
    void ExportWorlds();
    void ExportCookedResources();
    void ExportResource(SResourceInstance& rRes);

    // Convenience Functions
    inline SResourceInstance* FindResourceInstance(const CUniqueID& rkID)
    {
        u64 IntegralID = rkID.ToLongLong();
        auto Found = mResourceMap.find(IntegralID);
        return (Found == mResourceMap.end() ? nullptr : &Found->second);
    }

    inline SResourcePath* FindResourcePath(const CUniqueID& rkID)
    {
        u64 IntegralID = rkID.ToLongLong();
        auto Found = mResourcePaths.find(IntegralID);
        return (Found == mResourcePaths.end() ? nullptr : &Found->second);
    }

    inline void SetResourcePath(const CUniqueID& rkID, const TWideString& rkDir, const TWideString& rkName)
    {
        SetResourcePath(rkID.ToLongLong(), rkDir, rkName);
    }

    inline void SetResourcePath(u64 ID, const TWideString& rkDir, const TWideString& rkName)
    {
        mResourcePaths[ID] = SResourcePath { rkDir, rkName };
    }

    inline EGame Game() const       { return mpProject->Game(); }
    inline void SetGame(EGame Game) { mpProject->SetGame(Game); }
};

#endif // CGAMEEXPORTER_H