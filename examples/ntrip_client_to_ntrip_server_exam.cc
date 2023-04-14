#include <stdint.h>

#include <string>
#include <vector>

#include "ntrip/ntrip_client.h"
#include "ntrip/ntrip_server.h"
#include "ntrip/ntrip_util.h"


namespace {

using libntrip::NtripClient;
using libntrip::NtripServer;


constexpr char kNtripString[] =
    "STR;RTCM32;RTCM32;RTCM 3.2;1004(1),1005/1007(5),PBS(10);"
    "2;GPS;SGNET;CHN;31;121;1;1;SGCAN;None;B;N;0;;";

}  // namespace

int main(void) {
  NtripServer ntrip_server;
  ntrip_server.Init("127.0.0.1", 8090, "test01", "123456",
      "RTCM32", kNtripString);
  NtripClient ntrip_client;
  ntrip_client.Init("127.0.0.1", 8090, "test01", "123456", "RTCM32");
  ntrip_client.OnReceived([&] (const char *buffer, const int &size) {
    printf("Recv[%d]: ", size);
    for (int i = 0; i < size; ++i) {
      printf("%02X ", static_cast<uint8_t>(buffer[i]));
    }
    printf("\n");
    // 转发到NtripServer.
    if (ntrip_server.service_is_running()) {
      ntrip_server.SendData(buffer, size);
    }
  });
  ntrip_client.set_location(22.57311, 113.94905);
  ntrip_client.set_report_interval(1);
  ntrip_client.Run();
  do {
    if (!ntrip_client.service_is_running()) {
      ntrip_client.Run();
    }
    if (!ntrip_server.service_is_running()) {
      ntrip_server.Run();
    }
    std::this_thread::sleep_for(std::chrono::seconds(3));
  } while (1);
  ntrip_server.Stop();
  ntrip_client.Stop();
  return 0;
}
