#pragma once

#include <cstdio>
#include <cstdint>
#include <iostream>
#include <string>
#include <deque>
#include <thread>

#include "SecConn.h"
#include "packet_queue.h"

void connection_operator(SecConn conn, std::shared_ptr<PacketQueue> queue);