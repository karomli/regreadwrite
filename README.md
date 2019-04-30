Sample Register Read Write tool
Usage:

Read:
sudo ./regreadwrite -r 0x<register_address>

Write:
sudo ./regreadwrite -w 0x<register_address> <value_to_written_to_register>

Build instruction:

Android:
cd $(ANDROID_TOP)/project/<this_project>
mm -jN

Linux:
mkdir build
cd build
cmake ../
make -jN


Dependencies:
Android:
libdrm
libpciaccess

Linux:
libdrm
libpciaccess
