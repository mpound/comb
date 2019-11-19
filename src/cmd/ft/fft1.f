c fast       fourier analysis subroutine - fast (b,n)
c
c         this subroutine replaces the real vector b(k), for
c     k=1,2,...,n, with its finite discrete fourier transform.  the dc
c     term is returned in location b(1) with b(2) set to 0.  thereafter
c     the jth harmonic is returned as a complex
c     number stored as  b(2*j+1) + i b(2*j+2).
c     the n/2 harmonic is returned in b(n+1) with b(n+2) set to 0.  hence,
c     b must be dimensioned to size n+2.
c         the subroutine is called as  fast(b,n) where n=2**m and
c     b is the real array described above.
c
      subroutine fast (b,n)
        dimension b(2)
c
c     iw is a machine dependent write device number
      iw=6
c
       do 5 i=1,15
       m=i
       nt = 2**i
       if ( n.eq.nt ) goto 7
    5  continue
       write(iw,100)
  100  format(33h n is not a power of two for fast)
       stop
    7     n4pow=m/2
c
c       do a radix 2 iteration first if one is required.
c
          if (m-n4pow*2) 20,20,10
   10     nn=2
          int=n/nn
          call fr2tr(int,b(1),b(int+1))
          go to 35
   20     nn=1
c
c       perform radix 4 iterations.
c
   35     if(n4pow.eq.0)goto 60
   40     do 50 it=1,n4pow
            nn=nn*4
            int=n/nn
            call fr4tr(int,nn,b(1),b(int+1),b(2*int+1),b(3*int+1),
     1                b(1),b(int+1),b(2*int+1),b(3*int+1))
   50     continue
c
c       perform in-place reordering.
c
   60     call ford1(m,b)
          call ford2(m,b)
       t = b(2)
       b(2) = 0.
       b(n+1) = t
       b(n+2) = 0.
       do 65 it=4,n,2
   65  b(it)=-b(it)
   70     return
                 end
c     *********************************************
c fsst       fourier synthesis subroutine - fsst (b,n)
c
c         this subroutine synthesizes the real vector b(k), for
c     k=1,2,...,n, from the fourier coefficients stored in the
c     b array of size n+2.  the dc term is in b(1) with b(2) equal to 0.
c     the jth harmonic is stored as b(2*j+1) + i b(2*j+2).
c     the n/2 harmonic is in b(n+1) with b(n+2) equal to 0.
c         the subroutine is called as fsst(b,n) where n=2**m and
c     b is the real array discussed above.
c
c
      subroutine  fsst (b,n)
        dimension b(2)
c
c     iw is a machine dependent write device number
      iw=6
c
       do 4 i=1,15
       m=i
       nt = 2**i
       if(n.eq.nt) goto 7
    4  continue
       write(iw,100)
  100  format(33h n is not a power of two for fsst)
       stop
    7  b(2) = b(n+1)
       do 8 i=4,n,2
       b(i) = -b(i)
    8  continue
c      scale the input by n
       do 5 i=1,n
    5  b(i)=b(i)/float(n)
          n4pow=m/2
c
c       scramble the inputs
c
          call ford2(m,b)
          call ford1(m,b)
c
          if(n4pow.eq.0)goto15
          nn=4*n
          do 10 it=1,n4pow
            nn=nn/4
            int=n/nn
            call fr4syn(int,nn,b(1),b(int+1),b(2*int+1),b(3*int+1),
     1                b(1),b(int+1),b(2*int+1),b(3*int+1))
   10     continue
c
c       do a radix 2 iteration if one is required
c
   15     if(m-n4pow*2) 30,30,20
   20     int=n/2
          call fr2tr(int,b(1),b(int+1))
   30 return
      end
c     *********************************************
c fr2tr       radix 2 iteration subroutine
c
      subroutine fr2tr(int,b0,b1)
        dimension b0(2),b1(2)
        do 100 k=1,int
          t=b0(k)+b1(k)
          b1(k)=b0(k)-b1(k)
          b0(k)=t
  100   continue
      return
      end
c     *********************************************
c fr4tr       radix 4 iteration subroutine
c
      subroutine fr4tr(int,nn,b0,b1,b2,b3,b4,b5,b6,b7)
        dimension l(15),b0(2),b1(2),b2(2),b3(2),b4(2),b5(2),b6(2),b7(2)
        equivalence   (l15,l(1)),(l14,l(2)),(l13,l(3)),(l12,l(4)),
     1  (l11,l(5)),(l10,l(6)),(l9,l(7)),(l8,l(8)),(l7,l(9)),(l6,
     2  l(10)),(l5,l(11)),(l4,l(12)),(l3,l(13)),(l2,l(14)),(l1,l(15))
