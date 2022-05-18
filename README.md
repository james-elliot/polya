# Disproves the Polya conjecture

Disproves the [Polya conjecture](https://en.wikipedia.org/wiki/P%C3%B3lya_conjecture) by computing the Liouville
and the summatory Liouville function L(n) up to n=1,000,000,000

The first n such as L(n)>0 is 906150257, the last (in this range) is n=906488079. The maximum is reached
for n=906316571 and L(906316571)=829.

The minimum (in this range) is reached for n=712638284 and L(712638284)=-29736

The program takes around 5 minutes on an i9-9900K without parallelism. It is very simple, and could easily be improved.
