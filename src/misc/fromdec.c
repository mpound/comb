void decdouble(double *dp, int n)
{
        unsigned int de, df;
        unsigned int lmask = 0x00ff0000, rmask = 0x0000ff00;

        for(; n--; dp++) {
                de = *(int *)dp;
                df = ((int *)dp)[1];
                de = (de >> 24) | (de << 24) | ((de << 8) & lmask) |
                        ((de >> 8) & rmask);
                df = (df >> 24) | (df << 24) | ((df << 8) & lmask) |
                        ((df >> 8) & rmask);
                        *(int *)dp = df;
                        ((int *)dp)[1] = de;
        }
}

void decfloat(float *fp, int n)
{
        unsigned int t;
        unsigned int lmask = 0x00ff0000, rmask = 0x0000ff00;

        for(; n--; fp++) {
                t = *(int *)fp;
                t = (t >> 24) | (t << 24) | ((t << 8) & lmask) |
                        ((t >> 8) & rmask);
                        *(int *)fp = t;
        }
}
void declong(unsigned int *lp, int n)
{
        unsigned int t;
        unsigned int lmask = 0x00ff0000, rmask = 0x0000ff00;

        for(; n--; lp++) {
                t = *lp;
                *lp = (t >> 24) | (t << 24) | ((t << 8) & lmask) |
                        ((t >> 8) & rmask);
        }
}

void decshort(unsigned short *sp, int n)
{
        unsigned int t;
        unsigned int lmask = 0xff00, rmask = 0x00ff;

        for(; n--; sp++) {
                t = *sp;
                *sp = ((t << 8) & lmask) | ((t >> 8) & rmask);
        }
}

