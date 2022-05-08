/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#pragma once

#include "SocketId.h"

namespace MinimalSocket {
// return port actually binded (as you could pass to the function also AnyPort)
Port bind(const SocketID &socket_id, const AddressFamily &family,
          const Port &port);

void listen(const SocketID &socket_id);

void connect(const SocketID &socket_id, const Address &remote_address);
} // namespace MinimalSocket
