# asl-calculator
**A math expression parser and evaluator**

This class allows evaluating math expressions expressed as strings, including named variables and standard
functions. It implements the [Shunting-yard_algorithm](https://en.wikipedia.org/wiki/Shunting-yard_algorithm).

```cpp
Calculator calc("-3.5*cos(i*5)+pi*(-2+sqrt(i*pi^2))/3");
double y = calc.set("i", 25.5).compute();
```

Constants `pi` and `e` are predefined, as are the functions `abs`, `sqrt`, `sin`, `cos`, `tan`, `acos`, `asin`, `atan`
`exp`, `floor`, `ln`, `log`.

You can also evaluate directly in one statement:

```cpp
double y = Calculator::eval("-3.5*cos(i*5)+pi*(-2+sqrt(i*pi^2))/3", Dic<double>("i", 25.5));
```

But if you need to evaluate the same expression many times with different variable values, this
will be much slower as it has to parse the expression each time.

Requires the [ASL](https://github.com/aslze/asl) library.


**Benchmark**

Summing `-3.5*cos(i*5)+pi*(-2+sqrt(i*pi^2))/3 for i = 0 .. 10000`, time per each expression evaluation

| Evaluation | Time (μs) | Relative time |
|------------|----------:|--------------:|
| Native     | 0.012     | 1.0           |
| compute()  | 0.219     | 18.5          |
| eval()     | 4.374     | 370.1         |
| python     | 0.760     | 58.4          |
| javascript | 0.050     | 3.8           |

