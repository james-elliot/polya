# Disproves the Polya conjecture

Disproves the [Polya conjecture](https://en.wikipedia.org/wiki/P%C3%B3lya_conjecture) by computing the Liouville
and the summatory Liouville function L(n) up to n=1,000,000,000

The first n such as L(n)>0 is 906150257, the last (in this range) is n=906488079. The maximum is reached
for n=906316571 and L(906316571)=829.

The minimum (in this range) is reached for n=712638284 and L(712638284)=-29736

The Rust program takes around 10s on an i9-9900K. It runs only in release mode
> cargo run --release

The C version in *other/* is parallel and uses shared memory. It should work on any OS with the Posix thread model.

| Processor / nb threads used |    1 |    2 |   4 |   8 |  16 |  32 |
|-----------------------------|-----:|-----:|----:|----:|----:|----:|
| i9-9900K CPU @ 3.60GHz      | 1489 |  835 | 588 | 476 | 423 | 421 |
| i7-1068NG7 @ 2.30GHz        | 1929 |  968 | 653 | 566 | 566 |     |
| i7-10510U @ 1.80GHz         | 1861 | 1247 | 977 | 792 |     |     |

For the algorithm see:  

> MATHEMATICS OF COMPUTATION  
> Volume 77, Number 263, July 2008, Pages 1681-1694  
> S 0025-5718(08)02036-X  
> SIGN CHANGES IN SUMS OF THE LIOUVILLE FUNCTION  
> PETER BORWEIN, RON FERGUSON, AND MICHAEL J. MOSSINGHOFF  

