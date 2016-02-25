#!/bin/bash

# mkdir -p ../data

./../bin/main rtsp://root:akts@10.190.60.131/live.sdp vivotek_fd8136 &
./../bin/main rtsp://root:akts@10.190.60.128/live.sdp vivotek_ip8337h-c &
./../bin/main rtsp://root:akts@10.190.60.105/axis-media/media.amp axis_m1014 &
./../bin/main rtsp://root:akts@10.190.60.127/axis-media/media.amp axis_p5534 & #Domo!
./../bin/main rtsp://root:akts@10.190.60.106/axis-media/media.amp axis_q1755 &
./../bin/main rtsp://root:akts@10.190.60.117/axis-media/media.amp axis_p3364