// SPDX-License-Identifier: MIT
// Workshop.h — Steam Workshop (UGC) helper for SteamBridge
//
// Handles subscribing, downloading, installing, and syncing Workshop items.
// Automatically copies all *.pk3 files from downloaded item folders into your
// game’s data directory.
//
// Requires: Steamworks SDK (ISteamUGC), C++17, <filesystem>
// Link with: steam_api64.lib or libsteam_api.so
//
// ---------------------------------------------------------------------------

#pragma once

#include <public/steam/steam_api.h>
#include <public/steam/isteamremotestorage.h>
#include <filesystem>
#include <unordered_set>
#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <functional>
#include <optional>


namespace fs = std::filesystem;

class Workshop {
public:
    explicit Workshop(fs::path targetDataDir);

    void EnsureDownload(PublishedFileId_t id);  // Request download / install for a single Workshop item
    void SyncAll();                 // Iterate all subscribed items and make sure they are downloaded + installed
    void Subscribe(PublishedFileId_t id);       // Subscribe
    void Unsubscribe(PublishedFileId_t id);     // Unsubscribe
    void List();                                // Log all subscribed items to console
    void InstallNow(PublishedFileId_t id);      // Immediately copy *.pk3 from an already installed item


    // ==== PUBLISHING (CREATE/UPDATE) ====
    void NewDraft(EWorkshopFileType type = k_EWorkshopFileTypeCommunity);   // Start a fresh draft (for CreateItem). Default type: Community.
    void UpdateDraft(PublishedFileId_t id);                                 // Start an update draft for an existing item id

    // Set draft fields
    void SetDraftContentFolder(const std::string& folder);  // path to folder with your .pk3 etc.
    void SetDraftTitle(const std::string& title);
    void SetDraftDescription(const std::string& desc);
    void SetDraftPreviewImage(const std::string& imagePath); // jpg/png
    void SetDraftTags(const std::vector<std::string>& tags); // ["maps","mod","q3"]
    void SetDraftVisibility(ERemoteStoragePublishedFileVisibility vis);

    // Submit draft (create or update). changeNote may be empty.
    // Will log progress; on success keeps the resulting PublishedFileId_t.
    void CommitDraft(const std::string& changeNote = std::string());

    // Read current draft target id (after create completes)
    std::optional<PublishedFileId_t> DraftTargetId() const;

private:
    fs::path targetDataDir_;
    std::unordered_set<PublishedFileId_t> pending_;

    // Steam callbacks
    STEAM_CALLBACK(Workshop, OnItemInstalled, ItemInstalled_t, cbItemInstalled_);
    STEAM_CALLBACK(Workshop, OnDownloadItemResult, DownloadItemResult_t, cbDownloadItemResult_);
    STEAM_CALLBACK(Workshop, OnSubscribed, RemoteStoragePublishedFileSubscribed_t, cbSubscribed_);
    STEAM_CALLBACK(Workshop, OnUnsubscribed, RemoteStoragePublishedFileUnsubscribed_t, cbUnsubscribed_);

    // Internal helper
    void copyPk3From(const std::string& srcFolder, PublishedFileId_t id);

    // Callback handlers
    //void OnSubscribed(RemoteStoragePublishedFileSubscribed_t* p);
    //void OnUnsubscribed(RemoteStoragePublishedFileUnsubscribed_t* p);
    //void OnDownloadItemResult(DownloadItemResult_t* p);
    //void OnItemInstalled(ItemInstalled_t* p);

    // ==== PUBLISHING STATE ====
    struct Draft 
    {
        bool active = false;
        bool creating = false;
        EWorkshopFileType type = k_EWorkshopFileTypeCommunity;
        PublishedFileId_t targetId = k_PublishedFileIdInvalid;

        std::string folder;       // content root
        std::string title;
        std::string desc;
        std::string preview;      // file path
        std::vector<std::string> tags;
        ERemoteStoragePublishedFileVisibility visibility =
            k_ERemoteStoragePublishedFileVisibilityPublic;

        // runtime
        UGCUpdateHandle_t updateHandle = k_UGCUpdateHandleInvalid;
    } draft_;

    std::string pendingChangeNote_;

    // async results
    CCallResult<Workshop, CreateItemResult_t>        cbCreateItem_;
    CCallResult<Workshop, SubmitItemUpdateResult_t>  cbSubmitUpdate_;

    // helpers
    void ApplyDraftAndSubmit_();
    void OnCreateItem_(CreateItemResult_t* p, bool ioFailure);
    void OnSubmitItemUpdate_(SubmitItemUpdateResult_t* p, bool ioFailure);
};



// Visibility helper
inline ERemoteStoragePublishedFileVisibility WSParseVisibility(const std::string& v) {
    std::string s = v; for (auto& c : s) c = (char)tolower((unsigned char)c);
    if (s == "private")   return k_ERemoteStoragePublishedFileVisibilityPrivate;
    if (s == "friends")   return k_ERemoteStoragePublishedFileVisibilityFriendsOnly;
    if (s == "unlisted")  return k_ERemoteStoragePublishedFileVisibilityUnlisted;
    return k_ERemoteStoragePublishedFileVisibilityPublic; // default
}


