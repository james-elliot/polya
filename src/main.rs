const MAXV:usize = 2000000000;
const SIZE:usize = (MAXV/8)+1;

//const MAXV:usize = 100;
fn build_primes () -> Vec<u64> {
    let mut p = Vec::new();
    p.push(2);
    let mut i = 3;
    let sto = (MAXV as f64+0.01).sqrt() as  u64;
    loop {
	let mut j = 0;
	let st = (i as f64+0.01).sqrt() as u64;
	loop  {
	    if i % p[j] == 0 || p[j] > st {break}
	    j=j+1;
	}
	if p[j] > st {
	    p.push(i);
	    if i > sto {break}
	}
	i=i+2;
    }
    return p;
}

/*
For the algorithm see:
MATHEMATICS OF COMPUTATION
Volume 77, Number 263, July 2008, Pages 1681-1694
S 0025-5718(08)02036-X
Article electronically published on January 25, 2008
SIGN CHANGES IN SUMS OF THE LIOUVILLE FUNCTION
PETER BORWEIN, RON FERGUSON, AND MICHAEL J. MOSSINGHOFF
*/
fn liouville()-> Box<[u8;MAXV]> {
    let mut tab = Box::new([0 as u8; MAXV]);
    tab[1]=1;tab[2]=0;
    let primes = build_primes();

    let mut a = 3;
    let mut b;
    while a<MAXV {
	b=2*a-1;
	if b>=MAXV {b=MAXV-1}
	let m = (b as f64+0.01).sqrt() as usize;
	for p in primes.iter() {
	    let p = *p as usize;
	    if p>m {break}
	    let mut k = a/p;
	    let mut mp = k*p;
	    if mp<a {mp=mp+p;k=k+1}
	    loop {
		if mp>b {break}
		tab[mp]= 1^tab[k];
		mp=mp+p;
		k=k+1;
	    }
	}
	a=b+1
    }
    return tab;
}

fn liouville2()-> Box<[u8;SIZE]> {
    let mut tab = Box::new([0 as u8; SIZE]);
    tab[0]=0x2;
    let primes = build_primes();

    let mut a = 3;
    let mut b;
    while a<MAXV {
	b=2*a-1;
	if b>=MAXV {b=MAXV-1}
	let m = (b as f64+0.01).sqrt() as usize;
	for p in primes.iter() {
	    let p = *p as usize;
	    if p>m {break}
	    let mut k = a/p;
	    let mut mp = k*p;
	    if mp<a {mp=mp+p;k=k+1}
	    loop {
		if mp>b {break}
		let (i1,i2)=(mp/8,mp%8);
		let (j1,j2)=(k/8,k%8);
		if tab[j1]&(1<<j2)==0 {
		    tab[i1]=tab[i1]|(1<<i2)
		}
		mp=mp+p;
		k=k+1;
	    }
	}
	a=b+1
    }
    /*
    for v in tab.iter() {
	println!("{:#010b}",*v);
}
    */
    return tab;
}

fn summatory(tab: Box<[u8;MAXV]>) {
    let (mut first,mut last)=(0,0);
    let (mut imax,mut maxi)=(0,i64::MIN);
    let (mut imin,mut mini)=(0,i64::MAX);
    let mut num = 0 as i64;
    for i in 3..MAXV {
	if tab[i]==0 {num=num-1}
	else {num=num+1}
	if num > 0 {last=i}
	if num > 0 && first == 0 {first=i}
	if num > maxi {maxi=num;imax=i}
	if num < mini {mini=num;imin=i}
    }
    println!("first={},imax={},maxi={},imin={},mini={},last={})",
	     first,imax,maxi,imin,mini,last);
}

fn summatory2(tab: Box<[u8;SIZE]>) {
    let (mut first,mut last)=(0,0);
    let (mut imax,mut maxi)=(0,i64::MIN);
    let (mut imin,mut mini)=(0,i64::MAX);
    let mut num = 0 as i64;
    for i in 3..MAXV {
	let (i1,i2)=(i/8,i%8);
	if tab[i1]&(1<<i2)==0 {num=num-1}
	else {num=num+1}
	if num > 0 {last=i}
	if num > 0 && first == 0 {first=i}
	if num > maxi {maxi=num;imax=i}
	if num < mini {mini=num;imin=i}
    }
    println!("first={},imax={},maxi={},imin={},mini={},last={})",
	     first,imax,maxi,imin,mini,last);
}

fn main() {
    let tab = liouville2();
    summatory2(tab);
}
