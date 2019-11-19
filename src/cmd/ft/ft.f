	subroutine ft
	implicit undefined (a-z)
	include '../../stacks/F.h'
	include '../../main/F.h'
	include 'ft.df.h'
	include 'ft.if.h'

	real bft2(1028)
	automatic bft2(1028)
	integer i,j,mchan
	automatic i,j,mchan
c functions
	integer stfull
	real sqrt
c call parse to advance the ntty pointer.
	include 'ft.ef.h'
c
	if(stfull(1).eq.0) call error("No data in stack 1")
  102 do 110 i=1,nch
      j=2*i-1
  110 bft2(j)=stak1(i)
      mchan=2*nch-1
      do 111 i=2,mchan,2
  111 bft2(i)=(bft2(i-1)+bft2(i+1))/2.
      bft2(mchan+1)=bft2(mchan)
      call fast(bft2,2*nch)
      do 105 i=1,nch
  105 stak1(i)=sqrt(bft2(2*i+1)**2+bft2(2*i+2)**2)
c     write(1,106) nchan
c 106 format("fft in stack 1, plot channels 1 to ",i3)
c     numplt=nchan
	do 120 i=nch+1,numpt1
120	stak1(i)=0.
	numpt1 = nch
	pltd = 0
	pltd = 0
	end
c
      subroutine fast(b,n)
	implicit undefined (a-z)
c
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
	real b(2)
	integer n
	integer i,m,nt,n4pow,nn,int,it
	automatic i,m,nt,n4pow,nn,int,it
	real t
	automatic t
c
c
       do 5 i=1,15
       m=i
       nt = 2**i
       if ( n.eq.nt ) goto 7
    5  continue
	call error("n is not a power of two for fast")
    7  n4pow=m/2
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
      subroutine fr2tr(int,b0,b1)
	implicit undefined (a-z)
        real b0(2),b1(2)
	integer int
c
	real t
	automatic t
	integer k
	automatic k
        do 100 k=1,int
          t=b0(k)+b1(k)
          b1(k)=b0(k)-b1(k)
          b0(k)=t
  100   continue
      return
      end
      subroutine fr4tr(int,nn,b0,b1,b2,b3,b4,b5,b6,b7)
	implicit undefined (a-z)
	integer int,nn
	real b0,b1,b2,b3,b4,b5,b6,b7
c
c fr4tr       radix 4 iteration subroutine
c
        real l(15),b0(2),b1(2),b2(2),b3(2),b4(2),b5(2),b6(2),b7(2)
	real l15,l14,l13,l12,l11,l10,l9,l8,l7,l6,l5,l4,l3,l2,l1
        equivalence   (l15,l(1)),(l14,l(2)),(l13,l(3)),(l12,l(4)),
     1  (l11,l(5)),(l10,l(6)),(l9,l(7)),(l8,l(8)),(l7,l(9)),(l6,
     2  l(10)),(l5,l(11)),(l4,l(12)),(l3,l(13)),(l2,l(14)),(l1,l(15))
c
	integer k,j,ji,jl,jr,j0,j1,j2,j3,j4,j5,j6,j7,j8,j9,j10,j11,j12,
     1	j13,j14,jthet,jlast,k0,kl,int4,ks
	automatic k,j,ji,jl,jr,j0,j1,j2,j3,j4,j5,j6,j7,j8,j9,j10,j11,j12,
     1	j13,j14,jthet,jlast,k0,kl,int4,ks
	real arg,pr,pi,piovn,p7,th2,t0,t1,t2,t3,t4,t5,t6,t7,r1,r5,
     1	c1,c2,c3,s1,s2,s3
	automatic arg,pr,pi,piovn,p7,th2,t0,t1,t2,t3,t4,t5,t6,t7,r1,r5,
     1	c1,c2,c3,s1,s2,s3
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
      subroutine ford1(m,b)
	implicit undefined (a-z)
	integer m
        real b(2)
c
	integer k,kl,n,j
	automatic k,kl,n,j
	real t
	automatic t
c
c ford1       in-place reordering subroutines
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
      subroutine ford2(m,b)
	implicit undefined (a-z)
	integer m
        real b(2)
c
c ford2
        integer l(15),l15,l14,l13,l12,l11,l10,l9,l8,l7,l6,l5,l4,l3,l2,l1
        equivalence   (l15,l(1)),(l14,l(2)),(l13,l(3)),(l12,l(4)),
     1  (l11,l(5)),(l10,l(6)),(l9,l(7)),(l8,l(8)),(l7,l(9)),(l6,l(10)),
     2  (l5,l(11)),(l4,l(12)),(l3,l(13)),(l2,l(14)),(l1,l(15))
c
	integer n,k,j1,j2,j3,j4,j5,j6,j7,j8,j9,j10,j11,j12,j13,j14,ji,ij
	automatic n,k,j1,j2,j3,j4,j5,j6,j7,j8,j9,j10,j11,j12,j13,j14,ji,ij
	real t
	automatic t
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
