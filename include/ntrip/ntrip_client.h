#ifndef NTRIPLIB_NTRIP_CLIENT_H_
#define NTRIPLIB_NTRIP_CLIENT_H_

#if defined(WIN32) || defined(_WIN32)
#include <winsock2.h>
#endif  // defined(WIN32) || defined(_WIN32)

#include <atomic>
#include <string>
#include <thread>  // NOLINT.
#include <functional>

#include "./thread_raii.h"


namespace libntrip {

using ClientCallback = std::function<void (char const* _buffer, int _size)>;

class NtripClient {
 public:
  NtripClient() = default;
  NtripClient(NtripClient const&) = delete;
  NtripClient(NtripClient&&) = delete;
  NtripClient& operator=(NtripClient const&) = delete;
  NtripClient& operator=(NtripClient&&) = delete;

  NtripClient(std::string const& ip, int port,
      std::string const& user, std::string const& passwd,
      std::string const& mountpoint, std::string const& uart_port, int uart_baud) :
        server_ip_(ip), server_port_(port),
        user_(user), passwd_(passwd),
        mountpoint_(mountpoint),
        uart_port_(uart_port),
        uart_baud_(uart_baud) { }
  ~NtripClient() { Stop(); }


  void Init(std::string const& ip, int port,
      std::string const& user, std::string const& passwd,
      std::string const& mountpoint, std::string const& uart_port, int uart_baud) {
    server_ip_ = ip;
    server_port_ = port;
    user_ = user;
    passwd_ = passwd;
    mountpoint_ = mountpoint;
    uart_port_ = uart_port;
    uart_baud_ = uart_baud;

  }
  // Update the sent GGA statement.
// According to the requirements of the NTRIP account, if the distance from the server location is too far, the server will not return differential data.
  void set_gga_buffer(std::string const& gga_buffer) {
    gga_buffer_ = gga_buffer;
    gga_is_update_.store(true);
  }
  // Set a fixed position coordinate to automatically generate GGA data from it.
  void set_location(double latitude, double longitude) {
    latitude_ = latitude;
    longitude_ = longitude;
  }
  // Set the GGA reporting interval in seconds.
  void set_report_interval(int intv) {
    report_interval_ = intv;
  }

  // Set the callback function to be executed when data is received.
  void OnReceived(const ClientCallback &callback) { callback_ = callback; }
  bool Run(void);
  void Stop(void);
  bool service_is_running(void) const {
    return service_is_running_.load();
  }

 private:
  // Thread handler.
  void ThreadHandler(void);

  std::atomic_bool service_is_running_ = {false};
  std::atomic_bool gga_is_update_ = {false};  // Flag to indicate external update of GGA data.
  int report_interval_ = 1;  // GGA data reporting interval.
  double latitude_ = 22.570535;  // Fixed latitude coordinate.
  double longitude_ = 113.937739;  // Fixed longtitude coordinate.
  std::string server_ip_;
  int server_port_ = 8090;
  std::string user_;
  std::string passwd_;
  std::string mountpoint_;
  std::string gga_buffer_;
  std::string uart_port;
  int uart_baud;
#if defined(WIN32) || defined(_WIN32)
  SOCKET socket_fd_ = INVALID_SOCKET;
#else
  int socket_fd_ = -1;
#endif  // defined(WIN32) || defined(_WIN32)
  Thread thread_;
  ClientCallback callback_ = [] (char const*, int) -> void {};
};

}  // namespace libntrip

#endif  // NTRIPLIB_NTRIP_CLIENT_H_
