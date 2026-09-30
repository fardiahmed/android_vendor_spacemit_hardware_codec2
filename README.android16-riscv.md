# A16_RISCV/vendor/spacemit/hardware/codec2: android16-riscv

Changes made for the Android 16 (AOSP, riscv64) bring-up of the BananaPi BPI-F3 (SpacemiT K1) and the BananaPi BPI-SM10 (SpacemiT K3), on branch `android16-riscv`.

AIDL Codec2 service for the SpacemiT Linlon-V5 VPU (`android.hardware.media.c2-service.k1-v4l2`).

## Changes

- **codec2: add the Codec2 service for the SpacemiT VPU**: AIDL Codec2 service serving external/v4l2_codec2's V4L2 components (amvx VPU): H.264/HEVC/VP8/VP9 decode, H.264/VP8/VP9 encode.

## Notes

- The V4L2 components come from external/v4l2_codec2 (Soong namespace import). Enabled by `SPACEMIT_HW_CODEC2` in device/spacemit/common/spacemit-features.mk.

## Build

```
source build/envsetup.sh
lunch aosp_bananapi_f3_tablet trunk_staging userdebug   # BPI-F3 (K1)
lunch aosp_bananapi_sm10_tablet trunk_staging userdebug # BPI-SM10 (K3), not booted yet
m android.hardware.media.c2-service.k1-v4l2
```
