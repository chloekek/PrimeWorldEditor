#ifndef CRESTYPEINFO
#define CRESTYPEINFO

#include "EResType.h"
#include <Common/CFourCC.h>
#include <Common/EGame.h>
#include <Common/Flags.h>
#include <Common/TString.h>
#include <unordered_map>
#include <vector>

class CResTypeInfo
{
    struct SGameExtension
    {
        EGame Game;
        CFourCC CookedExt;
    };

    EResType mType;
    TString mTypeName;
    TString mRawExtension;
    std::vector<SGameExtension> mCookedExtensions;
    bool mHidden;
    bool mCanBeSerialized;

    static std::unordered_map<EResType, CResTypeInfo*> smTypeMap;

    // Private Methods
    CResTypeInfo(EResType Type, const TString& rkTypeName);
    ~CResTypeInfo();

    // Public Methods
public:
    bool IsInGame(EGame Game) const;
    CFourCC CookedExtension(EGame Game) const;

    // Accessors
    inline EResType Type() const            { return mType; }
    inline TString TypeName() const         { return mTypeName; }
    inline TString RawExtension() const     { return mRawExtension; }
    inline bool IsVisibleInBrowser() const  { return !mHidden; }
    inline bool CanBeSerialized() const     { return mCanBeSerialized; }

    // Static
    static void GetAllTypesInGame(EGame Game, std::list<CResTypeInfo*>& rOut);
    static CResTypeInfo* TypeForCookedExtension(EGame, CFourCC Ext);

    inline static CResTypeInfo* FindTypeInfo(EResType Type)
    {
        auto Iter = smTypeMap.find(Type);
        return (Iter == smTypeMap.end() ? nullptr : Iter->second);
    }

private:
    // Creation
    friend class CResTypeInfoFactory;

    class CResTypeInfoFactory
    {
    public:
        CResTypeInfoFactory();
        void AddExtension(CResTypeInfo *pType, CFourCC Ext, EGame FirstGame, EGame LastGame);
        void InitTypes();
    };
    static CResTypeInfoFactory smTypeInfoFactory;
};

#endif // CRESTYPEINFO
