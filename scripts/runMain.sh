#!/bin/bash

# mkdir -p ../data

./../bin/main rtsp://root:akts@10.190.60.131/live.sdp vivotek_fd8136 &
./../bin/main rtsp://root:akts@10.190.60.113/live.sdp vivotek_fd8136 &
./../bin/main rtsp://root:akts@10.190.60.125/axis-media/media.amp axis_m1014 &
./../bin/main rtsp://root:akts@10.190.60.114/axis-media/media.amp axis_p5534 & #Domo!
./../bin/main rtsp://root:akts@10.190.60.100/axis-media/media.amp axis_q1755 &
./../bin/main rtsp://root:akts@10.190.60.119/axis-media/media.amp axis_p3364