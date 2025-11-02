#ifndef SAT_DISCOVERY_HANDLE_FRAMES_H
#define SAT_DISCOVERY_HANDLE_FRAMES_H

#include <sat_discovery.h>
#include <sat_discovery_frame.h>

void sat_discovery_handle_frame_announce (sat_discovery_t *const service, sat_discovery_frame_t frame);
void sat_discovery_handle_frame_heartbeat (sat_discovery_t *const service, sat_discovery_frame_t frame);
void sat_discovery_handle_frame_interest (sat_discovery_t *const service, sat_discovery_frame_t frame);
void sat_discovery_handle_frame_vanish (sat_discovery_t *const service, sat_discovery_frame_t frame);

#endif /* SAT_DISCOVERY_HANDLE_FRAMES_H */
