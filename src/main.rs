//const SIZE:usize = 1000000000;
const SIZE:usize = 100;

fn build_primes () -> Vec<u32> {
    let mut p = Vec::new();
    p.push(2);
    let mut i = 3;
    let sto = (SIZE as f64+0.01).sqrt() as  u32;
    loop {
	let mut j = 0;
	let st = (i as f64+0.01).sqrt() as u32;
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

fn do_it() {
    let mut tab = Box::new([0 as u8; SIZE]);
    tab[1]=0;tab[2]=1;
    let mut num = 1-1;
    let (mut first,mut last)=(0,0);
    let (mut imax,mut maxi)=(0,i32::MIN);
    let (mut imin,mut mini)=(0,i32::MAX);
    let p = build_primes();
    for i in 3..SIZE {
	let mut j = 0;
	let st = (i as f64+0.01).sqrt() as u32;
	loop  {
	    if (i as u32) % p[j] == 0 || p[j] > st {break}
	    j=j+1;
	}
	if p[j] > st {
	    tab[i]=1;
	    num=num-1}
	else {
	    tab[i]=1+tab[(i as u32/p[j]) as usize];
	    if tab[i]%2 == 0 {num=num+1}
	    else {num=num-1}
	}
	if num > 0 {last=i}
	if num > 0 && first == 0 {first=i}
	if num > maxi {maxi=num;imax=i}
	if num < mini {mini=num;imin=i}
    }
    println!("first={},imax={},maxi={},imin={},mini={},last={})",
	     first,imax,maxi,imin,mini,last);
}

fn do_it2() {
    let mut tab = Box::new([0 as u8; SIZE]);
    tab[1]=1;tab[2]=0;
    let primes = build_primes();
    
    for p in primes.iter() {
	print!("p={}:",p);
	let mut v = 0;
	loop  {
	    for _i in 0..p-1 {
		v=v+(*p as usize);
		if v>= SIZE {break}
		tab[v] = tab[v]^1;
		print!("{} ",v);
	    }
	    v=v+(*p as usize);
	    if v>=SIZE {break}
	}
	tab[*p as usize]=0;
	println!("");
    }
    println!("{:?}",tab);

//    println!("first={},imax={},maxi={},imin={},mini={},last={})",
//	     first,imax,maxi,imin,mini,last);
}

fn main() {
    do_it2();
}
