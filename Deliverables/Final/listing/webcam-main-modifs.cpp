/* Set format to 320x240 and Motion JPEG */
    wbc.setFormat(320, 240, V4L2_PIX_FMT_UYVY);
/* Start stream and write to file */
    wbc.startStream("output.yuvy");
