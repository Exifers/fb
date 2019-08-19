#define _POSIX_C_SOURCE 200809

#include <stdio.h>
#include <sys/ioctl.h>
#include <linux/fb.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

int main(void) {
    int fd = open("/dev/dri/card0", O_RDONLY);
    if (!fd) {
        perror("open(\"/dev/dri/card0\")");
        return 1;
    }
    struct fb_fix_screeninfo si;
    int ret = ioctl(fd, FBIOGET_FSCREENINFO, &si);
    if (ret) {
        perror("ioctl(FBIOGET_FSCREENINFO)");
        return 1;
    }
    printf("Ioctl succeded\n");
    printf("%u\n", si.smem_len);
    printf("%u\n", si.line_length);
    printf("%lx\n", si.mmio_start);
    // on my computer this shows si.mmio_start = 0 certainly because I'm using
    // Nvidia proprietary drivers, thus calling the DRM is not the way to do it.
    return 0;
}