c
c       jthet is a reversed binary counter, jr steps two at a time to
c       locate the real parts of intermediate results, and ji locates
c       the imaginary part corresponding to jr.
c
          l(1)=nn/4
          do 10 k=2,15
            if(l(k-1)-2) 11,12,13
   11       l(k-1)=2
   12       l(k)=2
            go to 10
   13       l(k)=l(k-1)/2
   10     continue
c
          piovn=3.141592654/float(nn)
          p7=0.7071067812
          ji=3
          jl=2
          jr=2
c
          do 70 j1=2,l1,2
          do 70 j2=j1,l2,l1
          do 70 j3=j2,l3,l2
          do 70 j4=j3,l4,l3
          do 70 j5=j4,l5,l4
          do 70 j6=j5,l6,l5
          do 70 j7=j6,l7,l6
          do 70 j8=j7,l8,l7
          do 70 j9=j8,l9,l8
          do 70 j10=j9,l10,l9
          do 70 j11=j10,l11,l10
          do 70 j12=j11,l12,l11
          do 70 j13=j12,l13,l12
          do 70 j14=j13,l14,l13
          do 70 jthet=j14,l15,l14
            th2=jthet-2
            if(th2) 71,71,76
   71       do 72 k=1,int
              t0=b0(k)+b2(k)
              t1=b1(k)+b3(k)
              b2(k)=b0(k)-b2(k)
              b3(k)=b1(k)-b3(k)
              b0(k)=t0+t1
              b1(k)=t0-t1
   72       continue
c
            if(nn-4) 70,70,73
   73       k0=int*4+1
            kl=k0+int-1
            do 75 k=k0,kl
              pr=p7*(b1(k)-b3(k))
              pi=p7*(b1(k)+b3(k))
              b3(k)=b2(k)+pi
              b1(k)=pi-b2(k)
              b2(k)=b0(k)-pr
              b0(k)=b0(k)+pr
   75       continue
            go to 70
c
   76       arg=th2*piovn
            c1=cos(arg)
            s1=sin(arg)
            c2=c1**2-s1**2
            s2=c1*s1+c1*s1
            c3=c1*c2-s1*s2
            s3=c2*s1+s2*c1
c
            int4=int*4
            j0=jr*int4+1
            k0=ji*int4+1
            jlast=j0+int-1
            do 77 j=j0,jlast
              ks=k0+j-j0
            do 77 k=ks,ks
              r1=b1(j)*c1-b5(k)*s1
              r5=b1(j)*s1+b5(k)*c1
              t2=b2(j)*c2-b6(k)*s2
              t6=b2(j)*s2+b6(k)*c2
              t3=b3(j)*c3-b7(k)*s3
              t7=b3(j)*s3+b7(k)*c3
              t0=b0(j)+t2
              t4=b4(k)+t6
              t2=b0(j)-t2
              t6=b4(k)-t6
              t1=r1+t3
              t5=r5+t7
              t3=r1-t3
              t7=r5-t7
              b0(j)=t0+t1
              b7(k)=t4+t5
              b6(k)=t0-t1
              b1(j)=t5-t4
              b2(j)=t2-t7
              b5(k)=t6+t3
              b4(k)=t2+t7
              b3(j)=t3-t6
   77       continue
c
            jr=jr+2
            ji=ji-2
            if(ji-jl) 78,78,70
   78       ji=2*jr-1
            jl=jr
   70     continue
      return
      end
c     *********************************************
c fr4syn      radix 4 synthesis
c
      subroutine fr4syn(int,nn,b0,b1,b2,b3,b4,b5,b6,b7)
        dimension l(15),b0(2),b1(2),b2(2),b3(2),b4(2),b5(2),b6(2),b7(2)
        equivalence   (l15,l(1)),(l14,l(2)),(l13,l(3)),(l12,l(4)),
     1  (l11,l(5)),(l10,l(6)),(l9,l(7)),(l8,l(8)),(l7,l(9)),(l6,l(10)),
     2  (l5,l(11)),(l4,l(12)),(l3,l(13)),(l2,l(14)),(l1,l(15))
c
          l(1)=nn/4
          do 10 k=2,15
            if(l(k-1)-2) 11,12,13
   11       l(k-1)=2
   12       l(k)=2
            go to 10
   13       l(k)=l(k-1)/2
   10     continue
c
          piovn=3.141592654/float(nn)
          p7=0.7071067812
          ji=3
          jl=2
          jr=2
