# Image-Processor

## Task
- Implement some basic visual effects on a given image and generate a image (with time, more effect may be added).
- Following are the effect :
  - Make the color inverted image
  - Make gaussian blurred image
  - Make ring blurred image
  - Add grayscale effect

    
## Goals
- [x] Load and save images (stb_image)
- [x] `img` struct: width, height, channels, raw pixel buffer
- [x] Color inversion
- [x] Gaussian blur (3x3, repeated passes)
- [x] Ring blur (3x3, center excluded)
- [x] Grayscale
- [ ] Brightness, threshold
- [ ] Timing and single-threaded baseline
- [ ] Multithreading
- [ ] SIMD
- [ ] Dithering, FFT
- [ ] Effect pipeline

## Build and run
g++ -std=c++17 -O2 -I third_party main.cpp -o imgproc

.\imgproc.exe

## Notes
- Image files are decoded into raw pixels (and encoded back) by stb_image
  and stb_image_write. The effects are written from scratch.
- Each image is an `img` struct holding `width`, `height`, `channels`
  (always 3 here: R, G, B) and `pixels`, a flat array of bytes.
- The byte for column `x`, row `y`, channel `c` is at index
  `(y * width + x) * channels + c`.

## Reference 
[Additional library](https://github.com/nothings/stb/) has be taken from the given repo.

