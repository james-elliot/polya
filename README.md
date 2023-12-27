# Disproves the Polya conjecture

Disproves the [Polya conjecture](https://en.wikipedia.org/wiki/P%C3%B3lya_conjecture) by computing the Liouville
and the summatory Liouville function L(n) up to n=1,000,000,000

The first n such as L(n)>0 is 906150257, the last (in this range) is n=906488079. The maximum is reached
for n=906316571 and L(906316571)=829.

The minimum (in this range) is reached for n=712638284 and L(712638284)=-29736

The Rust program takes around 10s on an i9-9900K. It runs only in release mode
> cargo run --release

The C version in *other/* is parallel and uses shared memory. It should work on any OS with the Posix thread model.
On an i9-9900K it computes up to n=1,000,000,000 in 2s and up to n=100,000,000,000 in 6 minutes (13min17s on a i7-10510U CPU @ 1.80GHz with clang15, 13min41s with gcc12).

For the algorithm see:  

> MATHEMATICS OF COMPUTATION  
> Volume 77, Number 263, July 2008, Pages 1681-1694  
> S 0025-5718(08)02036-X  
> SIGN CHANGES IN SUMS OF THE LIOUVILLE FUNCTION  
> PETER BORWEIN, RON FERGUSON, AND MICHAEL J. MOSSINGHOFF  