c
          do 70 j1=2,l1,2
          do 70 j2=j1,l2,l1
          do 70 j3=j2,l3,l2
          do 70 j4=j3,l4,l3
          do 70 j5=j4,l5,l4
          do 70 j6=j5,l6,l5
          do 70 j7=j6,l7,l6
          do 70 j8=j7,l8,l7
          do 70 j9=j8,l9,l8
          do 70 j10=j9,l10,l9
          do 70 j11=j10,l11,l10
          do 70 j12=j11,l12,l11
          do 70 j13=j12,l13,l12
          do 70 j14=j13,l14,l13
          do 70 jthet=j14,l15,l14
            th2=jthet-2
            if(th2) 71,71,76
   71       do 72 k=1,int
              t0=b0(k)+b1(k)
              t1=b0(k)-b1(k)
              t2=b2(k)*2.0
              t3=b3(k)*2.0
              b0(k)=t0+t2
              b2(k)=t0-t2
              b1(k)=t1+t3
              b3(k)=t1-t3
   72       continue
c
            if(nn-4) 70,70,73
   73       k0=int*4+1
            kl=k0+int-1
            do 75 k=k0,kl
              t2=b0(k)-b2(k)
              t3=b1(k)+b3(k)
              b0(k)=(b0(k)+b2(k))*2.0
              b2(k)=(b3(k)-b1(k))*2.0
              b1(k)=(t2+t3)*1.414213562
              b3(k)=(t3-t2)*1.414213562
   75       continue
            go to 70
   76         arg=th2*piovn
              c1=cos(arg)
              s1=-sin(arg)
              c2=c1**2-s1**2
              s2=c1*s1+c1*s1
              c3=c1*c2-s1*s2
              s3=c2*s1+s2*c1
c
            int4=int*4
            j0=jr*int4+1
            k0=ji*int4+1
            jlast=j0+int-1
            do 77 j=j0,jlast
                ks=k0+j-j0
              do 77 k=ks,ks
                t0=b0(j)+b6(k)
                t1=b7(k)-b1(j)
                t2=b0(j)-b6(k)
                t3=b7(k)+b1(j)
                t4=b2(j)+b4(k)
                t5=b5(k)-b3(j)
                t6=b5(k)+b3(j)
                t7=b4(k)-b2(j)
                b0(j)=t0+t4
                b4(k)=t1+t5
                b1(j)=(t2+t6)*c1-(t3+t7)*s1
                b5(k)=(t2+t6)*s1+(t3+t7)*c1
                b2(j)=(t0-t4)*c2-(t1-t5)*s2
                b6(k)=(t0-t4)*s2+(t1-t5)*c2
                b3(j)=(t2-t6)*c3-(t3-t7)*s3
                b7(k)=(t2-t6)*s3+(t3-t7)*c3
   77         continue
              jr=jr+2
              ji=ji-2
              if(ji-jl) 78,78,70
   78         ji=2*jr-1
              jl=jr
   70     continue
      return
      end
c     *********************************************
c ford1       in-place reordering subroutines
      subroutine ford1(m,b)
        dimension b(2)
c
          k=4
          kl=2
          n=2**m
          do 94 j=4,n,2
            if (k-j) 92,92,91
   91       t=b(j)
            b(j)=b(k)
            b(k)=t
   92       k=k-2
            if (k-kl) 93,93,94
   93       k=2*j
            kl=j
   94     continue
      return
      end
c     *********************************************
c ford2
      subroutine ford2(m,b)
        dimension l(15),b(2)
        equivalence   (l15,l(1)),(l14,l(2)),(l13,l(3)),(l12,l(4)),
     1  (l11,l(5)),(l10,l(6)),(l9,l(7)),(l8,l(8)),(l7,l(9)),(l6,l(10)),
     2  (l5,l(11)),(l4,l(12)),(l3,l(13)),(l2,l(14)),(l1,l(15))
          n=2**m
  100     l(1)=n
          do 101 k=2,m
  101       l(k)=l(k-1)/2
          do 102 k=m,14
  102       l(k+1)=2
          ij=2
          do 103 j1=2,l1,2
          do 103 j2=j1,l2,l1
          do 103 j3=j2,l3,l2
          do 103 j4=j3,l4,l3
          do 103 j5=j4,l5,l4
          do 103 j6=j5,l6,l5
          do 103 j7=j6,l7,l6
          do 103 j8=j7,l8,l7
          do 103 j9=j8,l9,l8
          do 103 j10=j9,l10,l9
          do 103 j11=j10,l11,l10
          do 103 j12=j11,l12,l11
          do 103 j13=j12,l13,l12
          do 103 j14=j13,l14,l13
          do 103 ji=j14,l15,l14
            if (ij-ji) 108,103,103
  108       t=b(ij-1)
            b(ij-1)=b(ji-1)
            b(ji-1)=t
            t=b(ij)
            b(ij)=b(ji)
            b(ji)=t
  103       ij=ij+2
      return
      end
