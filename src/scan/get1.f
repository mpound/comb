      subroutine get1
c
c
	implicit undefined(a-z)
	include '../stacks/F.h'
	include '../scan/F.h'
	include '../main/F.h'
	integer*2 nwid(5),nchn(5),itemp(128)
      equivalence (nwid(1),ndat(44)),(nchn(1),ndat(50))
	automatic  nflg,nch1,np1,
     1	n1,n2,ncnt,navg,nen,jsign,i,ii
     2	,itemp
	integer  nflg,nch1,np1,
     1	n1,n2,ncnt,navg,nen,jsign,i,ii
	automatic  sinel,tabs,tc,tamb,tw,to,to2,eta,sp,sp2,rmage,attn,attn2,
     1	tauo,tauo2,oattn,oattn2,wattn,tsky2,dtsig,dtim,f,trans,trec
	real*4 sinel,tabs,tc,tamb,tw,to,to2,eta,sp,sp2,rmage,attn,attn2,
     1	tauo,tauo2,oattn,oattn2,wattn,tsky2,dtsig,dtim,f,trans,trec
	character*4 fn
	equivalence (fn,nstk1(1))
	real*4 pi
	parameter(pi=3.14159265359)
c clear stack header 
	call clear(numpt1,512)
c header version 1, no baseline removed yet
	nver1=1
	ibsln1=-1
	numst1 = 1
c put file name and scan number in hdr
	nlist1=1
	indx1=3
c	nstk1(1)=-32768
c	fn(1:1)=datfn(ndfn-2:ndfn-2)
c	fn(3:4)=datfn(ndfn-1:ndfn)
	call putfn
      nstk1(3)=ndat(1)
c Clear the data plotted flag
	pltd=0
	factt1=1.
      time1=ndat(18)/10.
      if(time1.le.0.01) time1=.01
	iobssec = (ndat(5) - 7305) * 86400 + (ndat(16) * 86400) / 16384

c Not needed here
c Set the kuse array to the current nuse array.
c	call move(nuse,kuse1,64)
	
c Set the backend number.
	if(ndat(13).ge.20)then
		nbknd=1
	else if(mbknd.ge.10)then
		do 30 i=1,ndat(3)
			if((mbknd.eq.10.and.ndat(43+i).lt.25).or.
     1			(mbknd.eq.ndat(43+i)))then
				nbknd=i
				goto 40
			endif
30		continue
	else
		nbknd=mbknd
	endif
40	continue
	if((nbknd.lt.1).or.(nbknd.gt.ndat(3))) call error("Bad backend")
	nflg=1
	if(ndat(44).eq.100.and.ndat(45).eq.100.and.nbknd.eq.1) then
		if(ndat(46).eq.100.and.ndat(3).eq.3) then
			nflg=3
		elseif(ndat(3).eq.2) then
			nflg=2
		endif
	endif
      ncur=nstrt
c  get correct backend
      if(nbknd.eq.1)go to 125
      do 120 i=1,nbknd-1
      ncur=ncur+nchn(i)
120	continue
      ndat(66)=ndat(17+nbknd)
125	continue
      fwid1=nwid(nbknd)/100.+ndat(5+nbknd)/1.e6
      nch1=nchn(nbknd)
      if(nflg.gt.1)nch1=512
c  nch1.eq.0 signifies a virtual backend
      if(nch1.le.0)call error("Virtual backend")
      numpt1=min0(nch1,maxchn)
	if (ndat(94).eq.256) then
		nplc1=11
	else 
		nplc1=30
	endif
	if(ndat(13).ge.20)nplc1=nplc1+300
      nen=ncur+numpt1
	if(nen.gt.lnfil.or.ncur.lt.nstrt) call error
     ^	("bad data loc this bknd")
c  take care of bad channels
	if(ndat(13).lt.20)then
c first if the expander, get rid of chan 1
		if(fwid1.lt.0.25) ndat(257)=ndat(258)
		do 140 i=1,numbch,2
			n1=nbadch(i)
			n2=nbadch(i+1)
			if(n1.gt.nen.or.n2.le.ncur)goto 140
			if(n1.gt.ncur+1) then
				ncnt=1
				navg=ndat(n1-1)
			else
				ncnt=0
				navg=0
			endif
			if(n2.lt.nen)then
				ncnt=ncnt+1
				navg=navg+ndat(n2+1)
			endif
			if(ncnt.gt.0) then
				navg=navg/ncnt
				do 135 ii=n1,n2
					ndat(ii)=navg
