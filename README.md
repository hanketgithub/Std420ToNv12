# 420ToNv12
Converts YUV 420 to NV12 in pipeline.

Sample usage:

ffmpeg -i test.265 -c:v rawvideo -pix_fmt yuv420p -f rawvideo - | \
./420ToNv12 1280 720 > test.yuv

