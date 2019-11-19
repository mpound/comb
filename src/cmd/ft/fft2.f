$ forty form
c fft842     fast fourier transform for n=2**m
c
c       this program replaces the vector z=x+iy by its  finite
c   discrete, complex fourier transform if in=0.  the inverse transform
c   is calculated for in=1.  it performs as many base
c   8 iterations as possible and then finishes with a base 4 iteration
c   or a base 2 iteration if needed.
c
c       the subroutine is called as subroutine fft842 (in,m,x,y).
c   the integer m (where n=2**m), the n real location array x, and
c   the n real location array y must be supplied to the subroutine.
c
c
c
      subroutine fft842(in,n2pow,x,y)
      dimension x(2),y(2),l(15)
      equivalence(l15,l(1)),(l14,l(2)),(l13,l(3)),(l12,l(4)),(l11,l(5)),
     1(l10,l(6)),(l9,l(7)),(l8,l(8)),(l7,l(9)),(l6,l(10)),(l5,l(11)),(l4
     2,l(12)),(l3,l(13)),(l2,l(14)),(l1,l(15))
      nthpo=2**n2pow
      fn=nthpo
      if(in.eq.1)goto10
      do 2 i=1,nthpo
    2 y(i)=-y(i)
   10 n8pow=n2pow/3
      if(n8pow.eq.0)goto3
c
c     radix 8 passes,if any.
c
   17 do 1 ipass=1,n8pow
      nxtlt =2**(n2pow-3*ipass)
      lengt =8*nxtlt
      call r8tx(nxtlt,nthpo,lengt,x(1),x(nxtlt+1),x(2*nxtlt+1),
     1  x(3*nxtlt+1),x(4*nxtlt+1),x(5*nxtlt+1),x(6*nxtlt+1),
     2  x(7*nxtlt+1),y(1),y(nxtlt+1),y(2*nxtlt+1),y(3*nxtlt+1),
     3  y(4*nxtlt+1),y(5*nxtlt+1),y(6*nxtlt+1),y(7*nxtlt+1))
    1 continue
c
c     is there a four factor left
c
    3 if (n2pow-3*n8pow-1)5,6,7
c
c     go through the base 2 iteration
c
c
    6 call r2tx(nthpo,x(1),x(2),y(1),y(2))
      go to 5
c     go through the base 4 iteration
c
    7 call r4tx(nthpo,x(1),x(2),x(3),x(4),y(1),y(2),y(3),y(4))
c
    5 do 51 j=1,15
      l(j)=1
      if(j-n2pow) 19,19,51
   19 l(j)=2**(n2pow+1-j)
   51 continue
      ij=1
      do 8 j1=1,l1
      do 8 j2=j1,l2,l1
      do 8 j3=j2,l3,l2
      do 8 j4=j3,l4,l3
      do 8 j5=j4,l5,l4
      do 8 j6=j5,l6,l5
      do 8 j7=j6,l7,l6
      do 8 j8=j7,l8,l7
      do 8 j9=j8,l9,l8
      do 8 j10=j9,l10,l9
      do 8 j11=j10,l11,l10
      do 8 j12=j11,l12,l11
      do 8 j13=j12,l13,l12
      do 8 j14=j13,l14,l13
      do 8 ji=j14,l15,l14
      if (ij-ji) 20,8,8
   20 r=x(ij)
      x(ij)=x(ji)
      x(ji)=r
      fi=y(ij)
      y(ij)=y(ji)
      y(ji)=fi
    8 ij=ij+1
      if(in.eq.1)goto56
      do 50 i=1,nthpo
   50 y(i)=-y(i)
      goto55
   56 do 57 i=1,nthpo
      x(i)=x(i)/fn
   57 y(i)=y(i)/fn
   55 return
      end
c     *********************************************
c r2tx       radix 2 iteration subroutine
c
      subroutine r2tx(nthpo,cr0,cr1,ci0,ci1)
      dimension cr0(2),cr1(2),ci0(2),ci1(2)
      do 61 k=1,nthpo,2
      r1=cr0(k)+cr1(k)
      cr1(k)=cr0(k)-cr1(k)
      cr0(k)=r1
      fi1=ci0(k)+ci1(k)
      ci1(k)=ci0(k)-ci1(k)
   61 ci0(k)=fi1
      return
      end
c     *********************************************
c r4tx       radix 4 iteration subroutine
c
      subroutine r4tx(nthpo,cr0,cr1,cr2,cr3,ci0,ci1,ci2,ci3)
      dimension cr0(2),cr1(2),cr2(2),cr3(2),ci0(2),ci1(2),ci2(2),ci3(2)
      do 71 k=1,nthpo,4
      r1=cr0(k)+cr2(k)
      r2=cr0(k)-cr2(k)
      r3=cr1(k)+cr3(k)
      r4=cr1(k)-cr3(k)
      fi1=ci0(k)+ci2(k)
      fi2=ci0(k)-ci2(k)
      fi3=ci1(k)+ci3(k)
      fi4=ci1(k)-ci3(k)
      cr0(k)=r1+r3
      ci0(k)=fi1+fi3
      cr1(k)=r1-r3
      ci1(k)=fi1-fi3
      cr2(k)=r2-fi4
      ci2(k)=fi2+r4
      cr3(k)=r2+fi4
   71 ci3(k)=fi2-r4
      return
      end
