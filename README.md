# Optimization-of-switch-statement
We're going to replace switch statement by bit manipulation and measure the impact.

## Function for optimization
Let's say we have function taking number in range [0, 3]. The function is returning 2 dimensional point in a following manner.
```
 f(0) = (0,0)
 f(1) = (1,0)
 f(2) = (1,1)
 f(3) = (0,1)
```
### First solution with switch
A straightforward solution is to use a switch statement with a case for each input. However it can affect execution speed of our program negatively.
```
Give examples
```

### Problem with switch
Before program instructions are executed they need to be loaded and decoded by processor. But a processor doesn't know which branch to choose ahead whenever there is a condition (e.g. an switch, if-else statement) in code. If the wrong branch is chosen then the work of loading and decoding instruction is wasted. Moreover today's processors try to execute instructions speculatively in advance even though they could choose an incorrect branch. Which means choosing a wrong branch can affect execution time a lot more. To prevent that modern processor attempt to predict the right branch based on history of choosing that branch in past.

## Second solution without switch
In order to replace the switch statement, we need to rewrite input of function into bits.

```
1 = 0000 0000  ==>  (0, 0)
2 = 0000 0001  ==>  (1, 0)
3 = 0000 0010  ==>  (1, 1)
4 = 0000 0011  ==>  (0, 1)
```

Immediately we can observe that the second bit of input (from right to left) corresponds to the second coordinate in output. The next not so clear observation is that the first coordinate of output is equal to XOR operation of the first and the second bit of input.

```
0000 0000  ==> first bit=0, second bit=0, XOR(0, 0)=0  ==> (XOR(0, 0), second bit) = (0, 0)
0000 0001  ==> first bit=1, second bit=0, XOR(1, 0)=1  ==> (XOR(1, 0), second bit) = (1, 0)
0000 0010  ==> first bit=0, second bit=1, XOR(0, 1)=1  ==> (XOR(0, 1), second bit) = (1, 1)
0000 0011  ==> first bit=1, second bit=1, XOR(1, 1)=0  ==> (XOR(1, 1), second bit) = (0, 1)
```

The first bit can be extracted with AND operation using 1.
```
AND(0000 0000, 0000 0001) = 0000 0000 = 0
AND(0000 0001, 0000 0001) = 0000 0001 = 1
AND(0000 0010, 0000 0001) = 0000 0001 = 0
AND(0000 0011, 0000 0001) = 0000 0001 = 1
```

The second bit can be extracted with right shift operation >> using 1.
```
>>(0000 0000, 0000 0001) = 0000 0000 = 0
>>(0000 0001, 0000 0001) = 0000 0000 = 0
>>(0000 0010, 0000 0001) = 0000 0001 = 1
>>(0000 0011, 0000 0001) = 0000 0001 = 1
```

Implementation of the function with switch and without switch can be found here.

## Compiler translation into Assembler
Let's see briefly how code will be affected by compiler's optimizations.

### With -O0 (most optimizations are disabled)
![](images/OptimizationLvl0.png)
getPointBranchFree function has 15 assembler instructions.
getPointBranch function has 24 assemlber instructions.

### With -O2 (most optimizations are performed, e.g. inlining small functions)
![](images/OptimizationLvl2.png)
getPointBranchFree function is reduced to 6 assembler instructions.
getPointBranch function is reduced to 15 assembler instructions.

### With -O3 (even more optimizations, e.g. all functions are considered for inlining, even if they are not declared inline)
![](images/OptimizationLvl3.png)
There is no further optimization difference between -O2 and -O3 flag for these individual functions.

## Benchmark

The functions were benchmarked in a following way. We generate 3 types of sequences. Each sequence represents inputs for tested functions. The sequence is generated pseudorandomly by uniform distribution or by repeating a pattern (such as 0, 1, 2, 3, 0, 1, 2 ...) or just simply consist of single number (1, 1, 1, 1...). This approach is selected to examine impact of processor's branch prediction.

![](images/ImpactOfPredictions1.png)
### Observations:
-getPointBranchFree function gives steady performance independently of sequence types. It is due to a fact that BranchFree version doesn't rely on branch predictor.

-getPointBranch function heavily depends on sequence type and does not perform stable performance.

-With -O3 optimization getPointBranch processing repeated number is nearly 5 times faster than processing random numbers.  It is due to a fact that Branch version relies on branch predictor.

-With -O3 optimization getPointBranch can be as fast as getPointBranchFree function. However it only happens in case where the sequence repeats only the same number. In other word function with switch can be as fast as function without switch if processor can perfectly predict branches.


![](images/randomSequence.png)
![](images/sequenceWithPattern.png)
![](images/sequenceWithSameNumber.png)

### Observations:
-At first glance we see huge difference in performance between -O0 and -O2/-O3 flags.
-If minimal optimization (e.g. -O0 flag) is provided, then BranchFree version beats Branch version only in random sequence.-- 	-Even though compiler with -O2 and -O3 produces same assembler code for individual getPointBranchFree and getPointBranch functions, there is still measurable performance gap between -O2 and -O3 flag.