c ffa        fast fourier analysis subroutine  (ffa)
c
c         this subroutine replaces the real vector b(k),  (k=1,2,...,n),
c     with its finite discrete fourier transform.  the dc term is
c     returned in location b(1) with b(2) set to 0.  thereafter, the
c     jth harmonic is returned as a complex number stored as
c     b(2*j+1) + i b(2*j+2).  note that the n/2 harmonic is returned
c     in b(n+1) with b(n+2) set to 0.  hence, b must be dimensioned
c     to size n+2.
c     subroutine is called as ffa (b,n) where n=2**m and b is an
c     n term real array.  a real-valued, radix 8  algorithm is used
c     with in-place reordering and the trig functions are computed as
c     needed.
c
      subroutine ffa(b,nfft)
          dimension b(2)
c
c     iw is a machine dependent write device number
      iw=6
c
       n=1
       do 9 i=1,15
       m=i
       n=n*2
       if(n.eq.nfft)go to 11
    9 continue
       write(iw,100)
  100  format(30h nfft not a power of 2 for ffa)
       stop
   11  continue
          n8pow=m/3
c
c     do a radix 2 or radix 4 iteration first if one is required
c
          if(m-n8pow*3-1) 30,20,10
   10     nn=4
          int=n/nn
          call r4tr(int,b(1),b(int+1),b(2*int+1),b(3*int+1))
          go to 35
   20     nn=2
          int=n/nn
          call r2tr(int,b(1),b(int+1))
          go to 35
   30     nn=1
c
c     perform radix 8 iterations
c
   35     if(n8pow)60,60,40
   40     do 50 it=1,n8pow
          nn=nn*8
          int=n/nn
          call r8tr(int,nn,b(1),b(int+1),b(2*int+1),b(3*int+1),
     1 b(4*int+1),b(5*int+1),b(6*int+1),b(7*int+1),b(1),b(int+1),
     2 b(2*int+1),b(3*int+1),b(4*int+1),b(5*int+1),b(6*int+1),
     3      b(7*int+1))
   50     continue
c
c     perform in-place reordering
c
   60     call ord1(m,b)
          call ord2(m,b)
       t = b(2)
       b(2) = 0.
       b(nfft+1) = t
       b(nfft+2) = 0.
       do 65 i=4,nfft,2
         b(i) = -b(i)
   65  continue
   70 return
      end
c     *********************************************
c ffs        fast fourier synthesis subroutine   (ffs)
c
c         this subroutine synthesizes the real vector b(k), where
c     k=1,2,...,n. the initial fourier coefficients are placed in
c     the b array of size n+2.  the dc term is in b(1) with
c     b(2) equal to 0.
c     the jth harmonic is stored as b(2*j+1) + i b(2*j+2).
c     the n/2 harmonic is in b(n+1) with b(n+2) equal to 0.
c         the subroutine is called as ffs(b,n) where n=2**m and
c     b is the n term real array discussed above.
c
c
      subroutine ffs(b,nfft)
          dimension b(2)
c
c     iw is a machine dependent write device number
      iw=6
       n=1
       do 9 i=1,15
       m=i
       n=n*2
       if(n.eq.nfft)go to 11
    9 continue
       write(iw,100)
  100  format(30h nfft not a power of 2 for ffs)
       stop
   11  continue
       b(2) = b(nfft+1)
       do 7 i=1,nfft
         b(i) = b(i)/float(nfft)
    7  continue
       do 8 i=4,nfft,2
         b(i) = -b(i)
    8  continue
          n8pow=m/3
c
c     reorder the input fourier coefficients
c
          call ord2(m,b)
          call ord1(m,b)
c
    5  if(n8pow.eq.0)goto15
c     perform the radix 8 iterations
       nn=n
          do 10 it=1,n8pow
       int=n/nn
               call r8syn(int,nn,b,b(int+1),b(2*int+1),b(3*int+1),
     1           b(4*int+1),b(5*int+1),b(6*int+1),b(7*int+1),b(1),
     2           b(int+1),b(2*int+1),b(3*int+1),b(4*int+1),b(5*int+1),
     3           b(6*int+1),b(7*int+1))
       nn = nn/8
   10     continue
c
c     do a radix 2 or radix 4 iteration if one is required
c
   15     if(m-n8pow*3-1) 40,30,20
   20     int=n/4
          call r4syn(int,b(1),b(int+1),b(2*int+1),b(3*int+1))
          go to 40
   30     int=n/2
          call r2tr(int,b(1),b(int+1))
   40 return
      end