135				continue
			endif
140		continue
141		continue
c now also set up the process flag
		nprc1=kontrl
	elseif (ndat(13).eq.20) then
		nprc1=2000
	else if (ndat(13).eq.21) then
		nprc1=1000
	else if (ndat(13).eq.22) then
		nprc1=3000
	endif
      ra1=ndat(55)+ndat(56)/60.+ndat(57)/3600.
c	if(ndat(58).lt.0)ndat(59)=-iabs(ndat(59))
c	if(ndat(59).lt.0)ndat(60)=-iabs(ndat(60))
      dec1=ndat(58)+ndat(59)/60.+ndat(60)/3600.
	epoch1=ndat(33)/10.
	vlsr1=ndat(4)*10.+ndat(106)/1.e3
c If data is from before 3/12/82 correct vlsr1 for old scan hdr
	if(ndat(5).lt.11757)vlsr1=vlsr1/100.
	freq1=ndat(40)*1.d3+ndat(41)+ndat(42)/1.d3
	dra1=(ndat(61)+ndat(107)/1e4)/9000.
	ddec1=(ndat(62)+ndat(108)/1e4)/600.
	oddec1=ddec1*60.
	odra1=dra1*900.*cos((dec1+ddec1)*pi/180.)
	l1=ndat(96)+ndat(97)/1000.
	b1=float(ndat(98))+float(ndat(99))/1000.
	dl1=float(ndat(100))/1000.
	db1=float(ndat(101))/1000.
	azimuth = -1.

c
c  calculate scaling for atmospheric attenuation,image sideband
c  suppression,spillover and beam efficiency
	if(ndat(23).le.0)ndat(23)=1
      sinel=ndat(23)/16384.
      tabs=ndat(29)-ndat(15)
      tamb=ndat(29)
      eta=ndat(27)/100.
      sp=ndat(26)/100.
      sp2=(ndat(26)+ndat(32))/100.
      rmage=10.**(-ndat(30)/1000.)
c Use f as a temporary variable
	f=-ndat(67)/1000./sinel

c If HI survey, set zenith tau to .0082
	if((freq1-1420.4).lt.0.01) f=(-0.0082/sinel)

	if(f.gt.-20.) then
		attn=exp(f)
	else
		attn=1.e-10
	endif
	f=-(ndat(67)+ndat(31))/1000./sinel
	if(f.gt.-20.) then
		attn2=exp(f)
	else
		attn2=1.e-10
	endif
	trans=attn*sp*eta
	tsky=tabs*sp*(1.-attn)+tamb*(1.-sp)
	tsky2=tabs*sp2*(1.-attn2)+tamb*(1.-sp2)
	if(((ndat(88)-1366).lt.1.).and.(ndat(30).eq.0.).and.
     1		(ndat(40).eq.115.)) go to 150
	go to 175
c Use 2-layer atm model to correct opacities of SIS DSB 12CO J=1-0 data;
c formulas for oxygen temp and opacity from Kutner(1978) Ap Lett vol.19 p.81
150	tw=tamb-10
	tauo= .18*(1.+4.0e7*((1./tamb**3)-3.56e-8))
	tauo2=.08*(1.+3.7e7*((1./tamb**3)-3.56e-8))
c In general, Tamb in tau's should be f*Tamb+(1.-f)*<Tamb>,
c where f=.44 at sea level and <Tamb> is averaged over the day
	to= tamb*(0.9+0.02*tauo)
	to2=tamb*(0.9+0.02*tauo2)
	oattn= exp(-tauo/sinel)
	oattn2=exp(-tauo2/sinel)
	wattn=(2.*tsky-2.*tamb+(tamb-tw)*(sp+sp2))/
     1	      (sp*to*(1.-oattn)+sp2*to2*(1.-oattn2)-tw*(sp+sp2))
	trans=oattn*wattn*sp*eta
	tsky=sp*(to*(1.-oattn)*wattn+tw*(1.-wattn))+(1.-sp)*tamb
	tsky2=sp2*(to2*(1.-oattn2)*wattn+tw*(1.-wattn))+(1.-sp2)*tamb
