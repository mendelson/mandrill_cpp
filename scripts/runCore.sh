#!/bin/bash

# mkdir -p ../data

# sudo arp-scan --interface=eth0 --localnet

#valgrind ./../bin/MandrillCore rtsp://root:akts@10.190.60.131/live.sdp vivotek_fd8136 #&
./../bin/MandrillCore rtsp://root:akts@10.190.60.107/live.sdp vivotek_fd8136 #&
# ./../bin/MandrillCore rtsp://root:akts@10.190.60.128/live.sdp vivotek_ip8337h-c &
# ./../bin/MandrillCore rtsp://root:akts@10.190.60.105/axis-media/media.amp axis_m1014 &
# ./../bin/MandrillCore rtsp://root:akts@10.190.60.101/axis-media/media.amp axis_p5534 & #PTZ!
# ./../bin/MandrillCore rtsp://root:akts@10.190.60.106/axis-media/media.amp axis_q1755 &
# ./../bin/MandrillCore rtsp://root:akts@10.190.60.117/axis-media/media.amp axis_p3364