c     *********************************************
c r2tr       radix 2 iteration subroutine
c
      subroutine r2tr(int,b0,b1)
        dimension b0(2),b1(2)
        do 100 k=1,int
          t=b0(k)+b1(k)
          b1(k)=b0(k)-b1(k)
          b0(k)=t
  100   continue
      return
      end
c     *********************************************
c r4tr       radix 4 iteration subroutine
c
      subroutine r4tr(int,b0,b1,b2,b3)
          dimension b0(2),b1(2),b2(2),b3(2)
          do 200 k=1,int
               r0=b0(k)+b2(k)
               r1=b1(k)+b3(k)
               b2(k)=b0(k)-b2(k)
               b3(k)=b1(k)-b3(k)
               b0(k)=r0+r1
               b1(k)=r0-r1
  200     continue
      return
      end
c     *********************************************
c r8tr       radix 8 iteration subroutine
c
      subroutine r8tr(int,nn,br0,br1,br2,br3,br4,br5,br6,br7,
     1                bi0,bi1,bi2,bi3,bi4,bi5,bi6,bi7)
          dimension l(15),br0(2),br1(2),br2(2),br3(2),br4(2),
     1              br5(2),br6(2),br7(2),bi0(2),bi1(2),bi2(2),
     2              bi3(2),bi4(2),bi5(2),bi6(2),bi7(2)
          equivalence (l15,l(1)),(l14,l(2)),(l13,l(3)),(l12,l(4)),
     1              (l11,l(5)),(l10,l(6)),(l9,l(7)),(l8,l(8)),
     2              (l7,l(9)),(l6,l(10)),(l5,l(11)),(l4,l(12)),
     3              (l3,l(13)),(l2,l(14)),(l1,l(15))
c
c     set up counters such that jthet steps through the arguments
c     of w, jr steps through starting locations for the real part of the
c     intermediate results and ji steps through starting locations
c     of the imaginary part of the intermediate results.
c
          l(1)=nn/8
          do 10 k=2,15
               if(l(k-1)-2) 11,12,13
   11          l(k-1)=2
   12          l(k)=2
               go to 10
   13          l(k)=l(k-1)/2
   10     continue
          piovn=3.141592654/float(nn)
          p7=0.7071067812
          c22=0.9238795325
          s22=0.3826834323
          ji=3
          jl=2
          jr=2
          do 70 j1=2,l1,2
          do 70 j2=j1,l2,l1
          do 70 j3=j2,l3,l2
          do 70 j4=j3,l4,l3
          do 70 j5=j4,l5,l4
          do 70 j6=j5,l6,l5
          do 70 j7=j6,l7,l6
          do 70 j8=j7,l8,l7
          do 70 j9=j8,l9,l8
          do 70 j10=j9,l10,l9
          do 70 j11=j10,l11,l10
          do 70 j12=j11,l12,l11
          do 70 j13=j12,l13,l12
          do 70 j14=j13,l14,l13
          do 70 jthet=j14,l15,l14
               th2=jthet-2
               if(th2) 71,71,76
   71          do 72 k=1,int
                    t0=br0(k)+br4(k)
                    t1=br1(k)+br5(k)
                    t2=br2(k)+br6(k)
                    t3=br3(k)+br7(k)
                    t4=br0(k)-br4(k)
                    t5=br1(k)-br5(k)
                    t6=br2(k)-br6(k)
                    t7=br3(k)-br7(k)
                    br2(k)=t0-t2
                    br3(k)=t1-t3
                    t0=t0+t2
                    t1=t1+t3
                    br0(k)=t0+t1
                    br1(k)=t0-t1
                    pr=p7*(t5-t7)
                    pi=p7*(t5+t7)
                    br4(k)=t4+pr
                    br7(k)=t6+pi
                    br6(k)=t4-pr
                    br5(k)=pi-t6
   72          continue
               if(nn-8) 70,70,73
   73          k0=int*8+1
               kl=k0+int-1
               do 75 k=k0,kl
                    pr=p7*(bi2(k)-bi6(k))
                    pi=p7*(bi2(k)+bi6(k))
                    tr0=bi0(k)+pr
                    ti0=bi4(k)+pi
                    tr2=bi0(k)-pr
                    ti2=bi4(k)-pi
                    pr=p7*(bi3(k)-bi7(k))
                    pi=p7*(bi3(k)+bi7(k))
                    tr1=bi1(k)+pr
                    ti1=bi5(k)+pi
                    tr3=bi1(k)-pr
                    ti3=bi5(k)-pi
                    pr=tr1*c22-ti1*s22
                    pi=ti1*c22+tr1*s22
                    bi0(k)=tr0+pr
                    bi6(k)=tr0-pr
                    bi7(k)=ti0+pi
                    bi1(k)=pi-ti0
                    pr=-tr3*s22-ti3*c22
                    pi= tr3*c22-ti3*s22
                    bi2(k)=tr2+pr
                    bi4(k)=tr2-pr
                    bi5(k)=ti2+pi
                    bi3(k)=pi-ti2
   75          continue
               go to 70
   76          arg=th2*piovn
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
               int8=int*8
               j0=jr*int8+1
               k0=ji*int8+1
               jlast=j0+int-1
               do 77 j=j0,jlast
                    ks=k0+j-j0
               do 77 k=ks,ks
                    tr1=br1(j)*c1-bi1(k)*s1
                    ti1=br1(j)*s1+bi1(k)*c1
                    tr2=br2(j)*c2-bi2(k)*s2
                    ti2=br2(j)*s2+bi2(k)*c2
                    tr3=br3(j)*c3-bi3(k)*s3
                    ti3=br3(j)*s3+bi3(k)*c3
                    tr4=br4(j)*c4-bi4(k)*s4
                    ti4=br4(j)*s4+bi4(k)*c4
                    tr5=br5(j)*c5-bi5(k)*s5
                    ti5=br5(j)*s5+bi5(k)*c5
                    tr6=br6(j)*c6-bi6(k)*s6
                    ti6=br6(j)*s6+bi6(k)*c6
                    tr7=br7(j)*c7-bi7(k)*s7
                    ti7=br7(j)*s7+bi7(k)*c7
