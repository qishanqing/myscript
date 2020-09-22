#!/bin/bash
if test ! -d "out/target/product/msm8953_64"; then
    ln -s $TARGET_PRODUCT out/target/product/msm8953_64
fi
