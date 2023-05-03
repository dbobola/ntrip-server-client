#include <stdint.h>

#include <string>
#include <vector>

#include "ntrip/ntrip_client.h"
#include "ntrip/ntrip_util.h"


using libntrip::NtripClient;

int main(void) {
  std::string ip = "16.16.107.177";
  int port = 8090;
  std::string user = "test01";
  std::string passwd = "123456";
  std::string mountpoint = "RTCM32";
  std::string uart_port = "/dev/ttyUSB0";
  int uart_baud = 460800;

  NtripClient ntrip_client;
  ntrip_client.Init(ip, port, user, passwd, mountpoint, uart_port, uart_baud);
  ntrip_client.OnReceived([] (const char *buffer, int size) {
    printf("Recv[%d]: ", size);
    for (int i = 0; i < size; ++i) {
      printf("%02X ", static_cast<uint8_t>(buffer[i]));
    }
    printf("\n");
  });
  // std::string gga;
  // if (libntrip::GetGGAFrameData(22.57311, 113.94905, 10.0, &gga) == 0) {
  //   printf("GGA buffer: %s\n", gga.c_str());
  //   ntrip_client.set_gga_buffer(gga);
  // }
  ntrip_client.set_location(22.57311, 113.94905);
  ntrip_client.set_report_interval(1);
  ntrip_client.Run();
  std::this_thread::sleep_for(std::chrono::seconds(1));  // Maybe take longer?
  while (ntrip_client.service_is_running()) {
    std::this_thread::sleep_for(std::chrono::seconds(1));
  }

  ntrip_client.Stop();
  return 0;
}
