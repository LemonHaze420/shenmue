#if defined(SHENMUE_EVENT_OVERLAY)
void AreaCleanup(int afternoon) {
#else
// @verified - p
void AreaQF01Cleanup(std::int32_t, std::uint32_t) {
#endif
    FTBL.EV_SetWeatherInstruction(0x0f, 0);
    FTBL.EV_SetWeatherInstruction(0x780, 0);
    FTBL.EV_SetWeatherInstruction(0x1800, 0);
    FTBL.SetMapModelDataDefaultVertexShader();
    FTBL.EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
    FTBL.PushGlobalLightingProgram();
    (void)FTBL.CheckIsArgEquZero(1, 1);
    FTBL.SetAreaFogParams();
#if defined(SHENMUE_EVENT_OVERLAY)
    AreaQF01LoadAudio(afternoon);
#endif
}

#if defined(SHENMUE_EVENT_OVERLAY)
void AreaQF01LoadAudio(int afternoon) {
}
#endif
