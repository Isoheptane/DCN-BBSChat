#pragma once

#include "SocketW.h"

using WinSock::SocketW;

void startListener(SocketW listener);

void connectionHandler(SocketW sockw);