const MAXV:usize = 1000000000;
const SIZE:usize = (MAXV/8)+1;

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
	    j += 1;
	}
	if p[j] > st {
	    p.push(i);
	    if i > sto {break}
	}
	i += 2;
    }
    p
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
use unchecked_index::{UncheckedIndex,unchecked_index};
unsafe fn liouville()-> UncheckedIndex<Box<[u8;SIZE]>> {
    let mut tab = unchecked_index(Box::new([0_u8; SIZE]));
    tab[0]=0x2;
    let primes = build_primes();

    let mut a = 3;
    while a<MAXV {
	let b = std::cmp::min(2*a-1,MAXV);
	let bsup = (b as f64+0.01).sqrt() as usize;
	for p in primes.iter() {
	    let p = *p as usize;
	    if p>bsup {break}
	    let mut k = a/p;
	    let mut mp = k*p;
	    if mp<a {mp += p;k += 1}
	    loop {
		if mp>b {break}
		let (i1,i2)=(mp/8,(mp%8) as u8);
		let (j1,j2)=(k/8,(k%8) as u8);
		if tab[j1]&(1<<j2)==0 {tab[i1] |= 1<<i2}
		mp += p;
		k += 1;
	    }
	}
	a=b+1
    }
    tab
}

unsafe fn summatory(tab: UncheckedIndex<Box<[u8;SIZE]>>) {
    let (mut first,mut last)=(0,0);
    let (mut imax,mut maxi)=(0,i64::MIN);
    let (mut imin,mut mini)=(0,i64::MAX);
    let mut num = 0_i64;
    for i in 3..MAXV {
	let (i1,i2)=(i/8,i%8);
	if tab[i1]&(1<<i2)==0 {num -= 1}
	else {num += 1}
	if num > 0 {last=i}
	if num > 0 && first == 0 {first=i}
	if num > maxi {maxi=num;imax=i}
	if num < mini {mini=num;imin=i}
    }
    println!(
	"first={},imax={},maxi={},last={},imin={},mini={}",
	first,imax,maxi,last,imin,mini);
}

fn main() {
    unsafe {
        let tab = liouville();
        summatory(tab);
    }
}
