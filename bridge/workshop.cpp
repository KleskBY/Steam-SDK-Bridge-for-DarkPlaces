#include "workshop.h"
#include "main.h"
#include <unordered_set>
#include <sstream>

Workshop::Workshop(fs::path targetDataDir) : targetDataDir_(std::move(targetDataDir)), cbItemInstalled_(this, &Workshop::OnItemInstalled)
    , cbDownloadItemResult_(this, &Workshop::OnDownloadItemResult)
    , cbSubscribed_(this, &Workshop::OnSubscribed)
    , cbUnsubscribed_(this, &Workshop::OnUnsubscribed)
{
}

void Workshop::EnsureDownload(PublishedFileId_t id) 
{
    if (!id || !SteamUGC()) return;

    uint64 size = 0; char folder[4096] = {}; uint32 ts = 0;
    if (SteamUGC()->GetItemInstallInfo(id, &size, folder, sizeof(folder), &ts)) 
    {
        copyPk3From(folder, id);
    }
    else 
    {
        SteamUGC()->DownloadItem(id, true);
        pending_.insert(id);
        log("[WS ] Download queued for " + std::to_string(id));
    }
}

void Workshop::SyncAll() 
{
    if (!SteamUGC()) return;
    uint32 count = SteamUGC()->GetNumSubscribedItems();
    std::vector<PublishedFileId_t> ids(count);
    uint32 got = SteamUGC()->GetSubscribedItems(ids.data(), count);
    log("[WS ] SyncAll: " + std::to_string(got) + " items");
    for (uint32 i = 0; i < got; ++i) EnsureDownload(ids[i]);
    //fs_rescan;
}

void Workshop::Subscribe(PublishedFileId_t id) 
{
    if (!SteamUGC()) return;
    SteamUGC()->SubscribeItem(id);
    log("[WS ] Subscribe requested for " + std::to_string(id));
}

void Workshop::Unsubscribe(PublishedFileId_t id) 
{
    if (!SteamUGC()) return;
    SteamUGC()->UnsubscribeItem(id);
    log("[WS ] Unsubscribe requested for " + std::to_string(id));
}

void Workshop::List() 
{
    if (!SteamUGC()) return;
    uint32 count = SteamUGC()->GetNumSubscribedItems();
    std::vector<PublishedFileId_t> ids(count);
    uint32 got = SteamUGC()->GetSubscribedItems(ids.data(), count);

    log("[WS ] ---- Subscribed Workshop Items ----");
    for (uint32 i = 0; i < got; ++i) 
    {
        auto id = ids[i];
        uint64 size = 0; char folder[4096] = {}; uint32 ts = 0;
        bool installed = SteamUGC()->GetItemInstallInfo(id, &size, folder, sizeof(folder), &ts);
        std::ostringstream oss;
        oss << "  > " << id << (installed ? " [installed] " : " [pending] ") << (installed ? std::string(folder) : "");
        log(oss.str());
    }
    log("[WS ] -----------------------------------");
}

void Workshop::InstallNow(PublishedFileId_t id) 
{
    uint64 size = 0; char folder[4096] = {}; uint32 ts = 0;
    if (SteamUGC()->GetItemInstallInfo(id, &size, folder, sizeof(folder), &ts))
        copyPk3From(folder, id);
    else
        log("[WS ] InstallNow: item not installed yet: " + std::to_string(id));
}