c
                    t0=br0(j)+tr4
                    t1=bi0(k)+ti4
                    tr4=br0(j)-tr4
                    ti4=bi0(k)-ti4
                    t2=tr1+tr5
                    t3=ti1+ti5
                    tr5=tr1-tr5
                    ti5=ti1-ti5
                    t4=tr2+tr6
                    t5=ti2+ti6
                    tr6=tr2-tr6
                    ti6=ti2-ti6
                    t6=tr3+tr7
                    t7=ti3+ti7
                    tr7=tr3-tr7
                    ti7=ti3-ti7
c
                    tr0=t0+t4
                    ti0=t1+t5
                    tr2=t0-t4
                    ti2=t1-t5
                    tr1=t2+t6
                    ti1=t3+t7
                    tr3=t2-t6
                    ti3=t3-t7
                    t0=tr4-ti6
                    t1=ti4+tr6
                    t4=tr4+ti6
                    t5=ti4-tr6
                    t2=tr5-ti7
                    t3=ti5+tr7
                    t6=tr5+ti7
                    t7=ti5-tr7
                    br0(j)=tr0+tr1
                    bi7(k)=ti0+ti1
                    bi6(k)=tr0-tr1
                    br1(j)=ti1-ti0
                    br2(j)=tr2-ti3
                    bi5(k)=ti2+tr3
                    bi4(k)=tr2+ti3
                    br3(j)=tr3-ti2
                    pr=p7*(t2-t3)
                    pi=p7*(t2+t3)
                    br4(j)=t0+pr
                    bi3(k)=t1+pi
                    bi2(k)=t0-pr
                    br5(j)=pi-t1
                    pr=-p7*(t6+t7)
                    pi=p7*(t6-t7)
                    br6(j)=t4+pr
                    bi1(k)=t5+pi
                    bi0(k)=t4-pr
                    br7(j)=pi-t5
   77          continue
               jr=jr+2
               ji=ji-2
               if(ji-jl) 78,78,70
   78          ji=2*jr-1
               jl=jr
   70     continue
      return
      end
c     *********************************************
c r4syn      radix 4 synthesis
c
      subroutine r4syn (int,b0,b1,b2,b3)
          dimension b0(2),b1(2),b2(2),b3(2)
          do 200 k=1,int
               t0=b0(k)+b1(k)
               t1=b0(k)-b1(k)
               t2=b2(k)+b2(k)
               t3=b3(k)+b3(k)
               b0(k)=t0+t2
               b2(k)=t0-t2
               b1(k)=t1+t3
               b3(k)=t1-t3
 200      continue
      return
      end
