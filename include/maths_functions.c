
/* HSV to RGB conversion function with only integer
 * math */
//---------------------------------------------------------------------------------
uint16_t hsl2rgb(unsigned char hue, unsigned char sat, unsigned char lum) {
//---------------------------------------------------------------------------------
	int v;

	v = (lum < 128) ? (lum * (256 + sat)) >> 8 :
		(((lum + sat) << 8) - lum * sat) >> 8;
    if (v <= 0) {
		return RGB8(0,0,0);
	} else {
		int m;
		int sextant;
		int fract, vsf, mid1, mid2;

		m = lum + lum - v;
		hue *= 6;
		sextant = hue >> 8;
		fract = hue - (sextant << 8);
		vsf = v * fract * (v - m) / v >> 8;
		mid1 = m + vsf;
		mid2 = v - vsf;
		switch (sextant) {
			case 0: return RGB8(v,mid1,m); break;
			case 1: return RGB8(mid2,v,m); break;
			case 2: return RGB8(m,v,mid1); break;
			case 3: return RGB8(m,mid2,v); break;
			case 4: return RGB8(mid1,m,v); break;
			default: return RGB8(v,m,mid2); break;
		}
	}
}