// ---------- Callbacks ----------
void Workshop::OnSubscribed(RemoteStoragePublishedFileSubscribed_t* p) 
{
    log("[WS ] OnSubscribed -> " + std::to_string(p->m_nPublishedFileId));
    EnsureDownload(p->m_nPublishedFileId);
}
void Workshop::OnUnsubscribed(RemoteStoragePublishedFileUnsubscribed_t* p) 
{
    log("[WS ] OnUnsubscribed -> " + std::to_string(p->m_nPublishedFileId));
}
void Workshop::OnDownloadItemResult(DownloadItemResult_t* p) 
{
    auto id = p->m_nPublishedFileId;
    if (p->m_eResult == k_EResultOK) 
    {
        uint64 size = 0; char folder[4096] = {}; uint32 ts = 0;
        if (SteamUGC()->GetItemInstallInfo(id, &size, folder, sizeof(folder), &ts))
            copyPk3From(folder, id);
    }
    else 
    {
        std::ostringstream oss;
        oss << "[WS ] Download failed for " << id << " result=" << p->m_eResult;
        log(oss.str());
    }
    pending_.erase(id);
}
void Workshop::OnItemInstalled(ItemInstalled_t* p) 
{
    if (p->m_unAppID != SteamUtils()->GetAppID()) return;
    uint64 size = 0; char folder[4096] = {}; uint32 ts = 0;
    if (SteamUGC()->GetItemInstallInfo(p->m_nPublishedFileId, &size, folder, sizeof(folder), &ts)) 
        copyPk3From(folder, p->m_nPublishedFileId);
}

void Workshop::copyPk3From(const std::string& srcFolder, PublishedFileId_t id) 
{
    std::error_code ec;
    fs::create_directories(targetDataDir_, ec);

    size_t copied = 0;
    for (auto& p : fs::recursive_directory_iterator(srcFolder, ec)) 
    {
        if (ec) break;
        if (!p.is_regular_file()) continue;
        auto ext = p.path().extension().u8string();
        for (auto& c : ext) c = (char)tolower((unsigned char)c);
        if (ext == ".pk3") 
        {
            fs::path dst = targetDataDir_ / p.path().filename();
            std::error_code ec2;
            fs::copy_file(p.path(), dst, fs::copy_options::overwrite_existing, ec2);
            if (!ec2) ++copied;
            else log("[WS ] Copy failed: " + p.path().string());
        }
    }

    std::ostringstream oss;
    oss << "[WS ] Installed " << copied << " pk3 from " << srcFolder << " to " << targetDataDir_.string() << " (item " << id << ")";
    log(oss.str());
}







// ==== PUBLISHING (CREATE/UPDATE) ====
// Usage:
//ws.NewDraft();
//ws.SetDraftContentFolder("D:\\GameDev\\ObjectN\\workshop\\");
//ws.SetDraftTitle("Test");
//ws.SetDraftDescription("faf");
//ws.SetDraftTags({ "faf", "faf" });
//ws.SetDraftVisibility(k_ERemoteStoragePublishedFileVisibilityPublic);
//ws.SetDraftPreviewImage("");
//ws.CommitDraft();


void Workshop::NewDraft(EWorkshopFileType type) 
{
    draft_ = Draft{};
    draft_.active = true;
    draft_.creating = true;
    draft_.type = type;
    draft_.targetId = k_PublishedFileIdInvalid;
    log("[WS ] Draft: NEW (CreateItem)");
}

void Workshop::UpdateDraft(PublishedFileId_t id) 
{
    draft_ = Draft{};
    draft_.active = true;
    draft_.creating = false;
    draft_.targetId = id;
    log("[WS ] Draft: UPDATE for item " + std::to_string(id));
}

void Workshop::SetDraftContentFolder(const std::string& folder) { draft_.folder = folder; }
void Workshop::SetDraftTitle(const std::string& title) { draft_.title = title; }
void Workshop::SetDraftDescription(const std::string& desc) { draft_.desc = desc; }
void Workshop::SetDraftPreviewImage(const std::string& imagePath) { draft_.preview = imagePath; }
void Workshop::SetDraftTags(const std::vector<std::string>& tags) { draft_.tags = tags; }
void Workshop::SetDraftVisibility(ERemoteStoragePublishedFileVisibility vis) { draft_.visibility = vis; }

std::optional<PublishedFileId_t> Workshop::DraftTargetId() const 
{
    if (!draft_.active) return std::nullopt;
    if (draft_.targetId == k_PublishedFileIdInvalid) return std::nullopt;
    return draft_.targetId;
}

