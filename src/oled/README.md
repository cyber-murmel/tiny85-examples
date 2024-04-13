1. download the [gif](https://events.ccc.de/media/37c3-cover-image.gif)

![](https://events.ccc.de/media/37c3-cover-image.gif)

```shell
wget https://events.ccc.de/media/37c3-cover-image.gif
mkdir png pbm bin
```

2. turn into images using ffmpeg

This crops the bolt cutter from the gif, resizes and pads it to fit 128x48 pixels, reduces the frame rate to 5 and dumps each frame as png.
```shell
ffmpeg \
  -i 37c3-cover-image.gif \
  -filter:v "crop=523:135:75:143,scale=iw*min(128/iw\,48/ih):ih*min(128/iw\,48/ih),pad=128:48:(128-iw)/2:(48-ih)/2" \
  -r 1.4 \
  png/37c3-cover-image_%03d.png
```

3. convert to pbm

```shell
for image in png/*
do
  name=$(basename "${image%.*}")
  convert png/$name.png -dither FloydSteinberg -remap pattern:gray50 pbm/$name.pbm
  tail -c +11 pbm/$name.pbm > bin/$name.bin
done
```

4. create binary with frames

```shell
cat bin/37c3-cover-image_001.bin bin/37c3-cover-image_002.bin bin/37c3-cover-image_003.bin bin/37c3-cover-image_004.bin bin/37c3-cover-image_005.bin > 37c3-cover-image.bin
```