175	trec=(1.+rmage)*ndat(66)
	if(ndat(28).lt.0)then
		tsys=(trec+tsky+rmage*tsky2)/trans
	else
		tsys=trec/trans
	endif
	if(ndat(13).gt.29)then
		call error("Bad observation type")
	elseif(ndat(13).ge.20)then
		f=-ndat(28)/ndat(65)/trans
	elseif(ndat(13).gt.10)then
		f=1.
	elseif(ndat(13).eq.10)then
		f=1.+rmage
	elseif(ndat(28).lt.0)then
		f=-ndat(28)/ndat(65)*(1.+rmage)/trans
	else
		tc=ndat(28)
		dtsig=tc-tsky
		dtim=tc-tsky2
		f=(dtsig+rmage*dtim)/(ndat(65)/10.*trans)
	endif
c  wt is the equivalent integration time assuming ssb noise
c  figure of 1000k, no atmosphere, no spillover, perfect beam
c  and sky temp=chopper temp=300k (tc=600k)
c  a single-switching mode is assumed -- thus if wt=60, then
c  30 sec was assumed spent looking at the source,30 sec off
	nwts1=1
	locwt1(1)=1
	wght1(1)=time1*(1000./tsys)**2
	if(ndat(13).lt.20) then
		rms1=2.0/sqrt(fwid1*wght1(1))
	else
c make the bad assumption that the full nbe was used in continuum observations.
		rms1=2.0/sqrt(64*wght1(1))
	endif
c fixup for b,1,b,4,b,2,3 backend
	if(nflg.eq.3)then
		call move(ndat(257),itemp(1),256)
		call move(ndat(385),ndat(257),512)
		call move(itemp(1),ndat(513),256)
	endif
c  now consider fudge factor and exponent of data
      fmult=f*(ndat(25)/100.)
      f=fmult*(1./10.**ndat(14))
	if(map.eq.1)call error("can't handle mapping data")
	if(ndat(13).ge.20)then
		inver=1
		if(ndat(13).le.21) fwid1=ndat(71)/36000.
		if(ndat(13).eq.20) fwid1=fwid1*sqrt(1.-sinel*sinel)
		expch1=nch1/2.+1.
		goto 300
	elseif(kontrl.eq.0) then
		inver=1
		expch1=(nch1+1)/2.
		goto 300
c  if using freq info must decide about inverting data rt to left
	elseif(ndat(43).gt.2)then
		inver=-invrt
	else
		inver=invrt
	endif
c  now velocity will increase with channel #
c  test sig and ref
c  assume center freq corresponds to center of filter bank (numpt1/2.+0.5)
c fixup for b,1,b,4,b,2,3 and b,1,2,b,3,4 backends
	if(nflg.gt.1)then
		ndat(77)=0
		ndat(89)=0
	endif
      expch1=(nch1+1.)/2.-(ndat(2)+ndat(11)/1.e4+inver*(ndat(76+nbknd)
     ^+ndat(88+nbknd)/1.e4))/(fwid1*100.)
	if((and(ndat(43),1).eq.0))then
		jsign=-1
	else
		jsign=1
	endif
300	if(ndat(13).eq.1.or.ndat(13).eq.3) then
		refch1=expch1+jsign*((ndat(63)+ndat(24)/1.e4)-ndat(64)
     1			-ndat(17)/1.e4)/(fwid1*10.)
	else
		refch1=notfs
	endif
	np1=numpt1+1
	if(numpt1.ge.maxchn) call error("Too many chans")
      do 310 i=1,numpt1
	if(inver.gt.0)then
		ii=i
	else
		ii=np1-i
	endif
      stak1(ii)=f*(ndat(i+ncur)+ndat(49))
310	continue
c This shouldn't be necessary, and now there are a lot of chans
c	if(numpt1.lt.maxchn)call clear(stak1(numpt1+1),(maxchn-numpt1)*4)
	call mrkful(1)
      return
      end
