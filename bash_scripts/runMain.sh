#!/bin/bash

mkdir -p ../data

./../bin/main rtsp://root:akts@10.190.60.131/live.sdp vivotek_fd8136 &
./../bin/main rtsp://root:akts@10.190.60.105/live.sdp vivotek_fd8136 &
./../bin/main rtsp://root:akts@10.190.60.125/axis-media/media.amp axis_m1014