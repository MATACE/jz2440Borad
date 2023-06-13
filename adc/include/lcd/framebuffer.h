#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

void paint_pot(const unsigned int x, const unsigned int y, const unsigned int rgb);
unsigned short get_16bpp_rgb(const unsigned int rgb);
void fb_buf_clear(void);

#endif // FRAMEBUFFER_H