void Workshop::CommitDraft(const std::string& changeNote) 
{
    if (!SteamUGC()) { log("[WS ] CommitDraft: SteamUGC not available"); return; }
    if (!draft_.active) { log("[WS ] CommitDraft: no active draft"); return; }
    pendingChangeNote_ = changeNote;

    if (draft_.creating) 
    {
        AppId_t app = SteamUtils()->GetAppID();
        SteamAPICall_t h = SteamUGC()->CreateItem(app, draft_.type);
        log("[WS ] CreateItem submitted...");
        cbCreateItem_.Set(h, this, &Workshop::OnCreateItem_);
        return;
    }

    // updating existing
    ApplyDraftAndSubmit_();
}

void Workshop::ApplyDraftAndSubmit_() 
{
    if (!SteamUGC()) return;
    if (draft_.targetId == k_PublishedFileIdInvalid) {
        log("[WS ] ApplyDraftAndSubmit_: invalid target id");
        return;
    }
    AppId_t app = SteamUtils()->GetAppID();
    draft_.updateHandle = SteamUGC()->StartItemUpdate(app, draft_.targetId);

    // Content folder (required)
    if (!draft_.folder.empty()) {
        SteamUGC()->SetItemContent(draft_.updateHandle, draft_.folder.c_str());
    }

    if (!draft_.title.empty())        SteamUGC()->SetItemTitle(draft_.updateHandle, draft_.title.c_str());
    if (!draft_.desc.empty())         SteamUGC()->SetItemDescription(draft_.updateHandle, draft_.desc.c_str());
    SteamUGC()->SetItemVisibility(draft_.updateHandle, draft_.visibility);

    if (!draft_.preview.empty())      SteamUGC()->SetItemPreview(draft_.updateHandle, draft_.preview.c_str());

    if (!draft_.tags.empty()) {
        // Build C string array with lifetime through the call
        std::vector<const char*> ctags; ctags.reserve(draft_.tags.size());
        for (auto& s : draft_.tags) ctags.push_back(s.c_str());
        SteamParamStringArray_t tagArray{ ctags.data(), static_cast<int>(ctags.size()) };
        SteamUGC()->SetItemTags(draft_.updateHandle, &tagArray);
    }

    SteamAPICall_t h = SteamUGC()->SubmitItemUpdate(draft_.updateHandle,
        pendingChangeNote_.empty() ? nullptr : pendingChangeNote_.c_str());
    log("[WS ] SubmitItemUpdate queued...");
    cbSubmitUpdate_.Set(h, this, &Workshop::OnSubmitItemUpdate_);
}

void Workshop::OnCreateItem_(CreateItemResult_t* p, bool ioFailure) 
{
    if (ioFailure || p->m_eResult != k_EResultOK) 
    {
        std::ostringstream oss; oss << "[WS ] CreateItem failed io=" << ioFailure << " res=" << p->m_eResult;
        log(oss.str());
        draft_.active = false;
        return;
    }
    draft_.targetId = p->m_nPublishedFileId;
    log("[WS ] CreateItem OK -> id " + std::to_string(draft_.targetId));
    ApplyDraftAndSubmit_(); // After creation, we must run an update to attach content/metadata.
}

void Workshop::OnSubmitItemUpdate_(SubmitItemUpdateResult_t* p, bool ioFailure) 
{
    if (ioFailure || p->m_eResult != k_EResultOK) 
    {
        std::ostringstream oss; oss << "[WS ] SubmitItemUpdate failed io=" << ioFailure << " res=" << p->m_eResult;
        log(oss.str());
        // keep draft so user can fix and retry
        return;
    }
    log("[WS ] SubmitItemUpdate OK. Needs moderation? " + std::to_string((int)p->m_bUserNeedsToAcceptWorkshopLegalAgreement));

    // Finished successfully; keep target id for reference but clear transient update handle
    draft_.updateHandle = k_UGCUpdateHandleInvalid;
    // Optional: deactivate draft now that it has been submitted
    // draft_.active = false;
}




