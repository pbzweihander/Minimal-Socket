/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#include <MinimalSocket/Error.h>
#include <MinimalSocket/udp/UdpSocket.h>

#include "../SocketFunctions.h"
#include "../Utils.h"

namespace MinimalSocket::udp {
UdpSender::UdpSender(const AddressFamily &accepted_connection_family)
    : RemoteAddressFamilyAware(accepted_connection_family) {
  getIDWrapper().reset(UDP, accepted_connection_family);
}

UdpSender::UdpSender(UdpSender &&o)
    : RemoteAddressFamilyAware(o.getRemoteAddressFamily()) {
  Socket::transfer(*this, o);
}
UdpSender &UdpSender::operator=(UdpSender &&o) {
  copy_as<RemoteAddressFamilyAware>(*this, o);
  Socket::transfer(*this, o);
  return *this;
}

UdpBindable UdpSender::bind(const Port port_to_bind) {
  UdpBindable result(port_to_bind, getRemoteAddressFamily());
  result.open();
  return std::move(result);
}

UdpBindable::UdpBindable(const Port port_to_bind,
                         const AddressFamily &accepted_connection_family)
    : PortToBindAware(port_to_bind),
      RemoteAddressFamilyAware(accepted_connection_family) {}

UdpBindable::UdpBindable(UdpBindable &&o)
    : PortToBindAware(o), RemoteAddressFamilyAware(o) {
  Openable::transfer(*this, o);
}
UdpBindable &UdpBindable::operator=(UdpBindable &&o) {
  copy_as<PortToBindAware>(*this, o);
  copy_as<RemoteAddressFamilyAware>(*this, o);
  Openable::transfer(*this, o);
  return *this;
}

void UdpBindable::open_() {
  getIDWrapper().reset(UDP, getRemoteAddressFamily());
  auto binded_port = MinimalSocket::bind(
      getIDWrapper().accessId(), getRemoteAddressFamily(), getPortToBind());
  setPort(binded_port);
}

UdpConnectable UdpBindable::connect(const Address &remote_address) {
  if (remote_address.getFamily() != getRemoteAddressFamily()) {
    throw Error{"Passed address has invalid family"};
  }
  UdpConnectable result(remote_address, getPortToBind());
  if (wasOpened()) {
    MinimalSocket::connect(getIDWrapper().accessId(), remote_address);
  }
  Openable::transfer(result, *this);
  return std::move(result);
}

std::optional<UdpConnectable> UdpBindable::connect(const Timeout &timeout) {
  auto maybe_received = this->receive(MAX_UDP_RECV_MESSAGE, timeout);
  if (!maybe_received) {
    return std::nullopt;
  }
  return connect(maybe_received->sender);
}

UdpConnectable::UdpConnectable(const Address &remote_address, const Port &port)
    : PortToBindAware(port), RemoteAddressAware(remote_address) {}

UdpConnectable::UdpConnectable(UdpConnectable &&o)
    : PortToBindAware(o), RemoteAddressAware(o) {
  Openable::transfer(*this, o);
}
UdpConnectable &UdpConnectable::operator=(UdpConnectable &&o) {
  copy_as<PortToBindAware>(*this, o);
  copy_as<RemoteAddressAware>(*this, o);
  Openable::transfer(*this, o);
  return *this;
}

void UdpConnectable::open_() {
  const auto &socket_id = getIDWrapper().accessId();
  const auto &remote_address = getRemoteAddress();
  getIDWrapper().reset(UDP, remote_address.getFamily());
  auto binded_port = MinimalSocket::bind(socket_id, remote_address.getFamily(),
                                         getPortToBind());
  setPort(binded_port);
  MinimalSocket::connect(socket_id, remote_address);
}

UdpBindable UdpConnectable::disconnect() {
  resetIDWrapper();
  UdpBindable result(getPortToBind(), getRemoteAddress().getFamily());
  result.open();
  return std::move(result);
}
} // namespace MinimalSocket::udp
