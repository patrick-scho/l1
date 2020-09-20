fn add(i1 i2:i64):i64 (
  C (( i1 + i2 ))
)
fn sub(i1 i2:i64):i64 (
  C (( i1 - i2 ))
)
fn print(i:i64):void (
  C (( printf("%lld\n", i) ))
)

fn fib(n:i64):i64 (
  i1 = 1
  i2 = 1
  C ((
    for (int i = 1; i < n; i++) {
      long long i3 = i1 + i2;
      i1 = i2;
      i2 = i3;
    }
  ))
  i1
)

print(fib(100))