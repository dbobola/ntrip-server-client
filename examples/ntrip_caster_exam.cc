#include "ntrip/ntrip_caster.h"


using libntrip::NtripCaster;

int main(int argc, char *argv[]) {
  NtripCaster ntrip_caster;
  ntrip_caster.Init(8090, 30, 2000);
  // ntrip_caster.Init("127.0.0.1", 8090, 10, 2000);
  ntrip_caster.Run();
  std::this_thread::sleep_for(std::chrono::seconds(1));  // Maybe take longer?
  while (ntrip_caster.service_is_running()) {
    // TODO(mengyuming@hotmail.com) : Add your code in here.
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }
  ntrip_caster.Stop();
  return 0;
}
