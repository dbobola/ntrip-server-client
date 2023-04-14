
#ifndef NTRIPLIB_MOUNT_POINT_H_
#define NTRIPLIB_MOUNT_POINT_H_

#include <list>

namespace libntrip {

struct MountPointInformation {
  int server_fd;
  std::string mountpoint;
  std::string username;
  std::string password;
  std::list<int> client_socket_list;
};

}  // namespace libntrip

#endif  // NTRIPLIB_MOUNT_POINT_H_
