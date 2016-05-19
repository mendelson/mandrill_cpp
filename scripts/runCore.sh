#!/bin/bash

# mkdir -p ../data

# sudo arp-scan --interface=eth0 --localnet

# valgrind bin/MandrillCore rtsp://root:akts@10.190.60.131/live.sdp vivotek_fd8136 #&
# bin/MandrillCore rtsp://root:akts@10.190.60.107/live.sdp vivotek_fd8136 #&
# bin/MandrillCore rtsp://root:akts@10.190.60.128/live.sdp vivotek_ip8337h-c #&
# bin/MandrillCore rtsp://root:akts@10.190.60.105/axis-media/media.amp axis_m1014 &
# bin/MandrillCore rtsp://root:akts@10.190.60.101/axis-media/media.amp axis_p5534 & #PTZ!
# bin/MandrillCore rtsp://root:akts@10.190.60.106/axis-media/media.amp axis_q1755 #&
# bin/MandrillCore rtsp://root:akts@10.190.60.117/live.sdp vivotek_fd8136
# bin/MandrillCore rtsp://root:akts@10.190.60.117/axis-media/media.amp axis_p3364
bin/MandrillCore rtsp://root:akts@10.190.60.102/live.sdp VIVOTEK_FD8136
# gdb --args bin/MandrillCore rtsp://root:akts@10.190.60.102/live.sdp VIVOTEK_FD8136
# bin/MandrillCore "rtsp://admin:admin@10.190.60.130:554/cam/realmonitor?channel=1&subtype=0" "INTELBRAS"


# 10.190.60.102 VIVOTEK FD8136 MENDI 01
# 10.190.60.109 VIVOTEK IP8337H MENDI 02
# 10.190.60.114 AXIS AXIS M1014
# 10.190.60.108 IntelBras VIP-S3120
# 10.190.60.115 AXIS AXIS Q1755
# 10.190.60.123 GIGANTONA