c     *********************************************
c r8syn         radix 8 synthesis subroutine
c
      subroutine r8syn(int,nn,br0,br1,br2,br3,br4,br5,br6,
     1                br7,bi0,bi1,bi2,bi3,bi4,bi5,bi6,bi7)
          dimension l(15),br0(2),br1(2),br2(2),br3(2),br4(2),
     1              br5(2),br6(2),br7(2),bi0(2),bi1(2),bi2(2),
     2              bi3(2),bi4(2),bi5(2),bi6(2),bi7(2)
          equivalence (l15,l(1)),(l14,l(2)),(l13,l(3)),(l12,l(4)),
     1              (l11,l(5)),(l10,l(6)),(l9,l(7)),(l8,l(8)),
     2              (l7,l(9)),(l6,l(10)),(l5,l(11)),(l4,l(12)),
     3              (l3,l(13)),(l2,l(14)),(l1,l(15))
          l(1)=nn/8
          do 10 k=2,15
               if(l(k-1)-2) 11,12,13
   11          l(k-1)=2
   12          l(k)=2
               go to 10
   13          l(k)=l(k-1)/2
   10     continue
          piovn=3.141592654/float(nn)
          p7=0.7071067812
          p7two=1.4142135624
          c22=0.9238795325
          s22=0.3826834323
          ji=3
          jl=2
          jr=2
c
          do 70 j1=2,l1,2
          do 70 j2=j1,l2,l1
          do 70 j3=j2,l3,l2
          do 70 j4=j3,l4,l3
          do 70 j5=j4,l5,l4
          do 70 j6=j5,l6,l5
          do 70 j7=j6,l7,l6
          do 70 j8=j7,l8,l7
          do 70 j9=j8,l9,l8
          do 70 j10=j9,l10,l9
          do 70 j11=j10,l11,l10
          do 70 j12=j11,l12,l11
          do 70 j13=j12,l13,l12
          do 70 j14=j13,l14,l13
          do 70 jthet=j14,l15,l14
               th2=jthet-2
               if(th2) 71,71,76
   71          do 72 k=1,int
                    t0=br0(k)+br1(k)
                    t1=br0(k)-br1(k)
                    t2=br2(k)+br2(k)
                    t3=br3(k)+br3(k)
                    t4=br4(k)+br6(k)
                    t6=br7(k)-br5(k)
                    t5=br4(k)-br6(k)
                    t7=br7(k)+br5(k)
                    pr=p7*(t7+t5)
                    pi=p7*(t7-t5)
                    tt0=t0+t2
                    tt1=t1+t3
                    t2=t0-t2
                    t3=t1-t3
                    t4=t4+t4
                    t5=pr+pr
                    t6=t6+t6
                    t7=pi+pi
                    br0(k)=tt0+t4
                    br1(k)=tt1+t5
                    br2(k)=t2+t6
                    br3(k)=t3+t7
                    br4(k)=tt0-t4
                    br5(k)=tt1-t5
                    br6(k)=t2-t6
                    br7(k)=t3-t7
   72          continue
               if(nn-8) 70,70,73
   73          k0=int*8+1
               kl=k0+int-1
               do 75 k=k0,kl
                    t1=bi0(k)+bi6(k)
                    t2=bi7(k)-bi1(k)
                    t3=bi0(k)-bi6(k)
                    t4=bi7(k)+bi1(k)
                    pr=t3*c22+t4*s22
                    pi=t4*c22-t3*s22
                    t5=bi2(k)+bi4(k)
                    t6=bi5(k)-bi3(k)
                    t7=bi2(k)-bi4(k)
                    t8=bi5(k)+bi3(k)
                    rr=t8*c22-t7*s22
                    ri=-t8*s22-t7*c22
                    bi0(k)=(t1+t5)+(t1+t5)
                    bi4(k)=(t2+t6)+(t2+t6)
                    bi1(k)=(pr+rr)+(pr+rr)
                    bi5(k)=(pi+ri)+(pi+ri)
                    t5=t1-t5
                    t6=t2-t6
                    bi2(k)=p7two*(t6+t5)
                    bi6(k)=p7two*(t6-t5)
                    rr=pr-rr
                    ri=pi-ri
                    bi3(k)=p7two*(ri+rr)
                    bi7(k)=p7two*(ri-rr)
   75          continue
               go to 70
   76          arg=th2*piovn
               c1=cos(arg)
               s1=-sin(arg)
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
               int8=int*8
               j0=jr*int8+1
               k0=ji*int8+1
               jlast=j0+int-1
               do 77 j=j0,jlast
                    ks=k0+j-j0
               do 77 k=ks,ks
                    tr0=br0(j)+bi6(k)
                    ti0=bi7(k)-br1(j)
                    tr1=br0(j)-bi6(k)
                    ti1=bi7(k)+br1(j)
                    tr2=br2(j)+bi4(k)
                    ti2=bi5(k)-br3(j)
                    tr3=bi5(k)+br3(j)
                    ti3=bi4(k)-br2(j)
                    tr4=br4(j)+bi2(k)
                    ti4=bi3(k)-br5(j)
                    t0=br4(j)-bi2(k)
                    t1=bi3(k)+br5(j)
                    tr5=p7*(t0+t1)
                    ti5=p7*(t1-t0)
                    tr6=br6(j)+bi0(k)
                    ti6=bi1(k)-br7(j)
                    t0=br6(j)-bi0(k)
                    t1=bi1(k)+br7(j)
                    tr7=-p7*(t0-t1)
                    ti7=-p7*(t1+t0)
                    t0=tr0+tr2
                    t1=ti0+ti2
                    t2=tr1+tr3
                    t3=ti1+ti3
                    tr2=tr0-tr2
                    ti2=ti0-ti2
                    tr3=tr1-tr3
                    ti3=ti1-ti3
                    t4 =tr4+tr6
                    t5 =ti4+ti6
                    t6 =tr5+tr7
                    t7 =ti5+ti7
                    ttr6=ti4-ti6
                    ti6=tr6-tr4
                    ttr7=ti5-ti7
                    ti7=tr7-tr5
                    br0(j)=t0+t4
                    bi0(k)=t1+t5
                    br1(j)=c1*(t2+t6 )-s1*(t3+t7 )
                    bi1(k)=c1*(t3+t7 )+s1*(t2+t6 )
                    br2(j)=c2*(tr2+ttr6)-s2*(ti2+ti6)
                    bi2(k)=c2*(ti2+ti6)+s2*(tr2+ttr6)
                    br3(j)=c3*(tr3+ttr7)-s3*(ti3+ti7)
                    bi3(k)=c3*(ti3+ti7)+s3*(tr3+ttr7)
                    br4(j)=c4*(t0-t4 )-s4*(t1-t5 )
                    bi4(k)=c4*(t1-t5 )+s4*(t0-t4 )
                    br5(j)=c5*(t2-t6 )-s5*(t3-t7 )
                    bi5(k)=c5*(t3-t7 )+s5*(t2-t6 )
                    br6(j)=c6*(tr2-ttr6)-s6*(ti2-ti6)
                    bi6(k)=c6*(ti2-ti6)+s6*(tr2-ttr6)
                    br7(j)=c7*(tr3-ttr7)-s7*(ti3-ti7)
                    bi7(k)=c7*(ti3-ti7)+s7*(tr3-ttr7)
   77          continue
               jr=jr+2
               ji=ji-2
               if(ji-jl) 78,78,70
   78          ji=2*jr-1
               jl=jr
   70     continue
      return
      end
