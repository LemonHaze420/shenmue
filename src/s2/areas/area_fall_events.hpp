#pragma once
#if defined(SHENMUE_EVENT_OVERLAY)
void QueueRockyAreaFallEvent(void);
#else

namespace shenmue::s2::areas {
void QueueRockyAreaFallEvent();
void QueueRoomFallEvent();
void UpdateShenhuaOcclusionAlpha();
void ResetAreaFallEvents();
} // namespace shenmue::s2::areas

#endif
