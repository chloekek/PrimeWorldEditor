#ifndef CRENAMERESOURCECOMMAND_H
#define CRENAMERESOURCECOMMAND_H

#include "IUndoCommand.h"
#include "Editor/CEditorApplication.h"
#include "Editor/ResourceBrowser/CResourceBrowser.h"
#include <Core/GameProject/CResourceEntry.h>

class CRenameResourceCommand : public IUndoCommand
{
    CResourceEntry *mpEntry;
    TString mOldName;
    TString mNewName;
    bool mOldNameAutoGenerated;

public:
    CRenameResourceCommand(CResourceEntry *pEntry, const TString& rkNewName)
        : IUndoCommand("Rename Resource")
        , mpEntry(pEntry)
        , mOldName(pEntry->Name())
        , mNewName(rkNewName)
        , mOldNameAutoGenerated(pEntry->HasFlag(EResEntryFlag::AutoResName))
    {}

    void undo() override { DoMove(mOldName, mOldNameAutoGenerated); }
    void redo() override { DoMove(mNewName, false); }
    bool AffectsCleanState() const override { return false; }

protected:
    void DoMove(const TString& rkName, bool IsAutoName)
    {
        TString FullNewName = rkName + "." + mpEntry->CookedExtension().ToString();
        gpEdApp->ResourceBrowser()->ResourceAboutToBeMoved(mpEntry, TO_QSTRING(mpEntry->Directory()->FullPath() + FullNewName));

        TString OldName = mpEntry->Name();
        bool Success = mpEntry->Rename(rkName, IsAutoName);
        ASSERT(Success);

        gpEdApp->ResourceBrowser()->ResourceMoved(mpEntry, mpEntry->Directory(), OldName);
    }
};

#endif // CRENAMERESOURCECOMMAND_H