c     *********************************************
c ord1       in-place reordering subroutines
      subroutine ord1(m,b)
        dimension b(2)
c
          k=4
          kl=2
          n=2**m
          do 94 j=4,n,2
            if (k-j) 92,92,91
   91       t=b(j)
            b(j)=b(k)
            b(k)=t
   92       k=k-2
            if (k-kl) 93,93,94
   93       k=2*j
            kl=j
   94     continue
      return
      end
c     *********************************************
c ord2
      subroutine ord2(m,b)
        dimension l(15),b(2)
        equivalence   (l15,l(1)),(l14,l(2)),(l13,l(3)),(l12,l(4)),
     1  (l11,l(5)),(l10,l(6)),(l9,l(7)),(l8,l(8)),(l7,l(9)),(l6,l(10)),
     2  (l5,l(11)),(l4,l(12)),(l3,l(13)),(l2,l(14)),(l1,l(15))
          n=2**m
  100     l(1)=n
          do 101 k=2,m
  101       l(k)=l(k-1)/2
          do 102 k=m,14
  102       l(k+1)=2
          ij=2
          do 103 j1=2,l1,2
          do 103 j2=j1,l2,l1
          do 103 j3=j2,l3,l2
          do 103 j4=j3,l4,l3
          do 103 j5=j4,l5,l4
          do 103 j6=j5,l6,l5
          do 103 j7=j6,l7,l6
          do 103 j8=j7,l8,l7
          do 103 j9=j8,l9,l8
          do 103 j10=j9,l10,l9
          do 103 j11=j10,l11,l10
          do 103 j12=j11,l12,l11
          do 103 j13=j12,l13,l12
          do 103 j14=j13,l14,l13
          do 103 ji=j14,l15,l14
            if (ij-ji) 108,103,103
  108       t=b(ij-1)
            b(ij-1)=b(ji-1)
            b(ji-1)=t
            t=b(ij)
            b(ij)=b(ji)
            b(ji)=t
  103       ij=ij+2
      return
      end
c     *********************************************
