#pragma once

#include "session.h"

#include <vector>
#include <cstdint>

void packet_processor(std::shared_ptr<Session> session, const std::vector<uint8_t>& packet);

void check_dm(std::shared_ptr<Session> session);