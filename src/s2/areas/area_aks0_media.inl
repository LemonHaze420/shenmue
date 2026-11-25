#if defined(SHENMUE_EVENT_OVERLAY)
void AreaCleanup(int afternoon)
#else
void ApplyAKS0Media()
#endif
{
#if !defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
#if defined(SHENMUE_EVENT_OVERLAY)
    FTBL.EV_LoadCutsceneFromCurrentScene("AKS0");
    if (!FTBL.EV_GetPlayerFlag(11, suppressAreaAudioPlayerFlag))
        FTBL.EV_LoadAudio("AKS0_000", 0);
#else
    EV_LoadCutsceneFromCurrentScene("AKS0");
    if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, suppressAreaAudioPlayerFlag) == 0)
        EV_LoadAudio("AKS0_000", 0);
#endif
#endif
}