c     *********************************************
c r8tx       radix 8 iteration subroutine
c
      subroutine r8tx(nxtlt,nthpo,lengt,cr0,cr1,cr2,cr3,cr4,cr5,cr6,cr7,
     1           ci0,ci1,ci2,ci3,ci4,ci5,ci6,ci7)
      dimension cr0(2),cr1(2),cr2(2),cr3(2),cr4(2),cr5(2),cr6(2),
     1            cr7(2),ci1(2),ci2(2),ci3(2),ci4(2),ci5(2),ci6(2),
     2            ci7(2),ci0(2)
      scale=6.283185307/float(lengt)
      do 1 j=1,nxtlt
      arg=float(j-1)*scale
      c1=cos(arg)
      s1=sin(arg)
      c2=c1**2-s1**2
      s2=c1*s1+c1*s1
      c3=c1*c2-s1*s2
      s3=c2*s1+s2*c1
      c4=c2**2-s2**2
      s4=c2*s2+c2*s2
      c5=c2*c3-s2*s3
      s5=c3*s2+s3*c2
      c6=c3**2-s3**2
      s6=c3*s3+c3*s3
      c7=c3*c4-s3*s4
      s7=c4*s3+s4*c3
      do 1 k=j,nthpo,lengt
      ar0=cr0(k)+cr4(k)
      ar1=cr1(k)+cr5(k)
      ar2=cr2(k)+cr6(k)
      ar3=cr3(k)+cr7(k)
      ar4=cr0(k)-cr4(k)
      ar5=cr1(k)-cr5(k)
      ar6=cr2(k)-cr6(k)
      ar7=cr3(k)-cr7(k)
      ai0=ci0(k)+ci4(k)
      ai1=ci1(k)+ci5(k)
      ai2=ci2(k)+ci6(k)
      ai3=ci3(k)+ci7(k)
      ai4=ci0(k)-ci4(k)
      ai5=ci1(k)-ci5(k)
      ai6=ci2(k)-ci6(k)
      ai7=ci3(k)-ci7(k)
      br0=ar0+ar2
      br1=ar1+ar3
      br2=ar0-ar2
      br3=ar1-ar3
      br4=ar4-ai6
      br5=ar5-ai7
      br6=ar4+ai6
      br7=ar5+ai7
      bi0=ai0+ai2
      bi1=ai1+ai3
      bi2=ai0-ai2
      bi3=ai1-ai3
      bi4=ai4+ar6
      bi5=ai5+ar7
      bi6=ai4-ar6
      bi7=ai5-ar7
      cr0(k)=br0+br1
      ci0(k)=bi0+bi1
      if(j.le.1)goto2
      cr1(k)=c4*(br0-br1)-s4*(bi0-bi1)
      ci1(k)=c4*(bi0-bi1)+s4*(br0-br1)
      cr2(k)=c2*(br2-bi3)-s2*(bi2+br3)
      ci2(k)=c2*(bi2+br3)+s2*(br2-bi3)
      cr3(k)=c6*(br2+bi3)-s6*(bi2-br3)
      ci3(k)=c6*(bi2-br3)+s6*(br2+bi3)
      tr=0.7071067812*(br5-bi5)
      ti=0.7071067812*(br5+bi5)
      cr4(k)=c1*(br4+tr)-s1*(bi4+ti)
      ci4(k)=c1*(bi4+ti)+s1*(br4+tr)
      cr5(k)=c5*(br4-tr)-s5*(bi4-ti)
      ci5(k)=c5*(bi4-ti)+s5*(br4-tr)
      tr=-0.7071067812*(br7+bi7)
      ti= 0.7071067812*(br7-bi7)
      cr6(k)=c3*(br6+tr)-s3*(bi6+ti)
      ci6(k)=c3*(bi6+ti)+s3*(br6+tr)
      cr7(k)=c7*(br6-tr)-s7*(bi6-ti)
      ci7(k)=c7*(bi6-ti)+s7*(br6-tr)
      go to 1
    2 cr1(k)=br0-br1
      ci1(k)=bi0-bi1
      cr2(k)=br2-bi3
      ci2(k)=bi2+br3
      cr3(k)=br2+bi3
      ci3(k)=bi2-br3
      tr=0.7071067812*(br5-bi5)
      ti=0.7071067812*(br5+bi5)
      cr4(k)=br4+tr
      ci4(k)=bi4+ti
      cr5(k)=br4-tr
      ci5(k)=bi4-ti
      tr=-0.7071067812*(br7+bi7)
      ti=0.7071067812*(br7-bi7)
      cr6(k)=br6+tr
      ci6(k)=bi6+ti
      cr7(k)=br6-tr
      ci7(k)=bi6-ti
    1 continue
      return
      end
